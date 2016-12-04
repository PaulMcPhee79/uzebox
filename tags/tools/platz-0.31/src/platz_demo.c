/*
 *  Platz demo
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
 *				Constants				*
 ****************************************/

#ifndef FORMAT_EEPROM
	FORMAT_EEPROM = 0
#endif

// Background tile indices
#define TILE_SKY						0
#define TILE_GRASS_LT					(TILE_SKY+1)
#define TILE_GRASS						(TILE_SKY+2)
#define TILE_GRASS_RT					(TILE_SKY+3)
#define TILE_GRASS_WATER_LT				(TILE_SKY+4)
#define TILE_GRASS_WATER_RT				(TILE_SKY+5)
#define TILE_GRASS_GND_LT				(TILE_SKY+6)
#define TILE_GRASS_GND_RT				(TILE_SKY+7)
#define TILE_GND_LT						(TILE_SKY+8)
#define TILE_GND						(TILE_SKY+9)
#define TILE_GND_RT						(TILE_SKY+10)
#define TILE_GND_WATER_LT				(TILE_SKY+11)
#define TILE_GND_WATER_RT				(TILE_SKY+12)
#define TILE_GND_WAVE_LT				(TILE_SKY+13)
#define TILE_GND_WAVE_RT				(TILE_SKY+14)
#define TILE_GND_CNR_WATER_LT			(TILE_SKY+15)
#define TILE_GND_CNR_WATER_RT			(TILE_SKY+16)
#define TILE_WATER						(TILE_SKY+17)
#define TILE_WAVE						(TILE_SKY+18)
#define TILE_SPIKE						(TILE_SKY+19)
#define TILE_GND_SHARP_LT				(TILE_SKY+20)
#define TILE_GND_SHARP_RT				(TILE_SKY+21)
#define TILE_GND_SHARP_BTM				(TILE_SKY+22)
#define TILE_GND_SHARP_CNR_LT			(TILE_SKY+23)
#define TILE_GND_SHARP_CNR_RT			(TILE_SKY+24)
#define TILE_STONE_TOP_LT				(TILE_SKY+25)
#define TILE_STONE_TOP_RT				(TILE_SKY+26)
#define TILE_STONE_BTM_LT				(TILE_SKY+27)
#define TILE_STONE_BTM_RT				(TILE_SKY+28)
#define TILE_END						(TILE_SKY+28)

// Invisible triggers
#define TRIG_WATER						(TILE_END+1)
#define TRIG_WFALL0						(TILE_END+2)
#define TRIG_WFALL1						(TILE_END+3)
#define TRIG_WFALL2						(TILE_END+4)

// Player
#define SPRITE_BOB						0
#define SPD_MAX_BOB_GND_X				1
#define SPD_MAX_BOB_GND_Y				2
#define SPD_MAX_LYNX_GND_X				12
#define SPD_MAX_LYNX_GND_Y				12
#define SPD_MAX_DRAGONFLY_X				12
#define SPD_MAX_DRAGONFLY_Y				8
#define SPD_MAX_TURTLE_X				12
#define SPD_MAX_TURTLE_Y				4
#define LOC_BOB_X						120
#define LOC_BOB_Y						111
	// States
#define BS_MOVING						1
#define BS_SWING						2
#define BS_COCONUT						4
#define BS_THROW						8

	// Terrestrial jumping
#define TM_YSTATE_IDLE					1
#define TM_YSTATE_TAKEOFF				2
#define TM_YSTATE_RISING				4
#define TM_YSTATE_PEAK					8
#define TM_YSTATE_FALLING				16
	// Forms
#define FORM_LYNX						1
#define FORM_DRAGONFLY					2
#define FORM_TURTLE						4

// Movement
#define MOVE_NONE						0
#define MOVE_LEFT						BTN_LEFT
#define MOVE_RIGHT						BTN_RIGHT
#define MOVE_DOWN						BTN_DOWN
#define MOVE_UP							BTN_UP

// Misc
#define EEPROM_JUNK_ID					5865

/****************************************
 *				Utils					*
 ****************************************/
