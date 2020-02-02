//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue May 28 08:51:30 2019 by ROOT version 6.12/06
// from TTree DTTREE/DT Tree
// found on file: DTDPGNtuple_10_3_3_ZMuSkim_2018D.root
//////////////////////////////////////////////////////////

#ifndef DTNtupleBaseAnalyzer_h
#define DTNtupleBaseAnalyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TVector.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "TClonesArray.h"

class DTNtupleDigi
{

 public:

  DTNtupleDigi ()
    {
      nDigis = nullptr;
      wheel  = nullptr;
      sector = nullptr;
      station = nullptr;
      superLayer = nullptr;
      layer = nullptr;
      wire = nullptr;
      time = nullptr;
    };

  DTNtupleDigi (UInt_t & nDigis,
	  vector<short> * wheel,
	  vector<short> * sector,
	  vector<short> * station,
	  vector<short> * superLayer,
	  vector<short> * layer,
	  vector<short> * wire,
	  vector<float> * time) :
  nDigis(&nDigis), wheel(wheel), sector(sector), station(station),
    superLayer(superLayer), layer(layer), wire(wire), time(time)
  { };

  ~DTNtupleDigi() { };

  UInt_t * nDigis;
  vector<short> * wheel;
  vector<short> * sector;
  vector<short> * station;
  vector<short> * superLayer;
  vector<short> * layer;
  vector<short> * wire;
  vector<float> * time;

};

class DTNtupleSegment
{

 public:

  DTNtupleSegment ()
    {
      wheel = nullptr;
      sector = nullptr;
      station = nullptr;
      hasPhi = nullptr;
      hasZed = nullptr;
      
      posLoc_x = nullptr;
      posLoc_y = nullptr;
      posLoc_z = nullptr;
      dirLoc_x = nullptr;
      dirLoc_y = nullptr;
      dirLoc_z = nullptr;
      posLoc_x_SL1 = nullptr;
      posLoc_x_SL3 = nullptr;
      posLoc_x_midPlane = nullptr;
      posGlb_phi = nullptr;
      posGlb_eta = nullptr;
      dirGlb_phi = nullptr;
      dirGlb_eta = nullptr;
      
      hitsExpPos = nullptr;
      hitsExpPosCh = nullptr;
      hitsExpWire = nullptr;
      
      phi_t0 = nullptr;
      phi_vDrift = nullptr;
      phi_normChi2 = nullptr;
      phi_nHits = nullptr;
      
      phiHits_pos = nullptr;
      phiHits_posCh = nullptr;
      phiHits_posErr = nullptr;
      phiHits_side = nullptr;
      phiHits_wire = nullptr;
      phiHits_wirePos = nullptr;
      phiHits_layer = nullptr;
      phiHits_superLayer = nullptr;
      phiHits_time = nullptr;
      phiHits_timeCali = nullptr;
      
      z_normChi2 = nullptr;
      z_nHits = nullptr;
      
      zHits_pos = nullptr;
      zHits_posCh = nullptr;
      zHits_posErr = nullptr;
      zHits_side = nullptr;
      zHits_wire = nullptr;
      zHits_wirePos = nullptr;
      zHits_layer = nullptr;
      zHits_time = nullptr;
      zHits_timeCali = nullptr;

    };
  
