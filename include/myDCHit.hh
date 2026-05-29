#ifndef MYDCHIT_HH
#define MYDCHIT_HH
#include "TString.h"
class myDCHit:public TObject{

	public:
		myDCHit(){};
	virtual	~myDCHit(){};
		
		void SetID(int id){fID =id;}
		void SetDetName(TString nm){fname =nm;}
		void SetWireNum(int wirenum){ fwirenum = wirenum;}
		void SetTDC(int tdc){fleadedge = tdc;}

		void SetToT(int tot) {ftot =tot;}

		void SetTrailTDC( int trailtdc){ftrailedge = trailtdc;}

		void SetWirePos( int wpos){fwirepos = wpos;}
		void SetWirePosz(int wposz){fwireposz =wposz;}

		void SetDir(TString nm){fdir =nm;}
		
		int GetID(){return fID;}
		int GetWireNum()const {return fwirenum;}
		int GetTDC(){return fleadedge;}
		int GetTrailTDC(){return ftrailedge;}
		int GetToT() {if (ftot!= ftdc_init) {return ftot;}else if (fleadedge!= ftdc_init && ftrailedge!= ftdc_init) {ftot = ftrailedge-fleadedge;} return ftot;		}

		double GetWirePos() {return fwirepos;}
		double GetWirePosz() {return fwireposz;}

		TString *GetDetName(){return &fname;}
		TString *GetDir(){return &fdir;}



		Bool_t IsSortable() const override
		{
		
			return kTRUE;
		}

		Int_t Compare(const TObject* obj) const override
		{
		
			const myDCHit* other =static_cast<const myDCHit*> (obj);

			if(fwirenum <other->GetWireNum())
				return -1;

			if(fwirenum >other->GetWireNum())
				return 1;

			return 0;
		
		}	


	private:

		int fID;
		int fwirenum;
		int fleadedge;
		int ftrailedge;
		int ftot;
		double fwirepos;
		double fwireposz;
		TString fname;
		TString fdir;

		const int ftdc_init = -99999;


	ClassDef(myDCHit,1);
};

#endif
