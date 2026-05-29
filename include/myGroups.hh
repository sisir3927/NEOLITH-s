#ifndef MYGROUPS_HH
#define MYGROUPS_HH

#include "TClonesArray.h"
#include "myDCHit.hh"

class myGroups:public TObject{

	public:
		myGroups()
		{
	
		farray.SetOwner(kFALSE);
		fstartID = -1;
		fendID = -1;
		
		}
		~myGroups() = default;


	//void FormGroups(TObjArray* dcwires, TClonesArray* dcwire_groups);
	
	//void SetPos(){};

	void Add(myDCHit * hit){
	
	if(farray.GetEntriesFast()==0)
		fstartID = hit->GetWireNum();


	fendID = hit->GetWireNum();


	farray.Add(hit);
	
	}
	



	int GetSize() const{
	
	return farray.GetEntriesFast();
	
	};
	
	/*int GetMaxToT(){};

	int GetMinTDC(){};

	int ImpID(){};

	double_t GetPos();*/

	Int_t GetStartID() {return fstartID;}
	Int_t GetEndID() {return fendID;}

	
	myDCHit* At(Int_t i) const{
	
	return (myDCHit*) farray.At(i);
	}
	
	
	
	private:
		TObjArray farray;
		Int_t fstartID;
		Int_t fendID;



	ClassDef(myGroups,1);

};

#endif
