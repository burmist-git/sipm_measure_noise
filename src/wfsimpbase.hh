#ifndef wfsimpbase_hh
#define wfsimpbase_hh

#include <TROOT.h>

class TChain;
class TFile;
class TTree;
class TString;
class TBranch;
class TGraph;
class TH1D;
class TH2D;
class TProfile;

class wfsimpbase {

public :
  wfsimpbase(TString fileList);
  wfsimpbase(TString inFileName, Int_t keyID);
  ~wfsimpbase();
  Int_t GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
  void Init(TTree *tree);
  void Loop(TString histOut);
  Bool_t Notify();
  void Show(Long64_t entry = -1);
  Int_t Cut(Long64_t entry);
  void printMetaData(Long64_t entry);

protected :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
  //
  Int_t NPoints;
  Double_t Volt[5000];
  Double_t dt;
  Double_t Vbias;
  Double_t Ibias;
  Double_t IDiode;
  Double_t IDiodeStd;
  //
  ///////////////////
  TString _fileList;
  TString _inFileName;
  Int_t _keyID;
  ///////////////////
  //
  //---------------------------------------------------
  // ADD HERE :
  //Tree name
  const TString treeName = "T";
  static const Int_t nChannels = 10;  
  //---------------------------------------------------
  
  // List of branches
  //TBranch        *b_evt;
  //TBranch        *b_run;
  //TBranch        *b_pValue;
  //...
  //...
  //
  //---------------------------------------------------
  // ADD HERE :
  TBranch *b_NPoints;
  TBranch *b_Volt;
  TBranch *b_dt;
  TBranch *b_Vbias;
  TBranch *b_Ibias;
  TBranch *b_IDiode;
  TBranch *b_IDiodeStd;
  //---------------------------------------------------
  void tGraphInit(TGraph *gr[nChannels], TString grName, TString grTitle);
  void h1D1Init(TH1D *h1D1[nChannels],TString h1name, TString h1Title,
		Int_t Nbin, Float_t Vmin, Float_t Vmax);
  void h2D2Init(TH2D *h2D1[nChannels],TString h2name, TString h2Title,
                Int_t Nbin1, Float_t Vmin1, Float_t Vmax1,
                Int_t Nbin2, Float_t Vmin2, Float_t Vmax2);
  void tProfInit(TProfile *tprof[nChannels],TString prname, TString prTitle,
                 Int_t Nbin, Float_t Vmin, Float_t Vmax);
  double getUnixTimeFromTime(double d_year, double d_month, double d_day, double d_hour, double d_min, double d_sec);  
  //
  
};

#endif
