#include "Analyze.hh"
#include "iostream"

Analyze::Analyze(int nRun, bool force): 
	fRunNum(nRun){

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


		LoadParameters("map/NEOLITHs.xml");
		int neve =0;
		TFile *fout1 = new TFile (Form("root/sisir/Ana_Data_%d.root",nRun), "RECREATE");
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
		if(force){

			estore = new TArtEventStore();
			estore->Open(Form("ridf/neotest%04d.ridf",nRun));
			rawevent = estore->GetRawEventObject();
			neve = 0;

			h_test= new TH1D("h_test","h_test",100,-0.5,8.5);
			while(estore->GetNextEvent()){
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
			Clear();

			MakeSTC();

			ftree->Write();
		}//force
		fout1->Write();
		fout1->Close();
		delete fout1;


		TFile *fin = new TFile (Form("root/sisir/Ana_Data_%d.root",nRun), "READ");
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
TFile *fout2 =new TFile("root/test2.root","RECREATE");

		BookHistograms();

		for (Long64_t i=0; i<ftreein->GetEntries(); ++i) {
			ftreein->GetEntry(i);
			CopyInputVariables();
			FormHitArrays(0);
			for(int l=0 ; l<2; l++){
				fDCKvHits[l]->Sort();
				fDCKuHits[l]->Sort();
				fDCKpHits[l]->Sort();

				//	std::cout<<"HITs "<<fDCKvHits[l]->GetEntriesFast()<<" "<<fDCKuHits[l]->GetEntriesFast()<<" "<<fDCKvHits[l]->GetEntriesFast()<<std::endl;
			}

			MakeGroups();

			MakeDCHits();
			MakeTracks();
			Clear();
		}
 h_npvertex_basic->Draw("colz");
		fin->Close();





	}
///////////////////////////
Analyze::~Analyze(){

	Clear();

	delete listOfDCHitPara;
	;
}
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


			if(geo<21)continue; //geo = 25 QTC and timing geo=26 QDC 

			if(geo <25 &&ch ==127){if(edge==0) {ref_trig_bool[3-(24-geo)] =1; ref_tdc[geo-21]= val;}   }
			//geo < 25 DC
			if(geo <25&& ch<127){

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
						//		std::cout<<para->GetWireZPosition()<<std::endl;;
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

			if(geo == 25 ) {

				if (edge == 0 && tdc_ch[ch]== tdc_init) tdc_ch[ch] = val;
				else if (edge == 1 && tdc_ch[ch]!= tdc_init && trail_tdc_ch[ch]== tdc_init) trail_tdc_ch[ch]= val;

				if(ch == 16){veto_bool =1; veto_tdc =val;}
				else if(ch == 17){sbt_bool =1; sbt_tdc =val;}


			}

			else if( geo == 26){

				if (ch ==8) continue;
				if(ch ==15) ch = 8;
				//      if(ch == 6) {ch =4;}
				//      else if(ch==4) {ch =6;}

				if(ch >npmt) continue;
				qdc_pmt[ch] = val-qdc_ped[ch];
				qdcorg_ch[ch] = val;
			}


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
		if (tdc_ch[31]!=tdc_init) {ref_tdc[4] = tdc_ch[31];ref_trig_bool[4] = 1;
		}


		for(int ich = 0; ich<31; ich++){

			if (tdc_ch[ich]== tdc_init || trail_tdc_ch[ich]== tdc_init) continue;

			if(ich<npmt) pmt_bool[ich] =1;

			tdc_ch[ich] -= ref_tdc[4];
			trail_tdc_ch[ich] -= ref_tdc[4];

		}


		for(int ipmt = 0 ; ipmt< npmt; ipmt++){

			if (!pmt_bool[ipmt]) continue;

			tdc_pmt[ipmt] = tdc_ch[ipmt];
			qtc_pmt[ipmt] = trail_tdc_ch[ipmt] - tdc_pmt[ipmt];
			/*			h_t_ch[ipmt]->Fill(tdc_pmt[ipmt]);
						h_tid->Fill(ipmt,tdc_pmt[ipmt]);
						h_qtc_pmt[ipmt]->Fill(qtc_pmt[ipmt]);
						h_qtcid->Fill(ipmt,qtc_pmt[ipmt]);
						h_qdcid->Fill(ipmt,qdc_pmt[ipmt]);
						h_qdc_pmt[ipmt]->Fill(qdc_pmt[ipmt]);
						h_qdcorg_ch[ipmt]->Fill(qdcorg_ch[ipmt]);

*/		

		}


		for(int ipla =0; ipla<npla; ipla++){
			if (tdc_pmt[ipla]==tdc_init|| tdc_pmt[ipla+npla]== tdc_init) continue;
			pla_bool[ipla]=1;
			t_pla[ipla] = 0.5*(tdc_pmt[ipla]+tdc_pmt[ipla+npla]);
			tdiff_pla[ipla] = tdc_pmt[ipla]-tdc_pmt[ipla+npla];
			if(qtc_pmt[ipla]>0&&qtc_pmt[ipla+npla]>0)qtc_pla[ipla]= TMath::Sqrt(qtc_pmt[ipla]*qtc_pmt[ipla+6]);
			if(qdc_pmt[ipla]>0&&qdc_pmt[ipla+npla]>0)qdc_pla[ipla]= TMath::Sqrt(qdc_pmt[ipla]*qdc_pmt[ipla+6]);
			/*			h_Qtdiff_pla[ipla]->Fill(tdiff_pla[ipla],qdc_pla[ipla]);
						h_qdc_pla[ipla]->Fill(qdc_pla[ipla]);
						h_qtc_pla[ipla]->Fill(qtc_pla[ipla]);
						h_tdiff_pla[ipla]->Fill(tdiff_pla[ipla]);
						h_t_pla[ipla]->Fill(t_pla[ipla]);

						if(sbt_bool){
						h_Qtofsbt_pla[ipla]->Fill(t_pla[ipla]-sbt_tdc,qdc_pla[ipla]);

						h_tdifftofsbt_pla[ipla]->Fill(t_pla[ipla]-sbt_tdc,tdiff_pla[ipla]);
						}
						*/

		}

		for(int ipla=0; ipla<npla/2; ipla++){

			if(t_pla[ipla]==tdc_init) continue;

			if(tdc_pmt[ipla]<incidence_tdc) {incidence_tdc = tdc_pmt[ipla]; incidence_pla = ipla;}

		}
		if(sbt_bool){   tof_incidence = incidence_tdc - sbt_tdc;}
		//			h_Qtof_in->Fill(tof_incidence,qdc_pla[incidence_pla]);}


		for(int irow =0; irow<nrow;irow++){
			if(t_pla[irow] != tdc_init && t_pla[irow+nrow]!=tdc_init){
				row_bool[irow] =1;
				tof_row[irow] = t_pla[irow+nrow]-t_pla[irow];
				/*				h_tof_row[irow]->Fill(tof_row[irow]);
								h_Q1tof_row[irow]->Fill(tof_row[irow],qdc_pla[irow]);
								h_Q2tof_row[irow]->Fill(tof_row[irow],qdc_pla[irow+nrow]);
								h_Qsumtof_row[irow]->Fill(tof_row[irow],qdc_pla[irow+nrow]+qdc_pla[irow]);
								*/			}

				for(int jrow = 0; jrow<nrow;jrow++){

					//      tof_pattern[];

				}

		}

}//Force

//Forming DC Hits

Int_t nDCHits = fDCWireHits->GetEntriesFast();

for (Int_t i = 0; i < nDCHits; i++) {
	myDCHit *hit = (myDCHit*)fDCWireHits->At(i);
	int geo_t = fDCWireHitsPara[i]->GetGeo()-21;
	if(!ref_trig_bool[geo_t]) continue;
	if(force){
		hit->SetTDC(hit->GetTDC()-ref_tdc[geo_t]);
		hit->SetTrailTDC(hit->GetTrailTDC()-ref_tdc[geo_t]);
	}else {
		hit->SetTDC(hit->GetTDC());
		hit->SetTrailTDC(hit->GetTrailTDC());
	}

	if (hit->GetDetName()->Contains("s1")) {

		if (hit->GetDetName()->Contains("Kv")) {

			fDCKvHits[0]->Add(hit);
		}
		else if (hit->GetDetName()->Contains("Ku")) {

			fDCKuHits[0]->Add(hit);
		}

		else if (hit->GetDetName()->Contains("Pot")) {

			fDCKpHits[0]->Add(hit);
		}



	}
	else if (hit->GetDetName()->Contains("s2")) {

		if (hit->GetDetName()->Contains("Kv")) {

			fDCKvHits[1]->Add(hit);
		}
		else if (hit->GetDetName()->Contains("Ku")) {

			fDCKuHits[1]->Add(hit);
		}

		else if (hit->GetDetName()->Contains("Pot")) {

			fDCKpHits[1]->Add(hit);
		}



	}




}





}

