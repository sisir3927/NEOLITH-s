#include "myTrack.hh"

myTrack::myTrack(){

fnpvertex = nullptr;


}
//////////////////////////////
myTrack::GetnpVertex(){

if(fnpVertex){

return fnpVertex;
}
else{

	double z_int = fConvPos->Z();
if(resolution) z_int = 
fnpvertex=Extralpolate(fXang,fYAng,fDC1Hitpos,fConvPos->Z());


}


}
///////////////////////////////
