#include "DTNtupleTriggerAnalyzer.h"

DTNtupleTriggerAnalyzer::DTNtupleTriggerAnalyzer(const TString & inFileName,
						 const TString & outFileName) :
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)  
{ 

  // Configuration parameters to switch on / off
  // different parts of the analyses
  m_twinMux = true;   
  m_ph2TpgPhiHw = true;
  m_ph2TpgPhiEmuAm = false;
  m_ph2TpgPhiEmuHb = false;
  m_segments = true;

  nMaxEvents = 99999999999; // maximum number of events to be processed

  iWh = 2;
  iSec = 12;
  iPhihits_min = 4;

  phi_Ph1_conv = 0.5/2048.;  // conversion from trig phi to phi in rad in Phase1
  phiB_Ph1_conv = 1./512.;  // conversion from trig phiB to phiB in rad in Phase1

  phi_Ph2_conv = 0.025; // in cm, local position in the station 
  phiB_Ph2_conv = 1/4096.; // to transform in radians

  BXOK_TwinMuxOut = 0;
  // BXOK_ph2Hw = 3295; // From ntuple based on SX5 run 329705
  // BXOK_ph2Hw = 3391; // From ntuple based on global run 330160
  // BXOK_ph2Hw = 3392; // From ntuple based on global run 330163
  // BXOK_ph2Hw = 3366; // From ntuple based on miniDAQ run 330463
  BXOK_ph2Hw = 0; // From ntuple based on miniDAQ run 338150
  BXOK_ph2EmuHb = 0; // to be properly set
  BXOK_ph2EmuAm = 0; // to be properly set

  pi=TMath::Pi();

}

DTNtupleTriggerAnalyzer::~DTNtupleTriggerAnalyzer() 
{ 

}


void DTNtupleTriggerAnalyzer::Loop()
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
	std::cout << "[DTNtupleTriggerAnalyzer::Loop] processed : " 
		  << jentry << " entries\r" << std::flush;

      if(jentry >nMaxEvents) break;  // set maximum number of processed events

      fill();

    }

  std::cout << std::endl; 
  endJob();

}

