
/*
 *  Platz - A platformer framework for the Uzebox (supports VIDEO_MODE 2 & 3)
 *  Copyright (C) 2009 Paul McPhee
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/****************************************
 *			Library Dependencies		*
 ****************************************/
#include <avr/pgmspace.h>
#include <string.h>
#include <uzebox.h>
#include <stdint.h>
#include "platz.h"

/****************************************
 *				Utils					*
 ****************************************/
#define MIN(_x,_y)  ((_x)<(_y) ? (_x) : (_y))
#define MAX(_x,_y)  ((_x)>(_y) ? (_x) : (_y))
#define ABS(_x)		(((_x) > 0) ? (_x) : -(_x))
#define NORMALIZE(x) (((x) > 0) ? 1 : ((x) < 0) ? -1 : 0)
// Faster to prove the negative case
#define PT_NOT_IN_RECT(p,r) (((p).x < ((r).left)) || ((p).x >= ((r).right)) || ((p).y < ((r).top)) || ((p).y >= ((r).btm)))

// Constants
#define TO_PIXELS_Y(y)		((y)<<3)
#define TO_TILES_Y(y)		((y)>>3)
#define MOD_Y(y)			((y)&7)

#if VIDEO_MODE == 2
	#define SCRL_WID		192
	#define TO_PIXELS_X(x)	((x)*6)
	#define TO_TILES_X(x)	((x)/6)
	#define SCRL_X			ss->scrollX
	#define MOD_X(x)		((x)%6)
#elif VIDEO_MODE == 3
	#define SCRL_WID		256
	#define TO_PIXELS_X(x)	((x)<<3)
	#define TO_TILES_X(x)	((x)>>3)
	#define SCRL_X			Screen.scrollX
	#define MOD_X(x)		((x)&7)
#endif

// The number of speeds that should be updated each frame (2 for the player's speeds and the remaining for moving platforms)
#define TICK_COUNT			(2+MAX_VIS_SLICES*MAX_MOVING_PLATFORMS)

/****************************************
 *			Type declarations			*
 ****************************************/

typedef struct platforms {
	u8			slice[MAX_VIS_SLICES];						// World slice pointers
	u8 			count[MAX_VIS_SLICES];						// # of platforms in each slice
	velocity 	v[MAX_VIS_SLICES][MAX_MOVING_PLATFORMS];	// Individual platform velocities
	platform 	p[MAX_VIS_SLICES][MAX_MOVING_PLATFORMS];	// Platform data
} platforms;

typedef struct platformsCommon {
	u8 vtiles;					// Offset to vertical platform tiles
	u8 htiles;					// Offset to horizontal platform tiles
	u8 vstepped;				// Offset to vertical stepped platform tile (only paints square)
	u8 hstepped;				// Offset to horizontal stepped platform tile (only paints square)
} platformsCommon;

typedef struct animatedBgs {
	u8 			count[MAX_VIS_SLICES];
	bgAnimIndex	ids[MAX_VIS_SLICES][MAX_ANIMATED_BGS];
	bgInner 	bgs[MAX_VIS_SLICES][MAX_ANIMATED_BGS];
#ifdef MUTABLE_BGS
	bgInner		mutBgs[MAX_VIS_SLICES][MAX_ANIMATED_BGS];
#endif
	animation 	anims[MAX_VIS_SLICES][MAX_ANIMATED_BGS];
} animatedBgs;

#if MAX_PROJECTILES && MAX_PROJECTILE_CLASSES
// Note: Could have an animation to display based on the projectile and the surface it hits. Could
// also have a tile/map to display at the collision point (like a burn mark for a fireball). Query
// vram array for the tile that was hit (as Outer BG hides it) so we know how to react/which overlay tile to show.

typedef struct projectileClass {
	u8			type;					// Determines how we treat collisions/deflections/animations/etc
	velocity 	vx;
	velocity 	vy;
} projectileClass;

typedef struct projectile {
	pt				loc;
	char			xDir;				// Needed because we re-use projectileClass.vx for many projectiles
	char			yDir;				// Needed because we re-use projectileClass.vy for many projectiles
	projectileClass *pc;
} projectile;

projectileClass projClasses[MAX_PROJECTILE_CLASSES];
projectile projs[MAX_PROJECTILES];
#endif

/****************************************
 *			File-level variables		*
 ****************************************/
trigCallback trigCb;				// Triggerable bg callback function (client-defined logic)
mutCallback mutCb;					// Mutable bg callback function (client-defined logic)
queryCallback queryCb;				// Queryable bg callback function (client-defined logic)
const animation *animBgTbl;			// Animated bgs' location in flash
const bgAnimIndex *bgAnimDir;		// Animated BG directory in flash
const char **bgMaps;				// Background maps in flash (BGP flag)
const object *objTbl;				// Objects' tile maps in flash
const bgInner *bgiTbl;				// Non-collidable decorative bgs in flash
const bgOuter *bgoTbl;				// Collidable bg containers in flash
const bgDirectory *bgDir;			// Bg directory in flash
const platformDirectory *platDir;	// Moving platform headers
const platform *platTbl;			// Moving platforms' attributes
const mutableClass *mcTbl;			// Mutable classes directory to consolidate similar functionality

#if VIDEO_MODE == 2
	struct ScreenSectionStruct *ss;	// Screen section on which Platz will operate
#endif

#if MAX_ANIMATED_BGS
	animatedBgs	animBgs;			// Animated bgs container
#endif

#if MAX_MOVING_PLATFORMS
	platformsCommon cp;				// Common moving platform attributes
	platforms mp;					// Dynamically loaded collection of moving platforms
#endif

u8 wsp;								// World Slice Pointer (points to our current drawing canvas)
u8 wspMax;							// Max value that wsp can validly point to (equal to user's level size-1)
u8 csp;								// Collision Slice Pointer (points to our current collision bounds)
u8 xPrev;							// Playerable character's previous x position
#ifdef VIEWPORT_SLACK
	u8 vpSlack = VIEWPORT_SLACK;	// Viewport slack - offset from center before scrolling begins
#else
	u8 vpSlack = 0;
#endif
u8 vpAnchor;						// Viewport anchor
u8 *ticks[TICK_COUNT];				// Holds pointers to speed frames for updating each game frame
char prevScrDirX;					// The previous scrolling direction
char scrDirX;						// The current scrolling direction
char scrDirAdj;						// Seam drawing directional adjustment
u8 prevScrX;						// The previous scroll position in pixels
u8 scrXMod;							// The current scroll position in tiles

/****************************************
 *		Static function declarations	*
 ****************************************/

#if MAX_MOVING_PLATFORMS
	void PlatzLoadMovingPlatforms(u8 sp, char dir);
	void PlatzMoveMovingPlatforms(void);
	void PlatzDrawMovingPlatforms(u8);
	u8 PlatzDetectMovingPlatformCollisions(platzActor *a, char *xDelta);
#endif

#if MAX_ANIMATED_BGS
	void PlatzLoadAnimatedBgs(u8 sp, char dir);
	void PlatzAnimateBgs(void);
	void PlatzDrawAnimatedBgs(void);
#endif
void PlatzBuildMutableBgFromClass(bgInner *bgi, bgInner *bgm);
u8 DetectBgCollisions(platzActor *a, u8 enTrig);
void PlatzDrawColumn(u8 paintX, char dir);
void PlatzUpdateCollisionPointer(platzActor *a);
void PlatzScroll(char xDelta);
//u8 BinaryFlashSample(int compare, int flashAdj, char dir, u8 count, u8 size, const char *flash);
//u8 LinearFlashSample(int compare, int flashAdj, char dir, u8 count, u8 size, const char *flash);

/****************************************
 *			Function definitions		*
 ****************************************/

#if VIDEO_MODE == 2
// Set screen section on which Platz will operate
void PlatzSetScreenSection(u8 index) {
	ss = &screenSections[index];
}
#endif

// Trigger callback is a user-defined function to handle game logic when triggers fire
void PlatzSetTriggerCallback(trigCallback tcb) {
	trigCb = tcb;
}

// Mutator callback is a user-defined function to handle mutable background events
void PlatzSetMutCallback(mutCallback mcb) {
	mutCb = mcb;
}

void PlatzSetMutClassTable(const mutableClass *mc) {
	mcTbl = mc;
}

// Query callback is a user-defined function to handle queryable background collisions
void PlatzSetQueryCallback(queryCallback qb) {
	queryCb = qb;
}

#if MAX_MOVING_PLATFORMS
// Contains the data that describes each platform's dimensions and movement
void PlatzSetMovingPlatformTable(const platform *p) {
	platTbl = p;
}

