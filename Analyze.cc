#include "Analyze.hh"
#include "iostream"

Analyze::Analyze(int nRun, bool force):
	fRunNum(nRun){

		dw_mean.assign(2, std::vector<double>(48, 0.0));
		std::ifstream in("dw_mean.csv");
		std::string line1;

		while (std::getline(in, line1)) {
			std::stringstream ss(line1);
			int i, j;
			double mean;
			char comma;

			ss >> i >> comma >> j >> comma >> mean;
			dw_mean[i-1][j] = mean;
		}

		in.close();




kv_corrections[0].assign(80,{0.,1.});
    kv_corrections[1].assign(80,{0.,1.});

    ku_corrections[0].assign(80,{0.,1.});
    ku_corrections[1].assign(80,{0.,1.});
std::ifstream fincorr("LinearCorrections.csv");

    std::string line;
    getline(fincorr,line);   // skip header

    while(getline(fincorr,line))
    {
        std::stringstream ss(line);

        int detector, strip;
        double a,b;
        std::string side,tmp;

        getline(ss,tmp,','); detector = stoi(tmp);
        getline(ss,side,',');
        getline(ss,tmp,','); strip = stoi(tmp);
        getline(ss,tmp,','); a = stod(tmp);
        getline(ss,tmp,','); b = stod(tmp);

        if(side=="KV")
            kv_corrections[detector-1][strip]={a,b};
        else
            ku_corrections[detector-1][strip]={a,b};
    }

fincorr.close();


		std::cout<<"Hello"<<endl;
		listOfDCHitPara = new TList();
		listOfDCHitPara->SetOwner();
		fDCWireHits = new TClonesArray("myDCHit", 100);
		fDCWireHits->SetOwner(kTRUE);
		fPlaHits = new TClonesArray("myPlaHit", 100);
		for(int l=0 ; l<2; l++){
			fDCKvHits[l] = new TObjArray(100);
			fDCKuHits[l] = new TObjArray(100);
			fDCKpHits[l] = new TObjArray(100);

			fDCKvHits[l]->SetOwner(kFALSE);
			fDCKuHits[l]->SetOwner(kFALSE);
			fDCKpHits[l]->SetOwner(kFALSE);


			fDCKvGroups[l] = new TObjArray(100);
			fDCKuGroups[l] = new TObjArray(100);
			fDCKpGroups[l] = new TObjArray(100);

			fDCKvGroups[l]->SetOwner(kTRUE);
			fDCKuGroups[l]->SetOwner(kTRUE);
			fDCKpGroups[l]->SetOwner(kTRUE);
			fDCevts[l] = new TClonesArray("myDCevt",100);
			fDCevts[l]->SetOwner(kTRUE);

		}

		ftracks = new TClonesArray("myTrack",100);
		ftracks->SetOwner(kTRUE);
		ftracks_cum = new TClonesArray("myTrack",100);
		ftracks_cum->SetOwner(kTRUE);


		LoadParameters("map/NEOLITHs.xml");
		int neve =0;

		if(force){
			TFile *fout1 = new TFile (Form("root/sisir/Ana_Data_%d_v000.root",nRun), "RECREATE");
			TTree* ftree = new TTree("Hit_Tree", "Data Hit Information");

			ftree->Branch("fDCWireHits",&fDCWireHits);
			ftree->Branch("eve",&neve);
			ftree->Branch("tdc_pmt",&tdc_pmt);
			ftree->Branch("tdc_ch",&tdc_ch);
			ftree->Branch("trail_tdc_ch",&trail_tdc_ch);
			ftree->Branch("qtc_pmt",&qtc_pmt);
			ftree->Branch("qdc_pmt",&qdc_pmt);
			ftree->Branch("qdc_pla",&qdc_pla);
			ftree->Branch("t_pla",&t_pla);
			ftree->Branch("tdiff_pla",&tdiff_pla);
			ftree->Branch("tof_row",&tof_row);
			ftree->Branch("tof_pattern",&tof_pattern);
			ftree->Branch("ref_trig_bool",&ref_trig_bool);
			ftree->Branch("pla_bool",&pla_bool);
			ftree->Branch("pattern_bool",&pattern_bool);
			ftree->Branch("pmt_bool",&pmt_bool);
			ftree->Branch("row_bool",&row_bool);
			ftree->Branch("veto_bool",&veto_bool);
			ftree->Branch("sbt_bool",&sbt_bool);
			ftree->Branch("tof_sbt_pla",&tof_sbt_pla);
			ftree->Branch("veto_tdc",&veto_tdc);
			ftree->Branch("sbt_tdc",&sbt_tdc);
			ftree->Branch("incidence_tdc",&incidence_tdc);
			ftree->Branch("tof_incidence",&tof_incidence);
			ftree->Branch("incidence_pla",&incidence_pla);
			ftree->Branch("ref_tdc",&ref_tdc);



			BookHistograms();

			estore = new TArtEventStore();
			estore->Open(Form("ridf/neotest%04d.ridf",nRun));
			rawevent = estore->GetRawEventObject();
			neve = 0;

			h_test= new TH1D("h_test","h_test",100,-0.5,8.5);
			h_test2= new TH1D("h_test2","h_test2",100,-0.5,8.5);
			while(estore->GetNextEvent()&&neve<=100000){
				std::cout << "\rProcessing Event: " << neve << std::flush;
				//Initialize


				Clear();
				LoadData();
				FormHitArrays(1);
				for(int l=0 ; l<2; l++){
					fDCKvHits[l]->Sort();
					fDCKuHits[l]->Sort();
					fDCKpHits[l]->Sort();
				}

				MakeGroups();
				PrepareForSTC();
				neve++;
				ftree->Fill();
				estore->ClearData();

			}//estore
			stc_make_kv =true;
			Clear();
			ClearSTC();
			MakeSTC();

			ftree->Write();
			fout1->Write();
			fout1->Close();
			delete fout1;
		}//force



		TFile *fin = new TFile (Form("root/sisir/Ana_Data_%d_v000.root",nRun), "READ");

		std::cout <<" READING INPUT File"<<std::endl;
		TTree *ftreein;
		fin->GetObject("Hit_Tree",ftreein);
		ftreein->SetBranchAddress("fDCWireHits",&fDCWireHits);
		ftreein->SetBranchAddress("eve",&neve);
		ftreein->SetBranchAddress("tdc_pmt",&tdc_pmt_in);
		ftreein->SetBranchAddress("tdc_ch",&tdc_ch_in);
		ftreein->SetBranchAddress("trail_tdc_ch",&trail_tdc_ch_in);
		ftreein->SetBranchAddress("qtc_pmt",&qtc_pmt_in);
		ftreein->SetBranchAddress("qdc_pmt",&qdc_pmt_in);
		ftreein->SetBranchAddress("qdc_pla",&qdc_pla_in);
		ftreein->SetBranchAddress("t_pla",&t_pla_in);
		ftreein->SetBranchAddress("tdiff_pla",&tdiff_pla_in);
		ftreein->SetBranchAddress("tof_row",&tof_row_in);
		ftreein->SetBranchAddress("tof_pattern",&tof_pattern_in);
		ftreein->SetBranchAddress("ref_trig_bool",&ref_trig_bool_in);
		ftreein->SetBranchAddress("pla_bool",&pla_bool_in);
		ftreein->SetBranchAddress("pattern_bool",&pattern_bool_in);
		ftreein->SetBranchAddress("pmt_bool",&pmt_bool_in);
		ftreein->SetBranchAddress("row_bool",&row_bool_in);
		ftreein->SetBranchAddress("veto_bool",&veto_bool_in);
		ftreein->SetBranchAddress("sbt_bool",&sbt_bool_in);
		ftreein->SetBranchAddress("tof_sbt_pla",&tof_sbt_pla_in);
		ftreein->SetBranchAddress("veto_tdc",&veto_tdc);
		ftreein->SetBranchAddress("sbt_tdc",&sbt_tdc);
		ftreein->SetBranchAddress("incidence_tdc",&incidence_tdc);
		ftreein->SetBranchAddress("tof_incidence",&tof_incidence_in);
		ftreein->SetBranchAddress("incidence_pla",&incidence_pla);
		ftreein->SetBranchAddress("ref_tdc",&ref_tdc_in);
		/*
		   for (int l = 0; l < 2; ++l) {
		   h_kp_tdcprim_s[l]   = (TH1I*)fin->Get(Form("h_kp_tdcprim_s%d", l+1));
		   h_kp_tdcprim_gs[l]  = (TH1I*)fin->Get(Form("h_kp_tdcprim_gnd_s%d", l+1));
		   h_kp_tdcprim_as[l]  = (TH1I*)fin->Get(Form("h_kp_tdcprim_asa_s%d", l+1));

		   h_kv_totid_s[l]     = (TH2I*)fin->Get(Form("h_kv_totid_s%d", l+1));
		   h_ku_totid_s[l]     = (TH2I*)fin->Get(Form("h_ku_totid_s%d", l+1));
		   h_kp_tdcid_s[l]     = (TH2I*)fin->Get(Form("h_kp_tdcid_s%d", l+1));

		   h_kv_dtot_l_s[l]    = (TH1I*)fin->Get(Form("h_kv_dtot_l_s%d", l+1));
		   h_ku_dtot_l_s[l]    = (TH1I*)fin->Get(Form("h_ku_dtot_l_s%d", l+1));
		   h_kv_dtot_r_s[l]    = (TH1I*)fin->Get(Form("h_kv_dtot_r_s%d", l+1));
		   h_ku_dtot_r_s[l]    = (TH1I*)fin->Get(Form("h_ku_dtot_r_s%d", l+1));

		   h_kv_dtot_l_gs[l]   = (TH1I*)fin->Get(Form("h_kv_dtot_l_gnd_s%d", l+1));
		   h_ku_dtot_l_gs[l]   = (TH1I*)fin->Get(Form("h_ku_dtot_l_gnd_s%d", l+1));
		   h_kv_dtot_r_gs[l]   = (TH1I*)fin->Get(Form("h_kv_dtot_r_gnd_s%d", l+1));
		   h_ku_dtot_r_gs[l]   = (TH1I*)fin->Get(Form("h_ku_dtot_r_gnd_s%d", l+1));

		   h_kv_dtot_l_as[l]   = (TH1I*)fin->Get(Form("h_kv_dtot_l_asa_s%d", l+1));
		   h_ku_dtot_l_as[l]   = (TH1I*)fin->Get(Form("h_ku_dtot_l_asa_s%d", l+1));
		   h_kv_dtot_r_as[l]   = (TH1I*)fin->Get(Form("h_kv_dtot_r_asa_s%d", l+1));
		   h_ku_dtot_r_as[l]   = (TH1I*)fin->Get(Form("h_ku_dtot_r_asa_s%d", l+1));

		   h_ku_dtot_as[l]     = (TH1I*)fin->Get(Form("h_ku_dtot_asa_s%d", l+1));
		   h_kv_dtot_as[l]     = (TH1I*)fin->Get(Form("h_kv_dtot_asa_s%d", l+1));
		   h_ku_dtot_gs[l]     = (TH1I*)fin->Get(Form("h_ku_dtot_gnd_s%d", l+1));
		   h_kv_dtot_gs[l]     = (TH1I*)fin->Get(Form("h_kv_dtot_gnd_s%d", l+1));
		   h_driflen_kp_s[l]      = (TH1D*)fin->Get(Form("h_driflen_kp_s%d", l+1));
		   h_driflen_kv_l_as[l]   = (TH1D*)fin->Get(Form("h_driflen_kv_l_as%d", l+1));
		   h_driflen_kv_l_gs[l]   = (TH1D*)fin->Get(Form("h_driflen_kv_l_gs%d", l+1));
		   h_driflen_kv_r_as[l]   = (TH1D*)fin->Get(Form("h_driflen_kv_r_as%d", l+1));
		   h_driflen_kv_r_gs[l]   = (TH1D*)fin->Get(Form("h_driflen_kv_r_gs%d", l+1));
		   h_driflen_ku_l_as[l]   = (TH1D*)fin->Get(Form("h_driflen_ku_l_as%d", l+1));
		   h_driflen_ku_l_gs[l]   = (TH1D*)fin->Get(Form("h_driflen_ku_l_gs%d", l+1));
		   h_driflen_ku_r_as[l]   = (TH1D*)fin->Get(Form("h_driflen_ku_r_as%d", l+1));
		   h_driflen_ku_r_gs[l]   = (TH1D*)fin->Get(Form("h_driflen_ku_r_gs%d", l+1));

		   }
		   */
		TFile *fout2 =new TFile("root/test2_v000.root","RECREATE");
		TTree* ftree2 = new TTree("Track_Tree", "Data Track Information");
		ftree2->Branch("ftracks",&ftracks);

		BookHistograms();

		nftracks = 0;
		for (Long64_t i=0; i<ftreein->GetEntries(); ++i) {
			std::cout << "\rProcessing Event: " << i << std::flush;
			ftreein->GetEntry(i);
			CopyInputVariables();
			FormHitArrays(0);
			for(int l=0 ; l<2; l++){
				fDCKvHits[l]->Sort();
				fDCKuHits[l]->Sort();
				fDCKpHits[l]->Sort();

				//              std::cout<<"HITs "<<fDCKvHits[l]->GetEntriesFast()<<" "<<fDCKuHits[l]->GetEntriesFast()<<" "<<fDCKvHits[l]->GetEntriesFast()<<std::endl;
			}

			MakeGroups();

			MakeDCHits();
			MakeTracks();
			if(ftracks->GetEntriesFast()>0)nftracks++;
			Clear();
		}

		std::cout<<"\n";
		std::cout<<"Total Tracks: "<<nftracks<<std::endl;

		//      fin->Close();
		fout2->Write();
		//      fout2->Close();
		//      delete fout2;

		h_npvertex_basic->Reset();
		h_npvertex_corr->Reset();
		h_dist_vertex->Reset();
		h_dxdy_vertex->Reset();
		h_iddy_vertex->Reset();
		h_npvertex_b1->Reset();
		h_npvertex_b2->Reset();
		h_XY_s[0]->Reset();
		h_XY_s[1]->Reset();
		h_XYcath_s[0]->Reset();
		h_XYcath_s[1]->Reset();
		h_driflen_dchit_s[0]->Reset();
		h_driflen_dchit_s[1]->Reset();
		h_driflen_dchit_sign_s[0]->Reset();
		h_driflen_dchit_sign_s[1]->Reset();
		h_driflen_dctrack_s[0]->Reset();
		h_driflen_dctrack_s[0]->Reset();
		h_driflen_dctrack_s[1]->Reset();

		h_driflen_kv_gs[0]->Reset();
		h_driflen_ku_gs[0]->Reset();
		h_driflen_kv_gs[1]->Reset();
		h_driflen_ku_gs[1]->Reset();

		h_alpha->Reset();
		h_beta->Reset();
		h_theta->Reset();
		h_alpha_drift[0]->Reset();
		h_alpha_drift[1]->Reset();
		h_alpha_x[0]->Reset();
		h_alpha_x[1]->Reset();
		h_beta_y[0]->Reset();
		h_beta_y[1]->Reset();
		h_ang_dist_vertex->Reset();
		h_ang_dx_vertex->Reset();
		h_ang_dy_vertex->Reset();
h_deltaXcath_shift_s[0]->Reset() ;
h_deltaXcath_s[0]->Reset() ;
h_deltaXcath_left_s[0]->Reset() ;
h_deltaXcath_right_s[0]->Reset() ;
h_deltaXcath_shift_s[1]->Reset() ;
h_deltaXcath_s[1]->Reset() ;
h_deltaXcath_left_s[1]->Reset() ;
h_deltaXcath_right_s[1]->Reset() ;


		fout2 = new TFile("root/test3_v000.root","RECREATE");
		ftree2 = new TTree("Track_Tree", "Data Track Information");
		ftree2->Branch("ftracks",&ftracks);

		BookHistograms();

		ReconstructSTC(1);

nftracks=0;
		std::cout<<"------------------------------------------------------------------"<<std::endl;
		for (Long64_t i=0; i<ftreein->GetEntries(); ++i) {
			ftreein->GetEntry(i);
			CopyInputVariables();
			FormHitArrays(0);
			for(int l=0 ; l<2; l++){
				fDCKvHits[l]->Sort();
				fDCKuHits[l]->Sort();
				fDCKpHits[l]->Sort();

				//      std::cout<<"HITs "<<fDCKvHits[l]->GetEntriesFast()<<" "<<fDCKuHits[l]->GetEntriesFast()<<" "<<fDCKvHits[l]->GetEntriesFast()<<std::endl;
			}

			MakeGroups();

			MakeDCHits();
			MakeTracks();
			if(ftracks->GetEntriesFast()>0)nftracks++;
			Clear();
		}
		std::cout<<"\n";
		std::cout<<"Total Tracks: "<<nftracks<<std::endl;

		h_npvertex_basic->Draw("colz");

		fin->Close();
		fout2->Write();



	}
