#include "TROOT.h"
#include "TString.h"

#include "DTNtupleDigiAnalyzer.h"
#include "DTNtupleTriggerAnalyzer.h"
#include "DTNtupleSegmentAnalyzer.h"

void runAllAnalyses(TString inputFile, Int_t runNumber)
{

  TString runName;
  runName.Form("run%d", runNumber);

  gSystem->Exec("mkdir -p " + runName + "/digi/");
  gSystem->Exec("mkdir -p " + runName + "/trigger/");
  gSystem->Exec("mkdir -p " + runName + "/segment/");

  auto digiAnalysis = DTNtupleDigiAnalyzer(inputFile, runName + "/digi/results_digi.root");
  digiAnalysis.Loop();

  auto triggerAnalysis = DTNtupleTriggerAnalyzer(inputFile, runName + "/trigger/results_trigger.root");
  triggerAnalysis.Loop();

  auto segmentAnalysis = DTNtupleSegmentAnalyzer(inputFile, runName + "/segment/results_segment.root");
  segmentAnalysis.PreLoop("Ph1");
  segmentAnalysis.PreLoop("Ph2");
  segmentAnalysis.Loop();

  gSystem->Exec("rm *d *pcm *so");

}