void DTNtupleTriggerAnalyzer::book()
{

  m_outFile.cd();



  for (Int_t iCh = 1; iCh < 5; ++iCh)
    {
      std::stringstream iChTag;
      iChTag << "MB" << iCh;

      std::string hName = "dummy" + iChTag.str();


      if(m_ph2TpgPhiHw && m_segments)   {  //*** ph2TpgPhiHw and segments must be "true"

	// -------------- efficiency plots for ph2TpgPhiHw
	hName = "trigeff_ph2TpgPhiHw_AnyBX_vs_t0" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs t0; t0; primitive effic.",
					100,-300.,300.);

	hName = "trigeff_ph2TpgPhiHw_AnyBX_vs_philoc" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs philoc; philoc; primitive effic",
					100,-80.,80.);

	hName = "trigeff_ph2TpgPhiHw_AnyBX_vs_nHits" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs nHits; nHits; primitive effic",
					11,-0.5,10.5);

	hName = "trigeff_ph2TpgPhiHw_AnyBX_vs_dirGlb_phi" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs dirGlb_phi; dirGlb_phi; primitive effic",
					100,-2.,1.);

	hName = "trigeff_ph2TpgPhiHw_AnyBX_vs_posGlb_phi" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs posGlb_phi; posGlb_phi; ph2TpgPhiHw effic",
					100,-0.6,-0.2);

	hName = "trigeff_ph2TpgPhiHw_AnyBX_vs_normChi2" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs normChi2; normChi2; ph2TpgPhiHw effic",
					60,0.,30.);


	// efficiency plots for ph2TphPhiHw for BXOK

	// -------------- efficiency plots for ph2TpgPhiHw
	hName = "trigeff_ph2TpgPhiHw_BXOK_vs_t0" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs t0; t0; primitive effic.",
					100,-300.,300.);

	hName = "trigeff_ph2TpgPhiHw_BXOK_vs_philoc" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs philoc; philoc; primitive effic",
					100,-80.,80.);

	hName = "trigeff_ph2TpgPhiHw_BXOK_vs_nHits" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs nHits; nHits; primitive effic",
					11,-0.5,10.5);

	hName = "trigeff_ph2TpgPhiHw_BXOK_vs_dirGlb_phi" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs dirGlb_phi; dirGlb_phi; primitive effic",
					100,-2.,1.);

	hName = "trigeff_ph2TpgPhiHw_BXOK_vs_posGlb_phi" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs posGlb_phi; posGlb_phi; ph2TpgPhiHw effic",
					100,-0.6,-0.2);

	hName = "trigeff_ph2TpgPhiHw_BXOK_vs_normChi2" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs normChi2; normChi2; ph2TpgPhiHw effic",
					60,0.,30.);
      }

      if(m_ph2TpgPhiEmuAm && m_segments)   {  //*** ph2TpgEmuAm and segments must be "true"

	// -------------- efficiency plots for ph2TpgPhiEmuAm
	hName = "trigeff_ph2TpgPhiEmuAm_AnyBX_vs_t0" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs t0; t0; primitive effic.",
					100,-300.,300.);

	hName = "trigeff_ph2TpgPhiEmuAm_AnyBX_vs_philoc" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs philoc; philoc; primitive effic",
					100,-80.,80.);

	hName = "trigeff_ph2TpgPhiEmuAm_AnyBX_vs_nHits" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs nHits; nHits; primitive effic",
					11,-0.5,10.5);

	hName = "trigeff_ph2TpgPhiEmuAm_AnyBX_vs_dirGlb_phi" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs dirGlb_phi; dirGlb_phi; primitive effic",
					100,-2.,1.);

	hName = "trigeff_ph2TpgPhiEmuAm_AnyBX_vs_posGlb_phi" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs posGlb_phi; posGlb_phi; ph2TpgPhiEmuAm effic",
					100,-0.6,-0.2);

	hName = "trigeff_ph2TpgPhiEmuAm_AnyBX_vs_normChi2" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs normChi2; normChi2; ph2TpgPhiEmuAm effic",
					60,0.,30.);


	// efficiency plots for ph2TphPhiEmuAm for BXOK

   

	hName = "trigeff_ph2TpgPhiEmuAm_BXOK_vs_t0" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs t0; t0; primitive effic.",
					100,-300.,300.);

	hName = "trigeff_ph2TpgPhiEmuAm_BXOK_vs_philoc" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs philoc; philoc; primitive effic",
					100,-80.,80.);

	hName = "trigeff_ph2TpgPhiEmuAm_BXOK_vs_nHits" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs nHits; nHits; primitive effic",
					11,-0.5,10.5);

	hName = "trigeff_ph2TpgPhiEmuAm_BXOK_vs_dirGlb_phi" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs dirGlb_phi; dirGlb_phi; primitive effic",
					100,-2.,1.);

	hName = "trigeff_ph2TpgPhiEmuAm_BXOK_vs_posGlb_phi" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs posGlb_phi; posGlb_phi; ph2TpgPhiEmuAm effic",
					100,-0.6,-0.2);

	hName = "trigeff_ph2TpgPhiEmuAm_BXOK_vs_normChi2" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs normChi2; normChi2; ph2TpgPhiEmuAm effic",
					60,0.,30.);

   
      }

      if(m_twinMux && m_segments)   {  //*** TwinMux and segments must be "true"

	// efficiency for TwinMuxOut AnyBX

	hName = "trigeff_TwinMuxOut_AnyBX_vs_t0" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs t0; t0; primitive effic.",
					100,-300.,300.);

	hName = "trigeff_TwinMuxOut_AnyBX_vs_philoc" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs philoc; philoc; primitive effic",
					100,-80.,80.);

	hName = "trigeff_TwinMuxOut_AnyBX_vs_nHits" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs nHits; nHits; primitive effic",
					11,-0.5,10.5);

	hName = "trigeff_TwinMuxOut_AnyBX_vs_dirGlb_phi" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs dirGlb_phi; dirGlb_phi; primitive effic",
					100,-2.,1.);

	hName = "trigeff_TwinMuxOut_AnyBX_vs_posGlb_phi" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs posGlb_phi; posGlb_phi; TwinMuxOut effic",
					100,-0.6,-0.2);

	hName = "trigeff_TwinMuxOut_AnyBX_vs_normChi2" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency AnyBX vs normChi2; normChi2; TwinMuxOut effic",
					60,0.,30.);

  

	// efficiency for TwinMuxOut BXOK 

	hName = "trigeff_TwinMuxOut_BXOK_vs_t0" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs t0; t0; primitive effic.",
					100,-300.,300.);

	hName = "trigeff_TwinMuxOut_BXOK_vs_philoc" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs philoc; philoc; primitive effic",
					100,-80.,80.);

	hName = "trigeff_TwinMuxOut_BXOK_vs_nHits" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs nHits; nHits; primitive effic",
					11,-0.5,10.5);

	hName = "trigeff_TwinMuxOut_BXOK_vs_dirGlb_phi" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs dirGlb_phi; dirGlb_phi; primitive effic",
					100,-2.,1.);

	hName = "trigeff_TwinMuxOut_BXOK_vs_posGlb_phi" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs posGlb_phi; posGlb_phi; TwinMuxOut effic",
					100,-0.6,-0.2);

	hName = "trigeff_TwinMuxOut_BXOK_vs_normChi2" + iChTag.str();
	m_effs[hName] = new TEfficiency(hName.c_str(),
					"trigger efficiency BXOK vs normChi2; normChi2; TwinMuxOut effic",
					60,0.,30.);

      }

      if(m_ph2TpgPhiHw && m_segments)   {  //*** ph2TpgPhiHw and segments must be "true"

	// correlations btw segment and primitive

	hName = "ph2TpgPhiHw_BX_vs_nHits" + iChTag.str();   // BX vs nHits
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "ph2TpgPhiHw BX vs segment nHits; segment nHits; ph2TpgHw BX ", 
				   11, -0.5, 10.5, 21,-10.5,10.5);  

	hName = "ph2TpgPhiHw_quality_vs_nHits" + iChTag.str();   // BX vs nHits
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "ph2TpgPhiHw quality vs segment nHits; segment nHits; ph2TpgHw quality ", 
				   10, 0.5, 10.5, 10,0.5,10.5);  

	hName = "ph2TpgPhiHw_phi_vs_seg_posLoc_x" + iChTag.str();   // phi vs posLoc_x
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "ph2TpgPhiHw phi vs segment posLoc x; segment posLoc x; ph2TpgHw phi", 
				   100, -150., 100., 100,-40000,40000);  
				 

	hName = "ph2TpgPhiHw_phiB_vs_seg_dirLoc_x" + iChTag.str();   // phiB vs dirLoc_x
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "ph2TpgPhiHw phiB vs segment dirLoc x; segment dirLoc x; ph2TpgHw phiB", 
				   100, -1., 1., 100,-3000,3000);  
			       
      }

      if(m_twinMux)   {  //*** TwinMux must be "true"

	// -------------  distribution plots for TwinMux Out
	hName = "BX_" + iChTag.str();   // BX distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "BX;BX;#entries/BX",
				   11,-5.5,5.5); 

	hName = "quality_" + iChTag.str();   // quality distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "quality;quality;#entries/quality",
				   11,-0.5,10.5); 

	hName = "phi_" + iChTag.str();   // phi distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "phi;phi;#entries/phi",
				   100,-0.6,0.6); 

	hName = "phiB_" + iChTag.str();   // phiB distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "phiB;phiB;#entries/phiB",
				   100,-0.2,0.2);

	// hName = "pos_" + iChTag.str();   // pos distribution 
	// m_plots[hName]  = new TH1F(hName.c_str(),
	// 				 "pos;pos;#entries/pos",
	// 				 100,-250.,250.); 

	// hName = "dir_" + iChTag.str();   // dir distribution 
	// m_plots[hName]  = new TH1F(hName.c_str(),
	// 				 "dir;dir;#entries/dir",
	// 				 100,-78.,78.); 

	// ---- BX = BXOK distributions

	hName = "BXOK_quality_" + iChTag.str();   // quality distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "whenBXOK quality;quality;#entries/quality",
				   11,-0.5,10.5); 

	hName = "BXOK_phi_" + iChTag.str();   // phi distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "whenBXOK phi;phi;#entries/phi",
				   100,-0.6,0.6); 

	hName = "BXOK_phiB_" + iChTag.str();   // phiB distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "whenBXOK phiB;phiB;#entries/phiB",
				   100,-0.2,0.2);
      }

      if(m_ph2TpgPhiHw)   {  //*** ph2TpgPhiHw  must be "true"

	// ---- Distributions for PHASE2Hw - Madrid primitives

	hName = "Phase2Hw_BX_" + iChTag.str();   // BX distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2Hw BX;BX;#entries/Phase2Hw BX",
				   21,-10.5,10.5);  

	hName = "Phase2Hw_quality_" + iChTag.str();   // quality distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2Hw quality; quality;#entries/quality",
				   11,-0.5,10.5); 

	hName = "Phase2Hw_phi_" + iChTag.str();   // phi distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2Hw phi;Phase2Hw phi;#entries/Phase2Hw phi",
				   100,0.,180.); 
	//	 100,-0.6,0.6); 

	hName = "Phase2Hw_phiB_" + iChTag.str();   // phiB distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Pjase2 phiB;phiB;#entries/phiB",
				   100,-100,100);

	hName = "Phase2Hw_chi2_" + iChTag.str();   // chi2 distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2Hw chi2;Phase2Hw chi2;#entries/Phase2Hw chi2",
				   32,0.,32.); 

	hName = "Phase2Hw_t0_" + iChTag.str();   // t0 distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2Hw t0;Phase2Hw t0;#entries/Phase2Hw t0",
				   100,0.,90000.); 

	hName = "Phase2Hw_BX_vs_quality_" + iChTag.str();   // Phase2Hw BX vs quality distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw BX vs quality;Phase2Hw quality;Phase2Hw BX",
				   11, -0.5,10.5,21,-10.5,10.5); 

  
	// ------------ now Phase2Hw (Madrid) with BXOK

	hName = "Phase2Hw_BXOK_quality_" + iChTag.str();   // quality distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2Hw BXOK quality; quality;#entries/quality",
				   11,-0.5,10.5); 

	hName = "Phase2Hw_BXOK_phi_" + iChTag.str();   // phi distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2Hw BXOK phi;Phase2Hw phi;#entries/Phase2Hw phi",
				   100,0.,180.); 
    

	hName = "Phase2Hw_BXOK_phiB_" + iChTag.str();   // phiB distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Pjase2 BXOK phiB;phiB;#entries/phiB",
				   100,-100,100);
				

	hName = "Phase2Hw_BXOK_chi2_" + iChTag.str();   // chi2 distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2Hw BXOK chi2;Phase2Hw chi2;#entries/Phase2Hw chi2",
				   32,0.,32.); 

	hName = "Phase2Hw_BXOK_t0_" + iChTag.str();   // t0 distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2Hw BXOK t0;Phase2Hw t0;#entries/Phase2Hw t0",
				   100,0.,90000.); 
      

      }
      if(m_ph2TpgPhiHw && m_twinMux)   { //*** ph2TpgPhiHw and TwinMux must be "true"

	// ***** PHASE2Hw - Hw Madrid primitives real vs TwinMuxOut

	hName = "Phase2Hw_vs_TwinMuxOut_BX_" + iChTag.str();   // BX distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_TMux BX;TMux_BX; Ph2TpgHw BX",
				   11,-5.5,5.5, 21,-10.5,10.5);  

	hName = "Phase2Hw_vs_TwinMuxOut_quality_" + iChTag.str();   // quality distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_TMux quality; TMux quality; Ph2TpgHw quality",
				   11,-0.5,10.5, 10,-0.5,10.5); 

	hName = "Phase2Hw_vs_TwinMuxOut_phi_" + iChTag.str();   // phi distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_TMux phi;TMux phi; Ph2TpgHw phi",
				   100,-0.6,0.6, 100,0.,180.); 
	//	 100,-0.6,0.6); 

	hName = "Phase2Hw_vs_TwinMuxOut_phiB_" + iChTag.str();   // phiB distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Pjase2Hw_vs_TMux phiB; TMux phiB; Ph2TpgHw phiB",
				   100,-0.2,0.2, 100,-100,100);

      }

      if(m_ph2TpgPhiHw && m_ph2TpgPhiEmuAm)   {  //*** ph2TpgPhiHw  and ph2TpgPhiEmuAm must be "true"

	// ***** PHASE2Hw - Hw Madrid primitives real vs Hw Emulator

	hName = "Phase2Hw_vs_EmuAm_BX_" + iChTag.str();   // BX distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_Emu BX;BX_emu; BX_Hw",
				   21,-10.5,10.5, 21,-10.5,10.5);  

	hName = "Phase2Hw_vs_EmuAm_quality_" + iChTag.str();   // quality distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_Emu quality; quality;#entries/quality",
				   11,-0.5,10.5, 11,-0.5,10.5); 

	hName = "Phase2Hw_vs_EmuAm_phi_" + iChTag.str();   // phi distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_Emu phi;Phase2Hw phi;#entries/Phase2Hw phi",
				   100,0.,180., 100,0.,180.); 
	//	 100,-0.6,0.6); 

	hName = "Phase2Hw_vs_EmuAm_phiB_" + iChTag.str();   // phiB distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Pjase2Hw_vs_Emu phiB;phiB;#entries/phiB",
				   100,-100,100, 100,-100,100);

	hName = "Phase2Hw_vs_EmuAm_chi2_" + iChTag.str();   // chi2 distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_Emu chi2;Phase2Hw chi2;#entries/Phase2Hw chi2",
				   32,0.,32., 32,0.,32.); 

	hName = "Phase2Hw_vs_EmuAm_t0_" + iChTag.str();   // t0 distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_Emu t0;Phase2Hw t0;#entries/Phase2Hw t0",
				   100,0.,90000., 100,0.,90000.); 
      }

      if(m_ph2TpgPhiHw && m_ph2TpgPhiEmuHb)   {   //*** ph2TpgPhiHw  and ph2TpgPhiEmuHb must be "true"
	// ***** PHASE2Hw - Hw Madrid primitives vs Hw Pozzobon emulator

	hName = "Phase2Hw_vs_EmuHb_BX_" + iChTag.str();   // BX distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_EmuHb BX;BX_emu; BX_Hw",
				   21,-10.5,10.5, 20,10.5,30.5);  

	hName = "Phase2Hw_vs_EmuHb_quality_" + iChTag.str();   // quality distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_EmuHb quality; quality;#entries/quality",
				   11,-0.5,10.5, 11,-0.5,10.5); 

	hName = "Phase2Hw_vs_EmuHb_phi_" + iChTag.str();   // phi distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_EmuHb phi;Phase2Hw phi;#entries/Phase2Hw phi",
				   100,0.,180., 100,0.,180.); 

	hName = "Phase2Hw_vs_EmuHb_phiB_" + iChTag.str();   // phiB distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Pjase2Hw_vs_EmuHb phiB;phiB;#entries/phiB",
				   100,-100,100, 100,-100,100);

	hName = "Phase2Hw_vs_EmuHb_chi2_" + iChTag.str();   // chi2 distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_EmuHb chi2;Phase2Hw chi2;#entries/Phase2Hw chi2",
				   32,0.,32., 32,0.,32.); 

	hName = "Phase2Hw_vs_EmuHb_t0_" + iChTag.str();   // t0 distribution 
	m_plots[hName]  = new TH2F(hName.c_str(),
				   "Phase2Hw_vs_EmuHb t0;Phase2Hw t0;#entries/Phase2Hw t0",
				   100,0.,90000., 100,0.,90000.); 
      }

 

      if(m_ph2TpgPhiEmuHb)   {  
	//   ----------  Ph2 Emu Pozzobon

	hName = "Phase2EmuHb_BX_" + iChTag.str();   // BX distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuHb BX;BX;#entries/Phase2EmuHb BX",
				   60,-300.5,-240.5);  

	hName = "Phase2EmuHb_quality_" + iChTag.str();   // quality distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuHb quality; quality;#entries/quality",
				   11,-0.5,10.5); 

	hName = "Phase2EmuHb_phi_" + iChTag.str();   // phi distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuHb phi;Phase2EmuHb phi;#entries/Phase2EmuHb phi",
				   100,0.,180.); 
	//	 100,-0.6,0.6); 

	hName = "Phase2EmuHb_phiB_" + iChTag.str();   // phiB distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Pjase2 phiB;phiB;#entries/phiB",
				   100,-100,100);

	hName = "Phase2EmuHb_chi2_" + iChTag.str();   // chi2 distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuHb chi2;Phase2EmuHb chi2;#entries/Phase2EmuHb chi2",
				   32,0.,32.); 

	hName = "Phase2EmuHb_t0_" + iChTag.str();   // t0 distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuHb t0;Phase2EmuHb t0;#entries/Phase2EmuHb t0",
				   100,0.,90000.); 

	// ------------ now Phase2EmuHb (Pozzobon) with BXOK

	hName = "Phase2EmuHb_BXOK_quality_" + iChTag.str();   // quality distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuHb BXOK quality; quality;#entries/quality",
				   11,-0.5,10.5); 

	hName = "Phase2EmuHb_BXOK_phi_" + iChTag.str();   // phi distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuHb BXOK phi;Phase2EmuHb phi;#entries/Phase2EmuHb phi",
				   100,0.,180.); 
	//	 100,-0.6,0.6); 

	hName = "Phase2EmuHb_BXOK_phiB_" + iChTag.str();   // phiB distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Pjase2 BXOK phiB;phiB;#entries/phiB",
				   100,-100,100);


	hName = "Phase2EmuHb_BXOK_chi2_" + iChTag.str();   // chi2 distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuHb BXOK chi2;Phase2EmuHb chi2;#entries/Phase2EmuHb chi2",
				   32,0.,32.); 

	hName = "Phase2EmuHb_BXOK_t0_" + iChTag.str();   // t0 distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuHb BXOK t0;Phase2EmuHb t0;#entries/Phase2EmuHb t0",
				   100,0.,90000.); 

      }


      if(m_ph2TpgPhiEmuAm)   {  
	//   ----------  Ph2 EmuAm Madrid

	hName = "Phase2EmuAm_BX_" + iChTag.str();   // BX distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuAm BX;BX;#entries/Phase2EmuAm BX",
				   60,-300.5,-240.5);  

	hName = "Phase2EmuAm_quality_" + iChTag.str();   // quality distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuAm quality; quality;#entries/quality",
				   11,-0.5,10.5); 

	hName = "Phase2EmuAm_phi_" + iChTag.str();   // phi distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuAm phi;Phase2EmuAm phi;#entries/Phase2EmuAm phi",
				   100,0.,180.); 
	//	 100,-0.6,0.6); 

	hName = "Phase2EmuAm_phiB_" + iChTag.str();   // phiB distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Pjase2 phiB;phiB;#entries/phiB",
				   100,-100,100);

	hName = "Phase2EmuAm_chi2_" + iChTag.str();   // chi2 distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuAm chi2;Phase2EmuAm chi2;#entries/Phase2EmuAm chi2",
				   32,0.,32.); 

	hName = "Phase2EmuAm_t0_" + iChTag.str();   // t0 distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuAm t0;Phase2EmuAm t0;#entries/Phase2EmuAm t0",
				   100,0.,90000.); 

	// ------------ now Phase2EmuAm (Madrid) with BXOK

	hName = "Phase2EmuAm_BXOK_quality_" + iChTag.str();   // quality distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuAm BXOK quality; quality;#entries/quality",
				   11,-0.5,10.5); 

	hName = "Phase2EmuAm_BXOK_phi_" + iChTag.str();   // phi distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuAm BXOK phi;Phase2EmuAm phi;#entries/Phase2EmuAm phi",
				   100,0.,180.); 
	//	 100,-0.6,0.6); 

	hName = "Phase2EmuAm_BXOK_phiB_" + iChTag.str();   // phiB distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Pjase2 BXOK phiB;phiB;#entries/phiB",
				   100,-100,100);

	hName = "Phase2EmuAm_BXOK_chi2_" + iChTag.str();   // chi2 distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuAm BXOK chi2;Phase2EmuAm chi2;#entries/Phase2EmuAm chi2",
				   32,0.,32.); 

	hName = "Phase2EmuAm_BXOK_t0_" + iChTag.str();   // t0 distribution 
	m_plots[hName]  = new TH1F(hName.c_str(),
				   "Phase2EmuAm BXOK t0;Phase2EmuAm t0;#entries/Phase2EmuAm t0",
				   100,0.,90000.); 

      }
    }
}