////////////////////////////////////////////////////////////////////////////////////////////////
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
		}	//Ku Grouping Complete

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
			else if(TMath::Abs(hit->GetWireNum() - grp->GetEndID()) >= 2) {
				grp = new myGroups(); 
				fDCKpGroups[l]->Add(grp); 

				grp->Add(hit);


			}	

			else {
				grp->Add(hit);
			}
		}	//Kp Grouping Complete



		//	std::cout<<fDCKpGroups[l]->GetEntriesFast()<<" "<<fDCKvGroups[l]->GetEntriesFast()<<" "<<fDCKuGroups[l]->GetEntriesFast()<<std::endl;	


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
			if(mul>=3){
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
					h_kv_dtot_l_as[l]->Fill(maxtot-lefttot);
					h_kv_dtot_r_as[l]->Fill(maxtot-righttot);
					h_kv_totprim_as[l]->Fill(maxtot);
				}
				else{					
					h_kv_dtot_l_gs[l]->Fill(maxtot-lefttot);
					h_kv_dtot_r_gs[l]->Fill(maxtot-righttot);
					h_kv_totprim_gs[l]->Fill(maxtot);
				}


			}// mul>=3

		}//Loop over groups

		//Ku
		ngrps =  fDCKuGroups[l]->GetEntriesFast();
		for(int ig=0; ig<ngrps; ig++ ){
			grp = (myGroups*)fDCKuGroups[l]->At(ig);
			Int_t mul = grp->GetSize();
			if(mul>=3){
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

				h_ku_dtot_l_s[l]->Fill(maxtot-lefttot);
				h_ku_dtot_r_s[l]->Fill(maxtot-righttot);
				h_ku_totprim_s[l]->Fill(maxtot);
				if(grp->At(maxid)->IsAsagi()){
					h_ku_dtot_l_as[l]->Fill(maxtot-lefttot);
					h_ku_dtot_r_as[l]->Fill(maxtot-righttot);
					h_ku_totprim_as[l]->Fill(maxtot);
				}
				else{					
					h_ku_dtot_l_gs[l]->Fill(maxtot-lefttot);
					h_ku_dtot_r_gs[l]->Fill(maxtot-righttot);
					h_ku_totprim_gs[l]->Fill(maxtot);
				}




			}// mul>=3

		}//Loop over groups

		//Kp
		ngrps =  fDCKpGroups[l]->GetEntriesFast();

		for(int ig=0; ig<ngrps; ig++ ){
			grp = (myGroups*)fDCKpGroups[l]->At(ig);
			Int_t mul = grp->GetSize();
			if(mul>=2){
				int minid = -1;
				int maxid = -1;
				for(int ihit=0; ihit<mul; ihit++){
					Int_t wireid = grp->At(ihit)->GetWireNum();
					Int_t tdc = grp->At(ihit)->GetTDC();
					if(wireid==grp->GetMaxToTID()){maxid = ihit;}//break;
					if(wireid==grp->GetMinTDCID()){minid = ihit;}//break;
					h_kp_tdcid_s[l]->Fill(wireid,tdc);
				}//ihit
				//if(maxid==0||maxid==mul-1) continue;
				//	if(minid != maxid) continue
				Int_t maxtot = grp->At(minid)->GetToT();
				Int_t mintdc = grp->GetMinTDC();
				Int_t max2tot = maxtot;
				bool right = 0;
				if(minid == 0) {max2tot = grp->At(minid+1)->GetToT(); right =false;}
				else if(minid == mul-1) {max2tot = grp->At(minid-1)->GetToT(); right = true;}
				else if(grp->At(minid-1)->GetToT()<grp->At(minid+1)->GetToT()){max2tot = grp->At(minid-1)->GetToT(); right = false;}
				else {max2tot = grp->At(minid+1)->GetToT(); right =1;}


				h_kp_dw_s[l]->Fill((maxtot-max2tot)*(2*(right-0.5)));
				h_kp_tdcprim_s[l]->Fill(mintdc);
				if(grp->At(minid)->IsAsagi()){
					h_kp_dw_as[l]->Fill((maxtot-max2tot)*(2*(right-0.5)));
					h_kp_tdcprim_as[l]->Fill(mintdc);
				}
				else{h_kp_dw_gs[l]->Fill((maxtot-max2tot)*(2*(right-0.5)));
					h_kp_tdcprim_gs[l]->Fill(mintdc);}


			}// mul>=2

			if(mul ==1) {
				int	minid = 0;

				Int_t mintdc = grp->GetMinTDC();
				h_kp_tdcprim_s[l]->Fill(mintdc);
				if(grp->At(minid)->IsAsagi()){
					h_kp_tdcprim_as[l]->Fill(mintdc);
				}
				else{
					h_kp_tdcprim_gs[l]->Fill(mintdc);}

			} //mul==1 

		}//Loop over groups






	} //l	


}
///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void Analyze::MakeSTC(){
	TH1* h =nullptr;
	int total_int_al = 0;
	int total_int_ar = 0;
	int total_int_gl = 0;
	int total_int_gr = 0;
	int total_kp = 0;

	for (int l=0; l<2; l++){

		//Kv
		//


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
			stc_kv_l_a[l][ist] = (1- (h->Integral(h->FindBin(0),h->FindBin(ist)))/(1.*total_int_al))*(4.);
			h_driflen_kv_l_as[l]->Fill(stc_kv_l_a[l][ist]);
			h =h_kv_dtot_r_as[l];
			stc_kv_r_a[l][ist] = (1- (h->Integral(h->FindBin(0),h->FindBin(ist)))/(1.*total_int_ar))*(4.);
			h_driflen_kv_r_as[l]->Fill(stc_kv_r_a[l][ist]);

		}


		//GND
		for(int ist = 0; ist<difftot_kvmax_gnd[l]; ist++ ){
			h = h_kv_dtot_l_gs[l];
			stc_kv_l_g[l][ist] = (1- (h->Integral(h->FindBin(0),h->FindBin(ist)))/(1.*total_int_gl))*(4.);
			h_driflen_kv_l_gs[l]->Fill(stc_kv_l_g[l][ist]);

			h = h_kv_dtot_r_gs[l];
			stc_kv_r_g[l][ist] = (1- (h->Integral(h->FindBin(0),h->FindBin(ist)))/(1.*total_int_gr))*(4.);
			h_driflen_kv_r_gs[l]->Fill(stc_kv_r_g[l][ist]);
		}



		//KU


		h = h_ku_dtot_l_as[l];
		total_int_al = h->Integral(h->FindBin(0),h->FindBin(difftot_kumax_asa[l]));

		total_int_ar = h->Integral(h->FindBin(0),h->FindBin(difftot_kumax_asa[l]));
		h = h_ku_dtot_l_gs[l];
		total_int_gl = h->Integral(h->FindBin(0),h->FindBin(difftot_kumax_gnd[l]));
		h = h_ku_dtot_r_gs[l];
		total_int_gr = h->Integral(h->FindBin(0),h->FindBin(difftot_kumax_gnd[l]));


		//ASAGI 
		for(int ist = 0; ist<difftot_kumax_asa[l]; ist++ ){
			h = h_ku_dtot_l_as[l];
			stc_ku_l_a[l][ist] = (1- (h->Integral(h->FindBin(0),h->FindBin(ist)))/(1.*total_int_al))*(4.);
			h_driflen_ku_l_as[l]->Fill(stc_ku_l_a[l][ist]);

			h = h_ku_dtot_r_as[l];
			stc_ku_r_a[l][ist] = (1- (h->Integral(h->FindBin(0),h->FindBin(ist)))/(1.*total_int_ar))*(4.);
			h_driflen_ku_r_as[l]->Fill(stc_ku_r_a[l][ist]);

		}


		//GND
		for(int ist = 0; ist<difftot_kumax_gnd[l]; ist++ ){
			h = h_ku_dtot_l_gs[l];
			stc_ku_l_g[l][ist] = (1- (h->Integral(h->FindBin(0),h->FindBin(ist)))/(1.*total_int_gl))*(4.);
			h_driflen_ku_l_gs[l]->Fill(stc_ku_l_g[l][ist]);

			h = h_ku_dtot_r_gs[l];
			stc_ku_r_g[l][ist] = (1- (h->Integral(h->FindBin(0),h->FindBin(ist)))/(1.*total_int_gr))*(4.);
			h_driflen_ku_r_gs[l]->Fill(stc_ku_r_g[l][ist]);
		}



		//KP
		//
		h = h_kp_tdcprim_s[l];
		total_kp = h->Integral(h->FindBin(tdcprim_kpmin[l]),h->FindBin(tdcprim_kpmax[l]));
		for(int ist = 0; ist<tdcprim_kpmax[l]-tdcprim_kpmin[l]+1;ist++){
			int modbin = ist+tdcprim_kpmin[l];
			stc_kp[l][ist] =  (h->Integral(h->FindBin(tdcprim_kpmin[l]),h->FindBin(modbin)))/(1.*total_kp)*(8.);
			h_driflen_kp_s[l]->Fill(stc_kp[l][ist]);

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

	for (int l =0 ; l<2;l++){


		//Kv 
		ngrps = fDCKvGroups[l]->GetEntriesFast(); 

		for(int ig =0; ig<ngrps; ig ++){
			grp = (myGroups*) fDCKvGroups[l]->At(ig);
			mul = grp->GetSize();
			if (mul>=3){

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
			if (mul>=3){

				int maxid = grp->GetMaxToTID()-grp->GetStartID();
				if(maxid==0 || maxid == mul-1) continue;

				u.push_back(MakePositionCathode(grp,l));
				uid.push_back(ig);
			}

		}//Ku Grousp	



		for (int iu=0; iu<u.size();iu++){


			for(int iv =0; iv<v.size(); iv++){


				double xc = -(u[iu]+v[iv])/TMath::Sqrt(2);
				ngrps =  fDCKpGroups[l]->GetEntriesFast();
				if(TMath::Abs(static_cast<int>(std::round(TMath::Abs(xc)))%16-7)<5) continue;
				double xcr = std::round(xc/16)*16;

				for(int ig=0; ig<ngrps; ig++ ){
					grp = (myGroups*)fDCKpGroups[l]->At(ig);
					Int_t mul = grp->GetSize();
					int minid = grp->GetMinTDCID()-grp->GetStartID();
					double potpos = grp->At(minid)->GetWirePos();
					double anx[2];
					anx[0]	= potpos -8;
					anx[1] = potpos +8;
					int tdc =  grp->GetMinTDC();
					int kdir =0;
					Double_t x_drift = CalcDriftLenPot( tdc,  l);
					Double_t x_pot = -99999;
					if(TMath::Abs(anx[1]-xcr)<1){
						kdir = -1;
						x_pot = anx[1] - x_drift;
					}

					else if( TMath::Abs(anx[0]-xcr<1)){
						kdir = 1;	
						x_pot = anx[1] + x_drift;
					}
					else {continue;}
					Double_t y = 	(u[iu]-v[iv])/TMath::Sqrt(2);
					//Add Hits to myDCevt;
					Double_t z = grp->GetZ();
					int ndceve = fDCevts[l]->GetEntriesFast(); 
					myDCevt * dcevt = nullptr;
					new ((*fDCevts[l])[ndceve]) myDCevt();
					dcevt = (myDCevt*) fDCevts[l]->At(ndceve);
					dcevt->SetPos(x_pot,y,z);

				}//Loop over groups X




			} //iv

		} //iu







	}// layers 

}
///////////////////////////////

Double_t Analyze::MakePositionCathode(myGroups* grp,int l){

	int maxid = grp->GetMaxToTID()-grp->GetStartID();
	int maxtot  =grp->GetMaxToT();
	int maxid_l = maxid- 1;
	int maxid_r = maxid- 1;
	int maxid_ltot= grp->At(maxid_l)->GetToT();
	int maxid_rtot= grp->At(maxid_r)->GetToT();
	bool Asagi = grp->At(maxid)->IsAsagi();
	int dtot_l = maxtot-maxid_ltot;
	int dtot_r = maxtot-maxid_rtot;
	double pos;
	double strippos = grp->At(maxid)->GetWirePos();

	TString v =*( grp->At(maxid)->GetDir());
	if(v == 'v'){

		if(!Asagi){		
			if(dtot_l>difftot_kvmax_gnd[l]) dtot_l = difftot_kvmax_gnd[l];
			pos = strippos + stc_kv_l_g[l][dtot_l];
			if(dtot_r>difftot_kvmax_gnd[l]) dtot_r = difftot_kvmax_gnd[l];
			pos = strippos + stc_kv_r_g[l][dtot_r];
		}else{
			if(dtot_l>difftot_kvmax_asa[l]) dtot_l = difftot_kvmax_asa[l];
			pos = strippos + stc_kv_l_a[l][dtot_l];
			if(dtot_r>difftot_kvmax_asa[l]) dtot_r = difftot_kvmax_asa[l];
			pos = strippos + stc_kv_r_a[l][dtot_r];

		}


	} else if (v== 'u'){

		if(!Asagi){		
			if(dtot_l>difftot_kumax_gnd[l]) dtot_l = difftot_kumax_gnd[l];
			pos = strippos + stc_ku_l_g[l][dtot_l];
			if(dtot_r>difftot_kumax_gnd[l]) dtot_r = difftot_kumax_gnd[l];
			pos = strippos + stc_ku_r_g[l][dtot_r];
		}else{
			if(dtot_l>difftot_kumax_asa[l]) dtot_l = difftot_kumax_asa[l];
			pos = strippos + stc_ku_l_a[l][dtot_l];
			if(dtot_r>difftot_kumax_asa[l]) dtot_r = difftot_kumax_asa[l];
			pos = strippos + stc_ku_r_a[l][dtot_r];

		}



	}

	return pos;


}
////////////////////////////////////////////////////////////
Double_t Analyze::CalcDriftLenPot(int tdc, int l){

	int modtdc;
	if(tdc<=tdcprim_kpmin[l]){modtdc =0;}
	else if(tdc>=tdcprim_kpmax[l]){modtdc = tdcprim_kpmax[l] - tdcprim_kpmin[l];}
	else {modtdc = tdc-tdcprim_kpmin[l];}
	return stc_kp[l][modtdc];
}
////////////////////////////

void Analyze::MakeDCPosition(TClonesArray *KUgroups,TClonesArray *KVgrousp,  TVector3 *dcpos){



}
/////////////////////////////
void Analyze::MakeTracks(){

	myTrack *tr = nullptr;

	int n_dceve[2]={fDCevts[0]->GetEntriesFast(),fDCevts[1]->GetEntriesFast()}; 

	TVector3 *fl1pos = new TVector3(0,0,-124.95);
	TVector3 *fl2pos = new TVector3(0,0,124.95);

	for (int ieve1 = 0; ieve1<n_dceve[0];ieve1++){
		if (n_dceve[1] ==0) break;
		myTrack* track = (myTrack*)ftracks->ConstructedAt(ftracks->GetEntriesFast());
		track->SetDC1evt((myDCevt*)fDCevts[0]->At(ieve1));
		track->SetConvPos(fl1pos);
		track->SetCatPos(fl2pos);
		for (int ieve2 = 0; ieve2< n_dceve[1]; ieve2++){

			track->SetDC2evt((myDCevt*)fDCevts[1]->At(ieve2));	

			track->Calibrate();

			bool fconv =false;
			bool fcat = false;	


			for (int ipla =0 ; ipla<3; ipla ++){
				if (! pla_bool[ipla]) continue;
				if (TMath::Abs(track->GetnpVertex().Y()-y_pla[ipla])<=50){
					fconv = true;
					track->SetConvPos(0,y_pla[ipla],-124.95);
					break;
				}

			}// Layer 1 Plastic Hit Check

			for (int ipla =3 ; ipla<6; ipla ++){
				if (! pla_bool[ipla]) continue;
				if (TMath::Abs(track->GetpCatch().Y()-y_pla[ipla])<=50){
					fcat = true;
					track->SetCatPos(0,y_pla[ipla],124.95);
					break;
				}

			}// Layer 2 plastic hit check
			h_npvertex_basic->Fill(track->GetnpVertex().X(), track->GetnpVertex().Y());
			if(!(fconv && fcat)) {
				if(ftracks->GetEntriesFast()>0)
					ftracks->RemoveAt(ftracks->GetEntriesFast()-1);
			}


		}//NEOLITH-s2 evts


	}// NEOLITH-s1 evts	


}
///////////////////////////////

void Analyze::ReconstructSTC(){



}
//////////////////////////////

void Analyze::ReconstructTracks(){


}



////////////////////////////////

const myDCHitPara* Analyze::FindDCHitPara(TArtRIDFMap *rmap) const
{
	std::map<TArtRIDFMap, myDCHitPara *>::const_iterator itr = dc_pmap.find(*rmap);
	if(itr != dc_pmap.end()) return (myDCHitPara*)itr->second;
	else return 0;
}
/////////////////////////////////////
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
			if (strcmp(node->GetNodeName(), "dir") == 0)
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

		fDCevts[l]->Clear();

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
void Analyze::BookHistograms(){

	for(int l=0; l<2; l++){


		h_kv_totprim_s[l] = new TH1I(Form("h_kv_totprim_s%d",l+1),Form("Q0 ToT Kv NEOLITH-s %d",l+1),1200,0,14000);
		h_ku_totprim_s[l] = new TH1I(Form("h_ku_totprim_s%d",l+1),Form("Q0 ToT Ku NEOLITH-s %d",l+1),1200,0,14000);
		h_kv_totprim_as[l] = new TH1I(Form("h_kv_totprim_asa_s%d",l+1),Form("Q0 ToT Kv ASA NEOLITH-s %d",l+1),1200,0,14000);
		h_kv_totprim_gs[l] = new TH1I(Form("h_kv_totprim_gnd_s%d",l+1),Form("Q0 ToT Kv GND NEOLITH-s %d",l+1),1200,0,14000);
		h_ku_totprim_as[l] = new TH1I(Form("h_ku_totprim_asa_s%d",l+1),Form("Q0 ToT Ku ASA NEOLITH-s %d",l+1),1200,0,14000);
		h_ku_totprim_gs[l] = new TH1I(Form("h_ku_totprim_gnd_s%d",l+1),Form("Q0 ToT Ku GND NEOLITH-s %d",l+1),1200,0,14000);
		h_kp_tdcprim_s[l] = new TH1I(Form("h_kp_tdcprim_s%d",l+1),Form("Least TDC in Group Kp NEOLITH-s %d",l+1),850,-4000,4000);
		h_kp_tdcprim_gs[l] = new TH1I(Form("h_kp_tdcprim_gnd_s%d",l+1),Form("Least TDC in Group Kp GND NEOLITH-s %d",l+1),850,-4000,4000);
		h_kp_tdcprim_as[l] = new TH1I(Form("h_kp_tdcprim_asa_s%d",l+1),Form("Least TDC in Group Kp ASAGI NEOLITH-s %d",l+1),850,-4000,4000);
		h_kv_totid_s[l] = new TH2I(Form("h_kv_totid_s%d",l+1),Form(" ToT ID Kv NEOLITH-s %d",l+1),80,-0.5,79.5,1200,0,14000);
		h_ku_totid_s[l] = new TH2I(Form("h_ku_totid_s%d",l+1),Form(" ToT ID Ku NEOLITH-s %d",l+1),80,-0.5,79.5,1200,0,14000);
		h_kp_tdcid_s[l] = new TH2I(Form("h_kp_tdcid_s%d",l+1),Form(" TDC ID Kp NEOLITH-s %d",l+1),80,-0.5,79.5,850,-4000,4000);
		h_kv_dtot_l_s[l] = new TH1I(Form("h_kv_dtot_l_s%d",l+1),Form("diff ToT Left Kv NEOLITH-s %d",l+1),450,0,10000);
		h_ku_dtot_l_s[l] = new TH1I(Form("h_ku_dtot_l_s%d",l+1),Form("diff ToT Left Ku NEOLITH-s %d",l+1),450,0,10000);
		h_kv_dtot_r_s[l] = new TH1I(Form("h_kv_dtot_r_s%d",l+1),Form("diff ToT Right Kv NEOLITH-s %d",l+1),450,0,10000);
		h_ku_dtot_r_s[l] = new TH1I(Form("h_ku_dtot_r_s%d",l+1),Form("diff ToT Right Ku NEOLITH-s %d",l+1),450,0,10000);
		h_kv_dtot_l_gs[l] = new TH1I(Form("h_kv_dtot_l_gnd_s%d",l+1),Form("diff ToT Left Kv GND NEOLITH-s %d",l+1),450,0,10000);
		h_ku_dtot_l_gs[l] = new TH1I(Form("h_ku_dtot_l_gnd_s%d",l+1),Form("diff ToT Left Ku GND NEOLITH-s %d",l+1),450,0,10000);
		h_kv_dtot_r_gs[l] = new TH1I(Form("h_kv_dtot_r_gnd_s%d",l+1),Form("diff ToT Right Kv GND NEOLITH-s %d",l+1),450,0,10000);
		h_ku_dtot_r_gs[l] = new TH1I(Form("h_ku_dtot_r_gnd_s%d",l+1),Form("diff ToT Right Ku GND NEOLITH-s %d",l+1),450,0,10000);
		h_kv_dtot_l_as[l] = new TH1I(Form("h_kv_dtot_l_asa_s%d",l+1),Form("diff ToT Left Kv ASAGI NEOLITH-s %d",l+1),450,0,10000);
		h_ku_dtot_l_as[l] = new TH1I(Form("h_ku_dtot_l_asa_s%d",l+1),Form("diff ToT Left Ku ASAGI NEOLITH-s %d",l+1),450,0,10000);
		h_kv_dtot_r_as[l] = new TH1I(Form("h_kv_dtot_r_asa_s%d",l+1),Form("diff ToT Right Kv ASAGI NEOLITH-s %d",l+1),450,0,10000);
		h_ku_dtot_r_as[l] = new TH1I(Form("h_ku_dtot_r_asa_s%d",l+1),Form("diff ToT Right Ku ASAGI NEOLITH-s %d",l+1),450,0,10000);

		h_kp_dw_as[l] = new TH1I(Form("h_kp_dw_asa_s%d",l+1),Form("dW KP Asagi NEOLITH-s%d",l+1),500,-2000,2000);	
		h_kp_dw_gs[l] = new TH1I(Form("h_kp_dw_gnd_s%d",l+1),Form("dW KP GND NEOLITH-s%d",l+1),500,-2000,2000);	
		h_kp_dw_s[l] = new TH1I(Form("h_kp_dw_s%d",l+1),Form("dW KP NEOLITH-s%d",l+1),500,-2000,2000);	

		h_driflen_kp_s[l] = new TH1D(Form("h_driflen_kp_s%d",l+1),Form("Drift Length Distribution Kp NEOLITH-s %d",l+1),100,0,10 );
		h_driflen_kv_l_as[l] = new TH1D(Form("h_driflen_kv_l_as%d",l+1),Form("Left Drift Length Distribution Kv Asagi NEOLITH-s %d",l+1),100,0,5 );
		h_driflen_kv_l_gs[l] = new TH1D(Form("h_driflen_kv_l_gs%d",l+1),Form("Left Drift Length Distribution Kv GND NEOLITH-s %d",l+1),100,0,5 );
		h_driflen_kv_r_as[l] = new TH1D(Form("h_driflen_kv_r_as%d",l+1),Form("Right Drift Length Distribution Kv Asagi NEOLITH-s %d",l+1),100,0,5 );
		h_driflen_kv_r_gs[l] = new TH1D(Form("h_driflen_kv_r_gs%d",l+1),Form("Right Drift Length Distribution Kv GND NEOLITH-s %d",l+1),100,0,5 );
		h_driflen_ku_l_as[l] = new TH1D(Form("h_driflen_ku_l_as%d",l+1),Form("Left Drift Length Distribution Ku Asagi NEOLITH-s %d",l+1),100,0,5 );
		h_driflen_ku_l_gs[l] = new TH1D(Form("h_driflen_ku_l_gs%d",l+1),Form("Left Drift Length Distribution Ku GND NEOLITH-s %d",l+1),100,0,5 );
		h_driflen_ku_r_as[l] = new TH1D(Form("h_driflen_ku_r_as%d",l+1),Form("Right Drift Length Distribution Ku Asagi NEOLITH-s %d",l+1),100,0,5 );
		h_driflen_ku_r_gs[l] = new TH1D(Form("h_driflen_ku_r_gs%d",l+1),Form("Right Drift Length Distribution Ku GND NEOLITH-s %d",l+1),100,0,5 );




	}

	h_npvertex_basic = new TH2D("h_npvertex_basic","N-P Vertex at Middle of Layer1 before any correction",250,-300,300,250,-300,300);

}



