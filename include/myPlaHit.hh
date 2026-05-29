#ifndef MYPlaHIT_HH
#define MYPlaHIT_HH
#include "TString.h"
class myPlaHit:public TObject{

	public:
		myPlaHit(){};
	virtual	~myPlaHit(){};
		
		void SetID(int id){fID =id;}
		void SetDetName(TString nm){fname =nm;}
		void SetPlaNum(int wirenum){ fplanum = wirenum;}
		void SetTDC(int tdc){fleadedge = tdc;}

		void SetQTC(int qtc) {fqtc =qtc;}
		void SetQDC(int qtc) {fqdc =qtc;}


		void SetPos( int wpos){fwirepos = wpos;}
		void SetPosz(int wposz){fwireposz =wposz;}

		void SetDir(TString nm){fdir =nm;}
		
		int GetID(){return fID;}
		double GetPlaNum(){return fplanum;}
		int GetTDC(){return fleadedge;}
		int GetQTC() {return fqtc;}
		int  GetQDC(){return fqdc;}


		double GetPos() {return fwirepos;}
		double GetPosz() {return fwireposz;}

		TString *GetDetName(){return &fname;}
		TString *GetDir(){return &fdir;}



	private:

		int fID;
		int fplanum;
		int fleadedge;
		int fqtc;
		int fqdc;
		double fwirepos;
		double fwireposz;
		TString fname;
		TString fdir;

		const int ftdc_init = -99999;


	ClassDef(myPlaHit,1);
};

#endif
