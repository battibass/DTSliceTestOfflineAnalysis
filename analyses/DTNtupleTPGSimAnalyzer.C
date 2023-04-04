#include "DTNtupleTPGSimAnalyzer.h"

#include"TMath.h"
#include <iostream>
#include <fstream>

struct primitive {
  short station; 
  short sector; 	
  short wheel; 
  short quality; 
  short superLayer;
  int phi; 
  int phiB; 
  float position; 
  float direction; 
  int chi2; 
  int BX; 
  int t0; 
};

typedef std::vector <primitive> primitives;


DTNtupleTPGSimAnalyzer::DTNtupleTPGSimAnalyzer(const TString & inFileName,
const TString & outFileName) :
m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)  
{ 
  
  m_minMuPt = 20;
  
  m_maxMuSegDPhi = 0.2;
  m_maxMuSegDEta = 0.3;
  
  m_minSegHits = 4;
  
  m_maxSegTrigDPhi = 0.1;
  m_maxMuTrigDPhi  = 0.2;
  
  entryNumber = -1; 
  debug = false;   
  correctL1A = false; 
}
DTNtupleTPGSimAnalyzer::DTNtupleTPGSimAnalyzer(const TString & inFileName,
  const TString & outFileName,
const bool & correct) :
m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)  
{ 
  
  m_minMuPt = 20;
  
  m_maxMuSegDPhi = 0.2;
  m_maxMuSegDEta = 0.3;
  
  m_minSegHits = 4;
  
  m_maxSegTrigDPhi = 0.1;
  m_maxMuTrigDPhi  = 0.2;
  
  entryNumber = -1; 
  debug = false;  ////// 
  correctL1A = correct; 
}

DTNtupleTPGSimAnalyzer::~DTNtupleTPGSimAnalyzer() 
{ 
  
}


void DTNtupleTPGSimAnalyzer::Loop()
{
  
  if (fChain == 0) return;
  
  unlink( "failedEventNumbers.txt" );
  unlink( "failedPh1Segs.txt"      );
  unlink( "failedPh1Prims.txt"     );
  unlink( "failedPh2Prims.txt"     );
  unlink( "failedPh2Hits.txt"      );
  
  Long64_t nentries = fChain->GetEntries();
  totalEntries = nentries;
  //cout << "Creating histograms ";
  book();
  //cout << " -> Finished" << endl;
  Long64_t nbytes = 0, nb = 0;
  // for (Long64_t jentry=0; jentry<50000;jentry++) 
  for (Long64_t jentry=0; jentry<nentries;jentry++)
  
  {
    //cout << "Loading entry " << jentry; 
    Long64_t ientry = LoadTree(jentry);
    //cout << " -> Finished" << endl;
    if (ientry < 0) break;
    nb = fChain->GetEvent(jentry);   nbytes += nb;
    if(jentry % 100 == 0) 
    std::cout << "[DTNtupleTPGSimAnalyzer::Loop] processed : " 
    << jentry << " entries\r" << std::flush;
    
    entryNumber = jentry; 
    //cout << "Filling histograms " << endl;
    fill(); 
    //cout << " -> Finished filling histograms" << endl;
  }
  
  std::cout << std::endl; 
  
  endJob();
  return;
}