void DTNtupleTriggerAnalyzer::fill()
{


  float_t tan_phi;
  float_t  philoc;


  // ============================================================================
  //  find the "best" tdc segments (according to number of hits) 
  //  and the "best" primitives (based on quality) for each algorithm, for the four stations of Sector 12
  // ============================================================================


  UInt_t iBestSeg[4];  //******  All from 0 to 3!
  UInt_t iBestTwinMuxOut[4];
  UInt_t iBestTwinMuxOutBXOK[4];
  UInt_t iBestTpgPhiHw[4];
  UInt_t iBestTpgPhiHwBXOK[4];
  UInt_t iBestTpgPhiEmuAm[4];
  UInt_t iBestTpgPhiEmuAmBXOK[4];
  UInt_t iBestTpgPhiEmuHb[4];

  
  for(Int_t iMB=1; iMB<5; ++iMB) {  // 
    iBestSeg[iMB-1] = getBestSegm(iMB,iSec,iWh);
    iBestTwinMuxOut[iMB-1] = getBestTwinMuxOut(iMB,iSec,iWh);
    iBestTpgPhiHw[iMB-1] = getBestTpgPhiHw(iMB,iSec,iWh);
    iBestTpgPhiEmuAm[iMB-1] = getBestTpgPhiEmuAm(iMB,iSec,iWh);
    iBestTpgPhiEmuAmBXOK[iMB-1] = getBestTpgPhiEmuAmBXOK(iMB,iSec,iWh);
    iBestTpgPhiEmuHb[iMB-1] = getBestTpgPhiEmuHb(iMB,iSec,iWh); 
    iBestTpgPhiHwBXOK[iMB-1] = getBestTpgPhiHwBXOK(iMB,iSec,iWh);
    iBestTwinMuxOutBXOK[iMB-1] = getBestTwinMuxOutBXOK(iMB,iSec,iWh);
  }

  // =========================================================
  //   efficiency and correlations btw segment and primitive
  // =========================================================


  for(Int_t iMB=1; iMB<5; ++iMB) {   

 
    std::stringstream iChTag;

    iChTag << "MB" << iMB;

    std::string  hName = "dummy" + iChTag.str(); 
    if(m_segments)  { 

      if(iBestSeg[iMB-1]<9999) {  // there is a good segment in the station we are in

	tan_phi = seg_dirLoc_x->at(iBestSeg[iMB-1])/seg_dirLoc_z->at(iBestSeg[iMB-1]);
	philoc = atan(tan_phi)*180/(pi);  // local phi

	if(m_ph2TpgPhiHw )   {   // ph2TpgPhiHw efficiency at AnyBX

	  std::string  hName = "trigeff_ph2TpgPhiHw_AnyBX_vs_t0" + iChTag.str(); // vs t0
	  m_effs[hName]->Fill(iBestTpgPhiHw[iMB-1] < 9999, seg_phi_t0->at(iBestSeg[iMB-1]));
	

	
	  hName = "trigeff_ph2TpgPhiHw_AnyBX_vs_philoc" + iChTag.str();  // vs philoc
	  m_effs[hName]->Fill(iBestTpgPhiHw[iMB-1] < 9999, philoc);

	  hName = "trigeff_ph2TpgPhiHw_AnyBX_vs_nHits" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTpgPhiHw[iMB-1] < 9999, seg_phi_nHits->at(iBestSeg[iMB-1]));

	  hName = "trigeff_ph2TpgPhiHw_AnyBX_vs_posGlb_phi" + iChTag.str();  // vs posGlb_phi
	  m_effs[hName]->Fill(iBestTpgPhiHw[iMB-1] < 9999, seg_posGlb_phi->at(iBestSeg[iMB-1]));

	  hName = "trigeff_ph2TpgPhiHw_AnyBX_vs_dirGlb_phi" + iChTag.str();  // vs dirGlb_phi
	  m_effs[hName]->Fill(iBestTpgPhiHw[iMB-1] < 9999, seg_dirGlb_phi->at(iBestSeg[iMB-1]));

	  hName = "trigeff_ph2TpgPhiHw_AnyBX_vs_normChi2" + iChTag.str();  // vs normChi2
	  m_effs[hName]->Fill(iBestTpgPhiHw[iMB-1] < 9999, seg_phi_normChi2->at(iBestSeg[iMB-1]));

	  // ph2TpgHw efficency at BXOK

	  hName = "trigeff_ph2TpgPhiHw_BXOK_vs_t0" + iChTag.str(); // vs t0
	  m_effs[hName]->Fill(iBestTpgPhiHwBXOK[iMB-1] < 9999, seg_phi_t0->at(iBestSeg[iMB-1]));
	
	  hName = "trigeff_ph2TpgPhiHw_BXOK_vs_philoc" + iChTag.str();  // vs philoc
	  m_effs[hName]->Fill(iBestTpgPhiHwBXOK[iMB-1] < 9999, philoc);

	  hName = "trigeff_ph2TpgPhiHw_BXOK_vs_nHits" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTpgPhiHwBXOK[iMB-1] < 9999, seg_phi_nHits->at(iBestSeg[iMB-1]));

	  hName = "trigeff_ph2TpgPhiHw_BXOK_vs_posGlb_phi" + iChTag.str();  // vs posGlb_phi
	  m_effs[hName]->Fill(iBestTpgPhiHwBXOK[iMB-1] < 9999, seg_posGlb_phi->at(iBestSeg[iMB-1]));

	  hName = "trigeff_ph2TpgPhiHw_BXOK_vs_dirGlb_phi" + iChTag.str();  // vs dirGlb_phi
	  m_effs[hName]->Fill(iBestTpgPhiHwBXOK[iMB-1] < 9999, seg_dirGlb_phi->at(iBestSeg[iMB-1]));

	  hName = "trigeff_ph2TpgPhiHw_BXOK_vs_normChi2" + iChTag.str();  // vs normChi2
	  m_effs[hName]->Fill(iBestTpgPhiHwBXOK[iMB-1] < 9999, seg_phi_normChi2->at(iBestSeg[iMB-1]));

	}

	if(m_ph2TpgPhiEmuAm)   {   // ph2TpgPhiEmuHw efficiency 

	  //at AnyBX
  	
	  hName = "trigeff_ph2TpgPhiEmuAm_AnyBX_vs_t0" + iChTag.str(); // vs t0
	  m_effs[hName]->Fill(iBestTpgPhiEmuAm[iMB-1] < 9999, seg_phi_t0->at(iBestSeg[iMB-1]));
	
	  tan_phi = seg_dirLoc_x->at(iBestSeg[iMB-1])/seg_dirLoc_z->at(iBestSeg[iMB-1]);
	  philoc = atan(tan_phi)*180/(pi);  // local phi
	
	  hName = "trigeff_ph2TpgPhiEmuAm_AnyBX_vs_philoc" + iChTag.str();  // vs philoc
	  m_effs[hName]->Fill(iBestTpgPhiEmuAm[iMB-1] < 9999, philoc);

	  hName = "trigeff_ph2TpgPhiEmuAm_AnyBX_vs_nHits" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTpgPhiEmuAm[iMB-1] < 9999, seg_phi_nHits->at(iBestSeg[iMB-1]));

	  hName = "trigeff_ph2TpgPhiEmuAm_AnyBX_vs_posGlb_phi" + iChTag.str();  // vs posGlb_phi
	  m_effs[hName]->Fill(iBestTpgPhiEmuAm[iMB-1] < 9999, seg_posGlb_phi->at(iBestSeg[iMB-1]));

	  hName = "trigeff_ph2TpgPhiEmuAm_AnyBX_vs_dirGlb_phi" + iChTag.str();  // vs dirGlb_phi
	  m_effs[hName]->Fill(iBestTpgPhiEmuAm[iMB-1] < 9999, seg_dirGlb_phi->at(iBestSeg[iMB-1]));

	  hName = "trigeff_ph2TpgPhiEmuAm_AnyBX_vs_normChi2" + iChTag.str();  // vs normChi2
	  m_effs[hName]->Fill(iBestTpgPhiEmuAm[iMB-1] < 9999, seg_phi_normChi2->at(iBestSeg[iMB-1]));

	  // ph2TpgEmuAm efficiency with BXOK

	  hName = "trigeff_ph2TpgPhiEmuAm_BXOK_vs_t0" + iChTag.str(); // vs t0
	  m_effs[hName]->Fill(iBestTpgPhiEmuAmBXOK[iMB-1] < 9999, seg_phi_t0->at(iBestSeg[iMB-1]));
	
	  hName = "trigeff_ph2TpgPhiEmuAm_BXOK_vs_philoc" + iChTag.str();  // vs philoc
	  m_effs[hName]->Fill(iBestTpgPhiEmuAmBXOK[iMB-1] < 9999, philoc);

	  hName = "trigeff_ph2TpgPhiEmuAm_BXOK_vs_nHits" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTpgPhiEmuAmBXOK[iMB-1] < 9999, seg_phi_nHits->at(iBestSeg[iMB-1]));

	  hName = "trigeff_ph2TpgPhiEmuAm_BXOK_vs_posGlb_phi" + iChTag.str();  // vs posGlb_phi
	  m_effs[hName]->Fill(iBestTpgPhiEmuAmBXOK[iMB-1] < 9999, seg_posGlb_phi->at(iBestSeg[iMB-1]));

	  hName = "trigeff_ph2TpgPhiEmuAm_BXOK_vs_dirGlb_phi" + iChTag.str();  // vs dirGlb_phi
	  m_effs[hName]->Fill(iBestTpgPhiEmuAmBXOK[iMB-1] < 9999, seg_dirGlb_phi->at(iBestSeg[iMB-1]));

	  hName = "trigeff_ph2TpgPhiEmuAm_BXOK_vs_normChi2" + iChTag.str();  // vs normChi2
	  m_effs[hName]->Fill(iBestTpgPhiEmuAmBXOK[iMB-1] < 9999, seg_phi_normChi2->at(iBestSeg[iMB-1]));

	}

	if(m_twinMux)   {   // ph2TpgPhiHw efficiency 
      
	  // AnyBX
	  hName = "trigeff_TwinMuxOut_AnyBX_vs_t0" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTwinMuxOut[iMB-1] < 9999, seg_phi_t0->at(iBestSeg[iMB-1]));

	  hName = "trigeff_TwinMuxOut_AnyBX_vs_philoc" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTwinMuxOut[iMB-1] < 9999, philoc);

	  hName = "trigeff_TwinMuxOut_AnyBX_vs_nHits" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTwinMuxOut[iMB-1] < 9999, seg_phi_nHits->at(iBestSeg[iMB-1]));

	  hName = "trigeff_TwinMuxOut_AnyBX_vs_posGlb_phi" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTwinMuxOut[iMB-1] < 9999, seg_posGlb_phi->at(iBestSeg[iMB-1]));

	  hName = "trigeff_TwinMuxOut_AnyBX_vs_dirGlb_phi" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTwinMuxOut[iMB-1] < 9999, seg_dirGlb_phi->at(iBestSeg[iMB-1]));

	  hName = "trigeff_TwinMuxOut_AnyBX_vs_normChi2" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTwinMuxOut[iMB-1] < 9999, seg_phi_normChi2->at(iBestSeg[iMB-1]));

	  // TwinMuxOut efficiency at BXOK

	  hName = "trigeff_TwinMuxOut_BXOK_vs_t0" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTwinMuxOutBXOK[iMB-1] < 9999, seg_phi_t0->at(iBestSeg[iMB-1]));

	  hName = "trigeff_TwinMuxOut_BXOK_vs_philoc" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTwinMuxOutBXOK[iMB-1] < 9999, philoc);

	  hName = "trigeff_TwinMuxOut_BXOK_vs_nHits" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTwinMuxOutBXOK[iMB-1] < 9999, seg_phi_nHits->at(iBestSeg[iMB-1]));

	  hName = "trigeff_TwinMuxOut_BXOK_vs_posGlb_phi" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTwinMuxOutBXOK[iMB-1] < 9999, seg_posGlb_phi->at(iBestSeg[iMB-1]));

	  hName = "trigeff_TwinMuxOut_BXOK_vs_dirGlb_phi" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTwinMuxOutBXOK[iMB-1] < 9999, seg_dirGlb_phi->at(iBestSeg[iMB-1]));

	  hName = "trigeff_TwinMuxOut_BXOK_vs_normChi2" + iChTag.str();  // vs nHits
	  m_effs[hName]->Fill(iBestTwinMuxOutBXOK[iMB-1] < 9999, seg_phi_normChi2->at(iBestSeg[iMB-1]));

	}

	// // correlation plots
   
	if(m_ph2TpgPhiHw)   {   // ph2TpgPhiHw and segment correlation

	  if(iBestTpgPhiHw[iMB-1] < 9999)  { 
	    hName = "ph2TpgPhiHw_BX_vs_nHits" + iChTag.str();   // BX vs nHits
	    m_plots[hName]->Fill(seg_phi_nHits->at(iBestSeg[iMB-1]), ph2TpgPhiHw_BX->at(iBestTpgPhiHw[iMB-1]));  

	    hName = "ph2TpgPhiHw_quality_vs_nHits" + iChTag.str();   // BX vs nHits
	    m_plots[hName]->Fill(seg_phi_nHits->at(iBestSeg[iMB-1]), ph2TpgPhiHw_quality->at(iBestTpgPhiHw[iMB-1]));  

	    hName = "ph2TpgPhiHw_phi_vs_seg_posLoc_x" + iChTag.str();   // BX vs nHits
	    m_plots[hName]->Fill(seg_posLoc_x->at(iBestSeg[iMB-1]), ph2TpgPhiHw_phi->at(iBestTpgPhiHw[iMB-1]));  

	    hName = "ph2TpgPhiHw_phiB_vs_seg_dirLoc_x" + iChTag.str();   // BX vs nHits
	    m_plots[hName]->Fill(seg_dirLoc_x->at(iBestSeg[iMB-1]), ph2TpgPhiHw_phiB->at(iBestTpgPhiHw[iMB-1]));  
	  }
	}

      } // end of there is a segment
    } // end of if(m_segments)
  

      // 

      // ====================================================================
      // ============= plots on trigger primitives only =====================
      // ====================================================================


      //         -------------ltTwinMuxOut only 
  

    if(m_twinMux)  {
      for  (UInt_t  itr=0; itr<ltTwinMuxOut_nTrigs; ++itr) {
    
	if(ltTwinMuxOut_sector->at(itr)==iSec && ltTwinMuxOut_wheel->at(itr)==iWh) {  // only wheel 2 and sector 12
      
	  std::stringstream iChTag;
	  iChTag << "MB" << ltTwinMuxOut_station->at(itr);
      
	  std::string hName = "BX_" + iChTag.str();    // BX
	  m_plots[hName]->Fill(ltTwinMuxOut_BX->at(itr));

	  hName = "quality_" + iChTag.str();   // quality
	  m_plots[hName]->Fill(ltTwinMuxOut_quality->at(itr));
	
		
	  hName = "phi_" + iChTag.str();   // phi
	  m_plots[hName]->Fill(ltTwinMuxOut_phi->at(itr)*phi_Ph1_conv);

	
	  hName = "phiB_" + iChTag.str();   // phiB
	  m_plots[hName]->Fill(ltTwinMuxOut_phiB->at(itr)*phiB_Ph1_conv);


	  if(ltTwinMuxOut_BX->at(itr)==BXOK_TwinMuxOut) {  // --------------when BX = 0

	    hName = "BXOK_quality_" + iChTag.str(); //  quality
	    m_plots[hName]->Fill(ltTwinMuxOut_quality->at(itr));
	  
	    hName = "BXOK_phi_" + iChTag.str(); //  phi
	    m_plots[hName]->Fill(ltTwinMuxOut_phi->at(itr)*phi_Ph1_conv);
	  
	    hName = "BXOK_phiB_" + iChTag.str(); //  phiB
	    m_plots[hName]->Fill(ltTwinMuxOut_phiB->at(itr)*phiB_Ph1_conv);

	  
	  }
	}    
      }
    }

    //         -------------Phase2HW: Madrid

    if(m_ph2TpgPhiHw)  { // Phase2HW: Madrid

      for  (UInt_t  itr=0; itr<ph2TpgPhiHw_nTrigs; ++itr) {
  
	if(ph2TpgPhiHw_sector->at(itr)==iSec && ph2TpgPhiHw_wheel->at(itr)==iWh) {  // only wheel 2 and sector 12

      
	  std::stringstream iChTag;    
	  iChTag << "MB" << ph2TpgPhiHw_station->at(itr);  
	  std::string hName = "Phase2Hw_BX_" + iChTag.str();    // BX
	  m_plots[hName]->Fill(ph2TpgPhiHw_BX->at(itr) - BXOK_ph2Hw);


	  hName = "Phase2Hw_quality_" + iChTag.str();    // quality
	  m_plots[hName]->Fill(ph2TpgPhiHw_quality->at(itr));

	  hName = "Phase2Hw_phi_" + iChTag.str();    // phi
	  m_plots[hName]->Fill(ph2TpgPhiHw_phi->at(itr)*phi_Ph2_conv);

	  hName = "Phase2Hw_phiB_" + iChTag.str();    // phiB
	  m_plots[hName]->Fill(ph2TpgPhiHw_phiB->at(itr)*phiB_Ph2_conv);

	  hName = "Phase2Hw_chi2_" + iChTag.str();    // chi2
	  m_plots[hName]->Fill(ph2TpgPhiHw_chi2->at(itr));
      
	  hName = "Phase2Hw_t0_" + iChTag.str();    // t0
	  m_plots[hName]->Fill(ph2TpgPhiHw_t0->at(itr));

	  hName = "Phase2Hw_BX_vs_quality_" + iChTag.str();   // Phase2Hw BX vs quality distribution 
	  m_plots[hName]->Fill(ph2TpgPhiHw_quality->at(itr),ph2TpgPhiHw_BX->at(itr) - BXOK_ph2Hw);
      
  
	  if(ph2TpgPhiHw_BX->at(itr)==BXOK_ph2Hw)  {  // -------- when BXOK 

	    hName = "Phase2Hw_BXOK_quality_" + iChTag.str();    // quality
	    m_plots[hName]->Fill(ph2TpgPhiHw_quality->at(itr));

	    hName = "Phase2Hw_BXOK_phi_" + iChTag.str();    // phi
	    m_plots[hName]->Fill(ph2TpgPhiHw_quality->at(itr)*phi_Ph2_conv);

	    hName = "Phase2Hw_BXOK_phiB_" + iChTag.str();    // phiB
	    m_plots[hName]->Fill(ph2TpgPhiHw_quality->at(itr)*phiB_Ph2_conv);

	    hName = "Phase2Hw_BXOK_chi2_" + iChTag.str();    // chi2
	    m_plots[hName]->Fill(ph2TpgPhiHw_chi2->at(itr));
      
	    hName = "Phase2Hw_BXOK_t0_" + iChTag.str();    // t0
	    m_plots[hName]->Fill(ph2TpgPhiHw_t0->at(itr));

	  }

	  // -------- find correlation btw Madrid Hw andw TwinMuxOut

	  if(m_ph2TpgPhiHw && m_twinMux)  { // Madrid and TwinMux

	    for  (UInt_t  itr2=0; itr2<ltTwinMuxOut_nTrigs; ++itr2) { // 
	
	      if(ltTwinMuxOut_sector->at(itr2)==iSec && ltTwinMuxOut_wheel->at(itr2)==iWh && ph2TpgPhiHw_station->at(itr) == ltTwinMuxOut_station->at(itr2) ) {  // same wheel, sec, st
	  
		std::string hName = "Phase2Hw_vs_TwinMuxOut_quality_" + iChTag.str();    // quality
		m_plots[hName]->Fill(ltTwinMuxOut_quality->at(itr2), ph2TpgPhiHw_quality->at(itr));

		hName = "Phase2Hw_vs_TwinMuxOut_BX_" + iChTag.str();    // BX
		m_plots[hName]->Fill(ltTwinMuxOut_BX->at(itr2), ph2TpgPhiHw_BX->at(itr));

		hName = "Phase2Hw_vs_TwinMuxOut_phi_" + iChTag.str();    // phi
		m_plots[hName]->Fill(ltTwinMuxOut_phi->at(itr2), ph2TpgPhiHw_phi->at(itr));

		hName = "Phase2Hw_vs_TwinMuxOut_phiB_" + iChTag.str();    // phiB
		m_plots[hName]->Fill(ltTwinMuxOut_phiB->at(itr2), ph2TpgPhiHw_phiB->at(itr));
     	  
	      }
	    }
	  }

	  // -------- find correlation with EmuAm

	  if(m_ph2TpgPhiHw && m_ph2TpgPhiEmuAm)  { // Madrid and Emu Madrid

	    for  (UInt_t  itr2=0; itr2<ph2TpgPhiEmuAm_nTrigs; ++itr2) { // 
	
	      if(ph2TpgPhiEmuAm_sector->at(itr2)==iSec && ph2TpgPhiEmuAm_wheel->at(itr2)==iWh && ph2TpgPhiHw_station->at(itr) == ph2TpgPhiEmuAm_station->at(itr2)) {  // same wheel, sec, st 
	  
		std::string hName = "Phase2Hw_vs_EmuAm_quality_" + iChTag.str();    // quality
		m_plots[hName]->Fill(ph2TpgPhiEmuAm_quality->at(itr2), ph2TpgPhiHw_quality->at(itr));

		hName = "Phase2Hw_vs_EmuAm_BX_" + iChTag.str();    // BX
		m_plots[hName]->Fill(ph2TpgPhiEmuAm_BX->at(itr2), ph2TpgPhiHw_BX->at(itr));

		hName = "Phase2Hw_vs_EmuAm_phi_" + iChTag.str();    // phi
		m_plots[hName]->Fill(ph2TpgPhiEmuAm_phi->at(itr2), ph2TpgPhiHw_phi->at(itr));

		hName = "Phase2Hw_vs_EmuAm_phiB_" + iChTag.str();    // phiB
		m_plots[hName]->Fill(ph2TpgPhiEmuAm_phiB->at(itr2), ph2TpgPhiHw_phiB->at(itr));

		hName = "Phase2Hw_vs_EmuAm_chi2_" + iChTag.str();    // chi2
		m_plots[hName]->Fill(ph2TpgPhiEmuAm_chi2->at(itr2), ph2TpgPhiHw_chi2->at(itr));

		hName = "Phase2Hw_vs_EmuAm_t0_" + iChTag.str();    // t0
		m_plots[hName]->Fill(ph2TpgPhiEmuAm_t0->at(itr2), ph2TpgPhiHw_t0->at(itr));
     	  
	      }
	    }
	  }
	  // -------- find correlation with EmuHb

	  if(m_ph2TpgPhiHw && m_ph2TpgPhiEmuHb)  { // Madrid and Pozzobon

	    for  (UInt_t  itr2=0; itr2<ph2TpgPhiEmuHb_nTrigs; ++itr2) { // 
	
	      if(ph2TpgPhiEmuHb_sector->at(itr2)==iSec && ph2TpgPhiEmuHb_wheel->at(itr2)==iWh && ph2TpgPhiHw_station->at(itr) == ph2TpgPhiEmuHb_station->at(itr2) ) {  // same wheel, sec, st 
	  
		std::string hName = "Phase2Hw_vs_EmuHb_quality_" + iChTag.str();    // quality
		m_plots[hName]->Fill(ph2TpgPhiEmuHb_quality->at(itr2), ph2TpgPhiHw_quality->at(itr));

		hName = "Phase2Hw_vs_EmuHb_BX_" + iChTag.str();    // BX
		m_plots[hName]->Fill(ph2TpgPhiEmuHb_BX->at(itr2), ph2TpgPhiHw_BX->at(itr));

		hName = "Phase2Hw_vs_EmuHb_phi_" + iChTag.str();    //phi
		m_plots[hName]->Fill(ph2TpgPhiEmuHb_phi->at(itr2), ph2TpgPhiHw_phi->at(itr));

		hName = "Phase2Hw_vs_EmuHb_phiB_" + iChTag.str();    // phiB
		m_plots[hName]->Fill(ph2TpgPhiEmuHb_phiB->at(itr2), ph2TpgPhiHw_phiB->at(itr));

		hName = "Phase2Hw_vs_EmuHb_chi2_" + iChTag.str();    // chi2
		m_plots[hName]->Fill(ph2TpgPhiEmuHb_chi2->at(itr2), ph2TpgPhiHw_chi2->at(itr));

		hName = "Phase2Hw_vs_EmuHb_t0_" + iChTag.str();    // t0
		m_plots[hName]->Fill(ph2TpgPhiEmuHb_t0->at(itr2), ph2TpgPhiHw_t0->at(itr));
     	  
	      }
	    }
	  }
	}
      }
    } // end if(ph2TpgPhiHw)

    //         -------------Phase2EmuHb (Pozzobon) only 

    if(m_ph2TpgPhiEmuHb)  { // Emu Madrid
      
      for  (UInt_t  itr=0; itr<ph2TpgPhiEmuHb_nTrigs; ++itr) {
	
	if(ph2TpgPhiEmuHb_sector->at(itr)==iSec && ph2TpgPhiEmuHb_wheel->at(itr)==iWh) {  // only wheel 2 and sector 12
	  
	  std::stringstream iChTag;    
	  iChTag << "MB" << ph2TpgPhiEmuHb_station->at(itr);  
	  std::string hName = "Phase2EmuHb_BX_" + iChTag.str();    // BX
	  m_plots[hName]->Fill(ph2TpgPhiEmuHb_BX->at(itr));
	  
	  hName = "Phase2EmuHb_quality_" + iChTag.str();    // quality
	  m_plots[hName]->Fill(ph2TpgPhiEmuHb_quality->at(itr));
	  
	  hName = "Phase2EmuHb_phi_" + iChTag.str();    // phi
	  m_plots[hName]->Fill(ph2TpgPhiEmuHb_quality->at(itr)*phi_Ph2_conv);
	  
	  hName = "Phase2EmuHb_phiB_" + iChTag.str();    // phiB
	  m_plots[hName]->Fill(ph2TpgPhiEmuHb_quality->at(itr)*phiB_Ph2_conv);
	  
	  hName = "Phase2EmuHb_chi2_" + iChTag.str();    // chi2
	  m_plots[hName]->Fill(ph2TpgPhiEmuHb_chi2->at(itr));
	  
	  hName = "Phase2EmuHb_t0_" + iChTag.str();    // t0
	  m_plots[hName]->Fill(ph2TpgPhiEmuHb_t0->at(itr));
	  
	  if(ph2TpgPhiEmuHb_BX->at(itr)==BXOK_ph2EmuHb)  {  // -------- when BXOK 
	    
	    hName = "Phase2EmuHb_BXOK_quality_" + iChTag.str();    // quality
	    m_plots[hName]->Fill(ph2TpgPhiEmuHb_quality->at(itr));
	    
	    hName = "Phase2EmuHb_BXOK_phi_" + iChTag.str();    // phi
	    m_plots[hName]->Fill(ph2TpgPhiEmuHb_quality->at(itr)*phi_Ph2_conv);
	    
	    hName = "Phase2EmuHb_BXOK_phiB_" + iChTag.str();    // phiB
	    m_plots[hName]->Fill(ph2TpgPhiEmuHb_quality->at(itr)*phiB_Ph2_conv);
	    
	    hName = "Phase2EmuHb_BXOK_chi2_" + iChTag.str();    // chi2
	    m_plots[hName]->Fill(ph2TpgPhiEmuHb_chi2->at(itr));
	    
	    hName = "Phase2EmuHb_BXOK_t0_" + iChTag.str();    // t0
	    m_plots[hName]->Fill(ph2TpgPhiEmuHb_t0->at(itr));
	    
	  }
	  
	}
      }
    }
    
    //         -------------Phase2Emu Madrid only 

    if(m_ph2TpgPhiEmuAm)  {

      for  (UInt_t  itr=0; itr<ph2TpgPhiEmuAm_nTrigs; ++itr) {
    
	if(ph2TpgPhiEmuAm_sector->at(itr)==iSec && ph2TpgPhiEmuAm_wheel->at(itr)==iWh) {  // only wheel 2 and sector 12
      
	  std::stringstream iChTag;    
	  iChTag << "MB" << ph2TpgPhiEmuAm_station->at(itr);  
	  std::string hName = "Phase2EmuAm_BX_" + iChTag.str();    // BX
	  m_plots[hName]->Fill(ph2TpgPhiEmuAm_BX->at(itr));

	  hName = "Phase2EmuAm_quality_" + iChTag.str();    // quality
	  m_plots[hName]->Fill(ph2TpgPhiEmuAm_quality->at(itr));

	  hName = "Phase2EmuAm_phi_" + iChTag.str();    // phi
	  m_plots[hName]->Fill(ph2TpgPhiEmuAm_quality->at(itr)*phi_Ph2_conv);

	  hName = "Phase2EmuAm_phiB_" + iChTag.str();    // phiB
	  m_plots[hName]->Fill(ph2TpgPhiEmuAm_quality->at(itr)*phiB_Ph2_conv);

	  hName = "Phase2EmuAm_chi2_" + iChTag.str();    // chi2
	  m_plots[hName]->Fill(ph2TpgPhiEmuAm_chi2->at(itr));
      
	  hName = "Phase2EmuAm_t0_" + iChTag.str();    // t0
	  m_plots[hName]->Fill(ph2TpgPhiEmuAm_t0->at(itr));

	  if(ph2TpgPhiEmuAm_BX->at(itr)==BXOK_ph2EmuAm)  {  // -------- when BXOK 

	    hName = "Phase2EmuAm_BXOK_quality_" + iChTag.str();    // quality
	    m_plots[hName]->Fill(ph2TpgPhiEmuAm_quality->at(itr));

	    hName = "Phase2EmuAm_BXOK_phi_" + iChTag.str();    // phi
	    m_plots[hName]->Fill(ph2TpgPhiEmuAm_quality->at(itr)*phi_Ph2_conv);

	    hName = "Phase2EmuAm_BXOK_phiB_" + iChTag.str();    // phiB
	    m_plots[hName]->Fill(ph2TpgPhiEmuAm_quality->at(itr)*phiB_Ph2_conv);

	    hName = "Phase2EmuAm_BXOK_chi2_" + iChTag.str();    // chi2
	    m_plots[hName]->Fill(ph2TpgPhiEmuAm_chi2->at(itr));
      
	    hName = "Phase2EmuAm_BXOK_t0_" + iChTag.str();    // t0
	    m_plots[hName]->Fill(ph2TpgPhiEmuAm_t0->at(itr));

	  }
	}
      }
    }
  
  }
}

