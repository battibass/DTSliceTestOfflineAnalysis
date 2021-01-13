#ifndef DTNtupleSegmentAnalyzer_h
#define DTNtupleSegmentAnalyzer_h

#include "DTNtupleBaseAnalyzer.h"

#include "TFile.h"
#include "TEfficiency.h"
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
  void PreLoop();

  string m_deadFileName;
  
 protected:

  virtual void book() override;
  virtual void fill() override;
  virtual void endJob() override;

  void baseAnalysis();

  void measureEfficiency(string tag);

  void computeResidual(string tag);

  void comparisonAnalysis();

  TFile m_outFile;

  std::vector<std::string> m_tags;
  std::map<std::string, std::vector<WireId>> m_deadWires;

  std::map<std::string,float> m_timeBoxMin;
  std::map<std::string,float> m_timeBoxMax;

  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TH2*> m_2Dplots;
  std::map<std::string, TEfficiency*> m_effs;
  
};

#endif
