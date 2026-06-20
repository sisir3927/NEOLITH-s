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

	void SetXID(Int_t l,int id){fxid[l] =id;}
	void SetVID(Int_t l,int id){fvid[l] =id;}
	void SetUID(Int_t l,int id){fuid[l] =id;}

	void SetDCHit(Int_t l, Double_t x, Double_t y, Double_t z){fDCHitPos_val[l].SetXYZ(x,y,z);fDCHitPos[l]->SetXYZ(x,y,z); }
	void SetDCHit(Int_t l, TVector3* pos){fDCHitPos[l] = pos;}
	void SetDCHit(Int_t l, TVector3 pos){fDCHitPos_val[l] = pos;fDCHitPos[l] = &fDCHitPos_val[l];}
	
	void SetXAngle(Double_t ang){fXang = ang;}
	void SetYAngle(Double_t ang){fYang = ang;}

	void SetXtdc(Int_t l, Int_t drift){fXdtdc[l] = drift;}
	void SetULdtot(Int_t l,Int_t drift){fULdtot[l] = drift;}
	void SetVLdtot(Int_t l,Int_t drift){fVLdtot[l] = drift;}
	void SetURdtot(Int_t l,Int_t drift){fURdtot[l] = drift;}
	void SetVRdtot(Int_t l,Int_t drift){fVRdtot[l] = drift;}


	void SetDC1evt(myDCevt *eve){fdceve1 = eve;}
	void SetDC2evt(myDCevt *eve){fdceve2 = eve;}

	void SetAsagiX(Int_t l,bool asa){fasagiX[l] =asa;}
	void SetAsagiV(Int_t l,bool asa){fasagiV[l] =asa;}
	void SetAsagiU(Int_t l,bool asa){fasagiU[l] =asa;}
	
	//void SetXAngle(Double_t ang){fXang = ang;}
	//void SetYAngle(Double_t ang){fYang = ang;}

	void Calibrate();

	TVector3* GetConvPos(){return fConvPos;}
	TVector3* GetCatPos(){return fCatPos;}

	TVector3 GetDCHitPos(Int_t l){return fDCHitPos_val[l];}

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

	Double_t GetXtdc(Int_t l){return fXdtdc[l];}
	Double_t GetULdtot(Int_t l){return fULdtot[l];}
	Double_t GetVLdtot(Int_t l){return fVLdtot[l];}
	Double_t GetURdtot(Int_t l){return fURdtot[l];}
	Double_t GetVRdtot(Int_t l){return fVRdtot[l];}



	Int_t GetXID(Int_t l){return fxid[l];}
	Int_t GetVID(Int_t l){return fvid[l];}
	Int_t GetUID(Int_t l){return fuid[l];}

	TVector3 GetnpVertex();	
	TVector3 GetpCatch(){return fpcatch;}
	TVector3 GetVect(){return ftrackvec;}

	Bool_t IsAsagiX(Int_t l){return fasagiX[l];}
	Bool_t IsAsagiV(Int_t l){return fasagiV[l];}
	Bool_t IsAsagiU(Int_t l){return fasagiU[l];}


	TVector3 Extrapolate(Double_t xang, Double_t yang, TVector3* pos, Double_t zint);

	private:

	TVector3 *fConvPos;
	TVector3 *fCatPos;
	TVector3 *fDCHitPos[2];
	TVector3 fDCHitPos_val[2];

	TVector3 fnpvertex;
	TVector3 fpcatch;
	TVector3 ftrackvec;

	myDCevt* fdceve1;
	myDCevt* fdceve2;

	int fid1;
	int fid2;

	int fxid[2];
	int fvid[2];
	int fuid[2];

	Double_t fXang;
	Double_t fYang;
	Double_t fZang;

	Int_t fXdtdc[2];
	Int_t fULdtot[2];
	Int_t fVLdtot[2];
	Int_t fURdtot[2];
	Int_t fVRdtot[2];

	Bool_t fasagiX[2];
	Bool_t fasagiV[2];
	Bool_t fasagiU[2];


ClassDef(myTrack,1);

};

#endif
