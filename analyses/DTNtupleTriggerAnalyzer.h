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
  
  virtual void Loop() override;

 protected:

  virtual void book() override;
  virtual void fill() override;
  virtual void endJob() override;
  
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
  
  Bool_t m_twinMux;   
  Bool_t m_ph2TpgPhiHw;
  Bool_t m_ph2TpgPhiEmuAm;
  Bool_t m_ph2TpgPhiEmuHb;
  Bool_t m_segments;

  // definition of other variables hat are not changed in the analysis

  Int_t iWh;
  Int_t iSec;
  Int_t iPhihits_min;

  Double_t pi;
  
  Double_t phi_Ph1_conv;  // conversion from trig phi to phi in rad in Phase1
  Double_t phiB_Ph1_conv;  // conversion from trig phiB to phiB in rad in Phase1
 

  Double_t phi_Ph2_conv; // in cm, local position in the station
  Double_t phiB_Ph2_conv; // to transform in radians
  Double_t phi_offset;

  Int_t BXOK_TwinMuxOut;
  Int_t BXOK_ph2Hw;  // -269
  Int_t BXOK_ph2EmuHb; // to be properly set
  Int_t BXOK_ph2EmuAm; // to be properly set

  Long64_t nMaxEvents;
  
  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TEfficiency*> m_effs; 
  
};

  

#endif
