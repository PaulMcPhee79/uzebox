
const animation bobAnimations[] PROGMEM = {
	/* Lynx form */
	{6,3,2,1,0,0,0,animLynxIdleLt},
	{6,3,2,1,0,0,0,animLynxIdleRt},
	{6,3,2,6,0,0,4,animLynxRunLt},
	{6,3,2,6,0,0,4,animLynxRunRt},
	/* Dragonfly form */
	{4,2,2,4,0,0,4,animDFlyLt},
	{4,2,2,4,0,0,4,animDFlyRt},
	/* Turtle form */
	{2,2,1,3,0,0,8,animTurtleLt},
	{2,2,1,3,0,0,8,animTurtleRt}
};

const animation bgAnimations[] PROGMEM = {
	{2,2,1,4,0,0,8,animWaterfallTop},
	{8,2,4,4,0,0,8,animWaterfallBtm},
	{4,2,2,3,0,0,6,animFire}
};
