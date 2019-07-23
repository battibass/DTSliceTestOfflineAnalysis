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

void EvDisp::book()
{

  m_outFile.cd();

  m_2Dplots["display1"] = new TH2F("display1","digi display legacy",60,0.5,60.5,12,0.5,12.5);
  m_2Dplots["display2"] = new TH2F("display2","digi display Phase2",60,0.5,60.5,12,0.5,12.5);
  m_2Dplots["timecomp"] = new TH2F("timecomp","ph2 time vs legacy",500,2200,3200,500,82000,83000);

}

void EvDisp::fill()
{

  for (int idigi=0; idigi<digi_nDigis; idigi++) {
    float x=digi_wire->at(idigi);
    float y=digi_layer->at(idigi) + 4*(digi_superLayer->at(idigi)-1);
    m_2Dplots["display1"]->Fill(x,y);

    for (int idigi2=0; idigi2<ph2Digi_nDigis; idigi2++) {
      if (ph2Digi_superLayer->at(idigi2)==digi_superLayer->at(idigi) &&
          ph2Digi_layer->at(idigi2)==digi_layer->at(idigi) &&
          ph2Digi_wire->at(idigi2)==digi_wire->at(idigi) )
      {
        m_2Dplots["timecomp"]->Fill(digi_time->at(idigi),ph2Digi_time->at(idigi2));
      }
    }
  }

  for (int idigi=0; idigi<ph2Digi_nDigis; idigi++) {
    float x=ph2Digi_wire->at(idigi);
    float y=ph2Digi_layer->at(idigi) + 4*(ph2Digi_superLayer->at(idigi)-1);
    m_2Dplots["display2"]->Fill(x,y);
  }

}

void EvDisp::endJob()
{

  m_outFile.cd();
  m_outFile.Write();
  m_outFile.Close();

}