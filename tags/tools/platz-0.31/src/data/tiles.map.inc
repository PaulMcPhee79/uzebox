#define mapMushroom_Width   (1)
#define mapMushroom_Height  (1)

const char mapMushroom[1 * 1 + 2] PROGMEM=
{mapMushroom_Width,mapMushroom_Height,
   29
};

#define mapPlant_Width   (2)
#define mapPlant_Height  (1)

const char mapPlant[2 * 1 + 2] PROGMEM=
{mapPlant_Width,mapPlant_Height,
   30, 31
};

#define mapPlantWater_Width   (2)
#define mapPlantWater_Height  (1)

const char mapPlantWater[2 * 1 + 2] PROGMEM=
{mapPlantWater_Width,mapPlantWater_Height,
   32, 33
};

#define mapWeedSml_Width   (3)
#define mapWeedSml_Height  (3)

const char mapWeedSml[3 * 3 + 2] PROGMEM=
{mapWeedSml_Width,mapWeedSml_Height,
   34, 35, 36,
   37, 38, 39,
   37, 38, 39
};

#define mapWeedLge_Width   (3)
#define mapWeedLge_Height  (7)

const char mapWeedLge[3 * 7 + 2] PROGMEM=
{mapWeedLge_Width,mapWeedLge_Height,
   34, 35, 36,
   37, 38, 39,
   37, 38, 39,
   37, 38, 39,
   37, 38, 39,
   37, 38, 39,
   37, 38, 39
};

#define mapSign_Width   (4)
#define mapSign_Height  (4)

const char mapSign[4 * 4 + 2] PROGMEM=
{mapSign_Width,mapSign_Height,
   40, 41, 42, 43,
   44, 45, 46, 47,
    0, 48, 49,  0,
    0, 50, 51,  0
};

#define mapPalm_Width   (4)
#define mapPalm_Height  (8)

const char mapPalm[4 * 8 + 2] PROGMEM=
{mapPalm_Width,mapPalm_Height,
   52, 53, 54, 55,
   56, 57, 58, 59,
   60, 61, 62, 63,
   64, 65, 66, 67,
    0, 68, 69,  0,
    0, 68, 69,  0,
    0, 68, 69,  0,
    0, 70, 71, 72
};

#define mapVine_Width   (2)
#define mapVine_Height  (4)

const char mapVine[2 * 4 + 2] PROGMEM=
{mapVine_Width,mapVine_Height,
   73, 74,
   75, 76,
   77, 78,
   79, 80
};

#define mapCloud_Width   (5)
#define mapCloud_Height  (3)

const char mapCloud[5 * 3 + 2] PROGMEM=
{mapCloud_Width,mapCloud_Height,
   81, 82, 83, 84, 85,
   86, 87, 88, 89, 90,
   91, 92, 93, 94, 95
};

const char animFire[2 * 6] PROGMEM={
   96, 97,
   98, 99,
  100,101,
  102,103,
  104,105,
  106,107
};

const char animWaterfallTop[2 * 4] PROGMEM={
  108,109,
  110,111,
  112,113,
  114,115
};

const char animWaterfallBtm[2 * 16] PROGMEM={
  116,117,
  118,119,
  120,121,
   17, 17,
  122,123,
  124,125,
  126,127,
   17, 17,
  128,129,
  130,131,
  132,133,
   17, 17,
  134,135,
  124,125,
  126,127,
   17, 17
};

