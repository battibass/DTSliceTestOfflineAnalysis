//////////////////////////////////
// TO COMPILE (in root shell)
// .L DTNtupleBaseAnalyzer.C++ 
// .L EvDisp.C++ 
//
// TO RUN (in root shell)
// auto evtDisplay = EvDisp(inputFile);
// evtDisplay.Loop(); // all events 
// evtDisplay.Loop(evt_number); // one event 
// evtDisplay.LoopEntry(entry); // one entry 
// evtDisplay.Loop(start, stop); // evt range 
// E.G.:
// auto evtDisplay = EvDisp("/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/ntuples/DTDPGNtuple_run329806.root", "out.root");
// evtDisplay.Loop(0, 10);
//////////////////////////////////


#include "EvDisp.h"

EvDisp::EvDisp(const TString & inFileName) :
  DTNtupleBaseAnalyzer(inFileName)
  {

  }

EvDisp::~EvDisp() 
{ 

}

void EvDisp::Loop()
{

  book();
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  for(Long64_t jentry=0; jentry<nentries;jentry++) 
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEvent(jentry);
    nbytes += nb;
    if(jentry % 100 == 0) 
      std::cout << "[EvDisp::Loop] processed : "<< jentry << " entries\r" << std::flush;

    fill();
  }
  std::cout << std::endl;
  endJob();

}

void EvDisp::Loop(Long64_t evt)
{

  book();
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  for(Long64_t jentry=0; jentry<nentries;jentry++) 
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEvent(jentry);
    nbytes += nb;
    if(event_eventNumber != evt){
      continue;
    }else{
      std::cout << "[EvDisp::Loop] processing event  : "<< evt << "\r";
      fill();
      break;
    }
    std::cout << "[EvDisp::Loop] event  : "<< evt << "not found\r";
  }
  std::cout << std::endl;
  endJob();

}

void EvDisp::LoopEntry(Long64_t entry)
{

  book();
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  Long64_t nbytes = 0, nb = 0;
  if(entry>=nentries) entry = nentries -1;
  Long64_t ientry = LoadTree(entry);
  if (ientry < 0) return;
  nb = fChain->GetEvent(entry);
  nbytes += nb;
  std::cout << "[EvDisp::Loop] processing entry : "<< entry << "\r";
  fill();
  std::cout << std::endl; 
  endJob();

}

void EvDisp::Loop(Long64_t start, Long64_t stop)
{

  book();
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  if(start>=nentries) {std::cout<<"start>=nentries"<<std::endl; return;}
  if(start<0) start = 0;
  if(stop>=nentries) stop = nentries -1;

  Long64_t nbytes = 0, nb = 0;
  for(Long64_t jentry=start; jentry<stop;jentry++) 
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEvent(jentry);
    nbytes += nb;

    if(jentry % 100 == 0) 
      std::cout << "[EvDisp::Loop] processed : "<< jentry << " entries\r" << std::flush;
    fill();
  }

  std::cout << std::endl;
  endJob();

}

void EvDisp::book()
{
  std::cout<<"[EvDisp::book] start"<<std::endl;

  // m_2Dplots["timecomp"] = new TH2F("timecomp","ph2 time vs legacy",500,2200,3200,500,82000,83000);

  zSL1 = computeY(2.5);   // middle of SL1
  zSL3 = computeY(10.5);  // middle of SL3

  std::vector<float> xStruct, yStruct;
  std::vector<float> exStruct, eyStruct;

  for(unsigned int i=1; i<=60; i++) {
    for(unsigned int j=1; j<=12; j++){
      xStruct.push_back(computeX(i,j));
      yStruct.push_back(computeY(j));
      exStruct.push_back(cellSizeX/2);
      eyStruct.push_back(cellSizeY/2); 
    }
  }

  graphStruct = new TGraphErrors(xStruct.size(),&xStruct[0],&yStruct[0],&exStruct[0],&eyStruct[0]);

  std::cout<<"[EvDisp::book] end"<<std::endl;

}

