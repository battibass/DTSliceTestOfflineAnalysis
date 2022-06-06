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
#include <vector>
#include <TClonesArray.h>

class WireId {
public:
  WireId() : m_chamb(0), m_sl(0), m_layer(0), m_wire(0){};

  WireId(int chamb, int sl, int layer, int wire) : m_chamb(chamb), m_sl(sl), m_layer(layer), m_wire(wire){};

  int m_chamb;
  int m_sl;
  int m_layer;
  int m_wire;

  bool operator<(const WireId &rhs) const {
    return std::tie(m_chamb, m_sl, m_layer, m_wire) < std::tie(rhs.m_chamb, rhs.m_sl, rhs.m_layer, rhs.m_wire);
  };

  bool operator==(const WireId &rhs) const {
    return std::tie(m_chamb, m_sl, m_layer, m_wire) == std::tie(rhs.m_chamb, rhs.m_sl, rhs.m_layer, rhs.m_wire);
  };
};

class DTNtupleDigi {
public:
  DTNtupleDigi() {
    nDigis = nullptr;
    wheel = nullptr;
    sector = nullptr;
    station = nullptr;
    superLayer = nullptr;
    layer = nullptr;
    wire = nullptr;
    time = nullptr;
  };

  DTNtupleDigi(UInt_t &nDigis,
               std::vector<short> *wheel,
               std::vector<short> *sector,
               std::vector<short> *station,
               std::vector<short> *superLayer,
               std::vector<short> *layer,
               std::vector<short> *wire,
               std::vector<float> *time)
      : nDigis(&nDigis),
        wheel(wheel),
        sector(sector),
        station(station),
        superLayer(superLayer),
        layer(layer),
        wire(wire),
        time(time){};

  ~DTNtupleDigi(){};

  UInt_t *nDigis;
  std::vector<short> *wheel;
  std::vector<short> *sector;
  std::vector<short> *station;
  std::vector<short> *superLayer;
  std::vector<short> *layer;
  std::vector<short> *wire;
  std::vector<float> *time;
};

class DTNtupleSegment {
public:
  DTNtupleSegment() {
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

  DTNtupleSegment(UInt_t &nSegments,
                  std::vector<short> *wheel,
                  std::vector<short> *sector,
                  std::vector<short> *station,
                  std::vector<short> *hasPhi,
                  std::vector<short> *hasZed,

                  std::vector<float> *posLoc_x,
                  std::vector<float> *posLoc_y,
                  std::vector<float> *posLoc_z,
                  std::vector<float> *dirLoc_x,
                  std::vector<float> *dirLoc_y,
                  std::vector<float> *dirLoc_z,
                  std::vector<float> *posLoc_x_SL1,
                  std::vector<float> *posLoc_x_SL3,
                  std::vector<float> *posLoc_x_midPlane,
                  std::vector<float> *posGlb_phi,
                  std::vector<float> *posGlb_eta,
                  std::vector<float> *dirGlb_phi,
                  std::vector<float> *dirGlb_eta,

                  TClonesArray *hitsExpPos,
                  TClonesArray *hitsExpPosCh,
                  TClonesArray *hitsExpWire,

                  std::vector<float> *phi_t0,
                  std::vector<float> *phi_vDrift,
                  std::vector<float> *phi_normChi2,
                  std::vector<short> *phi_nHits,

                  TClonesArray *phiHits_pos,
                  TClonesArray *phiHits_posCh,
                  TClonesArray *phiHits_posErr,
                  TClonesArray *phiHits_side,
                  TClonesArray *phiHits_wire,
                  TClonesArray *phiHits_wirePos,
                  TClonesArray *phiHits_layer,
                  TClonesArray *phiHits_superLayer,
                  TClonesArray *phiHits_time,
                  TClonesArray *phiHits_timeCali,

                  std::vector<float> *z_normChi2,
                  std::vector<short> *z_nHits,

                  TClonesArray *zHits_pos,
                  TClonesArray *zHits_posCh,
                  TClonesArray *zHits_posErr,
                  TClonesArray *zHits_side,
                  TClonesArray *zHits_wire,
                  TClonesArray *zHits_wirePos,
                  TClonesArray *zHits_layer,
                  TClonesArray *zHits_time,
                  TClonesArray *zHits_timeCali)
      : nSegments(&nSegments),
        wheel(wheel),
        sector(sector),
        station(station),
        hasPhi(hasPhi),
        hasZed(hasZed),
        posLoc_x(posLoc_x),
        posLoc_y(posLoc_y),
        posLoc_z(posLoc_z),
        dirLoc_x(dirLoc_x),
        dirLoc_y(dirLoc_y),
        dirLoc_z(dirLoc_z),
        posLoc_x_SL1(posLoc_x_SL1),
        posLoc_x_SL3(posLoc_x_SL3),
        posLoc_x_midPlane(posLoc_x_midPlane),
        posGlb_phi(posGlb_phi),
        posGlb_eta(posGlb_eta),
        dirGlb_phi(dirGlb_phi),
        dirGlb_eta(dirGlb_eta),
        hitsExpPos(hitsExpPos),
        hitsExpPosCh(hitsExpPosCh),
        hitsExpWire(hitsExpWire),
        phi_t0(phi_t0),
        phi_vDrift(phi_vDrift),
        phi_normChi2(phi_normChi2),
        phi_nHits(phi_nHits),
        phiHits_pos(phiHits_pos),
        phiHits_posCh(phiHits_posCh),
        phiHits_posErr(phiHits_posErr),
        phiHits_side(phiHits_side),
        phiHits_wire(phiHits_wire),
        phiHits_wirePos(phiHits_wirePos),
        phiHits_layer(phiHits_layer),
        phiHits_superLayer(phiHits_superLayer),
        phiHits_time(phiHits_time),
        phiHits_timeCali(phiHits_timeCali),
        z_normChi2(z_normChi2),
        z_nHits(z_nHits),
        zHits_pos(zHits_pos),
        zHits_posCh(zHits_posCh),
        zHits_posErr(zHits_posErr),
        zHits_side(zHits_side),
        zHits_wire(zHits_wire),
        zHits_wirePos(zHits_wirePos),
        zHits_layer(zHits_layer),
        zHits_time(zHits_time),
        zHits_timeCali(zHits_timeCali){};

  ~DTNtupleSegment(){};

  UInt_t *nSegments;

  std::vector<short> *wheel;
  std::vector<short> *sector;
  std::vector<short> *station;
  std::vector<short> *hasPhi;
  std::vector<short> *hasZed;

  std::vector<float> *posLoc_x;
  std::vector<float> *posLoc_y;
  std::vector<float> *posLoc_z;
  std::vector<float> *dirLoc_x;
  std::vector<float> *dirLoc_y;
  std::vector<float> *dirLoc_z;
  std::vector<float> *posLoc_x_SL1;
  std::vector<float> *posLoc_x_SL3;
  std::vector<float> *posLoc_x_midPlane;
  std::vector<float> *posGlb_phi;
  std::vector<float> *posGlb_eta;
  std::vector<float> *dirGlb_phi;
  std::vector<float> *dirGlb_eta;

  TClonesArray *hitsExpPos;
  TClonesArray *hitsExpPosCh;
  TClonesArray *hitsExpWire;

  std::vector<float> *phi_t0;
  std::vector<float> *phi_vDrift;
  std::vector<float> *phi_normChi2;
  std::vector<short> *phi_nHits;

  TClonesArray *phiHits_pos;
  TClonesArray *phiHits_posCh;
  TClonesArray *phiHits_posErr;
  TClonesArray *phiHits_side;
  TClonesArray *phiHits_wire;
  TClonesArray *phiHits_wirePos;
  TClonesArray *phiHits_layer;
  TClonesArray *phiHits_superLayer;
  TClonesArray *phiHits_time;
  TClonesArray *phiHits_timeCali;

  std::vector<float> *z_normChi2;
  std::vector<short> *z_nHits;

  TClonesArray *zHits_pos;
  TClonesArray *zHits_posCh;
  TClonesArray *zHits_posErr;
  TClonesArray *zHits_side;
  TClonesArray *zHits_wire;
  TClonesArray *zHits_wirePos;
  TClonesArray *zHits_layer;
  TClonesArray *zHits_time;
  TClonesArray *zHits_timeCali;
};

class DTNtupleBaseAnalyzer {
public:
  static constexpr bool USE_MUONS = false;

