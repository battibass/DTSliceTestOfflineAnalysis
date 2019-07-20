#include "DTNtupleBaseAnalyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <iostream>

DTNtupleBaseAnalyzer::DTNtupleBaseAnalyzer(const TString &inFileName) : 
  fChain(0), m_inFile(inFileName,"READ") 
{
  
  fChain = static_cast<TTree*>(m_inFile.Get("dtNtupleProducer/DTTREE"));
  Init(fChain);

}


DTNtupleBaseAnalyzer::~DTNtupleBaseAnalyzer()
{
   m_inFile.Close();
}


Int_t DTNtupleBaseAnalyzer::GetEntry(Long64_t entry)
{
   // Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}


Long64_t DTNtupleBaseAnalyzer::LoadTree(Long64_t entry)
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


void DTNtupleBaseAnalyzer::Init(TTree *tree)
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

Bool_t DTNtupleBaseAnalyzer::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void DTNtupleBaseAnalyzer::Loop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
   }
}