// Points slices to their respective platforms
void PlatzSetMovingPlatformDirectory(const platformDirectory *pDir) {
	platDir = pDir;
}
#endif

#if MAX_ANIMATED_BGS
// Contains data for how to animate a bg
void PlatzSetAnimatedBgTable(const animation *a) {
	animBgTbl = a; 
}

void PlatzSetAnimatedBgDirectory(const bgAnimIndex *bgad) {
	bgAnimDir = bgad;
}
#endif

// Points to maps in the bg map table
void PlatzSetMapsTable(const char **m) {
	bgMaps = m;
}

// Contains data for how to display object maps for each slice
void PlatzSetObjectTable(const object *obj) {
	objTbl = obj;
}

// Inner bgs contain display data
void PlatzSetInnerBgTable(const bgInner *bgi) {
	bgiTbl = bgi;
}

// Outer bgs contain collision data and point to their collection of inner bgs
void PlatzSetOuterBgTable(const bgOuter *bgo) {
	bgoTbl = bgo;
}

// A list of where to look for what for each world slice (except platforms, which manage themselves - this may change)
void PlatzSetBgDirectory(const bgDirectory *bgd) {
	bgDir = bgd;
}

void PlatzSetViewport(u8 anchor, u8 slack) {
	vpSlack = slack;
	vpAnchor = anchor;
}

u8 PlatzGetCollisionPointer(void) {
	return csp;
}

u8 PlatzGetWorldSlicePointer(void) {
	return wsp;
}


void PlatzInit(platzActor *a, u8 sliceCount) {
	// Init movement counter
	ticks[0] = &(a->vx.frames);
	ticks[1] = &(a->vy.frames);

#if MAX_MOVING_PLATFORMS
	for (u8 i = 0; i < MAX_VIS_SLICES; i++) {
		for (u8 j = 0; j < MAX_MOVING_PLATFORMS; j++) {
			ticks[2+i*MAX_MOVING_PLATFORMS+j] = &(mp.v[i][j].frames);
		}
	}
#endif

	vpAnchor = a->sprx;
	wspMax = sliceCount-1;
}

#if MAX_MOVING_PLATFORMS
// Initialises global moving platforms structure
void PlatzSetMovingPlatformTiles(u8 hTilesIndex, u8 vTilesIndex, u8 shTilesIndex, u8 svTilesIndex) {
	cp.htiles = hTilesIndex;
	cp.vtiles = vTilesIndex;
	cp.hstepped = shTilesIndex;
	cp.vstepped = svTilesIndex;
}
#endif

inline void PlatzBuildMutableBgFromClass(bgInner *bgi, bgInner *bgm) {
	mutableClass bgmc;
	memcpy_P(&bgmc,mcTbl+bgi->tile,sizeof(mutableClass));
	bgm->type = bgi->r.btm;		// Mutable bitmap index was stored in bgi rect.btm
	bgm->tile = bgmc.id;
	bgi->tile = bgmc.tile;
	bgi->r.btm = bgi->r.top+bgmc.hgt;
	bgm->r = bgi->r;
}

// Moves the actor to slice sp and centers the viewport on their sprite position
void PlatzMoveToSlice(platzActor *a, u8 sp) {
#if !PLATZ_SLIM
	u8 xOffset;
	bgOuter bgo;
	bgInner bgi,bgm;
	object obj;
	const char *objMap;
	bgDirectory bgd;
	rect robj;
#endif
	u8 scrX = (a->loc.x >= vpAnchor)?a->loc.x-vpAnchor:SCRL_WID+(a->loc.x-vpAnchor);
	rect vp = (rect){0,VRAM_TILES_H,0,VRAM_TILES_V};	// Init to squared viewport

    a->sprx = vpAnchor;
    xPrev = a->loc.x;
    prevScrDirX = a->vx.dir;
    scrDirX = a->vx.dir;
    scrDirAdj = (a->vx.dir == DIR_RIGHT)?0:-TILE_WIDTH;
    wsp = csp = sp;

    u8 sp2;

#if MAX_ANIMATED_BGS || MAX_MOVING_PLATFORMS
    char dir;

    if (a->loc.x < vpAnchor) {
        sp2 = (sp)?sp-1:wspMax;
        dir = DIR_LEFT;
    } else {
        sp2 = (sp < wspMax)?sp+1:0;
        dir = DIR_RIGHT;
    }
	#if MAX_ANIMATED_BGS
	    PlatzLoadAnimatedBgs(sp,dir);
	    PlatzLoadAnimatedBgs(sp2,dir);
	#endif

	#if MAX_MOVING_PLATFORMS
	    PlatzLoadMovingPlatforms(sp,dir);
	    PlatzLoadMovingPlatforms(sp2,dir);
	#endif
#else
	if (a->loc.x < vpAnchor)
        sp2 = (sp)?sp-1:wspMax;
    else
        sp2 = (sp < wspMax)?sp+1:0;
#endif

	// Center viewport on actor
	SCRL_X = prevScrX = scrX;
	scrXMod = TO_TILES_X(scrX);

#if PLATZ_SLIM
	u8 spIt = sp;

	for (u8 i = 0; i < MAX_VIS_SLICES; i++) {
		wsp = spIt;

		if (a->loc.x < vpAnchor) {
            if (i) {
                vp.left = scrXMod;
                vp.right = TO_TILES_X(SCRL_WID);
            } else {
                vp.left = 0;
                vp.right = scrXMod;
                spIt = (sp)?sp-1:wspMax;
            }
        } else {
            if (i) {
                vp.left = 0;
                vp.right = scrXMod;
            } else {
                vp.left = scrXMod;
                vp.right =  TO_TILES_X(SCRL_WID);
                spIt = (sp < wspMax)?sp+1:0;
            }
        }

		for (u8 j = vp.left; j < vp.right; j++)
			PlatzDrawColumn(j, a->vx.dir);
	}
	// Reposition world slice pointer
	if (a->loc.x < vpAnchor)
		wsp = (a->vx.dir == DIR_RIGHT)?sp:sp2;
    else
		wsp = (a->vx.dir == DIR_RIGHT)?sp2:sp;
#else
	// Draw sky
	ClearVram();

	for (u8 i = 0; i < MAX_VIS_SLICES; i++) {
		memcpy_P(&bgd,bgDir+sp,sizeof(bgDirectory));

		if (a->loc.x < vpAnchor) {
            if (i) {
                vp.left = scrXMod;
                vp.right = TO_TILES_X(SCRL_WID);
            } else {
                vp.left = 0;
                vp.right = scrXMod;
                sp = (sp)?sp-1:wspMax;
                wsp = (a->vx.dir == DIR_RIGHT)?csp:((csp)?csp-1:wspMax);
            }
        } else {
            if (i) {
                vp.left = 0;
                vp.right = scrXMod;
            } else {
                vp.left = scrXMod;
                vp.right =  TO_TILES_X(SCRL_WID);
                sp = (sp < wspMax)?sp+1:0;
                wsp = (a->vx.dir == DIR_RIGHT)?((csp < wspMax)?csp+1:0):csp;
            }
        }

		// Draw bgs
		for (u8 j = 0; j < bgd.bgoCount; j++) {
			memcpy_P(&bgo,bgoTbl+bgd.bgoIndex+j,sizeof(bgOuter));

			if ((bgo.type&(BGI|BGT)) == 0) {
				for (u8 k = 0; k < bgo.count; k++) {
					if (PlatzRectsIntersect(&bgo.r,&vp)) {
						memcpy_P(&bgi,bgiTbl+bgo.index+k,sizeof(bgInner));

						if ((bgi.type&(BGMC|BGM)) && mutCb) {
							if (bgi.type&BGM) {
								++k;
								memcpy_P(&bgm,bgiTbl+bgo.index+k,sizeof(bgInner));
							} else {
								PlatzBuildMutableBgFromClass(&bgi,&bgm);
							}
							if (mutCb(PLATZ_MUT_EV_DRAW,&bgi,&bgm,&bgo) == 0)
								continue;
						}

						if ((bgi.type&BGA) == 0) {
							xOffset = MAX(bgi.r.left,vp.left)-bgi.r.left;
							bgi.r.left = MAX(bgi.r.left,vp.left);
							bgi.r.right = MIN(bgi.r.right,vp.right);

							if (bgi.r.left >= bgi.r.right)
								continue;
							if (bgi.type&BGP)
								PlatzFillMap(&bgi.r,xOffset,0,(const char*)pgm_read_word(&(bgMaps[bgi.tile])),2);
							else
								PlatzFill(&bgi.r,bgi.tile);
						}
					}
				}
			}		
		}

		// Draw objects
		for (u8 j = 0; j < bgd.objCount; j++) {
			memcpy_P(&obj,objTbl+bgd.objOffset+j,sizeof(object));
			objMap = (const char*)pgm_read_word(&(bgMaps[obj.map]));
			robj.left = obj.begin.x;
			robj.top = obj.begin.y;
			robj.right = robj.left+pgm_read_byte(&(objMap[0]));
			robj.btm = robj.top+pgm_read_byte(&(objMap[1]));
			xOffset = MAX(robj.left,vp.left)-robj.left;

			if (PlatzRectsIntersect(&robj,&vp))
				PlatzFillMap(&robj,xOffset,0,objMap,2);
		}
	}
#endif
}


