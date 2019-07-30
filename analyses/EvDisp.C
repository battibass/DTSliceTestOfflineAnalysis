//////////////////////////////////
//
// Author: Alberto Bragagnolo alberto.bragagnolo@cern.ch
//
// TO COMPILE (in root shell)
// .L DTNtupleBaseAnalyzer.C++ 
// .L EvDisp.C++ 
//
// TO RUN (in root shell)
// auto evtDisplay = EvDisp(inputFile);
// evtDisplay.Loop(); // all events 
// evtDisplay.Loop(evt_number); // one event 
// evtDisplay.LoopEntry(entry); // one entry 
// evtDisplay.Loop(start, stop); // entries range 
// evtDisplay.Loop(start, stop, evt); // one event, searched only in the entries range 
// E.G.:
// auto evtDisplay = EvDisp("/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/ntuples/DTDPGNtuple_run329806.root", "out.root");
// evtDisplay.Loop(0, 10);
//
// evtDisplay.DumpOn() abilitate digi dump output (default is disabilitate)
// evtDisplay.DumpOff() disabilitate digi dump output 
//
//////////////////////////////////


#include "EvDisp.h"

EvDisp::EvDisp(const TString & inFileName) :
  DTNtupleBaseAnalyzer(inFileName), dumpFlag(0)
{
  cout<<endl;
  cout<<"INSTRUCTIONS"<<endl;
  cout<<endl;
  cout<<"evtDisplay.Loop(); // all events "<<endl;
  cout<<"evtDisplay.Loop(evt_number); // one event"<<endl;
  cout<<"evtDisplay.LoopEntry(entry); // one entry"<<endl;
  cout<<"evtDisplay.Loop(start, stop); // entries range"<<endl;
  cout<<"evtDisplay.Loop(start, stop, evt); // one event, searched only in the entries range"<<endl;
  cout<<endl;
  cout<<"evtDisplay.DumpOn(); // abilitate dump to screen digi information "<<endl;
  cout<<"evtDisplay.DumpOff(); // disabilitate dump to screen digi information (default)"<<endl;
  cout<<endl;
}

EvDisp::~EvDisp() 
{ 

}

void EvDisp::Loop(Long64_t start, Long64_t stop, Long64_t evt = -1)
{

  cout<<"[EvDisp::Loop] start"<<endl;
  book();
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();

  if(start>=nentries){
    cout<<"[EvDisp::Loop] start>=nentries --> start = 0"<<endl;
    start = 0;
  }
  if(stop>nentries){
    cout<<"[EvDisp::Loop] stop>nentries --> stop = nentries"<<endl;
    stop = nentries;
  }
  if(start<0) start = 0;
  if(stop<0) stop = nentries;

  Long64_t nbytes = 0, nb = 0;
  for(Long64_t jentry=start; jentry<stop; jentry++) 
  {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEvent(jentry);
    nbytes += nb;
    if(evt != -1){
      if(event_eventNumber != evt){
        continue;
      }else{
        cout << "[EvDisp::Loop] processing event  : "<< evt << endl;
        fill();
        break;
      }
      cout << "[EvDisp::Loop] event  : "<< evt << "not found";
    }else{
      cout << "[EvDisp::Loop] processing : "<< jentry << " entry" << endl;
      fill();

      TString continueFlag = "n";
      do{
        if(continueFlag != "n") cout<<"[EvDisp::Loop] Invalid choice"<<endl;
        cout<<"[EvDisp::Loop] Do you want to go to the next event? y/n(exit)"<<endl;
        cin>>continueFlag;
      }while(continueFlag != "y" && continueFlag != "n");

      if(continueFlag == "n") break;
    }
  }

  cout<<"[EvDisp::Loop] end"<<endl;
  endJob();

}

void EvDisp::Loop(Long64_t evt)
{
  Loop(-1, -1, evt);
}

void EvDisp::LoopEntry(Long64_t entry)
{
  cout << "[EvDisp::Loop] processing entry : "<< entry;
  Loop(entry, entry+1);
}

void EvDisp::Loop()
{
  Loop(-1,-1,-1);
}

void EvDisp::book()
{
  cout<<"[EvDisp::book] start"<<endl;

  // m_2Dplots["timecomp"] = new TH2F("timecomp","ph2 time vs legacy",500,2200,3200,500,82000,83000);
  gSystem->Exec("mkdir -p ./evDispPlots");

  zSL1 = computeY(2.5);   // middle of SL1
  zSL3 = computeY(10.5);  // middle of SL3

  vector<float> xStruct, yStruct;
  vector<float> exStruct, eyStruct;

  for(unsigned int i=1; i<=60; i++) {
    for(unsigned int j=1; j<=12; j++){
      xStruct.push_back(computeX(i,j));
      yStruct.push_back(computeY(j));
      exStruct.push_back(cellSizeX/2);
      eyStruct.push_back(cellSizeY/2); 
    }
  }

  graphStruct = new TGraphErrors(xStruct.size(),&xStruct[0],&yStruct[0],&exStruct[0],&eyStruct[0]);

  cout<<"[EvDisp::book] end"<<endl;

}

