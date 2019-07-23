#ifndef DTEvtDisplay_h
#define DTEvtDisplay_h

#include "DTNtupleBaseAnalyzer.h"

#include "TFile.h"
#include <TH2.h>
#include "TH1F.h"
#include "TH2F.h"
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <sstream>
#include <map>


class EvDisp : public DTNtupleBaseAnalyzer
{

public:

  EvDisp(const TString & inFileName, const TString & outFileName);
  ~EvDisp();

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