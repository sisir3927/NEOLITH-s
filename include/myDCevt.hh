#ifndef MYDCEVT_HH
#define MYDCEVT_HH
#include "TString.h"
#include "TVector3.h"
class myDCevt:public TObject{

	public:
		myDCevt(){
		

		fx =-99999;
		fy = -99999;
		fz = -99999;
		fpos.SetXYZ(fx,fy,fz);		
		
		};
	virtual	~myDCevt(){};
		

	void SetX(Double_t x){fx =x; 
	fpos.SetXYZ(fx,fy,fz);}
	void SetY(Double_t y){fy =y;
	
	fpos.SetXYZ(fx,fy,fz);}
	void SetZ(Double_t z){fz =z;
	fpos.SetXYZ(fx,fy,fz);}

	void SetPos(Double_t x, Double_t y, Double_t z){
	fx = x;
	fy = y;
	fz =z;
	fpos.SetXYZ(fx,fy,fz);
	}


	TVector3 GetPos(){return fpos;}
	Double_t GetX(){return fx;}
	Double_t GetY(){return fy;}
	Double_t GetZ(){return fz;}


	private:

		Int_t fx;
		Int_t fy;
		Int_t fz;
		TVector3 fpos;


	ClassDef(myDCevt,1);
};

#endif
