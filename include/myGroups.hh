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
			fmaxtot = -999999;
			fmaxtotid = -1;
			fmintdcid = -1;
			fmintdc = 99999;
			fpos = -99999;


		}
		~myGroups() = default;



		void SetPos(double_t pos){fpos =pos;};

		/*void Add(myDCHit * hit){

			if(farray.GetEntriesFast()==0)
				fstartID = hit->GetWireNum();


			fendID = hit->GetWireNum();


			farray.Add(hit);
			Int_t mintdc = hit->GetTDC();
			Int_t maxtot = hit->GetToT();
			Int_t mintdcid = hit->GetWireNum();
			Int_t maxtotid = hit->GetWireNum();

			if(maxtot> fmaxtot){

				fmaxtot = maxtot;
				fmaxtotid = maxtotid;
			}

			if(mintdc< fmintdc){

				fmintdc= mintdc;
				fmintdcid = mintdcid;
			}

		}*/

void Add(myDCHit *hit) {
    if (!hit) return; // Basic pointer safety check

    // If this is the very first hit in the cluster group
    if (farray.GetEntriesFast() == 0) {
        fstartID = hit->GetWireNum();
        
        // Establish the baseline using the first hit's actual properties
        fmaxtot   = hit->GetToT();
        fmaxtotid = hit->GetWireNum();
        
        fmintdc   = hit->GetTDC();
        fmintdcid = hit->GetWireNum();
    } 
    else {
        // For all subsequent hits, do comparison checks:
        Int_t current_tot = hit->GetToT();
        Int_t current_tdc = hit->GetTDC();
        Int_t current_wire = hit->GetWireNum();

        // 1. Is this the new maximum Time-over-Threshold (ToT)?
        if (current_tot > fmaxtot) {
            fmaxtot   = current_tot;
            fmaxtotid = current_wire;
        }

        // 2. Is this the new minimum Drift Time (TDC)?
        if (current_tdc < fmintdc) {
            fmintdc   = current_tdc;
            fmintdcid = current_wire;
        }
    }

    // Always update the trailing edge boundary and store the hit pointer
    fendID = hit->GetWireNum();
    farray.Add(hit);
}


		Int_t GetSize() const{

			return farray.GetEntriesFast();

		};

		Int_t  GetMaxToTID(){return fmaxtotid;}

		Int_t GetMaxToT(){return fmaxtot;}
		Int_t GetMinTDC(){return fmintdc;}

		Int_t GetMinTDCID(){return fmintdcid;}

		Double_t GetPos(){return fpos;};

		Int_t GetStartID() {return fstartID;}
		Int_t GetEndID() {return fendID;}


		myDCHit* At(Int_t i) const{

			return (myDCHit*) farray.At(i);
		}



	private:
		TObjArray farray;
		Int_t fstartID;
		Int_t fendID;
		Int_t fmaxtot;
		Int_t fmaxtotid;
		Int_t fmintdcid;
		Int_t fmintdc;
		Double_t fpos;



		ClassDef(myGroups,1);

};

#endif
