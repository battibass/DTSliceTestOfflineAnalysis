#include "DTNtupleDigiAnalyzer.h"

DTNtupleDigiAnalyzer::DTNtupleDigiAnalyzer(const TString & inFileName,
						 const TString & outFileName) :
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)  
{ 

  // The list of chambers to monitor
  // just MB2 for SX5, it ill be MB1 - MB4 
  // for the slice test
  m_stations.push_back(1);
  m_stations.push_back(2);
  m_stations.push_back(3);
  m_stations.push_back(4);

  // The time pedestal to subtract from
  // phase-2 digis
  ph2DigiPedestal = 82100;

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

      fill();

    }

  std::cout << std::endl; 

  endJob();

}

void DTNtupleDigiAnalyzer::book()
{

  m_outFile.cd();

  std::vector<std::string> typeTags = { "Ph1", "Ph2" };

  for (const auto typeTag : typeTags)
    {
  
      for (const auto iSt : m_stations)
	{
      
	  stringstream stTagS;
	  stTagS << typeTag << "St" << iSt;
      
	  string stTag = stTagS.str();

	  m_plots[("hOccupancy" + stTag).c_str()] = new TH2F(("hOccupancy" + stTag).c_str(),
							     "Occupancy; wire; layer / superlayer",
							     100,0.5,100.5, 12, -0.5, 11.5);

	  m_effs[("hWireByWireMatch" + stTag).c_str()] = new TEfficiency(("hWireByWireMatch" + stTag).c_str(),
									 "Wire by wire matching; wire; layer / superlayer",
									 100,0.5,100.5, 12, -0.5, 11.5);
	  
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
		  
		  int nBins = typeTag == "Ph2" ? 1200 : 1280;

		  m_plots[("hTimeBox" + layerTag).c_str()] = new TH1F(("hTimeBox" + layerTag).c_str(),
								     "Digi time box; time (ns); entries",
								     nBins,0.,5000.);
		}
	    } 
	}
    }

}

void DTNtupleDigiAnalyzer::fill()
{

  for (std::size_t iDigi = 0; iDigi < digi_nDigis; ++iDigi)
    {
      
      if(digi_sector->at(iDigi) != 12 || digi_wheel->at(iDigi) != 2) continue;    

      int st  = digi_station->at(iDigi);
      int sl  = digi_superLayer->at(iDigi);
      int lay = digi_layer->at(iDigi);

      int wire = digi_wire->at(iDigi);
      int slAndLay = (lay - 1) + (sl - 1) * 4;

      double time = digi_time->at(iDigi);
      double timeInRange = std::max(0.5,std::min(4999.5,time));

      stringstream stTagS;
      stTagS << "Ph1"
	     << "St" << st;

      string stTag = stTagS.str();
      
      m_plots[("hOccupancy" + stTag).c_str()]->Fill(wire,slAndLay);

      if ( time > 0 && time < 5000)
	{
	  bool hasWireMatch = false;

	  for (std::size_t iDigi = 0; iDigi < ph2Digi_nDigis; ++iDigi)
	    {
	      
	      if(ph2Digi_sector->at(iDigi) != 12 || ph2Digi_wheel->at(iDigi) != 2) continue;
	      
	      int ph2St  = ph2Digi_station->at(iDigi);
	      int ph2SL  = ph2Digi_superLayer->at(iDigi);
	      int ph2Lay = ph2Digi_layer->at(iDigi);
	      
	      int ph2Wire = ph2Digi_wire->at(iDigi);
	      double ph2Time = ph2Digi_time->at(iDigi) - ph2DigiPedestal;
	      
	      if ( st == ph2St && sl == ph2SL && lay == ph2Lay && 
		   wire == ph2Wire && ph2Time > 0 && ph2Time < 5000)
		{
		  hasWireMatch = true;
		  break;
		}
	      
	    }
	  
	  m_effs[("hWireByWireMatch" + stTag).c_str()]->Fill(hasWireMatch,wire,slAndLay);

	}

      stringstream layerTagS;
      layerTagS << "Ph1"
		<< "St" << st
		<< "SuperLayer" << sl
		<< "Layer" << lay;

      string layerTag = layerTagS.str();

      m_plots[("hTimeBox"+ layerTag).c_str()]->Fill(timeInRange);
    }

  std::map<std::vector<int>,int> ph2DigiPerWire;

  for (std::size_t iDigi = 0; iDigi < ph2Digi_nDigis; ++iDigi)
    {

      if(ph2Digi_sector->at(iDigi) != 12 || ph2Digi_wheel->at(iDigi) != 2) continue;    

      int st  = ph2Digi_station->at(iDigi);
      int sl  = ph2Digi_superLayer->at(iDigi);
      int lay = ph2Digi_layer->at(iDigi);
      
      int wire = ph2Digi_wire->at(iDigi);
      int slAndLay = (lay - 1) + (sl - 1) * 4;

      std::vector wireId = {sl, lay, wire};

      if (ph2DigiPerWire.find(wireId) == ph2DigiPerWire.end())
	  ph2DigiPerWire[wireId] = 0;
      
      ph2DigiPerWire[wireId]++;
	  
      double time = ph2Digi_time->at(iDigi) - ph2DigiPedestal;
      double timeInRange = std::max(0.5,std::min(4999.5,time));
      
      stringstream stTagS;
      stTagS << "Ph2"
   	     << "St" << st;
      
      string stTag = stTagS.str();
      
      m_plots[("hOccupancy" + stTag).c_str()]->Fill(wire,slAndLay);
      
      if ( time > 0 && time < 5000)
	{
	  bool hasWireMatch = false;

	  for (std::size_t iDigi = 0; iDigi < digi_nDigis; ++iDigi)
	    {
	      
	      if(digi_sector->at(iDigi) != 12 || digi_wheel->at(iDigi) != 2) continue;
	      
	      int ph1St  = digi_station->at(iDigi);
	      int ph1SL  = digi_superLayer->at(iDigi);
	      int ph1Lay = digi_layer->at(iDigi);
	      
	      int ph1Wire = digi_wire->at(iDigi);
	      double ph1Time = digi_time->at(iDigi);

	      if ( st == ph1St && sl == ph1SL && lay == ph1Lay && 
		   wire == ph1Wire && ph1Time > 0 && ph1Time < 5000)
		{
		  hasWireMatch = true;
		  break;
		}
	  
	    }

	  m_effs[("hWireByWireMatch" + stTag).c_str()]->Fill(hasWireMatch,wire,slAndLay);

	}

      stringstream layerTagS;
      layerTagS << "Ph2"
   		<< "St" << st
   		<< "SuperLayer" << sl
   		<< "Layer" << lay;
      
      string layerTag = layerTagS.str();
      
      m_plots[("hTimeBox"+ layerTag).c_str()]->Fill(timeInRange);
    }

  // if (ph2Digi_nDigis > 100) 
  //   {  

  //     std::cout << "event number : " << event_eventNumber << std::endl;

  //     for (const auto & nDigiPerWire : ph2DigiPerWire)
  //     	{
  //     	  std::cout << "SL : "    << nDigiPerWire.first.at(0)
  //     		    << "  layer : " << nDigiPerWire.first.at(1)
  //     		    << "  wire : "  << nDigiPerWire.first.at(2)
  //     		    << "  # digis : " << nDigiPerWire.second
  //     		    << std::endl;
  //     	}
  //   }

  // std::cout << std::endl << std::endl;
  
}

void DTNtupleDigiAnalyzer::endJob()
{

  m_outFile.cd();

  m_outFile.Write();
  m_outFile.Close();

}