// General
#define MIN(_x,_y)  ((_x)<(_y) ? (_x) : (_y))
#define MAX(_x,_y)  ((_x)>(_y) ? (_x) : (_y))
#define MOD2N(_a,_n) ((_a)&((_n)-1))	// 2^n mod
#define ABS(_x)		(((_x) > 0) ? (_x) : -(_x))

/****************************************
 *			Type declarations			*
 ****************************************/
typedef struct dsdt {
	u8 ds;
	u8 count;
	u8 limit;
} dsdt;

typedef struct accel {		// Single axis acceleration
	u8 		state;			// Movement state (jumping etc)
	u8 		max;
	dsdt 	acc;			// Acceleration
	dsdt 	dec;			// Deceleration
} accel;

typedef struct player {
	platzActor	pa;
	u8			sprite;
	u8			form;
	u8			state;
	u8			animFlag;
	accel		ax;
	accel		ay;
	animation 	anim;
	void (*moveFunc)(u16);
} player;

/****************************************
 *			Data Dependencies			*
 ****************************************/
// Tiles
#include "data/tiles.pic.inc"
#include "data/tiles.map.inc"
// Sprites
#include "data/sprites.pic.inc"
#include "data/sprites.map.inc"
// Animations
#include "data/animations.map.inc"
#include "data/patches.inc"

/****************************************
 *			File-level variables		*
 ****************************************/
player bob;
u8 wfall;
u8 wfallTmr;
u8 wfallVol;

/****************************************
 *			Function prototypes			*
 ****************************************/
void ActivateTrigger(u8 tile, u8 type, char trig);		// Platz event callback target
void AerialMovement(u16 move);
void AerialAnimation(char dir, char prevDir);
void TerrestrialMovement(u16 move);
void TerrestrialAnimation(char prevDir, u16 move, u8 collMask);
void AquaticMovement(u16 move);
void AquaticAnimation(char prevDir, u16 move);
void SetForm(u8 form, char dirX);
void LoadBobAnimations(char dirX);
void BobFormatEeprom(u8 quick);
u16 BobLoadGame(void);
void BobSaveGame(u8 sp);

/****************************************
 *			Function definitions		*
 ****************************************/

void BobFormatEeprom(u8 quick) {
	// Set sig. so we don't format next time
	WriteEeprom(0,(u8)EEPROM_SIGNATURE);
	WriteEeprom(1,(u8)(EEPROM_SIGNATURE>>8));
	
	// Paint free blocks
	for (u8 i = 2; i < 64; i++) {
		WriteEeprom(i*EEPROM_BLOCK_SIZE,(u8)EEPROM_FREE_BLOCK);
		WriteEeprom(i*EEPROM_BLOCK_SIZE+1,(u8)(EEPROM_FREE_BLOCK>>8));

		if (quick) break;
	}
}

// Loads bob's platz actor details and returns world slice
u16 BobLoadGame(void) {
	struct EepromBlockStruct ebs;
	
	ebs.id = EEPROM_JUNK_ID;	// We should have a designated "junk" id for demos

	if (isEepromFormatted() && (EepromReadBlock(EEPROM_JUNK_ID,&ebs) == 0)) {
		memcpy(&bob.pa,ebs.data,sizeof(platzActor));
		return (ebs.data[sizeof(platzActor)]<<8)|ebs.data[sizeof(platzActor)+1];
	} else {
		return MAX_SLICES+1; // Invalid slice flag in high byte
	}
}


void BobSaveGame(u8 sp) {
	struct EepromBlockStruct ebs = {EEPROM_JUNK_ID,{ 0 }};

	ebs.id = EEPROM_JUNK_ID;
	memcpy(ebs.data,&bob.pa,sizeof(platzActor));
	ebs.data[sizeof(platzActor)] = bob.form;
	ebs.data[sizeof(platzActor)+1] = sp;

	if (!isEepromFormatted()) {
		BobFormatEeprom(1);
	}

	EepromWriteBlock(&ebs);
}


void LoadBobAnimations(char dirX) {
	u8 index = 0;

	if (dirX > 0) index += 1;

	switch (bob.form) {
		case FORM_LYNX:
			if ((bob.state&BS_MOVING) || bob.pa.vx.vel) {
				// Move past lynx idle animation
				index += 2;
			}
			break;
		case FORM_DRAGONFLY:
			index += 4;
			break;
		case FORM_TURTLE:
			index += 6;
			break;
	}
		
	memcpy_P(&bob.anim,bobAnimations+index,sizeof(animation));
}


