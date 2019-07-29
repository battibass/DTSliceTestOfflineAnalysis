#ifndef DTEvtDisplay_h
#define DTEvtDisplay_h

#include "DTNtupleBaseAnalyzer.h"

#include "TFile.h"
#include <TH2.h>
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
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

  EvDisp(const TString & inFileName);
  ~EvDisp();

  virtual void Loop() override;
  virtual void Loop(Long64_t evt);
  virtual void LoopEntry(Long64_t entry);
  virtual void Loop(Long64_t start, Long64_t stop);

protected:

  virtual void book() override;
  virtual void fill() override;
  virtual void endJob() override;

  float computeX(float x, int y);
  float computeY(float y);
  double computeQ(double x1, double x2, double y1, double y2);
  double computeM(double x1, double x2, double y1, double y2);

  std::map<std::string, TH2*> m_2Dplots;

  TGraphErrors* graphStruct;
  float cellSizeX = 4.2;
  float cellSizeY = 1.3;
  float honeySize = 12.8;
  float x0chamber = 132.55;
  float zSL1, zSL3;

};

#endif