  TChain *fChain;  //!pointer to the analyzed TTree or TChain
  Int_t fCurrent;  //!current Tree number in a TChain

  std::map<std::string, DTNtupleDigi> digiObjs;
  std::map<std::string, DTNtupleSegment> segmentObjs;

  // Fixed size dimensions of array or collections stored in the TTree if any.

  // Declaration of leaf types
  Int_t event_runNumber;
  Int_t event_lumiBlock;
  Long64_t event_eventNumber;
  ULong64_t event_timeStamp;
  Int_t event_bunchCrossing;
  Long64_t event_orbitNumber;
  UInt_t gen_nGenParts;
  std::vector<int> *gen_pdgId;
  std::vector<float> *gen_pt;
  std::vector<float> *gen_phi;
  std::vector<float> *gen_eta;
  std::vector<short> *gen_charge;
  Short_t environment_truePileUp;
  Short_t environment_actualPileUp;
  Int_t environment_instLumi;
  Short_t environment_nPV;
  Float_t environment_pv_x;
  Float_t environment_pv_y;
  Float_t environment_pv_z;
  Float_t environment_pv_xxErr;
  Float_t environment_pv_yyErr;
  Float_t environment_pv_zzErr;
  Float_t environment_pv_xyErr;
  Float_t environment_pv_xzErr;
  Float_t environment_pv_yzErr;
  UInt_t digi_nDigis;
  std::vector<short> *digi_wheel;
  std::vector<short> *digi_sector;
  std::vector<short> *digi_station;
  std::vector<short> *digi_superLayer;
  std::vector<short> *digi_layer;
  std::vector<short> *digi_wire;
  std::vector<float> *digi_time;
  UInt_t ph2Digi_nDigis;
  std::vector<short> *ph2Digi_wheel;
  std::vector<short> *ph2Digi_sector;
  std::vector<short> *ph2Digi_station;
  std::vector<short> *ph2Digi_superLayer;
  std::vector<short> *ph2Digi_layer;
  std::vector<short> *ph2Digi_wire;
  std::vector<float> *ph2Digi_time;
  UInt_t seg_nSegments;
  std::vector<short> *seg_wheel;
  std::vector<short> *seg_sector;
  std::vector<short> *seg_station;
  std::vector<short> *seg_hasPhi;
  std::vector<short> *seg_hasZed;
  std::vector<float> *seg_posLoc_x;
  std::vector<float> *seg_posLoc_y;
  std::vector<float> *seg_posLoc_z;
  std::vector<float> *seg_dirLoc_x;
  std::vector<float> *seg_dirLoc_y;
  std::vector<float> *seg_dirLoc_z;
  std::vector<float> *seg_posLoc_x_SL1;
  std::vector<float> *seg_posLoc_x_SL3;
  std::vector<float> *seg_posLoc_x_midPlane;
  std::vector<float> *seg_posGlb_phi;
  std::vector<float> *seg_posGlb_eta;
  std::vector<float> *seg_dirGlb_phi;
  std::vector<float> *seg_dirGlb_eta;
  TClonesArray *seg_hitsExpPos;
  TClonesArray *seg_hitsExpPosCh;
  TClonesArray *seg_hitsExpWire;
  std::vector<float> *seg_phi_t0;
  std::vector<float> *seg_phi_vDrift;
  std::vector<float> *seg_phi_normChi2;
  std::vector<short> *seg_phi_nHits;
  TClonesArray *seg_phiHits_pos;
  TClonesArray *seg_phiHits_posCh;
  TClonesArray *seg_phiHits_posErr;
  TClonesArray *seg_phiHits_side;
  TClonesArray *seg_phiHits_wire;
  TClonesArray *seg_phiHits_wirePos;
  TClonesArray *seg_phiHits_layer;
  TClonesArray *seg_phiHits_superLayer;
  TClonesArray *seg_phiHits_time;
  TClonesArray *seg_phiHits_timeCali;
  std::vector<float> *seg_z_normChi2;
  std::vector<short> *seg_z_nHits;
  TClonesArray *seg_zHits_pos;
  TClonesArray *seg_zHits_posCh;
  TClonesArray *seg_zHits_posErr;
  TClonesArray *seg_zHits_side;
  TClonesArray *seg_zHits_wire;
  TClonesArray *seg_zHits_wirePos;
  TClonesArray *seg_zHits_layer;
  TClonesArray *seg_zHits_time;
  TClonesArray *seg_zHits_timeCali;
  UInt_t ph2Seg_nSegments;
  std::vector<short> *ph2Seg_wheel;
  std::vector<short> *ph2Seg_sector;
  std::vector<short> *ph2Seg_station;
  std::vector<short> *ph2Seg_hasPhi;
  std::vector<short> *ph2Seg_hasZed;
  std::vector<float> *ph2Seg_posLoc_x;
  std::vector<float> *ph2Seg_posLoc_y;
  std::vector<float> *ph2Seg_posLoc_z;
  std::vector<float> *ph2Seg_dirLoc_x;
  std::vector<float> *ph2Seg_dirLoc_y;
  std::vector<float> *ph2Seg_dirLoc_z;
  std::vector<float> *ph2Seg_posLoc_x_SL1;
  std::vector<float> *ph2Seg_posLoc_x_SL3;
  std::vector<float> *ph2Seg_posLoc_x_midPlane;
  std::vector<float> *ph2Seg_posGlb_phi;
  std::vector<float> *ph2Seg_posGlb_eta;
  std::vector<float> *ph2Seg_dirGlb_phi;
  std::vector<float> *ph2Seg_dirGlb_eta;
  TClonesArray *ph2Seg_hitsExpPos;
  TClonesArray *ph2Seg_hitsExpPosCh;
  TClonesArray *ph2Seg_hitsExpWire;
  std::vector<float> *ph2Seg_phi_t0;
  std::vector<float> *ph2Seg_phi_vDrift;
  std::vector<float> *ph2Seg_phi_normChi2;
  std::vector<short> *ph2Seg_phi_nHits;
  TClonesArray *ph2Seg_phiHits_pos;
  TClonesArray *ph2Seg_phiHits_posCh;
  TClonesArray *ph2Seg_phiHits_posErr;
  TClonesArray *ph2Seg_phiHits_side;
  TClonesArray *ph2Seg_phiHits_wire;
  TClonesArray *ph2Seg_phiHits_wirePos;
  TClonesArray *ph2Seg_phiHits_layer;
  TClonesArray *ph2Seg_phiHits_superLayer;
  TClonesArray *ph2Seg_phiHits_time;
  TClonesArray *ph2Seg_phiHits_timeCali;
  std::vector<float> *ph2Seg_z_normChi2;
  std::vector<short> *ph2Seg_z_nHits;
  TClonesArray *ph2Seg_zHits_pos;
  TClonesArray *ph2Seg_zHits_posCh;
  TClonesArray *ph2Seg_zHits_posErr;
  TClonesArray *ph2Seg_zHits_side;
  TClonesArray *ph2Seg_zHits_wire;
  TClonesArray *ph2Seg_zHits_wirePos;
  TClonesArray *ph2Seg_zHits_layer;
  TClonesArray *ph2Seg_zHits_time;
  TClonesArray *ph2Seg_zHits_timeCali;
  UInt_t mu_nMuons;
  std::vector<float> *mu_pt;
  std::vector<float> *mu_phi;
  std::vector<float> *mu_eta;
  std::vector<short> *mu_charge;
  std::vector<bool> *mu_isGlobal;
  std::vector<bool> *mu_isStandalone;
  std::vector<bool> *mu_isTracker;
  std::vector<bool> *mu_isTrackerArb;
  std::vector<bool> *mu_isRPC;
  std::vector<bool> *mu_firesIsoTrig;
  std::vector<bool> *mu_firesTrig;
  std::vector<bool> *mu_isLoose;
  std::vector<bool> *mu_isMedium;
  std::vector<bool> *mu_isTight;
  std::vector<float> *mu_trkIso03;
  std::vector<float> *mu_pfIso04;
  std::vector<float> *mu_trk_dxy;
  std::vector<float> *mu_trk_dz;
  std::vector<int> *mu_trk_algo;
  std::vector<int> *mu_trk_origAlgo;
  std::vector<int> *mu_trk_numberOfValidPixelHits;
  std::vector<int> *mu_trk_numberOfValidTrackerLayers;
  std::vector<unsigned int> *mu_trkMu_stationMask;
  std::vector<int> *mu_trkMu_numberOfMatchedStations;
  std::vector<int> *mu_trkMu_numberOfMatchedRPCLayers;
  std::vector<int> *mu_staMu_numberOfValidMuonHits;
  std::vector<float> *mu_staMu_normChi2;
  std::vector<float> *mu_glbMu_normChi2;
  std::vector<UInt_t> *mu_nMatches;
  TClonesArray *mu_matches_wheel;
  TClonesArray *mu_matches_sector;
  TClonesArray *mu_matches_station;
  TClonesArray *mu_matches_x;
  TClonesArray *mu_matches_y;
  TClonesArray *mu_matches_phi;
  TClonesArray *mu_matches_eta;
  TClonesArray *mu_matches_edgeX;
  TClonesArray *mu_matches_edgeY;
  TClonesArray *mu_matches_dXdZ;
  TClonesArray *mu_matches_dYdZ;
  std::vector<unsigned int> *mu_staMu_nMatchSeg;
  TClonesArray *mu_staMu_matchSegIdx;
  UInt_t ltTwinMuxIn_nTrigs;
  std::vector<short> *ltTwinMuxIn_wheel;
  std::vector<short> *ltTwinMuxIn_sector;
  std::vector<short> *ltTwinMuxIn_station;
  std::vector<short> *ltTwinMuxIn_quality;
  std::vector<int> *ltTwinMuxIn_phi;
  std::vector<int> *ltTwinMuxIn_phiB;
  std::vector<float> *ltTwinMuxIn_posLoc_x;
  std::vector<float> *ltTwinMuxIn_dirLoc_phi;
  std::vector<short> *ltTwinMuxIn_BX;
  std::vector<short> *ltTwinMuxIn_is2nd;
  UInt_t ltTwinMuxOut_nTrigs;
  std::vector<short> *ltTwinMuxOut_wheel;
  std::vector<short> *ltTwinMuxOut_sector;
  std::vector<short> *ltTwinMuxOut_station;
  std::vector<short> *ltTwinMuxOut_quality;
  std::vector<short> *ltTwinMuxOut_rpcBit;
  std::vector<int> *ltTwinMuxOut_phi;
  std::vector<int> *ltTwinMuxOut_phiB;
  std::vector<float> *ltTwinMuxOut_posLoc_x;
  std::vector<float> *ltTwinMuxOut_dirLoc_phi;
  std::vector<short> *ltTwinMuxOut_BX;
  std::vector<short> *ltTwinMuxOut_is2nd;
  UInt_t ltBmtfIn_nTrigs;
  std::vector<short> *ltBmtfIn_wheel;
  std::vector<short> *ltBmtfIn_sector;
  std::vector<short> *ltBmtfIn_station;
  std::vector<short> *ltBmtfIn_quality;
  std::vector<int> *ltBmtfIn_phi;
  std::vector<int> *ltBmtfIn_phiB;
  std::vector<float> *ltBmtfIn_posLoc_x;
  std::vector<float> *ltBmtfIn_dirLoc_phi;
  std::vector<short> *ltBmtfIn_BX;
  std::vector<short> *ltBmtfIn_is2nd;
  UInt_t ltTwinMuxInTh_nTrigs;
  std::vector<short> *ltTwinMuxInTh_wheel;
  std::vector<short> *ltTwinMuxInTh_sector;
  std::vector<short> *ltTwinMuxInTh_station;
  std::vector<short> *ltTwinMuxInTh_BX;
  std::vector<unsigned short> *ltTwinMuxInTh_hitMap;
  UInt_t ltBmtfInTh_nTrigs;
  std::vector<short> *ltBmtfInTh_wheel;
  std::vector<short> *ltBmtfInTh_sector;
  std::vector<short> *ltBmtfInTh_station;
  std::vector<short> *ltBmtfInTh_BX;
  std::vector<unsigned short> *ltBmtfInTh_hitMap;
  UInt_t tfBmtfOut_nBmtfCands;
  std::vector<float> *tfBmtfOut_pt;
  std::vector<float> *tfBmtfOut_phi;
  std::vector<float> *tfBmtfOut_eta;
  std::vector<int> *tfBmtfOut_dxy;
  std::vector<int> *tfBmtfOut_qual;
  std::vector<int> *tfBmtfOut_etaFine;
  TClonesArray *tfBmtfOut_matchedTpIdx;
  UInt_t ph2TpgPhiHw_nTrigs;
  std::vector<short> *ph2TpgPhiHw_wheel;
  std::vector<short> *ph2TpgPhiHw_sector;
  std::vector<short> *ph2TpgPhiHw_station;
  std::vector<short> *ph2TpgPhiHw_quality;
  std::vector<short> *ph2TpgPhiHw_superLayer;
  std::vector<short> *ph2TpgPhiHw_rpcFlag;
  std::vector<int> *ph2TpgPhiHw_chi2;
  std::vector<int> *ph2TpgPhiHw_phi;
  std::vector<int> *ph2TpgPhiHw_phiB;
  std::vector<float> *ph2TpgPhiHw_posLoc_x;
  std::vector<float> *ph2TpgPhiHw_dirLoc_phi;
  std::vector<int> *ph2TpgPhiHw_BX;
  std::vector<int> *ph2TpgPhiHw_t0;
  std::vector<short> *ph2TpgPhiHw_index;
  UInt_t ph2TpgPhiEmuHb_nTrigs;
  std::vector<short> *ph2TpgPhiEmuHb_wheel;
  std::vector<short> *ph2TpgPhiEmuHb_sector;
  std::vector<short> *ph2TpgPhiEmuHb_station;
  std::vector<short> *ph2TpgPhiEmuHb_quality;
  std::vector<short> *ph2TpgPhiEmuHb_superLayer;
  std::vector<short> *ph2TpgPhiEmuHb_rpcFlag;
  std::vector<int> *ph2TpgPhiEmuHb_chi2;
  std::vector<int> *ph2TpgPhiEmuHb_phi;
  std::vector<int> *ph2TpgPhiEmuHb_phiB;
  std::vector<float> *ph2TpgPhiEmuHb_posLoc_x;
  std::vector<float> *ph2TpgPhiEmuHb_dirLoc_phi;
  std::vector<int> *ph2TpgPhiEmuHb_BX;
  std::vector<int> *ph2TpgPhiEmuHb_t0;
  std::vector<short> *ph2TpgPhiEmuHb_index;
  UInt_t ph2TpgPhiEmuAm_nTrigs;
  std::vector<short> *ph2TpgPhiEmuAm_wheel;
  std::vector<short> *ph2TpgPhiEmuAm_sector;
  std::vector<short> *ph2TpgPhiEmuAm_station;
  std::vector<short> *ph2TpgPhiEmuAm_quality;
  std::vector<short> *ph2TpgPhiEmuAm_superLayer;
  std::vector<short> *ph2TpgPhiEmuAm_rpcFlag;
  std::vector<int> *ph2TpgPhiEmuAm_chi2;
  std::vector<int> *ph2TpgPhiEmuAm_phi;
  std::vector<int> *ph2TpgPhiEmuAm_phiB;
  std::vector<float> *ph2TpgPhiEmuAm_posLoc_x;
  std::vector<float> *ph2TpgPhiEmuAm_dirLoc_phi;
  std::vector<int> *ph2TpgPhiEmuAm_BX;
  std::vector<int> *ph2TpgPhiEmuAm_t0;
  std::vector<short> *ph2TpgPhiEmuAm_index;