// Adjusts the player's bounding box size, anchor point and trigger point in the event that their sprite
// dimensions have changed. Will fail safely if the increased size would result in an overlap with a bg
u8 PlatzSetBoundingBoxDimensions(platzActor *a, u8 wid, u8 hgt) {
	platzActor aTemp = *a;

	aTemp.bbx = wid>>1;
	aTemp.bby = hgt>>1;
	char xVel = (a->vx.dir > 0)?a->bbx-aTemp.bbx:aTemp.bbx-a->bbx;
	char yVel = (a->vy.dir > 0)?a->bby-aTemp.bby:aTemp.bby-a->bby;

	xVel <<= 2;
	yVel <<= 2;
	while (ABS(xVel)&3) (xVel>0)?++xVel:--xVel;	// Ensure modDisp and disp are equal so
	while (ABS(yVel)&3) (yVel>0)?++yVel:--yVel; // that GET_VEL will be independent of frames
	PlatzSetVelocity(&aTemp.vx,xVel,&aTemp.trLoc.x);
	PlatzSetVelocity(&aTemp.vy,yVel,&aTemp.trLoc.y);

	if (DetectBgCollisions(&aTemp,0)) {
		// Cannot switch size safely
		return 0;
	}

	xVel >>= 2;
	yVel >>= 2;
	a->loc.x += xVel;
	a->loc.y += yVel;
	a->trLoc.x -= xVel;
	a->trLoc.y -= yVel;
	a->sprx += xVel;
	a->bbx = aTemp.bbx;
	a->bby = aTemp.bby;
	return 1;
}

// Similar to kernel's MapSprite, but no wid/hgt header to read from flash
void PlatzMapSprite(u8 index, u8 wid, u8 hgt, const char *map, u8 spriteFlags) {
	u8 x,y,xStart,xEnd,xStep;

	if (spriteFlags&SPRITE_FLIP_X) {
		xStart = wid-1;
		xEnd = 255;
		xStep = -1;
	} else {
		xStart = 0;
		xEnd = wid;
		xStep = 1;
	}

	for (y = 0; y < hgt; y++) {
		for (x = xStart; x < xEnd; x += xStep,index++) {
			sprites[index].tileIndex = pgm_read_byte(&(map[(y*wid)+x]));
#if VIDEO_MODE == 3
			sprites[index].flags = spriteFlags;
#endif
		}
	}
}

// Moves a sprite array's position offscreen so that it is not drawn
void PlatzHideSprite(u8 spriteIndex, u8 wid, u8 hgt) {
	u8 size = wid*hgt;

	for (int i = 0; i < size; i++)
#if VIDEO_MODE == 2
		MoveSprite(spriteIndex+i,0,-TILE_HEIGHT,1,1);
#elif VIDEO_MODE == 3
		MoveSprite(spriteIndex+i,TO_PIXELS_X(SCREEN_TILES_H),0,1,1);
#endif
}


// Similar to kernel's Fill, but takes a rect
void PlatzFill(const rect *r, u8 tileId) {
#if 1
	u8 x,y;
	
    for (x = r->left; x < r->right; x++) {
        for (y = r->top; y < r->btm; y++) {
            //SetTile(x,y,tileId);
        	inline_set_tile(x,y,tileId);
        }
    }
#else
	u8 x = r->left,wid = r->right-r->left;
	int yLimit = r->btm*VRAM_TILES_H;

	for (int y = r->top*VRAM_TILES_H; y < yLimit; y+=VRAM_TILES_H)
		memset(vram+x+y,tileId+RAM_TILES_COUNT,wid);
#endif
}

// Draws a rectangular region of a map
void PlatzFillMap(const rect *r, u8 xOffset, u8 yOffset, const char *map, int dataOffset) {
#if 1
	u8 x,y,wid = r->right-r->left,hgt = r->btm-r->top,mapWid = pgm_read_byte(&(map[0])),mapHgt = pgm_read_byte(&(map[1]));
	u8 xMap, yMap, yMapMax = mapWid*mapHgt;

	// Don't burden non-repeating maps with possible mod overhead
	if ((xOffset > mapWid) || (yOffset > mapHgt)) {
#ifndef SLOW_BG_PATTERNS
		xOffset &= mapWid-1;
		yOffset &= mapHgt-1;
#else
		xOffset %= mapWid;
		yOffset %= mapHgt;
#endif
	}

	// Do in vertical strips as this is mostly called by PlatzDrawColumn
	for (x = 0, xMap = xOffset; x < wid; x++,xMap++) {
		if (xMap == mapWid)
			xMap = 0;
		for (y = 0, yMap = yOffset*mapWid; y < hgt; y++,yMap+=mapWid) {
			if (yMap == yMapMax)
				yMap = 0;
			inline_set_tile(x+r->left,y+r->top,pgm_read_byte(&(map[xMap+yMap+dataOffset])));
		}
	}
#else
	static u8 mapRam[MAX_MAP_SIZE];
    u8 x,yMap,mapWid = pgm_read_byte(&(map[0])),mapHgt = pgm_read_byte(&(map[1]));

    // Don't burden non-repeating maps with possible mod overhead
    if ((xOffset > mapWid) || (yOffset > mapHgt)) {
#ifndef SLOW_BG_PATTERNS
	    xOffset &= mapWid-1;
	    yOffset &= mapHgt-1;
#else
	    xOffset %= mapWid;
	    yOffset %= mapHgt;
#endif
    }
	
	u8 xLimit = r->right, yMapLimit = mapHgt*mapWid;
	int yLimit = r->btm*VRAM_TILES_H;

	memcpy_P(mapRam,map+dataOffset,yMapLimit);

	for (u8 i = 0; i < yMapLimit; i++)
		mapRam[i] += RAM_TILES_COUNT;

	yMap = yOffset*mapWid;

	for (int y = r->top*VRAM_TILES_H; y < yLimit; y+=VRAM_TILES_H,yMap+=mapWid) {
		if (yMap == yMapLimit)
        	yMap = 0;
		x = r->left;

		if (xOffset) {
			memcpy(vram+x+y,mapRam+yMap+xOffset,MIN(xLimit-x,mapWid-xOffset));
			x += MIN(xLimit-x,mapWid-xOffset);
		}

		for (; x < xLimit; x+=mapWid)
			memcpy(vram+x+y,mapRam+yMap,MIN(xLimit-x,mapWid));
	}
#endif
}