///////////////////////////
Analyze::~Analyze(){

	Clear();

	delete listOfDCHitPara;
	;
}
///////////
///////////
void Analyze::LoadData(){

	for (int i=0;i<rawevent->GetNumSeg();i++){
		TArtRawSegmentObject *seg = rawevent->GetSegment(i);
		int device = seg->GetDevice();
		int fp = seg->GetFP();
		int detector = seg->GetDetector();
		int module = seg->GetModule();
		for(int j=0;j<seg->GetNumData();j++){
			TArtRawDataObject *d = seg->GetData(j);
			int geo = d->GetGeo();
			int ch = d->GetCh();
			int val = d->GetVal();
			int cat = d->GetCategoryID();
			int det = d->GetDetectorID();
			int id = d->GetDatatypeID();
			int edge = d->GetEdge();


			if(geo<20)continue;

			if(geo <24 &&ch ==127){if(edge==0) {ref_trig_bool[geo-20] =1; ref_tdc[geo-20]= val;}   }

			if((geo <23&& ch<127) || (geo==23&& ch<=80)){
				TArtRIDFMap mm(fp,99,geo,ch);
				myDCHitPara *para = const_cast<myDCHitPara*>(FindDCHitPara(&mm));

				if(NULL == para){
					TArtCore::Error(__FILE__,"Could not find myDCHitPara...: Dev:%d, %s", device, mm.GetMapInfo());
					continue;
				}
				else {
					TArtCore::Debug(__FILE__,"Find myDCHitPara...: Dev:%d, %s, : %s", device, mm.GetMapInfo(), para->GetDetectorName()->Data());
				}

				Int_t id = para->GetID();

				Int_t nhit = fDCWireHits->GetEntries();
				TArtCore::Debug(__FILE__,"Entries in DCHitArray: %d", nhit);
				myDCHit * hit = FindDCHit(id);


				if(NULL==hit){

					if(edge ==1){
						new ((*fDCWireHits)[nhit]) myDCHit();
						hit = (myDCHit *)fDCWireHits->At(nhit);
						hit->SetID(id);
						fDCWireHitsPara.push_back(para);
						hit->SetDetName(*(para->GetDetectorName()));
						hit->SetWireNum(para->GetWireID());
						hit->SetWirePos(para->GetWirePosition());
						hit->SetWirePosz(para->GetWireZPosition());
						hit->SetDir(*(para->GetWireDirection()));
						hit->SetIsAsagi(para->GetIsAsagi());
						hit->SetTDC(val);
					}

				}else if(edge ==0){hit->SetTrailTDC(val);}

				// set raw data
				/*if(detectorid == detector){
				  if(mm==*((TArtRIDFMap *)para->GetTDCMap()))
				  hit->SetTDC(val);
				  }*/



			}

			//QTC for Refernce Signal

			if(geo == 23 && ch>=97 &&ch<118 ) {

				ch -=96;
				if(ch == 12) ch =0;
				else if(ch ==13 ) ch = 4;
				else if(ch == 14) ch =5;
				else if (ch == 15) ch = 7;

				if (edge == 0 && tdc_ch[ch]== tdc_init) tdc_ch[ch] = val;
				else if (edge == 1 && tdc_ch[ch]!= tdc_init && trail_tdc_ch[ch]== tdc_init) trail_tdc_ch[ch]= val;

				if(ch == 16){veto_bool =1; veto_tdc =val;}
				else if(ch == 17){sbt_bool =1; sbt_tdc =val;}


			}
			/*
			   else if( geo == 26){

			   if (ch ==8) continue;
			   if(ch ==15) ch = 8;
			//      if(ch == 6) {ch =4;}
			//      else if(ch==4) {ch =6;}

			if(ch >npmt) continue;
			qdc_pmt[ch] = val-qdc_ped[ch];
			qdcorg_ch[ch] = val;
			}
			*/

		}


	}//SegData


}
/////////////////////////////////////////////////////////////////////////////////////////////////
void Analyze::CopyInputVariables() {
	// --- 1. Copy integer vectors ---
	if (tdc_pmt_in)        tdc_pmt        = *tdc_pmt_in;
	if (trail_tdc_ch_in)   trail_tdc_ch   = *trail_tdc_ch_in;
	if (qtc_pmt_in)        qtc_pmt        = *qtc_pmt_in;
	if (qdc_pmt_in)        qdc_pmt        = *qdc_pmt_in; // This addresses your example!
	if (tof_row_in)        tof_row        = *tof_row_in;
	if (tof_pattern_in)    tof_pattern    = *tof_pattern_in;
	if (ref_tdc_in)        ref_tdc        = *ref_tdc_in;
	if (tof_sbt_pla_in)    tof_sbt_pla    = *tof_sbt_pla_in;

	// --- 2. Copy double vectors ---
	if (qtc_pla_in)        qtc_pla        = *qtc_pla_in;
	if (qdc_pla_in)        qdc_pla        = *qdc_pla_in;
	if (t_pla_in)          t_pla          = *t_pla_in;
	if (tdiff_pla_in)      tdiff_pla      = *tdiff_pla_in;

	// --- 3. Copy boolean vectors ---
	if (pla_bool_in)       pla_bool       = *pla_bool_in;
	if (pmt_bool_in)       pmt_bool       = *pmt_bool_in;
	if (row_bool_in)       row_bool       = *row_bool_in;
	if (pattern_bool_in)   pattern_bool   = *pattern_bool_in;
	if (ref_trig_bool_in)  ref_trig_bool  = *ref_trig_bool_in;

	// --- 4. Copy standard variables (Direct assignment) ---
	veto_bool     = veto_bool_in;
	sbt_bool      = sbt_bool_in;
	sbt_tdc       = sbt_tdc_in;
	veto_tdc      = veto_tdc_in;
	incidence_tdc = incidence_tdc_in;
	tof_incidence = tof_incidence_in;
	incidence_pla = incidence_pla_in;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void Analyze::FormHitArrays(bool force){
	if(force){
		//      if (tdc_ch[31]!=tdc_init) {ref_tdc[4] = tdc_ch[31];ref_trig_bool[4] = 1;}



		for(int ich = 0; ich<31; ich++){

			if (tdc_ch[ich]== tdc_init || trail_tdc_ch[ich]== tdc_init) continue;

			if(ich<npmt) pmt_bool[ich] =1;


			tdc_ch[ich] -= ref_tdc[3];
			trail_tdc_ch[ich] -= ref_tdc[3];

		}


		for(int ipmt = 0 ; ipmt< npmt; ipmt++){

			if (!pmt_bool[ipmt]) continue;
			tdc_pmt[ipmt] = tdc_ch[ipmt];
			qtc_pmt[ipmt] = trail_tdc_ch[ipmt] - tdc_pmt[ipmt];
			/*                      h_t_ch[ipmt]->Fill(tdc_pmt[ipmt]);
						h_tid->Fill(ipmt,tdc_pmt[ipmt]);
						h_qtc_pmt[ipmt]->Fill(qtc_pmt[ipmt]);
						h_qtcid->Fill(ipmt,qtc_pmt[ipmt]);
						h_qdcid->Fill(ipmt,qdc_pmt[ipmt]);
						h_qdc_pmt[ipmt]->Fill(qdc_pmt[ipmt]);
						h_qdcorg_ch[ipmt]->Fill(qdcorg_ch[ipmt]);

*/

		}


		for(int ipla =0; ipla<npla; ipla++){
			if(ipla !=0) {   {if (tdc_pmt[ipla]==tdc_init|| tdc_pmt[ipla+npla]== tdc_init) continue;}

				pla_bool[ipla]=1;
				t_pla[ipla] = 0.5*(tdc_pmt[ipla]+tdc_pmt[ipla+npla]);
				tdiff_pla[ipla] = tdc_pmt[ipla]-tdc_pmt[ipla+npla];
				if(qtc_pmt[ipla]>0&&qtc_pmt[ipla+npla]>0)qtc_pla[ipla]= TMath::Sqrt(qtc_pmt[ipla]*qtc_pmt[ipla+6]);
				/*                      h_Qtdiff_pla[ipla]->Fill(tdiff_pla[ipla],qdc_pla[ipla]);
							h_qdc_pla[ipla]->Fill(qdc_pla[ipla]);
							h_qtc_pla[ipla]->Fill(qtc_pla[ipla]);
							h_tdiff_pla[ipla]->Fill(tdiff_pla[ipla]);
							h_t_pla[ipla]->Fill(t_pla[ipla]);

							if(sbt_bool){
							h_Qtofsbt_pla[ipla]->Fill(t_pla[ipla]-sbt_tdc,qdc_pla[ipla]);

							h_tdifftofsbt_pla[ipla]->Fill(t_pla[ipla]-sbt_tdc,tdiff_pla[ipla]);
							}
							*/

			}       else{
				if(tdc_pmt[ipla]== tdc_init) continue;


				pla_bool[ipla]=1;
				t_pla[ipla] = 0.5*(tdc_pmt[ipla]+tdc_pmt[ipla+npla]);
				qtc_pla[ipla] = qtc_pmt[ipla];

			}

		}

		for(int ipla=0; ipla<npla/2; ipla++){

			if(t_pla[ipla]==tdc_init) continue;

			if(tdc_pmt[ipla]<incidence_tdc) {incidence_tdc = tdc_pmt[ipla]; incidence_pla = ipla;}

		}
		if(sbt_bool){   tof_incidence = incidence_tdc - sbt_tdc;}
		//                      h_Qtof_in->Fill(tof_incidence,qdc_pla[incidence_pla]);}


		for(int irow =0; irow<nrow;irow++){
			if(t_pla[irow] != tdc_init && t_pla[irow+nrow]!=tdc_init){
				row_bool[irow] =1;
				tof_row[irow] = t_pla[irow+nrow]-t_pla[irow];
				/*                              h_tof_row[irow]->Fill(tof_row[irow]);
								h_Q1tof_row[irow]->Fill(tof_row[irow],qdc_pla[irow]);
								h_Q2tof_row[irow]->Fill(tof_row[irow],qdc_pla[irow+nrow]);
								h_Qsumtof_row[irow]->Fill(tof_row[irow],qdc_pla[irow+nrow]+qdc_pla[irow]);
								*/                      }

				for(int jrow = 0; jrow<nrow;jrow++){

					//      tof_pattern[];

				}

		}

}//Force

//Forming DC Hits
Int_t nDCHits = fDCWireHits->GetEntriesFast();
for (Int_t i = 0; i < nDCHits; i++) {
bool fill_pot = false;
	myDCHit *hit = (myDCHit*)fDCWireHits->At(i);
	int geo_t = fDCWireHitsPara[i]->GetGeo()-20;
	if(!ref_trig_bool[geo_t]) continue;
	if(force){
		hit->SetTDC(hit->GetTDC()-ref_tdc[geo_t]);
		hit->SetTrailTDC(hit->GetTrailTDC()-ref_tdc[geo_t]);
	}else {
		hit->SetTDC(hit->GetTDC());
		hit->SetTrailTDC(hit->GetTrailTDC());
	}

	if(hit->GetToT()<0) continue;
	if (hit->GetDetName()->Contains("s1")) {
		if (hit->GetDetName()->Contains("Kv")) {
			fDCKvHits[0]->Add(hit);
			if(cath_tot_corr){
			hit->SetToT(kv_corrections[0][hit->GetWireNum()].a + kv_corrections[0][hit->GetWireNum()].b*hit->GetToT())
	;		}
		}
		else if (hit->GetDetName()->Contains("Ku")) {

			fDCKuHits[0]->Add(hit);
			if(cath_tot_corr){
			hit->SetToT(ku_corrections[0][hit->GetWireNum()].a + ku_corrections[0][hit->GetWireNum()].b*hit->GetToT())
	;		}
		}

		else if (hit->GetDetName()->Contains("Pot")) {
			if(!hit->IsAsagi()){

				if (hit->GetTDC() >= tdcprim_kpmin_gnd[0] && hit->GetTDC() <= tdcprim_kpmax_gnd[0]) fill_pot = true;
			}
			else{

				if (hit->GetTDC() >= tdcprim_kpmin_asa[0] && hit->GetTDC() <= tdcprim_kpmax_asa[0])fill_pot = true;

			}

			if(fill_pot)    fDCKpHits[0]->Add(hit);

		}



	}
	else if (hit->GetDetName()->Contains("s2")) {

		if (hit->GetDetName()->Contains("Kv")) {

			if(cath_tot_corr){
			hit->SetToT(kv_corrections[1][hit->GetWireNum()].a + kv_corrections[1][hit->GetWireNum()].b*hit->GetToT())
	;		}
			fDCKvHits[1]->Add(hit);
		}
		else if (hit->GetDetName()->Contains("Ku")) {
			if(cath_tot_corr){
			hit->SetToT(ku_corrections[1][hit->GetWireNum()].a + ku_corrections[1][hit->GetWireNum()].b*hit->GetToT())
	;		}

			fDCKuHits[1]->Add(hit);
		}

		else if (hit->GetDetName()->Contains("Pot")) {
			if(!hit->IsAsagi()){

				if (hit->GetTDC() >= tdcprim_kpmin_gnd[0] && hit->GetTDC() <= tdcprim_kpmax_gnd[0]) fill_pot = true;
			}
			else{

				if (hit->GetTDC() >= tdcprim_kpmin_asa[0] && hit->GetTDC() <= tdcprim_kpmax_asa[0])fill_pot = true;

			}

			if(fill_pot)    fDCKpHits[1]->Add(hit);
		}



	}




}





}

///////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////

void Analyze::MakeGroups(){

	myGroups *grp = nullptr;
	for(int l=0; l<2;l++){

		//KV

		for (Int_t ihit = 0; ihit < fDCKvHits[l]->GetEntriesFast(); ihit++) {
			myDCHit *hit = (myDCHit *) fDCKvHits[l]->At(ihit);
			if (!hit) continue;

			int ngrps = fDCKvGroups[l]->GetEntriesFast();

			if (ngrps == 0 ||!grp) {

				grp = new myGroups();
				fDCKvGroups[l]->Add(grp);

				grp->Add(hit);
			}
			else if(TMath::Abs(hit->GetWireNum() - grp->GetEndID()) >= 2) {
				grp = new myGroups();
				fDCKvGroups[l]->Add(grp);

				grp->Add(hit);


			}
			else {
				grp->Add(hit);
			}
		}
		//Kv Grouping Complete

		grp =nullptr;

		for (Int_t ihit = 0; ihit < fDCKuHits[l]->GetEntriesFast(); ihit++) {
			myDCHit *hit = (myDCHit *) fDCKuHits[l]->At(ihit);
			if (!hit) continue;

			int ngrps = fDCKuGroups[l]->GetEntriesFast();

			if (ngrps == 0|| !grp) {

				grp = new myGroups();
				fDCKuGroups[l]->Add(grp);

				grp->Add(hit);
			}
			else if(TMath::Abs(hit->GetWireNum() - grp->GetEndID()) >= 2) {
				grp = new myGroups();
				fDCKuGroups[l]->Add(grp);

				grp->Add(hit);


			}

			else {
				grp->Add(hit);
			}
		}       //Ku Grouping Complete

		grp = nullptr;
		for (Int_t ihit = 0; ihit < fDCKpHits[l]->GetEntriesFast(); ihit++) {
			myDCHit *hit = (myDCHit *) fDCKpHits[l]->At(ihit);
			if (!hit) continue;

			int ngrps = fDCKpGroups[l]->GetEntriesFast();

			if (ngrps == 0 || !grp) {

				grp = new myGroups();
				fDCKpGroups[l]->Add(grp);

				grp->Add(hit);
			}
			else if(TMath::Abs(hit->GetWireNum() - grp->GetEndID()) >= 2){
				grp = new myGroups();
				fDCKpGroups[l]->Add(grp);

				grp->Add(hit);


			}

			else {
				grp->Add(hit);
			}
		}       //Kp Grouping Complete





	}//2 Layers

}
////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void Analyze::PrepareForSTC(){

	//Loop Over Groups
	myGroups *grp = nullptr;
	Int_t ngrps;
	for(int l=0;l<2;l++){

		//Kv
		ngrps =  fDCKvGroups[l]->GetEntriesFast();

		for(int ig=0; ig<ngrps; ig++ ){
			grp = (myGroups*)fDCKvGroups[l]->At(ig);
			Int_t mul = grp->GetSize();
			if(mul>=3&&grp->IsCathOk()){
				int maxid = -1;
				for(int ihit=0; ihit<mul; ihit++){
					Int_t wireid = grp->At(ihit)->GetWireNum();

					Int_t tot = grp->At(ihit)->GetToT();
					if(wireid==grp->GetMaxToTID()){maxid = ihit;}//break;
					h_kv_totid_s[l]->Fill(wireid,tot);
				}//ihit
				if(maxid==0||maxid==mul-1) continue;
				Int_t maxtot = grp->GetMaxToT();
				Int_t lefttot = grp->At(maxid-1)->GetToT();
				Int_t righttot = grp->At(maxid+1)->GetToT();

				h_kv_dtot_l_s[l]->Fill(maxtot-lefttot);
				h_kv_dtot_r_s[l]->Fill(maxtot-righttot);
				h_kv_totprim_s[l]->Fill(maxtot);

				if(grp->At(maxid)->IsAsagi()){
					{ h_kv_dtot_l_as[l]->Fill(maxtot-lefttot); h_kv_dtot_as[l]->Fill(maxtot-lefttot);}
					{  h_kv_dtot_r_as[l]->Fill(maxtot-righttot);  h_kv_dtot_as[l]->Fill(maxtot-righttot); }
					{ h_kv_rtot_l_as[l]->Fill(cath_rat_const/maxtot*lefttot);}
					{  h_kv_rtot_r_as[l]->Fill(cath_rat_const/(maxtot)*righttot);}
					if(lefttot<0) std::cout<<lefttot<<"\n";
					if(righttot<0) std::cout<<righttot<<"\n";
					h_kv_totprim_as[l]->Fill(maxtot);
				}
				else{
					{ h_kv_dtot_gs[l]->Fill(maxtot-lefttot); h_kv_dtot_l_gs[l]->Fill(maxtot-lefttot);}
					{   h_kv_dtot_r_gs[l]->Fill(maxtot-righttot);  h_kv_dtot_gs[l]->Fill(maxtot-righttot);}
					{ h_kv_rtot_l_gs[l]->Fill(cath_rat_const /maxtot*lefttot);}
					{  h_kv_rtot_r_gs[l]->Fill(cath_rat_const/(maxtot)*righttot);}
					h_kv_totprim_gs[l]->Fill(maxtot);
if(TMath::Abs(lefttot-righttot)/(double)(maxtot-std::min(lefttot,righttot))<0.1)h_kv_calib1_qid_s[l]->Fill(grp->GetMaxToTID(),maxtot); 
if(TMath::Abs(lefttot-righttot)/(double)(maxtot-std::min(lefttot,righttot))>0.95)h_kv_calib2_qid_s[l]->Fill(grp->GetMaxToTID(),maxtot); 
h_test->Fill(TMath::Abs(lefttot-righttot)/(double)(maxtot-std::max(lefttot,righttot)));
h_test2->Fill(TMath::Abs(lefttot-righttot)/(double)(maxtot-std::min(lefttot,righttot)));
					if(lefttot<0) std::cout<<lefttot<<"\n";
					if(righttot<0) std::cout<<righttot<<"\n";
				}


			}// mul>=3

		}//Loop over groups

		//Ku
		ngrps =  fDCKuGroups[l]->GetEntriesFast();
		for(int ig=0; ig<ngrps; ig++ ){
			grp = (myGroups*)fDCKuGroups[l]->At(ig);
			Int_t mul = grp->GetSize();
			if(mul>=3&&grp->IsCathOk()){
				int maxid = -1;
				for(int ihit=0; ihit<mul; ihit++){
					Int_t wireid = grp->At(ihit)->GetWireNum();
					Int_t tot = grp->At(ihit)->GetToT();
					if(wireid==grp->GetMaxToTID()){maxid = ihit;}//break;
					h_ku_totid_s[l]->Fill(wireid,tot);

				}//ihit
				if(maxid==0||maxid==mul-1) continue;
				Int_t maxtot = grp->GetMaxToT();
				Int_t lefttot = grp->At(maxid-1)->GetToT();
				Int_t righttot = grp->At(maxid+1)->GetToT();
				if(lefttot<0) std::cout<<lefttot<<"\n";
				if(righttot<0) std::cout<<righttot<<"\n";

				h_ku_dtot_l_s[l]->Fill(maxtot-lefttot);
				h_ku_dtot_r_s[l]->Fill(maxtot-righttot);
				h_ku_totprim_s[l]->Fill(maxtot);


				if(grp->At(maxid)->IsAsagi()){
					{  h_ku_dtot_l_as[l]->Fill(maxtot-lefttot); h_ku_dtot_as[l]->Fill(maxtot-lefttot);}
					{  h_ku_dtot_r_as[l]->Fill(maxtot-righttot);  h_ku_dtot_as[l]->Fill(maxtot-righttot); }
					{ h_ku_rtot_l_as[l]->Fill(cath_rat_const/maxtot*lefttot);}
					{  h_ku_rtot_r_as[l]->Fill(cath_rat_const/(maxtot)*righttot);}
					h_ku_totprim_as[l]->Fill(maxtot);
				}
				else{
					{ h_ku_dtot_gs[l]->Fill(maxtot-lefttot); h_ku_dtot_l_gs[l]->Fill(maxtot-lefttot);}
					{   h_ku_dtot_r_gs[l]->Fill(maxtot-righttot);  h_ku_dtot_gs[l]->Fill(maxtot-righttot);}
					h_ku_totprim_gs[l]->Fill(maxtot);
					{ h_ku_rtot_l_gs[l]->Fill(cath_rat_const/maxtot*lefttot);}
					{  h_ku_rtot_r_gs[l]->Fill(cath_rat_const/(maxtot)*righttot);}
if(TMath::Abs(lefttot-righttot)/(double)(maxtot-std::min(lefttot,righttot))<0.1)h_ku_calib1_qid_s[l]->Fill(grp->GetMaxToTID(),maxtot); 
if(TMath::Abs(lefttot-righttot)/(double)(maxtot-std::min(lefttot,righttot))>0.9)h_ku_calib2_qid_s[l]->Fill(grp->GetMaxToTID(),maxtot); 
				}

			}// mul>=3

		}//Loop over groups

		//Kp
		ngrps =  fDCKpGroups[l]->GetEntriesFast();

		for(int ig=0; ig<ngrps; ig++ ){
			grp = (myGroups*)fDCKpGroups[l]->At(ig);
			Int_t maxid = -1;
			Int_t minid = -1;
			Int_t mul = grp->GetSize();
			for(int ihit=0; ihit<mul; ihit++){
				Int_t wireid = grp->At(ihit)->GetWireNum();
				Int_t tdc = grp->At(ihit)->GetTDC();
				if(wireid==grp->GetMaxToTID()){maxid = ihit;}//break;
				if(wireid==grp->GetMinTDCID()){minid = ihit;}//break;
				h_kp_tdcid_s[l]->Fill(wireid,tdc);
				if(mul<2|| ihit == mul-1) continue;

				h_kp_dw_s_cell[l][wireid]->Fill(grp->At(ihit)->GetToT()-grp->At(ihit+1)->GetToT());

			}//ihit

			Int_t maxtot = grp->At(minid)->GetToT();
			//              Int_t mintdc = grp->GetMinTDC();

			Int_t mintdc = grp->At(maxid)->GetTDC();
			Int_t max2tot = maxtot;

			if(grp->At(maxid)->IsAsagi()){

				h_kp_tdcprim_as[l]->Fill(mintdc);

			}else{
				h_kp_tdcprim_gs[l]->Fill(mintdc);
			}


			if(mul>=2){
				//if(maxid==0||maxid==mul-1) continue;
				//      if(minid != maxid) continue
				bool right = 0;
				if(minid == 0) {max2tot = grp->At(minid+1)->GetToT(); right =false;}
				else if(minid == mul-1) {max2tot = grp->At(minid-1)->GetToT(); right = true;}
				else if(grp->At(minid-1)->GetToT()<grp->At(minid+1)->GetToT()){max2tot = grp->At(minid-1)->GetToT(); right = false;}
				else {max2tot = grp->At(minid+1)->GetToT(); right =1;}

				if(mul == 2){
					if(grp->At(minid)->IsAsagi())
						h_kp_t1t2_as[l]->Fill( grp->At(0)-> GetTDC(), grp->At(1)->GetTDC());
					else
						h_kp_t1t2_gs[l]->Fill( grp->At(0)-> GetTDC(), grp->At(1)->GetTDC());


					if(grp->At(minid)->IsAsagi())
						h_kp_dtdw_as[l]->Fill( grp->At(1)->GetToT()-grp->At(0)->GetToT(),grp->At(0)->GetTDC()-grp->At(1)->GetTDC() );

					else

						h_kp_dtdw_gs[l]->Fill( grp->At(1)->GetToT()-grp->At(0)->GetToT(),grp->At(0)->GetTDC()-grp->At(1)->GetTDC() );

				}//mul ==2



				h_kp_dw_s[l]->Fill((maxtot-max2tot)*(2*(right-0.5)));
				h_kp_tdcprim_s[l]->Fill(mintdc);
				if(grp->At(minid)->IsAsagi()){
					h_kp_dw_as[l]->Fill((maxtot-max2tot)*(2*(right-0.5)));
				}
				else{h_kp_dw_gs[l]->Fill((maxtot-max2tot)*(2*(right-0.5)));
				}


			}// mul>=2

			if(mul ==1) {
				int     minid = 0;

				Int_t mintdc = grp->GetMinTDC();
				h_kp_tdcprim_s[l]->Fill(mintdc);

			} //mul==1

		}//Loop over groups






	} //l


}
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void Analyze::MakeSTC(){
	TH1* h =nullptr;
	double total_int_al = 0;
	double total_int_ar = 0;
	double total_int_gl = 0;
	double total_int_gr = 0;
	double total_kp = 0;
	double total_kp_a = 0;
	double total_kp_g = 0;

	for (int l=0; l<2; l++){

		//Kv
		//


		if(stc_make_kv){

			if(!cath_rat_method){

				h = h_kv_dtot_l_as[l];
				total_int_al = h->Integral(h->FindBin(0),h->FindBin(difftot_kvmax_asa[l]));
				h = h_kv_dtot_r_as[l];
				total_int_ar = h->Integral(h->FindBin(0),h->FindBin(difftot_kvmax_asa[l]));
				h = h_kv_dtot_l_gs[l];
				total_int_gl = h->Integral(h->FindBin(0),h->FindBin(difftot_kvmax_gnd[l]));
				h = h_kv_dtot_r_gs[l];
				total_int_gr = h->Integral(h->FindBin(0),h->FindBin(difftot_kvmax_gnd[l]));





				//ASAGI
				for(int ist = 0; ist<difftot_kvmax_asa[l]; ist++ ){
					h = h_kv_dtot_l_as[l];
					stc_kv_l_a[l][ist] = -((1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_al)))*(8.)-4);
					//      std::cout<<stc_kv_l_a[l][ist]<<std::endl;
					h_driflen_kv_l_as[l]->Fill(stc_kv_l_a[l][ist],h->GetBinContent(h->FindBin(ist)));
					h =h_kv_dtot_r_as[l];
					stc_kv_r_a[l][ist] = (1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_ar)))*(8.)-4;
					h_driflen_kv_r_as[l]->Fill(stc_kv_r_a[l][ist],h->GetBinContent(h->FindBin(ist)));

				}


				//GND
				for(int ist = 0; ist<difftot_kvmax_gnd[l]; ist++ ){
					h = h_kv_dtot_l_gs[l];
					stc_kv_l_g[l][ist] = -((1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_gl)))*(8.)-4);
					h_driflen_kv_l_gs[l]->Fill(stc_kv_l_g[l][ist],h->GetBinContent(h->FindBin(ist)));

					h = h_kv_dtot_r_gs[l];
					stc_kv_r_g[l][ist] = (1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_gr)))*(8.)-4;
					h_driflen_kv_r_gs[l]->Fill(stc_kv_r_g[l][ist],h->GetBinContent(h->FindBin(ist)));
				}



				//KU


				h = h_ku_dtot_l_as[l];
				total_int_al = h->Integral(h->FindBin(0),h->FindBin(difftot_kumax_asa[l]));

				h = h_ku_dtot_r_as[l];
				total_int_ar = h->Integral(h->FindBin(0),h->FindBin(difftot_kumax_asa[l]));
				h = h_ku_dtot_l_gs[l];
				total_int_gl = h->Integral(h->FindBin(0),h->FindBin(difftot_kumax_gnd[l]));
				h = h_ku_dtot_r_gs[l];
				total_int_gr = h->Integral(h->FindBin(0),h->FindBin(difftot_kumax_gnd[l]));


				//ASAGI
				for(int ist = 0; ist<difftot_kumax_asa[l]; ist++ ){
					h = h_ku_dtot_l_as[l];
					stc_ku_l_a[l][ist] = -(1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_al)))*(8.)+4;
					h_driflen_ku_l_as[l]->Fill(stc_ku_l_a[l][ist],h->GetBinContent(h->FindBin(ist)));

					h = h_ku_dtot_r_as[l];
					stc_ku_r_a[l][ist] = (1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_ar)))*(8.)-4;
					h_driflen_ku_r_as[l]->Fill(stc_ku_r_a[l][ist],h->GetBinContent(h->FindBin(ist)));

				}


				//GND
				for(int ist = 0; ist<difftot_kumax_gnd[l]; ist++ ){
					h = h_ku_dtot_l_gs[l];
					stc_ku_l_g[l][ist] = -(1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_gl)))*(8.)+4;
					h_driflen_ku_l_gs[l]->Fill(stc_ku_l_g[l][ist],h->GetBinContent(h->FindBin(ist)));

					h = h_ku_dtot_r_gs[l];
					stc_ku_r_g[l][ist] = (1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_gr)))*(8.)-4;
					h_driflen_ku_r_gs[l]->Fill(stc_ku_r_g[l][ist],h->GetBinContent(h->FindBin(ist)));
				}



			} else{ //cathratmethod

				h = h_kv_rtot_l_as[l];
				total_int_al = h->Integral(h->FindBin(0),h->FindBin(rattot_kvmax_asa[l]));
				h = h_kv_rtot_r_as[l];
				total_int_ar = h->Integral(h->FindBin(0),h->FindBin(rattot_kvmax_asa[l]));
				h = h_kv_rtot_l_gs[l];
				total_int_gl = h->Integral(h->FindBin(0),h->FindBin(rattot_kvmax_gnd[l]));
				h = h_kv_rtot_r_gs[l];
				total_int_gr = h->Integral(h->FindBin(0),h->FindBin(rattot_kvmax_gnd[l]));

				//ASAGI
				for(int ist = 0; ist<rattot_kvmax_asa[l]; ist++ ){
					h = h_kv_rtot_l_as[l];
					stc_kv_l_a[l][ist] = ((1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_al)))*(8.)-4);
					//      std::cout<<stc_kv_l_a[l][ist]<<std::endl;
					h_driflen_kv_l_as[l]->Fill(stc_kv_l_a[l][ist],h->GetBinContent(h->FindBin(ist)));
					h =h_kv_rtot_r_as[l];
					stc_kv_r_a[l][ist] = -((1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_ar)))*(8.)-4);
					h_driflen_kv_r_as[l]->Fill(stc_kv_r_a[l][ist],h->GetBinContent(h->FindBin(ist)));

				}


				//GND
				for(int ist = 0; ist<rattot_kvmax_gnd[l]; ist++ ){
					h = h_kv_rtot_l_gs[l];
					stc_kv_l_g[l][ist] = ((1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_gl)))*(8.)-4);
					h_driflen_kv_l_gs[l]->Fill(stc_kv_l_g[l][ist],h->GetBinContent(h->FindBin(ist)));

					h = h_kv_rtot_r_gs[l];
					stc_kv_r_g[l][ist] = -((1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_gr)))*(8.)-4);
					h_driflen_kv_r_gs[l]->Fill(stc_kv_r_g[l][ist],h->GetBinContent(h->FindBin(ist)));
				}




				h = h_ku_rtot_l_as[l];
				total_int_al = h->Integral(h->FindBin(0),h->FindBin(rattot_kumax_asa[l]));
				h = h_ku_rtot_r_as[l];
				total_int_ar = h->Integral(h->FindBin(0),h->FindBin(rattot_kumax_asa[l]));
				h = h_ku_rtot_l_gs[l];
				total_int_gl = h->Integral(h->FindBin(0),h->FindBin(rattot_kumax_gnd[l]));
				h = h_ku_rtot_r_gs[l];
				total_int_gr = h->Integral(h->FindBin(0),h->FindBin(rattot_kumax_gnd[l]));

				//ASAGI
				for(int ist = 0; ist<rattot_kumax_asa[l]; ist++ ){
					h = h_ku_rtot_l_as[l];
					stc_ku_l_a[l][ist] = ((1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_al)))*(8.)-4);
					//      std::cout<<stc_kv_l_a[l][ist]<<std::endl;
					h_driflen_ku_l_as[l]->Fill(stc_ku_l_a[l][ist],h->GetBinContent(h->FindBin(ist)));
					h =h_ku_rtot_r_as[l];
					stc_ku_r_a[l][ist] = -((1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_ar)))*(8.)-4);
					h_driflen_ku_r_as[l]->Fill(stc_ku_r_a[l][ist],h->GetBinContent(h->FindBin(ist)));

				}


				//GND
				for(int ist = 0; ist<rattot_kumax_gnd[l]; ist++ ){
					h = h_ku_rtot_l_gs[l];
					stc_ku_l_g[l][ist] = ((1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_gl)))*(8.)-4);
					h_driflen_ku_l_gs[l]->Fill(stc_ku_l_g[l][ist],h->GetBinContent(h->FindBin(ist)));

					h = h_ku_rtot_r_gs[l];
					stc_ku_r_g[l][ist] = -((1- (h->Integral(h->FindBin(0),h->FindBin(ist))/(1.*total_int_gr)))*(8.)-4);
					h_driflen_ku_r_gs[l]->Fill(stc_ku_r_g[l][ist],h->GetBinContent(h->FindBin(ist)));
				}


			} //cathratmethod


		}//stc_make_kv

		//KP
		//


		h = h_kp_tdcprim_s[l];
		total_kp = h->Integral(h->FindBin(tdcprim_kpmin[l]),h->FindBin(tdcprim_kpmax[l]));
		for(int ist = 0; ist<tdcprim_kpmax[l]-tdcprim_kpmin[l]+1;ist++){
			int modbin = ist+tdcprim_kpmin[l];
			stc_kp[l][ist] =  (h->Integral(h->FindBin(tdcprim_kpmin[l]),h->FindBin(modbin)))/(1.*total_kp)*(8.);
			h_driflen_kp_s[l]->Fill(stc_kp[l][ist],h->GetBinContent(h->FindBin(modbin)));

		}

		//Asagi
		h = h_kp_tdcprim_as[l];
		total_kp_a = h->Integral(h->FindBin(tdcprim_kpmin_asa[l]),h->FindBin(tdcprim_kpmax_asa[l]));
		for(int ist = 0; ist<tdcprim_kpmax_asa[l]-tdcprim_kpmin_asa[l]+1;ist++){
			int modbin = ist+tdcprim_kpmin_asa[l];
			stc_kp_a[l][ist] =  (h->Integral(h->FindBin(tdcprim_kpmin_asa[l]),h->FindBin(modbin)))/(1.*total_kp_a)*(8.);
			h_driflen_kp_as[l]->Fill(stc_kp_a[l][ist],h->GetBinContent(h->FindBin(modbin)));

		}


		//GND
		h = h_kp_tdcprim_gs[l];
		total_kp_g = h->Integral(h->FindBin(tdcprim_kpmin_gnd[l]),h->FindBin(tdcprim_kpmax_gnd[l]));
		for(int ist = 0; ist<tdcprim_kpmax_gnd[l]-tdcprim_kpmin_gnd[l]+1;ist++){
			int modbin = ist+tdcprim_kpmin_gnd[l];
			stc_kp_g[l][ist] =  (h->Integral(h->FindBin(tdcprim_kpmin_gnd[l]),h->FindBin(modbin)))/(1.*total_kp_g)*(8.);
			h_driflen_kp_gs[l]->Fill(stc_kp_g[l][ist],h->GetBinContent(h->FindBin(modbin)));

		}

	}// l<2


}
////////////////////////////////////////////////////////////////////////////////
void Analyze::MakeDCHits(){
	Int_t ngrps;
	Int_t mul;
	std::vector<int> vid;//grooupid
	std::vector<int> uid;
	std::vector<int> xid;//hitid
	std::vector<double> v;
	std::vector<double> u;
	std::vector<double> x;
	std::vector<double> xcat;

	myGroups * grp = nullptr;
	myGroups *ugrp = nullptr;
	myGroups *vgrp = nullptr;
	myGroups *xgrp = nullptr;
	for (int l =0 ; l<2;l++){
		vid.clear();
		uid.clear();
		xid.clear();
		x.clear();
		v.clear();
		u.clear();

		Int_t   npgrps =  fDCKpGroups[l]->GetEntriesFast();
		//Kv
		ngrps = fDCKvGroups[l]->GetEntriesFast();

		for(int ig =0; ig<ngrps; ig ++){
			grp = (myGroups*) fDCKvGroups[l]->At(ig);
			mul = grp->GetSize();
			if (mul>=3&&grp->IsCathOk()){

				int maxid = grp->GetMaxToTID()-grp->GetStartID();
				if(maxid==0 || maxid == mul-1) continue;

				v.push_back(MakePositionCathode(grp,l));
				vid.push_back(ig);
			}

		}//Kv Grousp


		//Ku
		ngrps = fDCKuGroups[l]->GetEntriesFast();

		for(int ig =0; ig<ngrps; ig ++){
			grp = (myGroups*) fDCKuGroups[l]->At(ig);
			mul = grp->GetSize();
			if (mul>=3&&grp->IsCathOk()){

				int maxid = grp->GetMaxToTID()-grp->GetStartID();
				if(maxid==0 || maxid == mul-1) continue;

				u.push_back(MakePositionCathode(grp,l));
				uid.push_back(ig);
			}

		}//Ku Grousp



		for (int iu=0; iu<u.size();iu++){


			for(int iv =0; iv<v.size(); iv++){

				bool move_out_of_x_loop = false;
				double xc = -(u[iu]+v[iv])/TMath::Sqrt(2);
				Double_t y =    (u[iu]-v[iv])/TMath::Sqrt(2);
				h_XYcath_s[l]->Fill(xc,y);
				if(TMath::Abs(static_cast<int>(std::round(TMath::Abs(xc)))%16-7)<5) continue;
				double xcr = std::round(xc/16)*16;
/*
				for(int ig=0; ig<npgrps; ig++ ){
					grp = (myGroups*)fDCKpGroups[l]->At(ig);
					Int_t mul = grp->GetSize();
					/*
					//int minid = grp->GetMinTDCID()-grp->GetStartID();
					int minid = grp->GetMaxToTID()-grp->GetStartID();
					bool Asagi = grp->At(minid)->IsAsagi();
					double potpos = grp->At(minid)->GetWirePos();
					double anx[2];
					anx[0]  = potpos -8;
					anx[1] = potpos +8;
					//int tdc =  grp->GetMinTDC();
					int tdc =  grp->At(minid)->GetTDC();
					int kdir =0;
					Double_t x_drift = CalcDriftLenPot( tdc,  l, Asagi);
					Double_t x_pot = -99999;
					if(TMath::Abs(anx[1]-xcr)<=5){
					kdir = -1;
					x_pot = anx[1] - x_drift; //potpos + kdir*(-8 + x_drift)
					}

					else if( TMath::Abs(anx[0]-xcr)<=5){
					kdir = 1;
					x_pot = anx[0] + x_drift; //potpos + kdir*(-8 + x_drift)
					}
					else {continue;}
					 //Comment end here

					bool Asagi;
					double potpos;
					Double_t x_pot = -99999;

					int kdir;
					int minid = grp->GetMaxToTID()-grp->GetStartID();
					for(int gid= 0; gid<mul; gid++){

						Asagi = grp->At(gid)->IsAsagi();
						potpos = grp->At(gid)->GetWirePos();
						double anx[2];
						kdir =0;
						anx[0]  = potpos -8;
						anx[1] = potpos +8;
						//int tdc =  grp->GetMinTDC();


						x_pot = -99999;
						if(TMath::Abs(anx[1]-xcr)<=5){
							kdir = -1;
							if(gid!= mul-1){

								if(grp->At(gid+1)->GetToT()-grp->At(gid)->GetToT()>=dw_mean[l][gid]){
									kdir =1;
									minid = gid+1;
								}else {
									minid = gid;
								}
							}
							else{
								minid = gid;
							}
							break;
						}

						else if( TMath::Abs(anx[0]-xcr)<=5){
							kdir = 1;
							if(gid!=0){
								if(grp->At(gid)->GetToT()-grp->At(gid-1)->GetToT()>=dw_mean[l][gid-1]){

									kdir  = 1;
									minid = gid;

								}else {
									minid = gid-1;
									kdir = -1;
								}

							}else{
								kdir =1;
								minid = gid;
							}

							break;
						}
						else {continue;move_out_of_x_loop = true;
							//continue; 
							}

					} // group index                                ///

					if(move_out_of_x_loop) continue;
					//Add Hits to myDCevt;
					int tdc =  grp->At(minid)->GetTDC();
					Double_t x_drift = CalcDriftLenPot( tdc,  l, Asagi);
					potpos = grp->At(minid)->GetWirePos();
h_deltaXcath_s[l]->Fill(xc-(potpos-kdir*8));

if(kdir<0){h_deltaXcath_left_s[l]->Fill(xc-(potpos-kdir*8));h_deltaXcath_shift_s[l]->Fill(xc-(potpos-kdir*8)+0.231759) ;;
}else{h_deltaXcath_right_s[l]->Fill(xc-(potpos-kdir*8));h_deltaXcath_shift_s[l]->Fill(xc-(potpos-kdir*8)-0.344433); 
}
					x_pot  = potpos + kdir*(-8 + x_drift);
					Double_t z = grp->GetZ();
					h_driflen_dchit_s[l]->Fill(x_drift);
					int ndceve = fDCevts[l]->GetEntriesFast();
					myDCevt * dcevt = nullptr;
					new ((*fDCevts[l])[ndceve]) myDCevt();
					dcevt = (myDCevt*) fDCevts[l]->At(ndceve);
					dcevt->SetPos(x_pot,y,z);
					h_XY_s[l]->Fill(x_pot,y);
					dcevt->SetXdir(kdir);
					ugrp = (myGroups*)fDCKuGroups[l]->At(uid[iu]);
					vgrp = (myGroups*)fDCKvGroups[l]->At(vid[iv]);
					dcevt->SetURdtot(ugrp->GetdToTR());
					dcevt->SetVRdtot(vgrp->GetdToTR());
					dcevt->SetULdtot(ugrp->GetdToTL());
					dcevt->SetVLdtot(vgrp->GetdToTL());
					dcevt->SetURrattot(ugrp->GetratToTR()*cath_rat_const);
					dcevt->SetVRrattot(vgrp->GetratToTR()*cath_rat_const);
					dcevt->SetULrattot(ugrp->GetratToTL()*cath_rat_const);
					dcevt->SetVLrattot(vgrp->GetratToTL()*cath_rat_const);
					dcevt->SetXdtdc(tdc);
					dcevt->SetXID(grp->At(minid)->GetID());
					dcevt->SetUID(ugrp->At(ugrp->GetMaxToTID()-ugrp->GetStartID())->GetID());
					dcevt->SetVID(vgrp->At(vgrp->GetMaxToTID()-vgrp->GetStartID())->GetID());

					if(y>170){std:cout<<"uid: "<<ugrp->At(ugrp->GetMaxToTID()-ugrp->GetStartID())->GetID()<<" "<<ugrp->At(ugrp->GetMaxToTID()-ugrp->GetStartID())->GetWirePosz() <<" vid: "<<vgrp->At(vgrp->GetMaxToTID()-vgrp->GetStartID())->GetID()<<"  "<<vgrp->At(vgrp->GetMaxToTID()-vgrp->GetStartID())->GetWirePosz()<<" "<<vgrp->GetMaxToTID()<<std::endl;}

					break;


				}//Loop over groups X
*/

for (int ig = 0; ig < npgrps; ig++) {
    grp = (myGroups*)fDCKpGroups[l]->At(ig);
    Int_t mul = grp->GetSize();
    if (mul <= 1) continue; // Safety check

    int minid = grp->GetMaxToTID() - grp->GetStartID();
    if (minid < 0 || minid >= mul) continue; // Safety bounds check

    bool found_wire = false;
    bool Asagi = false;
    double potpos = 0.0;
    int kdir = 0;
double x_pot = -99999;
    for (int gid = 0; gid < mul; gid++) {
        Asagi = grp->At(gid)->IsAsagi();
        potpos = grp->At(gid)->GetWirePos();
        double anx[2] = { potpos - 8.0, potpos + 8.0 };

        if (TMath::Abs(anx[1] - xcr) <= 5) {
            found_wire = true;
            kdir = -1;
            if (gid != mul - 1) {
                if (grp->At(gid + 1)->GetToT() - grp->At(gid)->GetToT() > dw_mean[l][gid]) {
                    kdir = 1;
                    minid = gid + 1;
                } else {
                    minid = gid;
                }
            } else {
                minid = gid;
            }
            break; // Found matching wire, exit gid loop
        }
        else if (TMath::Abs(anx[0] - xcr) <= 5) {
            found_wire = true;
            kdir = 1;
            if (gid != 0) {
                if (grp->At(gid)->GetToT() - grp->At(gid - 1)->GetToT() > dw_mean[l][gid - 1]) {
                    kdir = 1;
                    minid = gid;
                } else {
                    minid = gid - 1;
                    kdir = -1;
                }
            } else {
                kdir = 1;
                minid = gid;
            }
            break; // Found matching wire, exit gid loop
        }
    } // gid loop

    // If no matching wire was found in this Kp group, try the next group
    if (!found_wire) continue; 

    // Add Hits to myDCevt
    int tdc = grp->At(minid)->GetTDC();
    Double_t x_drift = CalcDriftLenPot(tdc, l, Asagi);
    potpos = grp->At(minid)->GetWirePos();

    h_deltaXcath_s[l]->Fill(xc - (potpos - kdir * 8));

    if (kdir < 0) {
        h_deltaXcath_left_s[l]->Fill(xc - (potpos - kdir * 8));
        h_deltaXcath_shift_s[l]->Fill(xc - (potpos - kdir * 8) + 0.231759);
    } else {
        h_deltaXcath_right_s[l]->Fill(xc - (potpos - kdir * 8));
        h_deltaXcath_shift_s[l]->Fill(xc - (potpos - kdir * 8) - 0.344433);
    }

    x_pot = potpos + kdir * (-8 + x_drift);
    Double_t z = grp->GetZ();
    h_driflen_dchit_s[l]->Fill(x_drift);
    h_driflen_dchit_sign_s[l]->Fill(kdir*x_drift);

    int ndceve = fDCevts[l]->GetEntriesFast();
    myDCevt* dcevt = new ((*fDCevts[l])[ndceve]) myDCevt();
    dcevt->SetPos(x_pot, y, z);
    h_XY_s[l]->Fill(x_pot, y);
    dcevt->SetXdir(kdir);

    ugrp = (myGroups*)fDCKuGroups[l]->At(uid[iu]);
    vgrp = (myGroups*)fDCKvGroups[l]->At(vid[iv]);
    dcevt->SetURdtot(ugrp->GetdToTR());
    dcevt->SetVRdtot(vgrp->GetdToTR());
    dcevt->SetULdtot(ugrp->GetdToTL());
    dcevt->SetVLdtot(vgrp->GetdToTL());
    dcevt->SetURrattot(ugrp->GetratToTR() * cath_rat_const);
    dcevt->SetVRrattot(vgrp->GetratToTR() * cath_rat_const);
    dcevt->SetULrattot(ugrp->GetratToTL() * cath_rat_const);
    dcevt->SetVLrattot(vgrp->GetratToTL() * cath_rat_const);
    dcevt->SetXdtdc(tdc);
    dcevt->SetXID(grp->At(minid)->GetID());
    dcevt->SetUID(ugrp->At(ugrp->GetMaxToTID() - ugrp->GetStartID())->GetID());
    dcevt->SetVID(vgrp->At(vgrp->GetMaxToTID() - vgrp->GetStartID())->GetID());

    if (y > 170) {
        std::cout << "uid: " << ugrp->At(ugrp->GetMaxToTID() - ugrp->GetStartID())->GetID() << " " 
                  << ugrp->At(ugrp->GetMaxToTID() - ugrp->GetStartID())->GetWirePosz() 
                  << " vid: " << vgrp->At(vgrp->GetMaxToTID() - vgrp->GetStartID())->GetID() << " " 
                  << vgrp->At(vgrp->GetMaxToTID() - vgrp->GetStartID())->GetWirePosz() << " " 
                  << vgrp->GetMaxToTID() << std::endl;
    }

    break; // Hit added for this (u, v) pair; move to next (u, v) pair
} // ig loop




			} //iv

		} //iu







	}// layers

}
///////////////////////////////

