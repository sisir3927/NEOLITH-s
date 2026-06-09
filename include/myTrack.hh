#ifndef MYTRACK_HH
#define MYTRACK_HH

#include "TVector3.h"
#include "TMath.h"
#include "myDCevt.hh"
class myTrack:public TObject{

	public:
		myTrack();
	virtual	~myTrack();


	void Clear();
	void Clear(TVector3*  pos);

	void SetConvPos(Double_t x, Double_t y, Double_t z){fConvPos->SetXYZ(x,y,z);}
	void SetConvPos(TVector3* pos) {fConvPos =pos;}
	void SetConID(int id){fid1 = id;}
	void SetCatPos(Double_t x, Double_t y, Double_t z){fCatPos->SetXYZ(x,y,z);}
	void SetCatPos(TVector3* pos) {fCatPos =pos;}
	void SetCatID(int id){fid2 = id;}

	void SetDC1Hit(Double_t x, Double_t y, Double_t z){fDC1HitPos->SetXYZ(x,y,z);}
	void SetDC1Hit(TVector3* pos){fDC1HitPos = pos;}
	void SetDC2Hit(Double_t x, Double_t y, Double_t z){fDC2HitPos->SetXYZ(x,y,z);}
	void SetDC2Hit(TVector3* pos){fDC2HitPos = pos;}\
	void SetXAngle(Double_t ang){fXang = ang;}
	void SetYAngle(Double_t ang){fYang = ang;}
	void SetXdrift(Double_t drift){fXdrift = drift;}
	void SetUdrift(Double_t drift){fUdrift = drift;}
	void SetVdrift(Double_t drift){fVdrift = drift;}
	
	void SetDC1evt(myDCevt *eve){fdceve1 = eve;}
	void SetDC2evt(myDCevt *eve){fdceve2 = eve;}
	
	//void SetXAngle(Double_t ang){fXang = ang;}
	//void SetYAngle(Double_t ang){fYang = ang;}

	void Calibrate();

	TVector3* GetConvPos(){return fConvPos;}
	TVector3* GetCatPos(){return fCatPos;}

	TVector3* GetDC1HitPos(){return fDC1HitPos;}
	TVector3* GetDC2HitPos(){return fDC2HitPos;}

	myDCevt* GetDC1evt(){return fdceve1;}	
	myDCevt* GetDC2evt(){return fdceve1;}	


	Double_t  GetConvPos(int i) {
		if(i==0) return fConvPos->X();
		else if(i==1) return fConvPos->Y();
		else if(i==2) return fConvPos->Z();
		else return 0;
	}
	Double_t GetCatPos(int i) {
		if(i==0) return fCatPos->X();
		else if(i==1) return fCatPos->Y();
		else if(i==2) return fCatPos->Z();
		else return 0;
	}

	int GetCatID(){return fid2;}
	int GetConvID(){return fid1;}

	Double_t GetXAngle(){return fXang;}
	Double_t GetYAngle(){return fYang;}
	Double_t GetZAngle(){return fZang;}

	Double_t GetXdrift(){return fXdrift;}
	Double_t GetUdrift(){return fUdrift;}
	Double_t GetVdrift(){return fVdrift;}


	TVector3 GetnpVertex();	
	TVector3 GetpCatch(){return fpcatch;}	








	TVector3 Extrapolate(Double_t xang, Double_t yang, TVector3* pos, Double_t zint);

	private:

	TVector3 *fConvPos;
	TVector3 *fCatPos;
	TVector3 *fDC1HitPos;
	TVector3 *fDC2HitPos;
	TVector3 fnpvertex;
	TVector3 fpcatch;
	TVector3 *ftrackvec;

	myDCevt* fdceve1;
	myDCevt* fdceve2;

	int fid1;
	int fid2;

	Double_t fXang;
	Double_t fYang;
	Double_t fZang;

	Double_t fXdrift;
	Double_t fUdrift;
	Double_t fVdrift;





ClassDef(myTrack,1);

};

#endif
