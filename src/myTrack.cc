
#include "myTrack.hh"
ClassImp(myTrack)
myTrack::myTrack(){

	fnpvertex = nullptr;

	fConvPos = nullptr;
	fCatPos = nullptr;
	fDC1HitPos = nullptr;
	fDC2HitPos = nullptr;

}
//////////////////////////////
TVector3* myTrack::GetnpVertex(bool resolution){

	if(fnpvertex){
		return fnpvertex;
	} else {
		if(!fConvPos || !fDC1HitPos) return nullptr;
		double z_int = fConvPos->Z();
		if(resolution) z_int = 0;
		fnpvertex = Extrapolate(fXang, fYang, fDC1HitPos, z_int);
		return fnpvertex;
	}

}
//////////////////////////////////
myTrack::~myTrack()
{
	delete fnpvertex;
}
///////////////////////////////
TVector3* myTrack::Extrapolate(double xang, double yang, TVector3* pos, double zint){

	if(!pos) return new TVector3(0,0,zint);

	TVector3 *fv = new TVector3();

	double x_int = pos->X() + TMath::Tan(xang)*(zint - pos->Z());
	double y_int = pos->Y() + TMath::Tan(yang)*(zint - pos->Z());

	fv->SetXYZ(x_int, y_int, zint);

	return fv;

}
///////////////////////////////
void myTrack::Clear(TVector3* pos){

pos = nullptr;

}
////

void myTrack::Clear(){

	fnpvertex = nullptr;
	fConvPos = nullptr;
	fCatPos = nullptr;
	fDC1HitPos = nullptr;
	fDC2HitPos = nullptr;

}