void ActivateTrigger(u8 tile, u8 type, char trig) {
	static char wfallState = -1;

	switch(tile) {
		case TRIG_WATER:				
			if (trig > 0) {
				if (bob.form == FORM_TURTLE) {
					SetForm(FORM_DRAGONFLY,bob.pa.vx.dir);
				}
			} else {
				if (bob.form != FORM_TURTLE) {
					SetForm(FORM_TURTLE,bob.pa.vx.dir);
				}					
			}				
			break;
		case TRIG_WFALL0:
			if (trig > 0) {
				if (wfallState != -1) {
					wfall = 0;
					wfallTmr = 0;
					wfallState = -1;
				}
			} else {
				if (wfallState != 0) {
					wfall = 1;
					wfallVol = 0x1F;
					wfallState = 0;
				}
			}
			break;
		case TRIG_WFALL1:
			if (trig > 0) {
				if (wfallState != 1) {
					wfallVol = 0x1F;
					wfallState = 1;
				}
			} else {
				if (wfallState != 2) {
					wfallVol = 0x2F;
					wfallTmr = 15;
					wfallState = 2;
				}
			}
			break;
		case TRIG_WFALL2:
			if (trig > 0) {
				if (wfallState != 3) {
					wfallVol = 0x2F;
					wfallState = 3;
				}
			} else {
				if (wfallState != 4) {
					wfallVol = 0x4F;
					wfallState = 4;
				}
			}		
			break;
	}
}


void SetForm(u8 form, char dirX) {
	if (bob.form == form) {
		return;
	}

	u8 prevForm = bob.form;
	PlatzHideSprite(bob.sprite,bob.anim.wid,bob.anim.hgt);
	bob.form = form;
	LoadBobAnimations(dirX);
	
	// Don't call PlatzSetBoundingBoxDimensions before bob has been initialised
	if (!prevForm || PlatzSetBoundingBoxDimensions(&bob.pa,bob.anim.wid<<3,bob.anim.hgt<<3)) {
		switch (form) {		
			case FORM_DRAGONFLY:
				PlatzSetVelocity(&bob.pa.vy,MIN(bob.pa.vy.vel,SPD_MAX_DRAGONFLY_Y),&bob.pa.trLoc.y);
				bob.moveFunc = AerialMovement;
				bob.ax.acc.ds = 2;
				bob.ax.dec.ds = 2;
				bob.ax.acc.count = 0;
				bob.ax.dec.count = 0;
				bob.ax.acc.limit = 8;
				bob.ax.dec.limit = 16;
				bob.ax.max = SPD_MAX_DRAGONFLY_X;
				bob.ay.acc.ds = 2;
				bob.ay.dec.ds = 1;
				bob.ay.acc.count = 0;
				bob.ay.dec.count = 0;
				bob.ay.acc.limit = 6;
				bob.ay.dec.limit = 10;
				bob.ay.max = SPD_MAX_DRAGONFLY_Y;
				break;
			case FORM_TURTLE:
				PlatzSetVelocity(&bob.pa.vy,MIN(bob.pa.vy.vel,SPD_MAX_TURTLE_Y),&bob.pa.trLoc.y);
				bob.moveFunc = AquaticMovement;
				bob.ax.acc.ds = 8;
				bob.ax.dec.ds = 2;
				bob.ax.acc.count = 0;
				bob.ax.dec.count = 0;
				bob.ax.acc.limit = 0;
				bob.ax.dec.limit = 12;
				bob.ax.max = SPD_MAX_TURTLE_X;
				bob.ay.acc.ds = 1;
				bob.ay.dec.ds = 1;
				bob.ay.acc.count = 0;
				bob.ay.dec.count = 0;
				bob.ay.acc.limit = 16;
				bob.ay.dec.limit = 32;
				bob.ay.max = SPD_MAX_TURTLE_Y;
				break;
			case FORM_LYNX:
			default:
				bob.moveFunc = TerrestrialMovement;
				bob.ax.acc.ds = 2;
				bob.ax.dec.ds = 2;
				bob.ax.acc.count = 0;
				bob.ax.dec.count = 0;
				bob.ax.acc.limit = 8;
				bob.ax.dec.limit = 5;
				bob.ax.max = SPD_MAX_LYNX_GND_X;
				bob.ay.acc.ds = 2;
				bob.ay.dec.ds = 16;
				bob.ay.acc.count = 0;
				bob.ay.dec.count = 0;
				bob.ay.acc.limit = 2;
				bob.ay.dec.limit = 2;
				bob.ay.max = SPD_MAX_LYNX_GND_Y;
				bob.ay.state = TM_YSTATE_IDLE;
				break;		
		}
	} else {
		bob.form = prevForm;
	}

	LoadBobAnimations(dirX);
	PlatzMapSprite(bob.anim.currFrame*bob.anim.size, bob.anim.wid, bob.anim.hgt, bob.anim.frames);
}