void EvDisp::fill()
{
  bool debug = false;
  if(debug) std::cout<<std::endl;
  if(debug) std::cout<<"digi"<<std::endl;
  // DIGI
  std::vector<float> xPhiLeg, yPhiLeg, xPhiPh2, yPhiPh2;
  std::vector<float> xEtaLeg, yEtaLeg, xEtaPh2, yEtaPh2;

  for(unsigned int idigi=0; idigi<digi_nDigis; idigi++) {
    if(digi_sector->at(idigi)!=12 || digi_wheel->at(idigi)!=2) continue; 
    float x=digi_wire->at(idigi);
    float y=digi_layer->at(idigi) + 4*(digi_superLayer->at(idigi)-1);

    if(digi_superLayer->at(idigi) == 2){
      xEtaLeg.push_back(computeX(x,y));
      yEtaLeg.push_back(computeY(y));
    }else{
      xPhiLeg.push_back(computeX(x,y));
      yPhiLeg.push_back(computeY(y));
    }

    // for(unsigned int idigi2=0; idigi2<ph2Digi_nDigis; idigi2++) {
    //   if (ph2Digi_superLayer->at(idigi2)==digi_superLayer->at(idigi) &&
    //       ph2Digi_layer->at(idigi2)==digi_layer->at(idigi) &&
    //       ph2Digi_wire->at(idigi2)==digi_wire->at(idigi) )
    //   {
    //     m_2Dplots["timecomp"]->Fill(digi_time->at(idigi),ph2Digi_time->at(idigi2));
    //   }
    // }
  }

  for(unsigned int idigi=0; idigi<ph2Digi_nDigis; idigi++) {
    if(ph2Digi_sector->at(idigi)!=12 || ph2Digi_wheel->at(idigi)!=2) continue;
    float x=ph2Digi_wire->at(idigi);
    float y=ph2Digi_layer->at(idigi) + 4*(ph2Digi_superLayer->at(idigi)-1);

    if(ph2Digi_superLayer->at(idigi)== 2){
      xEtaPh2.push_back(computeX(x,y));
      yEtaPh2.push_back(computeY(y));
    }else{
      xPhiPh2.push_back(computeX(x,y));
      yPhiPh2.push_back(computeY(y));
    }
  }

  if(debug) std::cout<<"segment"<<std::endl;
  // SEGMENTS
  //Legacy
  int nSegLeg = 0;
  TF1 **segments_LegSL1 = new TF1*[seg_nSegments];
  TF1 **segments_LegSL3 = new TF1*[seg_nSegments];

  for(unsigned int iSeg=0; iSeg<seg_nSegments; iSeg++){
    if(seg_sector->at(iSeg)!=12 || seg_wheel->at(iSeg)!=2) continue;
    if(!seg_hasPhi->at(iSeg)) continue;
    nSegLeg++;

    if(debug){
      std::cout<<std::endl;
      std::cout<<"iSeg "<<iSeg<<std::endl;
      for(int iHit=0; iHit<seg_phi_nHits->at(iSeg); iHit++){
        std::cout<<getXY<float>(seg_phiHits_pos,iSeg,iHit)<<" ";
        std::cout<<getXY<float>(seg_phiHits_posCh,iSeg,iHit)<<" ";
        std::cout<<getXY<float>(seg_phiHits_wire,iSeg,iHit)<<" ";
        std::cout<<getXY<float>(seg_phiHits_wirePos,iSeg,iHit)<<" ";
        std::cout<<getXY<float>(seg_phiHits_layer,iSeg,iHit)<<" ";
        std::cout<<getXY<float>(seg_phiHits_superLayer,iSeg,iHit)<<" ";
        std::cout<<std::endl;
      }
    }

    double x11 = x0chamber + seg_posLoc_x_SL1->at(iSeg);
    double z11 = zSL1;
    double x12 = x11 + seg_dirLoc_x->at(iSeg);
    double z12 = z11 - seg_dirLoc_z->at(iSeg); //z is pointed downwards

    double m1 = computeM(x11, x12, z11, z12);
    double q1 = computeQ(x11, x12, z11, z12);
    double range1 = 2*cellSizeY/m1;

    double x31 = x0chamber + seg_posLoc_x_SL3->at(iSeg);
    double z31 = zSL3;
    double x32 = x31 + seg_dirLoc_x->at(iSeg);
    double z32 = z31 - seg_dirLoc_z->at(iSeg); //z is pointed downwards

    double m3 = computeM(x31, x32, z31, z32);
    double q3 = computeQ(x31, x32, z31, z32);
    double range3 = 2*cellSizeY/m3;

    segments_LegSL1[iSeg] = new TF1(Form("segLegSL1%i",iSeg),"[0]+[1]*x", x11-range1, x11+range1);
    segments_LegSL1[iSeg]->SetParameters(q1,m1);

    segments_LegSL3[iSeg] = new TF1(Form("segLegSL3%i",iSeg),"[0]+[1]*x", x31-range3, x31+range3);
    segments_LegSL3[iSeg]->SetParameters(q3,m3);
  }

  //Phase 2
  // int nSegPh2 = 0;
  // TF1 **segments_Ph2SL1 = new TF1*[ph2Seg_nSegments];
  // TF1 **segments_Ph2Sl3 = new TF1*[ph2Seg_nSegments];

  // for(unsigned int iSeg=0; iSeg<ph2Seg_nSegments; iSeg++){
  //   if(ph2Seg_sector->at(iSeg)!=12 || ph2Seg_wheel->at(iSeg)!=2) continue;
  //   if(!ph2Seg_hasPhi->at(iSeg)) continue;
  //   nSegPh2++;

  //   double x11 = x0chamber + ph2Seg_posLoc_x_SL1->at(iSeg);
  //   double z11 = zSL1;
  //   double x12 = x11 + ph2Seg_dirLoc_x->at(iSeg);
  //   double z12 = z11 - ph2Seg_dirLoc_z->at(iSeg); //z is pointed downwards

  //   double m1 = computeM(x11, x12, z11, z12);
  //   double q1 = computeQ(x11, x12, z11, z12);
  //   double range1 = 2*cellSizeY/m1;

  //   double x31 = x0chamber + ph2Seg_posLoc_x_SL3->at(iSeg);
  //   double z31 = zSL3;
  //   double x32 = x31 + ph2Seg_dirLoc_x->at(iSeg);
  //   double z32 = z31 - ph2Seg_dirLoc_z->at(iSeg); //z is pointed downwards

  //   double m3 = computeM(x31, x32, z31, z32);
  //   double q3 = computeQ(x31, x32, z31, z32);
  //   double range3 = 2*cellSizeY/m3;

  //   segments_Ph2SL1[iSeg] = new TF1(Form("segPh2SL1%i",iSeg),"[0]+[1]*x", x11-range1, x11+range1);
  //   segments_Ph2SL1[iSeg]->SetParameters(q1,m1);

  //   segments_Ph2SL3[iSeg] = new TF1(Form("segPh2SL3%i",iSeg),"[0]+[1]*x", x31-range3, x31+range3);
  //   segments_Ph2SL3[iSeg]->SetParameters(q3,m3);
  // }

  // PLOTTING
  if(debug) std::cout<<"plotting"<<std::endl;
  TCanvas* c3 = new TCanvas();
  c3->Divide(1,2);
  c3->cd(1);
  graphStruct->SetMarkerStyle(1);
  graphStruct->SetTitle("Legacy");
  graphStruct->Draw("AP||");

  if(xPhiLeg.size()>0){
    TGraph* graphPhi_Legacy = new TGraph(xPhiLeg.size(),&xPhiLeg[0],&yPhiLeg[0]);
    graphPhi_Legacy->SetMarkerStyle(20);
    graphPhi_Legacy->SetMarkerSize(0.5);
    graphPhi_Legacy->SetMarkerColor(kRed);
    graphPhi_Legacy->Draw("PSAME");
  } 
  if(xEtaLeg.size()>0){
    TGraph* graphEta_Legacy = new TGraph(xEtaLeg.size(),&xEtaLeg[0],&yEtaLeg[0]);
    graphEta_Legacy->SetMarkerStyle(20);
    graphEta_Legacy->SetMarkerSize(0.5);
    graphEta_Legacy->SetMarkerColor(kOrange);
    graphEta_Legacy->Draw("PSAME");
  }

  for(int i=0;i<nSegLeg;i++){
    segments_LegSL1[i]->Draw("SAME");
    segments_LegSL3[i]->Draw("SAME");
  }

  c3->cd(2);
  TGraphErrors *graphStruct_ = (TGraphErrors*)graphStruct->Clone();
  graphStruct_->SetTitle("Phase2");
  graphStruct_->Draw("AP||");

  if(xPhiPh2.size()>0){
    TGraph* graphPhi_Ph2 = new TGraph(xPhiPh2.size(),&xPhiPh2[0],&yPhiPh2[0]);
    graphPhi_Ph2->SetMarkerStyle(20);
    graphPhi_Ph2->SetMarkerSize(0.5);
    graphPhi_Ph2->SetMarkerColor(kBlue);
    graphPhi_Ph2->Draw("PSAME");
  }
  if(xEtaPh2.size()>0){
    TGraph* graphEta_Ph2 = new TGraph(xEtaPh2.size(),&xEtaPh2[0],&yEtaPh2[0]);
    graphEta_Ph2->SetMarkerStyle(20);
    graphEta_Ph2->SetMarkerSize(0.5);
    graphEta_Ph2->SetMarkerColor(kAzure);
    graphEta_Ph2->Draw("PSAME");
  }

  // for(int i=0;i<nSegPh2;i++){
  //   segments_Ph2SL1[i]->SetLineColor(kBlue);
  //   segments_Ph2SL3[i]->SetLineColor(kBlue);
  //   segments_Ph2SL1[i]->Draw("SAME");
  //   segments_Ph2SL3[i]->Draw("SAME");
  // }

  c3->Update();
  c3->Print(Form("evDispPlots/display_run%i_evt%i.png", event_runNumber, (int)event_eventNumber));

}

