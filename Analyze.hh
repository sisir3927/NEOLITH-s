#ifndef ANALYZE_HH
#define ANALYZE_HH



#include "TArtReconstruction.hh"
#include "TArtEventStore.hh"
#include "TArtStoreManager.hh"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TVector3.h"
#include "TClonesArray.h"
#include "myTrack.hh"
#include "myGroups.hh"
#include "myDCHit.hh"
#include "myDCevt.hh"
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

		void FormHitArrays(bool force);
		void MakeDCHits();  // Group Wires, Create STC and Make Position
		void MakeGroups(); //Group the strips/wires in MakeDCHits 
		void PrepareForSTC();
		void MakeSTC();
		Double_t MakePositionCathode(myGroups* dcstripgroup, int l); //Input Should be Group
		Double_t CalcDriftLenPot(int tdc, int l);
		void MakeDCPosition(TClonesArray *KUgroups,TClonesArray *KVgrousp,  TVector3 *dcpos); // 
		void MakeTracks(); //Make Tracks and plot 
		void ReconstructSTC();
		void ReconstructTracks();
		
		void BookHistograms();
		void Clear();
		void Clearin();

		const myDCHitPara* FindDCHitPara(TArtRIDFMap *rmap) const;
		bool LoadParameters(const char *filexml);

		void CopyInputVariables();


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
		TClonesArray *fDCevts[2];

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



               const std::vector<int>* qdcorg_ch_in = nullptr;
                const  std::vector<int>* qdc_pmt_in= nullptr;
                 const std::vector<int>* tdc_ch_in= nullptr;
                 const std::vector<int>* tdc_pmt_in= nullptr;
                 const std::vector<int>* trail_tdc_ch_in= nullptr;
                 const std::vector<int>* qtc_pmt_in= nullptr;

                 const std::vector<double>* qtc_pla_in= nullptr;
                 const std::vector<double>* qdc_pla_in= nullptr;
                 const std::vector<double>* t_pla_in= nullptr;
                const  std::vector<double>* tdiff_pla_in= nullptr;

                 const std::vector<int>* tof_row_in= nullptr;
                const  std::vector<int>* tof_pattern_in= nullptr;

                 const std::vector<bool>* pla_bool_in= nullptr ;
                 const std::vector<bool>* pmt_bool_in= nullptr;
                 const std::vector<bool>* row_bool_in= nullptr;
                 const std::vector<bool>* pattern_bool_in= nullptr;
                 const std::vector<bool>* ref_trig_bool_in= nullptr;
                bool veto_bool_in;
                bool sbt_bool_in;
                 const std::vector<int>* ref_tdc_in= nullptr;
                int sbt_tdc_in;
                int veto_tdc_in;
                int incidence_tdc_in;
                int tof_incidence_in;
                const  std::vector<int>* tof_sbt_pla_in= nullptr;
                int incidence_pla_in;



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

		static constexpr int difftot_kumax_asa[2] = {10000,10000};
		static constexpr int difftot_kvmax_asa[2] = {10000,10000};
		static constexpr int difftot_kumax_gnd[2] = {3000,3000};
		static constexpr int difftot_kvmax_gnd[2] = {3000,3000};

		static constexpr int tdcprim_kpmax [2]= {00,0};
		static constexpr int tdcprim_kpmin [2]= {-2600,-2600};

		double stc_kv_l_a[2][difftot_kvmax_asa[0]+1];
		double stc_kv_l_g[2][difftot_kvmax_gnd[0]+1];;
		double stc_ku_l_a[2][difftot_kumax_asa[0]+1];;
		double stc_ku_l_g[2][difftot_kumax_gnd[0]+1];;
		double stc_kv_r_a[2][difftot_kvmax_asa[0]+1];;
		double stc_kv_r_g[2][difftot_kvmax_gnd[0]+1];;
		double stc_ku_r_a[2][difftot_kumax_asa[0]+1];;
		double stc_ku_r_g[2][difftot_kumax_gnd[0]+1];;
		double stc_kp[2][tdcprim_kpmax[0]-tdcprim_kpmin[0]+1];
		

		const int niter_stc = 50;

		const std::vector<double> y_pla={100,0,-100,100,0,-100};
		const std::vector<double> z_pla={-124.95,-124.95,-124.95,124.95,124.95,124.95};

		
		TH1* h_test;
		TH1* h_kv_totprim_s[2];
		TH1* h_ku_totprim_s[2];	
		TH1* h_kv_totprim_as[2];
		TH1* h_ku_totprim_as[2];
		TH1* h_kv_totprim_gs[2];
		TH1* h_ku_totprim_gs[2];

		TH1* h_kp_tdcprim_s[2];
		TH1* h_kp_tdcprim_gs[2];
		TH1* h_kp_tdcprim_as[2];
		TH1* h_kv_totid_s[2];
		TH1* h_ku_totid_s[2];
		TH1* h_kp_tdcid_s[2];
		TH1* h_kv_dtot_l_s[2];
		TH1* h_ku_dtot_l_s[2];
		TH1* h_kv_dtot_r_s[2];
		TH1* h_ku_dtot_r_s[2];
		TH1*  h_kp_dw_s[2]; 
		TH1* h_kv_dtot_l_gs[2];
		TH1* h_ku_dtot_l_gs[2];
		TH1* h_kv_dtot_r_gs[2];
		TH1* h_ku_dtot_r_gs[2];
		TH1*  h_kp_dw_gs[2]; 

		TH1* h_kv_dtot_l_as[2];
		TH1* h_ku_dtot_l_as[2];
		TH1* h_kv_dtot_r_as[2];
		TH1* h_ku_dtot_r_as[2];
		TH1*  h_kp_dw_as[2]; 
		

		TH1* h_driflen_kp_s[2];
		TH1* h_driflen_ku_l_as[2];
		TH1* h_driflen_ku_l_gs[2];
		TH1* h_driflen_ku_r_as[2];
		TH1* h_driflen_ku_r_gs[2];
		TH1* h_driflen_kv_l_as[2];
		TH1* h_driflen_kv_l_gs[2];
		TH1* h_driflen_kv_r_as[2];
		TH1* h_driflen_kv_r_gs[2];
	
	 	TH1 *h_npvertex_basic;	
		
		TH1* h_kp_tdcprim_corr_s[2];
		TH1* h_kv_dtot_l_corr_gs[2];
		TH1* h_ku_dtot_l_corr_gs[2];
		TH1* h_kv_dtot_r_corr_gs[2];
		TH1* h_ku_dtot_r_corr_gs[2];
		TH1* h_kv_dtot_l_corr_as[2];
		TH1* h_ku_dtot_l_corr_as[2];
		TH1* h_kv_dtot_r_corr_as[2];
		TH1* h_ku_dtot_r_corr_as[2];

		TFile* f;
		TTree* tree;
};
#endif
