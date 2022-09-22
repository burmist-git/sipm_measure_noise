//my
#include "src/wfsimp.hh"
#include "src/rootFileStr.hh"

//root
#include "TROOT.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGraph.h"

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]){
  if(argc == 4 && atoi(argv[1])==0){
    TString rootFilesList = argv[2];
    TString outRootFileF = argv[3];
    cout<<"--> Parameter calculation from the WF <--"<<endl
	<<"rootFilesList : "<<rootFilesList<<endl
	<<"outRootFileF  : "<<outRootFileF<<endl;
    wfsimp a(rootFilesList);
    a.Loop(outRootFileF);
  }
  else if(argc == 4 && atoi(argv[1])==1){
    TString inRootFiles = argv[2];
    TString outRootFileF = argv[3];
    cout<<"--> Parameter calculation from the WF <--"<<endl
	<<"inRootFiles : "<<inRootFiles<<endl
	<<"outRootFileF  : "<<outRootFileF<<endl;
    wfsimp a( inRootFiles, atoi(argv[1]));
    a.Loop(outRootFileF);
  }
  else if(argc == 3 && atoi(argv[1])==2){
    TString inRootFiles = argv[2];
    cout<<"--> Parameter calculation from the WF <--"<<endl
	<<"inRootFiles : "<<inRootFiles<<endl;
    wfsimp a( inRootFiles, 1);
    a.printMetaData(0);
  }
  else if(argc == 3 && atoi(argv[1])==3){
    TString inRootInfoFiles = argv[2];
    cout<<"--> Parameter calculation from the WF <--"<<endl
	<<"inRootInfoFiles : "<<inRootInfoFiles<<endl;
    rootFileStr root_file_str;
    root_file_str.readFromFile(inRootInfoFiles);
    root_file_str. printInfo();
  }
  else{
    cout<<" --> ERROR in input arguments "<<endl
	<<" runID [1] = 0 (execution ID number)"<<endl
      	<<"       [2] - file with list of the root files"<<endl
	<<"       [3] - name of root file with histograms"<<endl;
    cout<<" runID [1] = 1 (execution ID number)"<<endl
      	<<"       [2] - in root files"<<endl
	<<"       [3] - name of root file with histograms"<<endl;
    cout<<" runID [1] = 2 (execution ID number) - print meta data"<<endl
      	<<"       [2] - in root files"<<endl;
    cout<<" runID [1] = 3 (execution ID number) - test input root info. file"<<endl
      	<<"       [2] - input root info. files"<<endl;
  }
  return 0;
}
