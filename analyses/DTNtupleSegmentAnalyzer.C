#include "DTNtupleSegmentAnalyzer.h"

#include "TProfile.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TF1.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <bitset>


DTNtupleSegmentAnalyzer::DTNtupleSegmentAnalyzer(const TString & inFileName,
						 const TString & outFileName) :
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)  
{ 

  m_timeBoxMin["Ph1"]  =  -750.;
  m_timeBoxMax["Ph1"]  =  4250.;

  m_timeBoxMin["Ph2"]  = -1250.;
  m_timeBoxMax["Ph2"]  =  3750.;

  // CB make this constexpr
  m_tags.push_back("Ph1");
  m_tags.push_back("Ph2");

  TObjArray *tx = outFileName.Tokenize("/");
  m_deadFileName = (((TObjString *)(tx->At(0)))->String()).Data();
  cout<<"m_deadFileName "<<m_deadFileName<<endl;

}

DTNtupleSegmentAnalyzer::~DTNtupleSegmentAnalyzer() 
{ 

}

void DTNtupleSegmentAnalyzer::PreLoop()
{

  // maps by "tag"
  std::map<std::string, std::ofstream> deadChannelListRun, deadChannelList;

  // plot by tag and SL
  std::map<std::string, std::map<int, std::map<int, TH2F*>>> hOccupancyBySl;

  std::cout << "[DTNtupleSegmentAnalyzer] Running Preloop to save dead channels." << std::endl;

  for (const auto & tag : m_tags)
    {
      std::string deadFileNameRun = m_deadFileName 
                                    + "/segment/DeadList/DeadList_" 
	                            + m_deadFileName 
                                    + tag;

      std::string deadFileName = "deadChannelList" + tag;

      std::cout << "[DTNtupleSegmentAnalyzer] Info will be saved in "
		<< deadFileNameRun << " and in "
		<< deadFileName << " (for future use)" 
		<< std::endl;

      deadChannelListRun[tag].open(deadFileNameRun);
      deadChannelList[tag].open(deadFileName);

      //Create a Histogram per SL
      for (int iSt=1; iSt<=4; ++iSt)
	{
	  for (int iSL=1; iSL<=3; ++iSL)
	    {
	      if (iSt==4 &&iSL==2) continue; 
	      std::string hName = "MB" + std::to_string(iSt) + "_SL" + std::to_string(iSL) + "_" + tag;
	      hOccupancyBySl[tag][iSt][iSL] = new TH2F(hName.c_str(), "", 92, 1., 93., 4, 0.5, 4.5);
	    }
	}
    }

  if (fChain == 0) return;
  Long64_t nEntries = fChain->GetEntriesFast();

  //nEntries=50000;

  for (Long64_t jentry=0; jentry<nEntries; ++jentry) 
    {

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      fChain->GetEntry(jentry);

      DTNtupleBaseAnalyzer::LoadObjs();

      if (jentry%1000 == 0) 
	std::cout << setprecision (2) 
		  << "[DTNtupleSegmentAnalyzer] : Pre-Loop event " 
		  << jentry << " " << jentry/float(nEntries)*100. << "%\r" 
		  << std::flush;

      for (const auto & tag : m_tags) 
	{
	  for (uint iDigi=0; iDigi<(*digiObjs[tag].nDigis); iDigi++) 
	    {
	      int st = digiObjs[tag].station->at(iDigi);
	      int sl = digiObjs[tag].superLayer->at(iDigi);
	      int lay = digiObjs[tag].layer->at(iDigi);
	      int wire = digiObjs[tag].wire->at(iDigi);
	      
	      hOccupancyBySl[tag][st][sl]->Fill(wire, lay);
	    }
	}
    }

  std::cout << std::endl;
  
  // analyze occupancy histos and fill dead channel table
    
  for (const auto & tag : m_tags) 
    {
      int nWiresFile = 0;
      int nWiresTot = 0;
      
      for (int iSt=1; iSt<=4; ++iSt) 
	{
	  for (int iSL=1; iSL<=3; ++iSL) 
	    {
	      int nWiresLay=0;
 
	      if (iSL==2 && iSt==4) continue;

	      if (iSL==2) nWiresLay = 57; 
	      else if (iSt==1) nWiresLay = 49;
	      else if (iSt==2) nWiresLay = 60;
	      else if (iSt==3) nWiresLay = 72;
	      else if (iSt==4) nWiresLay = 92;	  	   
      
	      for (int iLay=1; iLay<=4; ++iLay) 
		{
		  for (int iW=1; iW<=nWiresLay; ++iW) 
		    {
		      nWiresTot++;

		      if (hOccupancyBySl[tag][iSt][iSL]->GetBinContent(iW,iLay)==0)
			{
			  nWiresFile++;

			  m_deadWires[tag].push_back(WireId(iSt,iSL,iLay,iW));
			  
			  std::stringstream wireString;
			  			  
			  wireString << nWiresFile 
				     << " " << iSt 
				     << " " << iSL 
				     << " " << iLay 
				     << " " << iW 
				     << std::endl;

			  deadChannelListRun[tag] << wireString.str();
			  deadChannelList[tag] << wireString.str();
			}
		    }
		}

	      // do not want to store these histos
	      delete hOccupancyBySl[tag][iSt][iSL];

	    }
	}

      std::cout << "[DTNtupleSegmentAnalyzer] : " 
		<< nWiresFile << " dead wires out of " 
		<< nWiresTot << std::endl;

      std::stringstream trailingString;
      trailingString << 0 
		     << " " << 0 
		     << " " << 0 
		     << " " << 0
		     << " " << 0
		     << " " << 0
		     << " " << 0 
		     << " " << 0 
		     << " " << std:: endl
		     << nWiresFile << " dead wires out of "
		     << nWiresTot << std::endl;
      
      deadChannelListRun[tag] << trailingString.str();
      deadChannelList[tag] << trailingString.str();

      deadChannelListRun[tag].close();
      deadChannelList[tag].close();
      
    }
  
}

