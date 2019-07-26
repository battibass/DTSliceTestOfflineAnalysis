//////////////////////////////////
// TO COMPILE (in root shell)
// .L DTNtupleBaseAnalyzer.C++ 
// .L EvDisp.C++ 
//
// TO RUN (in root shell)
// auto evtDisplay = EvDisp(inputFile, outFile);
// auto evtDisplay = EvDisp("/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/ntuples/DTDPGNtuple_run329806.root", "out.root");
// evtDisplay.Loop();
//////////////////////////////////


#include "EvDisp.h"

EvDisp::EvDisp(const TString & inFileName, const TString & outFileName) :
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)
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

void EvDisp::Loop(Long64_t entry)
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
  std::cout << "[EvDisp::Loop] processing : "<< entry << "\r" << std::flush;
  fill();
  std::cout << std::endl; 
  endJob();

}

void EvDisp::Loop(Long64_t start, Long64_t stop)
{

  book();
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  if(stop>=nentries) stop = nentries -1;
  if(start>=nentries) start = nentries -2;

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

  m_outFile.cd();

  m_2Dplots["display1"] = new TH2F("display1","digi display legacy",60,0.5,60.5,12,0.5,12.5);
  m_2Dplots["display2"] = new TH2F("display2","digi display Phase2",60,0.5,60.5,12,0.5,12.5);
  m_2Dplots["timecomp"] = new TH2F("timecomp","ph2 time vs legacy",500,2200,3200,500,82000,83000);

  zSL1 = computeY(2.5);
  zSL3 = computeY(10.5);

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

}

