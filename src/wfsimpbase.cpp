//my
#include "wfsimpbase.hh"

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

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;

wfsimpbase::wfsimpbase(TString fileList) : fChain(0) 
{
  ifstream indata;
  TString rootFileName;
  //TChain *theChain = new TChain("arich");
  TChain *theChain = new TChain(treeName.Data());
  indata.open(fileList.Data()); 
  assert(indata.is_open());  
  while (indata  >> rootFileName ){
    if(indata.eof()){
      std::cout<<"EOF"<<std::endl;
      break;
    }
    cout<<"        adding "<<rootFileName<<endl;
    theChain->Add(rootFileName.Data(),-1);
  }
  indata.close();
  Init(theChain);
  _fileList = fileList;
  _inFileName = "";
  _keyID = -999;
}

wfsimpbase::wfsimpbase(TString inFileName, Int_t keyID) : fChain(0) 
{
  //
  _fileList = "";
  _inFileName = inFileName;
  _keyID = keyID;
  //
  if(keyID == 1){
    ifstream indata;
    //TChain *theChain = new TChain("T");
    TChain *theChain = new TChain(treeName.Data());
    cout<<"        adding "<<inFileName<<endl;
    theChain->Add(inFileName.Data(),-1);
    Init(theChain);
  }
  else
    assert(0);
}

void wfsimpbase::tGraphInit(TGraph *gr[nChannels], TString grName, TString grTitle){
  Int_t i;
  TString grNameh;
  TString grTitleh;
  for(i = 0;i<nChannels;i++){
    grNameh = grName; grNameh += "_"; grNameh += "ch_"; grNameh += i;
    grTitleh = grTitle; grTitleh += " "; grTitleh += "ch "; grTitleh += i;
    gr[i] = new TGraph();
    gr[i]->SetTitle(grTitleh.Data());
    gr[i]->SetName(grNameh.Data());
  }
}

void wfsimpbase::h1D1Init(TH1D *h1D1[nChannels],TString h1name, TString h1Title,
		      Int_t Nbin, Float_t Vmin, Float_t Vmax){
  Int_t i;
  TString h1nameh;
  TString h1Titleh;
  for(i = 0;i<nChannels;i++){    
    h1nameh = h1name; h1nameh += "_"; h1nameh += "ch_"; h1nameh += i;
    h1Titleh = h1Title; h1nameh += " "; h1Titleh += "ch "; h1Titleh += i;
    h1D1[i] = new TH1D(h1nameh.Data(), h1Titleh.Data(),
                       Nbin, Vmin, Vmax);
  }
}

void wfsimpbase::h2D2Init(TH2D *h2D1[nChannels],TString h2name, TString h2Title,
		      Int_t Nbin1, Float_t Vmin1, Float_t Vmax1,
		      Int_t Nbin2, Float_t Vmin2, Float_t Vmax2){
  Int_t i;
  TString h2nameh;
  TString h2Titleh;
  for(i = 0;i<nChannels;i++){    
    h2nameh = h2name; h2nameh += "_"; h2nameh += "ch_"; h2nameh += i;
    h2Titleh = h2Title; h2nameh += " "; h2Titleh += "ch "; h2Titleh += i;
    h2D1[i] = new TH2D(h2nameh.Data(), h2Titleh.Data(),
                       Nbin1, Vmin1, Vmax1,
		       Nbin2, Vmin2, Vmax2);
  }  
}

void wfsimpbase::tProfInit(TProfile *tprof[nChannels],TString prname, TString prTitle,
		       Int_t Nbin, Float_t Vmin, Float_t Vmax){
  Int_t i;
  TString prnameh;
  TString prTitleh;
  for(i = 0;i<nChannels;i++){    
    prnameh = prname; prnameh += "_"; prnameh += "ch_"; prnameh += i;
    prTitleh = prTitle; prnameh += " "; prTitleh += "ch "; prTitleh += i;
    tprof[i] = new TProfile(prnameh.Data(), prTitleh.Data(), Nbin, Vmin, Vmax,"");
  }
}

double wfsimpbase::getUnixTimeFromTime(double d_year, double d_month, double d_day, double d_hour, double d_min, double d_sec){
  //time_t timer;
  struct tm y2k = {0};
  y2k.tm_year = d_year - 1900; y2k.tm_mon = d_month-1; y2k.tm_mday = d_day;
  y2k.tm_hour = d_hour;   y2k.tm_min = d_min; y2k.tm_sec = d_sec;
  return difftime(mktime(&y2k),0);
}

wfsimpbase::~wfsimpbase(){
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void wfsimpbase::Loop(TString histOut){
}

Int_t wfsimpbase::GetEntry(Long64_t entry){
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t wfsimpbase::LoadTree(Long64_t entry){
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void wfsimpbase::Init(TTree *tree){
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);
  //fChain->SetBranchAddress("evt", &evt, &b_evt);
  //fChain->SetBranchAddress("run", &run, &b_run);
  //fChain->SetBranchAddress("pValue", &pValue, &b_pValue);
  //...
  //...
  //
  //---------------------------------------------------
  // ADD HERE :
  fChain->SetBranchAddress("NPoints", &NPoints, &b_NPoints);
  fChain->SetBranchAddress("Volt", Volt, &b_Volt);
  fChain->SetBranchAddress("dt", &dt, &b_dt);
  fChain->SetBranchAddress("Vbias", &Vbias, &b_Vbias);
  fChain->SetBranchAddress("Ibias", &Ibias, &b_Ibias);
  fChain->SetBranchAddress("IDiode", &IDiode, &b_IDiode);
  fChain->SetBranchAddress("IDiodeStd", &IDiodeStd, &b_IDiodeStd);
  //---------------------------------------------------
  Notify();
}

Bool_t wfsimpbase::Notify(){
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.
  return kTRUE;
}

void wfsimpbase::Show(Long64_t entry){
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}

Int_t wfsimpbase::Cut(Long64_t entry){
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}

void wfsimpbase::printMetaData(Long64_t entry){
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nbytes = 0, nb = 0;
  if (entry<nentries) {
    LoadTree(entry);
    nb = fChain->GetEntry(entry);   nbytes += nb;
    cout<<"_inFileName "<<_inFileName<<endl
	<<"dt          "<<dt<<endl
	<<"Vbias       "<<Vbias<<endl
	<<"Ibias       "<<Ibias<<endl
	<<"IDiode      "<<IDiode<<endl
	<<"IDiodeStd   "<<IDiodeStd<<endl;
  }  
}
