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

		}


		LoadParameters("map/NEOLITHs.xml");
		if(force){

	estore = new TArtEventStore();
	estore->Open(Form("ridf/neotest%04d.ridf",nRun));
	rawevent = estore->GetRawEventObject();
	int neve = 0;

	h_test= new TH1I("h_test","h_test",80,-0.5,79.5);
	while(estore->GetNextEvent()){
		std::cout << "\rProcessing Event: " << neve << std::flush;
		//Initialize
	

		Clear();
		LoadData();
		FormHitArrays();
		for(int l=0 ; l<2; l++){
                        fDCKvHits[l]->Sort();
                        fDCKuHits[l]->Sort();
                        fDCKpHits[l]->Sort();
                }

		MakeGroups();



		estore->ClearData();
		neve++;
	}//estore

		}//force

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
							hit->SetDir(*(para->GetWireDirection()));

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

void Analyze::FormHitArrays(){		

		if (tdc_ch[31]!=tdc_init) {ref_tdc[5] = tdc_ch[31];ref_trig_bool[5] = 1;
		}


		for(int ich = 0; ich<31; ich++){

			if (tdc_ch[ich]== tdc_init || trail_tdc_ch[ich]== tdc_init) continue;

			if(ich<npmt) pmt_bool[ich] =1;

			tdc_ch[ich] -= ref_tdc[5];
			trail_tdc_ch[ich] -= ref_tdc[5];

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



		//Forming Layer Hits

		Int_t nDCHits = fDCWireHits->GetEntriesFast();

		for (Int_t i = 0; i < nDCHits; i++) {
			myDCHit *hit = (myDCHit*)fDCWireHits->At(i);
			int geo_t = fDCWireHitsPara[i]->GetGeo()-21;
			if(!ref_trig_bool[geo_t]) continue;

			hit->SetTDC(hit->GetTDC()-ref_tdc[geo_t]);

			if (hit->GetDetName()->Contains("s1")) {

				if (hit->GetDetName()->Contains("Kv")) {

					fDCKvHits[0]->Add(hit);
				}
				else if (hit->GetDetName()->Contains("Ku")) {

					fDCKpHits[0]->Add(hit);
				}

				else if (hit->GetDetName()->Contains("Kp")) {

					fDCKpHits[0]->Add(hit);
				}



			}
			else if (hit->GetDetName()->Contains("s2")) {

				if (hit->GetDetName()->Contains("Kv")) {

					fDCKvHits[1]->Add(hit);
				}
				else if (hit->GetDetName()->Contains("Ku")) {

					fDCKpHits[1]->Add(hit);
				}

				else if (hit->GetDetName()->Contains("Kp")) {

					fDCKpHits[1]->Add(hit);
				}



			}




		}





}

////////////////////////////

void Analyze::MakeGroups(){

myGroups *grp = nullptr;
for(int l=0; l<2;l++){

	//KV

for (Int_t ihit = 0; ihit < fDCKvHits[l]->GetEntriesFast(); ihit++) {
    myDCHit *hit = (myDCHit *) fDCKvHits[l]->At(ihit);
    if (!hit) continue;

    int ngrps = fDCKvGroups[l]->GetEntriesFast();

    if (ngrps == 0 || TMath::Abs(hit->GetWireNum() - grp->GetEndID()) >= 2) {
        
        grp = new myGroups(); 
        fDCKvGroups[l]->Add(grp); 
        
        grp->Add(hit);
    } 
    else {
        grp->Add(hit);
    }
}
	//Kv Grouping Complete

for (Int_t ihit = 0; ihit < fDCKuHits[l]->GetEntriesFast(); ihit++) {
    myDCHit *hit = (myDCHit *) fDCKuHits[l]->At(ihit);
    if (!hit) continue;

    int ngrps = fDCKuGroups[l]->GetEntriesFast();

    if (ngrps == 0 || TMath::Abs(hit->GetWireNum() - grp->GetEndID()) >= 2) {
        
        grp = new myGroups(); 
        fDCKuGroups[l]->Add(grp); 
        
        grp->Add(hit);
    } 
    else {
        grp->Add(hit);
    }
}	//Ku Grouping Complete




}//2 Layers

}
////////////////////////////////

void Analyze::MakeDCHits(){



}
///////////////////////////////

void Analyze::MakePositionCathode(TClonesArray* dcstripgroup,TClonesArray* cathodeposition){



}
////////////////////////////

void Analyze::MakeDCPosition(TClonesArray *KUgroups,TClonesArray *KVgrousp,  TVector3 *dcpos){



}
/////////////////////////////
void Analyze::MakeTracks(){





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

		}	

	}
	myDCHitPara * para = new myDCHitPara(id, name, wireid, wirepos, wirez,dir);
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



}
/////////////////////////////////////////////////////////////