void DTNtupleSegmentAnalyzer::Loop()
{

  book();

  if (fChain == 0) return;

  Long64_t nEntries = fChain->GetEntries();
  //nEntries = 50000;

  //Main loop over entries
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nEntries;jentry++) 
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEvent(jentry);   nbytes += nb;

      if(jentry % 100 == 0) 
	std::cout << "[DTNtupleSegmentAnalyzer::Loop] processed : " 
		  << jentry << " entries\r" << std::flush;

      DTNtupleBaseAnalyzer::LoadObjs();

      fill();
     
    }

  endJob();

}

void DTNtupleSegmentAnalyzer::book()
{
  
  m_outFile.cd();

  for (int st=1; st<5; st++)
    {

      auto hName = Form("NSegPh1VsPh2_st%d",st); 
      m_plots[hName] = new TH2F(hName,
				"# segment ph1 vs ph2;# seg (phase-1); # seg (phase-2)",
				10,-0.5,9.5,10,-0.5,9.5); 

      hName = Form("NHitsPhiPh1VsPh2_st%d",st);
      m_plots[hName] = new TH2F(hName,
				"# phi hits ph1 vs ph2;# phi hits (phase-1); # phi hits (phase-2)",
				10,-0.5,9.5,10,-0.5,9.5);

      hName = Form("NPh2Ph1HitsDiffPhiVsX_st%d",st);
      m_plots[hName] = new TH2F(hName,
				"# ph2 - ph1 phi hits vs x;local x (cm); # ph2 - ph1 phi hits",
				20,-200.,200.,17,-8.5,8.5);

      hName = Form("EffPhiPh1VsPh2_st%d",st);
      m_plots[hName] = new TH2F(hName,
				"eff ph1 vs ph2;# efficiency (phase-1); efficiency (phase-2)",
				101,0.9,1.01,101,0.9,1.01);
    }
	
  std::vector<std::string> typeTags = { "Ph1", "Ph2" };

  for (const auto & typeTag : typeTags)
    {
      
      auto hName = Form("%seffPhi_station",typeTag.c_str()); 
      m_effs[hName] = new TEfficiency(hName,
				      "#phi efficiency per station;station;eff.",
				      4,0.5,4.5);
      
      hName = Form("%seffTheta_station",typeTag.c_str()); 
      m_effs[hName] = new TEfficiency(hName,
				      "#theta efficiency per station;station;eff.",
				      4,0.5,4.5);
      
      hName = Form("%shitResZ",typeTag.c_str()); 
      m_plots[hName] = new TH1F(hName,
				"Hit residual z view;|d_hit|-|d_extr|(cm)",
				250,-1.,1.); 
      
      hName = Form("%shitResX",typeTag.c_str()); 
      m_plots[hName] = new TH1F(hName,
				"Hit residual X view;|d_hit|-|d_extr|(cm)",
				250,-.8,.8); 
      
      hName = Form("%shitResX_vsWireDist",typeTag.c_str()); 
      m_2Dplots[hName] = new TH2F(hName,
				  "Hit residual X view;dist wire;|d_hit|-|d_extr|(cm)",
				  100,0,2.5,200,-.8,.8); 

      hName = Form("%shitResZ_vsWireDist",typeTag.c_str()); 
      m_2Dplots[hName] = new TH2F(hName,
				  "Hit residual Z view;dist wire;|d_hit|-|d_extr|(cm)",
				  100,0,2.5,200,-.8,.8); 
      
      for (int st=1; st<5; st++)
	{
	  
	  hName = Form("%shPhiNhits_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName,
				    "# hits;# hits; entries",
				    8,0.5,8.5); 	  
	  
	  hName = Form("%shNhits_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName,
				    "# hits;# hits; entries",
				    12,0.5,12.5);  

	  hName = Form("%shT0_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName,
				    "Segment t0;t0 (ns); entries",
				    200,-100.,100.);  

	  hName = Form("%shT0VsX_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TProfile(hName,
					"Segment t0 vs local x;local x (cm);t0 (ns)",
					20,-200.,200.,-100.,100.);  

	  hName = Form("%shPhiProbChi2_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName, 
				    "Segment #phi prob(#chi^2); prob(#chi^2); entries", 
				    100,0.,1.);

	  hName = Form("%shPhiChi2_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName, 
				    "Segment #phi #chi^2;  #chi^2/ndof; entries", 
				    100,0.,50.);
	  
	  if (st < 4)
	    {
	      hName = Form("%shThetaNhits_st%d",typeTag.c_str(),st);
	      m_plots[hName] = new TH1F(hName,
					"# hits;# hits; entries",
					4,0.5,4.5); 
	      
	      hName = Form("%shThetaProbChi2_st%d",typeTag.c_str(),st);
	      m_plots[hName] = new TH1F(hName, 
					"Segment #theta Prob. #chi^2; prob(#chi^2); entries", 
					100,0.,1.);
	      
	      hName = Form("%shThetaChi2_st%d",typeTag.c_str(),st);
	      m_plots[hName] = new TH1F(hName, 
					"Segment #theta #chi^2; #chi^2/ndof; entries", 
					100,0.,50.);
	    }
	  
	  hName = Form("%shResPerSl_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName,
				    "Residual per Sl; Sl ; Residual [#mu m]",
				    3,0.5,3.5);

	  hName = Form("%seffByWire_st%d",typeTag.c_str(),st); 
	  m_effs[hName] = new TEfficiency(hName,
					  "Wire by wire efficiency; wire; layer / superlayer",
					  100,0.5,100.5, 12, 0.5, 12.5);

	  hName = Form("%seffSummary_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName,"Efficiency summary;efficiency,# wires",110,0.5,1.05);

	}
            
      for (int st=1; st<5; st++)
	{
	  for (int sl=1; sl<4; sl++)
	    {
	  
	      if(sl==2 && st!=4)
		{
		  hName = Form("%shitResZ_st%d",typeTag.c_str(),st);
		  m_plots[hName] = new TH1F(hName,
					    "Hit residual #theta view;|d_hit|-|d_extr|(cm)",
					    250,-.8,.8); 	
		  
		  
		  hName = Form("%shitResZvsWireDist_st%d",typeTag.c_str(),st);
		  m_2Dplots[hName] = new TH2F(hName,
					      "Hit residual #theta view;|d_wire|[cm];|d_hit|-|d_extr|(cm)",
					      100,0,2.5,250,-.8,.8); 	
		}
	      else
		{
		  hName = Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl);
		  m_plots[hName] = new TH1F(hName,
					    "Hit residual #phi view;|d_hit|-|d_extr|(cm)",
					    250,-.8,.8); 

		  hName = Form("%shitResXvsWireDist_st%d_sl%d",typeTag.c_str(),st,sl);
		  m_2Dplots[hName] = new TH2F(hName,
					      "Hit residual #phi view;|d_wire|[cm];|d_hit|-|d_extr|(cm)",
					      100,0,2.5, 250,-.8,.8);
		}
	      
	      for (int l=1; l<5; l++)
		{
		  if(sl==2 && st!=4)
		    {
		      hName = Form("%shitResZ_st%d_l%d",typeTag.c_str(),st,l); 
		      m_plots[hName] = new TH1F(hName,
						"Hit residual #theta view;|d_hit|-|d_extr|(cm)",
						250,-.8,.8); 	
		      
		      
		      hName = Form("%shitResZvsWireDist_st%d_l%d",typeTag.c_str(),st,l);
		      m_2Dplots[hName]  = new TH2F(hName,
						   "Hit residual #theta view;|d_wire|[cm];|d_hit|-|d_extr|(cm)",
						   100,0,2.5,250,-.8,.8); 	
		    }
		  else
		    {
		      hName = Form("%shitResX_st%d_sl%d_l%d",typeTag.c_str(),st,sl,l);
		      m_plots[hName] = new TH1F(hName,
						"Hit residual #phi view;|d_hit|-|d_extr|(cm)",
						250,-.8,.8); 
		      
		      hName = Form("%shitResXvsWireDist_st%d_sl%d_l%d",typeTag.c_str(),st,sl,l);
		      m_2Dplots[hName]  = new TH2F(hName,
						   "Hit residual #phi view;|d_wire|[cm];|d_hit|-|d_extr|(cm)",
						   100,0,2.5,250,-.8,.8);
		    }
		}
	    } 
	  
	}
      
    }  
  
}