void DTNtupleTriggerAnalyzer::endJob()
{
    
  m_outFile.cd();
    
  m_outFile.Write();
  m_outFile.Close();
    
}
  
// -----------------------------------

UInt_t DTNtupleTriggerAnalyzer::getBestSegm(const Int_t stMu,
					    const Int_t secMu,
					    const Int_t whMu
					    )
{
  UInt_t iBest = 9999;
  Int_t nhits = 0;
  Int_t nmin_phihits = 4;


  for(UInt_t iSeg=0; iSeg<seg_nSegments; ++iSeg) {  // request on the tdc segment
    if(seg_wheel->at(iSeg)==whMu && 
       seg_sector->at(iSeg)==secMu && 
       seg_station->at(iSeg)==stMu && 
       seg_hasPhi->at(iSeg)>0 &&
       seg_phi_nHits->at(iSeg)>=nmin_phihits && 
       nhits<=seg_phi_nHits->at(iSeg)) {

      //((dtsegm4D_hasZed->at(iSeg)>0 || dtsegm4D_station->at(iSeg)==4))) {

      // float_t tan_phi = dtsegm4D_x_dir_loc->at(iSeg)/dtsegm4D_z_dir_loc->at(iSeg);
      // float_t  philoc = atan(tan_phi)*180/(pi);
	  
      //   if(fabs(philoc)<80 && nhits<=dtsegm4D_phinhits->at(iSeg)) { // it must be in the BTI angular acceptance
      nhits = seg_phi_nHits->at(iSeg);
      iBest = iSeg;
      //    }
    }
  }
  //  if(iBest<9999) std::cout << " iMB = " << stMu << " iBest = " << iBest << std::endl;;
  return iBest;
}

