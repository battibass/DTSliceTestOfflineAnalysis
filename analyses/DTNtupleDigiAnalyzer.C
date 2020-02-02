#include "DTNtupleDigiAnalyzer.h"

#include <set>
#include <fstream>
#include <iomanip>

DTNtupleDigiAnalyzer::DTNtupleDigiAnalyzer(const TString & inFileName,
					   const TString & outFileName,
					   std::string outFolder) :
m_outFile(outFileName,"RECREATE"), m_outFolder(outFolder), DTNtupleBaseAnalyzer(inFileName)  
{ 

  // The list of chambers to monitor
  m_stations["Ph1"].push_back(1);
  m_stations["Ph1"].push_back(2);
  m_stations["Ph1"].push_back(3);
  m_stations["Ph1"].push_back(4);

  m_stations["Ph2"].push_back(1);
  m_stations["Ph2"].push_back(2);
  m_stations["Ph2"].push_back(3);
  m_stations["Ph2"].push_back(4);

  m_timeBoxMin["Ph1"]  =     0.;
  m_timeBoxMax["Ph1"]  =  5000.;
  m_timeBoxBins["Ph1"] =  1250;

  m_timeBoxMin["Ph2"]  = 83900.;
  m_timeBoxMax["Ph2"]  = 88900.;
  m_timeBoxBins["Ph2"] =  1250;

}

DTNtupleDigiAnalyzer::~DTNtupleDigiAnalyzer() 
{ 

}


void DTNtupleDigiAnalyzer::Loop()
{

  book();

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEvent(jentry);   nbytes += nb;

      if(jentry % 100 == 0) 
	std::cout << "[DTNtupleDigiAnalyzer::Loop] processed : " 
		  << jentry << " entries\r" << std::flush;

      DTNtupleBaseAnalyzer::LoadObjs();

      fill();

    }

  std::cout << std::endl; 

  endJob();

}

void DTNtupleDigiAnalyzer::book()
{

  m_outFile.cd();

  std::vector<std::string> typeTags = { "Ph1", "Ph2" };

  for (const auto & typeTag : typeTags)
    {
  
      for (const auto iSt : m_stations[typeTag])
	{
      
	  stringstream stTagS;
	  stTagS << typeTag << "St" << iSt;
      
	  string stTag = stTagS.str();
	  
	  TString hName = ("hOccupancy" + stTag).c_str(); 
	  m_plots[hName] = new TH2F(hName,"Occupancy;wire;layer / superlayer",100,0.5,100.5,12,-0.5,11.5);

	  hName = ("hWireByWireEff" + stTag).c_str();
	  m_effs[hName] = new TEfficiency(hName,"Wire by wire matching efficiency;wire;layer / superlayer",100,0.5,100.5,12,-0.5,11.5);


	  hName = ("hEffSummary" + stTag).c_str();
	  m_plots[hName] = new TH1F(hName,"Efficiency summary;efficiency,# wires",110,0.5,1.05);
	  
	  for (int iSl = 1; iSl <= 3; ++iSl)
	    {
	      for (int iLayer = 1; iLayer <= 4; ++iLayer)
		{
		  
		  stringstream layerTagS;
		  layerTagS << typeTag
			    << "St" << iSt
			    << "SuperLayer" << iSl
			    << "Layer" << iLayer;
		  
		  string layerTag = layerTagS.str();
		  
		  hName = ("hTimeBox" + layerTag).c_str();
		  m_plots[hName] = new TH1F(hName,"Digi time box;time (ns);entries",m_timeBoxBins[typeTag],m_timeBoxMin[typeTag],m_timeBoxMax[typeTag]);
		}
	    } 
	}

      TString hName = ("hFracEffWires" + typeTag).c_str();
      m_effs[hName] = new TEfficiency(hName,"Fraction of wires with eff in [0.90:1.00] range",4,0.5,4.5);
      
    }

}

void DTNtupleDigiAnalyzer::fill()
{

  std::set<WireId> wireIds;
  std::set<WireId> ph2WireIds;

  fillBasic("Ph1",wireIds);
  fillBasic("Ph2",ph2WireIds);

  fillEff("Ph1",wireIds,ph2WireIds);
  fillEff("Ph2",ph2WireIds,wireIds);

}