void DTNtupleTPGSimAnalyzer::book()
{  
  m_outFile.cd();
  
  std::vector<std::string> chambTags = {"MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> slTags = { "SL1", "SL3"};
  //std::vector<std::string> chambTags = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> quTags = {"3h","4h","Q6","Q7","Q8","bestQ"};
  //std::vector<std::string> quTags = {"Q1","Q2","Q3","Q4","Q5","Q6","Q7","Q8","Q9"};
  std::vector<std::string> labelTags = {"All","Correlated","Uncorrelated"};
  std::vector<std::string> obdtTags = {"MB1_phi1", "MB1_phi2", "MB2_phi1", "MB2_phi2", "MB3_phi1b", "MB3_phi2b", "MB4_phi1b", "MB4_phi2b","MB4_phi3b", "MB4_phi4b" };

  int nbinPosEmuFW = 101; double minPosEmuFW = 0.005 * (0.5 + nbinPosEmuFW / 2); 
  int nbinPsiEmuFW = 101; double minPsiEmuFW = 0.01 * (0.5 + nbinPsiEmuFW / 2); 
  int nbinTimeEmuFW = 101; double minTimeEmuFW = 1 * (0.5 + nbinTimeEmuFW / 2); 
  
  int nbinPosSegFW = 101; double minPosSegFW = 0.005 * (0.5 + nbinPosSegFW / 2); 
  int nbinPsiSegFW = 101; double minPsiSegFW = 0.01 * (0.5 + nbinPsiSegFW / 2); 
  int nbinTimeSegFW = 101; double minTimeSegFW = 1 * (0.5 + nbinTimeSegFW / 2); 
  
  int nbinPosph2SegFW = 101; double minPosph2SegFW = 0.005 * (0.5 + nbinPosph2SegFW / 2); 
  int nbinPsiph2SegFW = 101; double minPsiph2SegFW = 0.01 * (0.5 + nbinPsiph2SegFW / 2); 
  int nbinTimeph2SegFW = 101; double minTimeph2SegFW = 1 * (0.5 + nbinTimeph2SegFW / 2); 
  
  int nbinPhiTM = 201; double minPhiTM = 0.4;  
  int nbinPhiFW = 201; double minPhiFW = 0.4; 
  int nbinPhiBTM = 201; double minPhiBTM = 1.;  
  int nbinPhiBFW = 201; double minPhiBFW = 1.; 
  
  //return;
  m_effs["hEffCorAM"] = new TEfficiency("hEffCorAM",
    "Primitive percentage that appear in AM",
  10,-0.5,0.5); 
  m_effs["hEffCorHW"] = new TEfficiency("hEffCorHW",
    "Primitive percentage that appear in HW",
  10,-0.5,0.5); 
  m_effs["hEffCor"] = new TEfficiency("hEffCor",
    "Common correlated primitives percentage",
  10,-0.5,0.5); 
  m_plots["hQualityHW"] = new TH1F("hQualityHW",
    "Distribution of HW qualities",
  9,0.5,9.5); 
  m_plots["hQualityAM"] = new TH1F("hQualityAM",
    "Distribution of Emul qualities",
  9,0.5,9.5); 
  m_plots["hBXDif"] = new TH1F("hBXDif",
    "BX difference ; BX difference; Entries",
    //41,-220.5,-179.5); 
  7000,-3564,3564); 
  m_plots["hBXtotal"] = new TH1F("hBXtotal",
    "Distribution of BX; BX; Entries",
  3564,0,3564); 
  
  m_plots["offset"] = new TH1F("Offset",
    "Offsets used; ; Offset",
  4,-0.5,3.5);
  
  for (unsigned int ch = 0; ch < chambTags.size(); ch++) {
    m_plots["offset"]->GetXaxis()->SetBinLabel(ch+1, chambTags[ch].c_str());      
  }
 
  for (const auto & obdtTag : obdtTags) {
    m_plots["hTimeOBDT"+obdtTag] = new TH1F(("hTimeOBDT_" +  obdtTag).c_str(),
           ("Time distribution for OBDT " + obdtTag + "; Ph2 primitive t0 (ns); Entries").c_str(),
           201,-100.5,100.5);

  }

  std::vector <std::string> systems = {"HW", "TM", "AM"}; 
  std::vector <std::string> effvsWhat = {"vsSegX","vsSegT0","vsph2SegX","vsph2SegT0", "vsSegXLim","vsSegT0Lim","vsph2SegXLim","vsph2SegT0Lim",};  
  std::vector <std::string> effWhichBX = {"","GoodBX"};  
  std::vector <std::string> effQualCats = {"","Q>2","Corr"};  
  std::vector <std::string > effs = {"passed", "total"};
 
  std::map <std::string, float > minXaxis, maxXaxis, nBinsX;
  std::map <std::string, std::string > titleXaxis, qualCategory, BXtitle, unitsXaxis;
  
  minXaxis["vsSegX"] = -250.5; maxXaxis["vsSegX"] = 250.5; nBinsX["vsSegX"] = 50.; titleXaxis["vsSegX"] = "Phase-1 Segment Position"; unitsXaxis["vsSegX"] = "(cm)";
  minXaxis["vsph2SegX"] = -250.5; maxXaxis["vsph2SegX"] = 250.5; nBinsX["vsph2SegX"] = 50.; titleXaxis["vsph2SegX"] = "Phase-2 Segment Position"; unitsXaxis["vsph2SegX"] = "(cm)";
  minXaxis["vsSegT0"] = -20.5; maxXaxis["vsSegT0"] = 20.5; nBinsX["vsSegT0"] = 41.; titleXaxis["vsSegT0"] = "Phase-1 Segment t0"; unitsXaxis["vsSegT0"] = "(ns)";
  minXaxis["vsph2SegT0"] = -20.5; maxXaxis["vsph2SegT0"] = 20.5; nBinsX["vsph2SegT0"] = 41.; titleXaxis["vsph2SegT0"] = "Phase-2 Segment t0"; unitsXaxis["vsph2SegT0"] = "(ns)";
 
  minXaxis["vsSegXLim"] = -250.5; maxXaxis["vsSegXLim"] = 250.5; nBinsX["vsSegXLim"] = 50.; titleXaxis["vsSegXLim"] = "Phase-1 Segment Position"; unitsXaxis["vsSegXLim"] = "(cm)";
  minXaxis["vsph2SegXLim"] = -250.5; maxXaxis["vsph2SegXLim"] = 250.5; nBinsX["vsph2SegXLim"] = 50.; titleXaxis["vsph2SegXLim"] = "Phase-2 Segment Position"; unitsXaxis["vsph2SegXLim"] = "(cm)";
  minXaxis["vsSegT0Lim"] = -20.5; maxXaxis["vsSegT0Lim"] = 20.5; nBinsX["vsSegT0Lim"] = 41.; titleXaxis["vsSegT0Lim"] = "Phase-1 Segment t0"; unitsXaxis["vsSegT0Lim"] = "(ns)";
  minXaxis["vsph2SegT0Lim"] = -20.5; maxXaxis["vsph2SegT0Lim"] = 20.5; nBinsX["vsph2SegT0Lim"] = 41.; titleXaxis["vsph2SegT0Lim"] = "Phase-2 Segment t0"; unitsXaxis["vsph2SegT0Lim"] = "(ns)";
  
  qualCategory[""] = "Every Quality";  qualCategory["Q>2"] = "Quality>3h";   qualCategory["Corr"] = "Correlated Only"; 
  BXtitle[""] = "All BX"; BXtitle["GoodBX"] = "Good BX"; 

  for (const auto & chambTag : chambTags) {

    for (auto & effCat : effs) { // PASSED OR TOTAL
      for (auto & system : systems) { // TM, HW, Emul
        for (auto & vsWhat : effvsWhat ) { // vs Position, T0
          for (auto & whichBX : effWhichBX) { // All or Good BX
            for (auto & qualCat : effQualCats) { // Every, Q>2, Corr
             // cout << "hEff" << system << vsWhat << whichBX << qualCat << effCat << endl;
              m_plots["hEff" + system + vsWhat + whichBX + qualCat + chambTag +effCat] = new TH1F(("hEff" + system + vsWhat + whichBX + qualCat + "_" +chambTag + effCat).c_str(),
              (system + "Eff in " + BXtitle[whichBX] + " vs " + titleXaxis[vsWhat] + " - " + qualCategory[qualCat] + "; " + titleXaxis[vsWhat] + " " + unitsXaxis[vsWhat] + "; Efficiency").c_str(),
              nBinsX[vsWhat], minXaxis[vsWhat], maxXaxis[vsWhat]); 
            } 
          } 
        } 
      } 
    } 
    m_plots["hFWSeg_" + chambTag] = new TH1F(("hFWSeg_" + chambTag).c_str(),
        "Ph2 - Segment time; Ph2 - Segment time (ns); Entries",
        nbinTimeSegFW, -minTimeSegFW, minTimeSegFW); 
    m_plots["hFWSegQ>2_" + chambTag] = new TH1F(("hFWSegQg2_" + chambTag).c_str(),
        "Ph2 (Q>2) - Segment time; Ph2 (Q>2) - Segment time (ns); Entries",
        nbinTimeSegFW, -minTimeSegFW, minTimeSegFW); 
    m_plots["hTMSeg_" + chambTag] = new TH1F(("hTMSeg_" + chambTag).c_str(),
        "TM - Segment time; TM - Segment time (ns); Entries",
        nbinTimeSegFW, -minTimeSegFW, minTimeSegFW);
    m_plots["hBXFWSeg_" + chambTag] = new TH1F(("hBXFWSeg_" + chambTag).c_str(),
        "Phase 2 Primitive BX; Phase 2 Primitive BX (ns); Entries",
        5, -2.5, 2.5); 
    m_plots["hBXFWSegQ>2_" + chambTag] = new TH1F(("hBXFWSegQg2_" + chambTag).c_str(),
        "Phase 2 Primitive BX (Q > 2); Phase 2 Primitive BX (ns); Entries",
        5, -2.5, 2.5); 
    m_plots["hBXTMSeg_" + chambTag] = new TH1F(("hBXTMSeg_" + chambTag).c_str(),
        "Phase 1 Primitive BX; Phase 1 Primitive BX (ns); Entries",
        5, -2.5, 2.5); 

    //QUALITY DISTRIBUTIONS
    m_plots["hQualityHW"+chambTag] = new TH1F(("hQualityHW_" +chambTag).c_str(),
      "Distribution of HW qualities; HW Qualities; Entries",
    9,0.5,9.5); 
    m_plots["hQualityAM"+chambTag] = new TH1F(("hQualityAM_" +chambTag).c_str(),
      "Distribution of Emul qualities; Emul Qualities; Entries",
    9,0.5,9.5); 
    m_plots["hQualityTM"+chambTag] = new TH1F(("hQualityTM_" +chambTag).c_str(),
      "Distribution of TM qualities; TM Qualities; Entries",
    6,1.5,7.5); 
    
    m_plots2["hQualityVsBXHW"+chambTag] = new TH2F(("hQualityVsBXHW_" +chambTag).c_str(),
      "Distribution of Quality vs BX for HW; HW Primitive Quality; HW Primitive BX - Offset",
    9,0.5,9.5,21,-10.5,10.5); 
    m_plots2["hQualityVsBXAM"+chambTag] = new TH2F(("hQualityVsBXAM_" +chambTag).c_str(),
      "Distribution of Quality vs BX for Emul; Emul Primitive Quality; Emul Primitive BX - Offset",
    9,0.5,9.5,21,-10.5,10.5); 

    // multiplicity per event
    m_plots["hMultiplicityHW"+chambTag] = new TH1F(("hMultiplicityHW_" +chambTag).c_str(),
      "; Number of Phase-2 Primitives per event; Entries",
    26,-0.5,25.5);

    // BX window per event
    m_plots["hBXWindowHW"+chambTag] = new TH1F(("hBXWindowHW_" +chambTag).c_str(),
      "; Phase-2 Primitives BX window; Entries / 4 BXs",
    51,-0.5,50.5); 

    m_plots["hBXTM"+chambTag] = new TH1F(("hBXTM_" +chambTag).c_str(),
      "Distribution of BX Phase-1 Primitive; BX Phase-1 Primitive; Entries",
    21,-10.5,10.5); 
    
    m_plots["hLatenciesHW"+chambTag] = new TH1F(("hLatenciesHW_" +chambTag).c_str(),
      "Distribution of HW Latencies; HW Latency (BX); Entries",
    501,-0.5,500.5); 
    
    m_plots2["hHitsVsLatenciesHW"+chambTag] = new TH2F(("hHitsVsLatenciesHW_" +chambTag).c_str(),
      "Distribution of HW Latencies vs number of Hits; Number of hits in the chamber; HW Latency (BX)",
    51,-0.5, 50.5,501,-0.5,500.5); 
    m_plots2["hPositionVsHitsHW"+chambTag] = new TH2F(("hPositionVsHitsHW_" +chambTag).c_str(),
      "Distribution of Position vs number of Hits; HW Position (cm); Number of hits in the chamber",
    250,-250, 250,11,-0.5,10.5); 
    m_plots2["hQualityVsLatenciesHW"+chambTag] = new TH2F(("hQualityVsLatenciesHW_" +chambTag).c_str(),
      "Distribution of HW Latencies vs HW Quality; HW Quality; HW Latency (BX)",
    10,0.5, 10.5,501,-0.5,500.5); 
    
      
    m_plots["hHitsPerChamber"+chambTag] = new TH1F(("hHitsPerChamber_" +chambTag).c_str(),
      "Distribution of Hits per chamber; Number of hits; Entries",
    50,0.5,50.5);
    m_plots["hHitsPerChamber"+chambTag+"SL1"] = new TH1F(("hHitsPerChamber_" +chambTag + "SL1").c_str(),
      "Distribution of Hits in SL1; Number of hits; Entries",
    50,0.5,50.5);
    m_plots["hHitsPerChamber"+chambTag+"SL3"] = new TH1F(("hHitsPerChamber_" +chambTag + "SL3").c_str(),
      "Distribution of Hits in SL3; Number of hits; Entries",
    50,0.5,50.5);


    m_plots2["hHits"+chambTag] = new TH2F(("hHits_" +chambTag).c_str(),
      "Distribution of Hits; Cell Number; ",
    100,0.5,100.5,14,0,14.5); 
    std::vector<std::string> tagsHits = {"SL1 La1", "SL1 La2", "SL1 La3","SL1 La4", " ", "SL2 La1", "SL2 La2", "SL2 La3","SL2 La4", " ", "SL3 La1", "SL3 La2", "SL3 La3","SL3 La4"};
    
    for (unsigned int i = 0; i < tagsHits.size(); i++){
      m_plots2["hHits" + chambTag]->GetYaxis()->SetBinLabel(i+1, tagsHits[i].c_str());
    }
    
    
    
    /*****************************************************************************************************************************
      *				         		DATA - EMULATOR
    *****************************************************************************************************************************/
    
    m_plots2["hPrimNumber" + chambTag] = new TH2F(("hPrimNumber_" + chambTag).c_str(),
      "Distribution of number of primitives; Number of AM_HW; Number of AM_Emul",
    11,-0.5,10.5,11,-0.5,10.5); 
    m_plots2["hPrimTypeVsPos"+ chambTag] = new TH2F(("hPrimTypeVsPos_"+ chambTag).c_str(),
      "Number of each type of primitive vs Position; Position (cm); Type",
    50,-250.5,250.5, 3,-0.5,2.5); 
    m_plots2["hCorPrimNumber" + chambTag] = new TH2F(("hCorPrimNumber_" + chambTag).c_str(),
      "Distribution of number of correlated primitives; Number of Cor AM_HW; Number of Cor AM_Emul",
    11,-0.5,10.5,11,-0.5,10.5); 
    m_plots2["hAMQuality" + chambTag] = new TH2F(("hAMQuality_" + chambTag).c_str(),
      "; ; EntryNumber",
    5,-0.5,4.5,totalEntries,0,totalEntries); 
    m_plots2["hHWQuality" + chambTag] = new TH2F(("hHWQuality_" + chambTag).c_str(),
      "; ; EntryNumber",
    5,-0.5,4.5,totalEntries,0,totalEntries); 
    m_plots2["hAMQualityWhenHWcor" + chambTag] = new TH2F(("hAMQualityWhenHWcor_" + chambTag).c_str(),
      "; ; EntryNumber",
    5,-0.5,4.5,totalEntries,0,totalEntries); 
    m_plots2["hHWQualityWhenAMcor" + chambTag] = new TH2F(("hHWQualityWhenAMcor_" + chambTag).c_str(),
      "; ; EntryNumber",
    5,-0.5,4.5,totalEntries,0,totalEntries); 
    m_plots2["hEventQuality" + chambTag] = new TH2F(("hEventQuality_" + chambTag).c_str(),
      "; AM_FW; AM_Emul",
    5,-0.5,4.5,5,-0.5,4.5); 
    std::vector<std::string> tagsEvent = {"Correlated", "SL1&SL3", "Only SL1","Only SL3","None"};
    std::vector<std::string> tagsSL = {"Correlated", "SL1","SL3"};
    for (unsigned int i = 0; i < tagsSL.size(); i++){
      m_plots2["hPrimTypeVsPos"+ chambTag]->GetYaxis()->SetBinLabel(i+1, tagsSL[i].c_str());
    }
    for (unsigned int i = 0; i < tagsEvent.size(); i++){
      m_plots2["hAMQuality" + chambTag]->GetXaxis()->SetBinLabel(i+1, tagsEvent[i].c_str());
      m_plots2["hHWQuality" + chambTag]->GetXaxis()->SetBinLabel(i+1, tagsEvent[i].c_str());
      m_plots2["hAMQualityWhenHWcor" + chambTag]->GetXaxis()->SetBinLabel(i+1, tagsEvent[i].c_str());
      m_plots2["hHWQualityWhenAMcor" + chambTag]->GetXaxis()->SetBinLabel(i+1, tagsEvent[i].c_str());
      m_plots2["hEventQuality" + chambTag]->GetXaxis()->SetBinLabel(i+1, tagsEvent[i].c_str());
      m_plots2["hEventQuality" + chambTag]->GetYaxis()->SetBinLabel(i+1, tagsEvent[i].c_str());
    }
    
    
    
    /****************************************************************************************************************************/
    
    
    m_plots["hSLHW" + chambTag] = new TH1F(("hSLHW_" + chambTag).c_str(),
      "Distribution of HW SL; SL; Entries",
    4,-0.5,3.5); 
    m_plots["ht0" + chambTag] = new TH1F(("ht0_" + chambTag).c_str(),
      "Distribution of t0; t0 (ns); Entries",
    9000,-90000,90000); 
    m_plots["hBXAll" + chambTag] = new TH1F(("hBXAll_" + chambTag).c_str(),
      "Distribution of All BXs; BX; Entries",
    3564,0,3564); 
    m_plots["hBXNextOrbit" + chambTag] = new TH1F(("hBXNextOrbit_" + chambTag).c_str(),
      "Distribution of BX from Next Orbit Primx; BX; Entries",
    3564,0,3564); 
    m_plots["hdift0" + chambTag] = new TH1F(("hdift0_" + chambTag).c_str(),
      "Distribution of #Delta t0; #Delta t0 (ns); Entries",
    90000,-90000,90000); 
    m_plots["hSLAM" + chambTag] = new TH1F(("hSLAM_" + chambTag).c_str(),
      "Distribution of AM SL; SL; Entries",
    4,-0.5,3.5); 
    m_plots["hPrimsSegs" + chambTag] = new TH1F(("hPrimsSegs_" + chambTag).c_str(),
      "Number of primitives (Qu>=8) and 8-hit-Phase-1 segments; ; Entries",
    4,-0.5,3.5); 
    m_plots["hPrimsph2Segs" + chambTag] = new TH1F(("hPrimsph2Segs_" + chambTag).c_str(),
      "Number of primitives (Qu>=8) and 8-hit-Phase-2 segments; ; Entries",
    4,-0.5,3.5); 
    std::vector<std::string> tags = {"AM_FW", "AM_Emul","Phase-1 Segments","TwinMux In"};
    for (unsigned int i = 0; i < tags.size(); i++){
      m_plots["hPrimsSegs" + chambTag]->GetXaxis()->SetBinLabel(i+1, tags[i].c_str());
    }
    std::vector<std::string> ph2tags = {"AM_FW", "AM_Emul","Phase-2 Segments","TwinMux In"};
    for (unsigned int i = 0; i < tags.size(); i++){
      m_plots["hPrimsph2Segs" + chambTag]->GetXaxis()->SetBinLabel(i+1, ph2tags[i].c_str());
    }
    // m_plots2["h2DHwQualSegNHits" + chambTag] = new TH2F(("h2DHwQualSegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-1 Segment; Phase-2 Primitive Quality",5,3.5,8.5,5,-0.5,4.5);
    // m_plots2["h2DEmuQualSegNHits" + chambTag] = new TH2F(("h2DEmuQualSegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-1 Segment; Phase-2 Emulator Quality",5,3.5,8.5,5,-0.5,4.5);
    // m_plots2["h2DTMQualSegNHits" + chambTag] = new TH2F(("h2DTMQualSegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-1 Segment; Phase-1 Primitive Quality",5,3.5,8.5,5,0.5,5.5);
    // m_plots2["h2DHwQualph2SegNHits" + chambTag] = new TH2F(("h2DHwQualph2SegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-2 Segment; Phase-2 Primitive Quality",5,3.5,8.5,5,-0.5,4.5);
    // m_plots2["h2DEmuQualph2SegNHits" + chambTag] = new TH2F(("h2DEmuQualph2SegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-2 Segment; Phase-2 Emulator Quality",5,3.5,8.5,5,-0.5,4.5);
    // m_plots2["h2DTMQualph2SegNHits" + chambTag] = new TH2F(("h2DTMQualph2SegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-2 Segment; Phase-1 Primitive Quality",5,3.5,8.5,5,0.5,5.5);
    m_plots2["h2DHwQualSegNHits" + chambTag] = new TH2F(("h2DHwQualSegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-1 Segment; Phase-2 FW Primitive Quality; Events",5,3.5,8.5,5,-0.5,4.5);
    m_plots2["h2DEmuQualSegNHits" + chambTag] = new TH2F(("h2DEmuQualSegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-1 Segment; Phase-2 Emulator Primitive Quality; Events",5,3.5,8.5,5,-0.5,4.5);
    m_plots2["h2DTMQualSegNHits" + chambTag] = new TH2F(("h2DTMQualSegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-1 Segment; Phase-1 Primitive Quality; Events",5,3.5,8.5,5,0.5,5.5);
    m_plots2["h2DHwQualph2SegNHits" + chambTag] = new TH2F(("h2DHwQualph2SegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-2 Segment; Phase-2 FW Primitive Quality; Events",5,3.5,8.5,5,-0.5,4.5);
    m_plots2["h2DEmuQualph2SegNHits" + chambTag] = new TH2F(("h2DEmuQualph2SegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-2 Segment; Phase-2 Emulator Primitive Quality; Events",5,3.5,8.5,5,-0.5,4.5);
    m_plots2["h2DTMQualph2SegNHits" + chambTag] = new TH2F(("h2DTMQualph2SegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-2 Segment;  Phase-1 Primitive Quality; Events",5,3.5,8.5,5,0.5,5.5);    
    std::vector <std::string> qual_seg = {"h2DHwQualSegNHits", "h2DEmuQualSegNHits", "h2DTMQualSegNHits", "h2DHwQualph2SegNHits", "h2DEmuQualph2SegNHits", "h2DTMQualph2SegNHits"};
    //std::vector <std::string> quality_groups = {"3/4", "4/4", "6/8", "7/8", "8/8"};
    std::vector <std::string> quality_groups = {"1", "3", "6", "7", "8"};
    
    for (auto &st: qual_seg){
        for (unsigned int i = 0; i < quality_groups.size(); i++){
            m_plots2[st + chambTag] -> GetYaxis() -> SetBinLabel(i + 1, quality_groups[i].c_str());
        }
    }
    
    m_plots["hHwLowQualSegNHits" + chambTag] = new TH1F(("hHwLowQualSegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-1 Segment; Events", 6, 2.5, 8.5);
    m_plots["hEmuLowQualSegNHits" + chambTag] = new TH1F(("hEmuLowQualSegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-1 Segment; Events", 6, 2.5, 8.5);
    m_plots["hTMLowQualSegNHits" + chambTag] = new TH1F(("hTMLowQualSegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-1 Segment; Events", 6, 2.5, 8.5);
    m_plots["hHwLowQualph2SegNHits" + chambTag] = new TH1F(("hHwLowQualph2SegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-2 Segment; Events", 6, 2.5, 8.5);
    m_plots["hEmuLowQualph2SegNHits" + chambTag] = new TH1F(("hEmuLowQualph2SegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-2 Segment; Events", 6, 2.5, 8.5);
    m_plots["hTMLowQualph2SegNHits" + chambTag] = new TH1F(("hTMLowQualph2SegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-2 Segment; Events", 6, 2.5, 8.5);
    
    m_plots["hHwHighQualSegNHits" + chambTag] = new TH1F(("hHwHighQualSegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-1 Segment; Events", 6, 2.5, 8.5);
    m_plots["hEmuHighQualSegNHits" + chambTag] = new TH1F(("hEmuHighQualSegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-1 Segment; Events", 6, 2.5, 8.5);
    m_plots["hTMHighQualSegNHits" + chambTag] = new TH1F(("hTMHighQualSegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-1 Segment; Events", 6, 2.5, 8.5);
    m_plots["hHwHighQualph2SegNHits" + chambTag] = new TH1F(("hHwHighQualph2SegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-2 Segment; Events", 6, 2.5, 8.5);
    m_plots["hEmuHighQualph2SegNHits" + chambTag] = new TH1F(("hEmuHighQualph2SegNHits_" + chambTag).c_str(),"; PNo. of associated hits to the Phase-2 Segment; Events", 6, 2.5, 8.5);
    m_plots["hTMHighQualph2SegNHits" + chambTag] = new TH1F(("hTMHighQualph2SegNHits_" + chambTag).c_str(),"; No. of associated hits to the Phase-2 Segment; Events", 6, 2.5, 8.5);
    
    for (const auto & labelTag : labelTags) {
      m_plots["hBX" + chambTag + labelTag] = new TH1F(("hBX_" + chambTag + "_" + labelTag).c_str(),
        "Distribution of BX; Phase-2 Primitive BX; Entries",
      21,-10.5,10.5); 
      //201,3199.5,3400.5); 
      m_plots["hBXDif" + chambTag + labelTag] = new TH1F(("hBXDif_" + chambTag + "_" + labelTag).c_str(),
        "BX difference ; Phase-2 Primitive BX difference; Entries",
        //41,-220.5,-179.5); 
      7128,-3564,3564); 
      m_plots["hBXEmul" + chambTag + labelTag] = new TH1F(("hBXEmul_" + chambTag + "_" + labelTag).c_str(),
        "Distribution of BX; Phase-2 Emulator BX; Entries",
      21,-10.5,10.5); 
      m_plots["hBXDifEmul" + chambTag + labelTag] = new TH1F(("hBXDifEmul_" + chambTag + "_" + labelTag).c_str(),
        "BX difference ; Phase-2 Emulator BX difference; Entries",
        //41,-220.5,-179.5); 
      7000,-3564,3564); 
      m_plots["hBXfromT0" + chambTag + labelTag] = new TH1F(("hBXfromT0_" + chambTag + "_" + labelTag).c_str(),
        "Distribution of BX got from T0; BX; Entries",
      7128,-3564,3564); 
      //201,3199.5,3400.5); 
      m_plots["hChi2Emul" +  chambTag + labelTag] = new TH1F(("hChi2Emul_" + chambTag + "_" + labelTag).c_str(),
        "Distribution of #chi^2; Phase-2 Primitive #chi^2; Entries",
      10000,-0.5,99999.5); 
      m_plots["hChi2FW" +  chambTag + labelTag] = new TH1F(("hChi2FW_" + chambTag + "_" + labelTag).c_str(),
        "Distribution of #chi^2; Phase-2 Primitive #chi^2; Entries",
      10000,-0.5,99999.5); 
      m_plots["hPsiHW" + chambTag + labelTag] = new TH1F(("hPsiHW_"+ chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive #Psi Distribution; Phase-2 Primitive #Psi ( #circ); Entries",
      160,-80,80); 
      m_plots["hPsiEmul" + chambTag + labelTag] = new TH1F(("hPsiEmul_"+ chambTag + "_" + labelTag).c_str(),
        "Phase-2 Emulator #Psi Distribution; Phase-2 Emulator #Psi ( #circ); Entries",
      160,-80,80); 
      m_plots2["hPsi2D" + chambTag + labelTag] = new TH2F(("hPsi2D_"+ chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive vs Phase-2 Emulator #Psi; Phase-2 Primitive #Psi ( #circ); Phase-2 Emulator #Psi ( #circ)",
      160,-80,80,160,-80,80); 
      m_plots2["hTime2D"+ chambTag + labelTag] = new TH2F(("hTime2D_" + chambTag + "_"+ labelTag).c_str(),
        "Phase-2 Primitive vs Phase-2 Emulator Time; Phase-2 Primitive time (ns); Phase-2 Emulator time (ns)",
      400,-200,200,400,-200,200); 
      m_plots2["hPos2D"+ chambTag + labelTag] = new TH2F(("hPos2D_" + chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive vs Phase-2 Emulator Position; Phase-2 Primitive Position (cm); Phase-2 Emulator Position (cm)",
      600,-300,300,600,-300,300); 
      m_plots["hPsi"+ chambTag + labelTag] = new TH1F(("hPsi_"+ chambTag + "_" +labelTag).c_str(),
        "Phase-2 Primitive - Phase-2 Emulator #Psi; Phase-2 Primitive - Phase-2 Emulator #Psi (#circ); Entries",
      nbinPsiEmuFW,-minPsiEmuFW,minPsiEmuFW); 
      m_plots["hTime"+ chambTag + labelTag] = new TH1F(("hTime_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-2 Emulator time; Phase-2 Primitive - Phase-2 Emulator Time (ns); Entries",
      nbinTimeEmuFW,-minTimeEmuFW,minTimeEmuFW); 
      m_plots["hPos"+ chambTag + labelTag] = new TH1F(("hPos_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-2 Emulator Position; Phase-2 Primitive - Phase-2 Emulator Position (cm); Entries",
      nbinPosEmuFW,-minPosEmuFW,minPosEmuFW); 
      
      m_plots2["hPsi2DSeg" + chambTag + labelTag] = new TH2F(("hPsi2DSeg_" + chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Segment #Psi; Phase-2 Primitive #Psi (#circ); Phase-1 Segment #Psi (#circ)",
      160,-80,80,160,-80,80); 
      m_plots2["hTime2DSeg"+ chambTag + labelTag] = new TH2F(("hTime2DSeg_"+ chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Segment time; Phase-2 Primitive time (ns); Phase-1 Segment time (ns)",
      400,-200,200,200,-100,100); 
      m_plots2["hPos2DSeg"+ chambTag + labelTag] = new TH2F(("hPos2DSeg_"+ chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Segment Position; Phase-2 Primitive Position (cm); Phase-1 Segment Position (cm)",
      600,-300,300,600,-300,300); 
      m_plots2["hPsi2Dph2Seg" + chambTag + labelTag] = new TH2F(("hPsi2Dph2Seg_" + chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive vs Phase-2 Segment #Psi; Phase-2 Primitive #Psi (#circ); Phase-2 Segment #Psi (#circ)",
      160,-80,80,160,-80,80); 
      m_plots2["hTime2Dph2Seg"+ chambTag + labelTag] = new TH2F(("hTime2Dph2Seg_"+ chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive vs Phase-2 Segment time; Phase-2 Primitive time (ns); Phase-2 Segment time (ns)",
      400,-200,200,200,-100,100); 
      m_plots2["hPos2Dph2Seg"+ chambTag + labelTag] = new TH2F(("hPos2Dph2Seg_"+ chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive vs Phase-2 Segment Position; Phase-2 Primitive Position (cm); Phase-2 Segment Position (cm)",
      600,-300,300,600,-300,300); 
      
      m_plots2["hPos2DTM"+ chambTag + labelTag] = new TH2F(("hPos2DTM_"+ chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Primitive Position; Phase-2 Primitive Position (cm); Phase-1 Primitive Position (cm)",
      600,-300,300,600,-300,300); 
      m_plots2["hPsi2DTM" + chambTag + labelTag] = new TH2F(("hPsi2DTM_" + chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Primitive #Psi; Phase-2 Primitive #Psi (#circ); Phase-1 Primitive #Psi (#circ)",
      160,-80,80,160,-80,80); 
      m_plots2["hPhi2DTM"+ chambTag + labelTag] = new TH2F(("hPhi2DTM_"+ chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Primitive phi; Phase-2 Primitive phi (rad); Phase-1 Primitive phi (rad)",
      nbinPhiFW,-minPhiFW,minPhiFW,nbinPhiTM,-minPhiTM,minPhiTM); 
      m_plots2["hPhiB2DTM"+ chambTag + labelTag] = new TH2F(("hPhiB2DTM_"+ chambTag + "_" + labelTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Primitive phiB; Phase-2 Primitive phiB (rad); Phase-1 Primitive phiB (rad)",
      nbinPhiBFW,-minPhiBFW,minPhiBFW,nbinPhiBTM,-minPhiBTM,minPhiBTM); 
      m_plots["hPsiSeg"+ chambTag + labelTag] = new TH1F(("hPsiSeg_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-1 Segment #Psi; Phase-2 Primitive - Phase-1 Segment #Psi (#circ); Entries",
      nbinPsiSegFW,-minPsiSegFW,minPsiSegFW); 
      m_plots["hTimeSeg"+ chambTag + labelTag] = new TH1F(("hTimeSeg_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-1 Segment time; Phase-2 Primitive - Phase-1 Segment time (ns); Entries",
      nbinTimeSegFW,-minTimeSegFW,minTimeSegFW); 
      m_plots["hPosSeg"+ chambTag + labelTag] = new TH1F(("hPosSeg_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-1 Segment Position; Phase-2 Primitive - Phase-1 Segment Position (cm); Entries",
      nbinPosSegFW,-minPosSegFW,minPosSegFW); 
      m_plots2["hTimeSegvsPos"+ chambTag + labelTag] = new TH2F(("hTimeSegvsPos_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-1 Segment time vs Phase-1 Segment Position; Phase-1 Segment Position (cm); Phase-2 Primitive - Phase-1 Segment time (ns)",
      600,-300,300,200,-100,100); 
      m_plots2["hTimeSegvsPsi"+ chambTag + labelTag] = new TH2F(("hTimeSegvsPsi_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-1 Segment time vs Phase-1 Segment #Psi; Phase-1 Segment #Psi (#circ); Phase-2 Primitive - Phase-1 Segment time (ns)",
      160,-80,80,200,-100,100); 
      m_plots2["hTimeSegvsZ"+ chambTag + labelTag] = new TH2F(("hTimeSegvsSegZ_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-1 Segment time vs Phase-1 Segment z; Phase-1 Segment z; Phase-2 Primitive - Phase-1 Segment time (ns)",
      200,-200,200,50,-50,0); 
      
      m_plots["hPsiph2Seg"+ chambTag + labelTag] = new TH1F(("hPsiph2Seg_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-2 Segment #Psi; Phase-2 Primitive - Phase-2 Segment #Psi (#circ); Entries",
      nbinPsiph2SegFW,-minPsiph2SegFW,minPsiph2SegFW); 
      m_plots["hTimeph2Seg"+ chambTag + labelTag] = new TH1F(("hTimeph2Seg_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-2 Segment time; Phase-2 Primitive - Phase-2 Segment time (ns); Entries",
      nbinTimeph2SegFW,-minTimeph2SegFW,minTimeph2SegFW); 
      m_plots["hPosph2Seg"+ chambTag + labelTag] = new TH1F(("hPosph2Seg_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-2 Segment Position; Phase-2 Primitive - Phase-2 Segment Position (cm); Entries",
      nbinPosph2SegFW,-minPosph2SegFW,minPosph2SegFW); 
      m_plots2["hTimeph2SegvsPos"+ chambTag + labelTag] = new TH2F(("hTimeph2SegvsPos_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-2 Segment time vs Phase-2 Segment Position; Phase-2 Segment Position (cm); Phase-2 Primitive - Phase-2 Segment time (ns)",
      600,-300,300,200,-100,100); 
      m_plots2["hTimeph2SegvsPsi"+ chambTag + labelTag] = new TH2F(("hTimeph2SegvsPsi_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-2 Segment time vs Phase-2 Segment #Psi; Phase-2 Segment #Psi (#circ); Phase-2 Primitive - Phase-2 Segment time (ns)",
      160,-80,80,200,-100,100); 
      m_plots2["hTimeph2SegvsZ"+ chambTag + labelTag] = new TH2F(("hTimeph2Segvsph2SegZ_"+ chambTag + "_"+labelTag).c_str(),
        "Phase-2 Primitive - Phase-2 Segment time vs Phase-2 Segment z; Phase-2 Segment z; Phase-2 Primitive - Phase-2 Segment time (ns)",
      200,-200,200,50,-50,0); 
      if (labelTag == "All" || labelTag == "Correlated") continue;
      for (const auto & slTag : slTags) {
        m_plots2["hPhi2DTM"+ chambTag + labelTag + slTag] = new TH2F(("hPhi2DTM_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive vs Phase-1 Primitive phi; Phase-2 Primitive phi (rad); Phase-1 Primitive phi (rad)",
        nbinPhiFW,-minPhiFW,minPhiFW,nbinPhiTM,-minPhiTM,minPhiTM); 
        m_plots2["hPhiB2DTM"+ chambTag + labelTag + slTag] = new TH2F(("hPhiB2DTM_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive vs Phase-1 Primitive phiB; Phase-2 Primitive phiB (rad); Phase-1 Primitive phiB (rad)",
        nbinPhiBFW,-minPhiBFW,minPhiBFW,nbinPhiBTM,-minPhiBTM,minPhiBTM); 
        m_plots2["hPos2DTM"+ chambTag + labelTag + slTag] = new TH2F(("hPos2DTM_"+ chambTag + "_" + labelTag + "_" + slTag ).c_str(),
          "Phase-2 Primitive vs Phase-1 Primitive Position; Phase-2 Primitive Position (cm); Phase-1 Primitive Position (cm)",
        600,-300,300,600,-300,300); 
        m_plots2["hPsi2DTM" + chambTag + labelTag + slTag] = new TH2F(("hPsi2DTM_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive vs Phase-1 Primitive #Psi; Phase-2 Primitive #Psi (#circ); Phase-1 Primitive #Psi (#circ)",
        160,-80,80,160,-80,80); 
        m_plots["hBX" + chambTag + labelTag + slTag] = new TH1F(("hBX_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Distribution of BX; BX; Entries",
        21,-10.5,10.5); 
        m_plots["hBXDif" + chambTag + labelTag + slTag] = new TH1F(("hBXDif_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "BX difference ; BX difference; Entries",
          //41,-220.5,-179.5); 
        7128,-3564,3564); 
        m_plots["hBXEmul" + chambTag + labelTag + slTag] = new TH1F(("hBXEmul_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Distribution of BX; BX; Entries",
        21,-10.5,10.5); 
        m_plots["hBXDifEmul" + chambTag + labelTag + slTag] = new TH1F(("hBXDifEmul_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "BX difference ; BX difference; Entries",
          //41,-220.5,-179.5); 
        7000,-3564,3564); 
        m_plots["hBXfromT0" + chambTag + labelTag + slTag] = new TH1F(("hBXfromT0_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Distribution of BX got from T0; BX; Entries",
        201,3199.5,3400.5); 
        m_plots["hChi2Emul" +  chambTag + labelTag + slTag] = new TH1F(("hChi2Emul_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Distribution of #chi^2; #chi^2; Entries",
        10000,-0.5,99999.5); 
        m_plots["hChi2FW" +  chambTag + labelTag + slTag] = new TH1F(("hChi2FW_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Distribution of #chi^2; #chi^2; Entries",
        10000,-0.5,99999.5); 
        m_plots["hPsiHW" + chambTag + labelTag + slTag] = new TH1F(("hPsiHW_"+ chambTag + "_" + labelTag+ "_" + slTag).c_str(),
          "Phase-2 Primitive #Psi Distribution; Phase-2 Primitive #Psi ( #circ); Entries",
        160,-80,80); 
        m_plots["hPsiEmul" + chambTag + labelTag + slTag] = new TH1F(("hPsiEmul_"+ chambTag + "_" + labelTag+ "_" + slTag).c_str(),
          "Phase-2 Emulator #Psi Distribution; Phase-2 Emulator #Psi ( #circ); Entries",
        160,-80,80); 
        m_plots2["hPsi2D" + chambTag + labelTag + slTag] = new TH2F(("hPsi2D_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive vs Phase-2 Emulator #Psi; Phase-2 Primitive #Psi ( #circ); Phase-2 Emulator #Psi ( #circ)",
        160,-80,80,160,-80,80); 
        m_plots2["hTime2D"+ chambTag + labelTag + slTag] = new TH2F(("hTime2D_" + chambTag + "_"+ labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive vs Phase-2 Emulator Time; Phase-2 Primitive time (ns); Phase-2 Emulator time (ns)",
        200,-100,100,200,-100,100); 
        m_plots2["hPos2D"+ chambTag + labelTag + slTag] = new TH2F(("hPos2D_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive vs Phase-2 Emulator Position; Phase-2 Primitive Position (cm); Phase-2 Emulator Position (cm)",
        300,0,300,300,0,300); 
        m_plots["hPsi"+ chambTag + labelTag + slTag] = new TH1F(("hPsi_"+ chambTag + "_" +labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-2 Emulator #Psi; Phase-2 Primitive - Phase-2 Emulator #Psi (#circ); Entries",
        nbinPsiEmuFW,-minPsiEmuFW,minPsiEmuFW); 
        m_plots["hTime"+ chambTag + labelTag + slTag] = new TH1F(("hTime_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-2 Emulator time; Phase-2 Primitive - Phase-2 Emulator Time (ns); Entries",
        nbinTimeEmuFW,-minTimeEmuFW,minTimeEmuFW); 
        m_plots["hPos"+ chambTag + labelTag + slTag] = new TH1F(("hPos_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-2 Emulator Position; Phase-2 Primitive - Phase-2 Emulator Position (cm); Entries",
        nbinPosEmuFW,-minPosEmuFW,minPosEmuFW); 
        m_plots2["hPsi2DSeg" + chambTag + labelTag + slTag] = new TH2F(("hPsi2DSeg_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive vs Phase-1 Segment #Psi; Phase-2 Primitive #Psi (#circ); Phase-1 Segment #Psi (#circ)",
        160,-80,80,160,-80,80); 
        m_plots2["hTime2DSeg"+ chambTag + labelTag + slTag] = new TH2F(("hTime2DSeg_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive vs Phase-1 Segment time; Phase-2 Primitive time (ns); Phase-1 Segment time (ns)",
        400,-200,200,200,-100,100); 
        m_plots2["hPos2DSeg"+ chambTag + labelTag + slTag] = new TH2F(("hPos2DSeg_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive vs Phase-1 Segment Position; Phase-2 Primitive Position (cm); Phase-1 Segment Position (cm)",
        600,-300,300,600,-300,300); 
        m_plots["hPsiSeg"+ chambTag + labelTag + slTag] = new TH1F(("hPsiSeg_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-1 Segment #Psi; Phase-2 Primitive - Phase-1 Segment #Psi (#circ); Entries",
        nbinPsiSegFW,-minPsiSegFW,minPsiSegFW); 
        m_plots["hTimeSeg"+ chambTag + labelTag + slTag] = new TH1F(("hTimeSeg_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-1 Segment time; Phase-2 Primitive - Phase-1 Segment time (ns); Entries",
        nbinTimeSegFW,-minTimeSegFW,minTimeSegFW); 
        m_plots["hPosSeg"+ chambTag + labelTag + slTag] = new TH1F(("hPosSeg_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-1 Segment Position; Phase-2 Primitive - Phase-1 Segment Position (cm); Entries",
        nbinPosSegFW,-minPosSegFW,minPosSegFW); 
        m_plots2["hTimeSegvsPos"+ chambTag + labelTag + slTag] = new TH2F(("hTimeSegvsPos_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-1 Segment time vs Phase-1 Segment Position; Phase-1 Segment Position (cm); Phase-2 Primitive - Phase-1 Segment time (ns)",
        600,-300,300,200,-100,100); 
        m_plots2["hTimeSegvsPsi"+ chambTag + labelTag + slTag] = new TH2F(("hTimeSegvsPsi_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-1 Segment time vs Phase-1 Segment #Psi; Phase-1 Segment #Psi (#circ); Phase-2 Primitive - Phase-1 Segment time (ns)",
        120,-60,60,200,-100,100); 
        m_plots2["hTimeSegvsZ"+ chambTag + labelTag + slTag] = new TH2F(("hTimeSegvsSegZ_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-1 Segment time vs Phase-1 Segment z; Phase-1 Segment z; Phase-2 Primitive - Phase-1 Segment time (ns)",
        200,-200,200,50,-50,0); 
        
        m_plots2["hPsi2Dph2Seg" + chambTag + labelTag + slTag] = new TH2F(("hPsi2Dph2Seg_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive vs Phase-2 Segment #Psi; Phase-2 Primitive #Psi (#circ); Phase-2 Segment #Psi (#circ)",
        160,-80,80,160,-80,80); 
        m_plots2["hTime2Dph2Seg"+ chambTag + labelTag + slTag] = new TH2F(("hTime2Dph2Seg_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive vs Phase-2 Segment time; Phase-2 Primitive time (ns); Phase-2 Segment time (ns)",
        400,-200,200,200,-100,100); 
        m_plots2["hPos2Dph2Seg"+ chambTag + labelTag + slTag] = new TH2F(("hPos2Dph2Seg_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive vs Phase-2 Segment Position; Phase-2 Primitive Position (cm); Phase-2 Segment Position (cm)",
        600,-300,300,600,-300,300); 
        m_plots["hPsiph2Seg"+ chambTag + labelTag + slTag] = new TH1F(("hPsiph2Seg_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-2 Segment #Psi; Phase-2 Primitive - Phase-2 Segment #Psi (#circ); Entries",
        nbinPsiph2SegFW,-minPsiph2SegFW,minPsiph2SegFW); 
        m_plots["hTimeph2Seg"+ chambTag + labelTag + slTag] = new TH1F(("hTimeph2Seg_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-2 Segment time; Phase-2 Primitive - Phase-2 Segment time (ns); Entries",
        nbinTimeph2SegFW,-minTimeph2SegFW,minTimeph2SegFW); 
        m_plots["hPosph2Seg"+ chambTag + labelTag + slTag] = new TH1F(("hPosph2Seg_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-2 Segment Position; Phase-2 Primitive - Phase-2 Segment Position (cm); Entries",
        nbinPosph2SegFW,-minPosph2SegFW,minPosph2SegFW); 
        m_plots2["hTimeph2SegvsPos"+ chambTag + labelTag + slTag] = new TH2F(("hTimeph2SegvsPos_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-2 Segment time vs Phase-2 Segment Position; Phase-2 Segment Position (cm); Phase-2 Primitive - Phase-2 Segment time (ns)",
        600,-300,300,200,-100,100); 
        m_plots2["hTimeph2SegvsPsi"+ chambTag + labelTag + slTag] = new TH2F(("hTimeph2SegvsPsi_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-2 Segment time vs Phase-2 Segment #Psi; Phase-2 Segment #Psi (#circ); Phase-2 Primitive - Phase-2 Segment time (ns)",
        120,-60,60,200,-100,100); 
        m_plots2["hTimeph2SegvsZ"+ chambTag + labelTag + slTag] = new TH2F(("hTimeph2Segvsph2SegZ_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
          "Phase-2 Primitive - Phase-2 Segment time vs Phase-2 Segment z; Phase-2 Segment z; Phase-2 Primitive - Phase-2 Segment time (ns)",
        200,-200,200,50,-50,0); 
      }
    }
    for (const auto & quTag : quTags) {
      m_plots["hBX" + chambTag + quTag] = new TH1F(("hBX_" + chambTag + "_" + quTag).c_str(),
        "Distribution of BX; BX; Entries",
      21,-10.5,10.5); 
      m_plots["hBXDif" + chambTag + quTag] = new TH1F(("hBXDif_" + chambTag + "_" + quTag).c_str(),
        "BX difference ; BX difference; Entries",
        //41,-220.5,-179.5); 
      7000,-3564,3564); 
      m_plots["hBXEmul" + chambTag + quTag] = new TH1F(("hBXEmul_" + chambTag + "_" + quTag).c_str(),
        "Distribution of BX; BX; Entries",
      21,-10.5,10.5); 
      m_plots["hBXDifEmul" + chambTag + quTag] = new TH1F(("hBXDifEmul_" + chambTag + "_" + quTag).c_str(),
        "BX difference ; BX difference; Entries",
        //41,-220.5,-179.5); 
      7000,-3564,3564); 
      m_plots["hBXfromT0" + chambTag + quTag] = new TH1F(("hBXfromT0_" + chambTag + "_" + quTag).c_str(),
        "Distribution of BX got from T0; BX; Entries",
      201,3199.5,3400.5); 
      m_plots["hChi2Emul" +  chambTag + quTag] = new TH1F(("hChi2Emul_" + chambTag + "_" + quTag).c_str(),
        "Distribution of #chi^2; #chi^2; Entries",
      10000,-0.5,99999.5); 
      m_plots["hChi2FW" +  chambTag + quTag] = new TH1F(("hChi2FW_" + chambTag + "_" + quTag).c_str(),
        "Distribution of #chi^2; #chi^2; Entries",
      10000,-0.5,99999.5); 
      m_plots["hPsiHW" + chambTag + quTag] = new TH1F(("hPsiHW_"+ chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive #Psi Distribution; Phase-2 Primitive #Psi ( #circ); Entries",
      160,-80,80); 
      m_plots["hPsiEmul" + chambTag + quTag] = new TH1F(("hPsiEmul_"+ chambTag + "_" + quTag).c_str(),
        "Phase-2 Emulator #Psi Distribution; Phase-2 Emulator #Psi ( #circ); Entries",
      160,-80,80); 
      m_plots2["hPsi2D" + chambTag + quTag] = new TH2F(("hPsi2D_"+ chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive vs Phase-2 Emulator #Psi; Phase-2 Primitive #Psi ( #circ); Phase-2 Emulator #Psi ( #circ)",
      160,-80,80,160,-80,80); 
      m_plots2["hTime2D"+ chambTag + quTag] = new TH2F(("hTime2D_" + chambTag + "_"+ quTag).c_str(),
        "Phase-2 Primitive vs Phase-2 Emulator Time; Phase-2 Primitive time (ns); Phase-2 Emulator time (ns)",
      400,-200,200,400,-200,200); 
      m_plots2["hPos2D"+ chambTag + quTag] = new TH2F(("hPos2D_" + chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive vs Phase-2 Emulator Position; Phase-2 Primitive Position (cm); Phase-2 Emulator Position (cm)",
      600,-300,300,600,-300,300); 
      m_plots["hPsi"+ chambTag + quTag] = new TH1F(("hPsi_"+ chambTag + "_" +quTag).c_str(),
        "Phase-2 Primitive - Phase-2 Emulator #Psi; Phase-2 Primitive - Phase-2 Emulator #Psi (#circ); Entries",
      nbinPsiEmuFW,-minPsiEmuFW,minPsiEmuFW); 
      m_plots["hTime"+ chambTag + quTag] = new TH1F(("hTime_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-2 Emulator time; Phase-2 Primitive - Phase-2 Emulator Time (ns); Entries",
      nbinTimeEmuFW,-minTimeEmuFW,minTimeEmuFW); 
      m_plots["hPos"+ chambTag + quTag] = new TH1F(("hPos_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-2 Emulator Position; Phase-2 Primitive - Phase-2 Emulator Position (cm); Entries",
      nbinPosEmuFW,-minPosEmuFW,minPosEmuFW); 
      m_plots2["hPsi2DSeg" + chambTag + quTag] = new TH2F(("hPsi2DSeg_" + chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Segment #Psi; Phase-2 Primitive #Psi (#circ); Phase-1 Segment #Psi (#circ)",
      160,-80,80,160,-80,80); 
      m_plots2["hTime2DSeg"+ chambTag + quTag] = new TH2F(("hTime2DSeg_"+ chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Segment time; Phase-2 Primitive time (ns); Phase-1 Segment time (ns)",
      400,-200,200,200,-100,100); 
      m_plots2["hPos2DSeg"+ chambTag + quTag] = new TH2F(("hPos2DSeg_"+ chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Segment Position; Phase-2 Primitive Position (cm); Phase-1 Segment Position (cm)",
      600,-300,300,600,-300,300); 
      m_plots2["hPsi2Dph2Seg" + chambTag + quTag] = new TH2F(("hPsi2Dph2Seg_" + chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive vs Phase-2 Segment #Psi; Phase-2 Primitive #Psi (#circ); Phase-2 Segment #Psi (#circ)",
      160,-80,80,160,-80,80); 
      m_plots2["hTime2Dph2Seg"+ chambTag + quTag] = new TH2F(("hTime2Dph2Seg_"+ chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive vs Phase-2 Segment time; Phase-2 Primitive time (ns); Phase-2 Segment time (ns)",
      400,-200,200,200,-100,100); 
      m_plots2["hPos2Dph2Seg"+ chambTag + quTag] = new TH2F(("hPos2Dph2Seg_"+ chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive vs Phase-2 Segment Position; Phase-2 Primitive Position (cm); Phase-2 Segment Position (cm)",
      600,-300,300,600,-300,300); 
      m_plots2["hPos2DTM"+ chambTag + quTag] = new TH2F(("hPos2DTM_"+ chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Primitive Position; Phase-2 Primitive Position (cm); Phase-1 Primitive Position (cm)",
      600,-300,300,600,-300,300); 
      m_plots2["hPsi2DTM" + chambTag + quTag] = new TH2F(("hPsi2DTM_" + chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Primitive #Psi; Phase-2 Primitive #Psi (#circ); Phase-1 Primitive #Psi (#circ)",
      160,-80,80,160,-80,80); 
      m_plots2["hPhi2DTM"+ chambTag + quTag] = new TH2F(("hPhi2DTM_"+ chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Primitive phi; Phase-2 Primitive phi (rad); Phase-1 Primitive phi (rad)",
      nbinPhiFW,-minPhiFW,minPhiFW,nbinPhiTM,-minPhiTM,minPhiTM); 
      m_plots2["hPhiB2DTM"+ chambTag + quTag] = new TH2F(("hPhiB2DTM_"+ chambTag + "_" + quTag).c_str(),
        "Phase-2 Primitive vs Phase-1 Primitive phiB; Phase-2 Primitive phiB (rad); Phase-1 Primitive phiB (rad)",
      nbinPhiBFW,-minPhiBFW,minPhiBFW,nbinPhiBTM,-minPhiBTM,minPhiBTM); 
      m_plots["hPsiSeg"+ chambTag + quTag] = new TH1F(("hPsiSeg_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-1 Segment #Psi; Phase-2 Primitive - Phase-1 Segment #Psi (#circ); Entries",
      nbinPsiSegFW,-minPsiSegFW,minPsiSegFW); 
      m_plots["hTimeSeg"+ chambTag + quTag] = new TH1F(("hTimeSeg_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-1 Segment time; Phase-2 Primitive - Phase-1 Segment time (ns); Entries",
      nbinTimeSegFW,-minTimeSegFW,minTimeSegFW); 
      m_plots["hTimeTMSeg"+ chambTag + quTag] = new TH1F(("hTimeTMSeg_"+ chambTag + "_"+quTag).c_str(),
        "Phase-1 Primitive - Phase-1 Segment time; Phase-1 Primitive  - Phase-1 Segment time (ns); Entries",
      nbinTimeSegFW,-minTimeSegFW,minTimeSegFW); 
      m_plots["hPosSeg"+ chambTag + quTag] = new TH1F(("hPosSeg_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-1 Segment Position; Phase-2 Primitive - Phase-1 Segment Position (cm); Entries",
      nbinPosSegFW,-minPosSegFW,minPosSegFW); 
      m_plots2["hTimeSegvsPos"+ chambTag + quTag] = new TH2F(("hTimeSegvsPos_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-1 Segment time vs Phase-1 Segment Position; Phase-1 Segment Position (cm); Phase-2 Primitive - Phase-1 Segment time (ns)",
      600,-300,300,200,-100,100); 
      m_plots2["hTimeSegvsPsi"+ chambTag + quTag] = new TH2F(("hTimeSegvsPsi_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-1 Segment time vs Phase-1 Segment #Psi; Phase-1 Segment #Psi (#circ); Phase-2 Primitive - Phase-1 Segment time (ns)",
      160,-80,80,200,-100,100); 
      m_plots2["hTimeSegvsZ"+ chambTag + quTag] = new TH2F(("hTimeSegvsSegZ_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-1 Segment time vs Phase-1 Segment z; Phase-1 Segment z; Phase-2 Primitive - Phase-1 Segment time (ns)",
      200,-200,200,50,-50,0); 
      
      m_plots["hPsiph2Seg"+ chambTag + quTag] = new TH1F(("hPsiph2Seg_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-2 Segment #Psi; Phase-2 Primitive - Phase-2 Segment #Psi (#circ); Entries",
      nbinPsiph2SegFW,-minPsiph2SegFW,minPsiph2SegFW); 
      m_plots["hTimeph2Seg"+ chambTag + quTag] = new TH1F(("hTimeph2Seg_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-2 Segment time; Phase-2 Primitive - Phase-2 Segment time (ns); Entries",
      nbinTimeph2SegFW,-minTimeph2SegFW,minTimeph2SegFW); 
      m_plots["hPosph2Seg"+ chambTag + quTag] = new TH1F(("hPosph2Seg_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-2 Segment Position; Phase-2 Primitive - Phase-2 Segment Position (cm); Entries",
      nbinPosph2SegFW,-minPosph2SegFW,minPosph2SegFW); 
      m_plots2["hTimeph2SegvsPos"+ chambTag + quTag] = new TH2F(("hTimeph2SegvsPos_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-2 Segment time vs Phase-2 Segment Position; Phase-2 Segment Position (cm); Phase-2 Primitive - Phase-2 Segment time (ns)",
      600,-300,300,200,-100,100); 
      m_plots2["hTimeph2SegvsPsi"+ chambTag + quTag] = new TH2F(("hTimeph2SegvsPsi_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-2 Segment time vs Phase-2 Segment #Psi; Phase-2 Segment #Psi (#circ); Phase-2 Primitive - Phase-2 Segment time (ns)",
      160,-80,80,200,-100,100); 
      m_plots2["hTimeph2SegvsZ"+ chambTag + quTag] = new TH2F(("hTimeph2Segvsph2SegZ_"+ chambTag + "_"+quTag).c_str(),
        "Phase-2 Primitive - Phase-2 Segment time vs Phase-2 Segment z; Phase-2 Segment z; Phase-2 Primitive - Phase-2 Segment time (ns)",
      200,-200,200,50,-50,0); 
    }
  }
  
  return;
}

void DTNtupleTPGSimAnalyzer::fill()
{
  //getTheStupidPlots();
  //return;
  
  
  std::vector<std::string> chambTags = {"MB1", "MB2", "MB3","MB4"};
  std::vector<std::string> slTags = { "SL1", "SL3"};
  std::vector<std::string> quTags = {"3h","4h","Q6","Q7","Q8","bestQ"};
  std::vector<std::string> labelTags = {"All", "Correlated", "Uncorrelated"};
  int eventoBX = event_bunchCrossing; //848 
  int offset[4];
  
  primitives primitivesHW [4];
  primitives primitivesAM [4];
  for (int i = 0; i < 4; i++) {
	  primitivesHW[i].clear();
	  primitivesAM[i].clear();
  }
  
  
  // PLOTTING THE BX OFFSETS. W.R.T OFFLINE: Offset = GoodBX - 3564 //////////////////////
  offset[0] = 0; //FIXME
  offset[1] = 0; //FIXME
  offset[2] = 0; //FIXME
  offset[3] = 0; //FIXME
  int smallestOffset = 9999; 
  int biggestOffset = -9999; 
  for (int i = 0; i < 4; i++) {
    if (offset[i] < smallestOffset) smallestOffset = offset[i];  
    if (offset[i] > biggestOffset) biggestOffset  = offset[i];  
    m_plots["offset"] -> SetBinContent(i+1, offset[i]);  
  }
  m_plots["offset"] -> GetYaxis() -> SetRangeUser(smallestOffset - 1, biggestOffset + 1);
  ////////////////////////////////////////////////////////////////////////////////////////
  
  /* HIT PLOTS */

  //cout << "Filling hit plots";  
  int numOfDigis[4];
  int numOfDigisPerSL[4][2];
  for (int i = 0; i < 4; i++) {
    numOfDigis[i] = 0;
    numOfDigisPerSL[i][0] = 0;
    numOfDigisPerSL[i][1] = 0;
  }

  for (unsigned int iHit = 0; iHit < ph2Digi_nDigis; iHit++) {
    if (ph2Digi_superLayer->at(iHit) == 2) continue;
    m_plots2["hHits" + chambTags.at(ph2Digi_station -> at(iHit) - 1)]  -> Fill(ph2Digi_wire->at(iHit) , ph2Digi_layer->at(iHit) + ( ph2Digi_superLayer->at(iHit) - 1 ) * 5 );      
    numOfDigis[ph2Digi_station -> at(iHit) - 1]++;
    numOfDigisPerSL[ph2Digi_station -> at(iHit) - 1][ph2Digi_superLayer->at(iHit) / 2]++;
  } 
  
  for (int i = 0; i < 4; i++) {
    m_plots["hHitsPerChamber"+chambTags.at(i)] -> Fill (numOfDigis[i]);
    m_plots["hHitsPerChamber"+chambTags.at(i)+"SL1"] -> Fill (numOfDigisPerSL[i][0]);
    m_plots["hHitsPerChamber"+chambTags.at(i)+"SL3"] -> Fill (numOfDigisPerSL[i][1]);
  }
 
  //cout << " -> Finished" << endl;

  bool titPrint = false; 
  
  m_plots["hBXtotal"]->Fill(eventoBX);
  
  int bestI[6]; short bestQu[6];
  for (int i = 0; i<6; i++) {
    bestI[i] = -1; 
    bestQu[i] = -1; 
  }   
  
  //bool debug = false; 
  if (debug && ph2TpgPhiHw_nTrigs!=0 && !titPrint) { 
    cout << "====================Entry " << entryNumber << " =================="<< endl; 
    cout << "####################### L1A BX = " << eventoBX << " ############################" << endl;  
    titPrint = true; 
  }
  if (debug && ph2TpgPhiHw_nTrigs!=0) cout << "####################### HARDWARE PRIMITIVES ############################" << endl;  
  int bestTrigHW[chambTags.size()][quTags.size()];  
  int bestTimeHW[chambTags.size()][quTags.size()];
  
  for (unsigned int i = 0; i<chambTags.size(); i++){
    for (unsigned int j = 0; j<quTags.size(); j++){
      bestTrigHW[i][j] = -1; 
      bestTimeHW[i][j] = 999999;
    }
  }
  int bestQualTrigHW[4];
  int IbestQualTrigHW[4];
  int bestQualTrigBXHW[4];
  int IbestQualTrigBXHW[4];
  for (unsigned indstat = 0; indstat < 4; indstat++){
    bestQualTrigHW[indstat] = -1;
    IbestQualTrigHW[indstat] = -1;
    bestQualTrigBXHW[indstat] = -1;
    IbestQualTrigBXHW[indstat] = -1;
  }

  int number_of_prims[4], min_bx[4], max_bx[4];
  for (unsigned int i = 0; i < 4; i++){
    number_of_prims[i] = 0;
    min_bx[i] = 9999;
    max_bx[i] = -9999;
  }

  //cout << "Filling HW prims";  
  for (std::size_t iTrigHW = 0; iTrigHW < ph2TpgPhiHw_nTrigs; ++iTrigHW) {
    
    short myStationHW = ph2TpgPhiHw_station->at(iTrigHW);
    short mySectorHW = ph2TpgPhiHw_sector->at(iTrigHW);
    short myWheelHW = ph2TpgPhiHw_wheel->at(iTrigHW);
    short myQualityHW = ph2TpgPhiHw_quality->at(iTrigHW);
    short mySLHW = ph2TpgPhiHw_superLayer->at(iTrigHW);
    int myChiHW =  ph2TpgPhiHw_chi2->at(iTrigHW);
    int myPhiHW = ph2TpgPhiHw_phi->at(iTrigHW);
    int myPhiBHW =  ph2TpgPhiHw_phiB->at(iTrigHW);
    float myPosHW = ph2TpgPhiHw_posLoc_x->at(iTrigHW);
    float myDirHW = ph2TpgPhiHw_dirLoc_phi->at(iTrigHW);
    int myChi2HW = ph2TpgPhiHw_chi2->at(iTrigHW); 
    int myBXHW = ph2TpgPhiHw_BX->at(iTrigHW); //eventoBX = myBXHW; 
    int myt0HW = ph2TpgPhiHw_t0->at(iTrigHW);
    //int myArrivalBXHW = ph2TpgPhiHw_arrivalBX->at(iTrigHW);  
   
    // REMOVE SL PRIMS IF COR PRIMS IS FOUND 
    if (myQualityHW <= 4){
      if (iTrigHW + 1 <  ph2TpgPhiHw_nTrigs) {
        if (myQualityHW <= 4 && ((ph2TpgPhiHw_quality->at(iTrigHW+1)>=6 ))) continue;
      }
      if (iTrigHW + 2 <  ph2TpgPhiHw_nTrigs) {
        if (myQualityHW <= 4 && ((ph2TpgPhiHw_quality->at(iTrigHW+2)>=6 ))) continue;
      }
    }	
   
    int indstat = myStationHW - 1; 	
    //m_plots["hLatenciesHW"+chambTags.at(indstat)]->Fill(myArrivalBXHW - myBXHW); 
    //m_plots2["hHitsVsLatenciesHW"+chambTags.at(indstat)]->Fill(numOfDigis[indstat],myArrivalBXHW - myBXHW); 
    //m_plots2["hPositionVsHitsHW"+chambTags.at(indstat)]->Fill(myPosHW,numOfDigis[indstat]); 
    //m_plots2["hQualityVsLatenciesHW"+chambTags.at(indstat)]->Fill(myQualityHW,myArrivalBXHW - myBXHW); 
   
    
    if(myQualityHW > bestQualTrigHW[indstat]){
      bestQualTrigHW[indstat] = myQualityHW;
      IbestQualTrigHW[indstat] = iTrigHW ;
    }
    
    if (myQualityHW >=6 ) {
      //  if (!printeado && debug) {cout << "----------------------------------------" << endl; printeado = true;}
      if (debug) cout << "Correlated HW con quality " << myQualityHW << endl;
    }
    
    //myt0HW = myt0HW - eventoBX*25 + 3564*25;	
    if (correctL1A) {
      myt0HW = myt0HW - eventoBX*25;
      myBXHW = myBXHW - eventoBX;
    }
    m_plots["hBXAll" + chambTags.at(myStationHW-1)]->Fill(eventoBX);
    if (myt0HW > 0) { 
      m_plots["hBXNextOrbit" + chambTags.at(myStationHW-1)]->Fill(eventoBX);
    }
    
    // while (myt0HW > 0) { 
    //  myt0HW = myt0HW - 3564*25; 
    //  myBXHW = myBXHW - 3564   ; 
    //}
   
    number_of_prims[indstat]++; 
    
    if (myBXHW < min_bx[indstat]) {
        min_bx[indstat] = myBXHW;
    }
    if (myBXHW > max_bx[indstat]) {
        max_bx[indstat] = myBXHW;
    }
    
    m_plots["ht0" + chambTags.at(myStationHW-1)]->Fill(myt0HW);
    
    if (myStationHW == 1 && mySLHW == 1) m_plots["hTimeOBDTMB1_phi1"] -> Fill(myt0HW - offset[myStationHW -1] * 25);
    else if (myStationHW == 1 && mySLHW == 3) m_plots["hTimeOBDTMB1_phi2"] -> Fill(myt0HW - offset[myStationHW -1] * 25);
    else if (myStationHW == 2 && mySLHW == 1) m_plots["hTimeOBDTMB2_phi1"] -> Fill(myt0HW - offset[myStationHW -1] * 25);
    else if (myStationHW == 2 && mySLHW == 3) m_plots["hTimeOBDTMB2_phi2"] -> Fill(myt0HW - offset[myStationHW -1] * 25);
    else if (myStationHW == 3 && mySLHW == 1) m_plots["hTimeOBDTMB3_phi1b"] -> Fill(myt0HW - offset[myStationHW -1] * 25);
    else if (myStationHW == 3 && mySLHW == 3) m_plots["hTimeOBDTMB3_phi2b"] -> Fill(myt0HW - offset[myStationHW -1] * 25);
    else if (myStationHW == 4 && mySLHW == 1 && myPosHW < 0) m_plots["hTimeOBDTMB4_phi1b"] -> Fill(myt0HW - offset[myStationHW -1] * 25);
    else if (myStationHW == 4 && mySLHW == 3 && myPosHW < 0) m_plots["hTimeOBDTMB4_phi2b"] -> Fill(myt0HW - offset[myStationHW -1] * 25);
    else if (myStationHW == 4 && mySLHW == 1 && myPosHW > 0) m_plots["hTimeOBDTMB4_phi3b"] -> Fill(myt0HW - offset[myStationHW -1] * 25);
    else if (myStationHW == 4 && mySLHW == 3 && myPosHW > 0) m_plots["hTimeOBDTMB4_phi4b"] -> Fill(myt0HW - offset[myStationHW -1] * 25);
    
    if(myQualityHW>bestQualTrigBXHW[indstat] && (myBXHW - offset[myStationHW - 1] == 0)){
      bestQualTrigBXHW[indstat]=myQualityHW;
      IbestQualTrigBXHW[indstat]=iTrigHW ;
    }
    //if (myt0HW < 0) myt0HW += 3564*25;	
    
    primitivesHW[myStationHW-1].push_back(primitive({myStationHW, mySectorHW, myWheelHW, myQualityHW, mySLHW, myPhiHW, myPhiBHW, myPosHW, myDirHW, myChi2HW, myBXHW, myt0HW}));
    
    m_plots["hBXDif"]->Fill(myBXHW);
    if (myQualityHW >= 8) {
      m_plots["hPrimsSegs" + chambTags.at(myStationHW-1)] -> Fill(0);
    } // cout << "Habemus primitiva" << endl;  }
    
    m_plots["hdift0" + chambTags.at(myStationHW-1)]->Fill(myt0HW - offset[myStationHW -1]*25);
    if (abs(myt0HW - offset[myStationHW-1]*25) < bestTimeHW[myStationHW-1][get_quality_group(myQualityHW)]){
      bestTrigHW[myStationHW-1][get_quality_group(myQualityHW)] = iTrigHW; 
      bestTimeHW[myStationHW-1][get_quality_group(myQualityHW)] = abs(myt0HW - offset[myStationHW -1]*25);
    }


    int index = mySLHW; 
    if (mySLHW==3) index = 2; 
    if (bestQu[index] < myQualityHW) { bestQu[index] = myQualityHW; bestI[index] = iTrigHW;}

    if (debug) {
      cout << "Wh:" << myWheelHW << " Se:" <<  mySectorHW << " St:" << myStationHW << endl;	
      cout << "Quality " << myQualityHW << endl;	
      cout << "SL " << mySLHW << endl;	
      cout << "Chi2 " << myChiHW << endl;	
      cout << "Phi " << myPhiHW << endl;	
      cout << "PhiB " << myPhiBHW << endl;	
      cout << "Position " << myPosHW << endl;	
      cout << "Direction " << myDirHW << endl;	
      cout << "BX " << myBXHW << endl;	
      cout << "t0 " << myt0HW << endl; //myt0HW = myt0HW - eventoBX + 3564*25;	
      cout << "-------------------------------------------------------------------------" << endl;
    }

    m_plots["hQualityHW"]->Fill(myQualityHW);
    m_plots["hQualityHW"+ chambTags.at(myStationHW-1)]->Fill(myQualityHW);
    m_plots2["hQualityVsBXHW"+ chambTags.at(myStationHW-1)]->Fill(myQualityHW, myBXHW-offset[myStationHW-1]);
    m_plots["hSLHW" + chambTags.at(myStationHW-1)]->Fill(mySLHW);

    std::vector <std::string> categories = {};
    categories.push_back(labelTags.at(0));
    if (myQualityHW == 6 || myQualityHW == 7 || myQualityHW == 8) {	
      categories.push_back(labelTags.at(1));
    } else {
      categories.push_back(labelTags.at(2));
      categories.push_back(labelTags.at(2)+slTags.at(mySLHW / 2));
    }
    categories.push_back(quTags.at(get_quality_group(myQualityHW)));

    for (auto & category : categories){
      m_plots["hPsiHW" + chambTags.at(myStationHW-1) + category]->Fill(myDirHW);
      m_plots["hBX" + chambTags.at(myStationHW-1) + category]->Fill(myBXHW - offset[myStationHW - 1]);
      m_plots["hBXDif" + chambTags.at(myStationHW-1) + category]->Fill(myBXHW);
      m_plots["hBXfromT0" + chambTags.at(myStationHW-1) + category]->Fill(round(myt0HW / 25));
      m_plots["hChi2FW" + chambTags.at(myStationHW-1) + category]->Fill(1.E6*myChi2HW / (1024. * 100) );
    } 
  } // end HW
  for (unsigned int i = 0; i < 4; i++){
    if (number_of_prims[i] > 0)
      m_plots["hMultiplicityHW" + chambTags.at(i)]->Fill(number_of_prims[i]);
    if (max_bx[i] > -9999)
      m_plots["hBXWindowHW" + chambTags.at(i)]->Fill(max_bx[i] - min_bx[i]);
  }
  //cout << " -> Finished" << endl;  

  //cout << "Filling AM prims";  

  if (debug && ph2TpgPhiEmuAm_nTrigs!=0 && !titPrint) { 
    std::cout << "====================Entry " << entryNumber << " =================="<< std::endl; 
    cout << "####################### L1A BX = " << eventoBX << " ############################" << endl;  
    titPrint = true; 
  }

  if (debug && ph2TpgPhiEmuAm_nTrigs!=0) cout << "####################### Phase-2 Emulator PRIMITIVES ############################" << endl;  
  int bestTrigAM[chambTags.size()][quTags.size()];  
  int bestTimeAM[chambTags.size()][quTags.size()];  

  for (unsigned int i = 0; i<chambTags.size(); i++){
    for (unsigned int j = 0; j<quTags.size(); j++){
      bestTrigAM[i][j] = -1; 
      bestTimeAM[i][j] = 999999;
    }
  }

  int bestQualTrigAM[4];int IbestQualTrigAM[4] ; // 4 stations MB1 to MB4 
  int bestQualTrigBXAM[4];int IbestQualTrigBXAM[4] ; // 4 stations MB1 to MB4 
  for(unsigned indstat=0;indstat<4; indstat++){
    bestQualTrigAM[indstat]=-1;IbestQualTrigAM[indstat]=-1;
    bestQualTrigBXAM[indstat]=-1;IbestQualTrigBXAM[indstat]=-1;
  }

  for (std::size_t iTrigAM = 0; iTrigAM < ph2TpgPhiEmuAm_nTrigs; ++iTrigAM) {
		
    
    short myStationAM = ph2TpgPhiEmuAm_station->at(iTrigAM);
    short mySectorAM = ph2TpgPhiEmuAm_sector->at(iTrigAM);
    short myWheelAM = ph2TpgPhiEmuAm_wheel->at(iTrigAM);
    short myQualityAM = ph2TpgPhiEmuAm_quality->at(iTrigAM);
    short mySLAM = ph2TpgPhiEmuAm_superLayer->at(iTrigAM);
    int myPhiAM = ph2TpgPhiEmuAm_phi->at(iTrigAM);
    int myPhiBAM =   ph2TpgPhiEmuAm_phiB->at(iTrigAM);
    int myChi2AM =  ph2TpgPhiEmuAm_chi2->at(iTrigAM);
    float myPosAM =  ph2TpgPhiEmuAm_posLoc_x->at(iTrigAM);
    float myDirAM =  ph2TpgPhiEmuAm_dirLoc_phi->at(iTrigAM);
    int myBXAM = ph2TpgPhiEmuAm_BX->at(iTrigAM);
    int myt0AM = ph2TpgPhiEmuAm_t0->at(iTrigAM);//  myt0AM = myt0AM - eventoBX*25 + 3564*25;	

    int indstat;
    indstat=myStationAM-1;
    cout << "iTrigAM " << iTrigAM << ", myQualityAM " << myQualityAM << ", bestQualTrigAM[indstat]" << bestQualTrigAM[indstat] << endl;
    if(myQualityAM>bestQualTrigAM[indstat]){
      bestQualTrigAM[indstat]=myQualityAM;
      IbestQualTrigAM[indstat]=iTrigAM ;
    }

    if (correctL1A) {
      myt0AM = myt0AM - eventoBX*25;
      myBXAM = myBXAM - eventoBX   ;
    }

    primitivesAM[myStationAM-1].push_back(primitive({myStationAM, mySectorAM, myWheelAM, myQualityAM, mySLAM, myPhiAM, myPhiBAM, myPosAM, myDirAM, myChi2AM, myBXAM, myt0AM}));

    if(myQualityAM>bestQualTrigBXAM[indstat] && (myBXAM - offset[myStationAM -1] == 0)){
      bestQualTrigBXAM[indstat]=myQualityAM;
      IbestQualTrigBXAM[indstat]=iTrigAM ;
    }

    if (myQualityAM >=6 ) {
      if (debug) cout << "Correlated AM con quality " << myQualityAM << endl;
    }

    if (myQualityAM >= 8){ m_plots["hPrimsSegs" + chambTags.at(myStationAM-1)] -> Fill(1); } // cout << "Habemus primitiva" << endl;  }

    if (abs(myt0AM - offset[myStationAM - 1]*25) < bestTimeAM[myStationAM-1][get_quality_group(myQualityAM)]){
      bestTrigAM[myStationAM-1][get_quality_group(myQualityAM)] = iTrigAM; 
      bestTimeAM[myStationAM-1][get_quality_group(myQualityAM)] = abs(myt0AM - offset[myStationAM - 1]*25);
    }


    int index = mySLAM+3; 
    if (mySLAM==3) index = 5; 
    //if (debug) cout << "Index AM " << index <<  endl; 
    if (bestQu[index] < myQualityAM) { bestQu[index] = myQualityAM; bestI[index] = iTrigAM;}
    //if (debug) cout << "bestQu[index] " << bestQu[index] <<  endl; 

    if (debug) {
      cout << "Wh:" << myWheelAM << " Se:" <<  mySectorAM << " St:" << myStationAM << endl;	
      cout << "Quality " << myQualityAM << endl;	
      cout << "SL " << mySLAM << endl;	
      cout << "Chi2 " << myChi2AM << endl;	
      cout << "Phi " << myPhiAM << endl;	
      cout << "PhiB " << myPhiBAM << endl;	
      cout << "Position " << myPosAM << endl;	
      cout << "Direction " << myDirAM << endl;	
      cout << "BX " << myBXAM << endl;	
      cout << "t0 " << myt0AM << endl;	
      cout << "-------------------------------------------------------------------------" << endl;
    }

    m_plots["hQualityAM"]->Fill(myQualityAM);
    m_plots["hQualityAM"+ chambTags.at(myStationAM-1)]->Fill(myQualityAM);
    m_plots2["hQualityVsBXAM"+ chambTags.at(myStationAM-1)]->Fill(myQualityAM, myBXAM-offset[myStationAM-1]);
    m_plots["hSLAM" + chambTags.at(myStationAM-1)]->Fill(mySLAM);

    std::vector <std::string> categories = {};
    categories.push_back(labelTags.at(0));
    if (myQualityAM == 6 || myQualityAM == 7 || myQualityAM == 8){	
      categories.push_back(labelTags.at(1));
    } else {
      categories.push_back(labelTags.at(2));
      categories.push_back(labelTags.at(2)+slTags.at(mySLAM/2));
    }
    categories.push_back(quTags.at(get_quality_group(myQualityAM)));

    for (auto & category : categories){
      m_plots["hPsiEmul"+chambTags.at(myStationAM-1)+category]->Fill(myDirAM);
      m_plots["hBXEmul"+chambTags.at(myStationAM-1)+category]->Fill(myBXAM - offset[myStationAM-1]);
      m_plots["hBXDifEmul"+chambTags.at(myStationAM-1)+category]->Fill(myBXAM);
      m_plots["hChi2Emul"+chambTags.at(myStationAM-1)+category]->Fill(myChi2AM);
    }
  } // end AM
  //cout << " -> Finished" << endl;  

  /************************************************************************************************************************************************************
   *                                                OBTAINING FW - EMULATOR RESOLUIONS
  ************************************************************************************************************************************************************/


  //cout << "FW-Emu resolutions";
  for (unsigned int i = 0; i<chambTags.size(); i++){
    for (unsigned int j = 0; j<quTags.size(); j++){
      if (bestTrigHW[i][j] != -1 && bestTrigAM[i][j] != -1){
        int myt0HW = ph2TpgPhiHw_t0->at(bestTrigHW[i][j]); //myt0HW = myt0HW - eventoBX*25 + 3564*25;	
        if (correctL1A) myt0HW = myt0HW - eventoBX*25;
        int myt0AM = ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]); // myt0AM = myt0AM - eventoBX*25 + 3564*25;	
        if (correctL1A) myt0AM = myt0AM - eventoBX*25;

        std::vector <std::string> categories = {};
        categories.push_back(labelTags.at(0));
        if (j == 2 || j == 3 || j == 4){	
          categories.push_back(labelTags.at(1));
        } else {
          categories.push_back(labelTags.at(2));

          short myHWSL = ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]);	
          short myAMSL = ph2TpgPhiEmuAm_superLayer->at(bestTrigAM[i][j]);	
          if ( myHWSL != myAMSL ) continue; 
          categories.push_back(labelTags.at(2)+slTags.at(myHWSL/2));
        }
        categories.push_back(quTags.at(j));

        for (auto & category : categories){
          m_plots2["hPsi2D" +chambTags.at(i)+category]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
          m_plots2["hTime2D"+chambTags.at(i)+category]->Fill(myt0HW - offset[i]*25, myt0AM - offset[i]*25);
          m_plots2["hPos2D" +chambTags.at(i)+category]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));
          m_plots ["hPsi"   +chambTags.at(i)+category]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
          m_plots ["hTime"  +chambTags.at(i)+category]->Fill(myt0HW - offset[i]*25 - (myt0AM - offset[i]*25));
          m_plots ["hPos"   +chambTags.at(i)+category]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));  
        }
      }
    }
  }
  //cout << " -> Finished" << endl;  

  /************************************************************************************************************************************************************
  ************************************************************************************************************************************************************/



  //cout << "Fill TM plots" << endl;
  /************************************************************************************************************************************************************
   *                                                                  TWINMUX
  ************************************************************************************************************************************************************/
  bool printTwin = false; 
  
  int bestQualTrigTM[4];int IbestQualTrigTM[4] ; // 4 stations MB1 to MB4 
  int bestQualTrigBXTM[4];int IbestQualTrigBXTM[4] ; // 4 stations MB1 to MB4 
  for(unsigned indstat=0;indstat<4; indstat++){
    bestQualTrigTM[indstat]=-1;IbestQualTrigTM[indstat]=-1;
    bestQualTrigBXTM[indstat]=-1;IbestQualTrigBXTM[indstat]=-1;
  }

  bool HHMB3goodBX = false; 

  for (std::size_t iTwin = 0; iTwin <  ltTwinMuxOut_nTrigs; ++iTwin) {
    //break; 
    short myStationTwin = ltTwinMuxOut_station->at(iTwin);
    short mySectorTwin = ltTwinMuxOut_sector->at(iTwin);
    short myWheelTwin = ltTwinMuxOut_wheel->at(iTwin);
    short myQualityTwin = ltTwinMuxOut_quality->at(iTwin);
    int myPhiTwin = ltTwinMuxOut_phi->at(iTwin);
    int myPhiBTwin =   ltTwinMuxOut_phiB->at(iTwin);
    float myPosTwin =  ltTwinMuxOut_posLoc_x->at(iTwin);
    float myDirTwin =  ltTwinMuxOut_dirLoc_phi->at(iTwin);
    int myBXTwin = ltTwinMuxOut_BX->at(iTwin);

    if ( myBXTwin == 1 && myQualityTwin == 6 && myStationTwin == 3 && mySectorTwin == 12 && myWheelTwin == 2 ) HHMB3goodBX = true; 

    if (myQualityTwin >= 5 && myWheelTwin == 2 && mySectorTwin == 12 && myStationTwin == 2){ m_plots["hPrimsSegs" + chambTags.at(myStationTwin/2-1)] -> Fill(3); } // cout << "Habemus primitiva" << endl; 
    if (myQualityTwin >= 5 && myWheelTwin == 2 && mySectorTwin == 12 && myStationTwin == 4){ m_plots["hPrimsSegs" + chambTags.at(myStationTwin/2-1)] -> Fill(3); } // cout << "Habemus primitiva" << endl;  
    if (myWheelTwin == 2 && mySectorTwin == 12) {
      if (debug && !titPrint) { 
        std::cout << "====================Entry " << entryNumber << " =================="<< std::endl; 
        cout << "####################### L1A BX = " << eventoBX << " ############################" << endl;  
        titPrint = true; 
      }
      if (debug && !printTwin) { 
        cout << "####################### TwimMux PRIMITIVES ############################" << endl;
        printTwin = true;
      }
      if (debug) {
        cout << "Wh:" << myWheelTwin << " Se:" <<  mySectorTwin << " St:" << myStationTwin << endl;	
        cout << "Quality " << myQualityTwin << endl;	
        cout << "Phi " << myPhiTwin << endl;	
        cout << "PhiB " << myPhiBTwin << endl;	
        cout << "Position " << myPosTwin << endl;	
        cout << "Direction " << myDirTwin << endl;	
        cout << "BX " << myBXTwin << endl;	
        cout << "-------------------------------------------------------------------------" << endl;
      }	
    }
    int indstat = myStationTwin - 1;  
    if (myWheelTwin != 2 || mySectorTwin != 12) continue; 
    //cout << "Before filling qualities" << endl;
    if(myQualityTwin>bestQualTrigTM[indstat]){
      bestQualTrigTM[indstat]=myQualityTwin;
      IbestQualTrigTM[indstat]=iTwin;
    }
    if(myQualityTwin>bestQualTrigBXTM[indstat] && myBXTwin == 1){
      bestQualTrigBXTM[indstat]=myQualityTwin;
      IbestQualTrigBXTM[indstat]=iTwin;
    }

    m_plots["hBXTM"+ chambTags.at(indstat)] -> Fill(myBXTwin);
    m_plots["hQualityTM"+ chambTags.at(indstat)] -> Fill(myQualityTwin);
    // FILL TM PLOTS

    if (bestTrigHW[myStationTwin-1][myQualityTwin-2] != -1){

      int i = myStationTwin-1; 
      int j = myQualityTwin-2; 

      short myHwWheel = ph2TpgPhiHw_wheel->at(bestTrigHW[i][j]);
      short myHwSector = ph2TpgPhiHw_sector->at(bestTrigHW[i][j]);
      short myHwStation = ph2TpgPhiHw_station->at(bestTrigHW[i][j]);
      short myHwSuperlayer = ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]);
      float myPosHW =  ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]);
      float myDirHW =  ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]);

      std::vector <std::string> categories = {};
      categories.push_back(labelTags.at(0));
      if (j == 2 || j == 3 || j == 4){	
        categories.push_back(labelTags.at(1));
      } else {
        categories.push_back(labelTags.at(2));
        categories.push_back(labelTags.at(2)+slTags.at(myHwSuperlayer/2));
      }
      categories.push_back(quTags.at(j));

      for (auto & category : categories){
        m_plots2["hPsi2DTM" +chambTags.at(i)+category]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),myDirTwin);
        m_plots2["hPos2DTM" +chambTags.at(i)+category]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),myPosTwin);
        m_plots2["hPhi2DTM" +chambTags.at(i)+category]->Fill((0.8 / 65536.) * ph2TpgPhiHw_phi->at(bestTrigHW[i][j]), (1. / 4096.) * myPhiTwin);
        m_plots2["hPhiB2DTM"+chambTags.at(i)+category]->Fill((1.4 / 2048.) * ph2TpgPhiHw_phiB->at(bestTrigHW[i][j]), (1 / 512.) * myPhiBTwin);
      }
    }
  }
  /************************************************************************************************************************************************************
  ************************************************************************************************************************************************************/



  /***********************************************************************************************************************************************
    *
    * 							  FIRMWARE - EMULATOR
    * 
  ************************************************************************************************************************************************/

  int counterHW [4][2]; 
  int counterAM [4][2]; 
  for (int i = 0; i < 4; i++) {
    bool corHW = false, corAM = false, SL1HW = false, SL1AM = false, SL3HW = false, SL3AM = false; 
    for (int j = 0; j < 2; j++) {
      counterHW[i][j] = 0; 
      counterAM[i][j] = 0; 
    }
    for (auto & primitiveHW : primitivesHW[i]) {
      counterHW[i][0]++; 
      if (get_quality_group (primitiveHW.quality) > 1) {
        counterHW[i][1]++;
        corHW = true; 
        m_plots2["hPrimTypeVsPos"+ chambTags.at(i)]->Fill(primitiveHW.position, 0.);
        } else {
        if (primitiveHW.superLayer == 1){
          SL1HW = true;
          m_plots2["hPrimTypeVsPos"+ chambTags.at(i)]->Fill(primitiveHW.position, 1.);
          } else if (primitiveHW.superLayer == 3) {
          SL3HW = true;
          m_plots2["hPrimTypeVsPos"+ chambTags.at(i)]->Fill(primitiveHW.position, 2.);
        } 
      } 
    }
    for (auto & primitiveAM: primitivesAM[i]) {
      counterAM[i][0]++;
      if (get_quality_group(primitiveAM.quality) > 1){ 
        counterAM[i][1]++;
        corAM = true; 
      } else {
        if (primitiveAM.superLayer == 1) SL1AM = true; 
        else if (primitiveAM.superLayer == 3) SL3AM = true; 
      }
    }
    if (counterHW[i][0] != 0 || counterAM[i][0] != 0)
      m_plots2["hPrimNumber" + chambTags.at(i)] -> Fill(counterHW[i][0], counterAM[i][0]);	
    if (counterHW[i][1] != 0 || counterAM[i][1] != 0)
      m_plots2["hCorPrimNumber" + chambTags.at(i)] -> Fill(counterHW[i][1], counterAM[i][1]);	
    
    if (corHW) {
      m_plots2["hHWQuality" + chambTags.at(i)]->Fill(0., entryNumber);
      if (corAM) { 
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(0., entryNumber);
        m_plots2["hAMQualityWhenHWcor"+chambTags.at(i)]->Fill(0., entryNumber);
        m_plots2["hHWQualityWhenAMcor"+chambTags.at(i)]->Fill(0., entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(0., 0.);
        } else if (SL1AM && SL3AM){
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(1, entryNumber);
        m_plots2["hAMQualityWhenHWcor"+chambTags.at(i)]->Fill(1, entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(0., 1);
        } else if (SL3AM && !SL3AM){
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(2, entryNumber);
        m_plots2["hAMQualityWhenHWcor"+chambTags.at(i)]->Fill(2, entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(0., 2);
        } else if (SL3AM && !SL1AM){
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(3, entryNumber);
        m_plots2["hAMQualityWhenHWcor"+chambTags.at(i)]->Fill(3, entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(0., 3);
        } else {
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(4, entryNumber);
        m_plots2["hAMQualityWhenHWcor"+chambTags.at(i)]->Fill(4, entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(0., 4);
      } 
      } else if (SL1HW && SL3HW) {
      m_plots2["hHWQuality"+chambTags.at(i)]->Fill(1, entryNumber);
      if (corAM) { 
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(0., entryNumber);
        m_plots2["hHWQualityWhenAMcor"+chambTags.at(i)]->Fill(1., entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(1, 0.);
        } else if (SL1AM && SL3AM){
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(1, entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(1, 1);
        } else if (SL1AM && !SL3AM){
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(2, entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(1, 2);
        } else if (SL3AM && !SL1AM){
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(3,entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(1, 3);
        } else {
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(4,entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(1, 4);
      }
      } else if (SL1HW && !SL3HW) {
      m_plots2["hHWQuality"+chambTags.at(i)]->Fill(2, entryNumber);
      if (corAM) { 
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(0., entryNumber);
        m_plots2["hHWQualityWhenAMcor"+chambTags.at(i)]->Fill(2., entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(2, 0.);
        } else if (SL1AM && SL3AM){
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(1, entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(2, 1);
        } else if (SL1AM && !SL3AM){
	      m_plots2["hAMQuality"+chambTags.at(i)]->Fill(1, entryNumber);
	      m_plots2["hEventQuality"+chambTags.at(i)]->Fill(2, 2);
        } else if (SL3AM && !SL1AM){
	      m_plots2["hAMQuality"+chambTags.at(i)]->Fill(3, entryNumber);
	      m_plots2["hEventQuality"+chambTags.at(i)]->Fill(2, 3);
        } else {
	      m_plots2["hAMQuality"+chambTags.at(i)]->Fill(4, entryNumber);
	      m_plots2["hEventQuality"+chambTags.at(i)]->Fill(2, 4);
      }
      } else if (SL3HW && !SL1HW) {
	    if (corAM) { 
	      m_plots2["hAMQuality"+chambTags.at(i)]->Fill(0., entryNumber);
	      m_plots2["hHWQualityWhenAMcor"+chambTags.at(i)]->Fill(3., entryNumber);
	      m_plots2["hEventQuality"+chambTags.at(i)]->Fill(3, 0.);
        } else if (SL1AM && SL3AM){
	      m_plots2["hAMQuality"+chambTags.at(i)]->Fill(1, entryNumber);
	      m_plots2["hEventQuality"+chambTags.at(i)]->Fill(3, 1);
        } else if (SL1AM && !SL3AM){
	      m_plots2["hAMQuality"+chambTags.at(i)]->Fill(2, entryNumber);
	      m_plots2["hEventQuality"+chambTags.at(i)]->Fill(3, 2);
        } else if (SL3AM && !SL1AM){
	      m_plots2["hAMQuality"+chambTags.at(i)]->Fill(3, entryNumber);
	      m_plots2["hEventQuality"+chambTags.at(i)]->Fill(3, 3);
        } else {
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(4, entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(3, 4);
      } 
      } else if (corAM || SL1AM || SL3AM ){ 
      m_plots2["hHWQuality"+chambTags.at(i)]->Fill(4, entryNumber);
      if (corAM) { 
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(0., entryNumber);
        m_plots2["hHWQualityWhenAMcor"+chambTags.at(i)]->Fill(4., entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(4, 0.);
        } else if (SL1AM && SL3AM){
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(1, entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(4, 1);
        } else if (SL1AM && !SL3AM){
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(2, entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(4, 2);
        } else if (SL3AM && !SL1AM){
        m_plots2["hAMQuality"+chambTags.at(i)]->Fill(3, entryNumber);
        m_plots2["hEventQuality"+chambTags.at(i)]->Fill(4, 3);
      }  
    }
    
    
  }   
 
  //cout << " -> Finished" << endl;
  
  /***********************************************************************************************************************************************
    *
    * 							COMPARING WITH SEGMENTS
    * 
  ************************************************************************************************************************************************/
  
  bool entro = false; 
  for (std::size_t iSeg = 0; iSeg < seg_nSegments; ++iSeg) {
    float mySegt0 = seg_phi_t0->at(iSeg);
    short mySegStation = seg_station->at(iSeg);
    short mySegWheel = seg_wheel->at(iSeg);
    short mySegSector = seg_sector->at(iSeg);
    float mySegPos = seg_posLoc_x->at(iSeg);
    float mySegPosSL1 = seg_posLoc_x_SL1->at(iSeg);
    float mySegPosSL3 = seg_posLoc_x_SL3->at(iSeg);
    float mySegPosMid = seg_posLoc_x_midPlane->at(iSeg);
    float mySegZ = seg_posLoc_y->at(iSeg);
    short mySegHasZ = seg_hasZed->at(iSeg);
    float mySegPsi = 360 * TMath::ATan ( ( seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) / (2*TMath::Pi());

    // segment cleaning
    if (mySegt0 < -500 || fabs(mySegPsi) > 30 || seg_phi_nHits->at(iSeg) < 4)
      continue;

    if (mySegSector == 12 && mySegWheel == 2 && seg_phi_nHits->at(iSeg) == 8){ m_plots["hPrimsSegs" + chambTags.at(mySegStation - 1)] -> Fill(2);
      entro = true; 
      if (debug) cout <<"Segment in Wh:" << mySegWheel << " Se:" << mySegSector << " St:" << mySegStation << " Hits:" << seg_phi_nHits ->at(iSeg) << endl;
    }

    if(mySegWheel==2 && mySegSector==12) {
      int indstat = mySegStation-1;
      m_plots2["h2DHwQualSegNHits"+chambTags.at(indstat)]->Fill(seg_phi_nHits->at(iSeg), get_quality_group(bestQualTrigHW[indstat]));
      m_plots2["h2DEmuQualSegNHits"+chambTags.at(indstat)]->Fill(seg_phi_nHits->at(iSeg), get_quality_group(bestQualTrigAM[indstat]));
      m_plots2["h2DTMQualSegNHits"+chambTags.at(indstat)]->Fill(seg_phi_nHits->at(iSeg), get_tm_quality_group(bestQualTrigTM[indstat]));

      auto chambTag = chambTags.at(indstat);
      if (bestQualTrigHW[indstat] >= 6) m_plots["hHwHighQualSegNHits" + chambTag]->Fill(seg_phi_nHits->at(iSeg));
      else m_plots["hHwLowQualSegNHits" + chambTag]->Fill(seg_phi_nHits->at(iSeg));
      if (bestQualTrigAM[indstat] >= 6) m_plots["hEmuHighQualSegNHits" + chambTag]->Fill(seg_phi_nHits->at(iSeg));
      else m_plots["hEmuLowQualSegNHits" + chambTag]->Fill(seg_phi_nHits->at(iSeg));
      if (bestQualTrigTM[indstat] >= 4) m_plots["hTMHighQualSegNHits" + chambTag]->Fill(seg_phi_nHits->at(iSeg));
      else m_plots["hTMLowQualSegNHits" + chambTag]->Fill(seg_phi_nHits->at(iSeg));
      
      if (IbestQualTrigHW[indstat] != -1) m_plots["hEffHWvsSegX"      + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (bestQualTrigHW[indstat] >= 6)   m_plots["hEffHWvsSegXCorr"  + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (bestQualTrigHW[indstat] >= 3)   m_plots["hEffHWvsSegXQ>2"   + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (IbestQualTrigHW[indstat] != -1) m_plots["hEffHWvsSegT0"     + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      if (bestQualTrigHW[indstat] >= 6)   m_plots["hEffHWvsSegT0Corr" + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      if (bestQualTrigHW[indstat] >= 3)   m_plots["hEffHWvsSegT0Q>2"  + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      m_plots["hEffHWvsSegX"      + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffHWvsSegXCorr"  + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffHWvsSegXQ>2"   + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffHWvsSegT0"     + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      m_plots["hEffHWvsSegT0Corr" + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      m_plots["hEffHWvsSegT0Q>2"  + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      
      if (IbestQualTrigBXHW[indstat] != -1) m_plots["hEffHWvsSegXGoodBX"      + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (bestQualTrigBXHW[indstat] >= 6)   m_plots["hEffHWvsSegXGoodBXCorr"  + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (bestQualTrigBXHW[indstat] >= 3)   m_plots["hEffHWvsSegXGoodBXQ>2"   + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (IbestQualTrigBXHW[indstat] != -1) m_plots["hEffHWvsSegT0GoodBX"     + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      if (bestQualTrigBXHW[indstat] >= 6)   m_plots["hEffHWvsSegT0GoodBXCorr" + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      if (bestQualTrigBXHW[indstat] >= 3)   m_plots["hEffHWvsSegT0GoodBXQ>2"  + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      m_plots["hEffHWvsSegXGoodBX"       + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffHWvsSegXGoodBXCorr"   + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffHWvsSegXGoodBXQ>2"    + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffHWvsSegT0GoodBX"      + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      m_plots["hEffHWvsSegT0GoodBXCorr"  + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      m_plots["hEffHWvsSegT0GoodBXQ>2"   + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      
      
      if (IbestQualTrigAM[indstat] != -1)   m_plots["hEffAMvsSegX"           + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (bestQualTrigAM[indstat] >= 6)     m_plots["hEffAMvsSegXCorr"       + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (bestQualTrigAM[indstat] >= 3)     m_plots["hEffAMvsSegXQ>2"        + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (IbestQualTrigBXAM[indstat] != -1) m_plots["hEffAMvsSegXGoodBX"     + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (bestQualTrigBXAM[indstat] >= 6)   m_plots["hEffAMvsSegXGoodBXCorr" + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (bestQualTrigBXAM[indstat] >= 3)   m_plots["hEffAMvsSegXGoodBXQ>2"  + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      m_plots["hEffAMvsSegX"           + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffAMvsSegXCorr"       + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffAMvsSegXQ>2"        + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffAMvsSegXGoodBX"     + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffAMvsSegXGoodBXCorr" + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffAMvsSegXGoodBXQ>2"  + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      
      if (IbestQualTrigAM[indstat] != -1)   m_plots["hEffAMvsSegT0"           + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      if (bestQualTrigAM[indstat] >= 6)     m_plots["hEffAMvsSegT0Corr"       + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      if (bestQualTrigAM[indstat] >= 3)     m_plots["hEffAMvsSegT0Q>2"        + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      if (IbestQualTrigBXAM[indstat] != -1) m_plots["hEffAMvsSegT0GoodBX"     + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      if (bestQualTrigBXAM[indstat] >= 6)   m_plots["hEffAMvsSegT0GoodBXCorr" + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      if (bestQualTrigBXAM[indstat] >= 3)   m_plots["hEffAMvsSegT0GoodBXQ>2"  + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      m_plots["hEffAMvsSegT0"           + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      m_plots["hEffAMvsSegT0Corr"       + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      m_plots["hEffAMvsSegT0Q>2"        + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      m_plots["hEffAMvsSegT0GoodBX"     + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      m_plots["hEffAMvsSegT0GoodBXCorr" + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      m_plots["hEffAMvsSegT0GoodBXQ>2"  + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      
      if ( indstat + 1 == 4  && bestQualTrigTM[indstat] > 3 && IbestQualTrigHW[indstat]==-1 ) { 
        //DisplayPh2Hits(); 
        //DisplayPh2Prims(); 
        //DisplayPh1Segs(); 
        //DisplayPh1Prims(); 
      }
      
      if (IbestQualTrigTM[indstat]!=-1)   m_plots["hEffTMvsSegX"            + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (IbestQualTrigBXTM[indstat]!=-1) m_plots["hEffTMvsSegXGoodBX"      + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (IbestQualTrigTM[indstat]!=-1)   m_plots["hEffTMvsSegT0"           + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      if (IbestQualTrigBXTM[indstat]!=-1) m_plots["hEffTMvsSegT0GoodBX"     + chambTags.at(indstat) + "passed"] -> Fill(mySegt0); 
      if (bestQualTrigTM[indstat]>=4)     m_plots["hEffTMvsSegXCorr"        + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (bestQualTrigBXTM[indstat]>=4)   m_plots["hEffTMvsSegXGoodBXCorr"  + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
      if (bestQualTrigTM[indstat]>=4)     m_plots["hEffTMvsSegT0Corr"       + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
      if (bestQualTrigBXTM[indstat]>=4)   m_plots["hEffTMvsSegT0GoodBXCorr" + chambTags.at(indstat) + "passed"] -> Fill(mySegt0); 
      m_plots["hEffTMvsSegX"            + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffTMvsSegXGoodBX"      + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffTMvsSegT0"           + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      m_plots["hEffTMvsSegT0GoodBX"     + chambTags.at(indstat) + "total"] -> Fill(mySegt0); 
      m_plots["hEffTMvsSegXCorr"        + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffTMvsSegXGoodBXCorr"  + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      m_plots["hEffTMvsSegT0Corr"       + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      m_plots["hEffTMvsSegT0GoodBXCorr" + chambTags.at(indstat) + "total"] -> Fill(mySegt0); 
      
      float limInfT0 = -12.5;
      float limSupT0 = 12.5;
    
      float limInfPos = -100;
      float limSupPos = 100;
    
      if (mySegt0 >= limInfT0 && mySegt0 <= limSupT0){
        if (IbestQualTrigHW[indstat] != -1) m_plots["hEffHWvsSegXLim"     + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        if (bestQualTrigHW[indstat] >= 6)   m_plots["hEffHWvsSegXLimCorr" + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        if (bestQualTrigHW[indstat] >= 3)   m_plots["hEffHWvsSegXLimQ>2"  + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        m_plots["hEffHWvsSegXLim"     + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        m_plots["hEffHWvsSegXLimCorr" + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        m_plots["hEffHWvsSegXLimQ>2"  + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        
        if (IbestQualTrigBXHW[indstat]!=-1) m_plots["hEffHWvsSegXLimGoodBX"     + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        if (bestQualTrigBXHW[indstat]>=6)   m_plots["hEffHWvsSegXLimGoodBXCorr" + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        if (bestQualTrigBXHW[indstat]>=3)   m_plots["hEffHWvsSegXLimGoodBXQ>2"  + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        m_plots["hEffHWvsSegXLimGoodBX"     + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        m_plots["hEffHWvsSegXLimGoodBXCorr" + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        m_plots["hEffHWvsSegXLimGoodBXQ>2"  + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        
        
        if (IbestQualTrigAM[indstat] != -1)   m_plots["hEffAMvsSegXLim"           + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        if (bestQualTrigAM[indstat] >= 3)     m_plots["hEffAMvsSegXLimQ>2"        + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        if (bestQualTrigAM[indstat] >= 6)     m_plots["hEffAMvsSegXLimCorr"       + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        if (IbestQualTrigBXAM[indstat] != -1) m_plots["hEffAMvsSegXLimGoodBX"     + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        if (bestQualTrigBXAM[indstat] >= 3)   m_plots["hEffAMvsSegXLimGoodBXQ>2"  + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        if (bestQualTrigBXAM[indstat] >= 6)   m_plots["hEffAMvsSegXLimGoodBXCorr" + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        m_plots["hEffAMvsSegXLim"           + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        m_plots["hEffAMvsSegXLimQ>2"        + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        m_plots["hEffAMvsSegXLimCorr"       + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        m_plots["hEffAMvsSegXLimGoodBX"     + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        m_plots["hEffAMvsSegXLimGoodBXQ>2"  + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        m_plots["hEffAMvsSegXLimGoodBXCorr" + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        
        if (IbestQualTrigTM[indstat] != -1)   m_plots["hEffTMvsSegXLim"           + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        if (IbestQualTrigBXTM[indstat] != -1) m_plots["hEffTMvsSegXLimGoodBX"     + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        if (bestQualTrigTM[indstat] >= 4)     m_plots["hEffTMvsSegXLimCorr"       + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        if (bestQualTrigBXTM[indstat] >= 4)   m_plots["hEffTMvsSegXLimGoodBXCorr" + chambTags.at(indstat) + "passed"] -> Fill(mySegPos);
        m_plots["hEffTMvsSegXLim"           + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        m_plots["hEffTMvsSegXLimGoodBX"     + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        m_plots["hEffTMvsSegXLimCorr"       + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
        m_plots["hEffTMvsSegXLimGoodBXCorr" + chambTags.at(indstat) + "total"] -> Fill(mySegPos);
      }
    
      if (mySegPos >= limInfPos && mySegPos <= limSupPos){
        if (IbestQualTrigHW[indstat] != -1) m_plots["hEffHWvsSegT0Lim"     + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        if (bestQualTrigHW[indstat] >= 6)   m_plots["hEffHWvsSegT0LimCorr" + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        if (bestQualTrigHW[indstat] >= 3)   m_plots["hEffHWvsSegT0LimQ>2"  + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        m_plots["hEffHWvsSegT0Lim"     + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        m_plots["hEffHWvsSegT0LimCorr" + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        m_plots["hEffHWvsSegT0LimQ>2"  + chambTags.at(indstat) + "total"] -> Fill(mySegt0);

        if (IbestQualTrigBXHW[indstat] != -1) m_plots["hEffHWvsSegT0LimGoodBX"     + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        if (bestQualTrigBXHW[indstat] >= 6)   m_plots["hEffHWvsSegT0LimGoodBXCorr" + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        if (bestQualTrigBXHW[indstat] >= 3)   m_plots["hEffHWvsSegT0LimGoodBXQ>2"  + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        m_plots["hEffHWvsSegT0LimGoodBX"     + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        m_plots["hEffHWvsSegT0LimGoodBXCorr" + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        m_plots["hEffHWvsSegT0LimGoodBXQ>2"  + chambTags.at(indstat) + "total"] -> Fill(mySegt0);

        if (IbestQualTrigAM[indstat] != -1)   m_plots["hEffAMvsSegT0Lim"           + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        if (bestQualTrigAM[indstat] >= 3)     m_plots["hEffAMvsSegT0LimQ>2"        + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        if (bestQualTrigAM[indstat] >= 6)     m_plots["hEffAMvsSegT0LimCorr"       + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        if (IbestQualTrigBXAM[indstat] != -1) m_plots["hEffAMvsSegT0LimGoodBX"     + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        if (bestQualTrigBXAM[indstat] >= 3)   m_plots["hEffAMvsSegT0LimGoodBXQ>2"  + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        if (bestQualTrigBXAM[indstat] >= 6)   m_plots["hEffAMvsSegT0LimGoodBXCorr" + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        m_plots["hEffAMvsSegT0Lim"           + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        m_plots["hEffAMvsSegT0LimQ>2"        + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        m_plots["hEffAMvsSegT0LimCorr"       + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        m_plots["hEffAMvsSegT0LimGoodBX"     + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        m_plots["hEffAMvsSegT0LimGoodBXQ>2"  + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        m_plots["hEffAMvsSegT0LimGoodBXCorr" + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        
        if (IbestQualTrigTM[indstat] != -1)   m_plots["hEffTMvsSegT0Lim"           + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        if (IbestQualTrigBXTM[indstat] != -1) m_plots["hEffTMvsSegT0LimGoodBX"     + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        if (bestQualTrigTM[indstat] >= 4)     m_plots["hEffTMvsSegT0LimCorr"       + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        if (bestQualTrigBXTM[indstat] >= 4)   m_plots["hEffTMvsSegT0LimGoodBXCorr" + chambTags.at(indstat) + "passed"] -> Fill(mySegt0);
        m_plots["hEffTMvsSegT0Lim"           + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        m_plots["hEffTMvsSegT0LimGoodBX"     + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        m_plots["hEffTMvsSegT0LimCorr"       + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
        m_plots["hEffTMvsSegT0LimGoodBXCorr" + chambTags.at(indstat) + "total"] -> Fill(mySegt0);
      }

      for (unsigned int i = 0; i<chambTags.size(); i++){
        for (unsigned int j = 0; j<quTags.size(); j++){
          if (bestTrigHW[i][j] != -1) {
            //if (bestTrigHW[i][j] != -1 && mySegt0 > -500 && HHMB3goodBX && fabs(mySegPsi) < 30 && seg_phi_nHits->at(iSeg) >= 4 ) {
            //if (bestTrigHW[i][j] != -1 && mySegt0 > -500){
            short myHwWheel   = ph2TpgPhiHw_wheel      -> at(bestTrigHW[i][j]);
            short myHwSector  = ph2TpgPhiHw_sector     -> at(bestTrigHW[i][j]);
            short myHwStation = ph2TpgPhiHw_station    -> at(bestTrigHW[i][j]);
            float myPosHW     = ph2TpgPhiHw_posLoc_x   -> at(bestTrigHW[i][j]);
            float myDirHW     = ph2TpgPhiHw_dirLoc_phi -> at(bestTrigHW[i][j]);

            if (mySegWheel != myHwWheel || mySegSector != myHwSector || mySegStation != myHwStation ) continue;

            float mySegPos; 
            if ((j != 2 && j != 3 && j != 4 ) && (ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]) == 1)) mySegPos = mySegPosSL1;
            else if ((j != 2 && j != 3 && j != 4 ) && (ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]) == 3)) mySegPos = mySegPosSL3;
            else mySegPos = mySegPosMid; 

            int myt0HW = ph2TpgPhiHw_t0->at(bestTrigHW[i][j]); 
            if (correctL1A) myt0HW = myt0HW - eventoBX*25;

            // Fill plots
            std::vector <std::string> categories = {};
            categories.push_back(labelTags.at(0));
            if (j == 2 || j == 3 || j == 4) {
              categories.push_back(labelTags.at(1));
              } else {
              categories.push_back(labelTags.at(2));
              short myHWSL = ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]);	
              categories.push_back(labelTags.at(2) + slTags.at(myHWSL/2));
            }
            categories.push_back(quTags.at(j));

            for (auto & category : categories) { 
              m_plots2  ["hPsi2DSeg"     + chambTags.at(i) + category] -> Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]), mySegPsi );
              m_plots2  ["hTime2DSeg"    + chambTags.at(i) + category] -> Fill(myt0HW - offset[i] * 25, mySegt0                       );
              m_plots2  ["hPos2DSeg"     + chambTags.at(i) + category] -> Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]), mySegPos   );
              m_plots   ["hPsiSeg"       + chambTags.at(i) + category] -> Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - mySegPsi);
              m_plots   ["hTimeSeg"      + chambTags.at(i) + category] -> Fill(myt0HW - offset[i] * 25 - mySegt0                      );
              m_plots   ["hPosSeg"       + chambTags.at(i) + category] -> Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - mySegPos  );
              m_plots2  ["hTimeSegvsPos" + chambTags.at(i) + category] -> Fill(mySegPos, myt0HW - offset[i]*25 - mySegt0              );
              m_plots2  ["hTimeSegvsPsi" + chambTags.at(i) + category] -> Fill(mySegPsi, myt0HW - offset[i]*25 - mySegt0              );
              if (mySegHasZ > 0) 
                m_plots2["hTimeSegvsZ"   + chambTags.at(i) + category]->Fill(mySegZ,myt0HW - offset[i]*25 - mySegt0                   );
            }
          } 
        } //QU
        
        if (IbestQualTrigHW[i] != -1) {      
          short myBestHwWheel   = ph2TpgPhiHw_wheel      -> at(IbestQualTrigHW[i]);
          short myBestHwSector  = ph2TpgPhiHw_sector     -> at(IbestQualTrigHW[i]);
          short myBestHwStation = ph2TpgPhiHw_station    -> at(IbestQualTrigHW[i]);
          float myBestPosHW     = ph2TpgPhiHw_posLoc_x   -> at(IbestQualTrigHW[i]);
          float myBestDirHW     = ph2TpgPhiHw_dirLoc_phi -> at(IbestQualTrigHW[i]);
          int   myBestT0HW      = ph2TpgPhiHw_t0         -> at(IbestQualTrigHW[i]); 
          short myBestQualityHW = ph2TpgPhiHw_quality    -> at(IbestQualTrigHW[i]); 
          if (correctL1A) myBestT0HW = myBestT0HW - eventoBX*25;
          
          if (ph2TpgPhiHw_superLayer->at(IbestQualTrigHW[i]) == 1) mySegPos = mySegPosSL1;
          else if (ph2TpgPhiHw_superLayer->at(IbestQualTrigHW[i]) == 3) mySegPos = mySegPosSL3;
          else mySegPos = mySegPosMid;

          // FIXME: Position always in the center, maybe change also to SL
          m_plots2["hPsi2DSeg"    + chambTags.at(i) + "bestQ"] -> Fill(myBestDirHW, mySegPsi                           );
          m_plots2["hTime2DSeg"   + chambTags.at(i) + "bestQ"] -> Fill(myBestT0HW - offset[i] * 25, mySegt0            );
          m_plots2["hPos2DSeg"    + chambTags.at(i) + "bestQ"] -> Fill(myBestPosHW, mySegPos                           );
          m_plots ["hPsiSeg"      + chambTags.at(i) + "bestQ"] -> Fill(myBestDirHW - mySegPsi                          );
          m_plots ["hTimeSeg"     + chambTags.at(i) + "bestQ"] -> Fill(myBestT0HW - offset[i] * 25 - mySegt0           );
          m_plots ["hPosSeg"      + chambTags.at(i) + "bestQ"] -> Fill(myBestPosHW - mySegPos                          );
          m_plots2["hTimeSegvsPos"+ chambTags.at(i) + "bestQ"] -> Fill(mySegPos, myBestT0HW - offset[i] * 25 - mySegt0 );
          m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + "bestQ"] -> Fill(mySegPsi, myBestT0HW - offset[i] * 25 - mySegt0 );
          if (mySegHasZ > 0) 
          m_plots2["hTimeSegvsZ"  + chambTags.at(i) + "bestQ"] -> Fill(mySegZ, myBestT0HW - offset[i] * 25 - mySegt0   );
        }
      }	//STAT
    }
  } //for segments
  
  /*************************************************************************************************************************
    *
    *                                              PH2 SEGMENTS
    *
  * ***********************************************************************************************************************/
  
  for (std::size_t iph2Seg = 0; iph2Seg <  ph2Seg_nSegments; ++iph2Seg) {
    
    
    float myph2Segt0      = ph2Seg_phi_t0->at(iph2Seg);
    short myph2SegStation = ph2Seg_station->at(iph2Seg);
    short myph2SegWheel   = ph2Seg_wheel->at(iph2Seg);
    short myph2SegSector  = ph2Seg_sector->at(iph2Seg);
    float myph2SegPos     = ph2Seg_posLoc_x->at(iph2Seg);
    float myph2SegPosSL1  = ph2Seg_posLoc_x_SL1->at(iph2Seg);
    float myph2SegPosSL3  = ph2Seg_posLoc_x_SL3->at(iph2Seg);
    float myph2SegPosMid  = ph2Seg_posLoc_x_midPlane->at(iph2Seg);
    float myph2SegZ       = ph2Seg_posLoc_y->at(iph2Seg);
    short myph2SegHasZ    = ph2Seg_hasZed->at(iph2Seg);
    float myph2SegPsi     = 360 * TMath::ATan((ph2Seg_dirLoc_x->at(iph2Seg) / ph2Seg_dirLoc_z->at(iph2Seg))) / (2*TMath::Pi());
    
    // segment cleaning
    if (myph2Segt0 < -500 || fabs(myph2SegPsi) > 30 || ph2Seg_phi_nHits->at(iph2Seg) < 4)
      continue;
    
    if(myph2SegWheel==2 && myph2SegSector==12){
      int indstat = myph2SegStation-1;
      m_plots2["h2DHwQualph2SegNHits"+chambTags.at(indstat)]->Fill(ph2Seg_phi_nHits->at(iph2Seg), get_quality_group(bestQualTrigHW[indstat]));
      m_plots2["h2DEmuQualph2SegNHits"+chambTags.at(indstat)]->Fill(ph2Seg_phi_nHits->at(iph2Seg), get_quality_group(bestQualTrigAM[indstat]));
      m_plots2["h2DTMQualph2SegNHits"+chambTags.at(indstat)]->Fill(ph2Seg_phi_nHits->at(iph2Seg), get_tm_quality_group(bestQualTrigTM[indstat]));
      auto chambTag = chambTags.at(indstat);
      if (bestQualTrigHW[indstat] >= 6) m_plots["hHwHighQualph2SegNHits" + chambTag]->Fill(ph2Seg_phi_nHits->at(iph2Seg));
      else m_plots["hHwLowQualph2SegNHits" + chambTag]->Fill(ph2Seg_phi_nHits->at(iph2Seg));
      if (bestQualTrigAM[indstat] >= 6) m_plots["hEmuHighQualph2SegNHits" + chambTag]->Fill(ph2Seg_phi_nHits->at(iph2Seg));
      else m_plots["hEmuLowQualph2SegNHits" + chambTag]->Fill(ph2Seg_phi_nHits->at(iph2Seg));
      if (bestQualTrigTM[indstat] >= 4) m_plots["hTMHighQualph2SegNHits" + chambTag]->Fill(ph2Seg_phi_nHits->at(iph2Seg));
      else m_plots["hTMLowQualph2SegNHits" + chambTag]->Fill(ph2Seg_phi_nHits->at(iph2Seg));

      if (IbestQualTrigHW[indstat]!=-1) m_plots["hEffHWvsph2SegX"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (bestQualTrigHW[indstat]>=6) m_plots["hEffHWvsph2SegXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (bestQualTrigHW[indstat]>=3) m_plots["hEffHWvsph2SegXQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (IbestQualTrigHW[indstat]!=-1) m_plots["hEffHWvsph2SegT0"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      if (bestQualTrigHW[indstat]>=6) m_plots["hEffHWvsph2SegT0Corr"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      if (bestQualTrigHW[indstat]>=3) m_plots["hEffHWvsph2SegT0Q>2"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      m_plots["hEffHWvsph2SegX"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffHWvsph2SegXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffHWvsph2SegXQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffHWvsph2SegT0"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      m_plots["hEffHWvsph2SegT0Corr"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      m_plots["hEffHWvsph2SegT0Q>2"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);

      if (IbestQualTrigBXHW[indstat]!=-1) m_plots["hEffHWvsph2SegXGoodBX"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (bestQualTrigBXHW[indstat]>=6) m_plots["hEffHWvsph2SegXGoodBXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (bestQualTrigBXHW[indstat]>=3) m_plots["hEffHWvsph2SegXGoodBXQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (IbestQualTrigBXHW[indstat]!=-1) m_plots["hEffHWvsph2SegT0GoodBX"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      if (bestQualTrigBXHW[indstat]>=6) m_plots["hEffHWvsph2SegT0GoodBXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      if (bestQualTrigBXHW[indstat]>=3) m_plots["hEffHWvsph2SegT0GoodBXQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      m_plots["hEffHWvsph2SegXGoodBX"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffHWvsph2SegXGoodBXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffHWvsph2SegXGoodBXQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffHWvsph2SegT0GoodBX"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      m_plots["hEffHWvsph2SegT0GoodBXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      m_plots["hEffHWvsph2SegT0GoodBXQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);

      if (IbestQualTrigAM[indstat]!=-1) m_plots["hEffAMvsph2SegX"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (bestQualTrigAM[indstat]>=3) m_plots["hEffAMvsph2SegXQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (bestQualTrigAM[indstat]>=6) m_plots["hEffAMvsph2SegXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (IbestQualTrigBXAM[indstat]!=-1) m_plots["hEffAMvsph2SegXGoodBX"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (bestQualTrigBXAM[indstat]>=3) m_plots["hEffAMvsph2SegXGoodBXQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (bestQualTrigBXAM[indstat]>=6) m_plots["hEffAMvsph2SegXGoodBXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      m_plots["hEffAMvsph2SegX"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffAMvsph2SegXQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffAMvsph2SegXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffAMvsph2SegXGoodBX"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffAMvsph2SegXGoodBXQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffAMvsph2SegXGoodBXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);

      if (IbestQualTrigAM[indstat]!=-1) m_plots["hEffAMvsph2SegT0"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      if (bestQualTrigAM[indstat]>=3) m_plots["hEffAMvsph2SegT0Q>2"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      if (bestQualTrigAM[indstat]>=6) m_plots["hEffAMvsph2SegT0Corr"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      if (IbestQualTrigBXAM[indstat]!=-1) m_plots["hEffAMvsph2SegT0GoodBX"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      if (bestQualTrigBXAM[indstat]>=3) m_plots["hEffAMvsph2SegT0GoodBXQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      if (bestQualTrigBXAM[indstat]>=6) m_plots["hEffAMvsph2SegT0GoodBXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      m_plots["hEffAMvsph2SegT0"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      m_plots["hEffAMvsph2SegT0Q>2"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      m_plots["hEffAMvsph2SegT0Corr"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      m_plots["hEffAMvsph2SegT0GoodBX"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      m_plots["hEffAMvsph2SegT0GoodBXQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      m_plots["hEffAMvsph2SegT0GoodBXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      
      if (IbestQualTrigTM[indstat]!=-1) m_plots["hEffTMvsph2SegX"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (IbestQualTrigBXTM[indstat]!=-1) m_plots["hEffTMvsph2SegXGoodBX"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (bestQualTrigTM[indstat]>=4) m_plots["hEffTMvsph2SegXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (bestQualTrigBXTM[indstat]>=4) m_plots["hEffTMvsph2SegXGoodBXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
      if (IbestQualTrigTM[indstat]!=-1) m_plots["hEffTMvsph2SegT0"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      if (IbestQualTrigBXTM[indstat]!=-1) m_plots["hEffTMvsph2SegT0GoodBX"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      if (bestQualTrigTM[indstat]>=4) m_plots["hEffTMvsph2SegT0Corr"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      if (bestQualTrigBXTM[indstat]>=4) m_plots["hEffTMvsph2SegT0GoodBXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
      m_plots["hEffTMvsph2SegX"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffTMvsph2SegXGoodBX"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffTMvsph2SegXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffTMvsph2SegXGoodBXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      m_plots["hEffTMvsph2SegT0"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      m_plots["hEffTMvsph2SegT0GoodBX"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      m_plots["hEffTMvsph2SegT0Corr"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      m_plots["hEffTMvsph2SegT0GoodBXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);

      float limInfT0 = -17.5;
      float limSupT0 = 7.5;

      float limInfPos = -100;
      float limSupPos = 100;

      if (myph2Segt0 >= limInfT0 && myph2Segt0 <= limSupT0){
          if (IbestQualTrigHW[indstat]!=-1) m_plots["hEffHWvsph2SegXLim"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          if (bestQualTrigHW[indstat]>=6) m_plots["hEffHWvsph2SegXLimCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          if (bestQualTrigHW[indstat]>=3) m_plots["hEffHWvsph2SegXLimQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          m_plots["hEffHWvsph2SegXLim"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          m_plots["hEffHWvsph2SegXLimCorr"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          m_plots["hEffHWvsph2SegXLimQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          
          if (IbestQualTrigBXHW[indstat]!=-1) m_plots["hEffHWvsph2SegXLimGoodBX"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          if (bestQualTrigBXHW[indstat]>=6) m_plots["hEffHWvsph2SegXLimGoodBXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          if (bestQualTrigBXHW[indstat]>=3) m_plots["hEffHWvsph2SegXLimGoodBXQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          m_plots["hEffHWvsph2SegXLimGoodBX"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          m_plots["hEffHWvsph2SegXLimGoodBXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          m_plots["hEffHWvsph2SegXLimGoodBXQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          
          
          if (IbestQualTrigAM[indstat]!=-1) m_plots["hEffAMvsph2SegXLim"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          if (bestQualTrigAM[indstat]>=3) m_plots["hEffAMvsph2SegXLimQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          if (bestQualTrigAM[indstat]>=6) m_plots["hEffAMvsph2SegXLimCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          if (IbestQualTrigBXAM[indstat]!=-1) m_plots["hEffAMvsph2SegXLimGoodBX"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          if (bestQualTrigBXAM[indstat]>=3) m_plots["hEffAMvsph2SegXLimGoodBXQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          if (bestQualTrigBXAM[indstat]>=6) m_plots["hEffAMvsph2SegXLimGoodBXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          m_plots["hEffAMvsph2SegXLim"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          m_plots["hEffAMvsph2SegXLimQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          m_plots["hEffAMvsph2SegXLimCorr"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          m_plots["hEffAMvsph2SegXLimGoodBX"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          m_plots["hEffAMvsph2SegXLimGoodBXQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          m_plots["hEffAMvsph2SegXLimGoodBXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          
          if (IbestQualTrigTM[indstat]!=-1) m_plots["hEffTMvsph2SegXLim"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          if (IbestQualTrigBXTM[indstat]!=-1) m_plots["hEffTMvsph2SegXLimGoodBX"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          if (bestQualTrigTM[indstat]>=4) m_plots["hEffTMvsph2SegXLimCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          if (bestQualTrigBXTM[indstat]>=4) m_plots["hEffTMvsph2SegXLimGoodBXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2SegPos);
          m_plots["hEffTMvsph2SegXLim"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          m_plots["hEffTMvsph2SegXLimGoodBX"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          m_plots["hEffTMvsph2SegXLimCorr"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
          m_plots["hEffTMvsph2SegXLimGoodBXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2SegPos);
      }
      
      if (myph2SegPos >= limInfPos && myph2SegPos <= limSupPos){
          if (IbestQualTrigHW[indstat]!=-1) m_plots["hEffHWvsph2SegT0Lim"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          if (bestQualTrigHW[indstat]>=6) m_plots["hEffHWvsph2SegT0LimCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          if (bestQualTrigHW[indstat]>=3) m_plots["hEffHWvsph2SegT0LimQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          m_plots["hEffHWvsph2SegT0Lim"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          m_plots["hEffHWvsph2SegT0LimCorr"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          m_plots["hEffHWvsph2SegT0LimQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          
          if (IbestQualTrigBXHW[indstat]!=-1) m_plots["hEffHWvsph2SegT0LimGoodBX"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          if (bestQualTrigBXHW[indstat]>=6) m_plots["hEffHWvsph2SegT0LimGoodBXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          if (bestQualTrigBXHW[indstat]>=3) m_plots["hEffHWvsph2SegT0LimGoodBXQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          m_plots["hEffHWvsph2SegT0LimGoodBX"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          m_plots["hEffHWvsph2SegT0LimGoodBXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          m_plots["hEffHWvsph2SegT0LimGoodBXQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          
          
          if (IbestQualTrigAM[indstat]!=-1) m_plots["hEffAMvsph2SegT0Lim"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          if (bestQualTrigAM[indstat]>=3) m_plots["hEffAMvsph2SegT0LimQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          if (bestQualTrigAM[indstat]>=6) m_plots["hEffAMvsph2SegT0LimCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          if (IbestQualTrigBXAM[indstat]!=-1) m_plots["hEffAMvsph2SegT0LimGoodBX"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          if (bestQualTrigBXAM[indstat]>=3) m_plots["hEffAMvsph2SegT0LimGoodBXQ>2"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          if (bestQualTrigBXAM[indstat]>=6) m_plots["hEffAMvsph2SegT0LimGoodBXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          m_plots["hEffAMvsph2SegT0Lim"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          m_plots["hEffAMvsph2SegT0LimQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          m_plots["hEffAMvsph2SegT0LimCorr"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          m_plots["hEffAMvsph2SegT0LimGoodBX"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          m_plots["hEffAMvsph2SegT0LimGoodBXQ>2"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          m_plots["hEffAMvsph2SegT0LimGoodBXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          
          if (IbestQualTrigTM[indstat]!=-1) m_plots["hEffTMvsph2SegT0Lim"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          if (IbestQualTrigBXTM[indstat]!=-1) m_plots["hEffTMvsph2SegT0LimGoodBX"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          if (bestQualTrigTM[indstat]>=4) m_plots["hEffTMvsph2SegT0LimCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          if (bestQualTrigBXTM[indstat]>=4) m_plots["hEffTMvsph2SegT0LimGoodBXCorr"+chambTags.at(indstat)+"passed"]->Fill(myph2Segt0);
          m_plots["hEffTMvsph2SegT0Lim"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          m_plots["hEffTMvsph2SegT0LimGoodBX"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          m_plots["hEffTMvsph2SegT0LimCorr"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
          m_plots["hEffTMvsph2SegT0LimGoodBXCorr"+chambTags.at(indstat)+"total"]->Fill(myph2Segt0);
      }
      
    }
    
    for (unsigned int i = 0; i<chambTags.size(); i++){
      for (unsigned int j = 0; j<quTags.size(); j++){
        if (bestTrigHW[i][j] != -1){
          
          short myHwWheel = ph2TpgPhiHw_wheel->at(bestTrigHW[i][j]);
          short myHwSector = ph2TpgPhiHw_sector->at(bestTrigHW[i][j]);
          short myHwStation = ph2TpgPhiHw_station->at(bestTrigHW[i][j]);
          float myPosHW =  ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]);
          float myDirHW =  ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]);
          
          if (myph2SegWheel != myHwWheel || myph2SegSector != myHwSector || myph2SegStation != myHwStation ) continue;
          
          if ((j != 2 && j !=3 && j != 4 ) && (ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]) == 1)) myph2SegPos = myph2SegPosSL1;
          else if ((j != 2 && j != 3 && j != 4 ) && (ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]) == 3)) myph2SegPos = myph2SegPosSL3;
          else myph2SegPos = myph2SegPosMid; 

          int myt0HW = ph2TpgPhiHw_t0->at(bestTrigHW[i][j]); 
          if (correctL1A) myt0HW = myt0HW - eventoBX*25;
          
          // Fill plots
          std::vector <std::string> categories = {};
          categories.push_back(labelTags.at(0));
          if (j == 2 || j == 3 || j == 4) {
            categories.push_back(labelTags.at(1));
            } else {
            categories.push_back(labelTags.at(2));
            short myHWSL = ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]);	
            categories.push_back(labelTags.at(2) + slTags.at(myHWSL / 2));
          }
          categories.push_back(quTags.at(j));

          for (auto & category : categories){
            m_plots2  ["hPsi2Dph2Seg"     + chambTags.at(i) + category]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]), myph2SegPsi  );
            m_plots2  ["hTime2Dph2Seg"    + chambTags.at(i) + category]->Fill(myt0HW - offset[i] * 25,myph2Segt0                         );
            m_plots2  ["hPos2Dph2Seg"     + chambTags.at(i) + category]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]), myph2SegPos    );
            m_plots   ["hPsiph2Seg"       + chambTags.at(i) + category]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - myph2SegPsi );
            m_plots   ["hTimeph2Seg"      + chambTags.at(i) + category]->Fill(myt0HW - offset[i] * 25 - myph2Segt0                       );
            m_plots   ["hPosph2Seg"       + chambTags.at(i) + category]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - myph2SegPos   );
            m_plots2  ["hTimeph2SegvsPos" + chambTags.at(i) + category]->Fill(myph2SegPos, myt0HW - offset[i] * 25 - myph2Segt0          );
            m_plots2  ["hTimeph2SegvsPsi" + chambTags.at(i) + category]->Fill(myph2SegPsi, myt0HW - offset[i] * 25 - myph2Segt0          );
            if (myph2SegHasZ > 0) 
            m_plots2["hTimeph2SegvsZ"   + chambTags.at(i) + category]->Fill(myph2SegZ, myt0HW - offset[i] * 25 - myph2Segt0              );
          }
        }
      }

      if (IbestQualTrigHW[i] != -1){ 
        // FIXME: Position always in the center, optimizable 
        short myBestHwWheel   = ph2TpgPhiHw_wheel     ->at(IbestQualTrigHW[i]);
        short myBestHwSector  = ph2TpgPhiHw_sector    ->at(IbestQualTrigHW[i]);
        short myBestHwStation = ph2TpgPhiHw_station   ->at(IbestQualTrigHW[i]);
        float myBestPosHW     = ph2TpgPhiHw_posLoc_x  ->at(IbestQualTrigHW[i]);
        float myBestDirHW     = ph2TpgPhiHw_dirLoc_phi->at(IbestQualTrigHW[i]);
        int   myBestT0HW      = ph2TpgPhiHw_t0        ->at(IbestQualTrigHW[i]); 
        if (correctL1A) myBestT0HW = myBestT0HW - eventoBX*25;

        if (ph2TpgPhiHw_superLayer->at(IbestQualTrigHW[i]) == 1) myph2SegPos = myph2SegPosSL1;
            else if (ph2TpgPhiHw_superLayer->at(IbestQualTrigHW[i]) == 3) myph2SegPos = myph2SegPosSL3;
        else myph2SegPos = myph2SegPosMid;

        m_plots2["hPsi2Dph2Seg"    + chambTags.at(i) + "bestQ"] -> Fill(myBestDirHW, myph2SegPsi                              );
        m_plots2["hTime2Dph2Seg"   + chambTags.at(i) + "bestQ"] -> Fill(myBestT0HW - offset[i] * 25, myph2Segt0               );
        m_plots2["hPos2Dph2Seg"    + chambTags.at(i) + "bestQ"] -> Fill(myBestPosHW, myph2SegPos                              );
        m_plots ["hPsiph2Seg"      + chambTags.at(i) + "bestQ"] -> Fill(myBestDirHW - myph2SegPsi                             );
        m_plots ["hTimeph2Seg"     + chambTags.at(i) + "bestQ"] -> Fill(myBestT0HW - offset[i] * 25 - myph2Segt0              );
        m_plots ["hPosph2Seg"      + chambTags.at(i) + "bestQ"] -> Fill(myBestPosHW - myph2SegPos                             );
        m_plots2["hTimeph2SegvsPos"+ chambTags.at(i) + "bestQ"] -> Fill(myph2SegPos, myBestT0HW - offset[i] * 25 - myph2Segt0 );
        m_plots2["hTimeph2SegvsPsi"+ chambTags.at(i) + "bestQ"] -> Fill(myph2SegPsi, myBestT0HW - offset[i] * 25 - myph2Segt0 );
        if (myph2SegHasZ > 0) 
          m_plots2["hTimeph2SegvsZ"  + chambTags.at(i) + "bestQ"] -> Fill(myph2SegZ, myBestT0HW - offset[i] * 25 - myph2Segt0 );
      }
    } //STAT
  } //for ph2segments

  // if (entro) cout << "------------------------------------------------------" << endl; 

  if (bestI[0] != -1 && bestI[3] != -1) {
    m_effs["hEffCor"]->Fill(true,0);
    m_effs["hEffCorAM"]->Fill(true,0);
    m_effs["hEffCorHW"]->Fill(true,0);
  } else if (bestI[0] == -1 && bestI[3] != -1) {
    m_effs["hEffCor"]->Fill(false,0);
    m_effs["hEffCorAM"]->Fill(true,0);
    m_effs["hEffCorHW"]->Fill(false,0);
  } else if (bestI[3] == -1 && bestI[0] != -1) {
    m_effs["hEffCor"]->Fill(false,0);
    m_effs["hEffCorHW"]->Fill(true,0);
    m_effs["hEffCorAM"]->Fill(false,0);
  }

  return;  
}

void DTNtupleTPGSimAnalyzer::endJob()
{
  
  m_outFile.cd();
  
  m_outFile.Write();
  m_outFile.Close();
  return;
}

Double_t DTNtupleTPGSimAnalyzer::trigPhiInRad(Double_t trigPhi, Int_t sector)
{
  
  return trigPhi / 65536. * 0.8 + TMath::Pi() / 6 * (sector - 1); 
  
}


void DTNtupleTPGSimAnalyzer::DisplayEventNumbers () {
  
  ofstream f2;
  f2.open("failedEventNumbers.txt",fstream::app);
  
  f2 << event_eventNumber << endl;
  f2 << "-1" << endl;
  return;
}


void DTNtupleTPGSimAnalyzer::DisplayPh2Hits () {
  
  ofstream f2;
  f2.open("failedPh2Hits.txt",fstream::app);
  
  //  f2 << event_eventNumber << endl;
  for (unsigned int iHit = 0; iHit < ph2Digi_nDigis; iHit++) {
    if ( ph2Digi_station -> at(iHit) != 4) continue; 
    f2 << ph2Digi_wheel -> at(iHit) << " " 
    << ph2Digi_sector -> at(iHit) << " "
    << ph2Digi_station -> at(iHit) << " "
    << "8.4" << " "
    << ph2Digi_superLayer -> at(iHit) - 1 << " "
    << ph2Digi_layer -> at(iHit) - 1 << " "
    << ph2Digi_wire -> at(iHit)  << " "
    << (int) round(ph2Digi_time -> at(iHit)) << " "
    << endl; 
  } 
  
  f2 << "-1 -1 -1 -1 -1 -1 -1" << endl; 
  
  f2.close();
  return;  
  
}

void DTNtupleTPGSimAnalyzer::DisplayPh2Prims () {
  
  ofstream f2;
  f2.open("failedPh2Prims.txt",fstream::app);
  
  for (std::size_t iTrigHW = 0; iTrigHW < ph2TpgPhiHw_nTrigs; ++iTrigHW)
  {
    short myStationHW = ph2TpgPhiHw_station->at(iTrigHW);
    short mySectorHW = ph2TpgPhiHw_sector->at(iTrigHW);
    short myWheelHW = ph2TpgPhiHw_wheel->at(iTrigHW);
    short myQualityHW = ph2TpgPhiHw_quality->at(iTrigHW);
    short mySLHW = ph2TpgPhiHw_superLayer->at(iTrigHW);
    int myChiHW =  ph2TpgPhiHw_chi2->at(iTrigHW);
    int myPhiHW = ph2TpgPhiHw_phi->at(iTrigHW);
    int myPhiBHW =   ph2TpgPhiHw_phiB->at(iTrigHW);
    float myPosHW =  ph2TpgPhiHw_posLoc_x->at(iTrigHW);
    float myDirHW =  ph2TpgPhiHw_dirLoc_phi->at(iTrigHW);
    int myChi2HW = ph2TpgPhiHw_chi2->at(iTrigHW); 
    int myBXHW = ph2TpgPhiHw_BX->at(iTrigHW); //eventoBX = myBXHW; 
    int myt0HW = ph2TpgPhiHw_t0->at(iTrigHW);
    
    
    
    if ( myStationHW != 4) continue; 
    f2
    << myQualityHW << " "  
    << mySLHW << " "  
    << myPosHW << " "  
    << myDirHW << " "  
    << myChi2HW << " "  
    << myt0HW << " "  
    << endl; 
    
  }
  
  f2 << "-1 -1 -1 -1 -1 -1 -1" << endl; 
  f2.close();
  return;
}  

void DTNtupleTPGSimAnalyzer::DisplayPh1Prims () {
  
  
  ofstream f2;
  f2.open("failedPh1Prims.txt",fstream::app);
  
  for (std::size_t iTwin = 0; iTwin <  ltTwinMuxOut_nTrigs; ++iTwin) {
    short myStationTwin = ltTwinMuxOut_station->at(iTwin);
    short mySectorTwin = ltTwinMuxOut_sector->at(iTwin);
    short myWheelTwin = ltTwinMuxOut_wheel->at(iTwin);
    short myQualityTwin = ltTwinMuxOut_quality->at(iTwin);
    int myPhiTwin = ltTwinMuxOut_phi->at(iTwin);
    int myPhiBTwin =   ltTwinMuxOut_phiB->at(iTwin);
    float myPosTwin =  ltTwinMuxOut_posLoc_x->at(iTwin);
    float myDirTwin =  ltTwinMuxOut_dirLoc_phi->at(iTwin);
    int myBXTwin = ltTwinMuxOut_BX->at(iTwin);
    
    if ( myStationTwin != 4 || myWheelTwin != 2 || mySectorTwin != 12 ) continue; 
    
    f2
    << myQualityTwin << " "  
    << myPosTwin << " "  
    << myDirTwin << " "  
    << myBXTwin << " "  
    << endl; 
    
  }
  
  f2 << "-1 -1 -1 -1" << endl; 
  f2.close();
  
  return;
}

void DTNtupleTPGSimAnalyzer::DisplayPh1Segs () {
  
  
  ofstream f2;
  f2.open("failedPh1Segs.txt",fstream::app);
  
  for (std::size_t iSeg = 0; iSeg <  seg_nSegments; ++iSeg) {
    float mySegt0 = seg_phi_t0->at(iSeg);
    float mySegPos = seg_posLoc_x->at(iSeg);
    float mySegPsi = 360*TMath::ATan ( ( seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) / (2*TMath::Pi());
    if ( seg_station -> at(iSeg) != 4) continue; 
    f2  
    << event_bunchCrossing << " "
    << mySegt0 << " "
    << mySegPos << " "
    << mySegPsi
    << endl; 
  } 
  
  f2 << "-1 -1 -1 -1 -1 -1 -1" << endl; 
  
  f2.close();
  return;  
  
}

void DTNtupleTPGSimAnalyzer::getTheStupidPlots() {

  for (int i = 1; i <= 4; i++) {
    int bestISeg = -1;
    short bestQSeg = 0;
    int bestIPh2 = -1;
    short bestQPh2 = 0;
    int bestITM = -1;
    short bestQTM = 0;
     
    for (std::size_t iSeg = 0; iSeg <  seg_nSegments; ++iSeg) {
      float mySegt0 = seg_phi_t0->at(iSeg);
      short mySegStation = seg_station->at(iSeg);
      short mySegWheel = seg_wheel->at(iSeg);
      short mySegSector = seg_sector->at(iSeg);
      float mySegPos = seg_posLoc_x->at(iSeg);
      float mySegPosSL1 = seg_posLoc_x_SL1->at(iSeg);
      float mySegPosSL3 = seg_posLoc_x_SL3->at(iSeg);
      float mySegPosMid = seg_posLoc_x_midPlane->at(iSeg);
      float mySegZ = seg_posLoc_y->at(iSeg);
      short mySegHasZ = seg_hasZed->at(iSeg);
      float mySegPsi = 360*TMath::ATan ( ( seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) / (2*TMath::Pi());
      if ( mySegStation != i || mySegWheel != 2 || mySegSector != 12 ) continue; 
      if (fabs(mySegPsi) > 30 || seg_phi_nHits->at(iSeg) < 4 || fabs(mySegt0)>50 ) continue;
      
      if ( seg_phi_nHits->at(iSeg) > bestQSeg ) { 
        bestISeg = iSeg;    
        bestQSeg = seg_phi_nHits->at(iSeg);    
      }
    }
    
    if (bestISeg < 0) return;
    
    for (std::size_t iTwin = 0; iTwin <  ltTwinMuxOut_nTrigs; ++iTwin) {
      short myStationTwin = ltTwinMuxOut_station->at(iTwin);
      short mySectorTwin = ltTwinMuxOut_sector->at(iTwin);
      short myWheelTwin = ltTwinMuxOut_wheel->at(iTwin);
      short myQualityTwin = ltTwinMuxOut_quality->at(iTwin);
      int myPhiTwin = ltTwinMuxOut_phi->at(iTwin);
      int myPhiBTwin =   ltTwinMuxOut_phiB->at(iTwin);
      float myPosTwin =  ltTwinMuxOut_posLoc_x->at(iTwin);
      float myDirTwin =  ltTwinMuxOut_dirLoc_phi->at(iTwin);
      
      if ( myStationTwin != i || myWheelTwin != 2 || mySectorTwin != 12 ) continue; 
      if ( myQualityTwin > bestQTM ) { 
        bestITM = iTwin;    
        bestQTM = myQualityTwin;    
      }
    }
    
    if (bestITM > -1 ){
      m_plots["hTMSeg_MB" + std::to_string(i)]->Fill(ltTwinMuxOut_BX->at(bestITM) * 25 - seg_phi_t0->at(bestISeg) - 41);
      m_plots["hBXTMSeg_MB" + std::to_string(i)]->Fill(ltTwinMuxOut_BX->at(bestITM) - 2);
    }
    
    for (std::size_t iTrigHW = 0; iTrigHW < ph2TpgPhiHw_nTrigs; ++iTrigHW)
    {
      short myStationHW = ph2TpgPhiHw_station->at(iTrigHW);
      short mySectorHW = ph2TpgPhiHw_sector->at(iTrigHW);
      short myWheelHW = ph2TpgPhiHw_wheel->at(iTrigHW);
      short myQualityHW = ph2TpgPhiHw_quality->at(iTrigHW);
      short mySLHW = ph2TpgPhiHw_superLayer->at(iTrigHW);
      int myChiHW =  ph2TpgPhiHw_chi2->at(iTrigHW);
      int myPhiHW = ph2TpgPhiHw_phi->at(iTrigHW);
      int myPhiBHW =   ph2TpgPhiHw_phiB->at(iTrigHW);
      float myPosHW =  ph2TpgPhiHw_posLoc_x->at(iTrigHW);
      float myDirHW =  ph2TpgPhiHw_dirLoc_phi->at(iTrigHW);
      int myChi2HW = ph2TpgPhiHw_chi2->at(iTrigHW); 
      int myBXHW = ph2TpgPhiHw_BX->at(iTrigHW); //eventoBX = myBXHW; 
      int myt0HW = ph2TpgPhiHw_t0->at(iTrigHW);
      
      if ( myStationHW != i || myWheelHW != 2 || mySectorHW != 12 ) continue; 
      if ( myQualityHW > bestQPh2 ) { 
        bestIPh2 = iTrigHW;    
        bestQPh2 = myQualityHW;    
      }
    }
    if (bestIPh2 > -1 ){
      
      float ph2T0 = ph2TpgPhiHw_t0->at(bestIPh2);
      //float ph2T0 = ph2TpgPhiHw_t0->at(bestIPh2) -  (event_bunchCrossing )*25;
      // while (ph2T0 > 0) ph2T0 = ph2T0 - 25*3564;
      //ph2T0 = ph2T0 + 200*25;
      m_plots["hFWSeg_MB" + std::to_string(i)]->Fill(   ph2T0 - seg_phi_t0->at(bestISeg) + 1);
      float bx = ph2T0 / 25. + 100000;
      m_plots["hBXFWSeg_MB" + std::to_string(i)]->Fill(round(bx) - 100000);
      if (bestQPh2 > 2 ) m_plots["hFWSegQ>2_MB" + std::to_string(i)]->Fill(ph2T0 - seg_phi_t0->at(bestISeg) + 1);
      
      if (bestQPh2 > 2 ) m_plots["hBXFWSegQ>2_MB" + std::to_string(i)]->Fill(round(bx) - 100000);
    }
  }
  return;  
  
}