Double_t Analyze::MakePositionCathode(myGroups* grp,int l){

	int maxid = grp->GetMaxToTID()-grp->GetStartID();
	int maxtot  =grp->GetMaxToT();
	int maxid_l = maxid- 1;
	int maxid_r = maxid+ 1;
	int maxid_ltot= grp->At(maxid_l)->GetToT();
	int maxid_rtot= grp->At(maxid_r)->GetToT();
	bool Asagi = grp->At(maxid)->IsAsagi();
	int dtot_l = maxtot-maxid_ltot;
	int dtot_r = maxtot-maxid_rtot;
	int rtot_l = cath_rat_const/maxtot*maxid_ltot;
	int rtot_r = cath_rat_const/maxtot*maxid_rtot;
	//      std::cout<<maxtot<<" "<<maxid_ltot<<std::endl;
	//      std::cout<<maxtot<<" "<<maxid_rtot<<std::endl;
	//      std::cout<<rtot_r<<std::endl;
	double wt_l;
	double wt_r;

	double pos;
	double avg_drf;
	double strippos = grp->At(maxid)->GetWirePos();

	TString v =*( grp->At(maxid)->GetDir());

	if(!cath_rat_method){
		if(v == 'v'){

			if(!Asagi){
				if(dtot_l>difftot_kvmax_gnd[l]) dtot_l = difftot_kvmax_gnd[l];
				if(dtot_r>difftot_kvmax_gnd[l]) dtot_r = difftot_kvmax_gnd[l];

				if(dtot_r !=0) wt_r  = 1/double(dtot_r);
				else wt_r = 0.0000001;
				if(dtot_l !=0) wt_l = 1/double(dtot_l);
				else wt_l = 0.0000001;

				avg_drf =  (stc_kv_r_g[l][dtot_r]*wt_r+stc_kv_l_g[l][dtot_l]*wt_l)/(1*wt_l +wt_r);
				pos = strippos + avg_drf;
				h_driflen_kv_gs[l]->Fill(avg_drf);
				//      pos = strippos + stc_kv_r_g[l][dtot_r];

			}else{
				if(dtot_l>difftot_kvmax_asa[l]) dtot_l = difftot_kvmax_asa[l];
				if(dtot_r>difftot_kvmax_asa[l]) dtot_r = difftot_kvmax_asa[l];
				if(dtot_r !=0) wt_r = 1/double(dtot_r);
				else wt_r = 0.0000001;
				if(dtot_l !=0) wt_l = 1/double(dtot_l);
				else wt_l = 0.0000001;
				avg_drf =  (stc_kv_r_a[l][dtot_r]*wt_r+stc_kv_l_a[l][dtot_l]*wt_l)/(wt_l +wt_r);
				pos = strippos + avg_drf;

				//      pos = strippos + stc_kv_r_a[l][dtot_r];

			}


		} else if (v== 'u'){

			if(!Asagi){
				if(dtot_l>difftot_kumax_gnd[l]) dtot_l = difftot_kumax_gnd[l];
				if(dtot_r>difftot_kumax_gnd[l]) dtot_r = difftot_kumax_gnd[l];
				if(dtot_r !=0) wt_r  = 1/double(dtot_r);
				else wt_r = 0.0000001;
				if(dtot_l !=0) wt_l = 1/double(dtot_l);
				else wt_l = 0.0000001;
				avg_drf =  (stc_ku_r_g[l][dtot_r]*wt_r+stc_ku_l_g[l][dtot_l]*wt_l)/(wt_l +wt_r);
				pos = strippos + avg_drf;
				h_driflen_ku_gs[l]->Fill(avg_drf);
				//      pos = strippos + stc_ku_r_g[l][dtot_r];

			}else{
				if(dtot_l>difftot_kumax_asa[l]) dtot_l = difftot_kumax_asa[l];
				if(dtot_r>difftot_kumax_asa[l]) dtot_r = difftot_kumax_asa[l];
				if(dtot_r !=0) wt_r  = 1/double(dtot_r);
				else wt_r = 0.0000001;
				if(dtot_l !=0) wt_l = 1/double(dtot_l);
				else wt_l = 0.0000001;
				avg_drf =  (stc_ku_r_a[l][dtot_r]*wt_r+stc_ku_l_a[l][dtot_l]*wt_l)/(wt_l +wt_r);
				pos = strippos + avg_drf;
				//      pos = strippos + stc_ku_r_a[l][dtot_r];

			}



		}

	}else{  //cathratmethod
		if(v == 'v'){

			if(!Asagi){

				if(rtot_r !=0) wt_r  = double(rtot_r);
				if(rtot_l !=0) wt_l = double(rtot_l);

				avg_drf =  (stc_kv_r_g[l][rtot_r]*wt_r+stc_kv_l_g[l][rtot_l]*wt_l)/(1*wt_l +wt_r);
				pos = strippos + avg_drf;
				h_driflen_kv_gs[l]->Fill(avg_drf);
				//      pos = strippos + stc_kv_r_g[l][dtot_r];

			}else{
				if(rtot_r !=0) wt_r = double(rtot_r);
				else wt_r = 0.0000001;
				if(rtot_l !=0) wt_l = double(rtot_l);
				else wt_l = 0.0000001;
				avg_drf =  (stc_kv_r_a[l][rtot_r]*wt_r+stc_kv_l_a[l][rtot_l]*wt_l)/(wt_l +wt_r);
				pos = strippos + avg_drf;

				//      pos = strippos + stc_kv_r_a[l][dtot_r];

			}


		} else if (v== 'u'){

			if(!Asagi){
				if(rtot_r !=0) wt_r  = double(rtot_r);
				else wt_r = 0.0000001;
				if(rtot_l !=0) wt_l = double(rtot_l);
				else wt_l = 0.0000001;
				avg_drf =  (stc_ku_r_g[l][rtot_r]*wt_r+stc_ku_l_g[l][rtot_l]*wt_l)/(wt_l +wt_r);
				pos = strippos + avg_drf;
				h_driflen_ku_gs[l]->Fill(avg_drf);
				//      pos = strippos + stc_ku_r_g[l][dtot_r];

			}else{
				if(rtot_r !=0) wt_r  = double(rtot_r);
				else wt_r = 0.0000001;
				if(rtot_l !=0) wt_l = double(rtot_l);
				else wt_l = 0.0000001;
				avg_drf =  (stc_ku_r_a[l][rtot_r]*wt_r+stc_ku_l_a[l][rtot_l]*wt_l)/(wt_l +wt_r);
				pos = strippos + avg_drf;
				//      pos = strippos + stc_ku_r_a[l][dtot_r];

			}



		}

	}//cath_rat_method
	return pos;


}
////////////////////////////////////////////////////////////
Double_t Analyze::CalcDriftLenPot(int tdc, int l , bool Asagi){

	int modtdc;
	if(!Asagi){
		if(tdc<=tdcprim_kpmin_gnd[l]){modtdc =0;}
		else if(tdc>=tdcprim_kpmax_gnd[l]){modtdc = tdcprim_kpmax_gnd[l] - tdcprim_kpmin_gnd[l];}
		else {modtdc = tdc-tdcprim_kpmin_gnd[l];}
		return stc_kp_g[l][modtdc];}
	else
	{if(tdc<=tdcprim_kpmin_asa[l]){modtdc =0;}
		else if(tdc>=tdcprim_kpmax_asa[l]){modtdc = tdcprim_kpmax_asa[l] - tdcprim_kpmin_asa[l];}
		else {modtdc = tdc-tdcprim_kpmin_asa[l];}
		return stc_kp_a[l][modtdc];}
}
////////////////////////////

