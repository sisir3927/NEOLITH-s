
#include "myTrack.hh"

myTrack::myTrack(){

	fnpvertex = nullptr;

	fConvPos = nullptr;
	fCatPos = nullptr;
	fDC1HitPos = nullptr;
	fDC2HitPos = nullptr;
	fvtrackvec = nullptr;
	fXang = 0;
	fYang = 0;
	fZang = 0;

}
//////////////////////////////
void myTrack::Calibrate(){

		if(!fConvPos || !fDC1HitPos) return;
		double z_int = fConvPos->Z();
		if(resolution) z_int = 0;
		fnpvertex = Extrapolate(fXang, fYang, fDC1HitPos, z_int);

	if(fDC1HitPos && fDC2HitPos){
		ftrackvec = new TVector3(fDC2HitPos->X() - fDC1HitPos->X(), fDC2HitPos->Y() - fDC1HitPos->Y(), fDC2HitPos->Z() - fDC1HitPos->Z());
		Double_t x = ftrackvec->X();
		Double_t y = ftrackvec->Y();
		Double_t z = ftrackvec->Z();
		fXang = TMath::ATan2(x,z);
		fYang = TMath::ATan2(y,z);
		fZang = TMath::ATan2(TMath::Sqrt(x*x+y*y),z);
	}

}
//////////////////////////////
TVector3* myTrack::GetnpVertex(bool resolution){

	if(fnpvertex){
		return fnpvertex;
	} else {Calibrate();
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

