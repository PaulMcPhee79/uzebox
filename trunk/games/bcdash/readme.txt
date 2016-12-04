To build, checkout bcdash folder and place Uzebox kernel V5 or newer in src directory
at top level. Alternatively, edit the makefile to point to the location of the kernel.

Additionally, make the following kernel changes in uzeboxSoundEngine.c:
- In the function "TriggerFx", change the first conditional block from this:

///////////////////////////////////////
if(type==1 || type==2){
	//noise or PCM channel fx
	channel=3;
}else if(tracks[1].priority==0 || (tracks[1].fxPatchNo==patch && tracks[1].priority>0 && retrig==true)){ //fx already playing
	channel=1;
}else if(tracks[2].priority==0 || (tracks[2].fxPatchNo==patch && tracks[2].priority>0 && retrig==true)){ //fx already playing				
	channel=2;
}else{
	//both channels have fx playing, use the oldest one
	if(tracks[1].patchPlayingTime>tracks[2].patchPlayingTime){
		channel=1;
	}else{
		channel=2;
	}
}
//////////////////////////////////////

to this:

//////////////////////////////////////
if(type==1 || type==2){
      //noise or PCM channel fx
        channel=3;
}else if(tracks[2].priority==0 || (tracks[2].fxPatchNo==patch && tracks[2].priority>0 && retrig==true)){ //fx already playing                           
        channel=2;
}else if(tracks[1].priority==0 || (tracks[1].fxPatchNo==patch && tracks[1].priority>0 && retrig==true)){ //fx already playing
        channel=1;
}else{
        //both channels have fx playing, use the oldest one
        if(tracks[1].patchPlayingTime>tracks[2].patchPlayingTime){
                channel=1;
        }else{
                channel=2;
        }
}
//////////////////////////////////////

This ensures that sound effects do not clobber the music track.