const object pgmObjects[] PROGMEM = {
	{{4,11},mapSign},
	{{17,1},mapCloud},
	{{23,7},mapPalm},				// Slice 0 (3,3)	(count,total)
	{{5,27},mapPlantWater},
	{{22,27},mapPlantWater},
	{{5,2},mapCloud},
	{{25,1},mapCloud},				// Slice 1 (4,7)
	{{10,10},mapPalm},
	{{17,17},mapPlant},
	{{27,17},mapPlant},				// Slice 2 (3,10)
	{{4,21},mapWeedLge},
	{{15,27},mapPlantWater},
	{{25,21},mapWeedLge},			// Slice 6,7,8 (3,13)
	{{1,8},mapPlant},
	{{20,8},mapMushroom},			// Slice 9 (2,15)
	{{6,1},mapPalm},
	{{11,27},mapPlantWater},
	{{13,27},mapPlantWater},
	{{15,27},mapPlantWater},
	{{17,27},mapPlantWater},
	{{29,27},mapPlantWater},
	{{23,25},mapWeedSml},
	{{22,8},mapPlant},
	{{25,8},mapMushroom},			// Slice 10 (9,24)
	{{10,1},mapPalm},
	{{22,1},mapPalm},
	{{17,8},mapMushroom},
	{{20,27},mapPlantWater},
	{{24,25},mapWeedSml},			// Slice 11 (5,29)
	{{4,8},mapPlant},
	{{16,8},mapPlant},
	{{8,1},mapPalm},
	{{16,16},mapMushroom},
	{{2,23},mapVine},
	{{19,25},mapWeedSml},
	{{23,25},mapWeedSml},			// Slice 12 (7,36)
	{{2,1},mapPalm},
	{{6,1},mapPalm},
	{{10,1},mapPalm},
	{{18,1},mapPalm},
	{{22,1},mapPalm},
	{{26,1},mapPalm},
	{{15,16},mapMushroom},
	{{20,16},mapMushroom},
	{{11,23},mapVine},
	{{25,25},mapWeedSml},			// Slice 13 (10,46)
	{{15,8},mapPlant},
	{{16,27},mapPlantWater},
	{{18,27},mapPlantWater},
	{{26,25},mapWeedSml},			// Slice 14 (4,50)
	{{29,8},mapPlant},
	{{13,23},mapVine},
	{{28,23},mapVine},				// Slice 15 (3,53)
	{{9,1},mapCloud},
	{{0,27},mapPlantWater},
	{{2,27},mapPlantWater},
	{{4,27},mapPlantWater},
	{{6,27},mapPlantWater},
	{{8,27},mapPlantWater},
	{{10,27},mapPlantWater},
	{{12,27},mapPlantWater},
	{{14,27},mapPlantWater},
	{{16,27},mapPlantWater},
	{{18,27},mapPlantWater},
	{{20,27},mapPlantWater},		// Slice 16 (12,65)
	{{1,2},mapCloud},
	{{4,13},mapPlant},				// Slice 17 (2,67)
	{{8,1},mapCloud},				// Slice 18 (1,68)
	{{8,2},mapCloud},				// Slice 19 (1,69)
	{{3,1},mapCloud},
	{{6,15},mapPlant},
	{{18,22},mapVine},
	{{11,29},mapVine},
	{{14,27},mapPlantWater},
	{{16,27},mapPlantWater},
	{{18,27},mapPlantWater},
	{{20,27},mapPlantWater},
	{{22,27},mapPlantWater},
	{{24,27},mapPlantWater},
	{{26,27},mapPlantWater},
	{{28,27},mapPlantWater},
	{{30,27},mapPlantWater},		// Slice 20 (13,82)
	{{5,13},mapPlant},
	{{0,27},mapPlantWater},
	{{2,27},mapPlantWater},
	{{4,25},mapWeedSml},
	{{19,25},mapWeedSml},
	{{26,22},mapVine},				// Slice 21 (6,88)
	{{15,27},mapPlantWater},
	{{26,25},mapWeedSml},			// Slice 22 (2,90)
	{{5,13},mapPlant},
	{{22,19},mapPlant},
	{{5,22},mapVine},
	{{7,27},mapPlantWater},
	{{16,27},mapPlantWater},
	{{18,27},mapPlantWater},		// Slice 23 (6,96)
	{{21,3},mapCloud},
	{{8,23},mapPlant},				//Slice 24 (2,98)
	{{4,1},mapCloud},
	{{25,11},mapPlant},				// Slice 26 (2,100)
	{{5,4},mapPalm},				// Slice 27 (1,101)
	{{1,9},mapMushroom},			// Slice 28 (1,102)
	{{13,1},mapCloud},
	{{9,25},mapWeedSml},
	{{27,25},mapWeedSml},			// Slice 29 (3,105)
	{{16,1},mapCloud},
	{{13,25},mapWeedSml},			// Slice 30 (2,107)
	{{0,0},mapPalm},
	{{28,3},mapPalm},				// Slice 31 (2,109)
	{{29,10},mapPlant},				// Slice 32 (1,110)
	{{8,13},mapPalm},
	{{16,13},mapPalm},
	{{20,1},mapCloud},				// Slice 33 (3,113)
	{{5,3},mapCloud},
	{{25,2},mapCloud},
	{{5,18},mapPlant},
	{{21,11},mapPalm},				// Slice 34 (4,117)
	{{16,16},mapPlant},
	{{21,7},mapPalm},
	{{22,2},mapCloud}				// Slice 35 (3,120)
};

const patternDirectory pgmPatDir[] PROGMEM = {
	{2,2,0}
};
const char patterns[] PROGMEM = {
	TILE_STONE_TOP_LT,TILE_STONE_TOP_RT,TILE_STONE_BTM_LT,TILE_STONE_BTM_RT		// Pattern 0 - Stone
};

const platformDirectory pgmPlatformDir[] PROGMEM = {
	{0,2},		// Slice 10
	{2,1},		// Slice 18
	{3,1},		// Slice 19
	{4,1},		// Slice 21
	{5,2},		// Slice 22
	{7,1},		// Slice 23
	{8,1},		// Slice 24
	{9,2},		// Slice 25
	{11,1},		// Slice 26
	{12,1},		// Slice 27
	{13,1},		// Slice 28
	{14,1}		// Slice 30
};

// NB: Platform speeds must be a factor of 32
const platform pgmPlatforms[] PROGMEM = {
	{80,184,AXIS_Y,-2,{64,104,80,96}},
	{72,184,AXIS_Y,4,{200,224,160,176}},				// Slice 10
	{32,176,AXIS_X,2,{40,88,152,168}},					// Slice 18
	{32,112,AXIS_X,-4,{96,144,160,176}},				// Slice 19
	{40,136,AXIS_Y,1,{128,160,48,80}},					// Slice 21
	{40,136,AXIS_Y,-2,{32,64,128,160}},
	{40,136,AXIS_Y,4,{192,224,48,80}},					// Slice 22
	{40,136,AXIS_Y,-8,{96,128,96,128}},					// Slice 23
	{112,184,AXIS_X,-4,{144,184,192,200}},				// Slice 24
	{24,172,AXIS_Y,2,{32,80,128,136}},
	{48,136,AXIS_Y,4,{168,224,56,72}},					// Slice 25
	{32,112,AXIS_X,2,{40,88,96,112}},					// Slice 26
	{96,184,AXIS_X,-4,{168,208,56,72}},					// Slice 27
	{32,184,AXIS_X,8,{40,80,56,72}},					// Slice 28
	{64,104,AXIS_Y,2,{184,224,96,112}}					// Slice 30
};

