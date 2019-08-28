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
, vetoMB4(false)
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
      if(jentry%1000 == 0){
        cout << "[EvDisp::Loop] at entry "<< jentry << " evt "<<evt<<" still not found" << endl;
        if(event_eventNumber > evt ){
          cout << "[EvDisp::Loop] current evt number "<< event_eventNumber << " is higher than the requested one " << evt << ", is this normal?" <<endl;
        }
      }  
      if(event_eventNumber != evt){
        continue;
      }else{
        cout << "[EvDisp::Loop] processing event  : "<< event_eventNumber << endl;
        fill();
        break;
      }
      cout << "[EvDisp::Loop] event  : "<< evt << "not found"<< endl;
    }else if(evt == 0 || evt < -1){
      cout << "[EvDisp::Loop] Invalid evt number"<< endl;
      break;
    }else{
      if(!runOnlyIfOneEmptyFlag) cout << "[EvDisp::Loop] processing entry : "<< jentry << endl;
      else if(jentry%1000 == 0)  cout << "[EvDisp::Loop] processing entry : "<< jentry << endl;

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

  vector<float> xStruct[4], yStruct[4];
  vector<float> exStruct[4], eyStruct[4];

  // LOOP SL 1-3
  for(unsigned int iMB=1; iMB<=4; iMB++){
    for(unsigned int iWire=1; iWire<=nWiresMaxPhi[iMB-1]; iWire++) {
      for(unsigned int iLayer=1; iLayer<=12; iLayer++){
        if((iLayer > 4) && (iLayer < 9)) continue;
        xStruct[iMB-1].push_back(computeX(iWire,iLayer,iMB));
        yStruct[iMB-1].push_back(computeY(iLayer));
        exStruct[iMB-1].push_back(cellSizeX/2);
        eyStruct[iMB-1].push_back(cellSizeY/2);
      }
    }
  }

  // LOOP SL 2
  for(unsigned int iMB=1; iMB<=3; iMB++){
    for(unsigned int iWire=1; iWire<=nWiresMaxEta; iWire++) {
      for(unsigned int iLayer=5; iLayer<=8; iLayer++){
        xStruct[iMB-1].push_back(computeX(iWire,iLayer,iMB));
        yStruct[iMB-1].push_back(computeY(iLayer));
        exStruct[iMB-1].push_back(cellSizeX/2);
        eyStruct[iMB-1].push_back(cellSizeY/2);
      }
    }
  }

  graphStruct = new TGraphErrors**[4]; // [station][phase]
  for(int i=0;i<4;i++){
    graphStruct[i] = new TGraphErrors*[2];
    for(int j=0;j<2;j++){
      graphStruct[i][j] = new TGraphErrors(xStruct[i].size(),&xStruct[i][0],&yStruct[i][0],&exStruct[i][0],&eyStruct[i][0]); 
      if(j==0) graphStruct[i][j]->SetTitle(Form("MB %i LEGACY;x[cm];z[cm]",i+1));
      if(j==1) graphStruct[i][j]->SetTitle(Form("MB %i PHASE2;x[cm];z[cm]",i+1));

      gStyle->SetTitleSize(0.1,"t");
      graphStruct[i][j]->SetMarkerStyle(1);
      graphStruct[i][j]->GetXaxis()->SetLimits(0.,410.);
      graphStruct[i][j]->GetXaxis()->SetTitleSize(.1);
      graphStruct[i][j]->GetYaxis()->SetTitleSize(.1);
      graphStruct[i][j]->GetXaxis()->SetTitleOffset(-0.5);
      graphStruct[i][j]->GetYaxis()->SetTitleOffset(0.4);
      graphStruct[i][j]->GetXaxis()->SetLabelSize(.1);
      graphStruct[i][j]->GetYaxis()->SetLabelSize(.1);
    }
  }

  // STRUCT DRAWING
  c1 = new TCanvas("c1","c1",1200,800);
  c1->Divide(1,4);
  for(int iMB=1;iMB<=4;iMB++){
    c1->cd(iMB);
    gPad->Divide(2,1);
    gPad->cd(1);
    graphStruct[iMB-1][0]->Draw("AP||");

    c1->cd(iMB);
    gPad->cd(2);
    graphStruct[iMB-1][1]->Draw("AP||");
  }
  cout<<"[EvDisp::book] end"<<endl;

}