void DTNtupleSegmentAnalyzer::fill()
{

  baseAnalysis();
  comparisonAnalysis();

  for (const auto & tag : m_tags)
    {
      measureEfficiency(tag);
      computeResidual(tag);
    }

}

void DTNtupleSegmentAnalyzer::endJob()
{

  for (const auto & tag : m_tags)
    {
      for (int st=1; st<5; ++st)
	{

	  string hName = Form("%seffByWire_st%d",tag.c_str(),st);
	  
	  int nBinsX = m_effs[hName]->GetTotalHistogram()->GetNbinsX();
	  int nBinsY = m_effs[hName]->GetTotalHistogram()->GetNbinsY();

	  for (int iBinX = 1; iBinX <= nBinsX; ++ iBinX)
	    {
	      for (int iBinY = 1; iBinY <= nBinsY; ++ iBinY)
		{
		  
		  int iBin  = m_effs[hName]->GetGlobalBin(iBinX, iBinY);
		  float eff = m_effs[hName]->GetEfficiency(iBin);
		  if ( eff > 0.01 )
		    {
		      m_plots[Form("%seffSummary_st%d",tag.c_str(),st)]->Fill(eff);
		    }			
		}
	    }
	  
	  for (int sl=1; sl<4; ++sl)
	    {
	
	      if(sl==2 && st!=4)
		{  

		  string hName = Form("%shitResZ_st%d",tag.c_str(),st);

		  auto histo = m_plots[hName];

		  double center = histo->GetBinCenter(histo->GetMaximumBin());
		  double sigma  = histo->GetStdDev();
		  
		  cout << "Fit " << hName << endl << endl;
		  
		  TF1 *  fTheta = new TF1("fTheta","gaus",
					  center - 1.2 * sigma,
					  center + 1.2 * sigma);
		  
		  m_plots[hName]->Fit("fTheta","RM");
		  
		  fTheta->SetRange(fTheta->GetParameter(1) - 1.1 * fTheta->GetParameter(2),
				   fTheta->GetParameter(1) + 1.1 * fTheta->GetParameter(2));
		  
		  m_plots[hName]->Fit("fTheta","RM");
		  
		  m_plots[Form("%shResPerSl_st%d",tag.c_str(),st)]->SetBinContent(sl + 0.5, 1e4 * fTheta->GetParameter(2)); 	
		}
	      else if(sl!=2)
		{
	  
		  string hName = Form("%shitResX_st%d_sl%d",tag.c_str(),st,sl);
		  
		  auto histo = m_plots[hName];

		  double center = histo->GetBinCenter(histo->GetMaximumBin());
		  double sigma  = histo->GetStdDev();

		  cout << "Fit " << hName << endl;
		  
		  TF1 *  fPhi = new TF1("fPhi","gaus",
					center - 1.2 * sigma,
					center + 1.2 * sigma);
	  
		  m_plots[hName]->Fit("fPhi","RM");

		  fPhi->SetRange(fPhi->GetParameter(1) - 1.1 * fPhi->GetParameter(2),
				 fPhi->GetParameter(1) + 1.1 * fPhi->GetParameter(2));

		  m_plots[hName]->Fit("fPhi","RM");
	  
		  m_plots[Form("%shResPerSl_st%d",tag.c_str(),st)]->SetBinContent(sl+0.5,1e4*fPhi->GetParameter(2)); 	
		}

	      cout << std::endl << std::endl;
	    }
	}
    }

  for (int st=1; st<5; ++st)
    {

      string hNamePh1 = Form("Ph1effByWire_st%d",st);
      string hNamePh2 = Form("Ph2effByWire_st%d",st);
      
      int nBinsX = m_effs[hNamePh1]->GetTotalHistogram()->GetNbinsX();
      int nBinsY = m_effs[hNamePh1]->GetTotalHistogram()->GetNbinsY();
      
      for (int iBinX = 1; iBinX <= nBinsX; ++ iBinX)
	{
	  for (int iBinY = 1; iBinY <= nBinsY; ++ iBinY)
	    {
	      
	      int iBinPh1  = m_effs[hNamePh1]->GetGlobalBin(iBinX, iBinY);
	      float effPh1 = m_effs[hNamePh1]->GetEfficiency(iBinPh1);
	      
	      int iBinPh2  = m_effs[hNamePh2]->GetGlobalBin(iBinX, iBinY);
	      float effPh2 = m_effs[hNamePh2]->GetEfficiency(iBinPh2);
	      if ( effPh1 > 0.01 && effPh2 > 0.01 )
		{
		  m_plots[Form("EffPhiPh1VsPh2_st%d",st)]->Fill(std::max(effPh1, float(0.901)),
								std::max(effPh2, float(0.901)));
		}			
	    }
	}
    }

  m_outFile.cd();
  m_outFile.Write();
  m_outFile.Close();
  
}

