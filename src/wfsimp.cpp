//my
#include "wfsimp.hh"

//root
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TGraph.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TRandom3.h>

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;

void wfsimp::Loop(TString histOut){
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"nentries = "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  //////////////
  //
  Double_t t_L = 0.0;
  Double_t t_R = 1000;
  Double_t t_TRG_L = 900 - 5;
  Double_t t_TRG_R = 900 + 5;
  Double_t t_DCR_L = 0.0;
  Double_t t_DCR_R = t_TRG_L;
  //
  Int_t jj = 0;
  TH1D *h1_Volt = new TH1D("h1_Volt","h1_Volt",10000,-100.0,100.0);
  TH1D *h1_a_max = new TH1D("h1_a_max","h1_a_max",10000,-100.0,100.0);
  TH1D *h1_t_max = new TH1D("h1_t_max","h1_t_max",10000,t_L-10,t_R+10);
  TH1D *h1_a_max_trg = new TH1D("h1_a_max_trg","h1_a_max_trg",10000,-100.0,100.0);
  TH1D *h1_t_max_trg = new TH1D("h1_t_max_trg","h1_t_max_trg",10000,t_L-10,t_R+10);
  TH1D *h1_a_max_dcr = new TH1D("h1_a_max_dcr","h1_a_max_dcr",10000,-100.0,100.0);
  TH1D *h1_t_max_dcr = new TH1D("h1_t_max_dcr","h1_t_max_dcr",10000,t_L-10,t_R+10);
  ///////////////////
  Double_t pedestal_V = 0.0;
  ///////////////////
  TFile* rootFile = new TFile(histOut.Data(), "RECREATE", " Histograms", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<histOut.Data()<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<histOut.Data()<<endl;
  ///////////////////
  Double_t new_a;
  Double_t new_t;
  Double_t a_max;
  Double_t t_max;
  Double_t a_max_trg;
  Double_t t_max_trg;
  Double_t a_max_dcr;
  Double_t t_max_dcr;
  //
  ///////////////////
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    jj = (Int_t)jentry;
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    ////////////////
    TGraph *gr_wf = new TGraph();
    char buffer [10];
    sprintf(buffer ,"%09d", jj);
    TString wf_ID = buffer;
    TString gr_wf_name_title = "gr_wf_"; gr_wf_name_title += wf_ID;
    gr_wf->SetNameTitle(gr_wf_name_title.Data(),gr_wf_name_title.Data());
    ////////////////
    pedestal_V = getPedastal_V(2000);
    for(Int_t i = 0; i<NPoints;i++){
      new_t = i*dt*1.0e+9;
      new_a = -1.0*(Volt[i] - pedestal_V)*1000.0;
      h1_Volt->Fill(new_a);
      gr_wf->SetPoint(i,new_t,new_a);
      t[i] = new_t;
      v[i] = new_a;
    }
    ////////////////
    gr_wf->Write();
    delete gr_wf;
    ////////////////
    getMaxAmpl( t_max, a_max, t_L, t_R);
    getMaxAmpl( t_max_dcr, a_max_dcr, t_DCR_L, t_DCR_R);
    getMaxAmpl( t_max_trg, a_max_trg, t_TRG_L, t_TRG_R);
    h1_a_max->Fill(a_max);
    h1_t_max->Fill(t_max);
    h1_a_max_trg->Fill(a_max_trg);
    h1_t_max_trg->Fill(t_max_trg);
    h1_a_max_dcr->Fill(a_max_dcr);
    h1_t_max_dcr->Fill(t_max_dcr);
    ////////////////
  }
  h1_Volt->Write();
  h1_a_max->Write();
  h1_t_max->Write();
  h1_a_max_trg->Write();
  h1_t_max_trg->Write();
  h1_a_max_dcr->Write();
  h1_t_max_dcr->Write();
  rootFile->Close();
}

Double_t wfsimp::getPedastal_V(Int_t nmax){
  Double_t vpedestal = 0.0;
  for(Int_t i = 0;i<nmax;i++)
    vpedestal += Volt[i];
  return vpedestal/nmax;
}

void wfsimp::getMaxAmpl( Double_t &t_max, Double_t &a_max, Double_t t_TRG_L, Double_t t_TRG_R){
  a_max = -999.0;
  Int_t imax = 0;
  for(Int_t i = 0;i<NPoints;i++){
    if(t[i]>= t_TRG_L && t[i]<=t_TRG_R){
      if(a_max<v[i]){
	imax = i;
	a_max = v[i];
      }
    }
  }
  t_max = t[imax];
  a_max = v[imax];
}