void EvDisp::fill()
{

  if(debug) cout<<endl;
  if(debug) cout<<"start fill"<<endl;

  auto segments_LegSL1 = new TF1**[seg_nSegments]; // [segments][station]
  auto segments_LegSL3 = new TF1**[seg_nSegments];

  for (unsigned int i=0;i<seg_nSegments;i++){
    segments_LegSL1[i] = new TF1*[4];
    segments_LegSL3[i] = new TF1*[4];
    for(int j=0;j<4;j++){
      segments_LegSL1[i][j] = nullptr;
      segments_LegSL3[i][j] = nullptr;
    }
  }

  // auto segments_Ph2SL1 = new TF1**[ph2Seg_nSegments]; // [segments][station]
  // auto segments_Ph2SL3 = new TF1**[ph2Seg_nSegments];

  // for (unsigned int i=0;i<ph2Seg_nSegments;i++){
  //   segments_Ph2SL1[i] = new TF1*[4];
  //   segments_Ph2SL3[i] = new TF1*[4];
    // for(int j=0;j<4;j++){
    //   segments_Ph2SL1[i][j] = nullptr;
    //   segments_Ph2SL3[i][j] = nullptr;
    // }
  // }

    auto grPhi_Legacy = new TGraph**[5]; // [nHits][iMB]
    auto grEta_Legacy = new TGraph**[5];
    auto grPhi_Ph2    = new TGraph**[5];
    auto grEta_Ph2    = new TGraph**[5];

    for(int i=0;i<5;i++){
      grPhi_Legacy[i] = new TGraph*[4];
      grEta_Legacy[i] = new TGraph*[4];
      grPhi_Ph2[i]    = new TGraph*[4];
      grEta_Ph2[i]    = new TGraph*[4];
      for(int j=0;j<4;j++){
        grPhi_Legacy[i][j] = nullptr;
        grEta_Legacy[i][j] = nullptr;
        grPhi_Ph2[i][j] = nullptr;
        grEta_Ph2[i][j] = nullptr;
      }
    }


  // Station loop
  for(int iMB=1; iMB<=4; iMB++){

    if(vetoMB1 && iMB == 1) continue;
    if(vetoMB2 && iMB == 2) continue;
    if(vetoMB3 && iMB == 3) continue;
    if(vetoMB4 && iMB == 4) continue;

    if(debug) cout<<"station "<<iMB<<endl;
    // DIGI
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

    if(dumpFlag) cout<<"legDigi MB SL  L wire     time"<<endl;

    // 5-dimensional arrays of vector<float> to allocate multiple digit up to 5
    vector<float> xPhiLeg[5], yPhiLeg[5];
    vector<float> xEtaLeg[5], yEtaLeg[5];

    for(unsigned int idigi=0; idigi<digi_nDigis; idigi++){
      if(digi_sector->at(idigi)!=12 || digi_wheel->at(idigi)!=2 || digi_station->at(idigi)!=iMB) continue;

      float wire = digi_wire->at(idigi);
      float layer = digi_layer->at(idigi) + 4*(digi_superLayer->at(idigi)-1);

      float x = computeX(wire,layer,iMB);
      float y = computeY(layer);

      if(digi_superLayer->at(idigi) == 2){
        fillDigiVectors(xEtaLeg, yEtaLeg, x, y);
      }else{
        fillDigiVectors(xPhiLeg, yPhiLeg, x, y);
      }

      if(dumpFlag){
        cout<<setw(7)<<idigi;
        cout<<" "<<setw(2)<<digi_station->at(idigi);
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

      float x = computeX(wire,layer,iMB);
      float y = computeY(layer);

      if(ph2Digi_superLayer->at(idigi) == 2){
        fillDigiVectors(xEtaPh2, yEtaPh2, x, y);
      }else{
        fillDigiVectors(xPhiPh2, yPhiPh2, x, y);
      }

      if(dumpFlag){
        cout<<setw(7)<<idigi;
        cout<<" "<<setw(2)<<ph2Digi_station->at(idigi);
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
    int nSegLeg = 0; // number of segment to draw

    for(unsigned int iSeg=0; iSeg<seg_nSegments; iSeg++){
      bool skipSeg = false;
      if(seg_sector->at(iSeg)!=12 || seg_wheel->at(iSeg)!=2 || seg_station->at(iSeg)!=iMB) skipSeg = true;
      if(!seg_hasPhi->at(iSeg)) skipSeg = true;
      if(skipSeg) continue;

      // cout<<endl;
      // cout<<"iMB "<<iMB<<endl;
      // cout<<"iSeg "<<iSeg<<endl;
      // for(int iHit=0; iHit<seg_phi_nHits->at(iSeg); iHit++){
      //   cout<<getXY<float>(seg_phiHits_pos,iSeg,iHit)<<" ";
      //   cout<<getXY<float>(seg_phiHits_posCh,iSeg,iHit)<<" ";
      //   cout<<getXY<float>(seg_phiHits_wire,iSeg,iHit)<<" ";
      //   cout<<getXY<float>(seg_phiHits_wirePos,iSeg,iHit)<<" ";
      //   cout<<getXY<float>(seg_phiHits_layer,iSeg,iHit)<<" ";
      //   cout<<getXY<float>(seg_phiHits_superLayer,iSeg,iHit)<<" ";
      //   cout<<endl;
      // }

      double x11 = x0station[iMB-1] + seg_posLoc_x_SL1->at(iSeg);
      double z11 = zSL1;
      double x12 = x11 + seg_dirLoc_x->at(iSeg);
      double z12 = z11 - seg_dirLoc_z->at(iSeg); //z is pointed downwards

      double m1 = computeM(x11, x12, z11, z12);
      double q1 = computeQ(x11, x12, z11, z12);
      double range1 = computeSegRange(m1);

      double x31 = x0station[iMB-1] + seg_posLoc_x_SL3->at(iSeg);
      double z31 = zSL3;
      double x32 = x31 + seg_dirLoc_x->at(iSeg);
      double z32 = z31 - seg_dirLoc_z->at(iSeg); //z is pointed downwards

      double m3 = computeM(x31, x32, z31, z32);
      double q3 = computeQ(x31, x32, z31, z32);
      double range3 = computeSegRange(m3);

      segments_LegSL1[nSegLeg][iMB-1] = new TF1(Form("segLegSL1%i_%i",iSeg,iMB),"[0]+[1]*x", x11-range1, x11+range1);
      segments_LegSL1[nSegLeg][iMB-1]->SetParameters(q1,m1);
      segments_LegSL1[nSegLeg][iMB-1]->SetLineWidth(1);

      segments_LegSL3[nSegLeg][iMB-1] = new TF1(Form("segLegSL3%i_%i",iSeg,iMB),"[0]+[1]*x", x31-range3, x31+range3);
      segments_LegSL3[nSegLeg][iMB-1]->SetParameters(q3,m3);
      segments_LegSL3[nSegLeg][iMB-1]->SetLineWidth(1);

      nSegLeg++;
    }

    //Phase 2  // TO CHECK
    // int nSegPh2 = 0;

    // for(unsigned int iSeg=0; iSeg<ph2Seg_nSegments; iSeg++){
      // bool skipSeg = false;
      // if(ph2Seg_sector->at(iSeg)!=12 || ph2Seg_wheel->at(iSeg)!=2 || ph2Seg_station->at(iSeg)!=iMB) skipSeg = true;
      // if(!ph2Seg_hasPhi->at(iSeg)) skipSeg = true;
      // if(skipSeg) continue;

    //   double x11 = x0station[iMB-1] + ph2Seg_posLoc_x_SL1->at(iSeg);
    //   double z11 = zSL1;
    //   double x12 = x11 + ph2Seg_dirLoc_x->at(iSeg);
    //   double z12 = z11 - ph2Seg_dirLoc_z->at(iSeg); //z is pointed downwards

    //   double m1 = computeM(x11, x12, z11, z12);
    //   double q1 = computeQ(x11, x12, z11, z12);
    //   double range1 = computeSegRange(m1);

    //   double x31 = x0station[iMB-1] + ph2Seg_posLoc_x_SL3->at(iSeg);
    //   double z31 = zSL3;
    //   double x32 = x31 + ph2Seg_dirLoc_x->at(iSeg);
    //   double z32 = z31 - ph2Seg_dirLoc_z->at(iSeg); //z is pointed downwards

    //   double m3 = computeM(x31, x32, z31, z32);
    //   double q3 = computeQ(x31, x32, z31, z32);
    //   double range3 = computeSegRange(m3);

    //   segments_Ph2SL1[nSegPh2][iMB-1] = new TF1(Form("segPh2SL1%i_%i",iSeg,iMB),"[0]+[1]*x", x11-range1, x11+range1);
    //   segments_Ph2SL1[nSegPh2][iMB-1]->SetParameters(q1,m1);

    //   segments_Ph2SL3[nSegPh2][iMB-1] = new TF1(Form("segPh2SL3%i_%i",iSeg,iMB),"[0]+[1]*x", x31-range3, x31+range3);
    //   segments_Ph2SL3[nSegPh2][iMB-1]->SetParameters(q3,m3);
    //   nSegPh2 ++;
    // }

    // PLOTTING
    
    // LEGACY
    if(debug) cout<<"plotting legacy digi"<<endl;
    c1->cd(iMB);
    gPad->cd(1);

    for(int i=0;i<5;i++){
      if(xPhiLeg[i].size()>0){
        grPhi_Legacy[i][iMB-1] = new TGraph(xPhiLeg[i].size(),&xPhiLeg[i][0],&yPhiLeg[i][0]);
        setGraphColor(grPhi_Legacy[i][iMB-1], i);
        grPhi_Legacy[i][iMB-1]->Draw("PSAME");
      }else{ grPhi_Legacy[i][iMB-1]=nullptr; }

      if(xEtaLeg[i].size()>0){
        grEta_Legacy[i][iMB-1] = new TGraph(xEtaLeg[i].size(),&xEtaLeg[i][0],&yEtaLeg[i][0]);
        setGraphColor(grEta_Legacy[i][iMB-1], i);
        grEta_Legacy[i][iMB-1]->Draw("PSAME");
      }else{ grEta_Legacy[i][iMB-1]=nullptr; }
    }

    if(debug) cout<<"plotting legacy segments "<<nSegLeg<<endl;
    for(int i=0;i<nSegLeg;i++){
      segments_LegSL1[i][iMB-1]->Draw("SAME");
      segments_LegSL3[i][iMB-1]->Draw("SAME");
    }

    // PHASE 2
    if(debug) cout<<"plotting phase2 digi "<<endl;
    c1->cd(iMB);
    gPad->cd(2);

    for(int i=0;i<5;i++){
      if(xPhiPh2[i].size()>0){
        grPhi_Ph2[i][iMB-1] = new TGraph(xPhiPh2[i].size(),&xPhiPh2[i][0],&yPhiPh2[i][0]);
        setGraphColor(grPhi_Ph2[i][iMB-1], i);
        grPhi_Ph2[i][iMB-1]->Draw("PSAME");
      }else{ grPhi_Ph2[i][iMB-1]=nullptr; }

      if(xEtaPh2[i].size()>0){
        grEta_Ph2[i][iMB-1] = new TGraph(xEtaPh2[i].size(),&xEtaPh2[i][0],&yEtaPh2[i][0]);
        setGraphColor(grEta_Ph2[i][iMB-1], i);
        grEta_Ph2[i][iMB-1]->Draw("PSAME");
      }else{ grEta_Ph2[i][iMB-1]=nullptr; }
    }

    // PHASE 2 segment not present for now
    // if(debug) cout<<"plotting phase2 segments"<<endl;
    // for(int i=0;i<nSegPh2;i++){
    //   segments_Ph2SL1[i][iMB-1]->Draw("SAME");
    //   segments_Ph2SL3[i][iMB-1]->Draw("SAME");
    // }

  }

  if(debug) cout<<"printing"<<endl;
  c1->cd();
  auto legendPad = new TPad("legendPad","legendPad",.46,.9,.54,.99);
  legendPad->Draw();
  legendPad->cd();
  if(debug) cout<<"legendPad drawn"<<endl;

  auto legend = new TLegend(.01,.01,.99,.99);
  legend->AddEntry((TObject*)0, Form("Run %i",event_runNumber), "");
  legend->AddEntry((TObject*)0, Form("Event %i",(int)event_eventNumber), "");
  legend->Draw();
  legendPad->Update();
  c1->Update();
  if(debug) cout<<"legend drawn"<<endl;

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

  c1->Update();

  if(saveFlag == "y") c1->Print(Form("evDispPlots/display_run%i_evt%i.png", event_runNumber, (int)event_eventNumber));

  // MEMORY CLEANING

  // Memory Clenning
  if(debug) cout<<"memory cleaning 1"<<endl;
  for(int i=0;i<5;i++){
    for(int j=0;j<4;j++){
      delete grPhi_Legacy[i][j];
      delete grEta_Legacy[i][j];
      delete grPhi_Ph2[i][j];
      delete grEta_Ph2[i][j];
    }
  }

  if(debug) cout<<"memory cleaning 2"<<endl;
  for(int i=0;i<5;i++){
    delete[] grPhi_Legacy[i];
    delete[] grEta_Legacy[i];
    delete[] grPhi_Ph2[i];
    delete[] grEta_Ph2[i];
  }

  if(debug) cout<<"memory cleaning 3"<<endl;
  delete[] grPhi_Legacy;
  delete[] grEta_Legacy;
  delete[] grPhi_Ph2;
  delete[] grEta_Ph2;
  
  if(debug) cout<<"memory cleaning 4"<<endl;
  for(unsigned int i=0;i<seg_nSegments;i++){
    for(int j=0;j<4;j++){
      delete segments_LegSL1[i][j];
      delete segments_LegSL3[i][j];
    }
  }

  if(debug) cout<<"memory cleaning 5"<<endl;
  for(unsigned int i=0;i<seg_nSegments;i++){
    delete[] segments_LegSL1[i];
    delete[] segments_LegSL3[i];
  }

  if(debug) cout<<"memory cleaning 6"<<endl;
  delete[] segments_LegSL1;
  delete[] segments_LegSL3;

  if(debug) cout<<"memory cleaning 7"<<endl;
  delete legend;
  delete legendPad;

}

void EvDisp::endJob()
{
  cout<<"[EvDisp::endJob] start"<<endl;

  cout<<"[EvDisp::endJob] Memory cleaning"<<endl;

  for(int i=0;i<4;i++){
    for(int j=0;j<2;j++){
      delete graphStruct[i][j];
    }
  }
  for(int i=0;i<4;i++) delete[] graphStruct[i];
  delete[] graphStruct;
  delete c1;

  cout<<"[EvDisp::endJob] end"<<endl;
}

// --------------------------------------------------------------------------------------
//---------------------------------------FUNCTIONS---------------------------------------
// --------------------------------------------------------------------------------------

void EvDisp::fillDigiVectors(vector<float> vX[], vector<float> vY[], float x, float y)
{
  for(int i=5-1;i>0;i--){
    for(unsigned int j=0;j<vX[i-1].size();j++){
      if((abs(vX[i-1][j]-x) < cellSizeX/2) && (abs(vY[i-1][j] - y) < cellSizeY/2)){
        vX[i].push_back(x);
        vY[i].push_back(y);
        break;
      }
    }
  }
  vX[0].push_back(x);
  vY[0].push_back(y);
}

float EvDisp::computeX(float x, int y, int iMB) // x = wire, y = layer, MB = stat
{
  x = cellSizeX*x;
  if(y%2 == 1) x += cellSizeX/2;  // Layer stagger
  if(y >= 9){ // SL3 Stagger (station dependend)
    switch(iMB)
    {
      case 1:
      // do something
      break;

      case 2:
      x += cellSizeX;
      break;

      case 3:
      // do something
      break;

      case 4:
      x += 2*cellSizeX;
      break;
    }
  }  

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
    case 0: gr->SetMarkerColor(kGreen); break;
    case 1: gr->SetMarkerColor(kYellow); break;
    case 2: gr->SetMarkerColor(kOrange); break;
    case 3: gr->SetMarkerColor(kRed); break;
    case 4: gr->SetMarkerColor(kBlue); break;
  }
}