void Analyze::MakeDCPosition(TClonesArray *KUgroups,TClonesArray *KVgrousp,  TVector3 *dcpos){



}
/////////////////////////////
void Analyze::MakeTracks(){

	myTrack *tr = nullptr;
	myDCevt* dcevt = nullptr;
	int n_dceve[2]={fDCevts[0]->GetEntriesFast(),fDCevts[1]->GetEntriesFast()};

	TVector3 fl1pos(0, 0, -124.95);
	TVector3 fl2pos(0, 0, 124.95);

	if (n_dceve[1] ==0 || n_dceve[0] ==0) return;
	for (int ieve1 = 0; ieve1<n_dceve[0];ieve1++){
		for (int ieve2 = 0; ieve2< n_dceve[1]; ieve2++){
			myTrack local_track;
			myTrack* track = &local_track;
			//myTrack* track = (myTrack*)ftracks->ConstructedAt(ftracks->GetEntriesFast());
			track->SetDC1evt((myDCevt*)fDCevts[0]->At(ieve1));
			dcevt = (myDCevt*)fDCevts[0]->At(ieve1);
			track->SetXID(0,dcevt->GetXID());
			track->SetVID(0,dcevt->GetVID());
			track->SetUID(0,dcevt->GetUID());
			track->SetConvPos(&fl1pos);
			track->SetCatPos(&fl2pos);

			track->SetDC2evt((myDCevt*)fDCevts[1]->At(ieve2));

			dcevt = (myDCevt*)fDCevts[1]->At(ieve2);
			track->SetXID(1,dcevt->GetXID());
			track->SetVID(1,dcevt->GetVID());
			track->SetUID(1,dcevt->GetUID());
			track->Calibrate();
			ReconstructTrack(track,1);
			myDCHitPara* para = nullptr;
			for(int l =0 ;l<2;l++){ para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetXID(l)));
				Bool_t asagix = para->GetIsAsagi();
				int tdc = track->GetXtdc(l);
				double  strippos = para->GetWirePosition();
				Double_t x_drift = CalcDriftLenPot( tdc,  l, asagix);
				h_alpha_drift[l]->Fill(track->GetXAngle()*180/TMath::Pi(),x_drift);
				h_alpha_x[l]->Fill(track->GetXAngle()*180/TMath::Pi(),track->GetDCHitPos(l).X());
				h_beta_y[l]->Fill(track->GetYAngle()*180/TMath::Pi(),track->GetDCHitPos(l).Y());
			}

			h_x1x2->Fill(track->GetDCHitPos(0).X(),track->GetDCHitPos(1).X());
			h_y1y2->Fill(track->GetDCHitPos(0).Y(),track->GetDCHitPos(1).Y());

			bool fconv =false;
			bool fcat = false;


			for (int ipla =0 ; ipla<3; ipla ++){
				if (! pla_bool[ipla]) continue;
				if (TMath::Abs(track->GetnpVertex().Y()-y_pla[ipla])<=50){
					fconv = true;
					track->SetConvPos(0,y_pla[ipla],-124.95);
					track->SetConvID(ipla);
					break;
				}

			}// Layer 1 Plastic Hit Check

			for (int ipla =3 ; ipla<6; ipla ++){
				if (! pla_bool[ipla]) continue;
				if (TMath::Abs(track->GetpCatch().Y()-y_pla[ipla])<=50){
					fcat = true;
					track->SetCatPos(0,y_pla[ipla],124.95);
					track->SetCatID(ipla);
					break;
				}

			}// Layer 2 plastic hit check
			int track_idx = ftracks->GetEntriesFast();
			myTrack* ftrack_elem = (myTrack*)ftracks->ConstructedAt(track_idx);
			*ftrack_elem = *track;
			int cum_index = ftracks_cum->GetEntriesFast();
			new ((*ftracks_cum)[cum_index]) myTrack(*track);

			h_npvertex_basic->Fill(track->GetnpVertex().X(), track->GetnpVertex().Y());
			if(TMath::Abs(track->GetYAngle())<2*TMath::Pi()/180.){

				h_npvertex_b2->Fill(track->GetnpVertex().X(), track->GetnpVertex().Y());
			}
			if(TMath::Abs(track->GetYAngle())<1*TMath::Pi()/180.){

				h_npvertex_b1->Fill(track->GetnpVertex().X(), track->GetnpVertex().Y());
			}

			if(TMath::Abs(track->GetXAngle())<2*TMath::Pi()/180.){

				h_npvertex_a2->Fill(track->GetnpVertex().X(), track->GetnpVertex().Y());
			}
			if(TMath::Abs(track->GetXAngle())<1*TMath::Pi()/180.){

				h_npvertex_a1->Fill(track->GetnpVertex().X(), track->GetnpVertex().Y());
			}


			//      if(TMath::Abs(track->GetZAngle())<1*TMath::Pi()/180.){ std::cout<<"\n";std::cout<<track->GetConvID()<<" "<<track->GetCatID()<<std::endl;}
			h_alpha->Fill(track->GetXAngle()*180/TMath::Pi());
			h_beta->Fill(track->GetYAngle()*180/TMath::Pi());
			h_theta->Fill(track->GetZAngle()*180/TMath::Pi());


	//		if(!(fconv && fcat)) {
if(!fconv){
//if(0){
				if(ftracks->GetEntriesFast()>0){
					((myTrack*)ftracks->At(ftracks->GetEntriesFast()-1))->Clear();
					((myTrack*)ftracks_cum->At(cum_index))->Clear();
					ftracks->RemoveAt(ftracks->GetEntriesFast()-1);
					ftracks_cum->RemoveAt(cum_index);
					ftracks->Compress();
					ftracks_cum->Compress();
					;       }
			}else{

				h_npvertex_corr->Fill(track->GetnpVertex().X(), track->GetnpVertex().Y());

				//	std::cout<<"\n";	std::cout<<track->GetZAngle()*180/TMath::Pi()<<std::endl;
			}


		}//NEOLITH-s2 evts


	}// NEOLITH-s1 evts

	//      if(ftracks->GetEntriesFast()>1) std::cout<<((myTrack*)ftracks->At(0))->GetConvID()<<" "<<((myTrack*)ftracks->At(1))->GetConvID()<<std::endl;
	/*if(ftracks->GetEntriesFast()>1){
	  myTrack* tr1 = (myTrack*)ftracks->At(0);
	  myTrack* tr2 = (myTrack*)ftracks->At(1);
	  Double_t dist =(tr1->GetnpVertex()-tr2->GetnpVertex()).Mag() ;
	  TVector3 dist_vec =(tr1->GetnpVertex()-tr2->GetnpVertex()) ;
	  if((tr1)->GetConvID() ==(tr2)->GetConvID() && (tr1)->GetConvID() >= 0 ){
	  h_dist_vertex->Fill(dist);
	  h_dxdy_vertex->Fill(TMath::Abs(dist_vec.X()),TMath::Abs(dist_vec.Y()));

	  }
	//std::cout<<(tr1->GetnpVertex()-tr2->GetnpVertex()).Mag()<<std::endl;
	}*/

	Int_t nTracks = ftracks->GetEntriesFast();
	if(nTracks >=2)
		for (Int_t i = 0; i < nTracks - 1; ++i) {
			myTrack* tr1 = (myTrack*)ftracks->At(i);

			for (Int_t j = i + 1; j < nTracks; ++j) {
				myTrack* tr2 = (myTrack*)ftracks->At(j);

				TVector3 dist_vec = tr1->GetnpVertex() - tr2->GetnpVertex();
				Double_t dist = dist_vec.Mag();
				double ang = (tr1->GetVect()).Angle(tr2->GetVect())*180/3.141599;

				// Only fill for tracks from the same conversion
				if (tr1->GetConvID() == tr2->GetConvID() &&
						tr1->GetConvID() >= 0) {
					if(dist_vec.X()!=0){
						h_ang_vertex->Fill(ang);
						h_ang_dist_vertex->Fill(ang,dist);
						h_ang_dy_vertex->Fill(ang,TMath::Abs(dist_vec.Y()));
						h_ang_dx_vertex->Fill(ang,TMath::Abs(dist_vec.X()));
						h_dist_vertex->Fill(dist);
						h_dxdy_vertex->Fill(TMath::Abs(dist_vec.X()),TMath::Abs(dist_vec.Y()));
						h_iddy_vertex->Fill(tr1->GetConvID() ,
								TMath::Abs(dist_vec.Y()));
					}
					//      std::cout<<(dist_vec.X())<<std::endl;
				}
			} //loop over the tracks with index more than the current track
		}//loop over all the tracks
}
///////////////////////////////