void DTNtupleSegmentAnalyzer::baseAnalysis()
{

  //Phase1

  for (uint iSeg = 0; iSeg < seg_nSegments; ++iSeg) 
    {

      if(seg_sector->at(iSeg) != 12 || seg_wheel->at(iSeg) != 2) continue;    

      //Chi2 cut if needed
      
      //if(seg_hasPhi->at(iSeg)    && seg_phi_normChi2->at(iSeg) > 3.84) continue;
      //if(seg_hasZed->at(iSeg) && seg_z_normChi2->at(iSeg) > 3.84) continue;

      //Select segment with t0 < of a specific time  to avoid out of time events
      //if(abs(seg_phi_t0->at(iSeg)) > 15) continue;

      int nHits = 0;
      if(seg_hasPhi->at(iSeg))
	{
	  
	  nHits += seg_phi_nHits->at(iSeg);
	  m_plots[Form("Ph1hPhiNhits_st%d",seg_station->at(iSeg))]->Fill(seg_phi_nHits->at(iSeg));
	  m_plots[Form("Ph1hPhiChi2_st%d",seg_station->at(iSeg))]->Fill(seg_phi_normChi2->at(iSeg));

	  if(seg_phi_nHits->at(iSeg) == 8)
	    m_plots[Form("Ph1hPhiProbChi2_st%d",seg_station->at(iSeg))]->Fill(TMath::Prob(seg_phi_normChi2->at(iSeg)*6,6));

	  if (seg_phi_t0->at(iSeg) > -900)
	    {
	      float t0 = seg_phi_t0->at(iSeg);
	      float t0InRange = std::max(float(-99.9),(std::min(float(99.9),t0)));

	      m_plots[Form("Ph1hT0_st%d",seg_station->at(iSeg))]->Fill(t0InRange);
	      m_plots[Form("Ph1hT0VsX_st%d",seg_station->at(iSeg))]->Fill(seg_posLoc_x->at(iSeg), t0);
	    }
	  
	}

      if(seg_hasZed->at(iSeg))
	{

	  nHits += seg_z_nHits->at(iSeg);
	  m_plots[Form("Ph1hThetaNhits_st%d",seg_station->at(iSeg))]->Fill(seg_z_nHits->at(iSeg));

	  m_plots[Form("Ph1hThetaChi2_st%d",seg_station->at(iSeg))]->Fill(seg_z_normChi2->at(iSeg));

	  if(seg_z_nHits->at(iSeg) == 4)
	    m_plots[Form("Ph1hThetaProbChi2_st%d",seg_station->at(iSeg))]->Fill(TMath::Prob(seg_z_normChi2->at(iSeg)*2,2));

	}
      
      m_plots[Form("Ph1hNhits_st%d",seg_station->at(iSeg))]->Fill(nHits);

    }

  //Phase2

  for (uint iSeg = 0; iSeg < ph2Seg_nSegments; ++iSeg) 
    {
      
      if(ph2Seg_sector->at(iSeg) != 12 || ph2Seg_wheel->at(iSeg) != 2) continue;    

      //Chi2 cut if needed
      
      //if(ph2Seg_hasPhi->at(iSeg)    && ph2Seg_phi_normChi2->at(iSeg) > 3.84) continue;
      //if(ph2Seg_hasZed->at(iSeg) && ph2Seg_z_normChi2->at(iSeg) > 3.84) continue;
      
      //Select segment with t0 < of a specific time  to avoid out of time events
      //if(abs(ph2Seg_phi_t0->at(iSeg)) > 15) continue;
      
      int nHits = 0;
      if(ph2Seg_hasPhi->at(iSeg))
	{
	  
	  nHits += ph2Seg_phi_nHits->at(iSeg);
	  m_plots[Form("Ph2hPhiNhits_st%d",ph2Seg_station->at(iSeg))]->Fill(ph2Seg_phi_nHits->at(iSeg));
	  m_plots[Form("Ph2hPhiChi2_st%d",ph2Seg_station->at(iSeg))]->Fill(ph2Seg_phi_normChi2->at(iSeg));

	  if(ph2Seg_phi_nHits->at(iSeg) == 8)
	    m_plots[Form("Ph2hPhiProbChi2_st%d",ph2Seg_station->at(iSeg))]->Fill(TMath::Prob(ph2Seg_phi_normChi2->at(iSeg)*6,6));

	  if (ph2Seg_phi_t0->at(iSeg) > -900)
	    {
	      float t0 = ph2Seg_phi_t0->at(iSeg);
	      float t0InRange = std::max(float(-99.9),(std::min(float(99.9),t0)));

	      m_plots[Form("Ph2hT0_st%d",ph2Seg_station->at(iSeg))]->Fill(t0InRange);
	      m_plots[Form("Ph2hT0VsX_st%d",ph2Seg_station->at(iSeg))]->Fill(ph2Seg_posLoc_x->at(iSeg), t0);
	    }
	  
	}

      if(ph2Seg_hasZed->at(iSeg))
	{

	  nHits += ph2Seg_z_nHits->at(iSeg);
	  m_plots[Form("Ph2hThetaNhits_st%d",ph2Seg_station->at(iSeg))]->Fill(ph2Seg_z_nHits->at(iSeg));

	  m_plots[Form("Ph2hThetaChi2_st%d",ph2Seg_station->at(iSeg))]->Fill(ph2Seg_z_normChi2->at(iSeg));

	  if(ph2Seg_z_nHits->at(iSeg) == 4)
	    m_plots[Form("Ph2hThetaProbChi2_st%d",ph2Seg_station->at(iSeg))]->Fill(TMath::Prob(ph2Seg_z_normChi2->at(iSeg)*2,2));

	}

      m_plots[Form("Ph2hNhits_st%d",ph2Seg_station->at(iSeg))]->Fill(nHits);

    }

}

