#ifndef MYGROUPS_HH
#define MYGROUPS_HH

#include "TObject.h"
#include "TObjArray.h"
#include "myDCHit.hh"

class myGroups : public TObject {

public:
    myGroups() {
        farray.SetOwner(kFALSE);
        fstartID  = -1;
        fendID    = -1;
        fmaxtot   = -999999;
        fmaxtotid = -1;
        fmaxidx   = -1;
        fmintdcid = -1;
        fmintdc   = 999999;
        fpos      = -99999;
        fz        = -99999;
    }
    ~myGroups() = default;

    void SetPos(Double_t pos) { fpos = pos; }

    void Add(myDCHit *hit) {
        if (!hit) return; // Basic pointer safety check

        fz = hit->GetWirePosz();

        // Baseline setup on first hit
        if (farray.GetEntriesFast() == 0) {
            fstartID  = hit->GetWireNum();
            fmaxtot   = hit->GetToT();
            fmaxtotid = hit->GetWireNum();
            fmaxidx   = 0;

            fmintdc   = hit->GetTDC();
            fmintdcid = hit->GetWireNum();
        } else {
            Int_t current_tot  = hit->GetToT();
            Int_t current_tdc  = hit->GetTDC();
            Int_t current_wire = hit->GetWireNum();

            // 1. Is this the new maximum Time-over-Threshold (ToT)?
            if (current_tot > fmaxtot) {
                fmaxtot   = current_tot;
                fmaxtotid = current_wire;
                fmaxidx   = farray.GetEntriesFast(); // Store array index before adding
            }

            // 2. Is this the new minimum Drift Time (TDC)?
            if (current_tdc < fmintdc) {
                fmintdc   = current_tdc;
                fmintdcid = current_wire;
            }
        }

        fendID = hit->GetWireNum();
        farray.Add(hit);
    }

    Int_t GetSize() const { return farray.GetEntriesFast(); }

    // Safe cathode check using internal array indices
    Bool_t IsCathOk() const {
        Int_t n = farray.GetEntriesFast();
        return (fmaxidx > 0 && fmaxidx < n - 1);
    }

    Int_t GetMaxToTID() const { return fmaxtotid; }
    Int_t GetMaxToT() const   { return fmaxtot; }
    Int_t GetMinTDC() const   { return fmintdc; }
    Int_t GetMinTDCID() const { return fmintdcid; }

    Double_t GetPos() const   { return fpos; }
    Int_t GetStartID() const  { return fstartID; }
    Int_t GetEndID() const    { return fendID; }
    Double_t GetZ() const     { return fz; }

    myDCHit* At(Int_t i) const {
        return (myDCHit*) farray.At(i);
    }

    // Dynamic, safe computation of ToT left/right differences
    Double_t GetdToTL() const {
        if (!IsCathOk()) return -99999;
        Int_t lmaxtot = ((myDCHit*)farray.At(fmaxidx - 1))->GetToT();
        return static_cast<Double_t>(fmaxtot - lmaxtot);
    }

    Double_t GetdToTR() const {
        if (!IsCathOk()) return -99999;
        Int_t rmaxtot = ((myDCHit*)farray.At(fmaxidx + 1))->GetToT();
        return static_cast<Double_t>(fmaxtot - rmaxtot);
    }

	 Double_t GetratToTL() const {
        if (!IsCathOk()) return -99999;
        Int_t lmaxtot = ((myDCHit*)farray.At(fmaxidx - 1))->GetToT();
        return static_cast<Double_t>(1./fmaxtot*lmaxtot);
    }

    	Double_t GetratToTR() const {
        if (!IsCathOk()) return -99999;
        Int_t rmaxtot = ((myDCHit*)farray.At(fmaxidx + 1))->GetToT();
        return static_cast<Double_t>(1./fmaxtot*rmaxtot);
    }



private:
    TObjArray farray;
    Int_t fstartID;
    Int_t fendID;
    Int_t fmaxtot;
    Int_t fmaxtotid;
    Int_t fmaxidx; // Array index of the maximum ToT hit
    Int_t fmintdcid;
    Int_t fmintdc;
    Double_t fpos;
    Double_t fz;

    ClassDef(myGroups, 2); // Incremented schema version due to new member variable
};

#endif
