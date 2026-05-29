/*
 *This Macro is used to Analyze for Plastic Scintillators 
 *
 *This currently takes only one hit per scintillator for timing and QTC
 *
 *
 *Usage
 *root[0] .L macros/sisir/plachk.cc
 *root[1] plachk(nRun)
 *
 *
 *
 * */




/*#include "TArtEventStore.hh"
#include "TArtRawEventObject.hh"
#include "TArtRawSegmentObject.hh"
#include "TArtRawDataObject.hh"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"

#include <iostream>
*/

const int npmt = 12;
const int npla = 6;
const int nrow = 3;

const int ref_trig_ch = 31;

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
const int tdc_init = -999999;

//std::vector<int>qdc_ped= {91,45,55,65,90,80,62,90,95,87,100,81,90};
std::vector<int>qdc_ped =  {55,13,57,27,66,49,48,55,60,68,65,67};
//std::vector<int>qdc_ped(12);

void plachk(Int_t nRun, bool sbt_evt = 0){

	TArtEventStore *estore = new TArtEventStore();
	estore->Open(Form("ridf/neotest%04d.ridf",nRun));
	TArtRawEventObject *rawevent = estore->GetRawEventObject();

	TFile *fout = new TFile(Form("root/sisir/plachk_neotest%04d_sbt%d.root", nRun,sbt_evt),"RECREATE");
	//TFile *fout = new TFile(Form("root/sisir/plachk_neotest%04d_pedestal.root", nRun),"RECREATE");

	TH1 *h_qtc_pmt[npmt];
	TH1 *h_qdc_pmt[npmt];
	TH1 *h_qdcorg_ch[npmt];
	TH1 *h_t_ch[npmt];
	TH2 *h_tid = new TH2I("h_tid","Leading Edge vs ID",32,-0.5,31.5,tdc_bins,tdc_min ,tdc_max ) ;
	TH2 *h_qtcid =  new TH2I("h_qtcid","QTC (ToT) vs ID",12,-0.5,11.5,qtc_bins,qtc_min,qtc_max ) ;
	;
	TH1 *h_qdcid = new TH2I("h_qdcid","QTC (ToT) vs ID",12,-0.5,11.5,qdc_bins,qdc_min,qdc_max ) ;
	;

	for(int i = 0; i<npmt; i++){
		h_qtc_pmt[i] = new TH1I(Form("h_qtc_pmt%d",i),Form("QTC Distribution for Ch %d",i),qtc_bins,0,qtc_max);

		h_qdc_pmt[i] = new TH1I(Form("h_qdc_pmt%d",i),Form("QDC Distribution for Ch %d",i),qdc_bins,qdc_min,qdc_max);
		h_qdcorg_ch[i] = new TH1I(Form("h_qdcorg_ch%d",i),Form("Original QDC Distribution for Ch %d",i),qdc_bins,qdc_min,qdc_max);


		h_t_ch[i] = new TH1I(Form("h_t_ch%d",i),Form("Leading Edge Distribution for Ch %d",i),tdc_bins,tdc_min,tdc_max);

	}

	TH1 *h_qdc_pla[6];
	TH1 *h_qtc_pla[6];
	TH1 *h_tdiff_pla[6];
	TH1 *h_Qtdiff_pla[6];
	TH1 *h_t_pla[6];
	TH1 *h_Qtofsbt_pla[6];
	TH1 *h_tdifftofsbt_pla[6];

	for (int i =0; i<npla;i++){

		h_qdc_pla[i] = new TH1D(Form("h_qdc_pla%d",i),Form("QDC  Distribution for Pla %d",i),qdc_bins,qdc_min,qdc_max);	     
		h_qtc_pla[i] = new TH1D(Form("h_qtc_pla%d",i),Form("QTC Distribution for Pla %d",i),qtc_bins,qtc_min,qtc_max);
		if(i!=3){

			h_tdiff_pla[i] = new TH1D(Form("h_tdiff_pla%d",i),Form("Time Difference Distribution for Plastic %d",i),500,-500,500);

			h_Qtdiff_pla[i] = new TH2D(Form("h_Qtdiff_pla%d",i),Form("QDC vs Time Difference Distribution for Plastic %d",i),500,-500,500,400,1,1000);

			h_tdifftofsbt_pla[i] = new TH2D(Form("h_tdifftofsbt_pla%d",i),Form("Time Differece vs ToF_SBT for Plastic %d",i),tof_sbt_bins, tof_sbt_min,tof_sbt_max,500,-500,500);
		} else{

			h_tdiff_pla[i] = new TH1D(Form("h_tdiff_pla%d",i),Form("Time Difference Distribution for Plastic %d",i),500,-1000,000);
			h_Qtdiff_pla[i] = new TH2D(Form("h_Qtdiff_pla%d",i),Form("QDC vs Time Difference Distribution for Plastic %d",i),500,-1000,500,400,1,1000);

			h_tdifftofsbt_pla[i] = new TH2D(Form("h_tdifftofsbt_pla%d",i),Form("Time Differece vs ToF_SBT for Plastic %d",i),tof_sbt_bins, tof_sbt_min,tof_sbt_max,500,-1000,500);
		}

		h_t_pla[i] = new TH1D(Form("h_t_pla%d",i),Form("Time Distribution for Pla %d",i),tdc_bins,tdc_min,tdc_max);


		h_Qtofsbt_pla[i] = new TH2D(Form("h_Qtofsbt_pla%d",i),Form("QDC vs ToF from SBT for Plastic %d",i),tof_sbt_bins,tof_sbt_min,tof_sbt_max,qdc_bins, qdc_min,qdc_max);


	}

	TH1*   h_Qtof_in = new TH2D("h_Qtof_in","QDC vs ToF from SBT for Plastic ",tof_sbt_bins,tof_sbt_min,tof_sbt_max,qdc_bins, qdc_min,qdc_max);
	TH1 * h_tof_row[nrow];
	TH1 * h_Qsum_row[nrow];
	TH1 * h_Q1tof_row[nrow];
	TH1 * h_Q2tof_row[nrow];
	TH1 * h_Qsumtof_row[nrow];

	for (int i=0; i<nrow;i++){

		h_tof_row[i] = new TH1D(Form("h_tof_row%d",i),Form("Time of Flight for the Row %d",i),tof_bins, tof_min, tof_max);
		h_Qsum_row[i] = new TH1D(Form("h_Qsum_row%d",i),Form("Total Light Ouptut for the Row %d",i),2*qdc_bins, qdc_min, 2*qdc_max);	
		h_Q1tof_row[i] = new TH2D(Form("h_Q1tof_row%d",i),Form("Q1 vs ToF for the Row %d",i),tof_bins, tof_min, tof_max,2*qdc_bins, qdc_min, 1*qdc_max);	
		h_Q2tof_row[i] = new TH2D(Form("h_Q2tof_row%d",i),Form("Q2 vs ToF for the Row %d",i),tof_bins, tof_min, tof_max,2*qdc_bins, qdc_min, 1*qdc_max);
		h_Qsumtof_row[i] = new TH2D(Form("h_Qsumtof_row%d",i),Form("Qsum vs ToF for the Row %d",i),tof_bins, tof_min, tof_max,2*qdc_bins, qdc_min, 1*qdc_max);

	}


	TTree * ftree = new TTree("tree","Events Tree");




	int neve = 0;
	std::vector<int> qdcorg_ch(npmt,qdc_init);
	std::vector<int> qdc_pmt(npmt,qdc_init);
	std::vector<int> tdc_ch(32,tdc_init);
	std::vector<int> tdc_pmt(npmt,tdc_init);
	std::vector<int> trail_tdc_ch(32,tdc_init);
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
	bool ref_trig_bool = 0;
	bool veto_bool = 0;
	bool sbt_bool = 0;
	int ref_tdc;
	int sbt_tdc;
	int veto_tdc;
	int incidence_tdc;
	int tof_incidence;
	std::vector<int> tof_sbt_pla;
	int incidence_pla;



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
	ftree->Branch("ref_trig_bool_25",&ref_trig_bool);
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


	//initializing constants
	ftree->Branch("qdc_ped",&qdc_ped);

	while(estore->GetNextEvent()){

		//Initialize

		qdc_pmt.assign(npmt,0);
		qdcorg_ch.assign(npmt,0);
		qtc_pmt.assign(npmt,tdc_init);
		tdc_ch.assign(32,tdc_init);
		trail_tdc_ch.assign(32,tdc_init);

		tdiff_pla.assign(npla, tdc_init);
		t_pla.assign(npla, tdc_init);
		qtc_pla.assign(npla,tdc_init);
		qdc_pla.assign(npla,qdc_init);

		tof_row.assign(3,tdc_init);
		tof_pattern.assign(npla/2*3, tdc_init);
		tof_sbt_pla.assign(npla,tdc_init);
		tof_incidence= tdc_init;
		pla_bool.assign(npla,0);
		pmt_bool.assign(npmt,0);
		row_bool.assign(nrow,0);
		ref_trig_bool = 0;
		veto_bool = 0;
		sbt_bool = 0;
		pattern_bool.assign(9,0);
		ref_tdc = tdc_init;
		sbt_tdc = tdc_init;
		veto_tdc = tdc_init;
		incidence_tdc = -tdc_init;
		incidence_pla = -1;



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

				if(geo<25)continue; //geo = 25 QTC and timing geo=26 QDC 

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
				//	if(ch == 6) {ch =4;}
				//	else if(ch==4) {ch =6;}

					if(ch >npmt) continue;	
					qdc_pmt[ch] = val-qdc_ped[ch];
					qdcorg_ch[ch] = val;
				}


			}


		}

		if (tdc_ch[ref_trig_ch]!=tdc_init) {ref_tdc = tdc_ch[ref_trig_ch];ref_trig_bool = 1;
		}
		if(! ref_trig_bool) continue;

		if(sbt_evt){if(!sbt_bool) continue;}

		for(int ich = 0; ich<31; ich++){

			if (tdc_ch[ich]== tdc_init || trail_tdc_ch[ich]== tdc_init) continue;

			if(ich<npmt) pmt_bool[ich] =1; 

			tdc_ch[ich] -= ref_tdc;
			trail_tdc_ch[ich] -= ref_tdc;

		}



		for(int ipmt = 0 ; ipmt< npmt; ipmt++){

			if (!pmt_bool[ipmt]) continue;
			tdc_pmt[ipmt] = tdc_ch[ipmt];
			qtc_pmt[ipmt] = trail_tdc_ch[ipmt] - tdc_pmt[ipmt];
			h_t_ch[ipmt]->Fill(tdc_pmt[ipmt]); 
			h_tid->Fill(ipmt,tdc_pmt[ipmt]);
			h_qtc_pmt[ipmt]->Fill(qtc_pmt[ipmt]); 
			h_qtcid->Fill(ipmt,qtc_pmt[ipmt]); 
			h_qdcid->Fill(ipmt,qdc_pmt[ipmt]); 
			h_qdc_pmt[ipmt]->Fill(qdc_pmt[ipmt]);
			h_qdcorg_ch[ipmt]->Fill(qdcorg_ch[ipmt]);

		}



		for(int ipla =0; ipla<npla; ipla++){
			if (tdc_pmt[ipla]==tdc_init|| tdc_pmt[ipla+npla]== tdc_init) continue;
			pla_bool[ipla]=1;
			t_pla[ipla] = 0.5*(tdc_pmt[ipla]+tdc_pmt[ipla+npla]);
			tdiff_pla[ipla] = tdc_pmt[ipla]-tdc_pmt[ipla+npla];
			if(qtc_pmt[ipla]>0&&qtc_pmt[ipla+npla]>0)qtc_pla[ipla]= sqrt(qtc_pmt[ipla]*qtc_pmt[ipla+6]); 
			if(qdc_pmt[ipla]>0&&qdc_pmt[ipla+npla]>0)qdc_pla[ipla]= sqrt(qdc_pmt[ipla]*qdc_pmt[ipla+6]); 
			h_Qtdiff_pla[ipla]->Fill(tdiff_pla[ipla],qdc_pla[ipla]);
			h_qdc_pla[ipla]->Fill(qdc_pla[ipla]);
			h_qtc_pla[ipla]->Fill(qtc_pla[ipla]);
			h_tdiff_pla[ipla]->Fill(tdiff_pla[ipla]);
			h_t_pla[ipla]->Fill(t_pla[ipla]);

			if(sbt_bool){	
				h_Qtofsbt_pla[ipla]->Fill(t_pla[ipla]-sbt_tdc,qdc_pla[ipla]);

				h_tdifftofsbt_pla[ipla]->Fill(t_pla[ipla]-sbt_tdc,tdiff_pla[ipla]);
			}	

		}

		for(int ipla=0; ipla<npla/2; ipla++){

			if(t_pla[ipla]==tdc_init) continue;

			if(tdc_pmt[ipla]<incidence_tdc) {incidence_tdc = tdc_pmt[ipla]; incidence_pla = ipla;}

		}
		if(sbt_bool){	tof_incidence = incidence_tdc - sbt_tdc;
			h_Qtof_in->Fill(tof_incidence,qdc_pla[incidence_pla]);}


		for(int irow =0; irow<nrow;irow++){
			if(t_pla[irow] != tdc_init && t_pla[irow+nrow]!=tdc_init){
				row_bool[irow] =1;
				tof_row[irow] = t_pla[irow+nrow]-t_pla[irow];
				h_tof_row[irow]->Fill(tof_row[irow]);
				h_Q1tof_row[irow]->Fill(tof_row[irow],qdc_pla[irow]);	
				h_Q2tof_row[irow]->Fill(tof_row[irow],qdc_pla[irow+nrow]);
				h_Qsumtof_row[irow]->Fill(tof_row[irow],qdc_pla[irow+nrow]+qdc_pla[irow]);
			}

			for(int jrow = 0; jrow<nrow;jrow++){

				//	tof_pattern[];

			}

		}



		estore->ClearData();
		neve++;

		ftree->Fill();
	}

	fout->Write();
	// fout->Close();

}



