#ifndef MYTRACK_HH
#define MYTRACK_HH

#include "TVector3.h"
#include "TMath.h"

class myTrack{

	public:
		myTrack();
		~myTrack();


	void Clear();

	void SetConvPos(double x, double y, double z){fConvPos->SetXYZ(x,y,z);}
	void SetConvPos(TVector3* pos) {fConvPos =pos;}
	void SetConID(int id){fid1 = id;}
	void SetCatPos(double x, double y, double z){fCatPos->SetXYZ(x,y,z);}
	void SetCatPos(TVector3* pos) {fCatPos =pos;}
	void SetCatID(int id){fid2 = id;}

	void SetDC1Hit(double x, double y, double z){fDC1HitPos->SetXYZ(x,y,z);}
	void SetDC1Hit(TVector3* pos){fDC1HitPos = pos;}
	void SetDC2Hit(double x, double y, double z){fDC2HitPos->SetXYZ(x,y,z);}
	void SetDC2Hit(TVector3* pos){fDC2HitPos = pos;}

	//void SetXAngle(double ang){fXang = ang;}
	//void SetYAngle(double ang){fYang = ang;}


	TVector3* GetConvPos(){return fConvPos;}
	TVector3* GetCatPos(){return fCatPos;}

	TVector3* GetDC1Hit(){return fDC1HitPos;}
	TVector3* GetDC2Hit(){return fDC2HitPos;}
	


	double  GetConvPos(int i) {
		if(i==0) return fConvPos->X();
		else if(i==1) return fConvPos->Y();
		else if(i==2) return fConvPos->Z();
		else return 0;
	}
	double GetCatPos(int i) {
		if(i==0) return fCatPos->X();
		else if(i==1) return fCatPos->Y();
		else if(i==2) return fCatPos->Z();
		else return 0;
	}

	int GetCatID(){return fid2;}
	int GetConvID(){return fid1;}

	double GetXAngle(){return fXang;}
	double GetYAngle(){return fYang;}




	TVector3*  GetnpVertex(bool resolution=false);

	TVector3* Extrapolate(double xang, double yang, TVector3* pos, double zint);

	private:

	TVector3 *fConvPos;
	TVector3 *fCatPos;
	TVector3 *fDC1HitPos;
	TVector3 *fDC2HitPos;
	TVector3 *fnpvertex;

	int fid1;
	int fid2;

	double fXang;
	double fYang;





};

#endif
