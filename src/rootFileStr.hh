#ifndef rootFileStr_hh
#define rootFileStr_hh

//root
#include "TROOT.h"

//C, C++
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>

struct rootFileStr {
  TString fileDir;
  std::vector<TString> filenames;
  std::vector<Double_t> vbiases;
  std::vector<Double_t> temperatures;
  std::string mot;
  std::string motf;
  std::string motf_val;
  std::string motv;
  Double_t motv_val;
  std::string mott;
  Double_t mott_val;
  rootFileStr(){
    fileDir = "";
  }
  void printInfo(){
    std::cout<<"fileDir:     "<<fileDir<<std::endl;
    for (unsigned i = 0; i<filenames.size(); i++){
      std::cout<<"filename:    "<<filenames[i]<<std::endl
	       <<"vbias:       "<<vbiases[i]<<std::endl
	       <<"temperature: "<<temperatures[i]<<std::endl;
    }
  }  
  void readFromFile(TString name){
    //
    //fileDir:     ../data/S13360_3075CN_UVE_sn2_INFNfastAmpl_crosstalk/root/
    //filename:    53.000_V_T_23.471_C.txt.root
    //vbias:       53.000
    //temperature: 23.471
    //
    std::ifstream confFile(name.Data());
    if (confFile.is_open()) {
      confFile>>mot;
      if(mot != "fileDir:")
	assert(0);
      confFile>>mot;
      fileDir = mot;      
      while(confFile>>motf>>motf_val>>motv>>motv_val>>mott>>mott_val){
	if(motf != "filename:")
	  assert(0);
	if(motv != "vbias:")
	  assert(0);
	if(mott != "temperature:")
	  assert(0);
	TString filen = motf_val;
	filenames.push_back(filen);
	vbiases.push_back(motv_val);
	temperatures.push_back(mott_val);
      }
      confFile.close();
    }
    else {
      std::cout << "Unable to open file"<<std::endl; 
      assert(0);
    }
  }
};

#endif
