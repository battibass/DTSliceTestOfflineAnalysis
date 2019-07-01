#ifndef DTNtupleDigiAnalyzer_h
#define DTNtupleDigiAnalyzer_h

#include "DTNtupleBaseAnalyzer.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <map>

class DTNtupleDigiAnalyzer : public DTNtupleBaseAnalyzer 
{

 public:
  
  DTNtupleDigiAnalyzer(const TString & inFileName,
		       const TString & outFileName);

  ~DTNtupleDigiAnalyzer();

  void virtual Loop() override;

 protected:

  void book();
  void fill();
  void endJob();
  
  TFile m_outFile;

  std::vector<int> m_stations;
  float ph2DigiPedestal;

  std::map<std::string, TH1*> m_plots;
  
};

#endif
