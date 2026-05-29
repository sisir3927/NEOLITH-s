#ifndef ANALYZE_HH
#define ANALYZE_HH



#include "TArtReconstruction.hh"
#include "TArtEventStore.hh"
#include "TArtStoreManager.hh"
#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"
#include "TClonesArray.h"
#include "myTrack.hh"
#include "myGroups.hh"
#include "myDCHit.hh"
#include "myPlaHit.hh"
#include "myDCHitPara.hh"
#include "TXMLNode.h"
#include "TList.h"
#include "TArtMap.hh"
#include <TDOMParser.h>
#include <stdlib.h>
#include "TMath.h"

class Analyze{

	public:
		Analyze(){};
		Analyze(int nrun = 122, bool force= true);
		~Analyze();

		void LoadData();

		void FormHitArrays();
		void MakeDCHits();  // Group Wires, Create STC and Make Position
		void MakeGroups(); //Group the Cathode in MakeDCHits 
		void MakePositionCathode(TClonesArray* dcstripgroup,TClonesArray* cathodeposition); //Input Should be Group
		void MakeDCPosition(TClonesArray *KUgroups,TClonesArray *KVgrousp,  TVector3 *dcpos); // 
		void MakeTracks(); //Make Tracks and plot 
		void ReconstructSTC();
		void ReconstructTracks();

		void Clear();

		const myDCHitPara* FindDCHitPara(TArtRIDFMap *rmap) const;
		bool LoadParameters(const char *filexml);


		myDCHit *FindDCHit(Int_t id);


		void ParseParaList(TXMLNode* node);
		myDCHitPara* ParseDCHitPara(TXMLNode* node, std::map<TArtRIDFMap, myDCHitPara *> * map);

	private:


		TArtEventStore *estore;
        	TArtRawEventObject *rawevent;

		int fRunNum;
		TClonesArray *ftracks;
		TClonesArray *fDCHits;
		TClonesArray *fGroupHits;
		TClonesArray *fDCWireHits;
		TClonesArray *fPlaHits;
		std::vector<myDCHitPara*> fDCWireHitsPara;
		
		TObjArray *fDCKvHits[2];
		TObjArray *fDCKuHits[2];
		TObjArray *fDCKpHits[2];

		TObjArray *fDCKvGroups[2];
		TObjArray *fDCKuGroups[2];
		TObjArray *fDCKpGroups[2];



		std::map<TArtRIDFMap, myDCHitPara *> dc_pmap;
		TList* listOfDCHitPara;

		std::vector<int> qdcorg_ch;
		std::vector<int> qdc_pmt;
		std::vector<int> tdc_ch;
		std::vector<int> tdc_pmt;
		std::vector<int> trail_tdc_ch;
		std::vector<int> qtc_pmt;

		std::vector<double> qtc_pla;
		std::vector<double> qdc_pla;
		std::vector<double> t_pla;
		std::vector<double> tdiff_pla;

		std::vector<int> tof_row;
		std::vector<int> tof_pattern;

		std::vector<bool> pla_bool ;
		std::vector<bool> pmt_bool;
		std::vector<bool> row_bool;
		std::vector<bool> pattern_bool;
		std::vector<bool> ref_trig_bool;
		bool veto_bool;
		bool sbt_bool;
		std::vector<int> ref_tdc;
		int sbt_tdc;
		int veto_tdc;
		int incidence_tdc;
		int tof_incidence;
		std::vector<int> tof_sbt_pla;
		int incidence_pla;




		const int npmt = 12;
		const int npla = 6;
		const int nrow = 3;


		const int qtc_min = 0;
		const int qtc_max = 14000;
		const int qtc_bins = 100;
		const int tdc_min = -4000;
		const int tdc_max = -500;
		const int tdc_bins = 500;

		const int tof_min = -250;
		const int tof_max =50;
		const int tof_bins = 200;
		const int tof_sbt_min = -22500;
		const int tof_sbt_max = -16500;
		const int tof_sbt_bins = 500;



		const int qdc_max = 4400;
		const int qdc_min = 1;
		const int qdc_bins = static_cast<int>(1*(qdc_max-qdc_min+1));
		const int qdc_init = -1;
		const int tdc_init = -99999;
		const std::vector<int>qdc_ped =  {55,13,57,27,66,49,48,55,60,68,65,67};


		TH1* h_test;

};
#endif