void Analyze::ReconstructSTC(int niter){

	Int_t ntracks = ftracks_cum->GetEntriesFast();
	std::cout<<1<< " "<<ntracks<<std::endl;
	myTrack* track = nullptr;
	stc_make_kv = true;

	myDCHitPara *para = nullptr;
	for(int iter = 0; iter<niter; iter++){
		ClearSTCHist();

		std::cout << "\rCurrent STC Iteration: " << iter+1 << std::flush;
		for(int itra=0; itra<ntracks; itra++){
			track = (myTrack*)ftracks_cum->At(itra);

			/*Make Position on Cathode and potential*/

			//      std::cout<<"Y Angle "<<track->GetYAngle()<<std::endl;
			ModifyTrack(track);
			if(stc_make_kv ){
				if(TMath::Abs(track->GetZAngle())<90*TMath::Pi()/180.){

					for(int l =0; l<2;l++){


						if(!cath_rat_method){
							para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetVID(l)));
							Bool_t asagiv = para->GetIsAsagi();
							int diff_r = track->GetVRdtot(l);
							int diff_l = track->GetVLdtot(l);

							int rat_r = track->GetVRdtot(l);
							int rat_l = track->GetVLdtot(l);
							if(asagiv)
							{h_kv_dtot_l_as[l]->Fill(diff_l);
								h_kv_dtot_r_as[l]->Fill(diff_r);
							}else
							{       h_kv_dtot_l_gs[l]->Fill(diff_l);
								h_kv_dtot_r_gs[l]->Fill(diff_r);
							}
							para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetUID(l)));
							Bool_t asagiu = para->GetIsAsagi();
							diff_r = track->GetURdtot(l);
							diff_l = track->GetULdtot(l);
							if(asagiu)
							{       h_ku_dtot_l_as[l]->Fill(diff_l);
								h_ku_dtot_r_as[l]->Fill(diff_r);
							}else
							{       h_ku_dtot_l_gs[l]->Fill(diff_l);
								h_ku_dtot_r_gs[l]->Fill(diff_r);
							}


						} else{

							para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetVID(l)));
							Bool_t asagiv = para->GetIsAsagi();
							int diff_r = track->GetVRdtot(l);
							int diff_l = track->GetVLdtot(l);

							int rat_r = track->GetVRrattot(l);
							int rat_l = track->GetVLrattot(l);


							if(asagiv)
							{h_kv_rtot_l_as[l]->Fill(rat_l);
								h_kv_rtot_r_as[l]->Fill(rat_r);
							}else
							{       h_kv_rtot_l_gs[l]->Fill(rat_l);
								h_kv_rtot_r_gs[l]->Fill(rat_r);
							}
							para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetUID(l)));
							Bool_t asagiu = para->GetIsAsagi();
							rat_r = track->GetURrattot(l);
							rat_l = track->GetULrattot(l);
							if(asagiu)
							{       h_ku_rtot_l_as[l]->Fill(rat_l);
								h_ku_rtot_r_as[l]->Fill(rat_r);
							}else
							{       h_ku_rtot_l_gs[l]->Fill(rat_l);
								h_ku_rtot_r_gs[l]->Fill(rat_r);
							}





						}// cath_rat_method
					}



				}//TrackY angle
			} //stc_kv_make


			if(TMath::Abs(track->GetZAngle())<90*TMath::Pi()/180.){
				for(int l =0; l<2; l++){
					para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetXID(l)));
					Bool_t asagix = para->GetIsAsagi();
					Int_t mintdc = track->GetXtdc(l);
					h_kp_tdcprim_s[l]->Fill(mintdc);
					if(asagix){
						h_kp_tdcprim_as[l]->Fill(mintdc);
					}
					else{
						h_kp_tdcprim_gs[l]->Fill(mintdc);}
				}

			}

		}//Loop over All Tracks


		ClearSTC();
		MakeSTC();
	}



}
//////////////////////////////
void Analyze::ModifyTrack(myTrack* track){
	myDCHitPara* para = nullptr;
	double strippos,posv,posu,posx,posy,avg_drf,wt_l,wt_r;
	posv = -99999;
	posu = -99999;
	int dtot_r ;
	int dtot_l ;
	int rtot_r;
	int rtot_l ;


	if (!track) {
		std::cerr << "Error: track pointer is null!" << std::endl;
		return;
	}
	for(int l =0; l<2;l++){
		if(!cath_rat_method){

			para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetVID(l)));
			Bool_t asagiv = para->GetIsAsagi();
			dtot_r = track->GetVRdtot(l);
			dtot_l = track->GetVLdtot(l);
			strippos = para->GetWirePosition();

			if(asagiv){
				if(dtot_l>difftot_kvmax_asa[l]) dtot_l = difftot_kvmax_asa[l];
				if(dtot_r>difftot_kvmax_asa[l]) dtot_r = difftot_kvmax_asa[l];
				if(dtot_r !=0)wt_r = 1/double(dtot_r);
				else  wt_r = 0.0000001;
				if(dtot_l !=0) wt_l = 1/double(dtot_l);
				else wt_l = 0.0000001;
				avg_drf =  (stc_kv_r_a[l][dtot_r]*wt_r+stc_kv_l_a[l][dtot_l]*wt_l)/(wt_l +wt_r);
				//      std::cout<<dtot_l<<" "<<avg_drf;
				posv = strippos + avg_drf;

			}
			else{
				if(dtot_l>difftot_kvmax_gnd[l]) dtot_l = difftot_kvmax_gnd[l];
				if(dtot_r>difftot_kvmax_gnd[l]) dtot_r = difftot_kvmax_gnd[l];
				if(dtot_r !=0) wt_r  = 1/double(dtot_r);
				else wt_r = 0.0000001;
				if(dtot_l !=0) wt_l = 1/double(dtot_l);
				else wt_l = 0.0000001;

				avg_drf =  (stc_kv_r_g[l][dtot_r]*wt_r+stc_kv_l_g[l][dtot_l]*wt_l)/(wt_l +wt_r);
				posv = strippos + avg_drf;

			}//V pos

			para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetUID(l)));
			Bool_t asagiu = para->GetIsAsagi();
			dtot_r = track->GetURdtot(l);
			dtot_l = track->GetULdtot(l);
			//std::cout<<dtot_r<<" "<<dtot_l<<std::endl;
			strippos = para->GetWirePosition();
			if(asagiu){
				if(dtot_l>difftot_kumax_asa[l]) dtot_l = difftot_kumax_asa[l];
				if(dtot_r>difftot_kumax_asa[l]) dtot_r = difftot_kumax_asa[l];
				if(dtot_r !=0) wt_r  = 1/double(dtot_r);
				else wt_r  = 0.0000001;
				if(dtot_l !=0) wt_l = 1/double(dtot_l);
				else wt_l = 0.0000001;

				avg_drf =  (stc_ku_r_a[l][dtot_r]*wt_r+stc_ku_l_a[l][dtot_l]*wt_l)/(wt_l +wt_r);
				posu = strippos + avg_drf;

			}
			else{
				if(dtot_l>difftot_kumax_gnd[l]) dtot_l = difftot_kumax_gnd[l];
				if(dtot_r>difftot_kumax_gnd[l]) dtot_r = difftot_kumax_gnd[l];
				if(dtot_r !=0) wt_r  = 1/double(dtot_r);
				else wt_r = 0.0000001;
				if(dtot_l !=0) wt_l = 1/double(dtot_l);
				else wt_l = 0.0000001;

				avg_drf =  (stc_ku_r_g[l][dtot_r]*wt_r+stc_ku_l_g[l][dtot_l]*wt_l)/(wt_l +wt_r);
				posu = strippos + avg_drf;

			}//U pos


		}else{ //cath_Rat_method
			para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetVID(l)));
			Bool_t asagiv = para->GetIsAsagi();
			rtot_r = track->GetVRrattot(l);
			rtot_l = track->GetVLrattot(l);
			strippos = para->GetWirePosition();

			if(asagiv){
				if(rtot_r !=0)wt_r = double(rtot_r);
				else  wt_r = 0.0000001;
				if(rtot_l !=0) wt_l = double(rtot_l);
				else wt_l = 0.0000001;
				avg_drf =  (stc_kv_r_a[l][rtot_r]*wt_r+stc_kv_l_a[l][rtot_l]*wt_l)/(wt_l +wt_r);
				//      std::cout<<dtot_l<<" "<<avg_drf;
				posv = strippos + avg_drf;

			}
			else{
				if(rtot_r !=0) wt_r  = double(rtot_r);
				else wt_r = 0.0000001;
				if(rtot_l !=0) wt_l = double(rtot_l);
				else wt_l = 0.0000001;

				avg_drf =  (stc_kv_r_g[l][rtot_r]*wt_r+stc_kv_l_g[l][rtot_l]*wt_l)/(wt_l +wt_r);
				posv = strippos + avg_drf;

			}//V pos
			 //

			para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetUID(l)));
			Bool_t asagiu = para->GetIsAsagi();
			rtot_r = track->GetURrattot(l);
			rtot_l = track->GetULrattot(l);

			//std::cout<<dtot_r<<" "<<dtot_l<<std::endl;
			strippos = para->GetWirePosition();
			if(asagiu){
				if(rtot_r !=0) wt_r  = double(rtot_r);
				else wt_r  = 0.0000001;
				if(rtot_l !=0) wt_l = double(rtot_l);
				else wt_l = 0.0000001;

				avg_drf =  (stc_ku_r_a[l][rtot_r]*wt_r+stc_ku_l_a[l][rtot_l]*wt_l)/(wt_l +wt_r);
				posu = strippos + avg_drf;

			}
			else{
				if(rtot_r !=0) wt_r  = double(rtot_r);
				else wt_r = 0.0000001;
				if(rtot_l !=0) wt_l = double(rtot_l);
				else wt_l = 0.0000001;

				avg_drf =  (stc_ku_r_g[l][rtot_r]*wt_r+stc_ku_l_g[l][rtot_l]*wt_l)/(wt_l +wt_r);
				posu = strippos + avg_drf;

			}//U pos



		} //cath_rat_method




		posy = (posu-posv)/TMath::Sqrt(2);