void DTNtupleSegmentAnalyzer::comparisonAnalysis()
{

  std::array<int, 4> nSegPerStPh1 = {0, 0, 0, 0};
  std::array<int, 4> nSegPerStPh2 = {0, 0, 0, 0};

  std::array<int, 4> iBestSegPerStPh1 = {-1, -1, -1, -1};
  std::array<int, 4> iBestSegPerStPh2 = {-1, -1, -1, -1};

  for (std::size_t iSeg = 0; iSeg < seg_nSegments; ++iSeg) 
    {

      if(seg_sector->at(iSeg) != 12 || seg_wheel->at(iSeg) != 2) continue;    

      if(seg_hasPhi->at(iSeg))
	{
	  
	  int stat  = seg_station->at(iSeg);
	  int nHits = seg_phi_nHits->at(iSeg);
	  int iBestSeg = iBestSegPerStPh1[stat - 1];
	  int nHitsBestSeg = iBestSeg > -1 ? seg_phi_nHits->at(iBestSeg) : 0;
	  
	  nSegPerStPh1[stat - 1]++;
	  
	  if (nHits > nHitsBestSeg)
	    iBestSegPerStPh1[stat - 1] = iSeg;

	}

    }

    for (std::size_t iSeg = 0; iSeg < ph2Seg_nSegments; ++iSeg) 
    {

      if(ph2Seg_sector->at(iSeg) != 12 || ph2Seg_wheel->at(iSeg) != 2) continue;    

      if(ph2Seg_hasPhi->at(iSeg))
	{
	  
	  int stat  = ph2Seg_station->at(iSeg);
	  int nHits = ph2Seg_phi_nHits->at(iSeg);
	  int iBestSeg = iBestSegPerStPh2[stat - 1];
	  int nHitsBestSeg = iBestSeg > -1 ? ph2Seg_phi_nHits->at(iBestSeg) : 0;
	  
	  nSegPerStPh2[stat - 1]++;
	  
	  if (nHits > nHitsBestSeg)
	    iBestSegPerStPh2[stat - 1] = iSeg;

	}
      
    }

    for (int iSt = 1; iSt <= 4; ++iSt)
      {

	int nSegPh1 = nSegPerStPh1[iSt -1];
	int nSegPh2 = nSegPerStPh2[iSt -1];
	
	if (nSegPh1 > 0 || nSegPh2 > 0)
	  {
	    m_plots[Form("NSegPh1VsPh2_st%d",iSt)]->Fill(nSegPh1,nSegPh2);

	    if (nSegPh1 > 0 && nSegPh2 > 0)
	      {
		int nHitsPh1 = seg_phi_nHits->at(iBestSegPerStPh1[iSt - 1]);
		int nHitsPh2 = ph2Seg_phi_nHits->at(iBestSegPerStPh2[iSt - 1]);

		m_plots[Form("NHitsPhiPh1VsPh2_st%d",iSt)]->Fill(nHitsPh1,nHitsPh2);

		float posXPh1 = seg_posLoc_x->at(iBestSegPerStPh1[iSt - 1]);
		float posXPh2 = ph2Seg_posLoc_x->at(iBestSegPerStPh2[iSt - 1]);

		if (std::abs(posXPh1 - posXPh2) < 10.)
		  m_plots[Form("NPh2Ph1HitsDiffPhiVsX_st%d",iSt)]->Fill(posXPh1,nHitsPh2 - nHitsPh1);		
	      }
	  }
	
      }

}