// ----------------

UInt_t DTNtupleTriggerAnalyzer::getBestTwinMuxOut(const Int_t stMu,
						  const Int_t secMu,
						  const Int_t whMu
						  )
{
    
  UInt_t ibestTwinMuxOut   = 9999;
  Int_t bestTwinMuxOutQual = -1;

  for (UInt_t iTwinMuxOut = 0; iTwinMuxOut < ltTwinMuxOut_nTrigs; ++iTwinMuxOut)
    {
      
      Int_t whTwinMuxOut  = ltTwinMuxOut_wheel->at(iTwinMuxOut);
      Int_t secTwinMuxOut = ltTwinMuxOut_sector->at(iTwinMuxOut);
      Int_t stTwinMuxOut  = ltTwinMuxOut_station->at(iTwinMuxOut);
      // Int_t bxTwinMuxOut  = ltTwinMuxOut_BX->at(iTwinMuxOut);
      // Int_t phiTwinMuxOut  = ltTwinMuxOut_phi->at(iTwinMuxOut);
      // Int_t phiBTwinMuxOut  = ltTwinMuxOut_phiB->at(iTwinMuxOut);
      Int_t qualityTwinMuxOut  = ltTwinMuxOut_quality->at(iTwinMuxOut);
      //      Int_t rpcbitTwinMuxOut  = ltTwinMuxOut_rpcbit->at(iTwinMuxOut);

      if(stTwinMuxOut ==4 && secTwinMuxOut==13)  {secTwinMuxOut=4;}
      else if(stTwinMuxOut ==4 && secTwinMuxOut==14)  {secTwinMuxOut=10;}
      
      if(whMu  == whTwinMuxOut   &&
	 secMu == secTwinMuxOut  &&
	 stMu  == stTwinMuxOut   && 
	 //	 bxTwinMuxOut == 0       &&
	 qualityTwinMuxOut > bestTwinMuxOutQual
	 )
	
	{
	  ibestTwinMuxOut = iTwinMuxOut;
	  bestTwinMuxOutQual = qualityTwinMuxOut;	  
	}
    }
  return ibestTwinMuxOut;    
}
// ----------------