//		std::cout<<posy<<std::endl;
		para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetXID(l)));
		Bool_t asagix = para->GetIsAsagi();
		int tdc = track->GetXtdc(l);
		strippos = para->GetWirePosition();

		Double_t x_drift = CalcDriftLenPot( tdc,  l, asagix);
		posx = strippos + (track->GetXdir(l))*(-8+x_drift);
		//      std::cout<<posx<<" "<<posy<<std::endl;
		//std::cout<<posx<<" "<<posy<<" "<<para->GetWireZPosition()<<std::endl;
		track->SetDCHit(l,TVector3(posx,posy,para->GetWireZPosition()));
	}// layers

	track->Calibrate();

}
//////////////////////////////
void Analyze::ReconstructTrack(myTrack *track, int niter){

	myDCHitPara *para = nullptr;
	double x_drift;
	for(int iter = 0; iter<niter; iter++){



		for(int l =0; l<2;l++){


			para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetXID(l)));
			double x_wire  = para->GetWirePosition();
			int x_dir = track->GetXdir(l);
			x_drift = (track->GetDCHitPos(l).X() - x_wire)/x_dir +8;
	//	if(x_drift/TMath::Cos(track->GetXAngle())<=8)	
			x_drift = x_drift/TMath::Cos(track->GetXAngle());


			double x = x_wire +x_dir*(x_drift-8);
			double y = track->GetDCHitPos(l).Y();
			double z = track->GetDCHitPos(l).Z();

			track->SetDCHit(l,TVector3(x,y,z));

			if(iter+1==niter)      { h_driflen_dctrack_s[l]->Fill(x_drift);
			h_driflen_dctrack_sign_s[l]->Fill(x_dir*x_drift);
}
		}
		track->Calibrate();
	}//Mumber of iterations




}

//////////////////////////////////////////////////////////////
/*
   void Analyze::ReconstructTrack(myTrack *track, int niter) {
   std::vector<double> z_wire;
   std::vector<double> x_wire;
   std::vector<double> r_drift;
   std::vector<int>    x_dir;


   for (int l = 0; l < 2; l++) {
   myDCHitPara *para = const_cast<myDCHitPara*>(FindDCHitPara(track->GetXID(l)));
   if (!para) continue;

   double x_w = para->GetWirePosition();
   double z_w = track->GetDCHitPos(l).Z();
   int dir    = track->GetXdir(l);

   double raw_drift = (track->GetDCHitPos(l).X() - x_w) / dir + 8.0;

   z_wire.push_back(z_w);
   x_wire.push_back(x_w);
   r_drift.push_back(raw_drift);
   x_dir.push_back(dir);
   }

   int n = z_wire.size();
   if (n < 2) return; // 直線フィッティングには最低2点が必要

   for (int iter = 0; iter < niter; iter++) {

// Lambda関数として χ²(a, b) を定義 (式 4.3.1 および 4.3.2)
auto chi2_func = [&](const double *par) {
double a = par[0];
double b = par[1];
double denom = std::sqrt(a * a + 1.0);
double chi2 = 0.0;

for (int i = 0; i < n; i++) {
double di = std::abs(a * z_wire[i] - x_wire[i] + b) / denom;

double diff = di - r_drift[i];
chi2 += diff * diff;
}
return chi2;
};

ROOT::Math::Minimizer* minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
if (!minimizer) {
minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit", "Migrad");
}

minimizer->SetMaxFunctionCalls(10000);
minimizer->SetTolerance(1e-4);
minimizer->SetPrintLevel(0); // ログ出力を抑制

ROOT::Math::Functor functor(chi2_func, 2); // 2パラメータ (a, b)
minimizer->SetFunction(functor);

double init_a = std::tan(track->GetXAngle());
double init_b = track->GetDCHitPos(0).X() - init_a * z_wire[0];

minimizer->SetVariable(0, "a", init_a, 0.01);
minimizer->SetVariable(1, "b", init_b, 0.1);

minimizer->Minimize();

double best_a = minimizer->X()[0]; // 最適化された傾き a
double best_b = minimizer->X()[1]; // 最適化された切片 b

delete minimizer; // メモリ解放

double fit_angle = std::atan(best_a);
track->SetXAngle(fit_angle); // 新しい角度を反映

for (int l = 0; l < n; l++) {
	double x_reco = best_a * z_wire[l] + best_b;
	double y_reco = track->GetDCHitPos(l).Y();
	double z_reco = z_wire[l];

	track->SetDCHit(l, TVector3(x_reco, y_reco, z_reco));

	if (iter + 1 == niter && h_driflen_dctrack_s[l]) {
		double di = std::abs(best_a * z_wire[l] - x_wire[l] + best_b) / std::sqrt(best_a * best_a + 1.0);
		h_driflen_dctrack_s[l]->Fill(di);
	}
}

track->Calibrate();
}
}
*/
////////////////////////////////

