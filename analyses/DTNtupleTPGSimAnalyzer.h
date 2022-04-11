#ifndef DTTnPBaseAnalysis_h
#define DTTnPBaseAnalysis_h

#include "DTNtupleTPGBaseAnalyzer.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TEfficiency.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <map>

class DTNtupleTPGSimAnalyzer : public DTNtupleTPGBaseAnalyzer 
{
  
public:
  
  DTNtupleTPGSimAnalyzer(const TString & inFileName,
			 const TString & outFileName);
  DTNtupleTPGSimAnalyzer(const TString & inFileName,
			 const TString & outFileName,
			 const bool & correctL1A);
  ~DTNtupleTPGSimAnalyzer();

  void virtual Loop() override;

protected:
  
  void book();
  void fill();
  void endJob();

private:
  
  Double_t trigPhiInRad(Double_t trigPhi, Int_t sector);
  void DisplayPh2Hits () ;  
  void DisplayPh2Prims () ;  
  void DisplayPh1Prims () ;  
  void DisplayPh1Segs () ;  
  void DisplayEventNumbers () ;  
  void getTheStupidPlots () ; 
  int get_quality_group(int quality){
    if (quality == 1 || quality == 2) return 0;
    if (quality == 3 || quality == 4) return 1;
    if (quality == 6) return 2;
    if (quality == 7) return 3;
    if (quality == 8) return 4;
    return -1; 
  }
  int get_tm_quality_group(int quality){
    if (quality == 2 || quality == 3) return 2;
    if (quality == 4) return 3;
    if (quality == 5) return 4;
    if (quality == 6) return 5;
    return -1; 
  }
  TFile m_outFile;
  
  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TH2*> m_plots2;
  std::map<std::string, TEfficiency*> m_effs;
  
  Double_t m_minMuPt;
  
  Double_t m_maxMuSegDPhi;
  Double_t m_maxMuSegDEta;
  
  Int_t m_minSegHits;

  Double_t m_maxSegTrigDPhi;
  Double_t m_maxMuTrigDPhi;

  Long64_t entryNumber;  
  Long64_t totalEntries = 0;  
  bool debug;  
  bool correctL1A; 

  //struct primitive; 
};

#endif
