#include "DTNtupleSegmentAnalyzer.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TF1.h"

DTNtupleSegmentAnalyzer::DTNtupleSegmentAnalyzer(const TString & inFileName,
						 const TString & outFileName) :
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)  
{ 

}

DTNtupleSegmentAnalyzer::~DTNtupleSegmentAnalyzer() 
{ 

}


void DTNtupleSegmentAnalyzer::Loop()
{

  book();

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();
  //nentries = 10000;

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEvent(jentry);   nbytes += nb;

      if(jentry % 100 == 0) 
	std::cout << "[DTNtupleSegmentAnalyzer::Loop] processed : " 
		  << jentry << " entries\r" << std::flush;

     fill();
     
    }

  std::cout << std::endl; 
  endJob();

}

void DTNtupleSegmentAnalyzer::book()
{
  
  m_outFile.cd();
  m_plots["hNsegment"] = new TH1F("hNsegment",
				  "# segments;# segments; entries",
				  20,0.,20.); 
  
    for (int st=1; st<5; st++){

      m_plots[Form("hPhiNhits_st%d",st)] = new TH1F(Form("hPhiNhits_st%d",st),
				    "# hits;# hits; entries",
				    8,1,9); 
 
      m_plots[Form("hThetaNhits_st%d",st)] = new TH1F(Form("hThetaNhits_st%d",st),
				    "# hits;# hits; entries",
				    4,1,5); 

      m_plots[Form("hNhits_st%d",st)] = new TH1F(Form("hNhits_st%d",st),
				    "# hits;# hits; entries",
				    12,1,13);  

      m_plots[Form("hT0_st%d",st)] = new TH1F(Form("hT0_st%d",st),
					      "Segment t0;t0 (ns); entries",
					      400,-100.,100.);  

      m_plots[Form("hPhiProbChi2_st%d",st)] = new TH1F(Form("hPhiProbChi2_st%d",st), 
						       Form("Segment #phi prob(#chi^2) st%d; prob(#chi^2); entries",st), 
						       200,0.,1.);
 
      m_plots[Form("hThetaProbChi2_st%d",st)] = new TH1F(Form("hThetaProbChi2_st%d",st), 
							 Form("Segment #theta Prob. #chi^2 st%d; prob(#chi^2); entries",st), 
							 200,0.,1.);
 
      m_plots[Form("hPhiChi2_st%d",st)] = new TH1F(Form("hPhiChi2_st%d",st), 
						   Form("Segment #phi #chi^2 st%d;  #chi^2/ndof; entries",st), 
						   200,0.,200.);

      m_plots[Form("hThetaChi2_st%d",st)] = new TH1F(Form("hThetaChi2_st%d",st), 
						     Form("Segment #theta #chi^2 st%d; #chi^2/ndof; entries",st), 
						     200,0.,200.);
    }
}

void DTNtupleSegmentAnalyzer::fill()
{
  
  m_plots["hNsegment"] ->Fill(seg_nSegments);

  for (uint iSeg=0; iSeg<seg_nSegments; iSeg++) {

    if(seg_sector->at(iSeg)!=12 || seg_wheel->at(iSeg)!=2) continue;    

    //Chi2 cut if needed

    //if(seg_hasPhi->at(iSeg)    && seg_phi_normChi2->at(iSeg) > 3.84) continue;
    //if(seg_hasZed->at(iSeg) && seg_z_normChi2->at(iSeg) > 3.84) continue;

    //Select segment with t0 < of a specific time  to avoid out of time events
    //if(abs(seg_phi_t0->at(iSeg)) > 15) continue;

    
    int nHits = 0;
    if(seg_hasPhi->at(iSeg)){
      nHits+=seg_phi_nHits->at(iSeg);
      m_plots[Form("hPhiNhits_st%d",seg_station->at(iSeg))]->Fill(seg_phi_nHits->at(iSeg));
      if (seg_phi_t0->at(iSeg) > -900)
	m_plots[Form("hT0_st%d",seg_station->at(iSeg))]->Fill(std::max(float(-99.9),(std::min(float(99.9),seg_phi_t0->at(iSeg)))));
    }
    if(seg_hasZed->at(iSeg)){
      nHits+=seg_z_nHits->at(iSeg);
      m_plots[Form("hThetaNhits_st%d",seg_station->at(iSeg))]->Fill(seg_z_nHits->at(iSeg));
    }
    m_plots[Form("hNhits_st%d",seg_station->at(iSeg))]->Fill(nHits);

    if(seg_hasPhi->at(iSeg)) m_plots[Form("hPhiChi2_st%d",seg_station->at(iSeg))]->Fill(seg_phi_normChi2->at(iSeg));
    if(seg_hasZed->at(iSeg)) m_plots[Form("hThetaChi2_st%d",seg_station->at(iSeg))]->Fill(seg_z_normChi2->at(iSeg));

    if(seg_hasPhi->at(iSeg) && seg_phi_nHits->at(iSeg)==8){       
      m_plots[Form("hPhiProbChi2_st%d",seg_station->at(iSeg))]->Fill(TMath::Prob(seg_phi_normChi2->at(iSeg)*6,6));  
    }

    if(seg_hasZed->at(iSeg) && seg_z_nHits->at(iSeg)==4){ 
      m_plots[Form("hThetaProbChi2_st%d",seg_station->at(iSeg))]->Fill(TMath::Prob(seg_z_normChi2->at(iSeg)*2,2));  
    }
  }
}

void DTNtupleSegmentAnalyzer::endJob()
{


  for (int st=1; st<5; st++){
    TF1 *f1 = new TF1(Form("fgausSt%d",st),"gaus",m_plots[Form("hT0_st%d",st)]->GetBinCenter(m_plots[Form("hT0_st%d",st)]->GetMaximumBin())-1.2*m_plots[Form("hT0_st%d",st)]->GetStdDev(),m_plots[Form("hT0_st%d",st)]->GetBinCenter(m_plots[Form("hT0_st%d",st)]->GetMaximumBin())+1.2*m_plots[Form("hT0_st%d",st)]->GetStdDev());

    m_plots[Form("hT0_st%d",st)]->Fit(Form("fgausSt%d",st),"R");

  }

  m_outFile.cd();
  m_outFile.Write();
  m_outFile.Close();
  
}

