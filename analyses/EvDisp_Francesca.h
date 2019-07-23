//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul 22 13:30:39 2019 by ROOT version 6.14/09
// from TTree DTTREE/DT Tree
// found on file: /eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/ntuples/DTDPGNtuple_run329806.root
//////////////////////////////////////////////////////////

#ifndef EvDisp_h
#define EvDisp_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"
#include "TClonesArray.h"
#include "vector"

class EvDisp {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

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

   EvDisp(TTree *tree=0);
   virtual ~EvDisp();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef EvDisp_cxx
EvDisp::EvDisp(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/ntuples/DTDPGNtuple_run329806.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/ntuples/DTDPGNtuple_run329806.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/ntuples/DTDPGNtuple_run329806.root:/dtNtupleProducer");
      dir->GetObject("DTTREE",tree);

   }
   Init(tree);
}

EvDisp::~EvDisp()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t EvDisp::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t EvDisp::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void EvDisp::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   gen_pdgId = 0;
   gen_pt = 0;
   gen_phi = 0;
   gen_eta = 0;
   gen_charge = 0;
   digi_wheel = 0;
   digi_sector = 0;
   digi_station = 0;
   digi_superLayer = 0;
   digi_layer = 0;
   digi_wire = 0;
   digi_time = 0;
   ph2Digi_wheel = 0;
   ph2Digi_sector = 0;
   ph2Digi_station = 0;
   ph2Digi_superLayer = 0;
   ph2Digi_layer = 0;
   ph2Digi_wire = 0;
   ph2Digi_time = 0;
   seg_wheel = 0;
   seg_sector = 0;
   seg_station = 0;
   seg_hasPhi = 0;
   seg_hasZed = 0;
   seg_posLoc_x = 0;
   seg_posLoc_y = 0;
   seg_posLoc_z = 0;
   seg_dirLoc_x = 0;
   seg_dirLoc_y = 0;
   seg_dirLoc_z = 0;
   seg_posLoc_x_SL1 = 0;
   seg_posLoc_x_SL3 = 0;
   seg_posLoc_x_midPlane = 0;
   seg_posGlb_phi = 0;
   seg_posGlb_eta = 0;
   seg_dirGlb_phi = 0;
   seg_dirGlb_eta = 0;
   seg_hitsExpPos = 0;
   seg_hitsExpPosCh = 0;
   seg_hitsExpWire = 0;
   seg_phi_t0 = 0;
   seg_phi_vDrift = 0;
   seg_phi_normChi2 = 0;
   seg_phi_nHits = 0;
   seg_phiHits_pos = 0;
   seg_phiHits_posCh = 0;
   seg_phiHits_posErr = 0;
   seg_phiHits_side = 0;
   seg_phiHits_wire = 0;
   seg_phiHits_wirePos = 0;
   seg_phiHits_layer = 0;
   seg_phiHits_superLayer = 0;
   seg_phiHits_time = 0;
   seg_phiHits_timeCali = 0;
   seg_z_normChi2 = 0;
   seg_z_nHits = 0;
   seg_zHits_pos = 0;
   seg_zHits_posCh = 0;
   seg_zHits_posErr = 0;
   seg_zHits_side = 0;
   seg_zHits_wire = 0;
   seg_zHits_wirePos = 0;
   seg_zHits_layer = 0;
   seg_zHits_time = 0;
   seg_zHits_timeCali = 0;
   ph2Seg_wheel = 0;
   ph2Seg_sector = 0;
   ph2Seg_station = 0;
   ph2Seg_hasPhi = 0;
   ph2Seg_hasZed = 0;
   ph2Seg_posLoc_x = 0;
   ph2Seg_posLoc_y = 0;
   ph2Seg_posLoc_z = 0;
   ph2Seg_dirLoc_x = 0;
   ph2Seg_dirLoc_y = 0;
   ph2Seg_dirLoc_z = 0;
   ph2Seg_posLoc_x_SL1 = 0;
   ph2Seg_posLoc_x_SL3 = 0;
   ph2Seg_posLoc_x_midPlane = 0;
   ph2Seg_posGlb_phi = 0;
   ph2Seg_posGlb_eta = 0;
   ph2Seg_dirGlb_phi = 0;
   ph2Seg_dirGlb_eta = 0;
   ph2Seg_hitsExpPos = 0;
   ph2Seg_hitsExpPosCh = 0;
   ph2Seg_hitsExpWire = 0;
   ph2Seg_phi_t0 = 0;
   ph2Seg_phi_vDrift = 0;
   ph2Seg_phi_normChi2 = 0;
   ph2Seg_phi_nHits = 0;
   ph2Seg_phiHits_pos = 0;
   ph2Seg_phiHits_posCh = 0;
   ph2Seg_phiHits_posErr = 0;
   ph2Seg_phiHits_side = 0;
   ph2Seg_phiHits_wire = 0;
   ph2Seg_phiHits_wirePos = 0;
   ph2Seg_phiHits_layer = 0;
   ph2Seg_phiHits_superLayer = 0;
   ph2Seg_phiHits_time = 0;
   ph2Seg_phiHits_timeCali = 0;
   ph2Seg_z_normChi2 = 0;
   ph2Seg_z_nHits = 0;
   ph2Seg_zHits_pos = 0;
   ph2Seg_zHits_posCh = 0;
   ph2Seg_zHits_posErr = 0;
   ph2Seg_zHits_side = 0;
   ph2Seg_zHits_wire = 0;
   ph2Seg_zHits_wirePos = 0;
   ph2Seg_zHits_layer = 0;
   ph2Seg_zHits_time = 0;
   ph2Seg_zHits_timeCali = 0;
   ltTwinMuxIn_wheel = 0;
   ltTwinMuxIn_sector = 0;
   ltTwinMuxIn_station = 0;
   ltTwinMuxIn_quality = 0;
   ltTwinMuxIn_phi = 0;
   ltTwinMuxIn_phiB = 0;
   ltTwinMuxIn_posLoc_x = 0;
   ltTwinMuxIn_dirLoc_phi = 0;
   ltTwinMuxIn_BX = 0;
   ltTwinMuxIn_is2nd = 0;
   ltTwinMuxOut_wheel = 0;
   ltTwinMuxOut_sector = 0;
   ltTwinMuxOut_station = 0;
   ltTwinMuxOut_quality = 0;
   ltTwinMuxOut_rpcBit = 0;
   ltTwinMuxOut_phi = 0;
   ltTwinMuxOut_phiB = 0;
   ltTwinMuxOut_posLoc_x = 0;
   ltTwinMuxOut_dirLoc_phi = 0;
   ltTwinMuxOut_BX = 0;
   ltTwinMuxOut_is2nd = 0;
   ltBmtfIn_wheel = 0;
   ltBmtfIn_sector = 0;
   ltBmtfIn_station = 0;
   ltBmtfIn_quality = 0;
   ltBmtfIn_phi = 0;
   ltBmtfIn_phiB = 0;
   ltBmtfIn_posLoc_x = 0;
   ltBmtfIn_dirLoc_phi = 0;
   ltBmtfIn_BX = 0;
   ltBmtfIn_is2nd = 0;
   ltTwinMuxInTh_wheel = 0;
   ltTwinMuxInTh_sector = 0;
   ltTwinMuxInTh_station = 0;
   ltTwinMuxInTh_BX = 0;
   ltTwinMuxInTh_hitMap = 0;
   ltBmtfInTh_wheel = 0;
   ltBmtfInTh_sector = 0;
   ltBmtfInTh_station = 0;
   ltBmtfInTh_BX = 0;
   ltBmtfInTh_hitMap = 0;
   ph2TpgPhiHw_wheel = 0;
   ph2TpgPhiHw_sector = 0;
   ph2TpgPhiHw_station = 0;
   ph2TpgPhiHw_quality = 0;
   ph2TpgPhiHw_superLayer = 0;
   ph2TpgPhiHw_rpcFlag = 0;
   ph2TpgPhiHw_chi2 = 0;
   ph2TpgPhiHw_phi = 0;
   ph2TpgPhiHw_phiB = 0;
   ph2TpgPhiHw_posLoc_x = 0;
   ph2TpgPhiHw_dirLoc_phi = 0;
   ph2TpgPhiHw_BX = 0;
   ph2TpgPhiHw_t0 = 0;
   ph2TpgPhiHw_index = 0;
   ph2TpgPhiEmuHb_wheel = 0;
   ph2TpgPhiEmuHb_sector = 0;
   ph2TpgPhiEmuHb_station = 0;
   ph2TpgPhiEmuHb_quality = 0;
   ph2TpgPhiEmuHb_superLayer = 0;
   ph2TpgPhiEmuHb_rpcFlag = 0;
   ph2TpgPhiEmuHb_chi2 = 0;
   ph2TpgPhiEmuHb_phi = 0;
   ph2TpgPhiEmuHb_phiB = 0;
   ph2TpgPhiEmuHb_posLoc_x = 0;
   ph2TpgPhiEmuHb_dirLoc_phi = 0;
   ph2TpgPhiEmuHb_BX = 0;
   ph2TpgPhiEmuHb_t0 = 0;
   ph2TpgPhiEmuHb_index = 0;
   ph2TpgPhiEmuAm_wheel = 0;
   ph2TpgPhiEmuAm_sector = 0;
   ph2TpgPhiEmuAm_station = 0;
   ph2TpgPhiEmuAm_quality = 0;
   ph2TpgPhiEmuAm_superLayer = 0;
   ph2TpgPhiEmuAm_rpcFlag = 0;
   ph2TpgPhiEmuAm_chi2 = 0;
   ph2TpgPhiEmuAm_phi = 0;
   ph2TpgPhiEmuAm_phiB = 0;
   ph2TpgPhiEmuAm_posLoc_x = 0;
   ph2TpgPhiEmuAm_dirLoc_phi = 0;
   ph2TpgPhiEmuAm_BX = 0;
   ph2TpgPhiEmuAm_t0 = 0;
   ph2TpgPhiEmuAm_index = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event_runNumber", &event_runNumber, &b_event_runNumber);
   fChain->SetBranchAddress("event_lumiBlock", &event_lumiBlock, &b_event_lumiBlock);
   fChain->SetBranchAddress("event_eventNumber", &event_eventNumber, &b_event_eventNumber);
   fChain->SetBranchAddress("event_timeStamp", &event_timeStamp, &b_event_timeStamp);
   fChain->SetBranchAddress("event_bunchCrossing", &event_bunchCrossing, &b_event_bunchCrossing);
   fChain->SetBranchAddress("event_orbitNumber", &event_orbitNumber, &b_event_orbitNumber);
   fChain->SetBranchAddress("gen_nGenParts", &gen_nGenParts, &b_gen_nGenParts);
   fChain->SetBranchAddress("gen_pdgId", &gen_pdgId, &b_gen_pdgId);
   fChain->SetBranchAddress("gen_pt", &gen_pt, &b_gen_pt);
   fChain->SetBranchAddress("gen_phi", &gen_phi, &b_gen_phi);
   fChain->SetBranchAddress("gen_eta", &gen_eta, &b_gen_eta);
   fChain->SetBranchAddress("gen_charge", &gen_charge, &b_gen_charge);
   fChain->SetBranchAddress("environment_truePileUp", &environment_truePileUp, &b_environment_truePileUp);
   fChain->SetBranchAddress("environment_actualPileUp", &environment_actualPileUp, &b_environment_actualPileUp);
   fChain->SetBranchAddress("environment_instLumi", &environment_instLumi, &b_environment_instLumi);
   fChain->SetBranchAddress("environment_nPV", &environment_nPV, &b_environment_nPV);
   fChain->SetBranchAddress("environment_pv_x", &environment_pv_x, &b_environment_pv_x);
   fChain->SetBranchAddress("environment_pv_y", &environment_pv_y, &b_environment_pv_y);
   fChain->SetBranchAddress("environment_pv_z", &environment_pv_z, &b_environment_pv_z);
   fChain->SetBranchAddress("environment_pv_xxErr", &environment_pv_xxErr, &b_environment_pv_xxErr);
   fChain->SetBranchAddress("environment_pv_yyErr", &environment_pv_yyErr, &b_environment_pv_yyErr);
   fChain->SetBranchAddress("environment_pv_zzErr", &environment_pv_zzErr, &b_environment_pv_zzErr);
   fChain->SetBranchAddress("environment_pv_xyErr", &environment_pv_xyErr, &b_environment_pv_xyErr);
   fChain->SetBranchAddress("environment_pv_xzErr", &environment_pv_xzErr, &b_environment_pv_xzErr);
   fChain->SetBranchAddress("environment_pv_yzErr", &environment_pv_yzErr, &b_environment_pv_yzErr);
   fChain->SetBranchAddress("digi_nDigis", &digi_nDigis, &b_digi_nDigis);
   fChain->SetBranchAddress("digi_wheel", &digi_wheel, &b_digi_wheel);
   fChain->SetBranchAddress("digi_sector", &digi_sector, &b_digi_sector);
   fChain->SetBranchAddress("digi_station", &digi_station, &b_digi_station);
   fChain->SetBranchAddress("digi_superLayer", &digi_superLayer, &b_digi_superLayer);
   fChain->SetBranchAddress("digi_layer", &digi_layer, &b_digi_layer);
   fChain->SetBranchAddress("digi_wire", &digi_wire, &b_digi_wire);
   fChain->SetBranchAddress("digi_time", &digi_time, &b_digi_time);
   fChain->SetBranchAddress("ph2Digi_nDigis", &ph2Digi_nDigis, &b_ph2Digi_nDigis);
   fChain->SetBranchAddress("ph2Digi_wheel", &ph2Digi_wheel, &b_ph2Digi_wheel);
   fChain->SetBranchAddress("ph2Digi_sector", &ph2Digi_sector, &b_ph2Digi_sector);
   fChain->SetBranchAddress("ph2Digi_station", &ph2Digi_station, &b_ph2Digi_station);
   fChain->SetBranchAddress("ph2Digi_superLayer", &ph2Digi_superLayer, &b_ph2Digi_superLayer);
   fChain->SetBranchAddress("ph2Digi_layer", &ph2Digi_layer, &b_ph2Digi_layer);
   fChain->SetBranchAddress("ph2Digi_wire", &ph2Digi_wire, &b_ph2Digi_wire);
   fChain->SetBranchAddress("ph2Digi_time", &ph2Digi_time, &b_ph2Digi_time);
   fChain->SetBranchAddress("seg_nSegments", &seg_nSegments, &b_seg_nSegments);
   fChain->SetBranchAddress("seg_wheel", &seg_wheel, &b_seg_wheel);
   fChain->SetBranchAddress("seg_sector", &seg_sector, &b_seg_sector);
   fChain->SetBranchAddress("seg_station", &seg_station, &b_seg_station);
   fChain->SetBranchAddress("seg_hasPhi", &seg_hasPhi, &b_seg_hasPhi);
   fChain->SetBranchAddress("seg_hasZed", &seg_hasZed, &b_seg_hasZed);
   fChain->SetBranchAddress("seg_posLoc_x", &seg_posLoc_x, &b_seg_posLoc_x);
   fChain->SetBranchAddress("seg_posLoc_y", &seg_posLoc_y, &b_seg_posLoc_y);
   fChain->SetBranchAddress("seg_posLoc_z", &seg_posLoc_z, &b_seg_posLoc_z);
   fChain->SetBranchAddress("seg_dirLoc_x", &seg_dirLoc_x, &b_seg_dirLoc_x);
   fChain->SetBranchAddress("seg_dirLoc_y", &seg_dirLoc_y, &b_seg_dirLoc_y);
   fChain->SetBranchAddress("seg_dirLoc_z", &seg_dirLoc_z, &b_seg_dirLoc_z);
   fChain->SetBranchAddress("seg_posLoc_x_SL1", &seg_posLoc_x_SL1, &b_seg_posLoc_x_SL1);
   fChain->SetBranchAddress("seg_posLoc_x_SL3", &seg_posLoc_x_SL3, &b_seg_posLoc_x_SL3);
   fChain->SetBranchAddress("seg_posLoc_x_midPlane", &seg_posLoc_x_midPlane, &b_seg_posLoc_x_midPlane);
   fChain->SetBranchAddress("seg_posGlb_phi", &seg_posGlb_phi, &b_seg_posGlb_phi);
   fChain->SetBranchAddress("seg_posGlb_eta", &seg_posGlb_eta, &b_seg_posGlb_eta);
   fChain->SetBranchAddress("seg_dirGlb_phi", &seg_dirGlb_phi, &b_seg_dirGlb_phi);
   fChain->SetBranchAddress("seg_dirGlb_eta", &seg_dirGlb_eta, &b_seg_dirGlb_eta);
   fChain->SetBranchAddress("seg_hitsExpPos", &seg_hitsExpPos, &b_seg_hitsExpPos);
   fChain->SetBranchAddress("seg_hitsExpPosCh", &seg_hitsExpPosCh, &b_seg_hitsExpPosCh);
   fChain->SetBranchAddress("seg_hitsExpWire", &seg_hitsExpWire, &b_seg_hitsExpWire);
   fChain->SetBranchAddress("seg_phi_t0", &seg_phi_t0, &b_seg_phi_t0);
   fChain->SetBranchAddress("seg_phi_vDrift", &seg_phi_vDrift, &b_seg_phi_vDrift);
   fChain->SetBranchAddress("seg_phi_normChi2", &seg_phi_normChi2, &b_seg_phi_normChi2);
   fChain->SetBranchAddress("seg_phi_nHits", &seg_phi_nHits, &b_seg_phi_nHits);
   fChain->SetBranchAddress("seg_phiHits_pos", &seg_phiHits_pos, &b_seg_phiHits_pos);
   fChain->SetBranchAddress("seg_phiHits_posCh", &seg_phiHits_posCh, &b_seg_phiHits_posCh);
   fChain->SetBranchAddress("seg_phiHits_posErr", &seg_phiHits_posErr, &b_seg_phiHits_posErr);
   fChain->SetBranchAddress("seg_phiHits_side", &seg_phiHits_side, &b_seg_phiHits_side);
   fChain->SetBranchAddress("seg_phiHits_wire", &seg_phiHits_wire, &b_seg_phiHits_wire);
   fChain->SetBranchAddress("seg_phiHits_wirePos", &seg_phiHits_wirePos, &b_seg_phiHits_wirePos);
   fChain->SetBranchAddress("seg_phiHits_layer", &seg_phiHits_layer, &b_seg_phiHits_layer);
   fChain->SetBranchAddress("seg_phiHits_superLayer", &seg_phiHits_superLayer, &b_seg_phiHits_superLayer);
   fChain->SetBranchAddress("seg_phiHits_time", &seg_phiHits_time, &b_seg_phiHits_time);
   fChain->SetBranchAddress("seg_phiHits_timeCali", &seg_phiHits_timeCali, &b_seg_phiHits_timeCali);
   fChain->SetBranchAddress("seg_z_normChi2", &seg_z_normChi2, &b_seg_z_normChi2);
   fChain->SetBranchAddress("seg_z_nHits", &seg_z_nHits, &b_seg_z_nHits);
   fChain->SetBranchAddress("seg_zHits_pos", &seg_zHits_pos, &b_seg_zHits_pos);
   fChain->SetBranchAddress("seg_zHits_posCh", &seg_zHits_posCh, &b_seg_zHits_posCh);
   fChain->SetBranchAddress("seg_zHits_posErr", &seg_zHits_posErr, &b_seg_zHits_posErr);
   fChain->SetBranchAddress("seg_zHits_side", &seg_zHits_side, &b_seg_zHits_side);
   fChain->SetBranchAddress("seg_zHits_wire", &seg_zHits_wire, &b_seg_zHits_wire);
   fChain->SetBranchAddress("seg_zHits_wirePos", &seg_zHits_wirePos, &b_seg_zHits_wirePos);
   fChain->SetBranchAddress("seg_zHits_layer", &seg_zHits_layer, &b_seg_zHits_layer);
   fChain->SetBranchAddress("seg_zHits_time", &seg_zHits_time, &b_seg_zHits_time);
   fChain->SetBranchAddress("seg_zHits_timeCali", &seg_zHits_timeCali, &b_seg_zHits_timeCali);
   fChain->SetBranchAddress("ph2Seg_nSegments", &ph2Seg_nSegments, &b_ph2Seg_nSegments);
   fChain->SetBranchAddress("ph2Seg_wheel", &ph2Seg_wheel, &b_ph2Seg_wheel);
   fChain->SetBranchAddress("ph2Seg_sector", &ph2Seg_sector, &b_ph2Seg_sector);
   fChain->SetBranchAddress("ph2Seg_station", &ph2Seg_station, &b_ph2Seg_station);
   fChain->SetBranchAddress("ph2Seg_hasPhi", &ph2Seg_hasPhi, &b_ph2Seg_hasPhi);
   fChain->SetBranchAddress("ph2Seg_hasZed", &ph2Seg_hasZed, &b_ph2Seg_hasZed);
   fChain->SetBranchAddress("ph2Seg_posLoc_x", &ph2Seg_posLoc_x, &b_ph2Seg_posLoc_x);
   fChain->SetBranchAddress("ph2Seg_posLoc_y", &ph2Seg_posLoc_y, &b_ph2Seg_posLoc_y);
   fChain->SetBranchAddress("ph2Seg_posLoc_z", &ph2Seg_posLoc_z, &b_ph2Seg_posLoc_z);
   fChain->SetBranchAddress("ph2Seg_dirLoc_x", &ph2Seg_dirLoc_x, &b_ph2Seg_dirLoc_x);
   fChain->SetBranchAddress("ph2Seg_dirLoc_y", &ph2Seg_dirLoc_y, &b_ph2Seg_dirLoc_y);
   fChain->SetBranchAddress("ph2Seg_dirLoc_z", &ph2Seg_dirLoc_z, &b_ph2Seg_dirLoc_z);
   fChain->SetBranchAddress("ph2Seg_posLoc_x_SL1", &ph2Seg_posLoc_x_SL1, &b_ph2Seg_posLoc_x_SL1);
   fChain->SetBranchAddress("ph2Seg_posLoc_x_SL3", &ph2Seg_posLoc_x_SL3, &b_ph2Seg_posLoc_x_SL3);
   fChain->SetBranchAddress("ph2Seg_posLoc_x_midPlane", &ph2Seg_posLoc_x_midPlane, &b_ph2Seg_posLoc_x_midPlane);
   fChain->SetBranchAddress("ph2Seg_posGlb_phi", &ph2Seg_posGlb_phi, &b_ph2Seg_posGlb_phi);
   fChain->SetBranchAddress("ph2Seg_posGlb_eta", &ph2Seg_posGlb_eta, &b_ph2Seg_posGlb_eta);
   fChain->SetBranchAddress("ph2Seg_dirGlb_phi", &ph2Seg_dirGlb_phi, &b_ph2Seg_dirGlb_phi);
   fChain->SetBranchAddress("ph2Seg_dirGlb_eta", &ph2Seg_dirGlb_eta, &b_ph2Seg_dirGlb_eta);
   fChain->SetBranchAddress("ph2Seg_hitsExpPos", &ph2Seg_hitsExpPos, &b_ph2Seg_hitsExpPos);
   fChain->SetBranchAddress("ph2Seg_hitsExpPosCh", &ph2Seg_hitsExpPosCh, &b_ph2Seg_hitsExpPosCh);
   fChain->SetBranchAddress("ph2Seg_hitsExpWire", &ph2Seg_hitsExpWire, &b_ph2Seg_hitsExpWire);
   fChain->SetBranchAddress("ph2Seg_phi_t0", &ph2Seg_phi_t0, &b_ph2Seg_phi_t0);
   fChain->SetBranchAddress("ph2Seg_phi_vDrift", &ph2Seg_phi_vDrift, &b_ph2Seg_phi_vDrift);
   fChain->SetBranchAddress("ph2Seg_phi_normChi2", &ph2Seg_phi_normChi2, &b_ph2Seg_phi_normChi2);
   fChain->SetBranchAddress("ph2Seg_phi_nHits", &ph2Seg_phi_nHits, &b_ph2Seg_phi_nHits);
   fChain->SetBranchAddress("ph2Seg_phiHits_pos", &ph2Seg_phiHits_pos, &b_ph2Seg_phiHits_pos);
   fChain->SetBranchAddress("ph2Seg_phiHits_posCh", &ph2Seg_phiHits_posCh, &b_ph2Seg_phiHits_posCh);
   fChain->SetBranchAddress("ph2Seg_phiHits_posErr", &ph2Seg_phiHits_posErr, &b_ph2Seg_phiHits_posErr);
   fChain->SetBranchAddress("ph2Seg_phiHits_side", &ph2Seg_phiHits_side, &b_ph2Seg_phiHits_side);
   fChain->SetBranchAddress("ph2Seg_phiHits_wire", &ph2Seg_phiHits_wire, &b_ph2Seg_phiHits_wire);
   fChain->SetBranchAddress("ph2Seg_phiHits_wirePos", &ph2Seg_phiHits_wirePos, &b_ph2Seg_phiHits_wirePos);
   fChain->SetBranchAddress("ph2Seg_phiHits_layer", &ph2Seg_phiHits_layer, &b_ph2Seg_phiHits_layer);
   fChain->SetBranchAddress("ph2Seg_phiHits_superLayer", &ph2Seg_phiHits_superLayer, &b_ph2Seg_phiHits_superLayer);
   fChain->SetBranchAddress("ph2Seg_phiHits_time", &ph2Seg_phiHits_time, &b_ph2Seg_phiHits_time);
   fChain->SetBranchAddress("ph2Seg_phiHits_timeCali", &ph2Seg_phiHits_timeCali, &b_ph2Seg_phiHits_timeCali);
   fChain->SetBranchAddress("ph2Seg_z_normChi2", &ph2Seg_z_normChi2, &b_ph2Seg_z_normChi2);
   fChain->SetBranchAddress("ph2Seg_z_nHits", &ph2Seg_z_nHits, &b_ph2Seg_z_nHits);
   fChain->SetBranchAddress("ph2Seg_zHits_pos", &ph2Seg_zHits_pos, &b_ph2Seg_zHits_pos);
   fChain->SetBranchAddress("ph2Seg_zHits_posCh", &ph2Seg_zHits_posCh, &b_ph2Seg_zHits_posCh);
   fChain->SetBranchAddress("ph2Seg_zHits_posErr", &ph2Seg_zHits_posErr, &b_ph2Seg_zHits_posErr);
   fChain->SetBranchAddress("ph2Seg_zHits_side", &ph2Seg_zHits_side, &b_ph2Seg_zHits_side);
   fChain->SetBranchAddress("ph2Seg_zHits_wire", &ph2Seg_zHits_wire, &b_ph2Seg_zHits_wire);
   fChain->SetBranchAddress("ph2Seg_zHits_wirePos", &ph2Seg_zHits_wirePos, &b_ph2Seg_zHits_wirePos);
   fChain->SetBranchAddress("ph2Seg_zHits_layer", &ph2Seg_zHits_layer, &b_ph2Seg_zHits_layer);
   fChain->SetBranchAddress("ph2Seg_zHits_time", &ph2Seg_zHits_time, &b_ph2Seg_zHits_time);
   fChain->SetBranchAddress("ph2Seg_zHits_timeCali", &ph2Seg_zHits_timeCali, &b_ph2Seg_zHits_timeCali);
   fChain->SetBranchAddress("ltTwinMuxIn_nTrigs", &ltTwinMuxIn_nTrigs, &b_ltTwinMuxIn_nTrigs);
   fChain->SetBranchAddress("ltTwinMuxIn_wheel", &ltTwinMuxIn_wheel, &b_ltTwinMuxIn_wheel);
   fChain->SetBranchAddress("ltTwinMuxIn_sector", &ltTwinMuxIn_sector, &b_ltTwinMuxIn_sector);
   fChain->SetBranchAddress("ltTwinMuxIn_station", &ltTwinMuxIn_station, &b_ltTwinMuxIn_station);
   fChain->SetBranchAddress("ltTwinMuxIn_quality", &ltTwinMuxIn_quality, &b_ltTwinMuxIn_quality);
   fChain->SetBranchAddress("ltTwinMuxIn_phi", &ltTwinMuxIn_phi, &b_ltTwinMuxIn_phi);
   fChain->SetBranchAddress("ltTwinMuxIn_phiB", &ltTwinMuxIn_phiB, &b_ltTwinMuxIn_phiB);
   fChain->SetBranchAddress("ltTwinMuxIn_posLoc_x", &ltTwinMuxIn_posLoc_x, &b_ltTwinMuxIn_posLoc_x);
   fChain->SetBranchAddress("ltTwinMuxIn_dirLoc_phi", &ltTwinMuxIn_dirLoc_phi, &b_ltTwinMuxIn_dirLoc_phi);
   fChain->SetBranchAddress("ltTwinMuxIn_BX", &ltTwinMuxIn_BX, &b_ltTwinMuxIn_BX);
   fChain->SetBranchAddress("ltTwinMuxIn_is2nd", &ltTwinMuxIn_is2nd, &b_ltTwinMuxIn_is2nd);
   fChain->SetBranchAddress("ltTwinMuxOut_nTrigs", &ltTwinMuxOut_nTrigs, &b_ltTwinMuxOut_nTrigs);
   fChain->SetBranchAddress("ltTwinMuxOut_wheel", &ltTwinMuxOut_wheel, &b_ltTwinMuxOut_wheel);
   fChain->SetBranchAddress("ltTwinMuxOut_sector", &ltTwinMuxOut_sector, &b_ltTwinMuxOut_sector);
   fChain->SetBranchAddress("ltTwinMuxOut_station", &ltTwinMuxOut_station, &b_ltTwinMuxOut_station);
   fChain->SetBranchAddress("ltTwinMuxOut_quality", &ltTwinMuxOut_quality, &b_ltTwinMuxOut_quality);
   fChain->SetBranchAddress("ltTwinMuxOut_rpcBit", &ltTwinMuxOut_rpcBit, &b_ltTwinMuxOut_rpcBit);
   fChain->SetBranchAddress("ltTwinMuxOut_phi", &ltTwinMuxOut_phi, &b_ltTwinMuxOut_phi);
   fChain->SetBranchAddress("ltTwinMuxOut_phiB", &ltTwinMuxOut_phiB, &b_ltTwinMuxOut_phiB);
   fChain->SetBranchAddress("ltTwinMuxOut_posLoc_x", &ltTwinMuxOut_posLoc_x, &b_ltTwinMuxOut_posLoc_x);
   fChain->SetBranchAddress("ltTwinMuxOut_dirLoc_phi", &ltTwinMuxOut_dirLoc_phi, &b_ltTwinMuxOut_dirLoc_phi);
   fChain->SetBranchAddress("ltTwinMuxOut_BX", &ltTwinMuxOut_BX, &b_ltTwinMuxOut_BX);
   fChain->SetBranchAddress("ltTwinMuxOut_is2nd", &ltTwinMuxOut_is2nd, &b_ltTwinMuxOut_is2nd);
   fChain->SetBranchAddress("ltBmtfIn_nTrigs", &ltBmtfIn_nTrigs, &b_ltBmtfIn_nTrigs);
   fChain->SetBranchAddress("ltBmtfIn_wheel", &ltBmtfIn_wheel, &b_ltBmtfIn_wheel);
   fChain->SetBranchAddress("ltBmtfIn_sector", &ltBmtfIn_sector, &b_ltBmtfIn_sector);
   fChain->SetBranchAddress("ltBmtfIn_station", &ltBmtfIn_station, &b_ltBmtfIn_station);
   fChain->SetBranchAddress("ltBmtfIn_quality", &ltBmtfIn_quality, &b_ltBmtfIn_quality);
   fChain->SetBranchAddress("ltBmtfIn_phi", &ltBmtfIn_phi, &b_ltBmtfIn_phi);
   fChain->SetBranchAddress("ltBmtfIn_phiB", &ltBmtfIn_phiB, &b_ltBmtfIn_phiB);
   fChain->SetBranchAddress("ltBmtfIn_posLoc_x", &ltBmtfIn_posLoc_x, &b_ltBmtfIn_posLoc_x);
   fChain->SetBranchAddress("ltBmtfIn_dirLoc_phi", &ltBmtfIn_dirLoc_phi, &b_ltBmtfIn_dirLoc_phi);
   fChain->SetBranchAddress("ltBmtfIn_BX", &ltBmtfIn_BX, &b_ltBmtfIn_BX);
   fChain->SetBranchAddress("ltBmtfIn_is2nd", &ltBmtfIn_is2nd, &b_ltBmtfIn_is2nd);
   fChain->SetBranchAddress("ltTwinMuxInTh_nTrigs", &ltTwinMuxInTh_nTrigs, &b_ltTwinMuxInTh_nTrigs);
   fChain->SetBranchAddress("ltTwinMuxInTh_wheel", &ltTwinMuxInTh_wheel, &b_ltTwinMuxInTh_wheel);
   fChain->SetBranchAddress("ltTwinMuxInTh_sector", &ltTwinMuxInTh_sector, &b_ltTwinMuxInTh_sector);
   fChain->SetBranchAddress("ltTwinMuxInTh_station", &ltTwinMuxInTh_station, &b_ltTwinMuxInTh_station);
   fChain->SetBranchAddress("ltTwinMuxInTh_BX", &ltTwinMuxInTh_BX, &b_ltTwinMuxInTh_BX);
   fChain->SetBranchAddress("ltTwinMuxInTh_hitMap", &ltTwinMuxInTh_hitMap, &b_ltTwinMuxInTh_hitMap);
   fChain->SetBranchAddress("ltBmtfInTh_nTrigs", &ltBmtfInTh_nTrigs, &b_ltBmtfInTh_nTrigs);
   fChain->SetBranchAddress("ltBmtfInTh_wheel", &ltBmtfInTh_wheel, &b_ltBmtfInTh_wheel);
   fChain->SetBranchAddress("ltBmtfInTh_sector", &ltBmtfInTh_sector, &b_ltBmtfInTh_sector);
   fChain->SetBranchAddress("ltBmtfInTh_station", &ltBmtfInTh_station, &b_ltBmtfInTh_station);
   fChain->SetBranchAddress("ltBmtfInTh_BX", &ltBmtfInTh_BX, &b_ltBmtfInTh_BX);
   fChain->SetBranchAddress("ltBmtfInTh_hitMap", &ltBmtfInTh_hitMap, &b_ltBmtfInTh_hitMap);
   fChain->SetBranchAddress("ph2TpgPhiHw_nTrigs", &ph2TpgPhiHw_nTrigs, &b_ph2TpgPhiHw_nTrigs);
   fChain->SetBranchAddress("ph2TpgPhiHw_wheel", &ph2TpgPhiHw_wheel, &b_ph2TpgPhiHw_wheel);
   fChain->SetBranchAddress("ph2TpgPhiHw_sector", &ph2TpgPhiHw_sector, &b_ph2TpgPhiHw_sector);
   fChain->SetBranchAddress("ph2TpgPhiHw_station", &ph2TpgPhiHw_station, &b_ph2TpgPhiHw_station);
   fChain->SetBranchAddress("ph2TpgPhiHw_quality", &ph2TpgPhiHw_quality, &b_ph2TpgPhiHw_quality);
   fChain->SetBranchAddress("ph2TpgPhiHw_superLayer", &ph2TpgPhiHw_superLayer, &b_ph2TpgPhiHw_superLayer);
   fChain->SetBranchAddress("ph2TpgPhiHw_rpcFlag", &ph2TpgPhiHw_rpcFlag, &b_ph2TpgPhiHw_rpcFlag);
   fChain->SetBranchAddress("ph2TpgPhiHw_chi2", &ph2TpgPhiHw_chi2, &b_ph2TpgPhiHw_chi2);
   fChain->SetBranchAddress("ph2TpgPhiHw_phi", &ph2TpgPhiHw_phi, &b_ph2TpgPhiHw_phi);
   fChain->SetBranchAddress("ph2TpgPhiHw_phiB", &ph2TpgPhiHw_phiB, &b_ph2TpgPhiHw_phiB);
   fChain->SetBranchAddress("ph2TpgPhiHw_posLoc_x", &ph2TpgPhiHw_posLoc_x, &b_ph2TpgPhiHw_posLoc_x);
   fChain->SetBranchAddress("ph2TpgPhiHw_dirLoc_phi", &ph2TpgPhiHw_dirLoc_phi, &b_ph2TpgPhiHw_dirLoc_phi);
   fChain->SetBranchAddress("ph2TpgPhiHw_BX", &ph2TpgPhiHw_BX, &b_ph2TpgPhiHw_BX);
   fChain->SetBranchAddress("ph2TpgPhiHw_t0", &ph2TpgPhiHw_t0, &b_ph2TpgPhiHw_t0);
   fChain->SetBranchAddress("ph2TpgPhiHw_index", &ph2TpgPhiHw_index, &b_ph2TpgPhiHw_index);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_nTrigs", &ph2TpgPhiEmuHb_nTrigs, &b_ph2TpgPhiEmuHb_nTrigs);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_wheel", &ph2TpgPhiEmuHb_wheel, &b_ph2TpgPhiEmuHb_wheel);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_sector", &ph2TpgPhiEmuHb_sector, &b_ph2TpgPhiEmuHb_sector);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_station", &ph2TpgPhiEmuHb_station, &b_ph2TpgPhiEmuHb_station);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_quality", &ph2TpgPhiEmuHb_quality, &b_ph2TpgPhiEmuHb_quality);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_superLayer", &ph2TpgPhiEmuHb_superLayer, &b_ph2TpgPhiEmuHb_superLayer);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_rpcFlag", &ph2TpgPhiEmuHb_rpcFlag, &b_ph2TpgPhiEmuHb_rpcFlag);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_chi2", &ph2TpgPhiEmuHb_chi2, &b_ph2TpgPhiEmuHb_chi2);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_phi", &ph2TpgPhiEmuHb_phi, &b_ph2TpgPhiEmuHb_phi);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_phiB", &ph2TpgPhiEmuHb_phiB, &b_ph2TpgPhiEmuHb_phiB);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_posLoc_x", &ph2TpgPhiEmuHb_posLoc_x, &b_ph2TpgPhiEmuHb_posLoc_x);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_dirLoc_phi", &ph2TpgPhiEmuHb_dirLoc_phi, &b_ph2TpgPhiEmuHb_dirLoc_phi);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_BX", &ph2TpgPhiEmuHb_BX, &b_ph2TpgPhiEmuHb_BX);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_t0", &ph2TpgPhiEmuHb_t0, &b_ph2TpgPhiEmuHb_t0);
   fChain->SetBranchAddress("ph2TpgPhiEmuHb_index", &ph2TpgPhiEmuHb_index, &b_ph2TpgPhiEmuHb_index);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_nTrigs", &ph2TpgPhiEmuAm_nTrigs, &b_ph2TpgPhiEmuAm_nTrigs);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_wheel", &ph2TpgPhiEmuAm_wheel, &b_ph2TpgPhiEmuAm_wheel);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_sector", &ph2TpgPhiEmuAm_sector, &b_ph2TpgPhiEmuAm_sector);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_station", &ph2TpgPhiEmuAm_station, &b_ph2TpgPhiEmuAm_station);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_quality", &ph2TpgPhiEmuAm_quality, &b_ph2TpgPhiEmuAm_quality);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_superLayer", &ph2TpgPhiEmuAm_superLayer, &b_ph2TpgPhiEmuAm_superLayer);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_rpcFlag", &ph2TpgPhiEmuAm_rpcFlag, &b_ph2TpgPhiEmuAm_rpcFlag);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_chi2", &ph2TpgPhiEmuAm_chi2, &b_ph2TpgPhiEmuAm_chi2);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_phi", &ph2TpgPhiEmuAm_phi, &b_ph2TpgPhiEmuAm_phi);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_phiB", &ph2TpgPhiEmuAm_phiB, &b_ph2TpgPhiEmuAm_phiB);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_posLoc_x", &ph2TpgPhiEmuAm_posLoc_x, &b_ph2TpgPhiEmuAm_posLoc_x);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_dirLoc_phi", &ph2TpgPhiEmuAm_dirLoc_phi, &b_ph2TpgPhiEmuAm_dirLoc_phi);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_BX", &ph2TpgPhiEmuAm_BX, &b_ph2TpgPhiEmuAm_BX);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_t0", &ph2TpgPhiEmuAm_t0, &b_ph2TpgPhiEmuAm_t0);
   fChain->SetBranchAddress("ph2TpgPhiEmuAm_index", &ph2TpgPhiEmuAm_index, &b_ph2TpgPhiEmuAm_index);
   Notify();
}

Bool_t EvDisp::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void EvDisp::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t EvDisp::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef EvDisp_cxx
