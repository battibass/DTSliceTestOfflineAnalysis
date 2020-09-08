#include "TROOT.h"
#include "TString.h"

void loadEvDispAnalysis()
{

  gROOT->ProcessLine(".L DTNtupleBaseAnalyzer.C++");
  gROOT->ProcessLine(".L EvDisp.C++");

}
