#include "DTDPGAnalysis/DTSliceTestOfflineAnalysis/src/DTNtupleDigiAnalyzer.h"
#include "DTDPGAnalysis/DTSliceTestOfflineAnalysis/src/DTNtupleSegmentAnalyzer.h"
#include "DTDPGAnalysis/DTSliceTestOfflineAnalysis/src/DTNtupleTriggerAnalyzer.h"

#include "TROOT.h"
#include "TSystem.h"
#include "TString.h"

#include <string>
#include <iostream>
#include <filesystem>

void printUsage(std::string program_name) {
  std::cout << "[" << program_name << "] usage:\n\n";
  std::cout << program_name << " RUN_NUMBER PATH_TO_INPUT_FILE\n\n";
}

void runAllAnalyses(TString input_file, Int_t run_number) {
  TString run_folder;
  run_folder.Form("run%d", run_number);

  gSystem->Exec("mkdir -p " + run_folder + "/digi/");
  gSystem->Exec("mkdir -p " + run_folder + "/trigger/");
  gSystem->Exec("mkdir -p " + run_folder + "/segment/");

  //CB find a more elegant solution to pass directory
  DTNtupleDigiAnalyzer digi_analysis{
      input_file, run_folder + "/digi/results_digi.root", (run_folder + "/digi/").Data()};
  digi_analysis.Loop();

  DTNtupleSegmentAnalyzer segmentAnalysis{input_file, run_folder + "/segment/results_segment.root"};
  segmentAnalysis.Loop();

  DTNtupleTriggerAnalyzer triggerAnalysis{input_file, run_folder + "/trigger/results_trigger.root"};
  triggerAnalysis.Loop();
}

int main(int argc, char* argv[]) {
  std::string program_name{argv[0]};

  if (argc != 3 || (argc == 2 && std::string{argv[1]} == "--help")) {
    printUsage(program_name);
    return EXIT_FAILURE;
  }

  auto run_number = std::stoi(std::string{argv[1]});
  auto input_file_path = std::filesystem::path{argv[2]};

  if (!std::filesystem::exists(input_file_path)) {
    std::cout << "[" << program_name << "] " << argv[2] << " is NOT a valid path!\n\n";
    printUsage(program_name);
    return EXIT_FAILURE;
  }

  runAllAnalyses(TString{argv[2]}, run_number);

  return EXIT_SUCCESS;
}
