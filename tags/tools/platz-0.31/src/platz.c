
/*
 *  Platz - A platformer framework for the Uzebox (supports VIDEO_MODE 3)
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
#define SCRL_WID			256
#define SCRL_WID_SHFT		8		// 2^8 = 256
#define TICK_COUNT			6		// The number of speeds that should be updated each frame


/****************************************
 *			Type declarations			*
 ****************************************/

typedef struct platforms {
	u8			slice[2];									// World slice pointers
	u8 			count[2];									// # of platforms in each slice
	velocity 	v[MAX_VIS_SLICES][MAX_MOVING_PLATFORMS];	// Individual platform velocities
	platform 	p[MAX_VIS_SLICES][MAX_MOVING_PLATFORMS];	// Platform data
} platforms;

typedef struct platformsCommon {
	u8 vtiles;					// Offset to vertical platform tiles
	u8 htiles;					// Offset to horizontal platform tiles
	u8 clrTile;					// How to paint vacated tiles
} platformsCommon;

typedef struct animatedBgs {
	u8 			count[MAX_VIS_SLICES];
	bgInner 	bgs[MAX_VIS_SLICES][MAX_ANIMATED_BGS];
	animation 	anims[MAX_VIS_SLICES][MAX_ANIMATED_BGS];
} animatedBgs;

/****************************************
 *			File-level variables		*
 ****************************************/
trigCallback trigCb;				// Triggerable bg callback function (client-defined logic)
const animation *animBgTbl;			// Animated bgs' location in flash
const char *patTbl;					// Patterned bgs' tiles in flash
const patternDirectory *patDir;		// Patterned bgs' repeatable tile maps in flash
const object *objTbl;				// Objects' tile maps in flash
const bgInner *bgiTbl;				// Non-collidable decorative bgs in flash
const bgOuter *bgoTbl;				// Collidable bg containers in flash
const bgDirectory *bgDir;			// Bg directory in flash
const platformDirectory *platDir;	// Moving platform headers
const platform *platTbl;			// Moving platforms' attributes
#if MAX_ANIMATED_BGS
	animatedBgs	animBgs;			// Animated bgs container
#endif

#if MAX_MOVING_PLATFORMS
	platformsCommon cp;				// Common moving platform attributes
	platforms mp;					// Dynamically loaded collection of moving platforms
#endif

u8 xFrames;							// Playerable character's X-movement counter
u8 yFrames;							// Playerable character's Y-movement counter
u8 wsp;								// World Slice Pointer (points to our current drawing canvas)
u8 wspMax;							// Max value that wsp can validly point to (equal to user's level size-1)
u8 csp;								// Collision Slice Pointer (points to our current collision bounds)
u8 xPrev;							// Playerable character's previous x position
u8 vpSlack;							// Viewport slack - offset from center before scrolling begins
u8 vpAnchor;						// Viewport anchor
u8 *ticks[TICK_COUNT];				// Holds pointers to speed frames for updating each game frame
char prevScrDirX;					// The previous scrolling direction
char scrDirX;						// The current scrolling direction
char scrDirAdj;						// Seam drawing directional adjustment
u8 prevScrX;						// The previous scroll position in pixels
u8 scrXMod;							// The current scroll position in tiles


/****************************************
 *		Static function prototypes		*
 ****************************************/

#if MAX_MOVING_PLATFORMS
	void PlatzLoadMovingPlatforms(u8 sp, char dir);
	void PlatzMoveMovingPlatforms(void);
	void PlatzDrawMovingPlatforms(void);
#endif

#if MAX_ANIMATED_BGS
	void PlatzLoadAnimatedBgs(u8 sp, char dir);
	void PlatzAnimateBgs(void);
	void PlatzDrawAnimatedBgs(void);
#endif
u8 PlatzDetectMovingPlatformCollisions(platzActor *a, char *xDelta);
u8 DetectBgCollisions(platzActor *a, u8 enTrig);
void PlatzDrawColumn(u8 paintX, char dir);
void PlatzUpdateCollisionPointer(platzActor *a);
void PlatzScroll(char xDelta);
void PlatzFillPattern(const rect *r, u8 patWid, u8 patHgt, u8 patIndex, u8 patOffsetX, const char *map);
void PlatzFill(const rect *r, u8 tileId);

/****************************************
 *			Function definitions		*
 ****************************************/