  DTNtupleSegment (UInt_t & nSegments,  
		   vector<short>   *wheel,
		   vector<short>   *sector,
		   vector<short>   *station,
		   vector<short>   *hasPhi,
		   vector<short>   *hasZed,
		   
		   vector<float>   *posLoc_x,
		   vector<float>   *posLoc_y,
		   vector<float>   *posLoc_z,
		   vector<float>   *dirLoc_x,
		   vector<float>   *dirLoc_y,
		   vector<float>   *dirLoc_z,
		   vector<float>   *posLoc_x_SL1,
		   vector<float>   *posLoc_x_SL3,
		   vector<float>   *posLoc_x_midPlane,
		   vector<float>   *posGlb_phi,
		   vector<float>   *posGlb_eta,
		   vector<float>   *dirGlb_phi,
		   vector<float>   *dirGlb_eta,
		   
		   TClonesArray    *hitsExpPos,
		   TClonesArray    *hitsExpPosCh,
		   TClonesArray    *hitsExpWire,
		   
		   vector<float>   *phi_t0,
		   vector<float>   *phi_vDrift,
		   vector<float>   *phi_normChi2,
		   vector<short>   *phi_nHits,
		   
		   TClonesArray    *phiHits_pos,
		   TClonesArray    *phiHits_posCh,
		   TClonesArray    *phiHits_posErr,
		   TClonesArray    *phiHits_side,
		   TClonesArray    *phiHits_wire,
		   TClonesArray    *phiHits_wirePos,
		   TClonesArray    *phiHits_layer,
		   TClonesArray    *phiHits_superLayer,
		   TClonesArray    *phiHits_time,
		   TClonesArray    *phiHits_timeCali,
		   
		   vector<float>   *z_normChi2,
		   vector<short>   *z_nHits,
		   
		   TClonesArray    *zHits_pos,
		   TClonesArray    *zHits_posCh,
		   TClonesArray    *zHits_posErr,
		   TClonesArray    *zHits_side,
		   TClonesArray    *zHits_wire,
		   TClonesArray    *zHits_wirePos,
		   TClonesArray    *zHits_layer,
		   TClonesArray    *zHits_time,
		   TClonesArray    *zHits_timeCali) :
    nSegments(&nSegments), wheel(wheel), sector(sector), station(station), hasPhi(hasPhi),
    hasZed(hasZed), posLoc_x(posLoc_x), posLoc_y(posLoc_y), posLoc_z(posLoc_z), dirLoc_x(dirLoc_x),
    dirLoc_y(dirLoc_y), dirLoc_z(dirLoc_z), posLoc_x_SL1(posLoc_x_SL1), posLoc_x_SL3(posLoc_x_SL3),
    posLoc_x_midPlane(posLoc_x_midPlane), posGlb_phi(posGlb_phi), posGlb_eta(posGlb_eta),
    dirGlb_phi(dirGlb_phi), dirGlb_eta(dirGlb_eta), hitsExpPos(hitsExpPos),
    hitsExpPosCh(hitsExpPosCh), hitsExpWire(hitsExpWire), phi_t0(phi_t0), phi_vDrift(phi_vDrift),
    phi_normChi2(phi_normChi2), phi_nHits(phi_nHits), phiHits_pos(phiHits_pos),
    phiHits_posCh(phiHits_posCh), phiHits_posErr(phiHits_posErr), phiHits_side(phiHits_side),
    phiHits_wire(phiHits_wire), phiHits_wirePos(phiHits_wirePos), phiHits_layer(phiHits_layer),
    phiHits_superLayer(phiHits_superLayer), phiHits_time(phiHits_time),
    phiHits_timeCali(phiHits_timeCali), z_normChi2(z_normChi2), z_nHits(z_nHits),
    zHits_pos(zHits_pos), zHits_posCh(zHits_posCh), zHits_posErr(zHits_posErr),
    zHits_side(zHits_side), zHits_wire(zHits_wire), zHits_wirePos(zHits_wirePos),
    zHits_layer(zHits_layer), zHits_time(zHits_time), zHits_timeCali(zHits_timeCali)
  { };

  ~DTNtupleSegment() { };

  UInt_t * nSegments;
  
  vector<short>   *wheel;
  vector<short>   *sector;
  vector<short>   *station;
  vector<short>   *hasPhi;
  vector<short>   *hasZed;

  vector<float>   *posLoc_x;
  vector<float>   *posLoc_y;
  vector<float>   *posLoc_z;
  vector<float>   *dirLoc_x;
  vector<float>   *dirLoc_y;
  vector<float>   *dirLoc_z;
  vector<float>   *posLoc_x_SL1;
  vector<float>   *posLoc_x_SL3;
  vector<float>   *posLoc_x_midPlane;
  vector<float>   *posGlb_phi;
  vector<float>   *posGlb_eta;
  vector<float>   *dirGlb_phi;
  vector<float>   *dirGlb_eta;
  
  TClonesArray    *hitsExpPos;
  TClonesArray    *hitsExpPosCh;
  TClonesArray    *hitsExpWire;
  
  vector<float>   *phi_t0;
  vector<float>   *phi_vDrift;
  vector<float>   *phi_normChi2;
  vector<short>   *phi_nHits;
  
  TClonesArray    *phiHits_pos;
  TClonesArray    *phiHits_posCh;
  TClonesArray    *phiHits_posErr;
  TClonesArray    *phiHits_side;
  TClonesArray    *phiHits_wire;
  TClonesArray    *phiHits_wirePos;
  TClonesArray    *phiHits_layer;
  TClonesArray    *phiHits_superLayer;
  TClonesArray    *phiHits_time;
  TClonesArray    *phiHits_timeCali;
  
  vector<float>   *z_normChi2;
  vector<short>   *z_nHits;
  
  TClonesArray    *zHits_pos;
  TClonesArray    *zHits_posCh;
  TClonesArray    *zHits_posErr;
  TClonesArray    *zHits_side;
  TClonesArray    *zHits_wire;
  TClonesArray    *zHits_wirePos;
  TClonesArray    *zHits_layer;
  TClonesArray    *zHits_time;
  TClonesArray    *zHits_timeCali;

};

class DTNtupleBaseAnalyzer {

public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   TFile m_inFile;