void DTNtupleSegmentAnalyzer::measureEfficiency(string Tag)
{

  DTNtupleSegment & seg = segmentObjs[Tag];
  DTNtupleDigi & digi = digiObjs[Tag];

  // First analyze phi segments
  
  for (uint iSeg=0; iSeg<(*seg.nSegments); iSeg++)
    {
      int nHitsPhi = seg.phi_nHits->at(iSeg);
      int station = seg.station->at(iSeg);
      
      //selection
      if (nHitsPhi < 7) continue;
      if (!seg.hasPhi->at(iSeg)) continue;
      if (seg.sector->at(iSeg) != 12 || seg.wheel->at(iSeg) != 2) continue;     
      if (fabs(seg.dirLoc_x->at(iSeg))>0.7) continue;
      
      auto expWire = (TVectorF*)seg.hitsExpWire->At(iSeg);
      
      // If a hit is missing, let us check that the 
      // extrapolation doesn't fall beyond layer or 
      // cross a dead cell!
      bool crossDeadWire = false; 
      bool outOfLayer = false;
      
      if (nHitsPhi < 8 )
	{
	  for (int expSL=1; expSL<4; expSL +=2)
	    {
	      for (int expLay=1; expLay<5; ++expLay)
		{

		  int slAndLay = expLay + (expSL - 1) * 4; // 1 - 12
	      
		  int nWires = 0;
	      
		  if (station==1) nWires = 49;
		  else if (station==2) nWires = 60;
		  else if (station==3) nWires = 72;
		  else if (station==4) nWires = 92;
	      
		  int expW = (*expWire)(slAndLay - 1);
	      
		  if (expW > nWires)
		    {
		      outOfLayer = true;
		      break;
		    }

		  WireId refWireId(station,expSL,expLay,expW);

		  for (const auto & wireId : m_deadWires[Tag])
		    {
		      if (wireId == refWireId)
			{
			  crossDeadWire = true;
			  break;
			}
		    }

		  if (crossDeadWire) break;
	      
		}

	      if (crossDeadWire || outOfLayer) break;
	    }
	}

      // this segment goes out of layer boundary
      // or it crosses dead cell(s): drop it!
      if (outOfLayer || crossDeadWire) continue; 
      	  
      std::bitset<12> firedLayers(0); 

      auto hitSuperLayerPhi =(TVectorF*)seg.phiHits_superLayer->At(iSeg);
      auto hitLayerPhi      =(TVectorF*)seg.phiHits_layer->At(iSeg);
      
      // checking hits from segment
      for (int iHit=0; iHit<nHitsPhi; ++iHit)
	{
	  int hitSL = (*hitSuperLayerPhi)(iHit);
	  int hitSlAndLay = hitSL == 1 ? (*hitLayerPhi)(iHit) : (*hitLayerPhi)(iHit) + 8; // hit layer 1 - 12
	  firedLayers.set(hitSlAndLay - 1);
	}

      // checking if a digi exist in a wire from extrapolation
      // and filling TEfficiency histograms 
      for (int sl=1; sl<4; sl+=2)
	{
	  for (int lay=1; lay<5; ++lay)
	    {

	      int slAndLay = lay + (sl - 1) * 4; // 1 -12

	      auto firedLayersReq = firedLayers;
	      firedLayersReq.set(slAndLay - 1, 0);

	      if (firedLayersReq.count() < 7) 
		continue;

	      bool hasFired = firedLayers.test(slAndLay - 1);
	      int expW = (*expWire)(slAndLay - 1);

	      if (!hasFired)
		{		  

		  for (uint iDigi=0; iDigi<(*digi.nDigis); ++iDigi)
		    {
		  
		      if (digi.wheel->at(iDigi)   != 2) continue;
		      if (digi.sector->at(iDigi)  != 12) continue;
		      if (digi.station->at(iDigi) != station) continue;
		      if (digi.superLayer->at(iDigi) != sl) continue;
		      if (digi.layer->at(iDigi) != lay) continue;

		      float time = digi.time->at(iDigi);
		      if (time < m_timeBoxMin.at(Tag) +  750. ||
			  time > m_timeBoxMin.at(Tag) + 1650.)
			continue;
		      
		      int digiW = digi.wire->at(iDigi);

		      if (std::abs(expW - digiW) < 1.1)
			{
			  hasFired = true;
			  break;
			}
		    }
		}
	      
	      //variables, points, stations, wheels 
	      m_effs[(Tag+"effPhi_station").c_str()]->Fill(hasFired,seg.station->at(iSeg)-0.5);    		  
	      m_effs[Form("%seffByWire_st%d",Tag.c_str(),seg.station->at(iSeg))]->Fill(hasFired,expW,slAndLay);

	    }
	}
    }
  
  // Then analyze theta segments
  
  for (uint iSeg=0; iSeg<(*seg.nSegments); ++iSeg)
    {

      int station = seg.station->at(iSeg);
      int nHitsTheta= seg.z_nHits->at(iSeg);
      
      if (nHitsTheta < 3) continue;
      if (!seg.hasZed->at(iSeg) || seg.phi_nHits->at(iSeg) < 7) continue; 
      if(seg.sector->at(iSeg) != 12 || seg.wheel->at(iSeg) != 2) continue;     
      
      auto expWire = (TVectorF*)seg.hitsExpWire->At(iSeg);

      // If a hit is missing, let us check that the 
      // extrapolation doesn't fall beyond layer or 
      // cross a dead cell!
      bool crossDeadWire = false; 
      bool outOfLayer = false;
      
      if (nHitsTheta < 4 )
	{

	  for (int expLay=1; expLay<5; ++expLay)
	    {
	      
	      int slAndLay = expLay + 4; // SL == 2 and range: 1 - 12
	      int expW = (*expWire)(slAndLay - 1);

	      if (expW > 58) // theta SLs have 58 wires
		{
		  outOfLayer = true;
		  break;
		}

	      WireId refWireId(station,2,expLay,expW);

	      for (const auto & wireId : m_deadWires[Tag])
		{
		  if (wireId == refWireId)
		    {
		      crossDeadWire = true;
		      break;
		    }
		}
	      
	      if (crossDeadWire) break;
	      
	    }  
	}

      // this segment goes out of layer boundary
      // or it crosses dead cell(s): drop it!
      if (outOfLayer || crossDeadWire) continue; 

      std::bitset<4>firedLayers(0); 

      auto hitLayerTheta = (TVectorF*)seg.zHits_layer->At(iSeg);
      
      // checking hits from segment
      for (int iHit=0; iHit<nHitsTheta; ++iHit)
	{
	  int hitLay = (*hitLayerTheta)(iHit); // hit layer 1 - 4
	  firedLayers.set(hitLay - 1);
	}

      // checking if a digi exist in a wire from extrapolation
      // and filling TEfficiency histograms 
      for (int lay=1; lay<5; ++lay)
	{

	  auto firedLayersReq = firedLayers;
	  firedLayersReq.set(lay - 1, 0);

	  if (firedLayersReq.count() < 3)
	    continue;

	  int expW = (*expWire)(lay + 3); // 4 - 7
	  bool hasFired = firedLayers.test(lay - 1); // 0 - 3

	  if (!hasFired)
	    {		  

	      for (uint iDigi=0; iDigi<(*digi.nDigis); ++iDigi)
		{
		  
		  if (digi.wheel->at(iDigi)   != 2) continue;
		  if (digi.sector->at(iDigi)  != 12) continue;
		  if (digi.station->at(iDigi) != station) continue;
		  if (digi.superLayer->at(iDigi) != 2) continue;
		  if (digi.layer->at(iDigi) != lay) continue;

		  float time = digi.time->at(iDigi);
		  if (time < m_timeBoxMin.at(Tag) +  750. ||
		      time > m_timeBoxMin.at(Tag) + 1650.)
		    continue;
		  
		  int digiW = digi.wire->at(iDigi);
		  
		  if (std::abs(expW - digiW) < 1.1)
		    {
		      hasFired = true;
		      break;
		    }
		}
	    }

	  //variables, points, stations, wheels 
	  m_effs[(Tag+"effTheta_station").c_str()]->Fill(hasFired,seg.station->at(iSeg)-0.5);    		  
	  m_effs[Form("%seffByWire_st%d",Tag.c_str(),seg.station->at(iSeg))]->Fill(hasFired,expW,lay + 4);

	}
    }

}

