#include "TROOT.h"

void loadTPGAnalysis()
{
  
  gROOT->ProcessLine(".L DTNtupleBaseAnalyzer.C++");
  gROOT->ProcessLine(".L DTNtupleTPGAnalyzer.C++");

}