   std::map<std::string, DTNtupleDigi>    digiObjs;
   std::map<std::string, DTNtupleSegment> segmentObjs;

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           event_runNumber;
   Int_t           event_lumiBlock;
   Long64_t        event_eventNumber;
   ULong64_t       event_timeStamp;
   Int_t           event_bunchCrossing;
   Long64_t        event_orbitNumber;
   UInt_t          gen_nGenParts;
   vector<int>     *gen_pdgId;
   vector<float>   *gen_pt;
   vector<float>   *gen_phi;
   vector<float>   *gen_eta;
   vector<short>   *gen_charge;
   Short_t         environment_truePileUp;
   Short_t         environment_actualPileUp;
   Int_t           environment_instLumi;
   Short_t         environment_nPV;
   Float_t         environment_pv_x;
   Float_t         environment_pv_y;
   Float_t         environment_pv_z;
   Float_t         environment_pv_xxErr;
   Float_t         environment_pv_yyErr;
   Float_t         environment_pv_zzErr;
   Float_t         environment_pv_xyErr;
   Float_t         environment_pv_xzErr;
   Float_t         environment_pv_yzErr;
   UInt_t          digi_nDigis;
   vector<short>   *digi_wheel;
   vector<short>   *digi_sector;
   vector<short>   *digi_station;
   vector<short>   *digi_superLayer;
   vector<short>   *digi_layer;
   vector<short>   *digi_wire;
   vector<float>   *digi_time;
   UInt_t          ph2Digi_nDigis;
   vector<short>   *ph2Digi_wheel;
   vector<short>   *ph2Digi_sector;
   vector<short>   *ph2Digi_station;
   vector<short>   *ph2Digi_superLayer;
   vector<short>   *ph2Digi_layer;
   vector<short>   *ph2Digi_wire;
   vector<float>   *ph2Digi_time;
   UInt_t          seg_nSegments;
   vector<short>   *seg_wheel;
   vector<short>   *seg_sector;
   vector<short>   *seg_station;
   vector<short>   *seg_hasPhi;
   vector<short>   *seg_hasZed;
   vector<float>   *seg_posLoc_x;
   vector<float>   *seg_posLoc_y;
   vector<float>   *seg_posLoc_z;
   vector<float>   *seg_dirLoc_x;
   vector<float>   *seg_dirLoc_y;
   vector<float>   *seg_dirLoc_z;
   vector<float>   *seg_posLoc_x_SL1;
   vector<float>   *seg_posLoc_x_SL3;
   vector<float>   *seg_posLoc_x_midPlane;
   vector<float>   *seg_posGlb_phi;
   vector<float>   *seg_posGlb_eta;
   vector<float>   *seg_dirGlb_phi;
   vector<float>   *seg_dirGlb_eta;
   TClonesArray    *seg_hitsExpPos;
   TClonesArray    *seg_hitsExpPosCh;
   TClonesArray    *seg_hitsExpWire;
   vector<float>   *seg_phi_t0;
   vector<float>   *seg_phi_vDrift;
   vector<float>   *seg_phi_normChi2;
   vector<short>   *seg_phi_nHits;
   TClonesArray    *seg_phiHits_pos;
   TClonesArray    *seg_phiHits_posCh;
   TClonesArray    *seg_phiHits_posErr;
   TClonesArray    *seg_phiHits_side;
   TClonesArray    *seg_phiHits_wire;
   TClonesArray    *seg_phiHits_wirePos;
   TClonesArray    *seg_phiHits_layer;
   TClonesArray    *seg_phiHits_superLayer;
   TClonesArray    *seg_phiHits_time;
   TClonesArray    *seg_phiHits_timeCali;
   vector<float>   *seg_z_normChi2;
   vector<short>   *seg_z_nHits;
   TClonesArray    *seg_zHits_pos;
   TClonesArray    *seg_zHits_posCh;
   TClonesArray    *seg_zHits_posErr;
   TClonesArray    *seg_zHits_side;
   TClonesArray    *seg_zHits_wire;
   TClonesArray    *seg_zHits_wirePos;
   TClonesArray    *seg_zHits_layer;
   TClonesArray    *seg_zHits_time;
   TClonesArray    *seg_zHits_timeCali;
   UInt_t          ph2Seg_nSegments;
   vector<short>   *ph2Seg_wheel;
   vector<short>   *ph2Seg_sector;
   vector<short>   *ph2Seg_station;
   vector<short>   *ph2Seg_hasPhi;
   vector<short>   *ph2Seg_hasZed;
   vector<float>   *ph2Seg_posLoc_x;
   vector<float>   *ph2Seg_posLoc_y;
   vector<float>   *ph2Seg_posLoc_z;
   vector<float>   *ph2Seg_dirLoc_x;
   vector<float>   *ph2Seg_dirLoc_y;
   vector<float>   *ph2Seg_dirLoc_z;
   vector<float>   *ph2Seg_posLoc_x_SL1;
   vector<float>   *ph2Seg_posLoc_x_SL3;
   vector<float>   *ph2Seg_posLoc_x_midPlane;
   vector<float>   *ph2Seg_posGlb_phi;
   vector<float>   *ph2Seg_posGlb_eta;
   vector<float>   *ph2Seg_dirGlb_phi;
   vector<float>   *ph2Seg_dirGlb_eta;
   TClonesArray    *ph2Seg_hitsExpPos;
   TClonesArray    *ph2Seg_hitsExpPosCh;
   TClonesArray    *ph2Seg_hitsExpWire;
   vector<float>   *ph2Seg_phi_t0;
   vector<float>   *ph2Seg_phi_vDrift;
   vector<float>   *ph2Seg_phi_normChi2;
   vector<short>   *ph2Seg_phi_nHits;
   TClonesArray    *ph2Seg_phiHits_pos;
   TClonesArray    *ph2Seg_phiHits_posCh;
   TClonesArray    *ph2Seg_phiHits_posErr;
   TClonesArray    *ph2Seg_phiHits_side;
   TClonesArray    *ph2Seg_phiHits_wire;
   TClonesArray    *ph2Seg_phiHits_wirePos;
   TClonesArray    *ph2Seg_phiHits_layer;
   TClonesArray    *ph2Seg_phiHits_superLayer;
   TClonesArray    *ph2Seg_phiHits_time;
   TClonesArray    *ph2Seg_phiHits_timeCali;
   vector<float>   *ph2Seg_z_normChi2;
   vector<short>   *ph2Seg_z_nHits;
   TClonesArray    *ph2Seg_zHits_pos;
   TClonesArray    *ph2Seg_zHits_posCh;
   TClonesArray    *ph2Seg_zHits_posErr;
   TClonesArray    *ph2Seg_zHits_side;
   TClonesArray    *ph2Seg_zHits_wire;
   TClonesArray    *ph2Seg_zHits_wirePos;
   TClonesArray    *ph2Seg_zHits_layer;
   TClonesArray    *ph2Seg_zHits_time;
   TClonesArray    *ph2Seg_zHits_timeCali;
   UInt_t          ltTwinMuxIn_nTrigs;
   vector<short>   *ltTwinMuxIn_wheel;
   vector<short>   *ltTwinMuxIn_sector;
   vector<short>   *ltTwinMuxIn_station;
   vector<short>   *ltTwinMuxIn_quality;
   vector<int>     *ltTwinMuxIn_phi;
   vector<int>     *ltTwinMuxIn_phiB;
   vector<float>   *ltTwinMuxIn_posLoc_x;
   vector<float>   *ltTwinMuxIn_dirLoc_phi;
   vector<short>   *ltTwinMuxIn_BX;
   vector<short>   *ltTwinMuxIn_is2nd;
   UInt_t          ltTwinMuxOut_nTrigs;
   vector<short>   *ltTwinMuxOut_wheel;
   vector<short>   *ltTwinMuxOut_sector;
   vector<short>   *ltTwinMuxOut_station;
   vector<short>   *ltTwinMuxOut_quality;
   vector<short>   *ltTwinMuxOut_rpcBit;
   vector<int>     *ltTwinMuxOut_phi;
   vector<int>     *ltTwinMuxOut_phiB;
   vector<float>   *ltTwinMuxOut_posLoc_x;
   vector<float>   *ltTwinMuxOut_dirLoc_phi;
   vector<short>   *ltTwinMuxOut_BX;
   vector<short>   *ltTwinMuxOut_is2nd;
   UInt_t          ltBmtfIn_nTrigs;
   vector<short>   *ltBmtfIn_wheel;
   vector<short>   *ltBmtfIn_sector;
   vector<short>   *ltBmtfIn_station;
   vector<short>   *ltBmtfIn_quality;
   vector<int>     *ltBmtfIn_phi;
   vector<int>     *ltBmtfIn_phiB;
   vector<float>   *ltBmtfIn_posLoc_x;
   vector<float>   *ltBmtfIn_dirLoc_phi;
   vector<short>   *ltBmtfIn_BX;
   vector<short>   *ltBmtfIn_is2nd;
   UInt_t          ltTwinMuxInTh_nTrigs;
   vector<short>   *ltTwinMuxInTh_wheel;
   vector<short>   *ltTwinMuxInTh_sector;
   vector<short>   *ltTwinMuxInTh_station;
   vector<short>   *ltTwinMuxInTh_BX;
   vector<unsigned short> *ltTwinMuxInTh_hitMap;
   UInt_t          ltBmtfInTh_nTrigs;
   vector<short>   *ltBmtfInTh_wheel;
   vector<short>   *ltBmtfInTh_sector;
   vector<short>   *ltBmtfInTh_station;
   vector<short>   *ltBmtfInTh_BX;
   vector<unsigned short> *ltBmtfInTh_hitMap;
   UInt_t          ph2TpgPhiHw_nTrigs;
   vector<short>   *ph2TpgPhiHw_wheel;
   vector<short>   *ph2TpgPhiHw_sector;
   vector<short>   *ph2TpgPhiHw_station;
   vector<short>   *ph2TpgPhiHw_quality;
   vector<short>   *ph2TpgPhiHw_superLayer;
   vector<short>   *ph2TpgPhiHw_rpcFlag;
   vector<int>     *ph2TpgPhiHw_chi2;
   vector<int>     *ph2TpgPhiHw_phi;
   vector<int>     *ph2TpgPhiHw_phiB;
   vector<float>   *ph2TpgPhiHw_posLoc_x;
   vector<float>   *ph2TpgPhiHw_dirLoc_phi;
   vector<int>     *ph2TpgPhiHw_BX;
   vector<int>     *ph2TpgPhiHw_t0;
   vector<short>   *ph2TpgPhiHw_index;
   UInt_t          ph2TpgPhiEmuHb_nTrigs;
   vector<short>   *ph2TpgPhiEmuHb_wheel;
   vector<short>   *ph2TpgPhiEmuHb_sector;
   vector<short>   *ph2TpgPhiEmuHb_station;
   vector<short>   *ph2TpgPhiEmuHb_quality;
   vector<short>   *ph2TpgPhiEmuHb_superLayer;
   vector<short>   *ph2TpgPhiEmuHb_rpcFlag;
   vector<int>     *ph2TpgPhiEmuHb_chi2;
   vector<int>     *ph2TpgPhiEmuHb_phi;
   vector<int>     *ph2TpgPhiEmuHb_phiB;
   vector<float>   *ph2TpgPhiEmuHb_posLoc_x;
   vector<float>   *ph2TpgPhiEmuHb_dirLoc_phi;
   vector<int>     *ph2TpgPhiEmuHb_BX;
   vector<int>     *ph2TpgPhiEmuHb_t0;
   vector<short>   *ph2TpgPhiEmuHb_index;
   UInt_t          ph2TpgPhiEmuAm_nTrigs;
   vector<short>   *ph2TpgPhiEmuAm_wheel;
   vector<short>   *ph2TpgPhiEmuAm_sector;
   vector<short>   *ph2TpgPhiEmuAm_station;
   vector<short>   *ph2TpgPhiEmuAm_quality;
   vector<short>   *ph2TpgPhiEmuAm_superLayer;
   vector<short>   *ph2TpgPhiEmuAm_rpcFlag;
   vector<int>     *ph2TpgPhiEmuAm_chi2;
   vector<int>     *ph2TpgPhiEmuAm_phi;
   vector<int>     *ph2TpgPhiEmuAm_phiB;
   vector<float>   *ph2TpgPhiEmuAm_posLoc_x;
   vector<float>   *ph2TpgPhiEmuAm_dirLoc_phi;
   vector<int>     *ph2TpgPhiEmuAm_BX;
   vector<int>     *ph2TpgPhiEmuAm_t0;
   vector<short>   *ph2TpgPhiEmuAm_index;

