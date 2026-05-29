#ifndef MYDCPARAHIT_HH
#define MYDCPARAHIT_HH

#include "TArtMap.hh"

class myDCHitPara:public TObject{

	public:
		myDCHitPara(){};
		myDCHitPara(int i, TString n, Int_t wid, Double_t wp, Double_t wz,TString dr){
		id =i; detname =n; wireid = wid; wirepos = wp;  dir =dr;}
		~myDCHitPara(){};

		void SetParameters(Int_t i, TString n, Int_t wid, Double_t wp, Double_t wz, TString dr){
		
		id =i; detname =n; wireid = wid; wirepos = wp;  dir =dr;
		}
		void SetMap(Int_t tg, Int_t tc){
			fgeo =tg;
		map.SetMap(13,99,tg,tc);
		
		};


		Int_t GetID()const {return id;}
		TString *GetDetectorName(){return &detname;}
		Int_t GetWireID() const {return wireid;}
		Double_t GetWirePosition() const {return wirepos;}
		Double_t GetWireZPosition() const {return wirez;}
		TString *GetWireDirection(){return &dir;}
		Int_t GetGeo(){return fgeo;}
		


		TArtRIDFMap * GetTDCMap() { return &map;  } 

	private:
		TArtRIDFMap map;
		 
		Int_t id;
		TString detname;
	      	
		Int_t wireid;
		Double_t wirepos;
		Double_t wirez;
		TString dir;

		Int_t fgeo;
		

	ClassDef(myDCHitPara,1);	
//ClassDefInline(myDCHitPara,1);
};

#endif