void AerialMovement(u16 move) {
	char xVel = bob.pa.vx.vel, yVel = bob.pa.vy.vel;

	// X axis - Apply wind resistance
	if ((((move&(MOVE_LEFT|MOVE_RIGHT)) == 0) && xVel) || ((move&MOVE_LEFT) && (xVel > 0)) || ((move&MOVE_RIGHT) && (xVel < 0))) {			
		if (++(bob.ax.dec.count) == bob.ax.dec.limit) {
			bob.ax.dec.count = 0;
			xVel += (xVel > 0)?MAX(-xVel,-bob.ax.dec.ds):MIN(-xVel,bob.ax.dec.ds);
		}						
	} else {
		bob.ax.dec.count = 0;
	}

	// Apply player input
	if (move&MOVE_LEFT) {
		if ((xVel > -bob.ax.max) && (++(bob.ax.acc.count) == bob.ax.acc.limit)) {
			bob.ax.acc.count = 0;
			xVel -= bob.ax.acc.ds;
		}
	} else if (move&MOVE_RIGHT) {
		if ((xVel < bob.ax.max) && (++(bob.ax.acc.count) == bob.ax.acc.limit)) {
			bob.ax.acc.count = 0;
			xVel += bob.ax.acc.ds;
		}
	}

	// Y axis - Apply gravity
	if ((yVel < bob.ay.max) && (++(bob.ay.dec.count) == bob.ay.dec.limit)) {
		bob.ay.dec.count = 0;
		yVel += bob.ay.dec.ds;			
	}

	if (move&MOVE_UP) {
		if ((yVel > -bob.ay.max) && (++(bob.ay.acc.count) == bob.ay.acc.limit)) {
			bob.ay.acc.count = 0;
			yVel -= bob.ay.acc.ds;
		}
	} else if (move&MOVE_DOWN) {
		if ((yVel < bob.ay.max) && (++(bob.ay.acc.count) == bob.ay.acc.limit)) {
			bob.ay.acc.count = 0;
			yVel += bob.ay.acc.ds;
		}
	}

	PlatzSetVelocity(&bob.pa.vx,xVel,&bob.pa.trLoc.x);
	PlatzSetVelocity(&bob.pa.vy,yVel,&bob.pa.trLoc.y);
}


void AerialAnimation(char dir, char prevDir) {
	// Check for direction change
	if (dir != prevDir) {
		bob.animFlag |= 3;
	}

	if (++bob.anim.disp >= bob.anim.dpf) {
		// Bob has moved far enough to warrant a new frame
		bob.anim.disp = 0;
		bob.anim.currFrame++;

		if (bob.anim.currFrame == bob.anim.frameCount) {
			bob.anim.currFrame = 0;
		}

		bob.animFlag |= 0x02;
	}
}