void EvDisp::fill()
{
  bool debug = false;
  if(debug) cout<<endl;

  // DIGI
  if(debug) cout<<"digi"<<endl;
  vector<float> xPhiLeg, yPhiLeg, xPhiPh2, yPhiPh2;
  vector<float> xEtaLeg, yEtaLeg, xEtaPh2, yEtaPh2;

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

    if(dumpFlag){
      cout<<"digi "<<idigi<<", L = "<<digi_layer->at(idigi);
      cout<<", SL = "<<digi_superLayer->at(idigi);
      cout<<", wire = "<<digi_wire->at(idigi);      
      cout<<", time = "<<digi_time->at(idigi);      
      cout<<endl;
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

  if(dumpFlag) cout<<endl;

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

    if(dumpFlag){
      cout<<"Ph2Digi "<<idigi<<", L = "<<ph2Digi_layer->at(idigi);
      cout<<", SL = "<<ph2Digi_superLayer->at(idigi);
      cout<<", wire = "<<ph2Digi_wire->at(idigi);      
      cout<<", time = "<<ph2Digi_time->at(idigi);      
      cout<<endl;
    }
  }

  if(dumpFlag) cout<<endl;

  // SEGMENTS
  if(debug) cout<<"segment"<<endl;
  //Legacy
  int nSegLeg = 0;
  TF1 **segments_LegSL1 = new TF1*[seg_nSegments];
  TF1 **segments_LegSL3 = new TF1*[seg_nSegments];

  for(unsigned int iSeg=0; iSeg<seg_nSegments; iSeg++){
    if(seg_sector->at(iSeg)!=12 || seg_wheel->at(iSeg)!=2) continue;
    if(!seg_hasPhi->at(iSeg)) continue;
    nSegLeg++;

    if(debug){
      cout<<endl;
      cout<<"iSeg "<<iSeg<<endl;
      for(int iHit=0; iHit<seg_phi_nHits->at(iSeg); iHit++){
        cout<<getXY<float>(seg_phiHits_pos,iSeg,iHit)<<" ";
        cout<<getXY<float>(seg_phiHits_posCh,iSeg,iHit)<<" ";
        cout<<getXY<float>(seg_phiHits_wire,iSeg,iHit)<<" ";
        cout<<getXY<float>(seg_phiHits_wirePos,iSeg,iHit)<<" ";
        cout<<getXY<float>(seg_phiHits_layer,iSeg,iHit)<<" ";
        cout<<getXY<float>(seg_phiHits_superLayer,iSeg,iHit)<<" ";
        cout<<endl;
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
  if(debug) cout<<"plotting"<<endl;
  TCanvas* c1 = new TCanvas();
  c1->Divide(1,2);
  c1->cd(1);
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
    graphEta_Legacy->SetMarkerColor(kRed);
    graphEta_Legacy->Draw("PSAME");
  }

  for(int i=0;i<nSegLeg;i++){
    segments_LegSL1[i]->Draw("SAME");
    segments_LegSL3[i]->Draw("SAME");
  }

  c1->cd(2);
  TGraphErrors *graphStruct_ = (TGraphErrors*)graphStruct->Clone();
  graphStruct_->SetTitle("Phase2");
  graphStruct_->Draw("AP||");

  if(xPhiPh2.size()>0){
    TGraph* graphPhi_Ph2 = new TGraph(xPhiPh2.size(),&xPhiPh2[0],&yPhiPh2[0]);
    graphPhi_Ph2->SetMarkerStyle(20);
    graphPhi_Ph2->SetMarkerSize(0.5);
    graphPhi_Ph2->SetMarkerColor(kRed);
    graphPhi_Ph2->Draw("PSAME");
  }
  if(xEtaPh2.size()>0){
    TGraph* graphEta_Ph2 = new TGraph(xEtaPh2.size(),&xEtaPh2[0],&yEtaPh2[0]);
    graphEta_Ph2->SetMarkerStyle(20);
    graphEta_Ph2->SetMarkerSize(0.5);
    graphEta_Ph2->SetMarkerColor(kRed);
    graphEta_Ph2->Draw("PSAME");
  }

  // for(int i=0;i<nSegPh2;i++){
  //   segments_Ph2SL1[i]->SetLineColor(kRed);
  //   segments_Ph2SL3[i]->SetLineColor(kRed);
  //   segments_Ph2SL1[i]->Draw("SAME");
  //   segments_Ph2SL3[i]->Draw("SAME");
  // }

  c1->Update();

  TString saveFlag = "n";
  do{
    if(saveFlag != "n") cout<<"[EvDisp::fill] Invalid choice"<<endl;
    cout<<"[EvDisp::fill] Do you want to save the current event display? y/n"<<endl;
    cin>>saveFlag;
  }while(saveFlag != "y" && saveFlag != "n");

  if(saveFlag == "y") c1->Print(Form("evDispPlots/display_run%i_evt%i.png", event_runNumber, (int)event_eventNumber));

}

void EvDisp::endJob()
{
  cout<<"[EvDisp::endJob] start"<<endl;

  // TCanvas* c2 = new TCanvas();
  // gStyle->SetOptStat(0);
  // gStyle->SetPalette(1);
  // m_2Dplots["timecomp"]->Draw("colz");
  // c2->Update();
  // c2->Print("evDisplPlot/time.png");

  cout<<"[EvDisp::endJob] end"<<endl;
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