UInt_t DTNtupleTriggerAnalyzer::getBestTwinMuxOutBXOK(const Int_t stMu,
						      const Int_t secMu,
						      const Int_t whMu
						      )
{
    
  UInt_t ibestTwinMuxOut   = 9999;
  Int_t bestTwinMuxOutQual = -1;

  for (UInt_t iTwinMuxOut = 0; iTwinMuxOut < ltTwinMuxOut_nTrigs; ++iTwinMuxOut)
    {
      
      Int_t whTwinMuxOut  = ltTwinMuxOut_wheel->at(iTwinMuxOut);
      Int_t secTwinMuxOut = ltTwinMuxOut_sector->at(iTwinMuxOut);
      Int_t stTwinMuxOut  = ltTwinMuxOut_station->at(iTwinMuxOut);
      Int_t bxTwinMuxOut  = ltTwinMuxOut_BX->at(iTwinMuxOut);
      // Int_t phiTwinMuxOut  = ltTwinMuxOut_phi->at(iTwinMuxOut);
      // Int_t phiBTwinMuxOut  = ltTwinMuxOut_phiB->at(iTwinMuxOut);
      Int_t qualityTwinMuxOut  = ltTwinMuxOut_quality->at(iTwinMuxOut);
      //      Int_t rpcbitTwinMuxOut  = ltTwinMuxOut_rpcbit->at(iTwinMuxOut);

      if(stTwinMuxOut ==4 && secTwinMuxOut==13)  {secTwinMuxOut=4;}
      else if(stTwinMuxOut ==4 && secTwinMuxOut==14)  {secTwinMuxOut=10;}
      
      if(whMu  == whTwinMuxOut   &&
	 secMu == secTwinMuxOut  &&
	 stMu  == stTwinMuxOut   && 
	 bxTwinMuxOut == 0       &&
	 qualityTwinMuxOut > bestTwinMuxOutQual
	 )
	
	{
	  ibestTwinMuxOut = iTwinMuxOut;
	  bestTwinMuxOutQual = qualityTwinMuxOut;	  
	}
    }
  return ibestTwinMuxOut;    
}

