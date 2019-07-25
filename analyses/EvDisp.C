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
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName), 
  cellSizeX(0.4), cellSizeY(0.13), honeySize(1.28)
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
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
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
  for (Long64_t jentry=start; jentry<stop;jentry++) 
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

  std::vector<float> xStruct, yStruct;
  std::vector<float> exStruct, eyStruct;

  for (unsigned int i=1; i<=60; i++) {
    for (unsigned int j=1; j<=12; j++){
      xStruct.push_back(computeX(i));
      yStruct.push_back(computeY(j));
      exStruct.push_back(cellSizeX/2);
      eyStruct.push_back(cellSizeY/2); 
    }
  }

  graphStruct = new TGraphErrors(xStruct.size(),&xStruct[0],&yStruct[0],&exStruct[0],&eyStruct[0]);

}

void EvDisp::fill()
{

  std::vector<float> xPhiLeg, yPhiLeg, xPhiPh2, yPhiPh2;
  std::vector<float> xEtaLeg, yEtaLeg, xEtaPh2, yEtaPh2;

  for (unsigned int idigi=0; idigi<digi_nDigis; idigi++) {
    float x=digi_wire->at(idigi);
    float y=digi_layer->at(idigi) + 4*(digi_superLayer->at(idigi)-1);
    m_2Dplots["display1"]->Fill(x,y);

    if(digi_superLayer->at(idigi) == 2){
      xEtaLeg.push_back(computeX(x));
      yEtaLeg.push_back(computeY(y));
    }else{
      xPhiLeg.push_back(computeX(x));
      yPhiLeg.push_back(computeY(y));
    }

    for (unsigned int idigi2=0; idigi2<ph2Digi_nDigis; idigi2++) {
      if (ph2Digi_superLayer->at(idigi2)==digi_superLayer->at(idigi) &&
          ph2Digi_layer->at(idigi2)==digi_layer->at(idigi) &&
          ph2Digi_wire->at(idigi2)==digi_wire->at(idigi) )
      {
        m_2Dplots["timecomp"]->Fill(digi_time->at(idigi),ph2Digi_time->at(idigi2));
      }
    }
  }

  for (unsigned int idigi=0; idigi<ph2Digi_nDigis; idigi++) {
    float x=ph2Digi_wire->at(idigi);
    float y=ph2Digi_layer->at(idigi) + 4*(ph2Digi_superLayer->at(idigi)-1);
    m_2Dplots["display2"]->Fill(x,y);

    if(ph2Digi_superLayer->at(idigi)== 2){
      xEtaPh2.push_back(computeX(x));
      yEtaPh2.push_back(computeY(y));
    }else{
      xPhiPh2.push_back(computeX(x));
      yPhiPh2.push_back(computeY(y));
    }
  }

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


float EvDisp::computeX(float x)
{
  // if((int)x%2 == 0) x = cellSizeX*x;
  // else x = cellSizeX/2+cellSizeX*x;
  x = cellSizeX/2+cellSizeX*x;
  return x;
}

float EvDisp::computeY(float y)
{
  if(y>4 && y<9) y = cellSizeY/2+cellSizeY*y + honeySize/2;
  else if (y>=9) y = cellSizeY/2+cellSizeY*y + honeySize;
  else y = cellSizeY/2+cellSizeY*y;
  return y;
}