const bgInner pgmBgsInner[] PROGMEM = {
	{0,TILE_GRASS,{0,31,15,16}},
	{0,TILE_GND,{0,31,16,28}},
	{0,TILE_GRASS_RT,{31,32,15,16}},
	{0,TILE_GND_RT,{31,32,16,18}},
	{0,TILE_GND_WAVE_RT,{31,32,18,19}},
	{0,TILE_GND_WATER_RT,{31,32,19,28}},				// Slice 0 (6,6)	(count,total)
	{0,TILE_WAVE,{0,32,18,19}},
	{0,TILE_WATER,{0,32,19,28}},						// Slice 1 (2,8)
	{0,TILE_WAVE,{0,7,18,19}},
	{0,TILE_WATER,{0,7,19,28}},
	{0,TILE_GRASS_WATER_LT,{7,8,18,19}},
	{0,TILE_GND_WATER_LT,{7,8,19,28}},
	{0,TILE_GRASS,{8,32,18,19}},
	{0,TILE_GND,{8,32,19,28}},
	{0,TILE_GND_SHARP_LT,{27,28,0,11}},
	{0,TILE_GND,{28,32,0,11}},
	{0,TILE_GND_SHARP_CNR_LT,{27,28,11,12}},
	{0,TILE_GND_SHARP_BTM,{28,32,11,12}},				// Slice 2 (10,18)
	{0,TILE_GND,{0,32,0,11}},
	{0,TILE_GND_SHARP_BTM,{0,32,11,12}},
	{0,TILE_GRASS,{0,32,18,19}},
	{0,TILE_GND,{0,32,19,28}},
	{0,TILE_SPIKE,{8,13,17,18}},						// Slice 3 (5,23)
	{BGA,0,{7,23,2,19}},													// NB: Tile acts as bgAnimation index when type == BGA
	{BGA,1,{7,23,19,23}},
	{0,TILE_WATER,{7,23,23,28}},
	{0,TILE_GND,{0,6,0,11}},
	{0,TILE_GND,{6,7,0,2}},
	{0,TILE_GND_SHARP_RT,{6,7,2,11}},
	{0,TILE_GND_SHARP_BTM,{0,6,11,12}},
	{0,TILE_GND_SHARP_CNR_RT,{6,7,11,12}},
	{0,TILE_GRASS,{0,6,18,19}},
	{0,TILE_GRASS_WATER_RT,{6,7,18,19}},
	{0,TILE_GND,{0,6,19,28}},
	{0,TILE_GND_WATER_RT,{6,7,19,28}},
	{0,TILE_GND,{7,23,0,1}},
	{0,TILE_GND_SHARP_BTM,{7,23,1,2}},
	{0,TILE_GND,{23,24,0,2}},
	{0,TILE_GND_SHARP_LT,{23,24,2,11}},
	{0,TILE_GND_SHARP_CNR_LT,{23,24,11,12}},
	{0,TILE_GND,{24,32,0,11}},
	{0,TILE_GND_SHARP_BTM,{24,32,11,12}},
	{0,TILE_GRASS_WATER_LT,{23,24,18,19}},
	{0,TILE_GRASS,{24,32,18,19}},
	{0,TILE_GND_WATER_LT,{23,24,19,28}},
	{0,TILE_GND,{24,32,19,28}},							// Slice 4 (23,46)
	{BGA,2,{11,17,16,18}},
	{0,TILE_GND,{0,9,0,11}},
	{0,TILE_GND_SHARP_BTM,{0,9,11,12}},
	{0,TILE_GND,{9,10,0,2}},
	{0,TILE_GND_SHARP_RT,{9,10,2,11}},
	{0,TILE_GND_SHARP_CNR_RT,{9,10,11,12}},
	{0,TILE_GRASS,{0,17,18,19}},
	{0,TILE_GND,{0,17,19,28}},
	{0,TILE_GND,{10,32,0,1}},
	{0,TILE_GND_SHARP_BTM,{10,32,1,2}},
	{0,TILE_GRASS_LT,{17,18,9,10}},
	{0,TILE_GRASS,{18,31,9,10}},
	{0,TILE_GND_LT,{17,18,10,18}},
	{0,TILE_GRASS_GND_RT,{17,18,18,19}},
	{0,TILE_GND,{17,18,19,28}},
	{0,TILE_GND,{18,31,10,28}},
	{0,TILE_GRASS_WATER_RT,{31,32,9,10}},
	{0,TILE_GND_WATER_RT,{31,32,10,28}},				// Slice 5 (18,64)
	{0,TILE_GND,{0,32,0,1}},
	{0,TILE_GND_SHARP_BTM,{0,32,1,2}},
	{0,TILE_WAVE,{0,32,9,10}},
	{0,TILE_WATER,{0,32,10,28}},						// Slice 6,7,8 (4,68)
	{BGC,TILE_GND,{0,5,0,1}},
	{BGC,TILE_GND_SHARP_RT,{5,6,0,1}},
	{BGC,TILE_GND_SHARP_BTM,{0,5,1,2}},
	{BGC,TILE_GND_SHARP_CNR_RT,{5,6,1,2}},
	{0,TILE_GRASS_WATER_LT,{0,1,9,10}},
	{0,TILE_GRASS,{1,5,9,10}},
	{0,TILE_GRASS_RT,{5,6,9,10}},
	{0,TILE_GND_WATER_LT,{0,1,10,28}},
	{0,TILE_GND,{1,5,10,28}},
	{0,TILE_GND_RT,{5,6,10,28}},
	{0,TILE_SPIKE,{6,15,27,28}},
	{0,TILE_GRASS_WATER_LT,{15,16,9,10}},
	{0,TILE_GRASS,{16,20,9,10}},
	{0,TILE_GRASS_RT,{20,21,9,10}},
	{0,TILE_GND_LT,{15,16,10,28}},
	{0,TILE_GND,{16,20,10,28}},
	{0,TILE_GND_RT,{20,21,10,28}},
	{0,TILE_SPIKE,{21,32,27,28}},						// Slice 9 (18,86)
	{0,TILE_GRASS_LT,{0,1,9,10}},
	{0,TILE_GRASS,{1,10,9,10}},
	{0,TILE_GRASS_WATER_RT,{10,11,9,10}},
	{0,TILE_GND_LT,{0,1,10,28}},
	{0,TILE_GND,{1,10,10,28}},
	{0,TILE_GND_WATER_RT,{10,11,10,28}},
	{0,TILE_WAVE,{11,19,9,10}},
	{0,TILE_WATER,{11,19,10,28}},
	{0,TILE_WATER,{19,32,23,28}},
	{BGC,TILE_GRASS_WATER_LT,{19,20,9,10}},
	{BGC,TILE_GRASS,{20,32,9,10}},
	{BGC,TILE_GND_WATER_LT,{19,20,10,22}},
	{BGC,TILE_GND,{20,32,10,22}},
	{BGC,TILE_GND_CNR_WATER_LT,{19,20,22,23}},
	{BGC,TILE_GND_SHARP_BTM,{20,32,22,23}},				// Slice 10 (15,101)
	{BGC,TILE_GRASS,{0,32,9,10}},
	{BGC,TILE_GND,{0,32,10,22}},
	{BGC,TILE_GND_SHARP_BTM,{0,32,22,23}},
	{0,TILE_WATER,{0,32,23,28}},						// Slice 11 (4,105)
	{0,TILE_GRASS,{0,5,9,10}},
	{0,TILE_GND,{0,5,10,13}},
	{0,TILE_GND,{0,4,13,22}},
	{0,TILE_GND_SHARP_RT,{4,5,13,22}},
	{0,TILE_GND_SHARP_BTM,{0,4,22,23}},
	{0,TILE_GND_SHARP_CNR_RT,{4,5,22,23}},
	{0,TILE_WAVE,{5,10,17,18}},
	{0,TILE_WATER,{5,10,18,23}},
	{0,TILE_WATER,{0,32,23,28}},
	{0,TILE_GRASS,{5,32,9,10}},
	{0,TILE_GND,{5,32,10,12}},
	{0,TILE_GND_SHARP_BTM,{5,32,12,13}},
	{0,TILE_GRASS_WATER_LT,{10,11,17,18}},
	{0,TILE_GRASS,{11,32,17,18}},
	{0,TILE_GND_WATER_LT,{10,11,18,22}},
	{0,TILE_GND,{11,32,18,22}},
	{0,TILE_GND_CNR_WATER_LT,{10,11,22,23}},
	{0,TILE_GND_SHARP_BTM,{11,32,22,23}},				// Slice 12 (18,123)
	{0,TILE_GRASS,{0,32,9,10}},
	{0,TILE_GND,{0,32,10,12}},
	{0,TILE_GND_SHARP_BTM,{0,32,12,13}},
	{0,TILE_GRASS,{0,32,17,18}},
	{0,TILE_GND,{0,32,18,22}},
	{0,TILE_GND_SHARP_BTM,{0,32,22,23}},
	{0,TILE_WATER,{0,32,23,28}},						// Slice 13,14 (7,130)
	{0,TILE_GRASS,{0,11,9,10}},
	{0,TILE_GND,{0,11,10,12}},
	{0,TILE_GND_SHARP_BTM,{0,11,12,13}},
	{0,TILE_GRASS,{0,2,17,18}},
	{0,TILE_GRASS_WATER_RT,{2,3,17,18}},
	{0,TILE_GND,{0,2,18,22}},
	{0,TILE_GND_WATER_RT,{2,3,18,22}},
	{0,TILE_GND_SHARP_BTM,{0,2,22,23}},
	{0,TILE_GND_CNR_WATER_RT,{2,3,22,23}},
	{0,TILE_WAVE,{3,11,17,18}},
	{0,TILE_WATER,{3,11,18,23}},
	{0,TILE_WATER,{0,32,23,28}},
	{0,TILE_GRASS,{11,31,9,10}},
	{0,TILE_GRASS_RT,{31,32,9,10}},
	{0,TILE_GND,{11,31,10,13}},
	{0,TILE_GND_RT,{31,32,10,16}},
	{0,TILE_GND_SHARP_LT,{11,12,13,22}},
	{0,TILE_GND,{12,31,13,22}},
	{BGC,TILE_GND_WAVE_RT,{31,32,16,17}},
	{BGC,TILE_GND_WATER_RT,{31,32,17,22}},
	{0,TILE_GND_SHARP_CNR_LT,{11,12,22,23}},
	{0,TILE_GND_SHARP_BTM,{12,31,22,23}},
	{0,TILE_GND_CNR_WATER_RT,{31,32,22,23}},			// Slice 15 (23,153)
	{0,TILE_WAVE,{0,22,16,17}},
	{0,TILE_WATER,{0,22,17,28}},
	{BGP,0,{22,28,16,28}},													// NB: Tile acts as patterns index when type == BGP
	{BGP,0,{28,32,14,28}},								// Slice 16 (4,157)
	{BGP,0,{0,26,14,28}},
	{BGP,0,{26,32,16,28}},								// Slice 17 (2,159)
	{BGA,2,{0,32,22,24}},
	{BGP,0,{0,32,24,28}},								// Slice 18 (2,161)
	{BGA,2,{0,24,22,24}},
	{BGP,0,{0,24,24,28}},
	{BGP,0,{24,32,18,28}},								// Slice 19 (3,164)
	{BGP,0,{0,8,16,28}},
	{BGP,0,{8,16,14,22}},
	{BGP,0,{8,14,22,28}},
	{BGP,0,{16,24,12,22}},
	{0,TILE_WATER,{14,32,22,28}},
	{BGP,0,{24,32,10,22}},								// Slice 20 (6,170)
	{BGP,0,{0,8,0,4}},
	{BGP,0,{0,4,10,22}},
	{BGP,0,{8,12,0,8}},
	{BGP,0,{4,12,14,22}},
	{BGP,0,{12,24,0,2}},
	{0,TILE_WAVE,{12,24,21,22}},
	{0,TILE_WATER,{0,32,22,28}},
	{BGP,0,{24,32,0,8}},
	{BGP,0,{24,32,14,22}},								// Slice 21 (9,179)
	{BGP,0,{0,12,0,2}},
	{0,TILE_WAVE,{0,12,21,22}},
	{BGP,0,{12,20,0,8}},
	{BGP,0,{12,20,14,22}},
	{0,TILE_WAVE,{20,32,21,22}},
	{0,TILE_WATER,{0,32,22,28}},
	{BGP,0,{20,32,0,2}},								// Slice 22 (7,186)
	{BGP,0,{0,8,0,8}},
	{BGP,0,{0,8,14,22}},
	{BGP,0,{8,20,0,2}},
	{0,TILE_WAVE,{8,20,21,22}},
	{0,TILE_WATER,{0,20,22,28}},
	{BGP,0,{20,32,0,8}},
	{BGP,0,{20,32,20,28}},								// Slice 23 (7,193)
	{BGP,0,{0,4,20,28}},
	{BGP,0,{4,10,24,28}},
	{0,TILE_SPIKE,{10,32,25,26}},
	{BGP,0,{10,32,26,28}},								// Slice 24 (4,197)
	{BGP,0,{0,32,26,28}},
	{0,TILE_SPIKE,{0,32,25,26}},						// Slice 25 (2,199)
	{BGP,0,{0,24,26,28}},
	{0,TILE_SPIKE,{0,24,25,26}},
	{BGP,0,{24,32,12,28}},								// Slice 26 (3,202)
	{0,TILE_GRASS,{0,12,12,13}},
	{0,TILE_GND,{0,12,13,28}},
	{0,TILE_GRASS_LT,{12,13,10,11}},
	{0,TILE_GND_LT,{12,13,11,12}},
	{0,TILE_GRASS_GND_RT,{12,13,12,13}},
	{0,TILE_GND,{12,13,13,28}},
	{0,TILE_GRASS,{13,32,10,11}},
	{0,TILE_GND,{13,32,11,28}},							// Slice 27 (8,210)
	{0,TILE_GRASS,{0,32,10,11}},
	{0,TILE_GND,{0,32,11,28}},							// Slice 28 (2,212)
	{0,TILE_GRASS,{0,1,10,11}},
	{0,TILE_GRASS_RT,{1,2,10,11}},
	{0,TILE_GND,{0,1,11,28}},
	{0,TILE_GND_RT,{1,2,11,15}},
	{0,TILE_GND_WAVE_RT,{1,2,15,16}},
	{0,TILE_GND_WATER_RT,{1,2,16,28}},
	{0,TILE_WAVE,{2,32,15,16}},
	{0,TILE_WATER,{2,32,16,28}},						// Slice 29 (8,220)
	{0,TILE_WAVE,{0,32,15,16}},
	{0,TILE_WATER,{0,32,16,28}},						// Slice 30 (2,222)
	{0,TILE_GRASS_LT,{0,1,8,9}},
	{0,TILE_GRASS,{1,19,8,9}},
	{0,TILE_GRASS_RT,{19,20,8,9}},
	{0,TILE_GND_LT,{0,1,9,15}},
	{0,TILE_GND_RT,{19,20,9,11}},
	{0,TILE_GND_WAVE_LT,{0,1,15,16}},
	{0,TILE_GND,{1,19,9,28}},
	{0,TILE_GRASS_GND_LT,{19,20,11,12}},
	{0,TILE_GND_WATER_LT,{0,1,16,28}},
	{0,TILE_GND,{19,20,12,28}},
	{0,TILE_GRASS,{20,32,11,12}},
	{0,TILE_GND,{20,32,12,24}},
	{0,TILE_GND,{20,25,24,25}},
	{0,TILE_GND_SHARP_BTM,{25,32,24,25}},
	{0,TILE_GND,{20,24,25,28}},
	{0,TILE_GND_SHARP_RT,{24,25,25,28}},
	{0,TILE_WATER,{25,32,25,28}},						// Slice 31 (17,239)
	{BGA,0,{4,20,13,21}},
	{BGA,1,{4,20,21,25}},
	{0,TILE_GRASS,{0,32,11,12}},
	{0,TILE_GND,{0,4,12,13}},
	{0,TILE_GND_SHARP_BTM,{4,20,12,13}},
	{0,TILE_GND,{20,32,12,13}},
	{0,TILE_GND,{0,3,13,24}},
	{0,TILE_GND_SHARP_RT,{3,4,13,24}},
	{0,TILE_GND_SHARP_LT,{20,21,13,24}},
	{0,TILE_GND,{21,32,13,24}},
	{0,TILE_GND_SHARP_BTM,{0,3,24,25}},
	{0,TILE_GND_SHARP_CNR_RT,{3,4,24,25}},
	{0,TILE_GND_SHARP_CNR_LT,{20,21,24,25}},
	{0,TILE_GND_SHARP_BTM,{21,30,24,25}},
	{0,TILE_GND,{30,32,24,25}},
	{0,TILE_WATER,{0,30,25,28}},
	{0,TILE_GND_SHARP_LT,{30,31,25,28}},
	{0,TILE_GND,{31,32,25,28}},						// Slice 32 (18,257)
	{0,TILE_GRASS,{0,4,11,12}},
	{0,TILE_GRASS_RT,{4,5,11,28}},
	{0,TILE_GND,{0,4,12,28}},
	{0,TILE_GND_RT,{4,5,12,21}},
	{0,TILE_GRASS_GND_LT,{4,5,21,22}},
	{0,TILE_GND,{4,5,22,28}},
	{0,TILE_GRASS,{5,32,21,22}},
	{0,TILE_GND,{5,32,22,28}},						// Slice 33 (8,265)
	{0,TILE_GRASS_LT,{0,1,19,20}},
	{0,TILE_GRASS,{1,32,19,20}},
	{0,TILE_GND_LT,{0,1,20,21}},
	{0,TILE_GRASS_GND_RT,{0,1,21,22}},
	{0,TILE_GND,{0,1,22,28}},
	{0,TILE_GND,{1,32,20,28}},						// Slice 34 (6,271)
	{0,TILE_GRASS,{0,9,19,20}},
	{0,TILE_GND,{0,9,20,28}},
	{0,TILE_GRASS_LT,{9,10,17,18}},
	{0,TILE_GRASS,{10,18,17,18}},
	{0,TILE_GND_LT,{9,10,18,19}},
	{0,TILE_GRASS_GND_RT,{9,10,19,20}},
	{0,TILE_GND,{9,10,20,28}},
	{0,TILE_GND,{10,18,18,28}},
	{0,TILE_GRASS_LT,{18,19,15,16}},
	{0,TILE_GRASS,{19,32,15,16}},
	{0,TILE_GND_LT,{18,19,16,17}},
	{0,TILE_GRASS_GND_RT,{18,19,17,18}},
	{0,TILE_GND,{18,19,18,28}},
	{0,TILE_GND,{19,32,16,28}}						// Slice 35 (14,285)
};