// ----------------

UInt_t DTNtupleTriggerAnalyzer::getBestTpgPhiHw(const Int_t stMu,
						const Int_t secMu,
						const Int_t whMu
						)
{
    
  UInt_t ibestTpgPhiHw   = 9999;
  Int_t bestTpgPhiHwQual = -1;

  for (UInt_t iTpgPhiHw = 0; iTpgPhiHw < ph2TpgPhiHw_nTrigs; ++iTpgPhiHw)
    {
      
      Int_t whTpgPhiHw  = ph2TpgPhiHw_wheel->at(iTpgPhiHw);
      Int_t secTpgPhiHw = ph2TpgPhiHw_sector->at(iTpgPhiHw);
      Int_t stTpgPhiHw  = ph2TpgPhiHw_station->at(iTpgPhiHw);
      // Int_t bxTpgPhiHw  = ph2TpgPhiHw_BX->at(iTpgPhiHw);
      // Int_t phiTpgPhiHw  = ph2TpgPhiHw_phi->at(iTpgPhiHw);
      // Int_t phiBTpgPhiHw  = ph2TpgPhiHw_phiB->at(iTpgPhiHw);
      Int_t qualityTpgPhiHw  = ph2TpgPhiHw_quality->at(iTpgPhiHw);
      //      Int_t rpcbitTpgPhiHw  = ph2TpgPhiHw_rpcbit->at(iTpgPhiHw);

      // if(stTpgPhiHw ==4 && secTpgPhiHw==13)  {secTpgPhiHw=4;}
      // else if(stTpgPhiHw ==4 && secTpgPhiHw==14)  {secTpgPhiHw=10;}
      
      if(whMu  == whTpgPhiHw   &&
	 secMu == secTpgPhiHw  &&
	 stMu  == stTpgPhiHw   && 
	 //	 bxTpgPhiHw == 0       &&
	 qualityTpgPhiHw > bestTpgPhiHwQual
	 )
	
	{
	  ibestTpgPhiHw = iTpgPhiHw;
	  bestTpgPhiHwQual = qualityTpgPhiHw;	  
	}
    }
  return ibestTpgPhiHw;    
}

// --------------------

