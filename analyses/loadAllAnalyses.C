#include "TROOT.h"
#include "TString.h"

void loadAllAnalyses()
{

  gROOT->ProcessLine(".L DTNtupleBaseAnalyzer.C++");
  gROOT->ProcessLine(".L DTNtupleDigiAnalyzer.C++");
  gROOT->ProcessLine(".L DTNtupleTriggerAnalyzer.C++");
  gROOT->ProcessLine(".L DTNtupleSegmentAnalyzer.C++");
  gROOT->ProcessLine(".L DTNtupleTPGSimAnalyzer.C++");

}