const myDCHitPara* Analyze::FindDCHitPara(TArtRIDFMap *rmap) const
{
	std::map<TArtRIDFMap, myDCHitPara *>::const_iterator itr = dc_pmap.find(*rmap);
	if(itr != dc_pmap.end()) return (myDCHitPara*)itr->second;
	else return 0;
}
/////////////////////////////////////
const myDCHitPara* Analyze::FindDCHitPara(Int_t id) const
{
	for (const auto& entry : dc_pmap) {
		if (entry.second->GetID() == id)
			return entry.second;
	}
	return nullptr;
}
///////////////////////////////////////
bool Analyze::LoadParameters(const char *xmlfile){
	TArtCore::Info(__FILE__,"Load parameter from %s", xmlfile);
	TDOMParser domParser;
	domParser.SetValidate(false);
	Int_t parsecode = domParser.ParseFile(xmlfile);

	if(parsecode < 0){
		std::cerr << domParser.GetParseCodeMessage(parsecode) << std::endl;
		return false;
	}
	TXMLNode* node = domParser.GetXMLDocument()->GetRootNode();
	ParseParaList(node->GetChildren());
	return true;

}
/////////////////////////////////////
void Analyze::ParseParaList(TXMLNode *node){

	for(; node; node = node->GetNextNode()){
		if(node->GetNodeType() != TXMLNode::kXMLElementNode) continue; // Element Node

		if(strcmp(node->GetNodeName(), "NEOLITHs") == 0){
			listOfDCHitPara->Add(ParseDCHitPara(node->GetChildren(),&dc_pmap));
		}
	}

}
///////////////////////////////////////

myDCHitPara *Analyze::ParseDCHitPara(TXMLNode *node, std::map<TArtRIDFMap, myDCHitPara *> * map)
{
	Int_t     id=-1;
	TString   name;
	Int_t     fpl=13;

	Int_t wireid=-9999;
	Double_t wirepos=-9999; // wire position in a layer
	Double_t wirez=-9999; // wire z position

	// calibration value for TDC -> timing
	Double_t  tzero_offset=0;
	TString  dir ="none";
	Int_t det=-1, geo=-1, ch=-1;
	Bool_t asagi = 0;

	for ( ; node; node = node->GetNextNode()) {

		if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
			if (strcmp(node->GetNodeName(), "ID") == 0)
				id = std::stoi(node->GetText());
			if (strcmp(node->GetNodeName(), "name") == 0)
				name = node->GetText();
			if (strcmp(node->GetNodeName(), "wire") == 0)
				wireid = std::stoi(node->GetText());
			if (strcmp(node->GetNodeName(), "pos") == 0)
				wirepos = std::stod(node->GetText());
			if (strcmp(node->GetNodeName(), "posz") == 0)
				wirez = std::stod(node->GetText());
			if (strcmp(node->GetNodeName(), "direction") == 0)
				dir  = node->GetText();
			if (strcmp(node->GetNodeName(), "geo") == 0)
				geo = std::stoi(node->GetText());
			if (strcmp(node->GetNodeName(), "ch") == 0)
				ch = std::stoi(node->GetText());
			if (strcmp(node->GetNodeName(),"IsAsagi")==0)
				asagi = std::stoi(node->GetText());

		}

	}
	myDCHitPara * para = new myDCHitPara(id, name, wireid, wirepos, wirez,dir,asagi);
	para->SetMap(geo, ch);

	map->insert(std::pair<TArtRIDFMap, myDCHitPara *>(*((TArtRIDFMap *)para->GetTDCMap()), para));

	return para;


}
//////////////////////////////////////////////////////////////////////////////////
myDCHit* Analyze::FindDCHit(Int_t id){

	for(Int_t i = fDCWireHits->GetEntries()-1; i>=0;i--){

		if(id == ((myDCHit*)fDCWireHits->At(i))->GetID())
			return (myDCHit*)fDCWireHits->At(i);


	}
	return NULL;
}

