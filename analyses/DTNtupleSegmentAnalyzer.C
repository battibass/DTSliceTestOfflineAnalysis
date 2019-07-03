#include "DTNtupleSegmentAnalyzer.h"
#include "TCanvas.h"
#include "TMath.h"

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

      m_plots[Form("hPhiProbChi2_st%d",st)]    = new TH1F(Form("hPhiProbChi2_st%d",st), Form("Segment #phi prob(#chi^2) st%d; prob(#chi^2); entries",st), 200,0.,1.); 
      m_plots[Form("hThetaProbChi2_st%d",st)]  = new TH1F(Form("hThetaProbChi2_st%d",st), Form("Segment #theta Prob. #chi^2 st%d; prob(#chi^2); entries",st), 200,0.,1.); 
      m_plots[Form("hPhiChi2_st%d",st)]        = new TH1F(Form("hPhiChi2_st%d",st), Form("Segment #phi #chi^2 st%d;  #chi^2/ndof; entries",st), 200,0.,200.);
      m_plots[Form("hThetaChi2_st%d",st)]      = new TH1F(Form("hThetaChi2_st%d",st), Form("Segment #theta #chi^2 st%d; #chi^2/ndof; entries",st), 200,0.,200.);
    }
}

void DTNtupleSegmentAnalyzer::fill()
{
  
  m_plots["hNsegment"] ->Fill(seg_nSegments);

  int phiSeg = -1; int zSeg = -1;
  for (uint iseg=0; iseg<seg_nSegments; iseg++) {

    if(seg_sector->at(iseg)!=12 || seg_wheel->at(iseg)!=2) continue;    
    
    if(seg_hasPhi->at(iseg)) phiSeg++;
    if(seg_hasZed->at(iseg)) zSeg++;
    
    int nHits = 0;
    if(seg_hasPhi->at(iseg)){
      nHits+=seg_phi_nHits->at(phiSeg);
      m_plots[Form("hPhiNhits_st%d",seg_station->at(phiSeg))]->Fill(seg_phi_nHits->at(phiSeg));
    }
    if(seg_hasZed->at(iseg)){
      nHits+=seg_z_nHits->at(zSeg);
      m_plots[Form("hThetaNhits_st%d",seg_station->at(iseg))]->Fill(seg_z_nHits->at(zSeg));
    }
    m_plots[Form("hNhits_st%d",seg_station->at(iseg))]->Fill(nHits);

    if(seg_hasPhi->at(iseg)) m_plots[Form("hPhiChi2_st%d",seg_station->at(phiSeg))]->Fill(seg_phi_normChi2->at(phiSeg));
    if(seg_hasZed->at(iseg)) m_plots[Form("hThetaChi2_st%d",seg_station->at(zSeg))]->Fill(seg_z_normChi2->at(zSeg));

    if(seg_hasPhi->at(iseg) && seg_phi_nHits->at(phiSeg)==8){       
      m_plots[Form("hPhiProbChi2_st%d",seg_station->at(phiSeg))]->Fill(TMath::Prob(seg_phi_normChi2->at(phiSeg)*6,6));  
    }

    if(seg_hasZed->at(iseg) && seg_z_nHits->at(zSeg)==4){ 
      m_plots[Form("hThetaProbChi2_st%d",seg_station->at(zSeg))]->Fill(TMath::Prob(seg_z_normChi2->at(zSeg)*2,2));  
    }
  }
}

void DTNtupleSegmentAnalyzer::endJob()
{

  m_outFile.cd();
  m_outFile.Write();
  m_outFile.Close();
  
}