// Trigger callback is a user-defined function to handle game logic when triggers fire
void PlatzSetTriggerCallback(trigCallback tcb) {
	trigCb = tcb;
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
#endif

// Describes how to fill a bg in a patterned manner
void PlatzSetPatternTable(const char *p) {
	patTbl = p;
}

// Points to patterns in the pattern table
void PlatzSetPatternDirectory(const patternDirectory *pDir) {
	patDir = pDir;
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
// Initialises global moving platforms structure - FIXME: allow each platform its own speed (only 1 byte/platform)
void PlatzSetMovingPlatformTiles(u8 hTilesIndex, u8 vTilesIndex, u8 clrTile) {
	cp.htiles = hTilesIndex;
	cp.vtiles = vTilesIndex;
	cp.clrTile = clrTile;
}
#endif

// Moves the actor to slice sp and centers the viewport on their sprite position
void PlatzMoveToSlice(platzActor *a, u8 sp) {
	u8 i,j,k,l,wid,hgt,objBegin,objEnd,sp2,scrX = a->loc.x-vpAnchor;
	char dir;
	bgOuter bgo;
	bgInner bgi;
	object obj;
	patternDirectory tp;
	bgDirectory bgd;
	rect vp = (rect){0,32,0,28};	// Init to squared viewport
	rect robj;

	a->sprx = vpAnchor;
	xPrev = a->loc.x;
	prevScrDirX = a->vx.dir;
	scrDirX = a->vx.dir;
	scrDirAdj = (a->vx.dir == DIR_RIGHT)?0:-TILE_WIDTH;
	wsp = csp = sp;

#if MAX_ANIMATED_BGS|MAX_MOVING_PLATFORMS
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
#endif

	// Center viewport on actor
	Screen.scrollX = prevScrX = scrX;
	scrXMod = scrX>>3;
	// Draw sky
	ClearVram();

	for (i = 0; i < MAX_VIS_SLICES; i++) {
		memcpy_P(&bgd,bgDir+sp,sizeof(bgDirectory));

		if (a->loc.x < vpAnchor) {
			if (i) {
				vp.left = scrX>>3;
				vp.right = SCRL_WID>>3;
			} else {
				vp.left = 0;
				vp.right = scrX>>3;
				sp = (sp)?sp-1:wspMax;
				wsp = (a->vx.dir == DIR_RIGHT)?csp:((csp)?csp-1:wspMax);
			}
		} else {
			if (i) {
				vp.left = 0;
				vp.right = scrX>>3;
			} else {
				vp.left = scrX>>3;
				vp.right = SCRL_WID>>3;
				sp = (sp < wspMax)?sp+1:0;
				wsp = (a->vx.dir == DIR_RIGHT)?((csp < wspMax)?csp+1:0):csp;
			}
		}

		// Draw bgs
		for (j = 0; j < bgd.bgoCount; j++) {
			memcpy_P(&bgo,bgoTbl+bgd.bgoIndex+j,sizeof(bgOuter));

			if ((bgo.type&BGI) == 0) {
				for (k = 0; k < bgo.count; k++) {
					if (PlatzRectsIntersect(&bgo.r,&vp)) {
						memcpy_P(&bgi,bgiTbl+bgo.index+k,sizeof(bgInner));

						if ((bgi.type&BGA) == 0) {
							bgi.r.left = MAX(bgi.r.left,vp.left);
							bgi.r.right = MIN(bgi.r.right,vp.right);

							if (bgi.type&BGP) {
								memcpy_P(&tp, patDir+bgi.tile,sizeof(patternDirectory));
#ifndef SLOW_BG_PATTERNS
								PlatzFillPattern(&bgi.r,tp.wid,tp.hgt,tp.index,bgi.r.left&((tp.wid<<3)-1),patTbl);
#else
								PlatzFillPattern(&bgi.r,tp.wid,tp.hgt,tp.index,bgi.r.left%(tp.wid<<3),patTbl);
#endif
							} else {
								PlatzFill(&bgi.r,bgi.tile);
							}
						}
					}
				}
			}		
		}

		// Draw objects
		for (j = 0; j < bgd.objCount; j++) {
			memcpy_P(&obj,objTbl+bgd.objOffset+j,sizeof(object));
			wid = pgm_read_byte(&(obj.map[0]));
			hgt = pgm_read_byte(&(obj.map[1]));
			robj.left = obj.begin.x;
			robj.top = obj.begin.y;
			robj.right = robj.left+wid;
			robj.btm = robj.top+hgt;

			if (PlatzRectsIntersect(&robj,&vp)) {
				objBegin = MAX(robj.left,vp.left)-robj.left;
				objEnd = MIN(robj.right,vp.right)-robj.left;
				
				for (k = 0; k < hgt; k++) {
					for (l = objBegin; l < objEnd; l++) {
						SetTile(robj.left+l,robj.top+k,pgm_read_byte(&(obj.map[k*wid+l+2])));
					}
				}
			}
		}
	}
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
void PlatzMapSprite(u8 index, u8 wid, u8 hgt, const char *map) {
	u8 x,y;

	for (y = 0; y < hgt; y++) {
		for (x = 0; x < wid; x++) {
			sprites[index++].tileIndex = pgm_read_byte(&(map[(y*wid)+x]));
		}
	}
}

// Moves a sprite array's position offscreen so that it is not drawn
void PlatzHideSprite(u8 spriteIndex, u8 wid, u8 hgt) {
	MoveSprite(spriteIndex,SCREEN_TILES_H<<3,SCREEN_TILES_V<<3,wid,hgt);
}

// Similar to kernel's Fill, but takes a rect
void PlatzFill(const rect *r, u8 tileId) {
	u8 x,y;

	for (y = r->top; y < r->btm; y++) {
		for (x = r->left; x < r->right; x++) {		
			SetTile(x,y,tileId);
		}
	}
}


// Fills a region with a specified pattern. Pattern can be 2-dimensional. Handles partial draws (i.e. when scrolling).
// SLOW_BG_PATTERNS (non 2^n) is MUCH slower (especially for large fill areas)
void PlatzFillPattern(const rect *r, u8 patWid, u8 patHgt, u8 patIndex, u8 patOffsetX, const char *map) {
	u8 x,y,fillWid = r->right-r->left,fillHgt = r->btm-r->top;

	for (y = 0; y < fillHgt; y++) {
		for (x = 0; x < fillWid; x++) {
#ifndef SLOW_BG_PATTERNS
			SetTile(x+r->left,y+r->top,pgm_read_byte(&(map[patIndex+((x+patOffsetX)&(patWid-1))+((y&(patHgt-1))*patWid)])));
#else
			SetTile(x+r->left,y+r->top,pgm_read_byte(&(map[patIndex+((x+patOffsetX)%patWid)+((y%patHgt)*patWid)])));
#endif
		}
	}
}

// Draws a column of tiles based on the bgs supplied to platz and paintX. Handles initial frame of animated
// bgs in case their animation timer is not in sync. Columns can consist of normal, patterned or animated bgs
// and objects. Usually used at the scrolling edge
void PlatzDrawColumn(u8 paintX, char dir) {
	u8 i,j;
#if MAX_ANIMATED_BGS
	u8 iAnimBg;
#endif
	bgOuter bgo;
	bgInner bgi;
	object obj;
	patternDirectory tp;
	bgDirectory bgd;
	memcpy_P(&bgd,bgDir+wsp,sizeof(bgDirectory));

	// Paint sky
	PlatzFill(&(rect){paintX,paintX+1,0,SCREEN_TILES_V},0);

	// Paint bgs
	for (i = 0; i < bgd.bgoCount; i++) {
		memcpy_P(&bgo,bgoTbl+bgd.bgoIndex+i,sizeof(bgOuter));

		if ((bgo.type&BGI) == 0) {	// Only draw visible bgs
			if ((bgo.r.left <= paintX) && (bgo.r.right > paintX)) {	// Can skip all inner bgs if outer does not need painting
				for (j = 0; j < bgo.count; j++) {
					memcpy_P(&bgi,bgiTbl+bgo.index+j,sizeof(bgInner));

					if ((bgi.r.left <= paintX) && (bgi.r.right > paintX)) {
						if (bgi.type&BGA) {
#if MAX_ANIMATED_BGS
							if (dir == DIR_RIGHT) {
								iAnimBg = 1;
							} else {
								iAnimBg = 0;
							}

							// Paint initial frame of animated bgs - ensures they appear even if new frame is not ready for display
							PlatzFillPattern(&(rect){paintX,paintX+1,animBgs.bgs[iAnimBg][j].r.top,animBgs.bgs[iAnimBg][j].r.btm},animBgs.anims[iAnimBg][j].wid,animBgs.anims[iAnimBg][j].hgt,0,
								(paintX-animBgs.bgs[iAnimBg][j].r.left)&(animBgs.anims[iAnimBg][j].wid-1),animBgs.anims[iAnimBg][j].frames+animBgs.anims[iAnimBg][j].currFrame*animBgs.anims[iAnimBg][j].size);
#endif
						} else if (bgi.type&BGP) {
							memcpy_P(&tp,patDir+bgi.tile,sizeof(patternDirectory));
#ifndef SLOW_BG_PATTERNS
							PlatzFillPattern(&(rect){paintX,paintX+1,bgi.r.top,bgi.r.btm},tp.wid,tp.hgt,tp.index,
								(paintX - bgi.r.left)&(pgm_read_byte(&(patDir[bgi.tile]))-1),patTbl);
#else					
							PlatzFillPattern(&(rect){paintX,paintX+1,bgi.r.top,bgi.r.btm},tp.wid,tp.hgt,tp.index,
								(paintX - bgi.r.left)%pgm_read_byte(&(patDir[bgi.tile])),patTbl);
#endif
						} else {
							PlatzFill(&(rect){paintX,paintX+1,bgi.r.top,bgi.r.btm},bgi.tile);
						}
					}
				}
			}
		}
	}

	u8 wid, hgt;
	// Paint objects
	for (i = 0; i < bgd.objCount; i++) {
		memcpy_P(&obj,objTbl+bgd.objOffset+i,sizeof(object));
		wid = pgm_read_byte(&(obj.map[0]));
		hgt = pgm_read_byte(&(obj.map[1]));		

		if ((obj.begin.x <= paintX) && ((obj.begin.x + wid) > paintX)) {
			for (j = 0; j < hgt; j++) {
				SetTile(paintX, obj.begin.y + j, 
					pgm_read_byte(&(obj.map[(paintX - obj.begin.x) + j*wid+2])));
			}						
		}
	}
}


// Source: Sedgewick
// PlatzLinesIntersect helper
char PlatzCcw(const pt16 *p0, const pt16 *p1, const pt16 *p2) {
    long int dx1, dx2, dy1, dy2;
    dx1 = p1->x-p0->x; dy1 = p1->y-p0->y;
    dx2 = p2->x-p0->x; dy2 = p2->y-p0->y;

    if (dx1*dy2 > dy1*dx2) return 1;
    if (dx1*dy2 < dy1*dx2) return -1;
    if ((dx1*dx2 < 0) || (dy1*dy2 < 0)) return -1;
    if ((dx1*dx1+dy1*dy1) < (dx2*dx2+dy2*dy2)) return 1;
    return 0;
}


// Source: Sedgewick
// Determines if two line regions intersect
u8 PlatzLinesIntersect(const line16 *l1, const line16 *l2) {
	return ((PlatzCcw(&l1->p1, &l1->p2, &l2->p1)*PlatzCcw(&l1->p1, &l1->p2, &l2->p2)) <= 0)
        && ((PlatzCcw(&l2->p1, &l2->p2, &l1->p1)*PlatzCcw(&l2->p1, &l2->p2, &l1->p2)) <= 0);
}

// Determines if two rect16 overlap
inline u8 PlatzRectsIntersect16(const rect16 *r1, const rect16 *r2) {
	if (r1->btm < r2->top) return 0;
	if (r1->right < r2->left) return 0;
	if (r1->left > r2->right) return 0;
	if (r1->top > r2->btm) return 0;
	return 1;
}

// Determines if two rect's overlap (8-bit)
inline u8 PlatzRectsIntersect(const rect *r1, const rect *r2) {
	if (r1->btm < r2->top) return 0;
	if (r1->right < r2->left) return 0;
	if (r1->left > r2->right) return 0;
	if (r1->top > r2->btm) return 0;
	return 1;
}

#if MAX_MOVING_PLATFORMS
// Detects collisions between the player and moving platform. Also "sticks" players to moving platforms
// when they are not otherwise moving.
u8 PlatzDetectMovingPlatformCollisions(platzActor *a, char *xDelta) {
	u8 i,j,retVal = 0;
	char vel;
	char xVel = GET_VEL(a->vx);
	char yVel = GET_VEL(a->vy);
	rect r = {a->loc.x-a->bbx+xVel,a->loc.x+a->bbx+xVel,a->loc.y-a->bby+yVel,a->loc.y+a->bby+yVel};

	for (i = 0; i < 2; i++) {
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
						a->loc.y = mp.p[i][j].r.btm+8+1+ABS(vel);

						if (yVel < vel) {
							PlatzSetVelocity(&a->vy,mp.v[i][j].vel,&a->trLoc.y);
						}
						retVal = B_INTERSECT;
					} else {
						// Platform below player
						if (vel > 0) {
							if ((r.btm <= (mp.p[i][j].r.btm-vel-2)) && (r.btm >= (mp.p[i][j].r.top-vel-2))) {
								a->loc.y = mp.p[i][j].r.top-8-1;

								if (yVel > 0) {
									PlatzSetVelocity(&a->vy,0,&a->trLoc.y);
								}
								retVal = T_INTERSECT;
							}
						} else {
							if ((r.btm <= (mp.p[i][j].r.btm-1)) && (r.btm >= (mp.p[i][j].r.top-1))) {
								a->loc.y = mp.p[i][j].r.top-8-1;

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

// Relevant to DetectBgCollisions() only
#define DBGC_MODE_PLATS		0
#define DBGC_MODE_NO_SEAM	1
#define DBGC_MODE_SEAM_CURR	2
#define DBGC_MODE_SEAM_NEXT	3

// Detects collisions between the player and background elements and adjusts their speed to avoid these collisions.
// Also handles firing event triggers. Could be neater, but benefits from minimal function call overhead
u8 DetectBgCollisions(platzActor *a, u8 enTrig) {
	u8 i,j,start,fin,retVal = 0,colVal = 0,sp = 0,loops = 0,loopMax = 0,mode = 0,mpIndex = 0;
	char trig,collDir,overlap,xVel = GET_VEL(a->vx),yVel = GET_VEL(a->vy);
	rect16 rPre,rPost,rBg,rColl,rTrig;
	line16 lMove,lBg;
	pt pos = (pt){a->loc.x+a->trLoc.x,a->loc.y+a->trLoc.y};
	bgOuter bgo;
	bgDirectory bgd;

	// Pre-use rPost to save allocating another variable
	rPost.left = (int)(a->loc.y)+(int)yVel;

	if ((rPost.left+a->bby) > (PLATZ_SCRN_HGT-1)) {
		// Collision with bottom of screen
		yVel = PLATZ_SCRN_HGT-a->bby-a->loc.y-1;
		retVal |= T_INTERSECT;
	} else if ((rPost.left-a->bby) < 0) {
		// Collision with top of screen
		yVel = -(a->loc.y-a->bby);
		retVal |= B_INTERSECT;
	}

	rPre.left = a->loc.x-a->bbx;
	rPre.right = a->loc.x+a->bbx;
	rPre.top = a->loc.y-a->bby;
	rPre.btm = a->loc.y+a->bby;
	rPost.left = rPre.left+xVel;
	rPost.right = rPre.right+xVel;
	rPost.top = rPre.top+yVel;
	rPost.btm = rPre.btm+yVel;

	if ((rPre.left >= 0) && (rPost.left >= 0) && (rPre.right <= (SCRL_WID-1)) && (rPost.right <= (SCRL_WID-1))) {
		// Not on seam - test slice that player is on
		overlap = 0;
		loopMax = 2;
#if MAX_MOVING_PLATFORMS
		mode = DBGC_MODE_PLATS;
#else
		mode = DBGC_MODE_NO_SEAM;
#endif
		collDir = 0;
		sp = csp;
	} else {
		// On seam - test both slices
		loopMax = 2;
		mode = DBGC_MODE_SEAM_CURR;
	}

	// Only need to check through once on each slice IF bgs are tile-aligned AND max_speed == MIN(TILE_HEIGHT,TILE_WIDTH)
	while (loops++ < loopMax) {
		if (mode == DBGC_MODE_SEAM_CURR) {
			if ((rPost.left < 0) || (rPost.right > (SCRL_WID-1))) {
				if (a->loc.x > (SCRL_WID>>1)) {
					rPost.left -= SCRL_WID;
					rPost.right -= SCRL_WID;
					collDir = 1;
					sp = (csp < wspMax)?csp+1:0;
				} else {
					rPost.left += SCRL_WID+1;	// SCRL_WID would force a false positive due to 0 == 256
					rPost.right += SCRL_WID+1;	// Doesn't occur when going right because max xLoc is 255
					collDir = -1;
					sp = (csp)?csp-1:wspMax;
				}

				if ((rPost.left > (SCRL_WID>>1)) && (rPre.left < (SCRL_WID>>1))) {
					overlap = 1;
				} else if ((rPost.left < (SCRL_WID>>1)) && (rPre.left > (SCRL_WID>>1))) {
					overlap = -1;
				} else {
					overlap = 0;
				}
			} else {
				++mode;
				continue;
			}
		} else if (mode == DBGC_MODE_SEAM_NEXT) {
			sp = csp;
			collDir = -collDir;
			rPost.left = rPre.left+xVel;
			rPost.top = rPre.top+yVel;
			rPost.right = rPre.right+xVel;
			rPost.btm = rPre.btm+yVel;
				
			if ((rPost.left > (SCRL_WID>>1)) && (rPre.left < (SCRL_WID>>1))) {
				overlap = 1;
			} else if ((rPost.left < (SCRL_WID>>1)) && (rPre.left > (SCRL_WID>>1))) {
				overlap = -1;
			} else {
				overlap = 0;
			}
		}

		memcpy_P(&bgd, bgDir+sp, sizeof(bgDirectory));
		
#if MAX_MOVING_PLATFORMS
		if (mode == DBGC_MODE_PLATS) {
			start = 0;
			
			if (mp.slice[0] == sp) {
				fin = mp.count[0];
				mpIndex = 0;
			} else if (mp.slice[1] == sp) {
				fin = mp.count[1];
				mpIndex = 1;
			} else {
				fin = 0;
			}
		} else if (collDir > 0) {
#else
		if (collDir > 0) {
#endif
			start = bgd.bgoIndex;
			fin = start+bgd.bgoBeginCount;
		} else if (collDir < 0) {
			start = bgd.bgoIndex+bgd.bgoEndIndex;
			fin = start+bgd.bgoEndCount;
		} else {
			start = bgd.bgoIndex;
			fin = start+bgd.bgoCount;
		}

		for (i = start, j = 0; i < fin; i++) {
			colVal = 0;

#if MAX_MOVING_PLATFORMS
			if (mode == DBGC_MODE_PLATS) {
				rBg.left = mp.p[mpIndex][j].r.left;
				rBg.right = mp.p[mpIndex][j].r.right;
				rBg.top = mp.p[mpIndex][j].r.top;
				rBg.btm = mp.p[mpIndex][j++].r.btm;
			} else {
				memcpy_P(&bgo,bgoTbl+i,sizeof(bgOuter));
				rBg.left = (bgo.r.left<<3);
				rBg.right = (bgo.r.right<<3);
				rBg.top = (bgo.r.top<<3);
				rBg.btm = (bgo.r.btm<<3);
			}
#else
			memcpy_P(&bgo,bgoTbl+i,sizeof(bgOuter));
			rBg.left = (bgo.r.left<<3);
			rBg.right = (bgo.r.right<<3);
			rBg.top = (bgo.r.top<<3);
			rBg.btm = (bgo.r.btm<<3);
#endif

			if (PlatzRectsIntersect16(&rPost,&rBg)) {
				rColl.left = MAX(rPost.left,rBg.left);
				rColl.right = MIN(rPost.right,rBg.right);
				rColl.top = MAX(rPost.top,rBg.top);
				rColl.btm = MIN(rPost.btm,rBg.btm);

				// Process triggers
				if ((mode != DBGC_MODE_PLATS) && (bgo.type&(BGTH|BGTV))) {
					if (enTrig) {
						if (!PT_NOT_IN_RECT(pos,rBg)) {
							rTrig = rBg;

							if (bgo.type&(BGTH)) {
								rTrig.btm -= (rTrig.btm-rTrig.top)>>1;
							} else {
								rTrig.right -= (rTrig.right-rTrig.left)>>1;
							}
				
							if (!PT_NOT_IN_RECT(pos,rTrig)) {
								// Unused bgOuter.count in triggers used for firing orientation (not axis)
								trig = (bgo.count)?-1:1;
							} else {
								trig = (bgo.count)?1:-1;
							}

							trigCb(bgo.index,bgo.type,trig);
						}
					}
				} else if ((mode == DBGC_MODE_PLATS) || (bgo.type&BGC)) {
					if (xVel == 0) {
						colVal = 12;
					} else if (yVel == 0) {
						colVal = 3;
					} else {
						lMove.p1.x = (xVel > 0)?rPre.right+((int)overlap<<SCRL_WID_SHFT):rPre.left+((int)overlap<<SCRL_WID_SHFT);
						lMove.p1.y = (yVel > 0)?rPre.btm:rPre.top;
						lMove.p2.x = lMove.p1.x+xVel;
						lMove.p2.y = lMove.p1.y+yVel;

						if ((lMove.p1.x >= rBg.left) && (lMove.p1.x < rBg.right)) {
							colVal = 12;
						} else if ((lMove.p1.y >= rBg.top) && (lMove.p1.y < rBg.btm)) {
							colVal = 3;
						} else if ((PT_NOT_IN_RECT(lMove.p2,rBg)) && (((lMove.p1.x < rBg.left) || (lMove.p1.x >= rBg.right))
								&& ((lMove.p1.y >= rBg.btm) || (lMove.p1.y < rBg.top)))) {
							if (xVel > 0) {
								colVal = (rPost.right < rBg.right)?3:12;
							} else {
								colVal = (rPost.left > rBg.left)?3:12;
							}
						} else {
							lBg.p1.x = rBg.left;
							lBg.p2.x = rBg.right;

							if (lMove.p1.y < rBg.top) {
								lBg.p1.y = rBg.top;
								lBg.p2.y = rBg.top;
							} else {
								lBg.p1.y = rBg.btm;
								lBg.p2.y = rBg.btm;
							}
						
							if (PlatzLinesIntersect(&lMove,&lBg)) {
								// Top/Btm intersect
								colVal = 12;
							} else {
								// Left/Right intersect
								colVal = 3;
							}
						}
					}

					if (colVal&V_INTERSECT) {
						xVel -= NORMALIZE(xVel)+NORMALIZE(xVel)*(rColl.right-rColl.left);
					} else if (colVal&H_INTERSECT) {
						yVel -= NORMALIZE(yVel)+NORMALIZE(yVel)*(rColl.btm-rColl.top);
					}

					rPost.left = rPre.left+xVel;
					rPost.top = rPre.top+yVel;
					rPost.right = rPre.right+xVel;
					rPost.btm = rPre.btm+yVel;

					if (mode == DBGC_MODE_SEAM_CURR) {
						if (a->loc.x > (SCRL_WID>>1)) {
							rPost.left -= SCRL_WID;
							rPost.right -= SCRL_WID;
							collDir = 1;
							sp = (csp < wspMax)?csp+1:0;
						} else {
							rPost.left += SCRL_WID;
							rPost.right += SCRL_WID;
							collDir = -1;
							sp = (csp)?csp-1:wspMax;
						}
					}

					// Determine exact intersect type
					if (colVal&V_INTERSECT) {
						// Horizontal
						if (rPre.right < rBg.left) {
							// Left intersect
							retVal |= L_INTERSECT;
						} else {
							// Right intersect
							retVal |= R_INTERSECT;
						}
					} else if (colVal&H_INTERSECT) {
						// Vertical
						if (rPre.btm < rBg.top) {
							// Top intersect
							retVal |= T_INTERSECT;
						} else {
							// Btm intersect
							retVal |= B_INTERSECT;
						}
					}
				}
			}		
		}
		++mode;
	}

	// Commit altered speeds
	if (retVal&V_INTERSECT) {
		PlatzSetVelocity(&a->vx,xVel<<2,&a->trLoc.x);
	}

	if (retVal&H_INTERSECT) {
		PlatzSetVelocity(&a->vy,yVel<<2,&a->trLoc.y);
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

#define PD_INDEX_OFFSET 10	// Byte offset of pdIndex in bgDirectory
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
		mp.slice[src] = MAX_SLICES+1;	// Set as invalid
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
void PlatzDrawMovingPlatforms(void) {
	u8 i,j,k,offset,hgt,tile,*near,*far,scrX = Screen.scrollX>>3,tiles = 0,clear = 0,op = 0,len = 0;
	rect r,rc;

	for (i = 0; i < MAX_VIS_SLICES; i++) {
		for (j = 0; j < mp.count[i]; j++) {
			r.top = mp.p[i][j].r.top>>3;
			r.btm = mp.p[i][j].r.btm>>3;
			r.left = mp.p[i][j].r.left>>3;
			r.right = mp.p[i][j].r.right>>3;
			offset = (mp.p[i][j].axis == AXIS_X)?mp.p[i][j].r.left&7:mp.p[i][j].r.top&7;
			clear = 0;
			op = (offset)?1:0;

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
				PlatzFill(&rc,cp.clrTile);
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

			hgt = (*far)-(*near);
			*far = (*near)+1;
			len = (mp.p[i][j].r.right>>3)-(mp.p[i][j].r.left>>3)+((offset)?1:0);

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
#define AC_INDEX_OFFSET		9	// Byte offset of animCount in bgDirectory
// Loads all animated bgs for the visible slices.
void PlatzLoadAnimatedBgs(u8 sp, char dir) {
	u8 i,j,animIndex,animCount,src,dest;
	char *bgdAddr = (char*)bgDir;	// Only want animCount from bg directory
	u16 bgoIndex;
	bgOuter bgo;

	if (dir == DIR_RIGHT) {
		src = 1;
		dest = 0;
	} else {
		src = 0;
		dest = 1;
	}

	// Preserve animations that are still visible
	animBgs.count[dest] = animBgs.count[src];
	memcpy(animBgs.bgs[dest],animBgs.bgs[src],MAX_ANIMATED_BGS*sizeof(bgInner));
	memcpy(animBgs.anims[dest],animBgs.anims[src],MAX_ANIMATED_BGS*sizeof(animation));
	// Check next slice for animations
	animCount = pgm_read_byte(bgdAddr+((sp*sizeof(bgDirectory))+AC_INDEX_OFFSET));

	if (animCount) {
		animBgs.count[src] = animCount;
		bgoIndex = pgm_read_byte(bgdAddr+((sp*sizeof(bgDirectory))+BGO_INDEX_OFFSET));

		for (i = 0,animIndex = 0; i < animCount; i++) {
			// Load outer bgs that contain animations
			memcpy_P(&bgo,bgoTbl+bgoIndex+i,sizeof(bgOuter));
		
			for (j = 0; j < bgo.animCount; j++) {
				// Load inner bgs that contain animations
				memcpy_P(&animBgs.bgs[src][animIndex],bgiTbl+bgo.index+j,sizeof(bgInner));
				memcpy_P(&animBgs.anims[src][animIndex],animBgTbl+animBgs.bgs[src][animIndex].tile,sizeof(animation));
				++animIndex;
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
	u8 scrX = Screen.scrollX>>3;

	for (i = 0; i < MAX_VIS_SLICES; i++) {
		for (j = 0; j < animBgs.count[i]; j++) {
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
					PlatzFillPattern(&r,animBgs.anims[i][j].wid, animBgs.anims[i][j].hgt,0,patOffsetX, 
						animBgs.anims[i][j].frames+animBgs.anims[i][j].currFrame*animBgs.anims[i][j].size);
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
	if ((a->loc.x>>3) == 0) {
		if ((xPrev>>3) == 31) {
			csp = (csp < wspMax)?csp+1:0;
		}
	} else if ((a->loc.x>>3) == 31) {
		if ((xPrev>>3) == 0) {
			csp = (csp)?csp-1:wspMax;
		}
	}
	xPrev = a->loc.x;
}

// Manages drawing on the scrolling edge
void PlatzScroll(char xDelta) {
	// Update world slice pointer
	u8 scrX = Screen.scrollX;

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
		if ((scrX>>3) != scrXMod) {
			scrXMod = scrX>>3;

			if ((scrXMod == 31) && (scrDirX == DIR_LEFT)) {
				wsp = (wsp)?wsp-1:wspMax;
#if MAX_ANIMATED_BGS
				PlatzLoadAnimatedBgs(wsp,DIR_LEFT);
#endif

#if MAX_MOVING_PLATFORMS
				PlatzLoadMovingPlatforms(wsp,DIR_LEFT);
#endif
			}

			PlatzDrawColumn((u8)(scrX-xDelta+scrDirAdj)>>3,scrDirX);

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
	PlatzDrawMovingPlatforms();
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
		
		if ((xDelta > 0) && ((a->sprx+xDelta) < (vpAnchor+VIEWPORT_SLACK))) {
			a->sprx += xDelta;
		} else if ((xDelta < 0) && ((a->sprx+xDelta) > (vpAnchor-VIEWPORT_SLACK))) {
			a->sprx += xDelta;
		} else {
			Screen.scrollX += xDelta;
		}
	} else {
		scrDirX = DIR_NONE;
	}

	if (yVel) {
		a->loc.y += yVel;
	}

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


