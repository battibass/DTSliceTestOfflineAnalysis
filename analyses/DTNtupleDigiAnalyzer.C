#include "DTNtupleDigiAnalyzer.h"

#include <set>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>

DTNtupleDigiAnalyzer::DTNtupleDigiAnalyzer(const TString & inFileName,
                                           const TString & outFileName,
                                           std::string outFolder,
                                           const TString & maskFile = "") :
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

  m_timeBoxMin["Ph1"]  =  -750.;
  m_timeBoxMax["Ph1"]  =  4250.;
  m_timeBoxBins["Ph1"] =  1250;

  m_timeBoxMin["Ph2"]  = -1250.;
  m_timeBoxMax["Ph2"]  =  3750.;
  m_timeBoxBins["Ph2"] =  1250;


  if (maskFile != "") {
    TString tmpstr_1, tmpstr_2, tmpstr_3, tmpstr_4;
    ifstream myfile (maskFile);

    if (myfile.is_open()) {
      string line;
      while ( getline (myfile, line) ) {
        TString tmpline = line;
        if      (tmpline.Contains("MB1")) tmpstr_1 = tmpline;
        else if (tmpline.Contains("MB2")) tmpstr_2 = tmpline;
        else if (tmpline.Contains("MB3")) tmpstr_3 = tmpline;
        else if (tmpline.Contains("MB4")) tmpstr_4 = tmpline;
      }
      tmpstr_1.ReplaceAll("\n", ""); tmpstr_1.ReplaceAll("MB1 ", "");
      tmpstr_2.ReplaceAll("\n", ""); tmpstr_2.ReplaceAll("MB2 ", "");
      tmpstr_3.ReplaceAll("\n", ""); tmpstr_3.ReplaceAll("MB3 ", "");
      tmpstr_4.ReplaceAll("\n", ""); tmpstr_4.ReplaceAll("MB4 ", "");
      myfile.close();
    }
    else cout << "WARNING: unable to open mask wire file: NO WIRE WILL BE MASKED!";

    TString tok;
    Ssiz_t from = 0;
    while (tmpstr_1.Tokenize(tok, from, " ")) {
      maskedWires[1].push_back(atoi(tok));
    }
    while (tmpstr_2.Tokenize(tok, from, " ")) {
      maskedWires[2].push_back(atoi(tok));
    }
    while (tmpstr_3.Tokenize(tok, from, " ")) {
      maskedWires[3].push_back(atoi(tok));
    }
    while (tmpstr_4.Tokenize(tok, from, " ")) {
      maskedWires[4].push_back(atoi(tok));
    }

  }
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

      hName = ("hOccupancyMultiple" + stTag).c_str();
      m_plots[hName] = new TH2F(hName, "Occupancy;wire;layer / superlayer", 100, 0.5, 100.5, 12, -0.5, 11.5);

	  hName = ("hWireByWireEff" + stTag).c_str();
	  m_effs[hName] = new TEfficiency(hName,"Wire by wire matching efficiency;wire;layer / superlayer",100,0.5,100.5,12,-0.5,11.5);

	  hName = ("hEffSummary" + stTag).c_str();
	  m_plots[hName] = new TH1F(hName,"Efficiency summary;efficiency,# wires",110,0.5,1.05);	  

	  hName = ("hTimeDiff" + stTag).c_str();
	  m_plots[hName] = new TH1F(hName,"Wire by wire digi time difference;time difference;entries",125,0.,500.);

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

  std::map<WireId, std::vector<float>> digisByWire;
  std::map<WireId, std::vector<float>> ph2DigisByWire;

  fillBasic("Ph1",digisByWire);
  fillBasic("Ph2",ph2DigisByWire);

  auto wireIds    = wiresWithInTimeDigis("Ph1",digisByWire);
  auto ph2WireIds = wiresWithInTimeDigis("Ph2",ph2DigisByWire);
  
  fillEff("Ph1",wireIds,ph2WireIds);
  fillEff("Ph2",ph2WireIds,wireIds);

}

void DTNtupleDigiAnalyzer::fillBasic(std::string typeTag,
				     std::map<WireId, std::vector<float>> & digisByWire)
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

      WireId wireId(st,sl,lay,wire);
      digisByWire[wireId].push_back(time);

      stringstream layerTagS;
      layerTagS << typeTag
		<< "St" << st
		<< "SuperLayer" << sl
		<< "Layer" << lay;
      
      string layerTag = layerTagS.str();
      
      m_plots[("hTimeBox"+ layerTag).c_str()]->Fill(time);
    }

  for (auto & wireAndDigis : digisByWire)
    {
      
      auto & wireId    =  wireAndDigis.first;
      stringstream stTagS;
      stTagS << typeTag
              << "St" << wireId.m_chamb;

      string stTag = stTagS.str();

      if (wireId.m_sl == 2 || std::find(maskedWires[wireId.m_chamb].begin(), maskedWires[wireId.m_chamb].end(), wireId.m_wire) != maskedWires[wireId.m_chamb].end()) continue;

      auto & digiTimes =  wireAndDigis.second;

//       m_plots[("hOccupancyMultiple" + stTag).c_str()]->Fill(wireId.m_wire, (wireId.m_layer - 1) + (wireId.m_sl - 1) * 4);

      if (digiTimes.size() > 1)
        {

          std::sort(digiTimes.begin(),digiTimes.end());

          auto digiIt  = digiTimes.begin();
          auto digiEnd = digiTimes.end();

          double timePrev = (*digiIt);
          ++digiIt;

          for (; digiIt!=digiEnd; ++digiIt)
            {
              m_plots[("hTimeDiff"+ stTag).c_str()]->Fill((*digiIt) - timePrev);
              timePrev = (*digiIt);
              m_plots[("hOccupancyMultiple" + stTag).c_str()]->Fill(wireId.m_wire, (wireId.m_layer - 1) + (wireId.m_sl - 1) * 4);
            }
        }
    }
  
}

std::set<WireId> DTNtupleDigiAnalyzer::wiresWithInTimeDigis(std::string typeTag,
							    const std::map<WireId, std::vector<float>> & digisByWire) const
{
  
  std::set<WireId> wireIds;
  
  for (const auto & wireAndDigis : digisByWire)
    {
      const auto & wireId    =  wireAndDigis.first;
      const auto & digiTimes =  wireAndDigis.second;
      
      for (const auto & digiTime : digiTimes)
	{
	  if (digiTime > m_timeBoxMin.at(typeTag) +  750. &&
	      digiTime < m_timeBoxMin.at(typeTag) + 1650.)
	    {
	      wireIds.insert(wireId);
	      break;
	    }
	}
    }

  return wireIds;
  
}

void DTNtupleDigiAnalyzer::fillEff(std::string typeTag, const std::set<WireId> & wireIdProbes, const std::set<WireId> & wireIdRefs)
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

	  TString  hName = ("hWireByWireEff" + tag).c_str();
	  
	  int nBinsX = m_effs[hName]->GetTotalHistogram()->GetNbinsX();
	  int nBinsY = m_effs[hName]->GetTotalHistogram()->GetNbinsY();

	  for (int iBinX = 1; iBinX <= nBinsX; ++ iBinX)
	    {
	      for (int iBinY = 1; iBinY <= nBinsY; ++ iBinY)
		{
		  
		  int iBin = m_effs[hName]->GetGlobalBin(iBinX, iBinY);
		  float eff = m_effs[hName]->GetEfficiency(iBin);

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