/////////////////////////////////////////////////////////////
void Analyze::Clear(){


	fDCWireHitsPara.clear();
	qdc_pmt.assign(npmt,0);
	qdcorg_ch.assign(npmt,0);
	qtc_pmt.assign(npmt,tdc_init);
	tdc_ch.assign(32,tdc_init);
	tdc_pmt.assign(npmt,tdc_init);
	trail_tdc_ch.assign(32,tdc_init);

	tdiff_pla.assign(npla, tdc_init);
	t_pla.assign(npla, tdc_init);
	qtc_pla.assign(npla,tdc_init);
	qdc_pla.assign(npla,qdc_init);

	tof_row.assign(3,tdc_init);
	tof_pattern.assign(npla/2*3, tdc_init);
	tof_sbt_pla.assign(npla,tdc_init);
	tof_incidence= tdc_init;pla_bool.assign(npla,0);
	pmt_bool.assign(npmt,0);
	row_bool.assign(nrow,0);
	ref_trig_bool.assign(5,0);
	veto_bool = 0;
	sbt_bool = 0;
	pattern_bool.assign(9,0);
	ref_tdc.assign(5,tdc_init);
	sbt_tdc = tdc_init;
	veto_tdc = tdc_init;
	incidence_tdc = -tdc_init;
	incidence_pla = -1;
	fDCWireHits->Clear("C");
	fPlaHits->Clear("C");

	for(int l=0;l<2; l++){

		fDCKvHits[l]->Clear();
		fDCKuHits[l]->Clear();
		fDCKpHits[l]->Clear();


		fDCKvGroups[l]->Clear();
		fDCKuGroups[l]->Clear();
		fDCKpGroups[l]->Clear();

		fDCevts[l]->Clear("C");

	}

	ftracks->Clear();



}
/////////////////////////////////////////////////////////////
void Analyze::Clearin(){

	/*
	   qdc_pmt_in->assign(npmt,0);
	   qdcorg_ch_in->assign(npmt,0);
	   qtc_pmt_in->assign(npmt,tdc_init);
	   tdc_ch_in->assign(32,tdc_init);
	   tdc_pmt_in->assign(npmt,tdc_init);
	   trail_tdc_ch_in->assign(32,tdc_init);

	   tdiff_pla_in->assign(npla, tdc_init);
	   t_pla_in->assign(npla, tdc_init);
	   qtc_pla_in->assign(npla,tdc_init);
	   qdc_pla_in->assign(npla,qdc_init);

	   tof_row_in->assign(3,tdc_init);
	   tof_pattern_in->assign(npla/2*3, tdc_init);
	   tof_sbt_pla_in->assign(npla,tdc_init);
	   tof_incidence= tdc_init;
	   pla_bool.assign(npla,0);
	   pmt_bool_in->assign(npmt,0);
	   row_bool_in->assign(nrow,0);
	   ref_trig_bool_in->assign(5,0);
	   pattern_bool_in->assign(9,0);
	   ref_tdc_in->assign(5,tdc_init);

*/

}
/////////////////////////////////////////////////////////////
void Analyze::ClearSTC(){

	if(stc_make_kv){
		/*stc_kv_l_a.assign(2, std::vector<double>(difftot_kvmax_asa[0] + 1, 0.0));
		  stc_kv_l_g.assign(2, std::vector<double>(difftot_kvmax_gnd[0] + 1, 0.0));
		  stc_ku_l_a.assign(2, std::vector<double>(difftot_kumax_asa[0] + 1, 0.0));
		  stc_ku_l_g.assign(2, std::vector<double>(difftot_kumax_gnd[0] + 1, 0.0));
		  stc_kv_r_a.assign(2, std::vector<double>(difftot_kvmax_asa[0] + 1, 0.0));
		  stc_kv_r_g.assign(2, std::vector<double>(difftot_kvmax_gnd[0] + 1, 0.0));
		  stc_ku_r_a.assign(2, std::vector<double>(difftot_kumax_asa[0] + 1, 0.0));
		  stc_ku_r_g.assign(2, std::vector<double>(difftot_kumax_gnd[0] + 1, 0.0));
		  */


		if (!cath_rat_method) {

			stc_kv_l_a.assign(2, std::vector<double>(difftot_kvmax_asa[0] + 1,0));
			stc_kv_l_g.assign(2, std::vector<double>(difftot_kvmax_gnd[0] + 1,0));
			stc_ku_l_a.assign(2, std::vector<double>(difftot_kumax_asa[0] + 1,0));
			stc_ku_l_g.assign(2, std::vector<double>(difftot_kumax_gnd[0] + 1,0));

			stc_kv_r_a.assign(2, std::vector<double>(difftot_kvmax_asa[0] + 1,0));
			stc_kv_r_g.assign(2, std::vector<double>(difftot_kvmax_gnd[0] + 1,0));
			stc_ku_r_a.assign(2, std::vector<double>(difftot_kumax_asa[0] + 1,0));
			stc_ku_r_g.assign(2, std::vector<double>(difftot_kumax_gnd[0] + 1,0));

		} else {

			stc_kv_l_a.assign(2, std::vector<double>(rattot_kvmax_asa[0] + 1,0));
			stc_kv_l_g.assign(2, std::vector<double>(rattot_kvmax_gnd[0] + 1,0));
			stc_ku_l_a.assign(2, std::vector<double>(rattot_kumax_asa[0] + 1,0));
			stc_ku_l_g.assign(2, std::vector<double>(rattot_kumax_gnd[0] + 1,0));

			stc_kv_r_a.assign(2, std::vector<double>(rattot_kvmax_asa[0] + 1,0));
			stc_kv_r_g.assign(2, std::vector<double>(rattot_kvmax_gnd[0] + 1,0));
			stc_ku_r_a.assign(2, std::vector<double>(rattot_kumax_asa[0] + 1,0));
			stc_ku_r_g.assign(2, std::vector<double>(rattot_kumax_gnd[0] + 1,0));
		}



	} //STCMakeKV
	stc_kp.assign(2, std::vector<double>(tdcprim_kpmax[0] - tdcprim_kpmin[0] + 1, 0.0));
	stc_kp_g.assign(2, std::vector<double>(tdcprim_kpmax_gnd[0] - tdcprim_kpmin_gnd[0] + 1, 0.0));
	stc_kp_a.assign(2, std::vector<double>(tdcprim_kpmax_asa[0] - tdcprim_kpmin_asa[0] + 1, 0.0));

	for(int l=0;l<2;l++){

		if(stc_make_kv){
			h_driflen_kv_l_as[l]->Reset();
			h_driflen_kv_r_as[l]->Reset();
			h_driflen_kv_l_gs[l]->Reset();
			h_driflen_kv_r_gs[l]->Reset();
			h_driflen_ku_l_as[l]->Reset();
			h_driflen_ku_r_as[l]->Reset();
			h_driflen_ku_l_gs[l]->Reset();
			h_driflen_ku_r_gs[l]->Reset();
		}
		h_driflen_kp_s[l]->Reset();
		h_driflen_kp_as[l]->Reset();
		h_driflen_kp_gs[l]->Reset();
	}


}
//////////////////////////////////////////////////////////////////
void Analyze::ClearSTCHist(){

	for(int l=0; l<2; l++){

		if(stc_make_kv){
			h_kv_dtot_l_gs[l]->Reset();
			h_kv_dtot_r_gs[l]->Reset();
			h_kv_dtot_l_as[l]->Reset();
			h_kv_dtot_r_as[l]->Reset();
			h_ku_dtot_l_gs[l]->Reset();
			h_ku_dtot_r_gs[l]->Reset();
			h_ku_dtot_l_as[l]->Reset();
			h_ku_dtot_r_as[l]->Reset();


			h_kv_rtot_l_gs[l]->Reset();
			h_kv_rtot_r_gs[l]->Reset();
			h_kv_rtot_l_as[l]->Reset();
			h_kv_rtot_r_as[l]->Reset();
			h_ku_rtot_l_gs[l]->Reset();
			h_ku_rtot_r_gs[l]->Reset();
			h_ku_rtot_l_as[l]->Reset();
			h_ku_rtot_r_as[l]->Reset();


		}
		h_kp_tdcprim_s[l]->Reset();
		h_kp_tdcprim_as[l]->Reset();
		h_kp_tdcprim_gs[l]->Reset();
		h_kp_tdcprim_as[l]->Reset();
	}

}
//////////////////////////////////////////////////////////////
void Analyze::BookHistograms(){

	for(int l=0; l<2; l++){


		h_kv_totprim_s[l] = new TH1I(Form("h_kv_totprim_s%d",l+1),Form("Q0 ToT Kv NEOLITH-s %d",l+1),1200,0,14000);
		h_ku_totprim_s[l] = new TH1I(Form("h_ku_totprim_s%d",l+1),Form("Q0 ToT Ku NEOLITH-s %d",l+1),1200,0,14000);
		h_kv_totprim_as[l] = new TH1I(Form("h_kv_totprim_asa_s%d",l+1),Form("Q0 ToT Kv ASA NEOLITH-s %d",l+1),1200,0,14000);
		h_kv_totprim_gs[l] = new TH1I(Form("h_kv_totprim_gnd_s%d",l+1),Form("Q0 ToT Kv GND NEOLITH-s %d",l+1),1200,0,14000);
		h_ku_totprim_as[l] = new TH1I(Form("h_ku_totprim_asa_s%d",l+1),Form("Q0 ToT Ku ASA NEOLITH-s %d",l+1),1200,0,14000);
		h_ku_totprim_gs[l] = new TH1I(Form("h_ku_totprim_gnd_s%d",l+1),Form("Q0 ToT Ku GND NEOLITH-s %d",l+1),1200,0,14000);
		h_kp_tdcprim_s[l] = new TH1I(Form("h_kp_tdcprim_s%d",l+1),Form("Least TDC in Group Kp NEOLITH-s %d",l+1),8001,-4000,4000);
		h_kp_tdcprim_as[l] = new TH1I(Form("h_kp_tdcprim_asa_s%d",l+1),Form("Least TDC in Group Kp ASAGI NEOLITH-s %d",l+1),8001,-4000,4000);
		h_kp_tdcprim_gs[l] = new TH1I(Form("h_kp_tdcprim_gnd_s%d",l+1),Form("Least TDC in Group Kp GND NEOLITH-s %d",l+1),8001,-4000,4000);
		h_kp_t1t2_as[l] = new TH2I(Form("h_kp_t1t2_asa_s%d",l+1),Form("T1 T2 in Group Kp ASAGI NEOLITH-s %d",l+1),8001,-4000,4000,8001,-4000,4000);
		h_kp_t1t2_gs[l] = new TH2I(Form("h_kp_t1t2_gnd_s%d",l+1),Form("T1 T2 in Group Kp GND NEOLITH-s %d",l+1),8001,-4000,4000,8001,-4000,4000);
		h_kp_dtdw_as[l] = new TH2I(Form("h_kp_dtdw_asa_s%d",l+1),Form("dT vs dW in Group Kp ASAGI NEOLITH-s %d",l+1),700,-3000,3000,201,-100,100);
		h_kp_dtdw_gs[l] = new TH2I(Form("h_kp_dtdw_gnd_s%d",l+1),Form("dT vs dW in Group Kp GND NEOLITH-s %d",l+1),700,-3000,3000,201,-100,100);
		h_kv_totid_s[l] = new TH2I(Form("h_kv_totid_s%d",l+1),Form(" ToT ID Kv NEOLITH-s %d",l+1),80,-0.5,79.5,1200,0,14000);
		h_ku_totid_s[l] = new TH2I(Form("h_ku_totid_s%d",l+1),Form(" ToT ID Ku NEOLITH-s %d",l+1),80,-0.5,79.5,1200,0,14000);
	
	
		h_kv_calib1_qid_s[l] = new TH2I(Form("h_kv_calib1_qid_s%d",l+1),Form(" ToT ID  Calib 1 Kv NEOLITH-s %d",l+1),80,-0.5,79.5,1200,0,14000);
		h_kv_calib2_qid_s[l] = new TH2I(Form("h_kv_calib2_qid_s%d",l+1),Form(" ToT ID  Calib 2 Kv NEOLITH-s %d",l+1),80,-0.5,79.5,1200,0,14000);
	
		h_ku_calib1_qid_s[l] = new TH2I(Form("h_ku_calib1_qid_s%d",l+1),Form(" ToT ID  Calib 1 Ku NEOLITH-s %d",l+1),80,-0.5,79.5,1200,0,14000);
		h_ku_calib2_qid_s[l] = new TH2I(Form("h_ku_calib2_qid_s%d",l+1),Form(" ToT ID  Calib 2 Ku NEOLITH-s %d",l+1),80,-0.5,79.5,1200,0,14000);
 
	
	
		h_kp_tdcid_s[l] = new TH2I(Form("h_kp_tdcid_s%d",l+1),Form(" TDC ID Kp NEOLITH-s %d",l+1),80,-0.5,79.5,850,-4000,4000);
		h_kv_dtot_l_s[l] = new TH1I(Form("h_kv_dtot_l_s%d",l+1),Form("diff ToT Left Kv NEOLITH-s %d",l+1),10001,0,10000);
		h_ku_dtot_l_s[l] = new TH1I(Form("h_ku_dtot_l_s%d",l+1),Form("diff ToT Left Ku NEOLITH-s %d",l+1),10001,0,10000);
		h_kv_dtot_r_s[l] = new TH1I(Form("h_kv_dtot_r_s%d",l+1),Form("diff ToT Right Kv NEOLITH-s %d",l+1),10001,0,10000);
		h_ku_dtot_r_s[l] = new TH1I(Form("h_ku_dtot_r_s%d",l+1),Form("diff ToT Right Ku NEOLITH-s %d",l+1),10001,0,10000);
		h_kv_dtot_l_gs[l] = new TH1I(Form("h_kv_dtot_l_gnd_s%d",l+1),Form("diff ToT Left Kv GND NEOLITH-s %d",l+1),10001,0,10000);
		h_ku_dtot_l_gs[l] = new TH1I(Form("h_ku_dtot_l_gnd_s%d",l+1),Form("diff ToT Left Ku GND NEOLITH-s %d",l+1),10001,0,10000);
		h_kv_dtot_r_gs[l] = new TH1I(Form("h_kv_dtot_r_gnd_s%d",l+1),Form("diff ToT Right Kv GND NEOLITH-s %d",l+1),10001,0,10000);
		h_ku_dtot_r_gs[l] = new TH1I(Form("h_ku_dtot_r_gnd_s%d",l+1),Form("diff ToT Right Ku GND NEOLITH-s %d",l+1),10001,0,10000);
		h_kv_dtot_l_as[l] = new TH1I(Form("h_kv_dtot_l_asa_s%d",l+1),Form("diff ToT Left Kv ASAGI NEOLITH-s %d",l+1),10001,0,10000);
		h_ku_dtot_l_as[l] = new TH1I(Form("h_ku_dtot_l_asa_s%d",l+1),Form("diff ToT Left Ku ASAGI NEOLITH-s %d",l+1),10001,0,10000);
		h_kv_dtot_r_as[l] = new TH1I(Form("h_kv_dtot_r_asa_s%d",l+1),Form("diff ToT Right Kv ASAGI NEOLITH-s %d",l+1),10001,0,10000);
		h_ku_dtot_r_as[l] = new TH1I(Form("h_ku_dtot_r_asa_s%d",l+1),Form("diff ToT Right Ku ASAGI NEOLITH-s %d",l+1),10001,0,10000);
		h_kv_rtot_l_gs[l] = new TH1I(Form("h_kv_rtot_l_gnd_s%d",l+1),Form("ratio ToT Left Kv GND NEOLITH-s %d",l+1),cath_rat_const+1,0,cath_rat_const );

		h_ku_rtot_l_gs[l] = new TH1I(Form("h_ku_rtot_l_gnd_s%d",l+1),Form("ratio ToT Left Ku GND NEOLITH-s %d",l+1),cath_rat_const+1,0,cath_rat_const);
		h_kv_rtot_r_gs[l] = new TH1I(Form("h_kv_rtot_r_gnd_s%d",l+1),Form("ratio ToT Right Kv GND NEOLITH-s %d",l+1),cath_rat_const+1,0,cath_rat_const);
		h_ku_rtot_r_gs[l] = new TH1I(Form("h_ku_rtot_r_gnd_s%d",l+1),Form("ratio ToT Right Ku GND NEOLITH-s %d",l+1),cath_rat_const+1,0,cath_rat_const);
		h_kv_rtot_l_as[l] = new TH1I(Form("h_kv_rtot_l_asa_s%d",l+1),Form("ratio ToT Left Kv ASAGI NEOLITH-s %d",l+1),cath_rat_const+1,0,cath_rat_const);
		h_ku_rtot_l_as[l] = new TH1I(Form("h_ku_rtot_l_asa_s%d",l+1),Form("ratio ToT Left Ku ASAGI NEOLITH-s %d",l+1),cath_rat_const+1,0,cath_rat_const);
		h_kv_rtot_r_as[l] = new TH1I(Form("h_kv_rtot_r_asa_s%d",l+1),Form("ratio ToT Right Kv ASAGI NEOLITH-s %d",l+1),cath_rat_const+1,0,cath_rat_const);
		h_ku_rtot_r_as[l] = new TH1I(Form("h_ku_rtot_r_asa_s%d",l+1),Form("ratio ToT Right Ku ASAGI NEOLITH-s %d",l+1),cath_rat_const+1,0,cath_rat_const);


		h_ku_dtot_as[l] = new TH1I(Form("h_ku_dtot_asa_s%d",l+1),Form("diff ToT  Ku ASAGI NEOLITH-s %d",l+1),10001,0,10000);
		h_kv_dtot_as[l] = new TH1I(Form("h_kv_dtot_asa_s%d",l+1),Form("diff ToT  Kv ASAGI NEOLITH-s %d",l+1),10001,0,10000);
		h_ku_dtot_gs[l] = new TH1I(Form("h_ku_dtot_gnd_s%d",l+1),Form("diff ToT  Ku GND NEOLITH-s %d",l+1),10001,0,10000);
		h_kv_dtot_gs[l] = new TH1I(Form("h_kv_dtot_gnd_s%d",l+1),Form("diff ToT  Kv GND NEOLITH-s %d",l+1),10001,0,10000);
		h_kp_dw_as[l] = new TH1I(Form("h_kp_dw_asa_s%d",l+1),Form("dW KP Asagi NEOLITH-s%d",l+1),500,-2000,2000);
		h_kp_dw_gs[l] = new TH1I(Form("h_kp_dw_gnd_s%d",l+1),Form("dW KP GND NEOLITH-s%d",l+1),500,-2000,2000);
		h_kp_dw_s[l] = new TH1I(Form("h_kp_dw_s%d",l+1),Form("dW KP NEOLITH-s%d",l+1),500,-2000,2000);
		for(int c=0; c<48;c++){
			h_kp_dw_s_cell[l][c] = new TH1I(Form("h_kp_dw_s%d_cell_%d",l+1,c),Form("dW KP NEOLITH-s%d Celd %d",l+1,c),1000,-4000,4000);
		}
		h_driflen_kp_s[l] = new TH1D(Form("h_driflen_kp_s%d",l+1),Form("Drift Length Distribution Kp NEOLITH-s %d",l+1),100,0,10 );
		h_driflen_kp_as[l] = new TH1D(Form("h_driflen_kp_as%d",l+1),Form("ASAGI Drift Length Distribution Kp NEOLITH-s %d",l+1),100,0,10 );
		h_driflen_kp_gs[l] = new TH1D(Form("h_driflen_kp_gs%d",l+1),Form("GND Drift Length Distribution Kp NEOLITH-s %d",l+1),100,0,10 );
		h_driflen_kv_l_as[l] = new TH1D(Form("h_driflen_kv_l_as%d",l+1),Form("Left Drift Length Distribution Kv Asagi NEOLITH-s %d",l+1),100,-5,5 );
		h_driflen_kv_l_gs[l] = new TH1D(Form("h_driflen_kv_l_gs%d",l+1),Form("Left Drift Length Distribution Kv GND NEOLITH-s %d",l+1),100,-5,5 );
		h_driflen_kv_r_as[l] = new TH1D(Form("h_driflen_kv_r_as%d",l+1),Form("Right Drift Length Distribution Kv Asagi NEOLITH-s %d",l+1),100,-5,5 );
		h_driflen_kv_r_gs[l] = new TH1D(Form("h_driflen_kv_r_gs%d",l+1),Form("Right Drift Length Distribution Kv GND NEOLITH-s %d",l+1),100,-5,5 );
		h_driflen_ku_l_as[l] = new TH1D(Form("h_driflen_ku_l_as%d",l+1),Form("Left Drift Length Distribution Ku Asagi NEOLITH-s %d",l+1),100,-5,5 );
		h_driflen_ku_l_gs[l] = new TH1D(Form("h_driflen_ku_l_gs%d",l+1),Form("Left Drift Length Distribution Ku GND NEOLITH-s %d",l+1),100,-5,5 );
		h_driflen_ku_r_as[l] = new TH1D(Form("h_driflen_ku_r_as%d",l+1),Form("Right Drift Length Distribution Ku Asagi NEOLITH-s %d",l+1),100,-5,5 );
		h_driflen_ku_r_gs[l] = new TH1D(Form("h_driflen_ku_r_gs%d",l+1),Form("Right Drift Length Distribution Ku GND NEOLITH-s %d",l+1),100,-5,5 );


		h_driflen_kv_gs[l] = new TH1D(Form("h_driflen_kv_gs%d",l+1),Form(" Drift Length Distribution Kv GND NEOLITH-s %d",l+1),100,-5,5 );
		h_driflen_ku_gs[l] = new TH1D(Form("h_driflen_ku_gs%d",l+1),Form(" Drift Length Distribution Ku GND NEOLITH-s %d",l+1),100,-5,5 );

		h_driflen_dchit_s[l] = new TH1D(Form("h_driflen_dchit_s%d",l+1),Form("(WHile DC Hit formation) Drift Length Distribution Kp NEOLITH-s %d",l+1),100,0,10);
		h_driflen_dchit_sign_s[l] = new TH1D(Form("h_driflen_dchit_sign_s%d",l+1),Form("(WHile DC Hit formation) Signed Drift Length Distribution Kp NEOLITH-s %d",l+1),200,-10,10);
		h_driflen_dctrack_s[l] = new TH1D(Form("h_driflen_dctrack_s%d",l+1),Form("(WHile DC Track formation) Drift Length Distribution Kp NEOLITH-s %d",l+1),100,0,10);
		h_driflen_dctrack_sign_s[l] = new TH1D(Form("h_driflen_dctrack_sign_s%d",l+1),Form("(WHile DC Track formation) Signed Drift Length Distribution Kp NEOLITH-s %d",l+1),200,-10,10);
		h_XY_s[l] = new TH2D(Form("h_XY_s%d",l+1),Form("XY image at potential plane NEOLITH-s%d",l+1),300,-400,400,300,-400,400);
		h_XYcath_s[l] = new TH2D(Form("h_XYcath_s%d",l+1),Form("XY from Cathode Planes image at potential plane NEOLITH-s%d",l+1),3000,-400,400,300,-400,400);

		h_alpha_drift[l] = new TH2D(Form("h_alpha_drift_s%d",l+1),Form("alpha(X-axis) distribution vs xdrift NEOLITH-s%d",l+1),300,-90,90,300,-0.5,9.5);

		h_alpha_x[l] = new TH2D(Form("h_alpha_x_s%d",l+1),Form("alpha(X-axis) distribution vs x NEOLITH-s%d",l+1),300,-90,90,300,-400,400);
		h_beta_y[l] = new TH2D(Form("h_beta_y_s%d",l+1),Form("beta(X-axis) distribution vs y NEOLITH-s%d",l+1),300,-90,90,300,-400,400);

		h_deltaXcath_shift_s[l] = new TH1D(Form("h_deltaXcath_shift_s%d",l+1),Form("Delta Xcath with anode wire in DC Hits with shift in NEOLITH-s%d",l+1),800,-4,4); 
		h_deltaXcath_s[l] = new TH1D(Form("h_deltaXcath_s%d",l+1 ),Form("Delta Xcath with anode wire in DC Hits without shift in NEOLITH-s%d",l+1),800,-4,4); 
		h_deltaXcath_left_s[l] = new TH1D(Form("h_deltaXcath_left_s%d",l+1 ),Form("Delta Xcath with anode wire in DC Hits without shift in NEOLITH-s%d for Left events",l+1),800,-4,4); 
		h_deltaXcath_right_s[l] = new TH1D(Form("h_deltaXcath_right_s%d",l+1 ),Form("Delta Xcath with anode wire in DC Hits without shift in NEOLITH-s%d for right events",l+1),800,-4,4); 
	
	}

	h_npvertex_basic = new TH2D("h_npvertex_basic","N-P Vertex at Middle of Layer1 before any correction",250,-300,300,250,-300,300);
	h_npvertex_corr = new TH2D("h_npvertex_corr","N-P Vertex at Middle of Layer1 after correction",250,-300,300,250,-300,300);
	h_dist_vertex = new TH1D("h_dist_vertex","Distance between Common Vertex",100,0,200);
	h_dxdy_vertex = new TH2D("h_dxdy_vertex", "dX vs dY Common Vertex",100,0,200,100,0,200);
	h_iddy_vertex = new TH2D("h_iddy_vertex", "id vs dY Common Vertex",5,-0.5,4.5,100,0,200);

	h_npvertex_b1 = new TH2D("h_npvertex_b1","np Vertex with beta <1 deg",250,-300,300,250,-300,300);
	h_npvertex_b2 = new TH2D("h_npvertex_b2","np Vertex with beta <2 deg",250,-300,300,250,-300,300);
	h_npvertex_a1 = new TH2D("h_npvertex_a1","np Vertex with alpha <1 deg",250,-300,300,250,-300,300);
	h_npvertex_a2 = new TH2D("h_npvertex_a2","np Vertex with alpha <2 deg",250,-300,300,250,-300,300);


	h_alpha = new TH1D("h_alpha","alpha distribution",150,-90,90);
	h_beta = new TH1D("h_beta","beta distribution",150,-90,90);
	h_theta = new TH1D("h_theta","theta distribution",150,0,90);
	
	h_ang_vertex = new TH1D("h_ang_vertex", "Anlge betweent the tracks of in the same layer", 100,0,90);
	h_ang_dist_vertex = new TH2D("h_ang_dist_vertex", "Angle  vs Distance between the tracks  of in the same layer", 100,0,90,100,0,200);
	h_ang_dx_vertex = new TH2D("h_ang_dx_vertex", "Angle  vs dx between the tracks  of in the same layer", 100,0,90,100,0,200);
	h_ang_dy_vertex = new TH2D("h_ang_dy_vertex", "Angle  vs dy between the tracks  of in the same layer", 100,0,90,100,0,200);

	h_x1x2 = new TH2D("h_x1x2","x1 (x-axis) x2",300,-400,400,300,-400,400);
	h_y1y2 = new TH2D("h_y1y2","y1 (x-axis) y2",300,-400,400,300,-400,400);
}

