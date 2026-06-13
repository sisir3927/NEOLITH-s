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
	void SetConvID(int id){fid1 = id;}
	void SetCatPos(Double_t x, Double_t y, Double_t z){fCatPos->SetXYZ(x,y,z);}
	void SetCatPos(TVector3* pos) {fCatPos =pos;}
	void SetCatID(int id){fid2 = id;}

	void SetX1ID(int id){fx1id =id;}
	void SetV1ID(int id){fv1id =id;}
	void SetU1ID(int id){fu1id =id;}
	void SetX2ID(int id){fx2id =id;}
	void SetV2ID(int id){fv2id =id;}
	void SetU2ID(int id){fu2id =id;}

	void SetDC1Hit(Double_t x, Double_t y, Double_t z){fDC1HitPos_val.SetXYZ(x,y,z);fDC1HitPos->SetXYZ(x,y,z); }
	void SetDC1Hit(TVector3* pos){fDC1HitPos = pos;}
	void SetDC1Hit(TVector3 pos){fDC1HitPos_val = pos;fDC1HitPos = &fDC1HitPos_val;}
	void SetDC2Hit(Double_t x, Double_t y, Double_t z){fDC2HitPos->SetXYZ(x,y,z);}
	void SetDC2Hit(TVector3* pos){fDC2HitPos = pos;}
	void SetDC2Hit(TVector3 pos){fDC2HitPos_val = pos; fDC2HitPos = &fDC2HitPos_val;}
	void SetXAngle(Double_t ang){fXang = ang;}
	void SetYAngle(Double_t ang){fYang = ang;}

	void SetXdtdc(Int_t drift){fXdtdc = drift;}
	void SetULdtot(Int_t drift){fULdtot = drift;}
	void SetVLdtot(Int_t drift){fVLdtot = drift;}
	void SetURdtot(Int_t drift){fURdtot = drift;}
	void SetVRdtot(Int_t drift){fVRdtot = drift;}


	void SetDC1evt(myDCevt *eve){fdceve1 = eve;}
	void SetDC2evt(myDCevt *eve){fdceve2 = eve;}
	
	//void SetXAngle(Double_t ang){fXang = ang;}
	//void SetYAngle(Double_t ang){fYang = ang;}

	void Calibrate();

	TVector3* GetConvPos(){return fConvPos;}
	TVector3* GetCatPos(){return fCatPos;}

	TVector3 GetDC1HitPos(){return fDC1HitPos_val;}
	TVector3 GetDC2HitPos(){return fDC2HitPos_val;}

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

	Double_t GetXdtdc(){return fXdtdc;}
	Double_t GetULdtot(){return fULdtot;}
	Double_t GetVLdtot(){return fVLdtot;}
	Double_t GetURdtot(){return fURdtot;}
	Double_t GetVRdtot(){return fVRdtot;}



	Int_t GetX1ID(){return fx1id;}
	Int_t GetV1ID(){return fv1id;}
	Int_t GetU1ID(){return fu1id;}
	Int_t GetX2ID(){return fx2id;}
	Int_t GetV2ID(){return fv2id;}
	Int_t GetU2ID(){return fu2id;}

	TVector3 GetnpVertex();	
	TVector3 GetpCatch(){return fpcatch;}
	TVector3 GetVect(){return ftrackvec;}



	TVector3 Extrapolate(Double_t xang, Double_t yang, TVector3* pos, Double_t zint);

	private:

	TVector3 *fConvPos;
	TVector3 *fCatPos;
	TVector3 *fDC1HitPos;
	TVector3 *fDC2HitPos;
	TVector3 fDC1HitPos_val;
	TVector3 fDC2HitPos_val;

	TVector3 fnpvertex;
	TVector3 fpcatch;
	TVector3 ftrackvec;

	myDCevt* fdceve1;
	myDCevt* fdceve2;

	int fid1;
	int fid2;

	int fx1id;
	int fv1id;
	int fu1id;
	int fx2id;
	int fv2id;
	int fu2id;

	Double_t fXang;
	Double_t fYang;
	Double_t fZang;

	Double_t fXdtdc;
	Double_t fULdtot;
	Double_t fVLdtot;
	Double_t fURdtot;
	Double_t fVRdtot;




ClassDef(myTrack,1);

};

#endif