void TerrestrialMovement(u16 move) {
	static u8 jumpPeakCount = 8;
	char xVel = bob.pa.vx.vel, yVel = bob.pa.vy.vel;

	// X axis
	if (yVel == 0 && (bob.ay.state == TM_YSTATE_IDLE)) {	// Don't let lynx accelerate x axis while airborne
		// Apply ground friction (Note: could use a trigger to disable this for an oil-slick surface)
		if (((move == MOVE_NONE) && xVel) || ((move&MOVE_LEFT) && (xVel > 0)) || ((move&MOVE_RIGHT) && (xVel < 0))) {
			if (++(bob.ax.dec.count) == bob.ax.dec.limit) {
				bob.ax.dec.count = 0;
				xVel += (xVel > 0)?MAX(-xVel,-bob.ax.dec.ds):MIN(-xVel,bob.ax.dec.ds);
			}
		} else {
			bob.ax.dec.count = 0;
		}

		// Apply player input
		if (move&MOVE_LEFT) {
			if ((xVel > -bob.ax.max) && (++(bob.ax.acc.count) == bob.ax.acc.limit)) {
				bob.ax.acc.count = 0;
				xVel -= bob.ax.acc.ds;
			}
		} else if (move&MOVE_RIGHT) {
			if ((xVel < bob.ax.max) && (++(bob.ax.acc.count) == bob.ax.acc.limit)) {				
				bob.ax.acc.count = 0;
				xVel += bob.ax.acc.ds;
			}
		}
	}

	// Y axis - Apply player input
	if ((yVel == 0) && (bob.ay.state == TM_YSTATE_IDLE)) {
		if (move&MOVE_UP) {
			bob.ay.state = TM_YSTATE_TAKEOFF;
		}
	}

	// Jumping
	switch (bob.ay.state) {
		case TM_YSTATE_TAKEOFF:
			bob.ay.acc.count = 0;	// Ensure all jumps are of equal height
			yVel = -bob.ay.dec.ds;
			bob.anim.currFrame = 1;
			bob.state |= BS_MOVING;
			bob.animFlag |= 0x03;
			bob.ay.state = TM_YSTATE_RISING;
			break;
		case TM_YSTATE_RISING:
			if (yVel == 0) {
				bob.anim.currFrame = 2;
				bob.animFlag |= 0x02;
				jumpPeakCount = 8;
				bob.ay.state = TM_YSTATE_PEAK;
			}
			break;
		case TM_YSTATE_PEAK:
			if (--jumpPeakCount == 0) {
				bob.anim.currFrame = 3;
				bob.animFlag |= 0x02;
				bob.ay.state = TM_YSTATE_FALLING;
			}
			break;
		case TM_YSTATE_FALLING:	// Ground collision finalises the jump
		case TM_YSTATE_IDLE:
		default:
			// Do nothing
			break;
	}

	// Apply gravity (excluding jump peak)
	if ((bob.ay.state != TM_YSTATE_PEAK) && (yVel < bob.ay.max) && (++(bob.ay.acc.count) == bob.ay.acc.limit)) {
		bob.ay.acc.count = 0;
		yVel += bob.ay.acc.ds;
	}

	PlatzSetVelocity(&bob.pa.vx,xVel,&bob.pa.trLoc.x);
	PlatzSetVelocity(&bob.pa.vy,yVel,&bob.pa.trLoc.y);
}


void TerrestrialAnimation(char prevDir, u16 move, u8 collMask) {
	char yVel = GET_VEL(bob.pa.vy);

	// Check for direction change
	if (bob.pa.vx.dir != prevDir) {
		bob.animFlag |= 3;
	}
	
	if (collMask&4) {
		// Y-Axis collision detected
		if ((bob.pa.vx.vel == 0) && (bob.ay.state != TM_YSTATE_IDLE)) {
			bob.animFlag |= 3;
		}
		bob.ay.state = TM_YSTATE_IDLE;	// End of jump/fall
	} else if ((collMask&8) && (bob.ay.state != TM_YSTATE_IDLE)) {
		bob.ay.state = TM_YSTATE_FALLING;

	}

	if ((yVel > 0) && (bob.ay.state == TM_YSTATE_IDLE)) {
		if ((bob.state&BS_MOVING) == 0) {
			bob.state |= BS_MOVING;
		}
		bob.ay.state = TM_YSTATE_FALLING;
		bob.animFlag |= 3;
	}

	// Animations
	if ((move == MOVE_NONE) && ((bob.pa.vx.vel | bob.pa.vy.vel) == 0) && (bob.ay.state == TM_YSTATE_IDLE) && (bob.state&BS_MOVING)) {
		bob.state&=~BS_MOVING;
		bob.animFlag |= 0x03;
	} else if (((bob.state&BS_MOVING) == 0) && bob.pa.vx.vel){
		bob.state |= BS_MOVING;
		bob.animFlag |= 0x03;
	}

	if ((yVel == 0) && (bob.ay.state == TM_YSTATE_IDLE)) {
		bob.anim.disp++;

		if (bob.anim.disp >= bob.anim.dpf) {
			// Bob has moved far enough to warrant a new frame
			bob.anim.disp = 0;
			bob.anim.currFrame++;

			if (bob.anim.currFrame == bob.anim.frameCount) {
				bob.anim.currFrame = 0;
			}

			bob.animFlag |= 0x02;
		}
	} else if (bob.pa.vy.vel && (bob.ay.state == TM_YSTATE_FALLING)) {
		// Special frame for falling
		bob.anim.currFrame = 3;
		bob.animFlag |= 0x02;
	}
}