void EvDisp::endJob()
{
  std::cout<<"[EvDisp::endJob] start"<<std::endl;

  // TCanvas* c2 = new TCanvas();
  // gStyle->SetOptStat(0);
  // gStyle->SetPalette(1);
  // m_2Dplots["timecomp"]->Draw("colz");
  // c2->Update();
  // c2->Print("evDisplPlot/time.png");

  std::cout<<"[EvDisp::endJob] end"<<std::endl;
}


float EvDisp::computeX(float x, int y)
{
  x = cellSizeX*x;
  if(y%2 == 1) x += cellSizeX/2;  // Layer stagger
  if(y >= 9) x += cellSizeX;      // SL3 Stagger (station dependend)
  return x;
}

float EvDisp::computeY(float y)
{
  if(y>4 && y<9) y = cellSizeY/2+cellSizeY*y + honeySize/2;
  else if (y>=9) y = cellSizeY/2+cellSizeY*y + honeySize;
  else y = cellSizeY/2+cellSizeY*y;
  return y;
}

double EvDisp::computeQ(double x1, double x2, double y1, double y2)
{
  return (x1*y2-x2*y1)/(x1-x2);
}

double EvDisp::computeM(double x1, double x2, double y1, double y2)
{
  return (y1-y2)/(x1-x2);
}
