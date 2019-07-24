//////////////////////////////////
// TO COMPILE (in root shell)
// .L DTNtupleBaseAnalyzer.C++ 
// .L EvDisp.C++ 
//
// TO RUN (in root shell)
// auto evtDisplay = EvDisp(inputFile, outFile);
// evtDisplay.Loop();
//////////////////////////////////


#include "EvDisp.h"

EvDisp::EvDisp(const TString & inFileName, const TString & outFileName) :
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)
  {

  }

EvDisp::~EvDisp() 
{ 

}

void EvDisp::Loop()
{

  book();

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEvent(jentry);
    nbytes += nb;

    if(jentry % 100 == 0) 
      std::cout << "[EvDisp::Loop] processed : "<< jentry << " entries\r" << std::flush;

    fill();

  }

  std::cout << std::endl; 

  endJob();

}

void EvDisp::Loop(Long64_t entry)
{

  book();
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  if(entry>=nentries) entry = nentries -1;
  Long64_t ientry = LoadTree(entry);
  if (ientry < 0) return;
  nb = fChain->GetEvent(entry);
  nbytes += nb;
  std::cout << "[EvDisp::Loop] processing : "<< entry << "\r" << std::flush;
  fill();
  std::cout << std::endl; 
  endJob();

}

void EvDisp::Loop(Long64_t start, Long64_t stop)
{

  book();
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  if(stop>=nentries) stop = nentries -1;
  if(start>=nentries) start = nentries -2;

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=start; jentry<stop;jentry++) 
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEvent(jentry);
    nbytes += nb;

    if(jentry % 100 == 0) 
      std::cout << "[EvDisp::Loop] processed : "<< jentry << " entries\r" << std::flush;

    fill();
  }

  std::cout << std::endl; 
  endJob();

}

void EvDisp::book()
{

  m_outFile.cd();

  m_2Dplots["display1"] = new TH2F("display1","digi display legacy",60,0.5,60.5,12,0.5,12.5);
  m_2Dplots["display2"] = new TH2F("display2","digi display Phase2",60,0.5,60.5,12,0.5,12.5);
  m_2Dplots["timecomp"] = new TH2F("timecomp","ph2 time vs legacy",500,2200,3200,500,82000,83000);

}

void EvDisp::fill()
{

  for (unsigned int idigi=0; idigi<digi_nDigis; idigi++) {
    float x=digi_wire->at(idigi);
    float y=digi_layer->at(idigi) + 4*(digi_superLayer->at(idigi)-1);
    m_2Dplots["display1"]->Fill(x,y);

    for (unsigned int idigi2=0; idigi2<ph2Digi_nDigis; idigi2++) {
      if (ph2Digi_superLayer->at(idigi2)==digi_superLayer->at(idigi) &&
          ph2Digi_layer->at(idigi2)==digi_layer->at(idigi) &&
          ph2Digi_wire->at(idigi2)==digi_wire->at(idigi) )
      {
        m_2Dplots["timecomp"]->Fill(digi_time->at(idigi),ph2Digi_time->at(idigi2));
      }
    }
  }

  for (unsigned int idigi=0; idigi<ph2Digi_nDigis; idigi++) {
    float x=ph2Digi_wire->at(idigi);
    float y=ph2Digi_layer->at(idigi) + 4*(ph2Digi_superLayer->at(idigi)-1);
    m_2Dplots["display2"]->Fill(x,y);
  }

}

void EvDisp::endJob()
{

  TCanvas* c1 = new TCanvas();
  c1->Divide(2,1);
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  c1->cd(1);
  m_2Dplots["display1"]->Draw("colz");
  c1->cd(2);
  m_2Dplots["display2"]->Draw("colz");
  c1->Update();
  c1->Print("digi.png");

  TCanvas* c2 = new TCanvas();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  m_2Dplots["timecomp"]->Draw("colz");
  c2->Update();
  c2->Print("time.png");


  m_outFile.cd();
  m_outFile.Write();
  m_outFile.Close();

}