void AquaticMovement(u16 move) {
	char xVel = bob.pa.vx.vel, yVel = bob.pa.vy.vel;

	// X axis
	if (xVel) {
		// Apply friction
		if (++(bob.ax.dec.count) == bob.ax.dec.limit) {
			bob.ax.dec.count = 0;
			xVel += (xVel > 0)?MAX(-xVel,-bob.ax.dec.ds):MIN(-xVel,bob.ax.dec.ds);
		}
	} else if (move&MOVE_LEFT) {
		xVel = -bob.ax.acc.ds;
	} else if (move&MOVE_RIGHT) {
		xVel = bob.ax.acc.ds;
	}

	// Y axis - Apply buoyancy
	if ((yVel >= 0) && (++(bob.ay.dec.count) == bob.ay.dec.limit)) {
		bob.ay.dec.count = 0;			
		yVel -= bob.ay.dec.ds;
	}

	if (move&MOVE_UP) {
		if ((yVel > -bob.ay.max) && (++(bob.ay.acc.count) == bob.ay.acc.limit)) {
			bob.ay.acc.count = 0;
			yVel -= bob.ay.acc.ds;
		}
	} else if (move&MOVE_DOWN) {
		if ((yVel < bob.ay.max) && (++(bob.ay.acc.count) == bob.ay.acc.limit)) {
			bob.ay.acc.count = 0;
			yVel += bob.ay.acc.ds;
		}
	}

	PlatzSetVelocity(&bob.pa.vx,xVel,&bob.pa.trLoc.x);
	PlatzSetVelocity(&bob.pa.vy,yVel,&bob.pa.trLoc.y);
}


void AquaticAnimation(char prevDir, u16 move) {
	// Check for direction change
	if (bob.pa.vx.dir != prevDir) {
		bob.animFlag |= 3;
	}

	if (bob.pa.vx.vel == 0) {
		if (move&(MOVE_UP|MOVE_DOWN)) {
			bob.anim.disp++;

			if (bob.anim.disp >= bob.anim.dpf) {
				bob.anim.disp = 0;
				bob.anim.currFrame = (bob.anim.currFrame == 1) ? 2 : 1;
				bob.animFlag |= 0x02;
			}
		} else if (bob.anim.currFrame != 0) {
			bob.anim.currFrame = 0;
			bob.animFlag |= 0x02;
		}
	} else if (ABS(bob.pa.vx.vel) <= 4) {
		if (bob.anim.currFrame != 1) {
			bob.anim.currFrame = 1;
			bob.animFlag |= 0x02;
		}
	} else {
		if (bob.anim.currFrame != 2) {
			bob.anim.currFrame = 2;
			bob.animFlag |= 0x02;
		}
	}
}


