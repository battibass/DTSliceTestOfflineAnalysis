//////////////////////////////////
//
// Author: Alberto Bragagnolo alberto.bragagnolo@cern.ch
//
// TO COMPILE (in root shell)
// .L DTNtupleBaseAnalyzer.C++ 
// .L EvDisp.C++ 
//
// TO RUN (in root shell)
//
// auto evtDisplay = EvDisp(inputFile);
//
// See instruction in constuctor
//
//////////////////////////////////


#include "EvDisp.h"

EvDisp::EvDisp(const TString & inFileName) :
  DTNtupleBaseAnalyzer(inFileName)
, dumpFlag(false)
, saveDispFlag(0)
, askContinueFlag(true)
, runOnlyIfOneEmptyFlag(false)
, vetoMB1(true)
, vetoMB2(false)
, vetoMB3(true)
, vetoMB4(true)
, debug(false)
{
  cout<<endl;
  cout<<"INSTRUCTIONS"<<endl;
  cout<<endl;
  cout<<"EvDisp::Loop(); // all events "<<endl;
  cout<<"EvDisp::Loop(evt_number); // one event"<<endl;
  cout<<"EvDisp::LoopEntry(entry); // one entry"<<endl;
  cout<<"EvDisp::Loop(start, stop); // entries range"<<endl;
  cout<<"EvDisp::Loop(start, stop, evt); // one event, searched only in the entries range"<<endl;
  cout<<endl;
  cout<<"EvDisp::DumpON(); // digi dump to screen ON "<<endl;
  cout<<"EvDisp::DumpOFF(); // digi dump to screen OFF (default)"<<endl;
  cout<<endl;
  cout<<"EvDisp::SaveDisplayON(); // save all .png (no promt) "<<endl;
  cout<<"EvDisp::SaveDisplayPROMT(); // ask to save at every .png (default)"<<endl;
  cout<<"EvDisp::SaveDisplayOFF(); // do not save any .png (no promt) "<<endl;
  cout<<endl; 
  cout<<"EvDisp::AskContinueON(); // ask if continue at every event ON (default) "<<endl;
  cout<<"EvDisp::AskContinueOFF(); // ask if continue at every event OFF"<<endl;
  cout<<endl;
  cout<<"EvDisp::RunOnlyIfOneEmptyON(); // run only on events where one of the two Phases has no hits "<<endl;
  cout<<"EvDisp::RunOnlyIfOneEmptyOFF(); // disable RunOnlyIfOneEmpty (default)"<<endl;
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

  for(Long64_t jentry=start; jentry<stop; jentry++) 
  {

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    fChain->GetEvent(jentry);

    if(evt > 0){
      if(jentry%1000 == 0)  cout << "[EvDisp::Loop] at entry "<< jentry << " still not found" << endl;
      if(event_eventNumber != evt){
        continue;
      }else{
        cout << "[EvDisp::Loop] processing event  : "<< evt << endl;
        fill();
        break;
      }
      cout << "[EvDisp::Loop] event  : "<< evt << "not found"<< endl;
    }else if(evt == 0 || evt < -1){
      cout << "[EvDisp::Loop] Invalid evt number"<< endl;
      break;
    }else{
      if(!runOnlyIfOneEmptyFlag) cout << "[EvDisp::Loop] processing : "<< jentry << " entry" << endl;
      else if(jentry%1000 == 0)  cout << "[EvDisp::Loop] processing : "<< jentry << " entry" << endl;

      fill();

      TString continueFlag = "n";
      if(askContinueFlag){
        do{
          if(continueFlag != "n") cout<<"[EvDisp::Loop] Invalid choice"<<endl;
          cout<<"[EvDisp::Loop] Do you want to go to the next event? y/n(exit)"<<endl;
          cin>>continueFlag;
        }while(continueFlag != "y" && continueFlag != "n");
      }else{
        continueFlag = "y";
      }

      

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
  graphStruct->SetMarkerStyle(1);

  cout<<"[EvDisp::book] end"<<endl;

}

void EvDisp::fill()
{

  // STRUCT DRAWING
  auto c1 = new TCanvas("c1","c1",1500,1000);
  c1->Divide(2,2);
  for(int i=1;i<=4;i++){
    c1->cd(i);
    gPad->Divide(1,2);
    gPad->cd(1);
    graphStruct->SetTitle(Form("MB%i Legacy;x[cm];z[cm]", i ));
    graphStruct->DrawClone("AP||");

    c1->cd(i);
    gPad->cd(2);
    graphStruct->SetTitle(Form("MB%i Phase 2;x[cm];z[cm]", i));
    graphStruct->DrawClone("AP||");
  }

  // Station loop
  for(int iMB=1; iMB<=4; iMB++){

    if(vetoMB1 && iMB == 1) continue;
    if(vetoMB2 && iMB == 2) continue;
    if(vetoMB3 && iMB == 3) continue;
    if(vetoMB4 && iMB == 4) continue;

    // DIGI
    if(debug) cout<<endl;
    if(debug) cout<<"digi"<<endl;

    // RUN ONLY IF ONE PHASE EMPTY
    if(runOnlyIfOneEmptyFlag){
      unsigned int nDigiLegacy = 0, nDigiPh2 = 0;
      for(unsigned int idigi=0; idigi<digi_nDigis; idigi++){
        if(digi_sector->at(idigi)!=12 || digi_wheel->at(idigi)!=2 || digi_station->at(idigi)!=iMB) continue;
        if(digi_superLayer->at(idigi)==2) continue;
        nDigiLegacy++;
      }
      for(unsigned int idigi=0; idigi<ph2Digi_nDigis; idigi++){
        if(ph2Digi_sector->at(idigi)!=12 || ph2Digi_wheel->at(idigi)!=2 || ph2Digi_station->at(idigi)!=iMB) continue;
        if(ph2Digi_superLayer->at(idigi)==2) continue;
        nDigiPh2++;
      }
      bool onlyLegacy = false, onlyPh2 = false;
      if((nDigiLegacy == 0) && (nDigiPh2 !=0 )){
        onlyLegacy = true;
        cout<<"[EvDisp::fill] Only Legacy hits for station MB"<<iMB<<endl;
      }
      if((nDigiLegacy != 0) && (nDigiPh2 ==0 )){
        onlyPh2 = true;
        cout<<"[EvDisp::fill] Only Phase2 hits for station MB"<<iMB<<endl;
      }
      if(!(onlyLegacy || onlyPh2)) break;
    }

    if(dumpFlag) cout<<"digi MB SL  L wire     time"<<endl;

    // 5-dimensional arrays of vector<float> to allocate multiple digit up to 5
    vector<float> xPhiLeg[5], yPhiLeg[5];
    vector<float> xEtaLeg[5], yEtaLeg[5];

    for(unsigned int idigi=0; idigi<digi_nDigis; idigi++){
      if(digi_sector->at(idigi)!=12 || digi_wheel->at(idigi)!=2 || digi_station->at(idigi)!=iMB) continue;

      float wire = digi_wire->at(idigi);
      float layer = digi_layer->at(idigi) + 4*(digi_superLayer->at(idigi)-1);

      float x = computeX(wire,layer);
      float y = computeY(layer);

      if(digi_superLayer->at(idigi) == 2){
        fillDigiVectors(xEtaLeg, yEtaLeg, x, y);
      }else{
        fillDigiVectors(xPhiLeg, yPhiLeg, x, y);
      }

      if(dumpFlag){
        cout<<idigi;
        cout<<" "<<setw(5)<<digi_station->at(idigi);
        cout<<" "<<setw(2)<<digi_superLayer->at(idigi);
        cout<<" "<<setw(2)<<digi_layer->at(idigi);
        cout<<" "<<setw(4)<<digi_wire->at(idigi);      
        cout<<" "<<setw(8)<<digi_time->at(idigi);      
        cout<<endl;
      }

    }

    if(dumpFlag) cout<<endl<<"ph2Digi MB SL  L wire     time"<<endl;

    vector<float> xPhiPh2[5], yPhiPh2[5];
    vector<float> xEtaPh2[5], yEtaPh2[5];

    for(unsigned int idigi=0; idigi<ph2Digi_nDigis; idigi++){
      if(ph2Digi_sector->at(idigi)!=12 || ph2Digi_wheel->at(idigi)!=2 || ph2Digi_station->at(idigi)!=iMB) continue;

      float wire = ph2Digi_wire->at(idigi);
      float layer = ph2Digi_layer->at(idigi) + 4*(ph2Digi_superLayer->at(idigi)-1);

      float x = computeX(wire,layer);
      float y = computeY(layer);

      if(ph2Digi_superLayer->at(idigi) == 2){
        fillDigiVectors(xEtaPh2, yEtaPh2, x, y);
      }else{
        fillDigiVectors(xPhiPh2, yPhiPh2, x, y);
      }

      if(dumpFlag){
        cout<<idigi;
        cout<<" "<<setw(8)<<ph2Digi_station->at(idigi);
        cout<<" "<<setw(2)<<ph2Digi_superLayer->at(idigi);
        cout<<" "<<setw(2)<<ph2Digi_layer->at(idigi);
        cout<<" "<<setw(4)<<ph2Digi_wire->at(idigi);      
        cout<<" "<<setw(8)<<ph2Digi_time->at(idigi);      
        cout<<endl;
      }
    }

    if(dumpFlag) cout<<endl;

    // SEGMENTS
    if(debug) cout<<"segment"<<endl;

    //Legacy
    int nSegLeg = 0;
    segments_LegSL1 = new TF1*[seg_nSegments];
    segments_LegSL3 = new TF1*[seg_nSegments];

    for(unsigned int iSeg=0; iSeg<seg_nSegments; iSeg++){
      if(seg_sector->at(iSeg)!=12 || seg_wheel->at(iSeg)!=2 || seg_station->at(iSeg)!=iMB) continue;
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
      double range1 = computeSegRange(m1);

      double x31 = x0chamber + seg_posLoc_x_SL3->at(iSeg);
      double z31 = zSL3;
      double x32 = x31 + seg_dirLoc_x->at(iSeg);
      double z32 = z31 - seg_dirLoc_z->at(iSeg); //z is pointed downwards

      double m3 = computeM(x31, x32, z31, z32);
      double q3 = computeQ(x31, x32, z31, z32);
      double range3 = computeSegRange(m3);

      segments_LegSL1[iSeg] = new TF1(Form("segLegSL1%i",iSeg),"[0]+[1]*x", x11-range1, x11+range1);
      segments_LegSL1[iSeg]->SetParameters(q1,m1);
      segments_LegSL1[iSeg]->SetLineWidth(1);

      segments_LegSL3[iSeg] = new TF1(Form("segLegSL3%i",iSeg),"[0]+[1]*x", x31-range3, x31+range3);
      segments_LegSL3[iSeg]->SetParameters(q3,m3);
      segments_LegSL3[iSeg]->SetLineWidth(1);
    }

    //Phase 2
    // int nSegPh2 = 0;
    // TF1 **segments_Ph2SL1 = new TF1*[ph2Seg_nSegments];
    // TF1 **segments_Ph2Sl3 = new TF1*[ph2Seg_nSegments];

    // for(unsigned int iSeg=0; iSeg<ph2Seg_nSegments; iSeg++){
    //   if(ph2Seg_sector->at(iSeg)!=12 || ph2Seg_wheel->at(iSeg)!=2 || ph2Seg_station->at(iSeg)!=iMB) continue;
    //   if(!ph2Seg_hasPhi->at(iSeg)) continue;
    //   nSegPh2++;

    //   double x11 = x0chamber + ph2Seg_posLoc_x_SL1->at(iSeg);
    //   double z11 = zSL1;
    //   double x12 = x11 + ph2Seg_dirLoc_x->at(iSeg);
    //   double z12 = z11 - ph2Seg_dirLoc_z->at(iSeg); //z is pointed downwards

    //   double m1 = computeM(x11, x12, z11, z12);
    //   double q1 = computeQ(x11, x12, z11, z12);
    //   double range1 = computeSegRange(m1);

    //   double x31 = x0chamber + ph2Seg_posLoc_x_SL3->at(iSeg);
    //   double z31 = zSL3;
    //   double x32 = x31 + ph2Seg_dirLoc_x->at(iSeg);
    //   double z32 = z31 - ph2Seg_dirLoc_z->at(iSeg); //z is pointed downwards

    //   double m3 = computeM(x31, x32, z31, z32);
    //   double q3 = computeQ(x31, x32, z31, z32);
    //   double range3 = computeSegRange(m3);

    //   segments_Ph2SL1[iSeg] = new TF1(Form("segPh2SL1%i",iSeg),"[0]+[1]*x", x11-range1, x11+range1);
    //   segments_Ph2SL1[iSeg]->SetParameters(q1,m1);

    //   segments_Ph2SL3[iSeg] = new TF1(Form("segPh2SL3%i",iSeg),"[0]+[1]*x", x31-range3, x31+range3);
    //   segments_Ph2SL3[iSeg]->SetParameters(q3,m3);
    // }

    // PLOTTING
    if(debug) cout<<"plotting"<<endl;

    grPhi_Legacy = new TGraph*[5];
    grEta_Legacy = new TGraph*[5];
    grPhi_Ph2    = new TGraph*[5];
    grEta_Ph2    = new TGraph*[5];

    // LEGACY
    c1->cd(iMB);
    gPad->cd(1);

    for(int i=0;i<5;i++){
      if(xPhiLeg[i].size()>0){
        grPhi_Legacy[i] = new TGraph(xPhiLeg[i].size(),&xPhiLeg[i][0],&yPhiLeg[i][0]);
        setGraphColor(grPhi_Legacy[i], i);
        grPhi_Legacy[i]->Draw("PSAME");
      }else{ grPhi_Legacy[i]=nullptr; }
      if(xEtaLeg[i].size()>0){
        grEta_Legacy[i] = new TGraph(xEtaLeg[i].size(),&xEtaLeg[i][0],&yEtaLeg[i][0]);
        setGraphColor(grEta_Legacy[i], i);
        grEta_Legacy[i]->Draw("PSAME");
      }else{ grEta_Legacy[i]=nullptr; }
    }

    for(int i=0;i<nSegLeg;i++){
      segments_LegSL1[i]->Draw("SAME");
      segments_LegSL3[i]->Draw("SAME");
    }

    // PHASE 2
    c1->cd(iMB);
    gPad->cd(2);

    TGraphErrors *graphStruct_ = (TGraphErrors*)graphStruct->Clone();
    graphStruct_->SetTitle("Phase2");
    graphStruct_->Draw("AP||");

    for(int i=0;i<5;i++){
      if(xPhiPh2[i].size()>0){
        grPhi_Ph2[i] = new TGraph(xPhiPh2[i].size(),&xPhiPh2[i][0],&yPhiPh2[i][0]);
        setGraphColor(grPhi_Ph2[i], i);
        grPhi_Ph2[i]->Draw("PSAME");
      }else{ grPhi_Ph2[i]=nullptr; }
      if(xEtaPh2[i].size()>0){
        grEta_Ph2[i] = new TGraph(xEtaPh2[i].size(),&xEtaPh2[i][0],&yEtaPh2[i][0]);
        setGraphColor(grEta_Ph2[i], i);
        grEta_Ph2[i]->Draw("PSAME");
      }else{ grEta_Ph2[i]=nullptr; }
    }

    // PHASE 2 segment not present for now
    // for(int i=0;i<nSegPh2;i++){
    //   segments_Ph2SL1[i]->SetLineColor(kRed);
    //   segments_Ph2SL3[i]->SetLineColor(kRed);
    //   segments_Ph2SL1[i]->Draw("SAME");
    //   segments_Ph2SL3[i]->Draw("SAME");
    // }

    c1->Update();

  }

  c1->cd();
  auto legendPad = new TPad("legendPad","legendPad",.45,.9,.55,.99);
  legendPad->Draw();
  legendPad->cd();
  auto legend = new TLegend(.01,.01,.99,.99);
  legend->AddEntry((TObject*)0, Form("Run %i",event_runNumber), "");
  legend->AddEntry((TObject*)0, Form("Event %i",(int)event_eventNumber), "");
  legend->Draw();
  legendPad->Update();

  c1->Update();

  // PRINTING
  TString saveFlag = "n";
  if(saveDispFlag == 1){
    saveFlag = "y";
  }else if(saveDispFlag == -1){
    saveFlag = "n";
  }else{
    do{
      if(saveFlag != "n") cout<<"[EvDisp::fill] Invalid choice"<<endl;
      cout<<"[EvDisp::fill] Do you want to save the current event display? y/n"<<endl;
      cin>>saveFlag;
    }while(saveFlag != "y" && saveFlag != "n");
  }

  if(saveFlag == "y") c1->Print(Form("evDispPlots/display_run%i_evt%i.png", event_runNumber, (int)event_eventNumber));

  // MEMORY CLEANING
  delete legend;
  delete legendPad;
  delete c1;

  for(int i=0;i<5;i++) delete grPhi_Legacy[i];
  for(int i=0;i<5;i++) delete grEta_Legacy[i];
  for(int i=0;i<5;i++) delete grPhi_Ph2[i];
  for(int i=0;i<5;i++) delete grEta_Ph2[i];

  for(int i=0;i<seg_nSegments;i++) delete segments_LegSL1[i];
  for(int i=0;i<seg_nSegments;i++) delete segments_LegSL3[i];

}

void EvDisp::endJob()
{
  cout<<"[EvDisp::endJob] start"<<endl;

  cout<<"[EvDisp::endJob] Memory cleaning"<<endl;

  delete graphStruct;
  delete[] segments_LegSL1;
  delete[] segments_LegSL3;
  delete[] grPhi_Legacy;
  delete[] grEta_Legacy;
  delete[] grPhi_Ph2;
  delete[] grEta_Ph2;

  cout<<"[EvDisp::endJob] end"<<endl;
}

// --------------------------------------------------------------------------------------
//---------------------------------------FUNCTIONS---------------------------------------
// --------------------------------------------------------------------------------------

void EvDisp::fillDigiVectors(vector<float> vX[], vector<float> vY[], float x, float y)
{
  for(int i=5-1;i>0;i--){
    for(unsigned int j=0;j<vX[i].size();j++){
      if((abs(vX[i-1][j]-x) < cellSizeX/2) && (abs(vY[i-1][j] - y) < cellSizeY/2)){
        cout<<"----- "<<i<<" ----"<<endl;
        vX[i].push_back(x);
        vY[i].push_back(y);
        break;
      }
    }
  }
  vX[0].push_back(x);
  vY[0].push_back(y);
}

float EvDisp::computeX(float x, int y) // x = wire, y = layer
{
  x = cellSizeX*x;
  if(y%2 == 1) x += cellSizeX/2;  // Layer stagger
  if(y >= 9) x += cellSizeX;      // SL3 Stagger (station dependend)
  return x;
}

float EvDisp::computeY(float y) // x = wire, y = layer
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

double EvDisp::computeSegRange(double m)
{
  return 3*cellSizeY/m;
}

void EvDisp::setGraphColor(TGraph *gr, int i)
{
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(0.75);
  switch (i){
    case 0: gr->SetMarkerColor(kRed); break;
    case 1: gr->SetMarkerColor(kMagenta); break;
    case 2: gr->SetMarkerColor(kBlue); break;
    case 3: gr->SetMarkerColor(kCyan); break;
    case 4: gr->SetMarkerColor(kGreen); break;
  }
}