const bgOuter pgmBgsOuter[] PROGMEM = {
	{BGC,6,0,0,{0,32,15,28}},							// Slice 0 (1,1)	(count,total)
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{0,32,17,20}},							// NB: Index acts as trigger id when count == 0	
	{0,2,6,0,{0,32,18,28}},								// Slice 1 (2,3)
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{0,7,17,20}},							// NB: bgiCount acts as orientation flag for triggers (which way they will positive fire)
	{0,2,8,0,{0,7,18,28}},
	{BGC,4,10,0,{7,32,18,28}},
	{BGC,4,14,0,{27,32,0,12}},							// Slice 2 (4,7)
	{BGC,2,18,0,{0,32,0,12}},
	{BGC,2,20,0,{0,32,18,28}},
	{BGTV|BGI,ORI_LRUD,TRIG_WFALL0,0,{3,6,12,18}},
	{BGTV|BGI,ORI_LRUD,TRIG_WFALL1,0,{20,23,12,18}},
	{0,1,22,0,{8,13,17,18}},							// Slice 3 (5,12)
	{0,3,23,2,{7,23,0,28}},
	{BGC,5,26,0,{0,7,0,12}},
	{BGC,4,31,0,{0,7,18,28}},
	{BGTV|BGI,ORI_LRUD,TRIG_WFALL2,0,{3,6,12,18}},
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{7,23,20,23}},
	{BGTV|BGI,ORI_RLDU,TRIG_WFALL2,0,{25,28,12,18}},
	{BGC,2,35,0,{7,23,0,2}},
	{BGC,5,37,0,{23,32,0,12}},
	{BGC,4,42,0,{23,32,18,28}},							// Slice 4 (9,21)
	{0,1,46,1,{11,17,17,18}},
	{BGC,5,47,0,{0,10,0,12}},
	{BGC,2,52,0,{0,17,18,28}},
	{BGTV|BGI,ORI_RLDU,TRIG_WFALL1,0,{3,6,12,18}},
	{BGTV|BGI,ORI_RLDU,TRIG_WFALL0,0,{20,23,2,9}},
	{BGC,2,54,0,{10,32,0,2}},
	{BGC,8,56,0,{17,32,9,28}},							// Slice 5 (7,28)
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{0,32,8,11}},
	{BGC,2,64,0,{0,32,0,2}},
	{0,2,66,0,{0,32,9,28}},								// Slice 6,7,8 (3,31)
	{BGC,4,68,0,{0,6,0,2}},
	{BGC,6,72,0,{0,6,9,28}},
	{0,1,78,0,{6,15,27,28}},
	{BGC,6,79,0,{15,21,9,28}},							
	{0,1,85,0,{21,32,27,28}},							// Slice 9 (5,36)
	{BGC,6,86,0,{0,11,9,28}},
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{10,19,8,11}},
	{0,2,92,0,{11,19,9,28}},
	{0,1,94,0,{19,32,23,28}},
	{BGC,6,95,0,{19,32,9,23}},							// Slice 10 (5,41)
	{BGC,3,101,0,{0,32,9,23}},
	{0,1,104,0,{0,32,23,28}},							// Slice 11 (2,43)
	{BGC,6,105,0,{0,5,9,23}},
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{5,10,16,19}},
	{0,2,111,0,{5,10,17,23}},
	{0,1,113,0,{0,32,23,28}},
	{BGC,3,114,0,{5,32,9,13}},
	{BGC,6,117,0,{10,32,17,23}},						// Slice 12 (6,49)
	{BGC,3,123,0,{0,32,9,13}},
	{BGC,3,126,0,{0,32,17,23}},
	{0,1,129,0,{0,32,23,28}},							// Slice 13,14 (3,52)
	{BGC,3,130,0,{0,11,9,13}},
	{BGC,6,133,0,{0,3,17,23}},
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{3,11,16,19}},
	{0,2,139,0,{3,11,17,23}},
	{0,1,141,0,{0,32,23,28}},
	{BGC,11,142,0,{11,32,9,23}},						// Slice 15 (6,58)
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{0,22,15,18}},
	{0,2,153,0,{0,22,16,28}},
	{BGC,1,155,0,{22,28,16,28}},
	{BGC,1,156,0,{28,32,14,28}},						// Slice 16 (4,62)
	{BGC,1,157,0,{0,26,14,28}},
	{BGC,1,158,0,{26,32,16,28}},						// Slice 17 (2,64)
	{0,1,159,1,{0,32,23,24}},
	{BGC,1,160,0,{0,32,24,28}},							// Slice 18 (2,66)
	{0,1,161,1,{0,24,23,24}},
	{BGC,1,162,0,{0,24,24,28}},
	{BGC,1,163,0,{24,32,18,28}},						// Slice 19 (3,69)
	{BGC,1,164,0,{0,8,16,28}},
	{BGC,1,165,0,{8,16,14,22}},
	{BGC,1,166,0,{8,14,22,28}},
	{BGC,1,167,0,{16,24,12,22}},
	{0,1,168,0,{14,32,22,28}},
	{BGC,1,169,0,{24,32,10,22}},						// Slice 20 (6,75)
	{BGC,1,170,0,{0,8,0,4}},
	{BGC,1,171,0,{0,4,10,22}},
	{BGC,1,172,0,{8,12,0,8}},
	{BGC,1,173,0,{4,12,14,22}},
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{12,24,22,25}},
	{BGC,1,174,0,{12,24,0,2}},
	{0,1,175,1,{12,24,21,22}},
	{0,1,176,1,{0,32,22,28}},
	{BGC,1,177,0,{24,32,0,8}},
	{BGC,1,178,0,{24,32,14,22}},						// Slice 21 (10,85)
	{BGC,1,179,0,{0,12,0,2}},
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{0,12,20,23}},
	{0,1,180,1,{0,12,21,22}},
	{BGC,1,181,0,{12,20,0,8}},
	{BGC,1,182,0,{12,20,14,22}},
	{0,1,183,1,{20,32,21,22}},
	{0,1,184,1,{0,32,22,28}},
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{20,32,20,23}},
	{BGC,1,185,0,{20,32,0,2}},							// Slice 22 (9,94)
	{BGC,1,186,0,{0,8,0,8}},
	{BGC,1,187,0,{0,8,14,22}},
	{BGC,1,188,0,{8,20,0,2}},
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{8,20,20,23}},
	{0,1,189,1,{8,20,21,22}},
	{0,1,190,1,{0,20,22,28}},
	{BGC,1,191,0,{20,32,0,8}},
	{BGC,1,192,0,{20,32,20,28}},						// Slice 23 (8,102)
	{BGC,1,193,0,{0,4,20,28}},
	{BGC,1,194,0,{4,10,24,28}},
	{0,1,195,0,{10,32,25,26}},
	{BGC,1,196,0,{10,32,26,28}},						// Slice 24 (4,106)
	{BGC,1,197,0,{0,32,26,28}},
	{0,1,198,0,{0,32,25,26}},							// Slice 25 (2,108)
	{BGC,1,199,0,{0,24,26,28}},
	{0,1,200,0,{0,24,25,26}},
	{BGC,1,201,0,{24,32,12,28}},						// Slice 26 (3,111)
	{BGC,2,202,0,{0,12,12,28}},
	{BGC,6,204,0,{12,32,10,28}},						// Slice 27 (2,113)
	{BGC,2,210,0,{0,32,10,28}},							// Slice 28 (1,114)
	{BGC,6,212,0,{0,2,10,28}},
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{2,32,14,17}},
	{0,2,218,0,{2,32,15,28}},							// Slice 29 (3,117)
	{BGTH|BGI,ORI_LRUD,TRIG_WATER,0,{0,32,14,17}},
	{0,2,220,0,{0,32,15,28}},							// Slice 30 (2,119)
	{BGC,10,222,0,{0,20,8,28}},
	{BGTV|BGI,ORI_LRUD,TRIG_WFALL0,0,{3,6,0,7}},
	{BGTV|BGI,ORI_LRUD,TRIG_WFALL1,0,{17,20,0,11}},
	{BGC,7,232,0,{20,32,11,28}},						// Slice 31 (4,123)
	{BGC,18,239,2,{0,32,11,28}},						
	{BGTV|BGI,ORI_LRUD,TRIG_WFALL2,0,{3,6,0,11}},		
	{BGTV|BGI,ORI_RLDU,TRIG_WFALL2,0,{19,22,0,11}},		// Slice 32 (3,126)
	{BGC,6,257,0,{0,5,11,28}},
	{BGTV|BGI,ORI_RLDU,TRIG_WFALL1,0,{2,5,0,11}},
	{BGTV|BGI,ORI_RLDU,TRIG_WFALL0,0,{15,18,0,21}},
	{BGC,2,263,0,{5,32,21,28}},							// Slice 33 (4,130)
	{BGC,6,265,0,{0,32,19,28}},							// Slice 34 (1,131)
	{BGC,2,271,0,{0,9,19,28}},
	{BGC,6,273,0,{9,18,17,28}},
	{BGC,6,279,0,{18,32,15,28}}							// Slice 35 (3,134)
};

