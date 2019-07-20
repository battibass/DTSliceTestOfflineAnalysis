#ifndef DTTnPBaseAnalysis_h
#define DTTnPBaseAnalysis_h

#include "DTNtupleBaseAnalyzer.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <map>

class DTNtupleSegmentAnalyzer : public DTNtupleBaseAnalyzer 
{

 public:
  
  DTNtupleSegmentAnalyzer(const TString & inFileName,
			  const TString & outFileName);
  ~DTNtupleSegmentAnalyzer();

  virtual void Loop() override;

 protected:

  virtual void book() override;
  virtual void fill() override;
  virtual void endJob() override;
  
  TFile m_outFile;
  
  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TH2*> m_2Dplots;
  
};

#endif
