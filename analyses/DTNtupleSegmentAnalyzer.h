#ifndef DTTnPBaseAnalysis_h
#define DTTnPBaseAnalysis_h

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
  void PreLoop(string Tag);

  string m_deadFileName;
  
 protected:

  virtual void book() override;
  virtual void fill() override;
  virtual void endJob() override;

  void baseAnalysis();

  void measureEfficiency(string tag, int Ndead, int dead[][6],
			 DTNtupleSegment & seg, DTNtupleDigi & digi);

  void computeResidual(string tag, DTNtupleSegment & seg);

  void comparisonAnalysis();

  TFile m_outFile;
  
  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TH2*> m_2Dplots;
  std::map<std::string, TEfficiency*> m_effs;
  
};

#endif