void DTNtupleSegmentAnalyzer::computeResidual(string Tag)
{

  DTNtupleSegment & seg = segmentObjs[Tag];

  for (std::size_t iSeg = 0; iSeg < (*seg.nSegments); ++iSeg)
    {

      if(seg.sector->at(iSeg) != 12 || seg.wheel->at(iSeg) != 2)
	continue;

      int stat = seg.station->at(iSeg);

      int segPhiNHits = seg.phi_nHits->at(iSeg);
      int segZNHits   = seg.z_nHits->at(iSeg);

      if (seg.hasPhi->at(iSeg) && segPhiNHits == 8)
	{

	  if(seg.phi_normChi2->at(iSeg) > 3.84) continue;

	  auto hitSuperLayerPhi = (TVectorF*)seg.phiHits_superLayer->At(iSeg);
	  auto hitLayerPhi      = (TVectorF*)seg.phiHits_layer->At(iSeg);
	  auto hitWirePhi       = (TVectorF*)seg.phiHits_wire->At(iSeg);
	  auto hitPosPhi        = (TVectorF*)seg.phiHits_pos->At(iSeg);     
	  auto hitPosPhiCh      = (TVectorF*)seg.phiHits_posCh->At(iSeg);	 
	  auto hitPosPhiErr     = (TVectorF*)seg.phiHits_posErr->At(iSeg);	 

	  auto hitPhiSide       = (TVectorF*)seg.phiHits_side->At(iSeg);	 
	  
	  auto hitExpPos        = (TVectorF*)seg.hitsExpPos->At(iSeg);     
	  auto hitExpPosCh      = (TVectorF*)seg.hitsExpPosCh->At(iSeg);     
	  auto hitExpWire       = (TVectorF*)seg.hitsExpWire->At(iSeg);     

	  auto hitWirePosPhi    = (TVectorF*)seg.phiHits_wirePos->At(iSeg);
	  
	  for (int iHit=0; iHit < segPhiNHits; ++iHit)
	    {

	      double wireDist = std::abs((*hitWirePosPhi)(iHit) - (*hitPosPhi)(iHit));
	      
	      if(iHit<4)
		{
		  
 		  double res = ((*hitPosPhi)(iHit) - (*hitExpPos)(iHit)) * ((*hitPhiSide)(iHit) == 2 ? 1 : -1);
		  
		  m_plots[Form("%shitResX",Tag.c_str())]->Fill(res);
		  m_plots[Form("%shitResX_st%d_sl%d",Tag.c_str(),stat,1)]->Fill(res);
		  m_plots[Form("%shitResX_st%d_sl%d_l%d",Tag.c_str(),stat,1,iHit+1)]->Fill(res);
		  
		  m_2Dplots[Form("%shitResX_vsWireDist",Tag.c_str())]->Fill(wireDist,res);
		  m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d",Tag.c_str(),stat,1)]->Fill(wireDist,res);
		  m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d_l%d",Tag.c_str(),stat,1,iHit+1)]->Fill(wireDist,res);
		  
		}
	      else
		{
		  
 		  double res = ((*hitPosPhi)(iHit) - (*hitExpPos)(iHit+4)) * ((*hitPhiSide)(iHit) == 2 ? 1 : -1);
	
		  m_plots[Form("%shitResX",Tag.c_str())]->Fill(res);
		  m_plots[Form("%shitResX_st%d_sl%d",Tag.c_str(),stat,3)]->Fill(res);
		  m_plots[Form("%shitResX_st%d_sl%d_l%d",Tag.c_str(),stat,3,iHit-3)]->Fill(res);	      
		  
		  m_2Dplots[Form("%shitResX_vsWireDist",Tag.c_str())]->Fill(wireDist,res);
		  m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d",Tag.c_str(),stat,3)]->Fill(wireDist,res);
		  m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d_l%d",Tag.c_str(),stat,3,iHit-3)]->Fill(wireDist,res);
		  
		}
	    }
	}
      
      if (seg.hasZed->at(iSeg) && segZNHits == 4)
	{

	  if(seg.z_normChi2->at(iSeg) > 3.84) continue;

	  auto hitZSide    = (TVectorF*)seg.zHits_side->At(iSeg);	 

	  auto hitExpPos   = (TVectorF*)seg.hitsExpPos->At(iSeg);     
	  auto hitExpPosCh = (TVectorF*)seg.hitsExpPosCh->At(iSeg);     
	  auto hitExpWire  = (TVectorF*)seg.hitsExpWire->At(iSeg);     

	  auto hitWirePosZ = (TVectorF*)seg.zHits_wirePos->At(iSeg);

	  auto hitLayerZ   = (TVectorF*)seg.zHits_layer->At(iSeg);
	  auto hitWireZ    = (TVectorF*)seg.zHits_wire->At(iSeg);
	  auto hitPosZ     = (TVectorF*)seg.zHits_pos->At(iSeg);     
	  auto hitPosZCh   = (TVectorF*)seg.zHits_posCh->At(iSeg);	 
	  auto hitPosZErr  = (TVectorF*)seg.zHits_posErr->At(iSeg);

	  for (int iHit=0; iHit<segZNHits; ++iHit)
	    {

	      double wirePos = std::abs((*hitWirePosZ)(iHit) - (*hitPosZ)(iHit));
	      double res     = ((*hitPosZ)(iHit) - (*hitExpPos)(iHit+4)) * ((*hitZSide)(iHit) == 2 ? 1 : -1);
	      
	      m_plots[Form("%shitResZ",Tag.c_str())]->Fill(res);
	      m_plots[Form("%shitResZ_st%d",Tag.c_str(),stat)]->Fill(res);
	      m_plots[Form("%shitResZ_st%d_l%d",Tag.c_str(),stat,iHit+1)]->Fill(res);	    
	      
	      m_2Dplots[Form("%shitResZ_vsWireDist",Tag.c_str())]->Fill(wirePos,res);
	      m_2Dplots[Form("%shitResZvsWireDist_st%d",Tag.c_str(),stat)]->Fill(wirePos,res);
	      m_2Dplots[Form("%shitResZvsWireDist_st%d_l%d",Tag.c_str(),stat,iHit+1)]->Fill(wirePos,res);
	      
	    }
	}
    }
}
