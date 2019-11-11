#ifndef DTTnPBaseAnalysis_h
#define DTTnPBaseAnalysis_h

#include "DTNtupleBaseAnalyzer.h"

#include "TFile.h"
#include "TEfficiency.h"
#include "TH1F.h"
#include "TH2F.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <map>

class DTNtupleSegmentAnalyzer : public DTNtupleBaseAnalyzer 
{

 public:
  
  DTNtupleSegmentAnalyzer(const TString & inFileName,
			  const TString & outFileName);
  ~DTNtupleSegmentAnalyzer();

  virtual void Loop() override;
  void PreLoop(string Tag);

  string m_deadFileName;
  
  void MeasureEfficiency(string Tag,Long64_t jEntry,
			 int Ndead,
			 int dead[][6],
			 UInt_t nSegments,
			 vector<short> *hasPhi, 
			 vector<short> *station, 
			 vector<short> *wheel, 
			 vector<short> *sector,  
			 vector<short> *hasZed,  
			 vector<short> *phi_nHits, 
			 vector<short> *z_nHits,
			 vector<float> *dirLoc_x, 
			 TClonesArray  *hitsExpWire,
			 TClonesArray  *phiHits_pos,
			 TClonesArray  *phiHits_posCh,
			 TClonesArray  *phiHits_posErr,
			 TClonesArray  *phiHits_side,
			 TClonesArray  *phiHits_wire,
			 TClonesArray  *phiHits_wirePos,
			 TClonesArray  *phiHits_layer,
			 TClonesArray  *phiHits_superLayer,
			 TClonesArray  *zHits_pos,
			 TClonesArray  *zHits_posCh,
			 TClonesArray  *zHits_posErr,
			 TClonesArray  *zHits_side,
			 TClonesArray  *zHits_wire,
			 TClonesArray  *zHits_wirePos,
			 TClonesArray  *zHits_layer,
			 UInt_t         Digi_nDigis,
			 vector<short> *Digi_wheel,
			 vector<short> *Digi_sector,
			 vector<short> *Digi_station,
			 vector<short> *Digi_superLayer,
			 vector<short> *Digi_layer,
			 vector<short> *Digi_wire,
			 vector<float> *Digi_time
			 );


  void ComputeResidual(string Tag,
		       Long64_t jEntry, 
		       UInt_t nSegments,
		       vector<short> *hasPhi,
		       vector<short> *station,
		       vector<short> *wheel, 
		       vector<short> *sector,  
		       vector<short> *hasZed,  
		       vector<short> *phi_nHits, 
		       vector<short> *z_nHits,
		       vector<float> *phi_normChi2,
		       vector<float> *z_normChi2,
		       TClonesArray  *hitsExpWire,
		       TClonesArray  *hitsExpPos,
		       TClonesArray  *hitsExpPosCh,
		       TClonesArray  *phiHits_pos,
		       TClonesArray  *phiHits_posCh,
		       TClonesArray  *phiHits_posErr,
		       TClonesArray  *phiHits_side,
		       TClonesArray  *phiHits_wire,
		       TClonesArray  *phiHits_wirePos,
		       TClonesArray  *phiHits_layer,
		       TClonesArray  *phiHits_superLayer,
		       TClonesArray  *zHits_pos,
		       TClonesArray  *zHits_posCh,
		       TClonesArray  *zHits_posErr,
		       TClonesArray  *zHits_side,
		       TClonesArray  *zHits_wire,
		       TClonesArray  *zHits_wirePos,
		       TClonesArray  *zHits_layer
		       );



  
 protected:

  virtual void book() override;
  virtual void fill() override;
  virtual void endJob() override;
  
  TFile m_outFile;
  
  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TH2*> m_2Dplots;
  std::map<std::string, TEfficiency*> m_effs;
  
};

#endif