   // List of branches
   TBranch        *b_event_runNumber;   //!
   TBranch        *b_event_lumiBlock;   //!
   TBranch        *b_event_eventNumber;   //!
   TBranch        *b_event_timeStamp;   //!
   TBranch        *b_event_bunchCrossing;   //!
   TBranch        *b_event_orbitNumber;   //!
   TBranch        *b_gen_nGenParts;   //!
   TBranch        *b_gen_pdgId;   //!
   TBranch        *b_gen_pt;   //!
   TBranch        *b_gen_phi;   //!
   TBranch        *b_gen_eta;   //!
   TBranch        *b_gen_charge;   //!
   TBranch        *b_environment_truePileUp;   //!
   TBranch        *b_environment_actualPileUp;   //!
   TBranch        *b_environment_instLumi;   //!
   TBranch        *b_environment_nPV;   //!
   TBranch        *b_environment_pv_x;   //!
   TBranch        *b_environment_pv_y;   //!
   TBranch        *b_environment_pv_z;   //!
   TBranch        *b_environment_pv_xxErr;   //!
   TBranch        *b_environment_pv_yyErr;   //!
   TBranch        *b_environment_pv_zzErr;   //!
   TBranch        *b_environment_pv_xyErr;   //!
   TBranch        *b_environment_pv_xzErr;   //!
   TBranch        *b_environment_pv_yzErr;   //!
   TBranch        *b_digi_nDigis;   //!
   TBranch        *b_digi_wheel;   //!
   TBranch        *b_digi_sector;   //!
   TBranch        *b_digi_station;   //!
   TBranch        *b_digi_superLayer;   //!
   TBranch        *b_digi_layer;   //!
   TBranch        *b_digi_wire;   //!
   TBranch        *b_digi_time;   //!
   TBranch        *b_ph2Digi_nDigis;   //!
   TBranch        *b_ph2Digi_wheel;   //!
   TBranch        *b_ph2Digi_sector;   //!
   TBranch        *b_ph2Digi_station;   //!
   TBranch        *b_ph2Digi_superLayer;   //!
   TBranch        *b_ph2Digi_layer;   //!
   TBranch        *b_ph2Digi_wire;   //!
   TBranch        *b_ph2Digi_time;   //!
   TBranch        *b_seg_nSegments;   //!
   TBranch        *b_seg_wheel;   //!
   TBranch        *b_seg_sector;   //!
   TBranch        *b_seg_station;   //!
   TBranch        *b_seg_hasPhi;   //!
   TBranch        *b_seg_hasZed;   //!
   TBranch        *b_seg_posLoc_x;   //!
   TBranch        *b_seg_posLoc_y;   //!
   TBranch        *b_seg_posLoc_z;   //!
   TBranch        *b_seg_dirLoc_x;   //!
   TBranch        *b_seg_dirLoc_y;   //!
   TBranch        *b_seg_dirLoc_z;   //!
   TBranch        *b_seg_posLoc_x_SL1;   //!
   TBranch        *b_seg_posLoc_x_SL3;   //!
   TBranch        *b_seg_posLoc_x_midPlane;   //!
   TBranch        *b_seg_posGlb_phi;   //!
   TBranch        *b_seg_posGlb_eta;   //!
   TBranch        *b_seg_dirGlb_phi;   //!
   TBranch        *b_seg_dirGlb_eta;   //!
   TBranch        *b_seg_hitsExpPos;   //!
   TBranch        *b_seg_hitsExpPosCh;   //!
   TBranch        *b_seg_hitsExpWire;   //!
   TBranch        *b_seg_phi_t0;   //!
   TBranch        *b_seg_phi_vDrift;   //!
   TBranch        *b_seg_phi_normChi2;   //!
   TBranch        *b_seg_phi_nHits;   //!
   TBranch        *b_seg_phiHits_pos;   //!
   TBranch        *b_seg_phiHits_posCh;   //!
   TBranch        *b_seg_phiHits_posErr;   //!
   TBranch        *b_seg_phiHits_side;   //!
   TBranch        *b_seg_phiHits_wire;   //!
   TBranch        *b_seg_phiHits_wirePos;   //!
   TBranch        *b_seg_phiHits_layer;   //!
   TBranch        *b_seg_phiHits_superLayer;   //!
   TBranch        *b_seg_phiHits_time;   //!
   TBranch        *b_seg_phiHits_timeCali;   //!
   TBranch        *b_seg_z_normChi2;   //!
   TBranch        *b_seg_z_nHits;   //!
   TBranch        *b_seg_zHits_pos;   //!
   TBranch        *b_seg_zHits_posCh;   //!
   TBranch        *b_seg_zHits_posErr;   //!
   TBranch        *b_seg_zHits_side;   //!
   TBranch        *b_seg_zHits_wire;   //!
   TBranch        *b_seg_zHits_wirePos;   //!
   TBranch        *b_seg_zHits_layer;   //!
   TBranch        *b_seg_zHits_time;   //!
   TBranch        *b_seg_zHits_timeCali;   //!
   TBranch        *b_ph2Seg_nSegments;   //!
   TBranch        *b_ph2Seg_wheel;   //!
   TBranch        *b_ph2Seg_sector;   //!
   TBranch        *b_ph2Seg_station;   //!
   TBranch        *b_ph2Seg_hasPhi;   //!
   TBranch        *b_ph2Seg_hasZed;   //!
   TBranch        *b_ph2Seg_posLoc_x;   //!
   TBranch        *b_ph2Seg_posLoc_y;   //!
   TBranch        *b_ph2Seg_posLoc_z;   //!
   TBranch        *b_ph2Seg_dirLoc_x;   //!
   TBranch        *b_ph2Seg_dirLoc_y;   //!
   TBranch        *b_ph2Seg_dirLoc_z;   //!
   TBranch        *b_ph2Seg_posLoc_x_SL1;   //!
   TBranch        *b_ph2Seg_posLoc_x_SL3;   //!
   TBranch        *b_ph2Seg_posLoc_x_midPlane;   //!
   TBranch        *b_ph2Seg_posGlb_phi;   //!
   TBranch        *b_ph2Seg_posGlb_eta;   //!
   TBranch        *b_ph2Seg_dirGlb_phi;   //!
   TBranch        *b_ph2Seg_dirGlb_eta;   //!
   TBranch        *b_ph2Seg_hitsExpPos;   //!
   TBranch        *b_ph2Seg_hitsExpPosCh;   //!
   TBranch        *b_ph2Seg_hitsExpWire;   //!
   TBranch        *b_ph2Seg_phi_t0;   //!
   TBranch        *b_ph2Seg_phi_vDrift;   //!
   TBranch        *b_ph2Seg_phi_normChi2;   //!
   TBranch        *b_ph2Seg_phi_nHits;   //!
   TBranch        *b_ph2Seg_phiHits_pos;   //!
   TBranch        *b_ph2Seg_phiHits_posCh;   //!
   TBranch        *b_ph2Seg_phiHits_posErr;   //!
   TBranch        *b_ph2Seg_phiHits_side;   //!
   TBranch        *b_ph2Seg_phiHits_wire;   //!
   TBranch        *b_ph2Seg_phiHits_wirePos;   //!
   TBranch        *b_ph2Seg_phiHits_layer;   //!
   TBranch        *b_ph2Seg_phiHits_superLayer;   //!
   TBranch        *b_ph2Seg_phiHits_time;   //!
   TBranch        *b_ph2Seg_phiHits_timeCali;   //!
   TBranch        *b_ph2Seg_z_normChi2;   //!
   TBranch        *b_ph2Seg_z_nHits;   //!
   TBranch        *b_ph2Seg_zHits_pos;   //!
   TBranch        *b_ph2Seg_zHits_posCh;   //!
   TBranch        *b_ph2Seg_zHits_posErr;   //!
   TBranch        *b_ph2Seg_zHits_side;   //!
   TBranch        *b_ph2Seg_zHits_wire;   //!
   TBranch        *b_ph2Seg_zHits_wirePos;   //!
   TBranch        *b_ph2Seg_zHits_layer;   //!
   TBranch        *b_ph2Seg_zHits_time;   //!
   TBranch        *b_ph2Seg_zHits_timeCali;   //!
   TBranch        *b_ltTwinMuxIn_nTrigs;   //!
   TBranch        *b_ltTwinMuxIn_wheel;   //!
   TBranch        *b_ltTwinMuxIn_sector;   //!
   TBranch        *b_ltTwinMuxIn_station;   //!
   TBranch        *b_ltTwinMuxIn_quality;   //!
   TBranch        *b_ltTwinMuxIn_phi;   //!
   TBranch        *b_ltTwinMuxIn_phiB;   //!
   TBranch        *b_ltTwinMuxIn_posLoc_x;   //!
   TBranch        *b_ltTwinMuxIn_dirLoc_phi;   //!
   TBranch        *b_ltTwinMuxIn_BX;   //!
   TBranch        *b_ltTwinMuxIn_is2nd;   //!
   TBranch        *b_ltTwinMuxOut_nTrigs;   //!
   TBranch        *b_ltTwinMuxOut_wheel;   //!
   TBranch        *b_ltTwinMuxOut_sector;   //!
   TBranch        *b_ltTwinMuxOut_station;   //!
   TBranch        *b_ltTwinMuxOut_quality;   //!
   TBranch        *b_ltTwinMuxOut_rpcBit;   //!
   TBranch        *b_ltTwinMuxOut_phi;   //!
   TBranch        *b_ltTwinMuxOut_phiB;   //!
   TBranch        *b_ltTwinMuxOut_posLoc_x;   //!
   TBranch        *b_ltTwinMuxOut_dirLoc_phi;   //!
   TBranch        *b_ltTwinMuxOut_BX;   //!
   TBranch        *b_ltTwinMuxOut_is2nd;   //!
   TBranch        *b_ltBmtfIn_nTrigs;   //!
   TBranch        *b_ltBmtfIn_wheel;   //!
   TBranch        *b_ltBmtfIn_sector;   //!
   TBranch        *b_ltBmtfIn_station;   //!
   TBranch        *b_ltBmtfIn_quality;   //!
   TBranch        *b_ltBmtfIn_phi;   //!
   TBranch        *b_ltBmtfIn_phiB;   //!
   TBranch        *b_ltBmtfIn_posLoc_x;   //!
   TBranch        *b_ltBmtfIn_dirLoc_phi;   //!
   TBranch        *b_ltBmtfIn_BX;   //!
   TBranch        *b_ltBmtfIn_is2nd;   //!
   TBranch        *b_ltTwinMuxInTh_nTrigs;   //!
   TBranch        *b_ltTwinMuxInTh_wheel;   //!
   TBranch        *b_ltTwinMuxInTh_sector;   //!
   TBranch        *b_ltTwinMuxInTh_station;   //!
   TBranch        *b_ltTwinMuxInTh_BX;   //!
   TBranch        *b_ltTwinMuxInTh_hitMap;   //!
   TBranch        *b_ltBmtfInTh_nTrigs;   //!
   TBranch        *b_ltBmtfInTh_wheel;   //!
   TBranch        *b_ltBmtfInTh_sector;   //!
   TBranch        *b_ltBmtfInTh_station;   //!
   TBranch        *b_ltBmtfInTh_BX;   //!
   TBranch        *b_ltBmtfInTh_hitMap;   //!
   TBranch        *b_ph2TpgPhiHw_nTrigs;   //!
   TBranch        *b_ph2TpgPhiHw_wheel;   //!
   TBranch        *b_ph2TpgPhiHw_sector;   //!
   TBranch        *b_ph2TpgPhiHw_station;   //!
   TBranch        *b_ph2TpgPhiHw_quality;   //!
   TBranch        *b_ph2TpgPhiHw_superLayer;   //!
   TBranch        *b_ph2TpgPhiHw_rpcFlag;   //!
   TBranch        *b_ph2TpgPhiHw_chi2;   //!
   TBranch        *b_ph2TpgPhiHw_phi;   //!
   TBranch        *b_ph2TpgPhiHw_phiB;   //!
   TBranch        *b_ph2TpgPhiHw_posLoc_x;   //!
   TBranch        *b_ph2TpgPhiHw_dirLoc_phi;   //!
   TBranch        *b_ph2TpgPhiHw_BX;   //!
   TBranch        *b_ph2TpgPhiHw_t0;   //!
   TBranch        *b_ph2TpgPhiHw_index;   //!
   TBranch        *b_ph2TpgPhiEmuHb_nTrigs;   //!
   TBranch        *b_ph2TpgPhiEmuHb_wheel;   //!
   TBranch        *b_ph2TpgPhiEmuHb_sector;   //!
   TBranch        *b_ph2TpgPhiEmuHb_station;   //!
   TBranch        *b_ph2TpgPhiEmuHb_quality;   //!
   TBranch        *b_ph2TpgPhiEmuHb_superLayer;   //!
   TBranch        *b_ph2TpgPhiEmuHb_rpcFlag;   //!
   TBranch        *b_ph2TpgPhiEmuHb_chi2;   //!
   TBranch        *b_ph2TpgPhiEmuHb_phi;   //!
   TBranch        *b_ph2TpgPhiEmuHb_phiB;   //!
   TBranch        *b_ph2TpgPhiEmuHb_posLoc_x;   //!
   TBranch        *b_ph2TpgPhiEmuHb_dirLoc_phi;   //!
   TBranch        *b_ph2TpgPhiEmuHb_BX;   //!
   TBranch        *b_ph2TpgPhiEmuHb_t0;   //!
   TBranch        *b_ph2TpgPhiEmuHb_index;   //!
   TBranch        *b_ph2TpgPhiEmuAm_nTrigs;   //!
   TBranch        *b_ph2TpgPhiEmuAm_wheel;   //!
   TBranch        *b_ph2TpgPhiEmuAm_sector;   //!
   TBranch        *b_ph2TpgPhiEmuAm_station;   //!
   TBranch        *b_ph2TpgPhiEmuAm_quality;   //!
   TBranch        *b_ph2TpgPhiEmuAm_superLayer;   //!
   TBranch        *b_ph2TpgPhiEmuAm_rpcFlag;   //!
   TBranch        *b_ph2TpgPhiEmuAm_chi2;   //!
   TBranch        *b_ph2TpgPhiEmuAm_phi;   //!
   TBranch        *b_ph2TpgPhiEmuAm_phiB;   //!
   TBranch        *b_ph2TpgPhiEmuAm_posLoc_x;   //!
   TBranch        *b_ph2TpgPhiEmuAm_dirLoc_phi;   //!
   TBranch        *b_ph2TpgPhiEmuAm_BX;   //!
   TBranch        *b_ph2TpgPhiEmuAm_t0;   //!
   TBranch        *b_ph2TpgPhiEmuAm_index;   //!

   DTNtupleBaseAnalyzer(const TString &fileName);
   virtual ~DTNtupleBaseAnalyzer();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual void     LoadObjs();
   virtual Bool_t   Notify();

 protected:

   virtual void book() { };
   virtual void fill() { };
   virtual void endJob() { };
  
   template<typename T> T getXY(TClonesArray * arr, int x, int y) 
   { 
     return static_cast<T>((*((TVectorT<float> *)(arr->At(x))))[y]); 
   };

};

#endif