// Draws a column of tiles based on the bgs supplied to platz and paintX. Handles initial frame of animated
// bgs in case their animation timer is not in sync. Columns can consist of normal, patterned or animated bgs
// and objects. Usually used at the scrolling edge
void PlatzDrawColumn(u8 paintX, char dir) {
	u8 i,j,start = 0;
#if MAX_ANIMATED_BGS
	u8 k,iAnimBg;
#endif
	bgOuter bgo;
	bgInner bgi,bgm;
	object obj;
	const char *objMap;
	bgDirectory bgd;
	memcpy_P(&bgd,bgDir+wsp,sizeof(bgDirectory));

	// Paint sky
#if VIDEO_MODE == 2
	PlatzFill(&(rect){paintX,paintX+1,SS_OFFSET_Y,SS_OFFSET_Y+TO_TILES_Y(ss->height)},0);
#elif VIDEO_MODE == 3
	PlatzFill(&(rect){paintX,paintX+1,0,SCREEN_TILES_V-OVERLAY_LINES},0);
#endif

	// Paint bgs
	for (i = start; i < bgd.bgoCount; i++) {
		memcpy_P(&bgo,bgoTbl+bgd.bgoIndex+i,sizeof(bgOuter));

		if ((bgo.type&(BGI|BGT)) == 0) {	// Only draw visible bgs
			if ((bgo.r.left <= paintX) && (bgo.r.right > paintX)) {	// Can skip all inner bgs if outer does not need painting
				for (j = 0; j < bgo.count; j++) {
					memcpy_P(&bgi,bgiTbl+bgo.index+j,sizeof(bgInner));

					if ((bgi.r.left <= paintX) && (bgi.r.right > paintX)) {
						if ((bgi.type&(BGMC|BGM)) && mutCb) {
							if (bgi.type&BGM)
								memcpy_P(&bgm,bgiTbl+bgo.index+j+1,sizeof(bgInner));
							else
								PlatzBuildMutableBgFromClass(&bgi,&bgm);
							if (mutCb(PLATZ_MUT_EV_DRAW,&bgi,&bgm,&bgo) == 0) {
								if (bgi.type&BGM)	// Skip mutable bg payload for BGM bgs
									++j;
								continue;
							}
						}

						if (bgi.type&BGA) {
#if MAX_ANIMATED_BGS
							if (dir == DIR_RIGHT)
								iAnimBg = 1;
							else
								iAnimBg = 0;

							// Inactive mutable bgs will have been culled above after callback
							for (k = 0; k < animBgs.count[iAnimBg]; k++) {
								if ((animBgs.ids[iAnimBg][k].iOuter == i) && (animBgs.ids[iAnimBg][k].iInner == j))
									PlatzFillMap(&(rect){paintX,paintX+1,animBgs.bgs[iAnimBg][k].r.top,animBgs.bgs[iAnimBg][k].r.btm},paintX-animBgs.bgs[iAnimBg][k].r.left,0,
											animBgs.anims[iAnimBg][k].frames,animBgs.anims[iAnimBg][k].currFrame*animBgs.anims[iAnimBg][k].size+2);
							}
#endif
						} else if (bgi.type&BGP) {
							PlatzFillMap(&(rect){paintX,paintX+1,bgi.r.top,bgi.r.btm},paintX-bgi.r.left,0,(const char*)pgm_read_word(&(bgMaps[bgi.tile])),2);
						} else {
							PlatzFill(&(rect){paintX,paintX+1,bgi.r.top,bgi.r.btm},bgi.tile);
						}
					}
					
					if (bgi.type&BGM)	// Skip mutable bg payload for unpainted BGM bgs
						++j;
				}
			}
		}
	}

	// Paint objects
	for (i = 0; i < bgd.objCount; i++) {
		memcpy_P(&obj,objTbl+bgd.objOffset+i,sizeof(object));
		objMap = (const char*)pgm_read_word(&(bgMaps[obj.map]));

		if ((obj.begin.x <= paintX) && ((obj.begin.x+pgm_read_byte(&(objMap[0]))) > paintX)) {
			PlatzFillMap(&(rect){paintX,paintX+1,obj.begin.y,obj.begin.y+pgm_read_byte(&(objMap[1]))},paintX-obj.begin.x,0,objMap,2);
		}
	}

#if MAX_MOVING_PLATFORMS
	u8 mpTile,mpBtm,mpTop;

	// Paint stepped moving platforms
	i = (mp.slice[0] == wsp)?0:1;

	for (j = 0; j < mp.count[i]; j++) {
		if (mp.p[i][j].clrTile&MP_STEPPED) {
			if ((TO_TILES_X(mp.p[i][j].r.left) <= paintX) && (TO_TILES_X(mp.p[i][j].r.right) > paintX)) {
				mpTop = TO_TILES_Y(mp.p[i][j].r.top);
				mpBtm = TO_TILES_Y(mp.p[i][j].r.btm);

				if (mp.p[i][j].axis == AXIS_X) {
					mpTile = cp.hstepped;
				} else {
					mpTile = cp.vstepped;

					// Negative vel can draw one tile off if not multiple of tile height
					if ((mp.v[i][j].dir < 0) && MOD_Y(mp.p[i][j].r.top)) {
						++mpTop;
						++mpBtm;
					}
				}

				PlatzFill(&(rect){paintX,paintX+1,mpTop,mpBtm},mpTile);
			}
		}
	}
#endif
}


// Determines if two rect16's overlap
inline u8 PlatzRectsIntersect16(const rect16 *r1, const rect16 *r2) {
	if ((r1->btm < r2->top) || (r1->right < r2->left) || (r1->left > r2->right) || (r1->top > r2->btm))
		return 0;
	return 1;
}

// Determines if two rect's overlap (8-bit)
inline u8 PlatzRectsIntersect(const rect *r1, const rect *r2) {
	if ((r1->btm < r2->top) || (r1->right < r2->left) || (r1->left > r2->right) || (r1->top > r2->btm))
		return 0;
	return 1;
}

#if MAX_MOVING_PLATFORMS
// Detects collisions between the player and moving platform. Also "sticks" players to moving platforms
// when the players are not otherwise moving.
u8 PlatzDetectMovingPlatformCollisions(platzActor *a, char *xDelta) {
	u8 i,j,retVal = 0;
	char vel;
	char xVel = GET_VEL(a->vx);
	char yVel = GET_VEL(a->vy);
	rect r = {a->loc.x-a->bbx+xVel,a->loc.x+a->bbx+xVel,a->loc.y-a->bby+yVel,a->loc.y+a->bby+yVel};

	if (queryCb) {
		retVal = 1;
		queryCb(0,&retVal);

		if (retVal)
			retVal = 0;
		else
			return retVal;
	}

	for (i = 0; i < MAX_VIS_SLICES; i++) {
		if (mp.slice[i] != csp) {
			continue;
		}

		for (j = 0; j < mp.count[i]; j++) {
			vel = GET_VEL(mp.v[i][j]);

			if (mp.p[i][j].axis == AXIS_X) {
				if (vel) {
					if (PlatzRectsIntersect(&r,&mp.p[i][j].r)) {
						if (a->vx.dir == mp.v[i][j].dir) {
							if (ABS(a->vx.vel) < ABS(mp.v[i][j].vel)) {
								PlatzSetVelocity(&a->vx,mp.v[i][j].vel,&a->trLoc.x);
							}
						} else {
							PlatzSetVelocity(&a->vx,mp.v[i][j].vel,&a->trLoc.x);
						}

						// Determine exact intersect type
						r.right -= xVel;

						if (r.right < mp.p[i][j].r.left) {
							retVal |= L_INTERSECT;
						} else {
							retVal |= R_INTERSECT;
						}
					} else {
						r.btm++;
						if ((xVel == 0) && PlatzRectsIntersect(&r,&mp.p[i][j].r)) {
							// Stick player to platform surface
							*xDelta += vel;
						}
						r.btm--;
					}
				}
			} else {
				if ((r.right >= mp.p[i][j].r.left) && (r.left < mp.p[i][j].r.right)) {
					if ((r.top >= mp.p[i][j].r.top) && (r.top <= mp.p[i][j].r.btm)) {
						// Platform above player
						a->loc.y = mp.p[i][j].r.btm+a->bby+1+ABS(vel);

						if (yVel < vel) {
							PlatzSetVelocity(&a->vy,mp.v[i][j].vel,&a->trLoc.y);
						}
						retVal = B_INTERSECT;
					} else {
						// Platform below player
						if (vel > 0) {
							if ((r.btm <= (mp.p[i][j].r.btm-vel-2)) && (r.btm >= (mp.p[i][j].r.top-vel-2))) {
								a->loc.y = mp.p[i][j].r.top-a->bby-1;

								if (yVel > 0) {
									PlatzSetVelocity(&a->vy,0,&a->trLoc.y);
								}
								retVal = T_INTERSECT;
							}
						} else {
							if ((r.btm <= (mp.p[i][j].r.btm-1)) && (r.btm >= (mp.p[i][j].r.top-1))) {
								a->loc.y = mp.p[i][j].r.top-a->bby-1;

								if (yVel > 0) {
									PlatzSetVelocity(&a->vy,0,&a->trLoc.y);
								}
								retVal = T_INTERSECT;
							}
						}
					}
				}
			}
		}
	}
	return retVal;
}
#endif


#if 0 // Doesn't work in all cases. Need to think of another way to order bgs in flash.
u8 BinaryFlashSample(int compare, int flashAdj, char dir, u8 count, u8 size, const char *flash) {
	u8 it;
	int sample;

	count >>= 1;
	it = count;

	while (count >>= 1) {
		sample = TO_PIXELS_X(pgm_read_byte(flash+((int)it*size)));
		
		if (dir == DIR_RIGHT)	// dir == DIR_RIGHT: compare = rPost.left, sample = rBg.right
			it = (compare > (sample+flashAdj))?it+count:it-count;
		else					// dir == DIR_LEFT: compare = rPost.right, sample = rBg.left
			it = (compare < (sample+flashAdj))?it+count:it-count;
	}

	while (it) {
		sample = TO_PIXELS_X(pgm_read_byte(flash+((int)(it-1)*size)));

		if (dir == DIR_RIGHT) {
			if (compare < (sample+flashAdj))
				--it;
			else
				break;
		} else {
			if (compare > (sample+flashAdj))
				--it;
			else
				break;
		}
	}

	return it;
}