int main() {
	u16 move = MOVE_NONE; 	// Bob's current movement instruction
	u8 collFlag = 0;		// Collision flag
	char prevDir = 0;
	u16 btnPrev = 0;		// Previous buttons that were held
	u16 btnHeld = 0;    	// Buttons that are held right now
	u16 btnPressed = 0;  	// Buttons that were pressed this frame
	u16 btnReleased = 0;	// Buttons that were released this frame
	u8 sp = 0;

#if FORMAT_EEPROM
	BobFormatEeprom(0);
#endif

	// Init kernel
	InitMusicPlayer(patches);
	SetMasterVolume(0xb0);
	SetTileTable(tileset);
	SetSpritesTileTable(spriteset);
	SetSpriteVisibility(true);

	// Init platz pointers
	PlatzSetTriggerCallback(ActivateTrigger);
	PlatzSetMovingPlatformTable(pgmPlatforms);
	PlatzSetMovingPlatformDirectory(pgmPlatformDir);
	PlatzSetAnimatedBgTable(bgAnimations);
	PlatzSetPatternTable(patterns);
	PlatzSetPatternDirectory(pgmPatDir);
	PlatzSetObjectTable(pgmObjects);
	PlatzSetInnerBgTable(pgmBgsInner);
	PlatzSetOuterBgTable(pgmBgsOuter);
	PlatzSetBgDirectory(pgmBgDir);

	// Local inits
	bob.sprite = SPRITE_BOB;
	bob.state = 0;
	SetForm(FORM_LYNX,DIR_RIGHT);
	
	// Init platz actor
	bob.pa.loc = (pt){LOC_BOB_X,LOC_BOB_Y};
	bob.pa.bbx = bob.anim.wid<<2;	// Set to half of animation's wid in pixels
	bob.pa.bby = bob.anim.hgt<<2;	// Set to half of animation's hgt in pixels
	bob.pa.sprx = (PLATZ_SCRN_WID>>1)-bob.pa.bbx;
	bob.pa.trLoc = (pt){0,0};		// Trigger loc can start at origin as Lynx is our largest player sprite
	bob.pa.vx.dir = DIR_RIGHT;
	bob.pa.vy.dir = DIR_DOWN;
	PlatzSetVelocity(&bob.pa.vx,0,&bob.pa.trLoc.x);
	PlatzSetVelocity(&bob.pa.vy,0,&bob.pa.trLoc.y);

	// Init platz scene
	PlatzSetMovingPlatformTiles(158,136,TILE_SKY);
	PlatzInit(&bob.pa,36);
	PlatzMoveToSlice(&bob.pa,sp);

	while(1) {
		if (GetVsyncFlag()) {
			ClearVsyncFlag();
			btnHeld = ReadJoypad(0);
        	btnPressed = btnHeld&(btnHeld^btnPrev);
        	btnReleased = btnPrev&(btnHeld^btnPrev);
			btnPrev = btnHeld;

			// Switch forms
			if (btnPressed&BTN_A) {
				if (bob.form == FORM_LYNX) {
					SetForm(FORM_DRAGONFLY,bob.pa.vx.dir);
				} else if (bob.form == FORM_DRAGONFLY) {
					SetForm(FORM_LYNX,bob.pa.vx.dir);
				}	
			}

			// Save/Load
			if (btnPressed&BTN_START) {
				BobSaveGame(PlatzGetCollisionPointer());
			} else if (btnPressed&BTN_SELECT) {
				u16 loadStatus = BobLoadGame();
				sp = loadStatus;

				if (sp != 0xff) {
					PlatzMoveToSlice(&bob.pa,sp);
					bob.form = 0;
					SetForm(loadStatus>>8,bob.pa.vx.dir);
				}
			}

			move &=~ btnReleased;
			move |= btnPressed;
			bob.animFlag = 0;			
			prevDir = bob.pa.vx.dir;
			bob.moveFunc(move);
			collFlag = PlatzMove(&bob.pa);

			switch(bob.form) {
				case FORM_DRAGONFLY:
					AerialAnimation(bob.pa.vx.dir,prevDir); break;
				case FORM_LYNX:
					TerrestrialAnimation(prevDir,move,collFlag); break;
				case FORM_TURTLE:
					AquaticAnimation(prevDir,move); break;
			}
			
			if (bob.animFlag&1) LoadBobAnimations(bob.pa.vx.dir);
			if (bob.animFlag&2) PlatzMapSprite(bob.sprite,bob.anim.wid,bob.anim.hgt,bob.anim.frames+bob.anim.currFrame*bob.anim.size);
			MoveSprite(bob.sprite,bob.pa.sprx-bob.pa.bbx,bob.pa.loc.y-bob.pa.bby+1,bob.anim.wid,bob.anim.hgt);
			PlatzTick();

			if (wfall) {
				if ((++wfallTmr&15) == 0) {
					TriggerFx(0,wfallVol,1);
				}
			}
		}
	}
}