UInt_t DTNtupleTriggerAnalyzer::getBestTpgPhiHwBXOK(const Int_t stMu,
						    const Int_t secMu,
						    const Int_t whMu
						    )
{
    
  UInt_t ibestTpgPhiHw   = 9999;
  Int_t bestTpgPhiHwQual = -1;

  for (UInt_t iTpgPhiHw = 0; iTpgPhiHw < ph2TpgPhiHw_nTrigs; ++iTpgPhiHw)
    {
      
      Int_t whTpgPhiHw  = ph2TpgPhiHw_wheel->at(iTpgPhiHw);
      Int_t secTpgPhiHw = ph2TpgPhiHw_sector->at(iTpgPhiHw);
      Int_t stTpgPhiHw  = ph2TpgPhiHw_station->at(iTpgPhiHw);
      Int_t bxTpgPhiHw  = ph2TpgPhiHw_BX->at(iTpgPhiHw);
      // Int_t phiTpgPhiHw  = ph2TpgPhiHw_phi->at(iTpgPhiHw);
      // Int_t phiBTpgPhiHw  = ph2TpgPhiHw_phiB->at(iTpgPhiHw);
      Int_t qualityTpgPhiHw  = ph2TpgPhiHw_quality->at(iTpgPhiHw);
      //      Int_t rpcbitTpgPhiHw  = ph2TpgPhiHw_rpcbit->at(iTpgPhiHw);

      // if(stTpgPhiHw ==4 && secTpgPhiHw==13)  {secTpgPhiHw=4;}
      // else if(stTpgPhiHw ==4 && secTpgPhiHw==14)  {secTpgPhiHw=10;}
      
      if(whMu  == whTpgPhiHw   &&
	 secMu == secTpgPhiHw  &&
	 stMu  == stTpgPhiHw   && 
	 bxTpgPhiHw == 0       && // -269
	 qualityTpgPhiHw > bestTpgPhiHwQual
	 )
	
	{
	  ibestTpgPhiHw = iTpgPhiHw;
	  bestTpgPhiHwQual = qualityTpgPhiHw;	  
	}
    }
  return ibestTpgPhiHw;    
}

// --------------------

UInt_t DTNtupleTriggerAnalyzer::getBestTpgPhiEmuAm(const Int_t stMu,
						   const Int_t secMu,
						   const Int_t whMu
						   )
{
    
  UInt_t ibestTpgPhiEmuAm   = 9999;
  Int_t bestTpgPhiEmuAmQual = -1;

  for (UInt_t iTpgPhiEmuAm = 0; iTpgPhiEmuAm < ph2TpgPhiEmuAm_nTrigs; ++iTpgPhiEmuAm)
    {
      
      Int_t whTpgPhiEmuAm  = ph2TpgPhiEmuAm_wheel->at(iTpgPhiEmuAm);
      Int_t secTpgPhiEmuAm = ph2TpgPhiEmuAm_sector->at(iTpgPhiEmuAm);
      Int_t stTpgPhiEmuAm  = ph2TpgPhiEmuAm_station->at(iTpgPhiEmuAm);
      //  Int_t bxTpgPhiEmuAm  = ph2TpgPhiEmuAm_BX->at(iTpgPhiEmuAm);
      // Int_t phiTpgPhiEmuAm  = ph2TpgPhiEmuAm_phi->at(iTpgPhiEmuAm);
      // Int_t phiBTpgPhiEmuAm  = ph2TpgPhiEmuAm_phiB->at(iTpgPhiEmuAm);
      Int_t qualityTpgPhiEmuAm  = ph2TpgPhiEmuAm_quality->at(iTpgPhiEmuAm);
      //      Int_t rpcbitTpgPhiEmuAm  = ph2TpgPhiEmuAm_rpcbit->at(iTpgPhiEmuAm);

      // if(stTpgPhiEmuAm ==4 && secTpgPhiEmuAm==13)  {secTpgPhiEmuAm=4;}
      // else if(stTpgPhiEmuAm ==4 && secTpgPhiEmuAm==14)  {secTpgPhiEmuAm=10;}
      
      if(whMu  == whTpgPhiEmuAm   &&
	 secMu == secTpgPhiEmuAm  &&
	 stMu  == stTpgPhiEmuAm   && 
	 //	 bxTpgPhiEmuAm == 0       &&
	 qualityTpgPhiEmuAm > bestTpgPhiEmuAmQual
	 )
	
	{
	  ibestTpgPhiEmuAm = iTpgPhiEmuAm;
	  bestTpgPhiEmuAmQual = qualityTpgPhiEmuAm;	  
	}
    }
  return ibestTpgPhiEmuAm;    
}

// --------------------

UInt_t DTNtupleTriggerAnalyzer::getBestTpgPhiEmuAmBXOK(const Int_t stMu,
						       const Int_t secMu,
						       const Int_t whMu
						       )
{
    
  UInt_t ibestTpgPhiEmuAm   = 9999;
  Int_t bestTpgPhiEmuAmQual = -1;

  for (UInt_t iTpgPhiEmuAm = 0; iTpgPhiEmuAm < ph2TpgPhiEmuAm_nTrigs; ++iTpgPhiEmuAm)
    {
      
      Int_t whTpgPhiEmuAm  = ph2TpgPhiEmuAm_wheel->at(iTpgPhiEmuAm);
      Int_t secTpgPhiEmuAm = ph2TpgPhiEmuAm_sector->at(iTpgPhiEmuAm);
      Int_t stTpgPhiEmuAm  = ph2TpgPhiEmuAm_station->at(iTpgPhiEmuAm);
      Int_t bxTpgPhiEmuAm  = ph2TpgPhiEmuAm_BX->at(iTpgPhiEmuAm);
      // Int_t phiTpgPhiEmuAm  = ph2TpgPhiEmuAm_phi->at(iTpgPhiEmuAm);
      // Int_t phiBTpgPhiEmuAm  = ph2TpgPhiEmuAm_phiB->at(iTpgPhiEmuAm);
      Int_t qualityTpgPhiEmuAm  = ph2TpgPhiEmuAm_quality->at(iTpgPhiEmuAm);
      //      Int_t rpcbitTpgPhiEmuAm  = ph2TpgPhiEmuAm_rpcbit->at(iTpgPhiEmuAm);

      // if(stTpgPhiEmuAm ==4 && secTpgPhiEmuAm==13)  {secTpgPhiEmuAm=4;}
      // else if(stTpgPhiEmuAm ==4 && secTpgPhiEmuAm==14)  {secTpgPhiEmuAm=10;}
      
      if(whMu  == whTpgPhiEmuAm   &&
	 secMu == secTpgPhiEmuAm  &&
	 stMu  == stTpgPhiEmuAm   && 
	 bxTpgPhiEmuAm == 0       &&
	 qualityTpgPhiEmuAm > bestTpgPhiEmuAmQual
	 )
	
	{
	  ibestTpgPhiEmuAm = iTpgPhiEmuAm;
	  bestTpgPhiEmuAmQual = qualityTpgPhiEmuAm;	  
	}
    }
  return ibestTpgPhiEmuAm;    
}

// --------------------

UInt_t DTNtupleTriggerAnalyzer::getBestTpgPhiEmuHb(const Int_t stMu,
						   const Int_t secMu,
						   const Int_t whMu
						   )
{
    
  UInt_t ibestTpgPhiEmuHb   = 9999;
  Int_t bestTpgPhiEmuHbQual = -1;

  for (UInt_t iTpgPhiEmuHb = 0; iTpgPhiEmuHb < ph2TpgPhiEmuHb_nTrigs; ++iTpgPhiEmuHb)
    {
      
      Int_t whTpgPhiEmuHb  = ph2TpgPhiEmuHb_wheel->at(iTpgPhiEmuHb);
      Int_t secTpgPhiEmuHb = ph2TpgPhiEmuHb_sector->at(iTpgPhiEmuHb);
      Int_t stTpgPhiEmuHb  = ph2TpgPhiEmuHb_station->at(iTpgPhiEmuHb);
      //  Int_t bxTpgPhiEmuHb  = ph2TpgPhiEmuHb_BX->at(iTpgPhiEmuHb);
      // Int_t phiTpgPhiEmuHb  = ph2TpgPhiEmuHb_phi->at(iTpgPhiEmuHb);
      // Int_t phiBTpgPhiEmuHb  = ph2TpgPhiEmuHb_phiB->at(iTpgPhiEmuHb);
      Int_t qualityTpgPhiEmuHb  = ph2TpgPhiEmuHb_quality->at(iTpgPhiEmuHb);
      //      Int_t rpcbitTpgPhiEmuHb  = ph2TpgPhiEmuHb_rpcbit->at(iTpgPhiEmuHb);

      // if(stTpgPhiEmuHb ==4 && secTpgPhiEmuHb==13)  {secTpgPhiEmuHb=4;}
      // else if(stTpgPhiEmuHb ==4 && secTpgPhiEmuHb==14)  {secTpgPhiEmuHb=10;}
      
      if(whMu  == whTpgPhiEmuHb   &&
	 secMu == secTpgPhiEmuHb  &&
	 stMu  == stTpgPhiEmuHb   && 
	 //	 bxTpgPhiEmuHb == 0       &&
	 qualityTpgPhiEmuHb > bestTpgPhiEmuHbQual
	 )
	
	{
	  ibestTpgPhiEmuHb = iTpgPhiEmuHb;
	  bestTpgPhiEmuHbQual = qualityTpgPhiEmuHb;	  
	}
    }
  return ibestTpgPhiEmuHb;    
}