void DTNtupleDigiAnalyzer::fillBasic(std::string typeTag, std::set<WireId> & wireIds)
{

  DTNtupleDigi & digi = digiObjs[typeTag];

  for (std::size_t iDigi = 0; iDigi < (*digi.nDigis); ++iDigi)
    {
      
      // The slice test sector
      if(digi.sector->at(iDigi) != 12 || digi.wheel->at(iDigi) != 2) continue;    
      
      int st  = digi.station->at(iDigi);
      int sl  = digi.superLayer->at(iDigi);
      int lay = digi.layer->at(iDigi);
      
      int wire = digi.wire->at(iDigi);
      int slAndLay = (lay - 1) + (sl - 1) * 4;
      
      double time = digi.time->at(iDigi);
      
      stringstream stTagS;
      stTagS << typeTag
	     << "St" << st;
      
      string stTag = stTagS.str();
      
      m_plots[("hOccupancy" + stTag).c_str()]->Fill(wire,slAndLay);
      
      if ( time > m_timeBoxMin[typeTag] + 100. && time < m_timeBoxMin[typeTag] + 900.)
	{
	  WireId wireId(st,sl,lay,wire);
	  wireIds.insert(wireId);
	}

      stringstream layerTagS;
      layerTagS << typeTag
		<< "St" << st
		<< "SuperLayer" << sl
		<< "Layer" << lay;
      
      string layerTag = layerTagS.str();
      
      m_plots[("hTimeBox"+ layerTag).c_str()]->Fill(time);
    }
  
}

void DTNtupleDigiAnalyzer::fillEff(std::string typeTag, std::set<WireId> & wireIdProbes, std::set<WireId> & wireIdRefs)
{

  for (const auto & wireIdRef : wireIdRefs)
    {
      stringstream stTagS;
      stTagS << typeTag
             << "St" << wireIdRef.m_chamb;
      
      std::string stTag = stTagS.str();

      bool hasWireMatch = false;

      for (const auto & wireIdProbe : wireIdProbes)
	{
	  if (wireIdRef == wireIdProbe)
	    {
	      hasWireMatch = true;
	      break;
	    }
	}
      
      int slAndLay = (wireIdRef.m_layer - 1) + (wireIdRef.m_sl - 1) * 4;
      m_effs[("hWireByWireEff" + stTag).c_str()]->Fill(hasWireMatch,wireIdRef.m_wire,slAndLay);
      
    } 

}

void DTNtupleDigiAnalyzer::endJob()
{

  std::vector<std::string> typeTags = { "Ph1", "Ph2" };

  for (const auto & typeTag : typeTags)
    {
      for (const auto & iSt : m_stations[typeTag])
	{

	  ofstream outTxtFile;

	  std::string tag = typeTag + "St" + std::to_string(iSt);

	  outTxtFile.open(m_outFolder + "/" + "ineffCh" + tag + ".txt");
	  outTxtFile << "+--------------------------------+\n"; 
	  outTxtFile << "| SL | layer | wire | efficiency |\n"; 

	  int nBinsX = m_effs[("hWireByWireEff" + tag).c_str()]->GetTotalHistogram()->GetNbinsX();
	  int nBinsY = m_effs[("hWireByWireEff" + tag).c_str()]->GetTotalHistogram()->GetNbinsY();

	  for (int iBinX = 1; iBinX <= nBinsX; ++ iBinX)
	    {
	      for (int iBinY = 1; iBinY <= nBinsY; ++ iBinY)
		{
		  
		  int iBin = m_effs[("hWireByWireEff" + tag).c_str()]->GetGlobalBin(iBinX, iBinY);
		  float eff = m_effs[("hWireByWireEff" + tag).c_str()]->GetEfficiency(iBin);
		  if ( eff > 0.01 )
		    {
		      m_plots[("hEffSummary" + tag).c_str()]->Fill(eff);
		      m_effs[("hFracEffWires" + typeTag).c_str()]->Fill(eff >= 0.9,iSt);
		      if (eff < 0.9)
			{
			  outTxtFile << "| "  << std::setw(2)  << (iBinY/4+1)
				     << " | " << std::setw(5)  << (iBinY%4)
				     << " | " << std::setw(4)  << iBinX
				     << " | " << std::setw(10) << std::setprecision(3) << eff
				     << " |\n";
			}
		    }			
		}
	    }
	  
	  outTxtFile.close();
	  
	}
    }
  
  m_outFile.Write();
  m_outFile.Close();

}

