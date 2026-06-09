#include <iostream>
#include "myTrack.hh"

myTrack::myTrack(){

	fnpvertex = TVector3(-99999,-99999,-99999);
	fpcatch = TVector3(-99999,-99999,-99999);

	fConvPos = nullptr;
	fCatPos = nullptr;
	fDC1HitPos = nullptr;
	fDC2HitPos = nullptr;
	ftrackvec = nullptr;
	fdceve1 = nullptr;
	fdceve2 = nullptr;
	fXang = 0;
	fYang = 0;
	fZang = 0;

}
//////////////////////////////
void myTrack::Calibrate(){

		if(!fDC2HitPos || !fDC1HitPos){
		
		if(!fdceve1 || !fdceve2){ std::cout<<"Cannot Calibrate"<<std::endl;return;}
				
		fDC2HitPos = &(fdceve2->GetPos());	
		fDC1HitPos = &(fdceve1->GetPos());	
		
		}
		double z_int = fConvPos->Z();

	if(fDC1HitPos && fDC2HitPos){
		ftrackvec = new TVector3(fDC2HitPos->X() - fDC1HitPos->X(), fDC2HitPos->Y() - fDC1HitPos->Y(), fDC2HitPos->Z() - fDC1HitPos->Z());
		Double_t x = ftrackvec->X();
		Double_t y = ftrackvec->Y();
		Double_t z = ftrackvec->Z();
		fXang = TMath::ATan2(x,z);
		fYang = TMath::ATan2(y,z);
		fZang = TMath::ATan2(TMath::Sqrt(x*x+y*y),z);
	if(fConvPos)
		fnpvertex = Extrapolate(fXang, fYang, fDC1HitPos, z_int);
	
	if(fCatPos) 
		fpcatch = Extrapolate(fXang, fYang, fDC1HitPos, fCatPos->Z());
	}

}
//////////////////////////////
TVector3 myTrack::GetnpVertex(){

	if(fnpvertex!=TVector3(-99999,-99999,-99999)){
		return fnpvertex;
	} else {Calibrate();
		return fnpvertex;

	}

}
//////////////////////////////////
myTrack::~myTrack()
{
}
///////////////////////////////
TVector3 myTrack::Extrapolate(double xang, double yang, TVector3* pos, double zint){


	TVector3 *fv = new TVector3();

	double x_int = pos->X() + TMath::Tan(xang)*(zint - pos->Z());
	double y_int = pos->Y() + TMath::Tan(yang)*(zint - pos->Z());
//	std::cout<<x_int<<std::endl;
	return TVector3(x_int, y_int, zint);

}
///////////////////////////////
void myTrack::Clear(TVector3* pos){

pos = nullptr;

}
////

void myTrack::Clear(){

	fnpvertex = TVector3(-99999,-99999,-99999);
	fpcatch = TVector3(-99999,-99999,-99999);
	fConvPos = nullptr;
	fCatPos = nullptr;
	fDC1HitPos = nullptr;
	fDC2HitPos = nullptr;

}

