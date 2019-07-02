#ifndef DTNtupleTriggerAnalyzer_h
#define DTNtupleTriggerAnalyzer_h

#include "DTNtupleBaseAnalyzer.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"

#include <TStyle.h>
#include <TCanvas.h>
#include <fstream>
#include <TMath.h>
#include <TEfficiency.h>

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <map>

class DTNtupleTriggerAnalyzer : public DTNtupleBaseAnalyzer 
{

 public:
  
  DTNtupleTriggerAnalyzer( const TString & inFileName,
			   const TString & outFileName
			   );

  ~DTNtupleTriggerAnalyzer();
  
  void virtual Loop() override;

 protected:

  void book();
  void fill();
  void endJob();
  
  TFile m_outFile;

  UInt_t getBestSegm( const Int_t muSt,
		      const Int_t muSec,
		      const Int_t muWh
		      );

  UInt_t getBestTwinMuxOut( const Int_t muSt,
			    const Int_t muSec,
			    const Int_t muWh
			    );

  UInt_t getBestTwinMuxOutBXOK( const Int_t muSt,
				const Int_t muSec,
				const Int_t muWh
				);
  
  UInt_t getBestTpgPhiHw( const Int_t muSt,
			  const Int_t muSec,
			  const Int_t muWh
			  );
  
  UInt_t getBestTpgPhiHwBXOK( const Int_t muSt,
			      const Int_t muSec,
			      const Int_t muWh
			      );
  
  UInt_t getBestTpgPhiEmuAm( const Int_t muSt,
			     const Int_t muSec,
			     const Int_t muWh
			     );
  
  UInt_t getBestTpgPhiEmuAmBXOK( const Int_t muSt,
				 const Int_t muSec,
				 const Int_t muWh
				 );
  
  UInt_t getBestTpgPhiEmuHb( const Int_t muSt,
			     const Int_t muSec,
			     const Int_t muWh
			     );
  
  
  // Boolean variables to switch on/off parts of the code for the analysis
  
  Bool_t m_twinMux = true;   
  Bool_t m_ph2TpgPhiHw = true;
  Bool_t m_ph2TpgPhiEmuAm = true;
  Bool_t m_ph2TpgPhiEmuHb = false;
  Bool_t m_segments = true;
  
  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TEfficiency*> m_effs; 
  
};

  

#endif
