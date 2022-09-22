#ifndef wfsimp_hh
#define wfsimp_hh

//My
#include "wfsimpbase.hh"

//root
#include <TROOT.h>

class TChain;
class TFile;
class TTree;
class TString;
class TBranch;


class wfsimp: public wfsimpbase {
public:
  wfsimp(TString fileList) : wfsimpbase(fileList)
  {
  }

  wfsimp(TString file, Int_t key) : wfsimpbase(file, key)
  {
  }

  void Loop(TString histOut);
  Double_t getPedastal_V(Int_t nmax);
  void getMaxAmpl( Double_t &t_max, Double_t &a_max, Double_t t_TRG_L, Double_t t_TRG_R);
  Double_t v[5000];
  Double_t t[5000];
};

#endif