void EvDisp::fill()
{

  // DIGI
  std::vector<float> xPhiLeg, yPhiLeg, xPhiPh2, yPhiPh2;
  std::vector<float> xEtaLeg, yEtaLeg, xEtaPh2, yEtaPh2;

  for(unsigned int idigi=0; idigi<digi_nDigis; idigi++) {
    if(digi_sector->at(idigi)!=12 || digi_wheel->at(idigi)!=2) continue; 
    float x=digi_wire->at(idigi);
    float y=digi_layer->at(idigi) + 4*(digi_superLayer->at(idigi)-1);
    m_2Dplots["display1"]->Fill(x,y);

    // cout<<x<<"   "<<y<<endl;

    if(digi_superLayer->at(idigi) == 2){
      xEtaLeg.push_back(computeX(x,y));
      yEtaLeg.push_back(computeY(y));
    }else{
      xPhiLeg.push_back(computeX(x,y));
      yPhiLeg.push_back(computeY(y));
    }

    for(unsigned int idigi2=0; idigi2<ph2Digi_nDigis; idigi2++) {
      if (ph2Digi_superLayer->at(idigi2)==digi_superLayer->at(idigi) &&
          ph2Digi_layer->at(idigi2)==digi_layer->at(idigi) &&
          ph2Digi_wire->at(idigi2)==digi_wire->at(idigi) )
      {
        m_2Dplots["timecomp"]->Fill(digi_time->at(idigi),ph2Digi_time->at(idigi2));
      }
    }
  }

  for(unsigned int idigi=0; idigi<ph2Digi_nDigis; idigi++) {
    if(digi_sector->at(idigi)!=12 || digi_wheel->at(idigi)!=2) continue; 
    float x=ph2Digi_wire->at(idigi);
    float y=ph2Digi_layer->at(idigi) + 4*(ph2Digi_superLayer->at(idigi)-1);
    m_2Dplots["display2"]->Fill(x,y);

    if(ph2Digi_superLayer->at(idigi)== 2){
      xEtaPh2.push_back(computeX(x,y));
      yEtaPh2.push_back(computeY(y));
    }else{
      xPhiPh2.push_back(computeX(x,y));
      yPhiPh2.push_back(computeY(y));
    }
  }

  // SEGMENTS
  //Legacy
  int nSegLeg = 0;
  TF1 **segments_Leg = new TF1*[seg_nSegments];

  for(unsigned int iSeg=0; iSeg<seg_nSegments; iSeg++){
    if(seg_sector->at(iSeg)!=12 || seg_wheel->at(iSeg)!=2) continue;
    if(!seg_hasPhi->at(iSeg)) continue;
    nSegLeg += 2;

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

    segments_Leg[iSeg] = new TF1(Form("segLeg1%i",iSeg),"[0]+[1]*x", x11-range1, x11+range1);
    segments_Leg[iSeg]->SetParameters(q1,m1);

    segments_Leg[iSeg+1] = new TF1(Form("segLeg3%i",iSeg),"[0]+[1]*x", x31-range3, x31+range3);
    segments_Leg[iSeg+1]->SetParameters(q3,m3);
  }

  //Phase 2
  int nSegPh2 = 0;
  TF1 **segments_Ph2 = new TF1*[ph2Seg_nSegments];

  for(unsigned int iSeg=0; iSeg<ph2Seg_nSegments; iSeg++){
    if(ph2Seg_sector->at(iSeg)!=12 || ph2Seg_wheel->at(iSeg)!=2) continue;
    if(!ph2Seg_hasPhi->at(iSeg)) continue;
    nSegPh2 += 2;

    double x11 = x0chamber + ph2Seg_posLoc_x_SL1->at(iSeg);
    double z11 = zSL1;
    double x12 = x11 + ph2Seg_dirLoc_x->at(iSeg);
    double z12 = z11 - ph2Seg_dirLoc_z->at(iSeg); //z is pointed downwards

    double m1 = computeM(x11, x12, z11, z12);
    double q1 = computeQ(x11, x12, z11, z12);
    double range1 = 2*cellSizeY/m1;

    double x31 = x0chamber + ph2Seg_posLoc_x_SL3->at(iSeg);
    double z31 = zSL3;
    double x32 = x31 + ph2Seg_dirLoc_x->at(iSeg);
    double z32 = z31 - ph2Seg_dirLoc_z->at(iSeg); //z is pointed downwards

    double m3 = computeM(x31, x32, z31, z32);
    double q3 = computeQ(x31, x32, z31, z32);
    double range3 = 2*cellSizeY/m3;

    segments_Ph2[iSeg] = new TF1(Form("segPh21%i",iSeg),"[0]+[1]*x", x11-range1, x11+range1);
    segments_Ph2[iSeg]->SetParameters(q1,m1);

    segments_Ph2[iSeg+1] = new TF1(Form("segPh23%i",iSeg),"[0]+[1]*x", x31-range3, x31+range3);
    segments_Ph2[iSeg+1]->SetParameters(q3,m3);
  }

  // PLOTTING
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
    segments_Leg[i]->Draw("SAME");
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

  for(int i=0;i<nSegPh2;i++){
    segments_Ph2[i]->SetLineColor(kBlue);
    segments_Ph2[i]->Draw("SAME");
  }

  c3->Update();
  c3->Print(Form("evDisplPlot/display_run%i_evt%i.png", event_runNumber, (int)event_eventNumber));

}

void EvDisp::endJob()
{

  TCanvas* c1 = new TCanvas();
  c1->Divide(2,1);
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  c1->cd(1);
  m_2Dplots["display1"]->Draw("colz");
  c1->cd(2);
  m_2Dplots["display2"]->Draw("colz");
  c1->Update();
  c1->Print("evDisplPlot/digi.png");

  TCanvas* c2 = new TCanvas();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  m_2Dplots["timecomp"]->Draw("colz");
  c2->Update();
  c2->Print("evDisplPlot/time.png");

  m_outFile.cd();
  m_outFile.Write();
  m_outFile.Close();

}


float EvDisp::computeX(float x, int y)
{
  if(y%2 == 0) x = cellSizeX*x;
  else x = cellSizeX/2+cellSizeX*x;
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
