#include "TROOT.h"
#include "TString.h"

#include "DTNtupleDigiAnalyzer.h"
#include "DTNtupleTPGAnalyzer.h"

void runAllAnalyses(TString inputFile, Int_t runNumber)
{

  TString runName;
  runName.Form("run%d", runNumber);

  gSystem->Exec("mkdir -p " + runName + "/digi/");
  gSystem->Exec("mkdir -p " + runName + "/trigger/");

  auto digiAnalysis = DTNtupleDigiAnalyzer(inputFile, runName + "/digi/results_digi.root");
  digiAnalysis.Loop();

  auto triggerAnalysis = DTNtupleTPGAnalyzer(inputFile, runName + "/trigger/results_trigger.root");
  triggerAnalysis.Loop();

  gSystem->Exec("rm *d *pcm *so");

}
