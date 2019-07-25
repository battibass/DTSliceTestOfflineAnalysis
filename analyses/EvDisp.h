#ifndef DTEvtDisplay_h
#define DTEvtDisplay_h

#include "DTNtupleBaseAnalyzer.h"

#include "TFile.h"
#include <TH2.h>
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>


class EvDisp : public DTNtupleBaseAnalyzer
{

public:

  EvDisp(const TString & inFileName, const TString & outFileName);
  ~EvDisp();

  virtual void Loop() override;
  virtual void Loop(Long64_t entry);
  virtual void Loop(Long64_t start, Long64_t stop);

protected:

  virtual void book() override;
  virtual void fill() override;
  virtual void endJob() override;

  float computeX(float x);
  float computeY(float y);

  TFile m_outFile;
  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TH2*> m_2Dplots;

  TGraphErrors* graphStruct;
  float cellSizeX, cellSizeY, honeySize;

};

#endif