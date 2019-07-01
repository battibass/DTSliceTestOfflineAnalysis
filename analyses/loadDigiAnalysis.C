#include "TROOT.h"

void loadDigiAnalysis()
{
  
  gROOT->ProcessLine(".L DTNtupleBaseAnalyzer.C++");
  gROOT->ProcessLine(".L DTNtupleDigiAnalyzer.C++");

}
