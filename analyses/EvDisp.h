#ifndef DTEvtDisplay_h
#define DTEvtDisplay_h

#include "DTNtupleBaseAnalyzer.h"

#include "TSystem.h"
#include "TFile.h"
#include "TH2.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>


class EvDisp : public DTNtupleBaseAnalyzer
{

public:

  EvDisp(const TString & inFileName);
  ~EvDisp();

  virtual void Loop() override;
  void Loop(Long64_t start, Long64_t stop, Long64_t evt);
  void Loop(Long64_t evt);
  void LoopEntry(Long64_t entry);

  void DumpON(){ dumpFlag = true; }
  void DumpOFF(){ dumpFlag = false; }
  void SaveDisplayON(){ saveDispFlag = 1; }
  void SaveDisplayPROMT(){ saveDispFlag = 0; }
  void SaveDisplayOFF(){ saveDispFlag = -1; }
  void AskContinueON(){ askContinueFlag = true; }
  void AskContinueOFF(){ askContinueFlag = false; }
  void RunOnlyIfOneEmptyON(){ runOnlyIfOneEmptyFlag = true; }
  void RunOnlyIfOneEmptyOFF(){ runOnlyIfOneEmptyFlag = false; }


protected:

  virtual void book() override;
  virtual void fill() override;
  virtual void endJob() override;

  float computeX(float x, int y, int iMB);
  float computeY(float y);
  double computeQ(double x1, double x2, double y1, double y2);
  double computeM(double x1, double x2, double y1, double y2);
  double computeSegRange(double m);

  void fillDigiVectors(vector<float> vX[], vector<float> vY[], float x, float y);
  void setGraphColor(TGraph *gr, int i);

  TCanvas *c1;

  TGraphErrors ***graphStruct;
  float cellSizeX = 4.2;
  float cellSizeY = 1.3;
  float honeySize = 12.8;
  float x0station[4] = {0, 132.55, 0, 200.55};

  unsigned int nWiresMaxPhi[4] = {49, 60, 72, 92};
  unsigned int nWiresMaxEta = 57;

  float zSL1, zSL3;

  bool dumpFlag;
  short int saveDispFlag;
  bool askContinueFlag;
  bool runOnlyIfOneEmptyFlag;

  bool vetoMB1;
  bool vetoMB2;
  bool vetoMB3;
  bool vetoMB4;

  bool debug;

};

#endif