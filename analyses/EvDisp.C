#include "EvDisp.h"

EvDisp::EvDisp(const TString & inFileName, const TString & outFileName) :
  m_outFile(outFileName,"RECREATE"), EvDisp(inFileName)
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

}

void EvDisp::fill()
{

}

void EvDisp::endJob()
{
  m_outFile.cd();
  m_outFile.Write();
  m_outFile.Close();
}