u8 LinearFlashSample(int compare, int flashAdj, char dir, u8 count, u8 size, const char *flash) {
	u8 it = 0;
	int sample;

	while (it != count) {
		sample = TO_PIXELS_X(pgm_read_byte(flash+((int)it*size)));

		if (dir == DIR_RIGHT) {	// dir == DIR_RIGHT: compare = rPost.left, sample = rBg.right
			if (compare <= (sample+flashAdj))
				break;
		} else {				// dir == DIR_LEFT: compare = rPost.right, sample = rBg.left
			if (compare < (sample+flashAdj))
				break;
		}
		++it;
	}
	return it;
}
#endif


// Detects collisions between the player and background elements and adjusts their speed to avoid these collisions.
// Also handles firing event triggers and mutable bg callbacks.
u8 DetectBgCollisions(platzActor *a, u8 enTrig) {
	u8 colVal;			// Immediate collision flags
	u8 retVal = 0;		// Aggregate collision flags
	u8 sp;				// Slice pointer of slice being processed
	u8 start,fin;		// Iteration range
	char step;			// Iteration increment
#if MAX_MOVING_PLATFORMS
	u8 mpsp = 0;	// Moving platforms slice pointer
#endif
	char xVel = GET_VEL(a->vx),yVel = GET_VEL(a->vy); // Actor's velocities
	int xAdjust = 0, xOffset;
	int xDist, yDist; // For calculating collision specifics
	pt16 trigPos; 	// Trigger point
	rect16 rPre,rPost,rBg; // Collision bounds
	bgDirectory bgd;
	bgOuter bgo;

	// Pre-use rPost to save allocating another variable
	rPost.left = (int)(a->loc.y)+(int)yVel;

#if VIDEO_MODE == 2
	if ((rPost.left+a->bby) >= (TO_PIXELS_Y(SS_OFFSET_Y)+ss->height)) {
		// Collision with bottom of screen
		yVel = (TO_PIXELS_Y(SS_OFFSET_Y)+ss->height)-a->bby-a->loc.y-1;
		retVal |= T_INTERSECT;
	} else if ((rPost.left-a->bby) < TO_PIXELS_Y(SS_OFFSET_Y)) {
		// Collision with top of screen
		yVel = -(TO_PIXELS_Y(SS_OFFSET_Y)-(a->loc.y-a->bby));
		retVal |= B_INTERSECT;
	}
#elif VIDEO_MODE == 3
	if ((rPost.left+a->bby) >= (PLATZ_SCRN_HGT-TO_PIXELS_Y(OVERLAY_LINES))) {
		// Collision with bottom of screen
		yVel = PLATZ_SCRN_HGT-(TO_PIXELS_Y(OVERLAY_LINES))-a->bby-a->loc.y-1;
		retVal |= T_INTERSECT;
	} else if ((rPost.left-a->bby) < 0) {
		// Collision with top of screen
		yVel = -(a->loc.y-a->bby);
		retVal |= B_INTERSECT;
	}
#endif
	
	// If csp is the right slice
	if (((a->vx.dir == DIR_RIGHT) && (csp == wsp)) || ((a->vx.dir == DIR_LEFT) && (csp != wsp)))
		xAdjust = SCRL_WID;

	rPre.left = a->loc.x-a->bbx+xAdjust;
	rPre.right = a->loc.x+a->bbx+xAdjust;
	rPre.top = a->loc.y-a->bby;
	rPre.btm = a->loc.y+a->bby;
	rPost.left = rPre.left+xVel;
	rPost.right = rPre.right+xVel;
	rPost.top = rPre.top+yVel;
	rPost.btm = rPre.btm+yVel;
	trigPos = (pt16){rPre.left+a->bbx,rPre.top+a->bby};
	sp = csp;
	
	// i: 0-1 Moving Platforms (when MAX_MOVING_PLATFORMS != 0)
	// i: 2-3 Outer Bgs
	
#if MAX_MOVING_PLATFORMS
	for (u8 i = 0; i < 2*MAX_VIS_SLICES; i++) {
#else
	for (u8 i = 0; i < MAX_VIS_SLICES; i++) {
#endif
		start = 0;
		step = 1;
		xOffset = (((i&1) == 0 && xAdjust) || ((i&1) && !xAdjust))?SCRL_WID:0;

		if (i&1) {	// 1 && 3
#if VIDEO_MODE == 2
			if (xAdjust)
				rBg = (rect16){0,SCRL_WID,TO_PIXELS_Y(SS_OFFSET_Y),TO_PIXELS_Y(SS_OFFSET_Y)+ss->height};
			else
				rBg = (rect16){SCRL_WID,SCRL_WID<<1,TO_PIXELS_Y(SS_OFFSET_Y),TO_PIXELS_Y(SS_OFFSET_Y)+ss->height};
#elif VIDEO_MODE == 3
			if (xAdjust)
				rBg = (rect16){0,SCRL_WID,0,PLATZ_SCRN_HGT-(TO_PIXELS_Y(OVERLAY_LINES))};
			else
				rBg = (rect16){SCRL_WID,SCRL_WID<<1,0,PLATZ_SCRN_HGT-(TO_PIXELS_Y(OVERLAY_LINES))};
#endif
			if (!PlatzRectsIntersect16(&rPost,&rBg))
				continue;
			if (sp != wsp)
				sp = wsp;
			else if (a->vx.dir == DIR_RIGHT)
				sp = (sp)?sp-1:wspMax;
			else
				sp = (sp < wspMax)?sp+1:0;
		} else {
			sp = csp;
		}
#if MAX_MOVING_PLATFORMS
		if (i < 2) {
			if (mp.slice[0] == sp) {
				fin = mp.count[0];
				mpsp = 0;
			} else if (mp.slice[1] == sp) {
				fin = mp.count[1];
				mpsp = 1;
			} else {
				fin = 0;
			}
		} else {
#else
		if (1) {
#endif
			memcpy_P(&bgd, bgDir+sp, sizeof(bgDirectory));

			if (a->vx.dir == DIR_RIGHT) {
				start = 0;
				fin = bgd.bgoCount;
				step = 1;
			} else {
				start = bgd.bgoCount-1;
				fin = 255;
				step = -1;
			}
		}

		for (u8 j = start; j < fin; j+=step) {
			colVal = 0;
#if MAX_MOVING_PLATFORMS
			if (i < 2) {
				bgo.type = 0;
				rBg.left = mp.p[mpsp][j].r.left+xOffset;
				rBg.right = mp.p[mpsp][j].r.right+xOffset;
				rBg.top = mp.p[mpsp][j].r.top;
				rBg.btm = mp.p[mpsp][j].r.btm;
			} else {
#else
			if (1) {
#endif
				memcpy_P(&bgo,bgoTbl+bgd.bgoIndex+j,sizeof(bgOuter));

				if (!(bgo.type&(BGC|BGM|BGT)))
					continue;
				rBg.left = TO_PIXELS_X(bgo.r.left)+xOffset;
				rBg.right = TO_PIXELS_X(bgo.r.right)+xOffset;
				rBg.top = TO_PIXELS_Y(bgo.r.top);
				rBg.btm = TO_PIXELS_Y(bgo.r.btm);
			}

			if (PlatzRectsIntersect16(&rPost,&rBg)) {
				// Triggers
				if (bgo.type&BGT) {	// Leave this on separate line so that a false enTrig hits the continue
					if (enTrig && trigCb && !PT_NOT_IN_RECT(trigPos,rBg)) {
						char trig;	// Trigger orientation

						if ((rBg.btm-rBg.top) < (rBg.right-rBg.left))
							rBg.btm -= (rBg.btm-rBg.top)>>1;
						else
							rBg.right -= (rBg.right-rBg.left)>>1;
						// Unused bgOuter.count in triggers used for firing orientation (not axis)
						if (!PT_NOT_IN_RECT(trigPos,rBg))
							trig = (bgo.count)?-1:1;
						else
							trig = (bgo.count)?1:-1;
						trigCb(bgo.index,bgo.type,trig);
					}
					continue;
				} else {
					// Mutable bgs
					if ((bgo.type&BGM) && mutCb) {
						bgInner bgi,bgm;
						memcpy_P(&bgi,bgiTbl+bgo.index,sizeof(bgInner));

						if (bgi.type&BGM)
							memcpy_P(&bgm,bgiTbl+bgo.index+1,sizeof(bgInner));
						else
							PlatzBuildMutableBgFromClass(&bgi,&bgm);
						if (mutCb(PLATZ_MUT_EV_COLLISION,&bgi,&bgm,&bgo) == 0)
							continue;
					}
					// Collidable bgs
					if (xVel == 0) {
						colVal = H_INTERSECT;
					} else if (yVel == 0) {
						colVal = V_INTERSECT;
					} else {
						xDist = (xVel > 0)?rBg.left-rPre.right:rPre.left-rBg.right;
						yDist = (yVel > 0)?rBg.top-rPre.btm:rPre.top-rBg.btm;

						if (yDist < 0) {
							colVal = V_INTERSECT;
						} else if (xDist < 0) {
							colVal = H_INTERSECT;
						} else {
							colVal = ((xDist-ABS(xVel)) < (yDist-ABS(yVel)))?V_INTERSECT:H_INTERSECT;
						}
					}

					// Query collision callback
					if (queryCb && (bgo.type&BGQ))
						queryCb(&bgo,&colVal);
#if MAX_MOVING_PLATFORMS
					else if (queryCb && (i < 2))
						queryCb(0,&colVal);
#endif

					if (colVal&V_INTERSECT) {
						xVel -= NORMALIZE(xVel)*(MIN(rPost.right,rBg.right)-MAX(rPost.left,rBg.left)+1);
						rPost.left = rPre.left+xVel;
						rPost.right = rPre.right+xVel;

						if ((rPre.right) < rBg.left)
							retVal |= L_INTERSECT;		// Left intersect
						else
							retVal |= R_INTERSECT;		// Right intersect
					} else if (colVal&H_INTERSECT) {
						yVel -= NORMALIZE(yVel)*(MIN(rPost.btm,rBg.btm)-MAX(rPost.top,rBg.top)+1);
						rPost.top = rPre.top+yVel;
						rPost.btm = rPre.btm+yVel;

						if (rPre.btm < rBg.top)
							retVal |= T_INTERSECT;		// Top intersect
						else
							retVal |= B_INTERSECT;		// Btm intersect
					}
				}
			}
		}
	}

	// Commit altered velocities
	if (retVal&V_INTERSECT) {
		PlatzSetVelocity(&a->vx,xVel,&a->trLoc.x);
	}

	if (retVal&H_INTERSECT) {
		PlatzSetVelocity(&a->vy,yVel,&a->trLoc.y);
	}
	return retVal;
}


// 0.25 pixels per frame (ppf) increments - avoids floating point math and lib bloat
void PlatzSetVelocity(velocity *v, char val, u8 *trPos) {
	char dir = v->dir;
	u8 base,temp,vel;

	if (v->vel == 0) {
		v->frames = 0;
	}

	v->vel = val;
	vel = ABS(val);
	base = vel>>2;
	temp = vel&3;
	v->mod = (temp&1) ? 3 : 1;
	v->modDisp = ((temp == 1) || (temp == 2)) ? base + 1 : base;
	v->disp = (temp == 3) ? base + 1 : base;

	if (v->vel < 0) {
		v->dir = -1;
		v->modDisp *= -1;
		v->disp *= -1;
	} else if (v->vel > 0) {
		v->dir = 1;
	} // else 0 - leave it as it was

	// Keep trigger pos relative to actor's location/direction
	if ((trPos) && (v->dir != dir)) {
		*trPos *= -1;
	}
}

#if MAX_MOVING_PLATFORMS

#define PD_INDEX_OFFSET 8	// Byte offset of pdIndex in bgDirectory
// Loads all moving platforms for the visible slices. These currently manage their own pointers, but
// this is expensive - may change to bgDirectory pointing to relevant platforms
void PlatzLoadMovingPlatforms(u8 sp, char dir) {
	u8 i,src,dest,pdIndex;
	char *p = (char*)bgDir;	// Only want pdIndex from bg directory
	platformDirectory pd;

	if (dir == DIR_RIGHT) {
		src = 1;
		dest = 0;
	} else {
		src = 0;
		dest = 1;
	}

	// Preserve platforms that are still visible
	mp.slice[dest] = mp.slice[src];
	mp.count[dest] = mp.count[src];
	memcpy(mp.v[dest],mp.v[src],MAX_MOVING_PLATFORMS*sizeof(velocity));
	memcpy(mp.p[dest],mp.p[src],MAX_MOVING_PLATFORMS*sizeof(platform));
	// Check next slice for platforms
	pdIndex = pgm_read_byte(p+((sp*sizeof(bgDirectory))+PD_INDEX_OFFSET));

	if (pdIndex != PF_ZERO) {
		// Load platforms
		memcpy_P(&pd,platDir+pdIndex,sizeof(platformDirectory));
		mp.count[src] = pd.count;
		mp.slice[src] = sp;
		memcpy_P(&mp.p[src],platTbl+pd.index,pd.count*sizeof(platform));

		for (i = 0; i < pd.count; i++) {
			PlatzSetVelocity(&mp.v[src][i],mp.p[src][i].v,0);
		}
	} else {
		// Mark as no platforms
		mp.count[src] = 0;
		mp.slice[src] = sp;
	}
}

// Updates direction and position of platforms that are in ram
void PlatzMoveMovingPlatforms(void) {
	u8 i,j,*near,*far;
	char vel;
	
	for (i = 0; i < MAX_VIS_SLICES; i++) {
		for (j = 0; j < mp.count[i]; j++) {
			vel = GET_VEL(mp.v[i][j]);

			if (vel) {
				near = (mp.p[i][j].axis == AXIS_X)?&mp.p[i][j].r.left:&mp.p[i][j].r.top;
				far = (mp.p[i][j].axis == AXIS_X)?&mp.p[i][j].r.right:&mp.p[i][j].r.btm;

				if ((*near >= mp.p[i][j].max) || (*near <= mp.p[i][j].min)) {
					mp.v[i][j].vel *= -1;
					PlatzSetVelocity(&mp.v[i][j],mp.v[i][j].vel,0);
					vel = GET_VEL(mp.v[i][j]);
				}
				*near += vel;
				*far += vel;
			}
		}
	}
}


// Relevant to PlatzDrawMovingPlatforms() only
#define FLAG_NEAR 	1
#define FLAG_FAR	2

// Draws all platforms that should be at least partially visible
void PlatzDrawMovingPlatforms(u8 axis) {
	u8 i,j,k,offset,hgt,tile,*near,*far,scrX = TO_TILES_X(SCRL_X),tiles = 0,clear = 0,op = 0,len = 0;
	rect r,rc;

	for (i = 0; i < MAX_VIS_SLICES; i++) {
		for (j = 0; j < mp.count[i]; j++) {
#ifdef MP_HACK
			if (mp.p[i][j].axis != axis)	// Note: This and the need for an axis param are a hack fix to the vertical axis visual off-by-one bug
				continue;
#endif
			r.top = TO_TILES_Y(mp.p[i][j].r.top);
			r.btm = TO_TILES_Y(mp.p[i][j].r.btm);
			r.left = TO_TILES_X(mp.p[i][j].r.left);
			r.right = TO_TILES_X(mp.p[i][j].r.right);
			offset = (mp.p[i][j].axis == AXIS_X)?MOD_X(mp.p[i][j].r.left):MOD_Y(mp.p[i][j].r.top);
			clear = 0;
			op = (offset)?1:0;

			if (op && (mp.p[i][j].clrTile&MP_STEPPED))	// Only draw stepped platforms when square
				continue;

			if (mp.p[i][j].axis == AXIS_X) {
				near = &rc.left;
				far = &rc.right;

				if (i == 0) {	// Left slice
					if (scrX <= (r.right+op)) {
						if (mp.v[i][j].dir == DIR_RIGHT) {
							if (scrX >= r.left) {
								r.left = scrX;
								r.right += op;
							} else {
								if (offset == 0) {
									if (mp.p[i][j].r.left != mp.p[i][j].min) {
										clear = FLAG_NEAR;
									}
								} else {
									r.right++;
								}
							}
						} else {
							if (offset == 0) {
								if (mp.p[i][j].r.left != mp.p[i][j].max) {
									clear = FLAG_FAR;
								}
							} else {
								r.right++;
							}
							
							if (scrX >= r.left) {
								r.left = scrX;
							}
						}
					} else {	// Platform offscreen
						continue;
					}
				} else {	// Right slice
					if (scrX >= r.left) {
						if (mp.v[i][j].dir == DIR_RIGHT) {
							if (offset == 0) {
								if (mp.p[i][j].r.left != mp.p[i][j].min) {
									clear = FLAG_NEAR;
								}
							}

							if (scrX < (r.right+op)) {
								r.right = scrX;
							} else if (offset) {
								r.right++;
							}
						} else {
							if (scrX <= (r.right+op)) {
								r.right = scrX;
							} else {
								if (offset == 0) {
									if (mp.p[i][j].r.left != mp.p[i][j].max) {
										clear = FLAG_FAR;
									}
								} else {
									r.right++;
								}
							}
						}
					} else {	// Platform offscreen
						continue;
					}
				}
			} else {
				near = &rc.top;
				far = &rc.btm;

				if (i == 0) {
					if (scrX <= r.right) {
						r.left = MAX(r.left,scrX);
					} else {
						continue;
					}
				} else {
					if (scrX >= r.left) {
						r.right = MIN(r.right,scrX);
					} else {
						continue;
					}
				}

				if (offset == 0) {
					if ((mp.v[i][j].dir > 0) && (mp.p[i][j].r.top != mp.p[i][j].min)) {
						clear = FLAG_NEAR;
					} else if ((mp.v[i][j].dir < 0) && (mp.p[i][j].r.top != mp.p[i][j].max)) {
						clear = FLAG_FAR;
					}
				} else {
					r.btm++;
				}
			}

			if (clear) {
				rc = r;

				if (clear&FLAG_NEAR) {
					clear = 1;
					*far = *near;
					(*near)--;
				} else {
					clear = 1;
					*near = *far;
					(*far)++;
				}
				PlatzFill(&rc,mp.p[i][j].clrTile&0x3f);
			}

			// Drawing - point to appropriate sides and tiles for this axis
			if (mp.p[i][j].axis == AXIS_X) {
				near = &r.left;
				far = &r.right;
				tiles = cp.htiles;
			} else {
				near = &r.top;
				far = &r.btm;
				tiles = cp.vtiles;
			}

			if (mp.p[i][j].clrTile&MP_STEPPED) {
				tiles = (mp.p[i][j].axis == AXIS_X)?cp.hstepped:cp.vstepped;
			}

			hgt = (*far)-(*near);
			*far = (*near)+1;
			len = TO_TILES_X(mp.p[i][j].r.right)-TO_TILES_X(mp.p[i][j].r.left)+((offset)?1:0);

			for (k = 0; k < hgt; k++) {
				if ((mp.p[i][j].axis == AXIS_X) && (hgt < len)) {
					if (offset == 0) {
						tile = tiles;
					} else if (hgt == 1) {
						if (i == 0) {
							// Right end of platform
							tile = tiles+15+offset-1;
						} else {
							// Left end of platform
							tile = tiles+offset;
						}
					} else {
						if (k == 0) {
							if (i == 0) {
								// Middle of platform
								tile = tiles+8+offset-1;
							} else {
								// Left end of platform
								tile = tiles+offset;
							}
						} else if ((k == (hgt-1)) && (i == 0)) {
							// Right end of platform
							tile = tiles+15+offset-1;
						} else {
							// Middle of platform
							tile = tiles+8+offset-1;
						}
					}
				} else {
					if (offset == 0) {
						tile = tiles;				// Square
					} else if (k == 0) {
						tile = tiles+offset;		// Near end
					} else if (k == (hgt-1)) {
						tile = tiles+15+offset-1;	// Far end
					} else {
						tile = tiles+8+offset-1;	// Middle
					}
				}

				PlatzFill(&r,tile);
				(*near)++;
				(*far)++;
			}
		}	
	}
}
#endif // #if MAX_MOVING_PLATFORMS

#if MAX_ANIMATED_BGS

#define BGO_INDEX_OFFSET 	3	// Byte offset of bgoIndex in bgDirectory
#define AC_INDEX_OFFSET		6	// Byte offset of animCount in bgDirectory
#define AI_INDEX_OFFSET		7	// Byte offset of animIndex in bgDirectory
#define BGI_INDEX_OFFSET	2	// Byte offset of bgInner index in bgOuter

// Loads all animated bgs for the visible slices.
void PlatzLoadAnimatedBgs(u8 sp, char dir) {
	u8 i,j,animIndex,animCount,src,dest;
	char *byteGrab = (char*)bgDir;	// Only want a couple of bytes from bg directory
	u16 bgoIndex,bgiIndex;
	bgAnimIndex bgai;

	if (dir == DIR_RIGHT) {
		src = 1;
		dest = 0;
	} else {
		src = 0;
		dest = 1;
	}

	// Preserve animations that are still visible
	animBgs.count[dest] = animBgs.count[src];
	memcpy(animBgs.ids[dest],animBgs.ids[src],MAX_ANIMATED_BGS*sizeof(bgAnimIndex));
	memcpy(animBgs.bgs[dest],animBgs.bgs[src],MAX_ANIMATED_BGS*sizeof(bgInner));
#ifdef MUTABLE_BGS
	memcpy(animBgs.mutBgs[dest],animBgs.mutBgs[src],MAX_ANIMATED_BGS*sizeof(bgInner));
#endif
	memcpy(animBgs.anims[dest],animBgs.anims[src],MAX_ANIMATED_BGS*sizeof(animation));
	// Check next slice for animations
	animCount = pgm_read_byte(byteGrab+((sp*sizeof(bgDirectory))+AC_INDEX_OFFSET));

	if (animCount) {
		animBgs.count[src] = animCount;
		animIndex = pgm_read_byte(byteGrab+((sp*sizeof(bgDirectory))+AI_INDEX_OFFSET));
		bgoIndex = pgm_read_word(byteGrab+((sp*sizeof(bgDirectory))+BGO_INDEX_OFFSET));

		for (i = 0; i < animCount; i++) {
			memcpy_P(&bgai,bgAnimDir+animIndex+i,sizeof(bgAnimIndex));
			animBgs.ids[src][i] = bgai;
			byteGrab = (char*)(bgoTbl+bgoIndex+bgai.iOuter);
			bgiIndex = pgm_read_word(byteGrab+BGI_INDEX_OFFSET);
			memcpy_P(&animBgs.bgs[src][i],bgiTbl+bgiIndex+bgai.iInner,sizeof(bgInner));
#ifdef MUTABLE_BGS
			if (animBgs.bgs[src][i].type&(BGMC|BGM)) {
				if (animBgs.bgs[src][i].type&BGM)
					memcpy_P(&animBgs.mutBgs[src][i],bgiTbl+bgiIndex+bgai.iInner+1,sizeof(bgInner));
				else
					PlatzBuildMutableBgFromClass(&animBgs.bgs[src][i],&animBgs.mutBgs[src][i]);
			}
#endif
			memcpy_P(&animBgs.anims[src][i],animBgTbl+animBgs.bgs[src][i].tile,sizeof(animation));

			if (animBgs.anims[src][i].synch) {
				for (j = 0; j < animBgs.count[dest]; j++) {
					if (animBgs.anims[dest][j].synch && (animBgs.anims[src][i].frames == animBgs.anims[dest][j].frames)) {
						animBgs.anims[src][i].currFrame = animBgs.anims[dest][j].currFrame;
						animBgs.anims[src][i].disp = animBgs.anims[dest][j].disp;
					}
				}
			}
		}
	} else {
		animBgs.count[src] = 0;
	}
}

// Draws new frames for animated bgs that are visible. Handles partial visibility
// for patterned and tiled animations.
void PlatzDrawAnimatedBgs(void) {
	u8 i,j,patOffsetX;
	rect r;
	bool redraw;
	u8 scrX = TO_TILES_X(SCRL_X);

	for (i = 0; i < MAX_VIS_SLICES; i++) {
		for (j = 0; j < animBgs.count[i]; j++) {
#ifdef MUTABLE_BGS
			if ((animBgs.bgs[i][j].type&(BGMC|BGM)) && mutCb)
				if (mutCb(PLATZ_MUT_EV_ANIM,&animBgs.bgs[i][j],&animBgs.mutBgs[i][j],&animBgs.anims[i][j]) == 0)
					continue;
#endif
			if (animBgs.anims[i][j].disp == 0) {
				redraw = false;
				r = animBgs.bgs[i][j].r;

				if (i) {
					// Right slice
					if (scrX > r.left) {
						patOffsetX = 0;
						r.right = MIN(r.right,scrX);
						redraw = true;
					}
				} else {
					// Left slice
					if (scrX < r.right) {
#ifndef SLOW_BG_PATTERNS
						patOffsetX = (scrX > r.left)?(scrX-r.left)&(animBgs.anims[i][j].wid-1):0;
#else
						patOffsetX = (scrX > r.left)?(scrX-r.left)%(animBgs.anims[i][j].wid):0;
#endif
						r.left = MAX(r.left,scrX);
						redraw = true;
					}
				}
			
				// Draw new frame
				if (redraw) {
					PlatzFillMap(&r,patOffsetX,0,animBgs.anims[i][j].frames,animBgs.anims[i][j].currFrame*animBgs.anims[i][j].size+2);
				}
			}
		}
	}
}

// Increments frames for animated bgs that are in ram
void PlatzAnimateBgs(void) {
	u8 i,j;

	for (i = 0; i < MAX_VIS_SLICES; i++) {
		for (j = 0; j < animBgs.count[i]; j++) {
			if (++(animBgs.anims[i][j].disp) == animBgs.anims[i][j].dpf) {
				// Advance animation frames
				animBgs.anims[i][j].disp = 0;
				animBgs.anims[i][j].currFrame++;

				if (animBgs.anims[i][j].currFrame == animBgs.anims[i][j].frameCount) {
					animBgs.anims[i][j].currFrame = 0;
				}
			}
		}
	}
}
#endif // #if MAX_ANIMATED_BGS

// Maintains collision pointer as player moves across slice seams
void PlatzUpdateCollisionPointer(platzActor *a) {
	// Update collision slice pointer
	if (TO_TILES_X(a->loc.x) == 0) {
		if (TO_TILES_X(xPrev) == 31) {
			csp = (csp < wspMax)?csp+1:0;
		}
	} else if (TO_TILES_X(a->loc.x) == 31) {
		if (TO_TILES_X(xPrev) == 0) {
			csp = (csp)?csp-1:wspMax;
		}
	}
	xPrev = a->loc.x;
}

// Manages drawing on the scrolling edge
void PlatzScroll(char xDelta) {
        // Update world slice pointer
    u8 scrX = SCRL_X;

    if (scrX != prevScrX) {
        prevScrX = scrX;

        // Direction change
        if (scrDirX != prevScrDirX) {
            prevScrDirX = scrDirX;

            if (scrDirX == DIR_RIGHT) {
                wsp = (wsp < wspMax)?wsp+1:0;
                scrDirAdj = 0;
            } else if (scrDirX == DIR_LEFT) {
                wsp = (wsp)?wsp-1:wspMax;
                scrDirAdj = -TILE_WIDTH;
            }
        }

        // Tile column scroll boundary reached
        if (TO_TILES_X(scrX) != scrXMod) {
            scrXMod = TO_TILES_X(scrX);

            if ((scrXMod == 31) && (scrDirX == DIR_LEFT)) {
                wsp = (wsp)?wsp-1:wspMax;
#if MAX_ANIMATED_BGS
                PlatzLoadAnimatedBgs(wsp,DIR_LEFT);
#endif

#if MAX_MOVING_PLATFORMS
                PlatzLoadMovingPlatforms(wsp,DIR_LEFT);
#endif
            }

#if VIDEO_MODE == 2
			u8 temp = scrX-xDelta+scrDirAdj;

			if (temp > X_SCROLL_WRAP)
				PlatzDrawColumn(TO_TILES_X((u8)(temp-(256-X_SCROLL_WRAP))),scrDirX);
			else
            	PlatzDrawColumn(TO_TILES_X((u8)(scrX-xDelta+scrDirAdj)),scrDirX);
#elif VIDEO_MODE == 3
			PlatzDrawColumn(TO_TILES_X((u8)(scrX-xDelta+scrDirAdj)),scrDirX);
#endif

            if ((scrXMod == 0) && (scrDirX == DIR_RIGHT)) {
            	wsp = (wsp < wspMax)?wsp+1:0;
#if MAX_ANIMATED_BGS
              	PlatzLoadAnimatedBgs(wsp,DIR_RIGHT);
#endif

#if MAX_MOVING_PLATFORMS
                PlatzLoadMovingPlatforms(wsp,DIR_RIGHT);
#endif
            }
        }
    }

#if MAX_MOVING_PLATFORMS
    PlatzDrawMovingPlatforms(AXIS_X);
#endif

#if MAX_ANIMATED_BGS
    PlatzDrawAnimatedBgs();
    PlatzAnimateBgs();
#endif
}

// When called each frame by Platz clients, will route content management,
// collision, drawing and scrolling processing
u8 PlatzMove(platzActor *a) {
	u8 collFlag = 0;
	char xVel = GET_VEL(a->vx),yVel = GET_VEL(a->vy),xDelta = 0;

	collFlag |= DetectBgCollisions(a,1);

#ifdef MP_HACK
	#if MAX_MOVING_PLATFORMS
		PlatzDrawMovingPlatforms(AXIS_Y);
	#endif
#endif

#if MAX_MOVING_PLATFORMS
	PlatzMoveMovingPlatforms();	// Have to do in here to ensure safe movement
	collFlag |= PlatzDetectMovingPlatformCollisions(a,&xDelta);
#endif

	// Update player and camera positions
	xVel = GET_VEL(a->vx);
	yVel = GET_VEL(a->vy);

	if (xVel) {
		xDelta = xVel;
	}
	
	if (xDelta) {
		a->loc.x += xDelta;
		scrDirX = (xDelta > 0)?DIR_RIGHT:DIR_LEFT;
		
		if ((xDelta > 0) && ((a->sprx+xDelta) < (vpAnchor+vpSlack))) {
			a->sprx += xDelta;
		} else if ((xDelta < 0) && ((a->sprx+xDelta) > (vpAnchor-vpSlack))) {
			a->sprx += xDelta;
		} else {
			SCRL_X += xDelta;
#if VIDEO_MODE == 2
			if (SCRL_X >= X_SCROLL_WRAP) {
				if (SCRL_X < (X_SCROLL_WRAP+6))
					SCRL_X -= X_SCROLL_WRAP;
				else
					SCRL_X -= 256-X_SCROLL_WRAP;
			}
#endif
		}
	} else {
		scrDirX = DIR_NONE;
	}

	if (yVel) {
		a->loc.y += yVel;
	}

#if VIDEO_MODE == 2
	if (a->loc.x >= X_SCROLL_WRAP) {
		if (a->loc.x < (X_SCROLL_WRAP+6))
			a->loc.x -= X_SCROLL_WRAP;
		else
			a->loc.x -= 256-X_SCROLL_WRAP;
	}
#endif

	PlatzUpdateCollisionPointer(a);
	PlatzScroll(xDelta);
	return collFlag;
}

// Progresses movement counters
void PlatzTick(void) {
	for (u8 i = 0; i < TICK_COUNT; i++) {
		*ticks[i] += 1;
	}
}

//////////////Projectiles are WIP///////////////////

#if MAX_PROJECTILES&MAX_PROJECTILE_CLASSES
u8 projClassCount;
u8 projCount;

// There's no RemoveProjectile function because it takes up more flash; makes it more difficult to use;
// and will probably not be used much anyway
char PlatzAddProjectileClass(u8 type, u8 vx, u8 vy) {
	if (projClassCount < MAX_PROJECTILE_CLASSES) {
		projClass[projClassCount].type = type;
		PlatzSetVelocity(&projClass[projClassCount].vx,vx,0);
		PlatzSetVelocity(&projClass[projClassCount].vy,vy,0);
		++projClassCount;
		return 1;
	}
	return 0;
}


char PlatzCreateProjectile(pt loc, char xDir, char yDir, u8 pClass) {
	if ((projCount < MAX_PROJECTILES) && (pClass < projClassCount)) {
		for (u8 i = 0; i < MAX_PROJECTILES; i++) {
			if (projs[i].pc == 0) {
				projs[i].loc = loc;
				projs[i].xDir = xDir;
				projs[i].yDir = yDir;
				projs[i].pc = &projClass[pClass];
				++projCount;
				return 1;
			}
		}
	}
	return 0;
}


void PlatzDestroyProjectile(projectile *p) {
	if (p->pc) {
		p->pc = 0;
		--projCount;
	}
}


void PlatzMoveProjectiles(void) {
	if (projCount) {	// Most of the time, there will be none
		for (u8 i = 0; i < MAX_PROJECTILES; i++) {
			if (projs[i].pc) {
				projs[i].loc.x += projs[i].xDir*ABS(GET_VEL(projs[i].pc->vx));
				projs[i].loc.y += projs[i].yDir*ABS(GET_VEL(projs[i].pc->vy));


			}
		}
	}
}
#endif