  // List of branches
  TBranch *b_event_runNumber;                    //!
  TBranch *b_event_lumiBlock;                    //!
  TBranch *b_event_eventNumber;                  //!
  TBranch *b_event_timeStamp;                    //!
  TBranch *b_event_bunchCrossing;                //!
  TBranch *b_event_orbitNumber;                  //!
  TBranch *b_gen_nGenParts;                      //!
  TBranch *b_gen_pdgId;                          //!
  TBranch *b_gen_pt;                             //!
  TBranch *b_gen_phi;                            //!
  TBranch *b_gen_eta;                            //!
  TBranch *b_gen_charge;                         //!
  TBranch *b_environment_truePileUp;             //!
  TBranch *b_environment_actualPileUp;           //!
  TBranch *b_environment_instLumi;               //!
  TBranch *b_environment_nPV;                    //!
  TBranch *b_environment_pv_x;                   //!
  TBranch *b_environment_pv_y;                   //!
  TBranch *b_environment_pv_z;                   //!
  TBranch *b_environment_pv_xxErr;               //!
  TBranch *b_environment_pv_yyErr;               //!
  TBranch *b_environment_pv_zzErr;               //!
  TBranch *b_environment_pv_xyErr;               //!
  TBranch *b_environment_pv_xzErr;               //!
  TBranch *b_environment_pv_yzErr;               //!
  TBranch *b_digi_nDigis;                        //!
  TBranch *b_digi_wheel;                         //!
  TBranch *b_digi_sector;                        //!
  TBranch *b_digi_station;                       //!
  TBranch *b_digi_superLayer;                    //!
  TBranch *b_digi_layer;                         //!
  TBranch *b_digi_wire;                          //!
  TBranch *b_digi_time;                          //!
  TBranch *b_ph2Digi_nDigis;                     //!
  TBranch *b_ph2Digi_wheel;                      //!
  TBranch *b_ph2Digi_sector;                     //!
  TBranch *b_ph2Digi_station;                    //!
  TBranch *b_ph2Digi_superLayer;                 //!
  TBranch *b_ph2Digi_layer;                      //!
  TBranch *b_ph2Digi_wire;                       //!
  TBranch *b_ph2Digi_time;                       //!
  TBranch *b_seg_nSegments;                      //!
  TBranch *b_seg_wheel;                          //!
  TBranch *b_seg_sector;                         //!
  TBranch *b_seg_station;                        //!
  TBranch *b_seg_hasPhi;                         //!
  TBranch *b_seg_hasZed;                         //!
  TBranch *b_seg_posLoc_x;                       //!
  TBranch *b_seg_posLoc_y;                       //!
  TBranch *b_seg_posLoc_z;                       //!
  TBranch *b_seg_dirLoc_x;                       //!
  TBranch *b_seg_dirLoc_y;                       //!
  TBranch *b_seg_dirLoc_z;                       //!
  TBranch *b_seg_posLoc_x_SL1;                   //!
  TBranch *b_seg_posLoc_x_SL3;                   //!
  TBranch *b_seg_posLoc_x_midPlane;              //!
  TBranch *b_seg_posGlb_phi;                     //!
  TBranch *b_seg_posGlb_eta;                     //!
  TBranch *b_seg_dirGlb_phi;                     //!
  TBranch *b_seg_dirGlb_eta;                     //!
  TBranch *b_seg_hitsExpPos;                     //!
  TBranch *b_seg_hitsExpPosCh;                   //!
  TBranch *b_seg_hitsExpWire;                    //!
  TBranch *b_seg_phi_t0;                         //!
  TBranch *b_seg_phi_vDrift;                     //!
  TBranch *b_seg_phi_normChi2;                   //!
  TBranch *b_seg_phi_nHits;                      //!
  TBranch *b_seg_phiHits_pos;                    //!
  TBranch *b_seg_phiHits_posCh;                  //!
  TBranch *b_seg_phiHits_posErr;                 //!
  TBranch *b_seg_phiHits_side;                   //!
  TBranch *b_seg_phiHits_wire;                   //!
  TBranch *b_seg_phiHits_wirePos;                //!
  TBranch *b_seg_phiHits_layer;                  //!
  TBranch *b_seg_phiHits_superLayer;             //!
  TBranch *b_seg_phiHits_time;                   //!
  TBranch *b_seg_phiHits_timeCali;               //!
  TBranch *b_seg_z_normChi2;                     //!
  TBranch *b_seg_z_nHits;                        //!
  TBranch *b_seg_zHits_pos;                      //!
  TBranch *b_seg_zHits_posCh;                    //!
  TBranch *b_seg_zHits_posErr;                   //!
  TBranch *b_seg_zHits_side;                     //!
  TBranch *b_seg_zHits_wire;                     //!
  TBranch *b_seg_zHits_wirePos;                  //!
  TBranch *b_seg_zHits_layer;                    //!
  TBranch *b_seg_zHits_time;                     //!
  TBranch *b_seg_zHits_timeCali;                 //!
  TBranch *b_ph2Seg_nSegments;                   //!
  TBranch *b_ph2Seg_wheel;                       //!
  TBranch *b_ph2Seg_sector;                      //!
  TBranch *b_ph2Seg_station;                     //!
  TBranch *b_ph2Seg_hasPhi;                      //!
  TBranch *b_ph2Seg_hasZed;                      //!
  TBranch *b_ph2Seg_posLoc_x;                    //!
  TBranch *b_ph2Seg_posLoc_y;                    //!
  TBranch *b_ph2Seg_posLoc_z;                    //!
  TBranch *b_ph2Seg_dirLoc_x;                    //!
  TBranch *b_ph2Seg_dirLoc_y;                    //!
  TBranch *b_ph2Seg_dirLoc_z;                    //!
  TBranch *b_ph2Seg_posLoc_x_SL1;                //!
  TBranch *b_ph2Seg_posLoc_x_SL3;                //!
  TBranch *b_ph2Seg_posLoc_x_midPlane;           //!
  TBranch *b_ph2Seg_posGlb_phi;                  //!
  TBranch *b_ph2Seg_posGlb_eta;                  //!
  TBranch *b_ph2Seg_dirGlb_phi;                  //!
  TBranch *b_ph2Seg_dirGlb_eta;                  //!
  TBranch *b_ph2Seg_hitsExpPos;                  //!
  TBranch *b_ph2Seg_hitsExpPosCh;                //!
  TBranch *b_ph2Seg_hitsExpWire;                 //!
  TBranch *b_ph2Seg_phi_t0;                      //!
  TBranch *b_ph2Seg_phi_vDrift;                  //!
  TBranch *b_ph2Seg_phi_normChi2;                //!
  TBranch *b_ph2Seg_phi_nHits;                   //!
  TBranch *b_ph2Seg_phiHits_pos;                 //!
  TBranch *b_ph2Seg_phiHits_posCh;               //!
  TBranch *b_ph2Seg_phiHits_posErr;              //!
  TBranch *b_ph2Seg_phiHits_side;                //!
  TBranch *b_ph2Seg_phiHits_wire;                //!
  TBranch *b_ph2Seg_phiHits_wirePos;             //!
  TBranch *b_ph2Seg_phiHits_layer;               //!
  TBranch *b_ph2Seg_phiHits_superLayer;          //!
  TBranch *b_ph2Seg_phiHits_time;                //!
  TBranch *b_ph2Seg_phiHits_timeCali;            //!
  TBranch *b_ph2Seg_z_normChi2;                  //!
  TBranch *b_ph2Seg_z_nHits;                     //!
  TBranch *b_ph2Seg_zHits_pos;                   //!
  TBranch *b_ph2Seg_zHits_posCh;                 //!
  TBranch *b_ph2Seg_zHits_posErr;                //!
  TBranch *b_ph2Seg_zHits_side;                  //!
  TBranch *b_ph2Seg_zHits_wire;                  //!
  TBranch *b_ph2Seg_zHits_wirePos;               //!
  TBranch *b_ph2Seg_zHits_layer;                 //!
  TBranch *b_ph2Seg_zHits_time;                  //!
  TBranch *b_ph2Seg_zHits_timeCali;              //!
  TBranch *b_mu_nMuons;                          //!
  TBranch *b_mu_pt;                              //!
  TBranch *b_mu_phi;                             //!
  TBranch *b_mu_eta;                             //!
  TBranch *b_mu_charge;                          //!
  TBranch *b_mu_isGlobal;                        //!
  TBranch *b_mu_isStandalone;                    //!
  TBranch *b_mu_isTracker;                       //!
  TBranch *b_mu_isTrackerArb;                    //!
  TBranch *b_mu_isRPC;                           //!
  TBranch *b_mu_firesIsoTrig;                    //!
  TBranch *b_mu_firesTrig;                       //!
  TBranch *b_mu_isLoose;                         //!
  TBranch *b_mu_isMedium;                        //!
  TBranch *b_mu_isTight;                         //!
  TBranch *b_mu_trkIso03;                        //!
  TBranch *b_mu_pfIso04;                         //!
  TBranch *b_mu_trk_dxy;                         //!
  TBranch *b_mu_trk_dz;                          //!
  TBranch *b_mu_trk_algo;                        //!
  TBranch *b_mu_trk_origAlgo;                    //!
  TBranch *b_mu_trk_numberOfValidPixelHits;      //!
  TBranch *b_mu_trk_numberOfValidTrackerLayers;  //!
  TBranch *b_mu_trkMu_stationMask;               //!
  TBranch *b_mu_trkMu_numberOfMatchedStations;   //!
  TBranch *b_mu_trkMu_numberOfMatchedRPCLayers;  //!
  TBranch *b_mu_staMu_numberOfValidMuonHits;     //!
  TBranch *b_mu_staMu_normChi2;                  //!
  TBranch *b_mu_glbMu_normChi2;                  //!
  TBranch *b_mu_nMatches;                        //!
  TBranch *b_mu_matches_wheel;                   //!
  TBranch *b_mu_matches_sector;                  //!
  TBranch *b_mu_matches_station;                 //!
  TBranch *b_mu_matches_x;                       //!
  TBranch *b_mu_matches_y;                       //!
  TBranch *b_mu_matches_phi;                     //!
  TBranch *b_mu_matches_eta;                     //!
  TBranch *b_mu_matches_edgeX;                   //!
  TBranch *b_mu_matches_edgeY;                   //!
  TBranch *b_mu_matches_dXdZ;                    //!
  TBranch *b_mu_matches_dYdZ;                    //!
  TBranch *b_mu_staMu_nMatchSeg;                 //!
  TBranch *b_mu_staMu_matchSegIdx;               //!
  TBranch *b_ltTwinMuxIn_nTrigs;                 //!
  TBranch *b_ltTwinMuxIn_wheel;                  //!
  TBranch *b_ltTwinMuxIn_sector;                 //!
  TBranch *b_ltTwinMuxIn_station;                //!
  TBranch *b_ltTwinMuxIn_quality;                //!
  TBranch *b_ltTwinMuxIn_phi;                    //!
  TBranch *b_ltTwinMuxIn_phiB;                   //!
  TBranch *b_ltTwinMuxIn_posLoc_x;               //!
  TBranch *b_ltTwinMuxIn_dirLoc_phi;             //!
  TBranch *b_ltTwinMuxIn_BX;                     //!
  TBranch *b_ltTwinMuxIn_is2nd;                  //!
  TBranch *b_ltTwinMuxOut_nTrigs;                //!
  TBranch *b_ltTwinMuxOut_wheel;                 //!
  TBranch *b_ltTwinMuxOut_sector;                //!
  TBranch *b_ltTwinMuxOut_station;               //!
  TBranch *b_ltTwinMuxOut_quality;               //!
  TBranch *b_ltTwinMuxOut_rpcBit;                //!
  TBranch *b_ltTwinMuxOut_phi;                   //!
  TBranch *b_ltTwinMuxOut_phiB;                  //!
  TBranch *b_ltTwinMuxOut_posLoc_x;              //!
  TBranch *b_ltTwinMuxOut_dirLoc_phi;            //!
  TBranch *b_ltTwinMuxOut_BX;                    //!
  TBranch *b_ltTwinMuxOut_is2nd;                 //!
  TBranch *b_ltBmtfIn_nTrigs;                    //!
  TBranch *b_ltBmtfIn_wheel;                     //!
  TBranch *b_ltBmtfIn_sector;                    //!
  TBranch *b_ltBmtfIn_station;                   //!
  TBranch *b_ltBmtfIn_quality;                   //!
  TBranch *b_ltBmtfIn_phi;                       //!
  TBranch *b_ltBmtfIn_phiB;                      //!
  TBranch *b_ltBmtfIn_posLoc_x;                  //!
  TBranch *b_ltBmtfIn_dirLoc_phi;                //!
  TBranch *b_ltBmtfIn_BX;                        //!
  TBranch *b_ltBmtfIn_is2nd;                     //!
  TBranch *b_ltTwinMuxInTh_nTrigs;               //!
  TBranch *b_ltTwinMuxInTh_wheel;                //!
  TBranch *b_ltTwinMuxInTh_sector;               //!
  TBranch *b_ltTwinMuxInTh_station;              //!
  TBranch *b_ltTwinMuxInTh_BX;                   //!
  TBranch *b_ltTwinMuxInTh_hitMap;               //!
  TBranch *b_ltBmtfInTh_nTrigs;                  //!
  TBranch *b_ltBmtfInTh_wheel;                   //!
  TBranch *b_ltBmtfInTh_sector;                  //!
  TBranch *b_ltBmtfInTh_station;                 //!
  TBranch *b_ltBmtfInTh_BX;                      //!
  TBranch *b_ltBmtfInTh_hitMap;                  //!
  TBranch *b_tfBmtfOut_nBmtfCands;               //!
  TBranch *b_tfBmtfOut_pt;                       //!
  TBranch *b_tfBmtfOut_phi;                      //!
  TBranch *b_tfBmtfOut_eta;                      //!
  TBranch *b_tfBmtfOut_dxy;                      //!
  TBranch *b_tfBmtfOut_qual;                     //!
  TBranch *b_tfBmtfOut_etaFine;                  //!
  TBranch *b_tfBmtfOut_matchedTpIdx;             //!
  TBranch *b_ph2TpgPhiHw_nTrigs;                 //!
  TBranch *b_ph2TpgPhiHw_wheel;                  //!
  TBranch *b_ph2TpgPhiHw_sector;                 //!
  TBranch *b_ph2TpgPhiHw_station;                //!
  TBranch *b_ph2TpgPhiHw_quality;                //!
  TBranch *b_ph2TpgPhiHw_superLayer;             //!
  TBranch *b_ph2TpgPhiHw_rpcFlag;                //!
  TBranch *b_ph2TpgPhiHw_chi2;                   //!
  TBranch *b_ph2TpgPhiHw_phi;                    //!
  TBranch *b_ph2TpgPhiHw_phiB;                   //!
  TBranch *b_ph2TpgPhiHw_posLoc_x;               //!
  TBranch *b_ph2TpgPhiHw_dirLoc_phi;             //!
  TBranch *b_ph2TpgPhiHw_BX;                     //!
  TBranch *b_ph2TpgPhiHw_t0;                     //!
  TBranch *b_ph2TpgPhiHw_index;                  //!
  TBranch *b_ph2TpgPhiEmuHb_nTrigs;              //!
  TBranch *b_ph2TpgPhiEmuHb_wheel;               //!
  TBranch *b_ph2TpgPhiEmuHb_sector;              //!
  TBranch *b_ph2TpgPhiEmuHb_station;             //!
  TBranch *b_ph2TpgPhiEmuHb_quality;             //!
  TBranch *b_ph2TpgPhiEmuHb_superLayer;          //!
  TBranch *b_ph2TpgPhiEmuHb_rpcFlag;             //!
  TBranch *b_ph2TpgPhiEmuHb_chi2;                //!
  TBranch *b_ph2TpgPhiEmuHb_phi;                 //!
  TBranch *b_ph2TpgPhiEmuHb_phiB;                //!
  TBranch *b_ph2TpgPhiEmuHb_posLoc_x;            //!
  TBranch *b_ph2TpgPhiEmuHb_dirLoc_phi;          //!
  TBranch *b_ph2TpgPhiEmuHb_BX;                  //!
  TBranch *b_ph2TpgPhiEmuHb_t0;                  //!
  TBranch *b_ph2TpgPhiEmuHb_index;               //!
  TBranch *b_ph2TpgPhiEmuAm_nTrigs;              //!
  TBranch *b_ph2TpgPhiEmuAm_wheel;               //!
  TBranch *b_ph2TpgPhiEmuAm_sector;              //!
  TBranch *b_ph2TpgPhiEmuAm_station;             //!
  TBranch *b_ph2TpgPhiEmuAm_quality;             //!
  TBranch *b_ph2TpgPhiEmuAm_superLayer;          //!
  TBranch *b_ph2TpgPhiEmuAm_rpcFlag;             //!
  TBranch *b_ph2TpgPhiEmuAm_chi2;                //!
  TBranch *b_ph2TpgPhiEmuAm_phi;                 //!
  TBranch *b_ph2TpgPhiEmuAm_phiB;                //!
  TBranch *b_ph2TpgPhiEmuAm_posLoc_x;            //!
  TBranch *b_ph2TpgPhiEmuAm_dirLoc_phi;          //!
  TBranch *b_ph2TpgPhiEmuAm_BX;                  //!
  TBranch *b_ph2TpgPhiEmuAm_t0;                  //!
  TBranch *b_ph2TpgPhiEmuAm_index;               //!

  DTNtupleBaseAnalyzer(const TString &fileName);
  DTNtupleBaseAnalyzer(const std::vector<TString> &fileNames);
  virtual ~DTNtupleBaseAnalyzer();
  virtual Int_t GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void Init(TChain *tree);
  virtual void Loop();
  virtual void LoadObjs();
  virtual Bool_t Notify();

protected:
  virtual void book(){};
  virtual void fill(){};
  virtual void endJob(){};

  template <typename T>
  T getXY(TClonesArray *arr, int x, int y) {
    return static_cast<T>((*((TVectorT<float> *)(arr->At(x))))[y]);
  };
};

#endif
