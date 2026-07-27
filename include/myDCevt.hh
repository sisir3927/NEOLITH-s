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
			fgrpid_u = -1;
			fgrpid_v = -1;
			fgrpid_x = -1;
			fpos.SetXYZ(fx,fy,fz);		

		};
		virtual	~myDCevt(){};


		void SetX(Double_t x){fx =x; 
			fpos.SetXYZ(fx,fy,fz);}
		void SetY(Double_t y){fy =y;

			fpos.SetXYZ(fx,fy,fz);}
		void SetZ(Double_t z){fz =z;
			fpos.SetXYZ(fx,fy,fz);}

		void SetXID(int id){fxid =id;}
		void SetVID(int id){fvid =id;}
		void SetUID(int id){fuid =id;}

		void SetPos(Double_t x, Double_t y, Double_t z){
			fx = x;
			fy = y;
			fz = z;
			fpos.SetXYZ(fx,fy,fz);
		}

		void SetVGrpID(Int_t id){fgrpid_v = id;}
		void SetUGrpID(Int_t id){fgrpid_u = id;}
		void SetXGrpID(Int_t id){fgrpid_x = id;}

		void SetVLrattot(Int_t value){ fvrattot_l = value; }
		void SetULrattot(Int_t value){ furattot_l = value; }
		void SetVRrattot(Int_t value){ fvrattot_r = value; }
		void SetURrattot(Int_t value){ furattot_r = value; }
		void SetXdtdc(Int_t value){ fxdifftdc = value; }

		void SetVLdtot(Int_t value){ fvdifftot_l = value; }
		void SetULdtot(Int_t value){ fudifftot_l = value; }
		void SetVRdtot(Int_t value){ fvdifftot_r = value; }
		void SetURdtot(Int_t value){ fudifftot_r = value; }
		void SetXdir(int k){fXdir = k;}

		Int_t GetVGrpID(){return fgrpid_v;}
		Int_t GetUGrpID(){return fgrpid_u;}
		Int_t GetXGrpID(){return fgrpid_x;}

		TVector3& GetPos(){return fpos;}


		Double_t GetX(){return fx;}
		Double_t GetY(){return fy;}
		Double_t GetZ(){return fz;}

		Int_t GetXID(){return fxid;}
		Int_t GetVID(){return fvid;}
		Int_t GetUID(){return fuid;}


		Int_t GetVLrattot(){return fvrattot_l;}
		Int_t GetULrattot(){return furattot_l;}
		Int_t GetVRrattot(){return fvrattot_r;}
		Int_t GetURrattot(){return furattot_r;}
		Int_t GetXdtdc(){return fxdifftdc;}
		
		Int_t GetVLdtot(){return fvdifftot_l;}
		Int_t GetULdtot(){return fudifftot_l;}
		Int_t GetVRdtot(){return fvdifftot_r;}
		Int_t GetURdtot(){return fudifftot_r;}
		Int_t GetXdir(){return fXdir;}

	private:

		Int_t fx;
		Int_t fy;
		Int_t fz;
		Int_t fxid; //These IDs are Universal ID, not special for the plane
		Int_t fvid;
		Int_t fuid;
		Int_t fgrpid_u;
		Int_t fgrpid_v;
		Int_t fgrpid_x;
		Int_t fvdifftot_l;
		Int_t fudifftot_l;
		Int_t fvdifftot_r;
		Int_t fudifftot_r;
		Int_t fxdifftdc;
		
		Int_t fvrattot_l;
		Int_t furattot_l;
		Int_t fvrattot_r;
		Int_t furattot_r;
		Int_t fXdir;

		TVector3 fpos;


		ClassDef(myDCevt,1);
};

#endif