const bgDirectory pgmBgDir[] PROGMEM = {
	{0,3,0,1,1,0,1,0,PF_ZERO},				// Slice 0
	{3,4,1,2,1,0,1,0,PF_ZERO},				// Slice 1
	{7,3,3,4,1,2,2,0,PF_ZERO},				// Slice 2
	{10,0,7,5,2,0,2,0,PF_ZERO},				// Slice 3
	{10,0,12,9,3,7,2,2,PF_ZERO},			// Slice 4
	{10,0,21,7,3,5,2,1,PF_ZERO},			// Slice 5
	{10,3,28,3,2,0,2,0,PF_ZERO},			// Slice 6
	{10,3,28,3,2,0,2,0,PF_ZERO},			// Slice 7
	{10,3,28,3,2,0,2,0,PF_ZERO},			// Slice 8
	{13,2,31,5,2,0,0,0,0},					// Slice 9
	{15,9,36,5,1,4,1,0,PF_ZERO},			// Slice 10
	{24,5,41,2,1,0,1,0,PF_ZERO},			// Slice 11
	{29,7,43,6,1,4,2,0,PF_ZERO},			// Slice 12
	{36,10,49,3,2,0,2,0,PF_ZERO},			// Slice 13
	{46,4,49,3,2,0,2,0,PF_ZERO},			// Slice 14
	{50,3,52,6,2,5,1,0,PF_ZERO},			// Slice 15
	{53,12,58,4,1,3,1,0,PF_ZERO},			// Slice 16
	{65,2,62,2,1,1,1,0,PF_ZERO},			// Slice 17
	{67,1,64,2,2,1,1,1,1},					// Slice 18
	{68,1,66,3,2,2,1,1,2},					// Slice 19
	{69,13,69,6,1,5,1,0,PF_ZERO},			// Slice 20
	{82,6,75,10,2,8,2,0,3},					// Slice 21
	{88,2,85,9,2,7,2,0,4},					// Slice 22
	{90,6,94,8,2,6,2,0,5},					// Slice 23
	{96,2,102,4,1,3,1,0,6},					// Slice 24
	{98,0,106,2,1,0,1,0,7},					// Slice 25
	{98,2,108,3,1,2,1,0,8},					// Slice 26
	{100,1,111,2,1,1,1,0,9},				// Slice 27
	{101,1,113,1,1,0,1,0,10},				// Slice 28
	{102,3,114,3,1,2,1,0,PF_ZERO},			// Slice 29
	{105,2,117,2,1,0,1,0,11},				// Slice 30
	{107,2,119,4,1,3,1,0,PF_ZERO},			// Slice 31
	{109,1,123,3,1,0,1,2,PF_ZERO},			// Slice 32
	{110,3,126,4,1,3,1,0,PF_ZERO},			// Slice 33
	{113,4,130,1,1,0,1,0,PF_ZERO},			// Slice 34
	{117,3,131,3,1,2,1,0,PF_ZERO}			// Slice 35
};
