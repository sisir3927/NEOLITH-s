#include <iostream>
#include "myTrack.hh"

myTrack::myTrack(){

	fnpvertex = TVector3(-99999,-99999,-99999);
	fpcatch = TVector3(-99999,-99999,-99999);
	ftrackvec = TVector3(-99999,-99999,-99999);

	fDCHitPos_val[0] = TVector3(-99999,-99999,-99999);
	fDCHitPos_val[1] = TVector3(-99999,-99999,-99999);
for(int l=0; l<2;l++){
	fXdtdc[l]  = -99999;	
	fULdtot[l]  = -99999;	
	fURdtot[l]  = -99999;	
	fVLdtot[l]  = -99999;	
	fVRdtot[l]  = -99999;	
}
	fConvPos = nullptr;
	fCatPos = nullptr;
	fDCHitPos[0] = nullptr;
	fDCHitPos[1] = nullptr;
	fdceve1 = nullptr;
	fdceve2 = nullptr;
	fXang = -99999;
	fYang = -99999;
	fZang = -99999;
	fid1 = -1;
	fid2 = -1;
}
//////////////////////////////
void myTrack::Calibrate(){

		if(!fDCHitPos[0] || !fDCHitPos[1]){
		
		if(!fdceve1 || !fdceve2){ std::cout<<"Cannot Calibrate"<<std::endl;return;}
				
		fDCHitPos[1] = &(fdceve2->GetPos());	
		fDCHitPos[0] = &(fdceve1->GetPos());	
		fDCHitPos_val[1] = (fdceve2->GetPos());	
		fDCHitPos_val[0] = (fdceve1->GetPos());	
	
		fXdtdc[0] = fdceve1->GetXdtdc();	
		fULdtot[0] = fdceve1->GetULdtot();	
		fURdtot[0] = fdceve1->GetURdtot();	
		fVLdtot[0] = fdceve1->GetVLdtot();	
		fVRdtot[0] = fdceve1->GetVRdtot();	
		fXdir[0] = fdceve1->GetXdir();

		fXdtdc[1] = fdceve2->GetXdtdc();	
		fULdtot[1] = fdceve2->GetULdtot();	
		fURdtot[1] = fdceve2->GetURdtot();	
		fVLdtot[1] = fdceve2->GetVLdtot();	
		fVRdtot[1] = fdceve2->GetVRdtot();	
		fXdir[1]   = fdceve2->GetXdir();
		
	
		}
		double z_int = fConvPos->Z();

	if(fDCHitPos[0] && fDCHitPos[1]){
		ftrackvec =  TVector3(fDCHitPos_val[1].X() - fDCHitPos_val[0].X(), fDCHitPos_val[1].Y() - fDCHitPos_val[0].Y(), fDCHitPos_val[1].Z() - fDCHitPos_val[0].Z());
		
		Double_t x = ftrackvec.X();
		Double_t y = ftrackvec.Y();
		Double_t z = ftrackvec.Z();
		fXang = TMath::ATan2(x,z);
		fYang = TMath::ATan2(y,z);
//		std::cout<<fXang<<" "<<fYang<<std::endl;
	//	std::cout<<fXang<<" "<<fDCHitPos_val[1].X()<<" " <<fDCHitPos_val[0].X() <<" "<<x<<std::endl;
		fZang = TMath::ATan2(TMath::Sqrt(x*x+y*y),z);
	if(fConvPos)
		fnpvertex = Extrapolate(fXang, fYang, fDCHitPos[0], z_int);
	
	if(fCatPos) 
		fpcatch = Extrapolate(fXang, fYang, fDCHitPos[0], fCatPos->Z());
	}
	else{
		std::cout<<"Not Calibrated"<<std::endl;
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
        ftrackvec = TVector3(-99999,-99999,-99999);
        fConvPos = nullptr;
        fCatPos = nullptr;
        fDCHitPos[0] = nullptr;
        fDCHitPos[1] = nullptr;
        fdceve1 = nullptr; // Reset these too!
        fdceve2 = nullptr;
}

