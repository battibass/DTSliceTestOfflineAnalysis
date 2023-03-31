#include "TH1.h"
#include "TH2.h"
#include <TStyle.h>
#include <TCanvas.h>
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TGraphAsymmErrors.h"
#include "PlotTemplate.C"
#include "TFile.h"
#include "TSystem.h"
#include <iostream>
#include <string>
#include <sstream>

double getMeanEfficiency(TH1 *plot1, TH1 *plot2, double initialVal, double finalVal){
  double sumPassed = 0.;
  double sumTotal  = 0.;
  for (Int_t i = 1; i<=plot1->GetXaxis()->GetNbins(); ++i){
     if ( plot1->GetXaxis()->GetBinCenter(i) < initialVal || plot1->GetXaxis()->GetBinCenter(i) > finalVal  ) continue; 
     sumPassed = sumPassed + plot1->GetBinContent(i); 
     sumTotal  = sumTotal  + plot2->GetBinContent(i); 
  }
  
  return sumPassed/sumTotal;
}

void remove_x_errors(TGraphAsymmErrors *plot) {
  for (int i = 0; i < plot->GetXaxis()->GetNbins(); ++i) {
    plot->SetPointEXlow(i, 0);
    plot->SetPointEXhigh(i, 0);
  }
  plot->SetTitle("");
  // plot->SetMarkerStyle(20);
  // plot->SetMarkerSize(20);
}

std::string  legend(std::string s){
  //adding legend
  std::string effLeg;
  if (s.find("GoodBX") != std::string::npos)
    effLeg = "Good BX";   
  else
    effLeg = "All BX"; 
  return effLeg;
}    

void printPlots_run(std::string run) {
  const bool fileOK = false; 

  // gStyle->SetOptStat(111111);
  gStyle->SetOptStat(0);
  std::vector<std::string> stuffTags = {"Time", "Pos", "Psi"};

  TString file = run + "/emulator/results_emulator.root";

  TFile data1(file);
  
  int a = 0;

  std::vector<std::string> chambTags = {"MB1", "MB2","MB3", "MB4"};
  //std::vector<std::string> chambTags = { "MB2","MB3", "MB4"};
  std::vector<std::string> slTags = { "SL1", "SL3"};
  std::vector<std::string> quTags = {"3h","4h","Q6","Q7","Q8"};
  std::vector<std::string> quTagsSegs = {"3h","4h","Q6","Q7","Q8","bestQ"};
  std::vector<std::string> labelTags = {"All", "Correlated", "Uncorrelated"};
  std::vector<std::string> labelTagsPlots = {"All", "Correlated", "Uncorrelated", "UncorrelatedSL1", "UncorrelatedSL3"};
  
  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TH2*> m_plots2;
  std::map<std::string, TGraphAsymmErrors*> m_effs;

  std::string name;
  std::ostringstream outdir;

  std::vector<std::string> categories {"perGroup","perQuality"}; 

  std::vector<std::string> generalEffPlots {"hEffHW", "hEffTM", "hEffAM"};
  std::vector<std::string> effvsWhat = {"vsSegX","vsSegT0","vsph2SegX","vsph2SegT0","vsSegXLim","vsSegT0Lim","vsph2SegXLim","vsph2SegT0Lim"};
  std::vector<std::string> effWhichBX = {"","GoodBX"};  
  
  std::vector<std::string> general1DPlots {"hQualityHW", "hQualityAM"}; 
  std::vector<std::string> specific1DPlots {"hSLHW", "hSLAM", "hPrimsSegs","hQualityHW", "hQualityAM", "hQualityTM", "hLatenciesHW", "hMultiplicityHW"};
  std::vector<std::string> specific2DPlots {"hPrimTypeVsPos","h2DHwQualSegNHits","h2DEmuQualSegNHits","h2DTMQualSegNHits","hQualityVsBXHW", "hQualityVsBXAM", "hQualityVsLatenciesHW", "hPositionVsHitsHW", "hHits"};
  std::vector<std::string> moreSpecific1DPlots {"hBX","hBXEmul"}; // "hChi2FW","hChi2Emul",
  std::vector<std::string> moreSpecific1DPlotsSegs {"hPosph2Seg"};
  std::vector<std::string> moreSpecific2DPlots {"hPsi2D", "hPos2D"};
  std::vector<std::string> moreSpecific2DPlotsSegs {"hTime2DSeg"}; 

  std::vector<std::string> axisAndUnits {"BX (BX units)", "BX (BX units)","BX (BX units)", "BX (BX units)", "FW chi2 (U.A)", "Emul chi2 (U.A)", "HW Psi (#circ)", " Phase-2 Emulator Psi (#circ)","Phase-2 Primitive -  Phase-2 Emulator Psi (#circ)", "Phase-2 Primitive -  Phase-2 Emulator Time (ns)", "Phase-2 Primitive - Phase-2 Emulator Position (cm)"};
  std::vector <std::string> axisAndUnitsSegs {"Phase-2 Primitive - Phase-1 Segment Psi (#circ)", "Phase-2 Primitive - Phase-1 Segment Time (ns)", "Phase-2 Primitive - Phase-1 Segment Position (cm)", "Phase-2 Primitive -  Phase-2 Segment Psi (#circ)", "Phase-2 Primitive -  Phase-2 Segment Time (ns)", "Phase-2 Primitive -  Phase-2 Segment Position (cm)" };

  std::map<std::string, TH1*> m_plots_res;
  std::map<std::string, TH1*> m_plots_mean;
  std::map<std::string, TH1*> m_plots_peak;

  for (auto & chambTag : chambTags) {
    for (long unsigned int j = 0; j < moreSpecific1DPlots.size(); ++j){
      auto specific1DPlot = moreSpecific1DPlots.at(j);
   
      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)] = new TH1F((specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)).c_str(), 
											   "Resolutions; ; Resolution (a.u.) ",
											   5,-0.5,4.5); 
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)] = new TH1F((specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)).c_str(), 
											     "Means; ; Mean (a.u.) ",
											     5,-0.5,4.5);
      m_plots_peak[specific1DPlot + "_peak_" + chambTag + "_" + categories.at(0)] = new TH1F((specific1DPlot + "_peak_" + chambTag + "_" + categories.at(0)).c_str(), 
											     "Peak positions; ; Peak position (a.u.) ",
											     5,-0.5,4.5); 
      for (int i = 0; i < 5; ++i){
        m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)]->GetXaxis()->SetBinLabel(i+1, labelTagsPlots[i].c_str());
        m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)]->GetXaxis()->SetBinLabel(i+1, labelTagsPlots[i].c_str());
	m_plots_peak[specific1DPlot + "_peak_" + chambTag + "_" + categories.at(0)]->GetXaxis()->SetBinLabel(i+1, labelTagsPlots[i].c_str());
      }
      
      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(1)] = new TH1F((specific1DPlot + "_res_" +  chambTag + "_" + categories.at(1)).c_str(), 
											   "Resolutions; ; Resolution (a.u.) ",
											   5,-0.5,4.5); 
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(1)] = new TH1F((specific1DPlot + "_mean_" +  chambTag + "_" + categories.at(1)).c_str(), 
											     "Means; ; Mean (a.u.) ",
											     5,-0.5,4.5);
      m_plots_peak[specific1DPlot + "_peak_" + chambTag + "_" + categories.at(1)] = new TH1F((specific1DPlot + "_peak_" + chambTag + "_" + categories.at(1)).c_str(), 
											     "Peak positions; ; Peak position (a.u.) ",
											     5,-0.5,4.5); 
      for (int i = 0; i < 5; ++i){
        m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(1)]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
        m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(1)]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
	m_plots_peak[specific1DPlot + "_peak_" + chambTag + "_" + categories.at(1)]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
      }
      
      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());
      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(1)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());
      
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(1)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());
      
      m_plots_peak[specific1DPlot + "_peak_" + chambTag + "_" + categories.at(0)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());
      m_plots_peak[specific1DPlot + "_peak_" + chambTag + "_" + categories.at(1)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());
      
    }
  }
  for (auto & chambTag : chambTags) {
    for (long unsigned int j = 0; j < moreSpecific1DPlotsSegs.size(); ++j){
      auto specific1DPlot = moreSpecific1DPlotsSegs.at(j);
   
      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)] = new TH1F((specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)).c_str(), 
											   "Resolutions; ; Resolution (a.u.) ",
											   5,-0.5,4.5); 
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)] = new TH1F((specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)).c_str(), 
											     "Means; ; Mean (a.u.) ",
											     5,-0.5,4.5);
      m_plots_peak[specific1DPlot + "_peak_" + chambTag + "_" + categories.at(0)] = new TH1F((specific1DPlot + "_peak_" + chambTag + "_" + categories.at(0)).c_str(), 
											     "Peak positions; ; Peak position (a.u.) ",
											     5,-0.5,4.5); 
      for (int i = 0; i < 5; ++i){
        m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)]->GetXaxis()->SetBinLabel(i+1, labelTagsPlots[i].c_str());
        m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)]->GetXaxis()->SetBinLabel(i+1, labelTagsPlots[i].c_str());
	m_plots_peak[specific1DPlot + "_peak_" + chambTag + "_" + categories.at(0)]->GetXaxis()->SetBinLabel(i+1, labelTagsPlots[i].c_str());
      }
      
      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(1)] = new TH1F((specific1DPlot + "_res_" +  chambTag + "_" + categories.at(1)).c_str(), 
											   "Resolutions; ; Resolution (a.u.) ",
											   6,-0.5,5.5); 
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(1)] = new TH1F((specific1DPlot + "_mean_" +  chambTag + "_" + categories.at(1)).c_str(), 
											     "Means; ; Mean (a.u.) ",
											     6,-0.5,5.5);
      m_plots_peak[specific1DPlot + "_peak_" + chambTag + "_" + categories.at(1)] = new TH1F((specific1DPlot + "_peak_" + chambTag + "_" + categories.at(1)).c_str(), 
											     "Peak positions; ; Peak position (a.u.) ",
											     6,-0.5,5.5); 
      for (int i = 0; i < 6; ++i){
        m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(1)]->GetXaxis()->SetBinLabel(i+1, quTagsSegs[i].c_str());
        m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(1)]->GetXaxis()->SetBinLabel(i+1, quTagsSegs[i].c_str());
	m_plots_peak[specific1DPlot + "_peak_" + chambTag + "_" + categories.at(1)]->GetXaxis()->SetBinLabel(i+1, quTagsSegs[i].c_str());
      }
      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)]->GetYaxis()->SetTitle(axisAndUnitsSegs.at(j).c_str());
      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(1)]->GetYaxis()->SetTitle(axisAndUnitsSegs.at(j).c_str());
      
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)]->GetYaxis()->SetTitle(axisAndUnitsSegs.at(j).c_str());
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(1)]->GetYaxis()->SetTitle(axisAndUnitsSegs.at(j).c_str());
      
      m_plots_peak[specific1DPlot + "_peak_" + chambTag + "_" + categories.at(0)]->GetYaxis()->SetTitle(axisAndUnitsSegs.at(j).c_str());
      m_plots_peak[specific1DPlot + "_peak_" + chambTag + "_" + categories.at(1)]->GetYaxis()->SetTitle(axisAndUnitsSegs.at(j).c_str());
    }
  }
  std::string cmd = std::string("mkdir ") + run + std::string("/emulator"); // + "/" + generalPlot;
  std::vector <std::vector <std::string>> allplots {general1DPlots,generalEffPlots,specific1DPlots,specific2DPlots,moreSpecific1DPlots,moreSpecific1DPlotsSegs,moreSpecific2DPlots,moreSpecific2DPlotsSegs}; 
  
  
  for (auto & plots : allplots) {
    for (auto & plot : plots) {
      gSystem->Exec((cmd + "/" + plot).c_str());
    }
  }
  
  for (auto & generalPlot : general1DPlots) {
    std::string nameHisto = generalPlot;
    TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
    name = nameHisto;
    m_plots[name] = (TH1F*) data1.Get(name.c_str());
    m_plots[name]->SetTitle("");
    m_plots[name]->Draw();
    outdir.str("");
    outdir.clear();
    outdir << run.c_str() << "/emulator/" << generalPlot.c_str() << "/" << nameHisto.c_str() << ".png";
    DrawPrelimLabel(myCanvas);
    DrawLabel(myCanvas);
    SaveCanvas(myCanvas, outdir.str());
    delete myCanvas;
    // gPad->SaveAs(name);
  }
 
  //std::vector <std::string> effCats = {"","Q>2"};  
  std::vector <std::string> systems = {"HW","AM","TM"};  
  std::vector <std::string> effCats = {"","Corr","Q>2"};  
  std::map <std::string, std::string> effLeg;
  effLeg[""] = "Every Quality"; 
  effLeg["Corr"] = "Quality #geq 6"; 
  effLeg["Q>2"] = "Quality #geq 3"; 
  effLeg["hEffHW"] = "HW Q#geq3"; 
  effLeg["hEffTM"] = "TM"; 

  std::map <std::string, std::string> effHWCatsTitles; 
  
  effHWCatsTitles["vsSegX"] = "; Offline reconstructed segment Position (cm); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsSegXGoodBX"] = "; Offline reconstructed segment Position (cm); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsSegXCombi"] = "; Offline reconstructed segment Position (cm); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsSegT0"] = "; Offline reconstructed segment t0 (ns); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsSegT0GoodBX"] = "; Offline reconstructed segment t0 (ns); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsSegT0Combi"] = "; Offline reconstructed segment t0 (ns); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsph2SegX"] = "; Phase-2 Segment Position (cm); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsph2SegXGoodBX"] = "; Phase-2 Segment Position (cm); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsph2SegXCombi"] = "; Phase-2 Segment Position (cm); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsph2SegT0"] = "; Phase-2 Segment t0 (ns); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsph2SegT0GoodBX"] = "; Phase-2 Segment t0 (ns); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsph2SegT0Combi"] = "; Phase-2 Segment t0 (ns); Phase-2 Trigger Primitive Efficiency"; 
  
  effHWCatsTitles["vsSegXLim"] = "; Offline reconstructed segment Position (cm); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsSegXLimGoodBX"] = "; Offline reconstructed segment Position (cm); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsSegXLimCombi"] = "; Offline reconstructed segment Position (cm); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsSegT0Lim"] = "; Offline reconstructed segment t0 (ns); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsSegT0LimGoodBX"] = "; Offline reconstructed segment t0 (ns); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsSegT0LimCombi"] = "; Offline reconstructed segment t0 (ns); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsph2SegXLim"] = "; Phase-2 Segment Position (cm); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsph2SegXLimGoodBX"] = "; Phase-2 Segment Position (cm); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsph2SegXLimCombi"] = "; Phase-2 Segment Position (cm); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsph2SegT0Lim"] = "; Phase-2 Segment t0 (ns); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsph2SegT0LimGoodBX"] = "; Phase-2 Segment t0 (ns); Phase-2 Trigger Primitive Efficiency"; 
  effHWCatsTitles["vsph2SegT0LimCombi"] = "; Phase-2 Segment t0 (ns); Phase-2 Trigger Primitive Efficiency"; 
  
  std::map<std::string, float> limit_inf; 
  std::map<std::string, float> limit_sup;
  limit_inf["vsSegX"]=-100;
  limit_inf["vsph2SegX"]=-100;
  limit_sup["vsSegX"]=100;
  limit_sup["vsph2SegX"]=100;
  limit_inf["vsSegT0"]=-12.5;
  limit_inf["vsph2SegT0"]=-12.5;
  limit_sup["vsSegT0"]=12.5;
  limit_sup["vsph2SegT0"]=12.5;
  
  limit_inf["vsSegXLim"]=-100;
  limit_inf["vsph2SegXLim"]=-100;
  limit_sup["vsSegXLim"]=100;
  limit_sup["vsph2SegXLim"]=100;
  limit_inf["vsSegT0Lim"]=-12.5;
  limit_inf["vsph2SegT0Lim"]=-12.5;
  limit_sup["vsSegT0Lim"]=12.5;
  limit_sup["vsph2SegT0Lim"]=12.5;
  float limitInf, limitSup; 
  
  gSystem->Exec((std::string("mkdir ") + run + std::string("/emulator/hTimeOBDT")).c_str());
  std::vector<std::string> obdtTags = {"MB1_phi1", "MB1_phi2", "MB2_phi1", "MB2_phi2", "MB3_phi1b", "MB3_phi2b", "MB4_phi1b", "MB4_phi2b","MB4_phi3b", "MB4_phi4b"};
  for (auto & obdtTag : obdtTags) {
    std::string nameHisto = "hTimeOBDT_"  + obdtTag;
    TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
    name = nameHisto;
    m_plots[name] = (TH1F*) data1.Get(name.c_str());
    m_plots[name]->SetTitle("");
    m_plots[name]->Draw();
    outdir.str("");
    outdir.clear();
    outdir << run.c_str() << "/emulator/hTimeOBDT/" << nameHisto.c_str() << ".png";
    DrawPrelimLabel(myCanvas);
    DrawLabel(myCanvas);
    SaveCanvas(myCanvas, outdir.str());
    delete myCanvas;
    // gPad->SaveAs(name);
  }
  
  gSystem->Exec((std::string("mkdir ") + run + std::string("/emulator/hSegmentHits")).c_str());
  
  for (auto & chambTag : chambTags) {
    int index = &chambTag - &chambTags[0];
    // HIT PLOTS 
    std::vector <std::string> slTags2 = {"","SL1","SL3"};
    for (auto & slTag : slTags2) {
      std::string nameHisto = "hHitsPerChamber_" + chambTag + slTag;          
      TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
      name = nameHisto;
      m_plots[name] = (TH1F*) data1.Get(name.c_str());
      m_plots[name]->SetTitle("");
      m_plots[name]->Draw();
      outdir.str("");
      outdir.clear();
      outdir << run.c_str() << "/emulator/hHits/" << nameHisto.c_str() << ".png";
      // gPad->SaveAs(name);
      DrawPrelimLabel(myCanvas, chambTag);
      DrawLabel(myCanvas);
      SaveCanvas(myCanvas, outdir.str());
      delete myCanvas;
    } 
    
    // Segment hits plots
    std::vector <std::vector <std::string>> system_groups = {{"Hw", "Emu", "TM"}, {"Hw", "Emu"}, {"Hw", "TM"}};
    std::map <std::string, std::string> legends = {};
    legends["Hw"] = "Phase-2 Primitives";
    legends["Emu"] = "Phase-2 Emulator";
    legends["TM"] = "Phase-1 Primitives";
    
    std::vector <std::string> quality_groups = {"High", "Low"};
    std::vector <std::string> segment_types = {"", "ph2"};
    int system_group_number = 0;
    for (auto &system_group: system_groups){
      for (auto &segment: segment_types){
        for (auto &quality: quality_groups){
          std::string nameHisto = segment + "SegmentHits_" + chambTag + "_" + quality + "_" + to_string(index);
          TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
          int system_num = 2;
          TLegend *leg = new TLegend(0.55,0.3,0.8,0.5);
          int max_val = -999;
          for (auto &system: system_group){
            nameHisto = "h" + system + quality + "Qual" + segment + "SegNHits_" + chambTag;
            name = nameHisto;
            m_plots[name] = (TH1F*) data1.Get(name.c_str());
            if (m_plots[name] -> GetMaximum() > max_val){
              max_val = m_plots[name] -> GetMaximum();
            }
          }
          for (auto &system: system_group){
            nameHisto = "h" + system + quality + "Qual" + segment + "SegNHits_" + chambTag;
            name = nameHisto;
            m_plots[name] -> SetTitle("");
            m_plots[name] -> SetLineColor(system_num);
            m_plots[name] -> GetYaxis() -> SetRangeUser(0, 1.2 * max_val);
            m_plots[name] -> GetYaxis()->SetLabelSize(.03);
            leg->AddEntry(m_plots[name], legends[system].c_str(),"l" );
            m_plots[name]->Draw("same");
            system_num++;
          }
          leg->Draw("same");
          nameHisto = segment + "SegmentHits_" + chambTag + "_" + quality + "_" + to_string(system_group_number);
	  outdir.str("");
	  outdir.clear();
	  outdir << run.c_str() << "/emulator/hSegmentHits/" << nameHisto.c_str() << ".png";
          DrawPrelimLabel(myCanvas, chambTag);
          DrawLabel(myCanvas);
          SaveCanvas(myCanvas, outdir.str());
          delete myCanvas;
        }
      }
      system_group_number++;
    }
    

    ////////////////////////// EFFICIENCY PLOTS ////////////////////////////
    double mean;  

    // ALL vs GOOD BX
    for (auto & system : systems) {
      for (auto & what : effvsWhat) { 
	//if (chambTag == "MB2") continue; 
	std::string namePassed; 
	std::string nameTotal; 
	std::string cat = effCats[0];
	std::string nameHisto = "hEff" + system + what + "_" + chambTag;
	TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
	
	std::string HWCat = "hEff" + system + what + effWhichBX[0]; // All BX 
	TLegend *leg = new TLegend(0.55,0.3,0.8,0.5);
	nameHisto = HWCat + cat + "_" + chambTag;
	namePassed = nameHisto+"passed";
	m_plots[namePassed] = (TH1F*) data1.Get(namePassed.c_str());
	nameTotal = nameHisto+"total"; //this line does not work
	m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal.c_str());
	m_effs[name] = new TGraphAsymmErrors(m_plots[namePassed], m_plots[nameTotal]);
	remove_x_errors(m_effs[name]);
	m_effs[name]->SetLineColor(2);
	m_effs[name]->SetMarkerColor(2);
	remove_x_errors(m_effs[name]);
	std::string whatis = what; 
	limitInf = limit_inf[what]; 
	limitSup = limit_sup[what];
	mean = getMeanEfficiency (m_plots[namePassed], m_plots[nameTotal], limitInf, limitSup);
	//leg->AddEntry(m_effs[name], (effLeg[HWCat]).c_str(),"l" );
	leg->AddEntry(m_effs[name], (legend(HWCat)).c_str(),"l" );
	// m_effs[name]->SetTitle(( system + " " + effHWCatsTitles[what + "Combi"]).c_str());
	m_effs[name]->SetTitle((effHWCatsTitles[what + "Combi"]).c_str());
	m_effs[name]->SetMinimum(0);
	m_effs[name]->SetMaximum(1.2);
	m_effs[name]->Draw("AP");
	gPad->Update();
	
	HWCat = "hEff" + system + what + effWhichBX[1]; // Good BX
	nameHisto = HWCat + cat + "_" + chambTag;
	name = nameHisto;
	namePassed = nameHisto+"passed";
	nameTotal = nameHisto+"total";
	m_plots[namePassed] = (TH1F*) data1.Get(namePassed.c_str());
	m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal.c_str());
	m_effs[name] = new TGraphAsymmErrors(m_plots[namePassed], m_plots[nameTotal]);
	remove_x_errors(m_effs[name]);
	m_effs[name]->SetLineColor(3);
	m_effs[name]->SetMarkerColor(3);
	limitInf = limit_inf[what]; limitSup = limit_sup[what];
	mean = getMeanEfficiency (m_plots[namePassed], m_plots[nameTotal], limitInf, limitSup);
	leg->AddEntry(m_effs[name], (legend(HWCat)).c_str(),"l" );
	// m_effs[name]->SetTitle((system + " " + effHWCatsTitles[what + "Combi"]).c_str());
	m_effs[name]->SetTitle((effHWCatsTitles[what + "Combi"]).c_str());
	m_effs[name]->Draw("P");
	
	TLine l1 = TLine(limitInf, 0, limitInf, 1.2);
	TLine l2 = TLine(limitSup, 0, limitSup, 1.2);
	// l1.Draw("same");
	// l2.Draw("same");
	
	nameHisto = "hEff" + system + what + "_" + chambTag;
	leg->Draw();
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/hEff" << system.c_str() << "/" << nameHisto.c_str() << "_AllvsGood.png";
	// gPad->SaveAs(name);
	DrawPrelimLabel(myCanvas, chambTag);
	DrawLabel(myCanvas);
	SaveCanvas(myCanvas, outdir.str());
	delete myCanvas;
	
      } 
    } // SYSTEMS

    
    // EFF PER QUALITY
    for (auto & system : systems) {
      for (auto & what : effvsWhat) { 
	//if (chambTag == "MB2") continue; 
	std::string namePassed; 
	std::string nameTotal; 
	TGraphAsymmErrors pEff(std::nullptr_t);
	
	for (auto & HWCat : effWhichBX){
	  
	  std::string nameHisto = "hEff" + system + what + "_" + chambTag;
	  TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
	  
	  TLegend *leg = new TLegend(0.55,0.3,0.8,0.5);
	  std::string cat = effCats[0];
	  nameHisto = "hEff" + system + what + HWCat + cat + "_" + chambTag;
	  name = nameHisto;
	  namePassed = nameHisto+"passed";
	  nameTotal = nameHisto+"total";
	  m_plots[namePassed] = (TH1F*) data1.Get(namePassed.c_str());
	  m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal.c_str());
	  m_effs[name] = new TGraphAsymmErrors(m_plots[namePassed], m_plots[nameTotal]);
	  remove_x_errors(m_effs[name]);
	  m_effs[name]->SetLineColor(2);
	  m_effs[name]->SetMarkerColor(2);
	  
	  limitInf = limit_inf[what]; limitSup = limit_sup[what];
	  mean = getMeanEfficiency (m_plots[namePassed], m_plots[nameTotal], limitInf, limitSup);
	  leg->AddEntry(m_effs[name], (effLeg[cat]).c_str(),"l" );
	  // m_effs[name]->SetTitle((system + " " + effHWCatsTitles[what+HWCat]).c_str());
	  m_effs[name]->SetTitle((effHWCatsTitles[what+HWCat]).c_str());
	  m_effs[name]->SetMinimum(0);
	  m_effs[name]->SetMaximum(1.2);
	  m_effs[name]->Draw("AP");
	  // auto graph =  m_effs[name]->GetPaintedGraph(); 
	  // graph->SetMinimum(0);
	  // graph->SetMaximum(1.2);
	  
	  cat = effCats[1];
	  nameHisto = "hEff" + system + what + HWCat + cat + "_" + chambTag;
	  name = nameHisto;
	  namePassed = nameHisto+"passed";
	  nameTotal = nameHisto+"total";
	  m_plots[namePassed] = (TH1F*) data1.Get(namePassed.c_str());
	  m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal.c_str());
	  m_effs[name] = new TGraphAsymmErrors(m_plots[namePassed], m_plots[nameTotal]);
	  remove_x_errors(m_effs[name]);
	  m_effs[name]->SetLineColor(3);
	  m_effs[name]->SetMarkerColor(3);
	  limitInf = limit_inf[what]; limitSup = limit_sup[what];
	  mean = getMeanEfficiency (m_plots[namePassed], m_plots[nameTotal], limitInf, limitSup);
	  leg->AddEntry(m_effs[name], (effLeg[cat]).c_str(),"l" );
	  m_effs[name]->Draw("P");
	  
	  if (system != "TM") { // Q>2 only present in AM and HW
	    cat = effCats[2];
	    nameHisto = "hEff" + system + what + HWCat + cat + "_" + chambTag;
	    name = nameHisto;
	    namePassed = nameHisto+"passed";
	    nameTotal = nameHisto+"total";
	    m_plots[namePassed] = (TH1F*) data1.Get(namePassed.c_str());
	    m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal.c_str());
	    m_effs[name] = new TGraphAsymmErrors(m_plots[namePassed], m_plots[nameTotal]);
	    remove_x_errors(m_effs[name]);
	    m_effs[name]->SetLineColor(4);
	    m_effs[name]->SetMarkerColor(4);
	    limitInf = limit_inf[what]; limitSup = limit_sup[what];
	    mean = getMeanEfficiency (m_plots[namePassed], m_plots[nameTotal], limitInf, limitSup);
	    leg->AddEntry(m_effs[name], (effLeg[cat]).c_str(),"l" );
	    m_effs[name]->Draw("P");
	  }
	  nameHisto = "hEff" + system + what + HWCat + "_" + chambTag;
	  leg->Draw();
	  TLine l1 = TLine(limitInf, 0, limitInf, 1.2);
	  TLine l2 = TLine(limitSup, 0, limitSup, 1.2);
	  // l1.Draw("same");
	  // l2.Draw("same");
	  outdir.str("");
	  outdir.clear();
	  outdir << run.c_str() << "/emulator/hEff" << system.c_str() << "/" << nameHisto.c_str() << "_combined.png";
	  // gPad->SaveAs(name);
	  DrawPrelimLabel(myCanvas, chambTag);
	  DrawLabel(myCanvas);
	  SaveCanvas(myCanvas, outdir.str());
	  delete myCanvas;
	  
	  
	  
	  nameHisto = "hEff" + system + what + "_" + chambTag;
	  myCanvas = CreateCanvas(nameHisto, false, false);
	  
	  leg = new TLegend(0.55,0.3,0.8,0.5);
	  cat = "Q>2";
	  nameHisto = "hEffHW" + what + HWCat + cat + "_" + chambTag;
	  name = nameHisto;
	  namePassed = nameHisto+"passed";
	  nameTotal = nameHisto+"total";
	  m_plots[namePassed] = (TH1F*) data1.Get(namePassed.c_str());
	  m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal.c_str());
	  m_effs[name] = new TGraphAsymmErrors(m_plots[namePassed], m_plots[nameTotal]);
	  remove_x_errors(m_effs[name]);
	  m_effs[name]->SetLineColor(2);
	  m_effs[name]->SetMarkerColor(2);
	  limitInf = limit_inf[what]; limitSup = limit_sup[what];
	  mean = getMeanEfficiency (m_plots[namePassed], m_plots[nameTotal], limitInf, limitSup);
	  leg->AddEntry(m_effs[name], (effLeg["hEffHW"]).c_str(),"l" );
	  
	  m_effs[name]->SetTitle(effHWCatsTitles["hEff" + what + HWCat].c_str());
	  m_effs[name]->SetMinimum(0);
	  m_effs[name]->SetMaximum(1.2);
	  m_effs[name]->Draw("AP");
	  gPad->Update();
	  // auto graph =  m_effs[name]->GetPaintedGraph(); 
	  // graph->SetMinimum(0);
	  // graph->SetMaximum(1.2);
	  gPad->Update();
	  
	  cat = "";
	  nameHisto = "hEffTM" + what + HWCat + cat + "_" + chambTag;
	  name = nameHisto;
	  namePassed = nameHisto+"passed";
	  nameTotal = nameHisto+"total";
	  m_plots[namePassed] = (TH1F*) data1.Get(namePassed.c_str());
	  m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal.c_str());
	  m_effs[name] = new TGraphAsymmErrors(m_plots[namePassed], m_plots[nameTotal]);
	  remove_x_errors(m_effs[name]);
	  m_effs[name]->SetTitle(effHWCatsTitles["hEff" + what + HWCat].c_str());
	  m_effs[name]->SetLineColor(3);
	  m_effs[name]->SetMarkerColor(3);
	  limitInf = limit_inf[what]; limitSup = limit_sup[what];
	  mean = getMeanEfficiency (m_plots[namePassed], m_plots[nameTotal], limitInf, limitSup);
	  leg->AddEntry(m_effs[name], (effLeg["hEffTM"]).c_str(),"l" );
	  m_effs[name]->Draw("P");
	  
	  l1 = TLine(limitInf, 0, limitInf, 1.2);
	  l2 = TLine(limitSup, 0, limitSup, 1.2);
	  // l1.Draw("same");
	  // l2.Draw("same"); 
	  
	  nameHisto = "hEff" + what + HWCat + "_" + chambTag;
	  leg->Draw();
	  outdir.str("");
	  outdir.clear();
	  outdir << run.c_str() << "/emulator/hEffHW/" << nameHisto.c_str() << "_HWTW.png";
	  // gPad->SaveAs(name);
	  DrawPrelimLabel(myCanvas, chambTag);
	  DrawLabel(myCanvas);
	  SaveCanvas(myCanvas, outdir.str());
	  delete myCanvas;
	  
	  
	  
	}
      }
    } 
    
    
    
    for (auto & generalPlot : generalEffPlots) {
      //if (chambTag == "MB2") continue; 
      std::string namePassed; 
      std::string nameTotal; 
      for (auto & what : effvsWhat){ 
        for (auto & whichBX : effWhichBX){ 
          std::string nameHisto = generalPlot + what + whichBX + "_" + chambTag;
          TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
          name = nameHisto;
          namePassed = nameHisto+"passed";
          nameTotal = nameHisto+"total";
          m_plots[namePassed] = (TH1F*) data1.Get(namePassed.c_str());
          m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal.c_str());
          m_effs[name] = new TGraphAsymmErrors(m_plots[namePassed], m_plots[nameTotal]);
          remove_x_errors(m_effs[name]);
          m_effs[name]->SetMinimum(0);
          m_effs[name]->SetMaximum(1.2);
          m_effs[name]->Draw("AP");
          gPad->Update();
          // auto graph =  m_effs[name]->GetPaintedGraph(); 
          // graph->SetMinimum(0);
          // graph->SetMaximum(1.2);
          gPad->Update();
	  outdir.str("");
	  outdir.clear();
	  outdir << run.c_str() << "/emulator/" << generalPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	  // gPad->SaveAs(name);
          DrawPrelimLabel(myCanvas, chambTag);
          DrawLabel(myCanvas);
          SaveCanvas(myCanvas, outdir.str());
          delete myCanvas;
        }
      }
    }
  
    for (auto & specificPlot : specific1DPlots) {
      std::string nameHisto = specificPlot + "_" + chambTag;
      TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
      name = nameHisto;
      m_plots[name] = (TH1F*) data1.Get(name.c_str());
      m_plots[name]->SetTitle("");
      m_plots[name]->Draw();
      outdir.str("");
      outdir.clear();
      outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
      gPad->SaveAs(outdir.str().c_str());
      if (specificPlot == "hLatenciesHW" || specificPlot == "hBXWindowHW" || specificPlot == "hMultiplicityFW") {
        gPad->SetLogy();
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << "_log.png";
	// gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
        gPad->SetLogy(0);
      }
    }
    for (auto & specificPlot : specific2DPlots) {
      std::string nameHisto = specificPlot + "_" + chambTag;
      TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
      name = nameHisto;
      m_plots2[name] = (TH2F*) data1.Get(name.c_str());
      m_plots2[name]->SetTitle("");
      if (specificPlot == "h2DHwQualSegNHits" || specificPlot == "h2DEmuQualSegNHits" || specificPlot == "h2DTMQualSegNHits"){
        m_plots2[name]->GetXaxis()->SetNdivisions(5);
        m_plots2[name]->GetYaxis()->SetLabelSize(.05);
        m_plots2[name]->GetZaxis()->SetTitleOffset(1.5);
      } else {
        m_plots2[name]->GetYaxis()->SetTitleOffset(1.6);
      }
      m_plots2[name]->GetZaxis()->SetLabelSize(.03);
      m_plots2[name]->Draw("colz");
      outdir.str("");
      outdir.clear();
      outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
      // gPad->SaveAs(name);
      DrawPrelimLabel(myCanvas, chambTag);
      DrawLabel(myCanvas);
      SaveCanvas(myCanvas, outdir.str());
      delete myCanvas;
    }
    for (auto & labelTag:labelTags) {
      int j = &labelTag - &labelTags[0];
      for (auto & specificPlot : moreSpecific1DPlots) {
        std::string nameHisto = specificPlot + "_" + chambTag + "_" + labelTag;
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        name = nameHisto;
        m_plots[name] = (TH1F*) data1.Get(name.c_str());
        m_plots[name]->SetTitle("");
        m_plots[name]->Draw();
        m_plots_res[specificPlot + "_res_" + chambTag + "_" + categories.at(0)]->SetBinContent(j+1, m_plots[name]->GetRMS(1));
        m_plots_mean[specificPlot + "_mean_" + chambTag + "_" + categories.at(0)]->SetBinContent(j+1, m_plots[name]->GetMean(1));
	m_plots_peak[specificPlot + "_peak_" + chambTag + "_" + categories.at(0)]->SetBinContent(j+1, m_plots[name]->GetBinCenter(m_plots[name]->GetMaximumBin()));
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
        // gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
      
      for (auto & specificPlot : moreSpecific2DPlots) {
        std::string nameHisto = specificPlot + "_" + chambTag + "_" + labelTag;
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        name = nameHisto;
        m_plots2[name] = (TH2F*) data1.Get(name.c_str());
        m_plots2[name]->SetTitle("");
        m_plots2[name]->GetZaxis()->SetLabelSize(.03);
        m_plots2[name]->GetYaxis()->SetTitleOffset(1.6);
        m_plots2[name]->Draw("colz");
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
        // gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
      for (auto & specificPlot : moreSpecific1DPlotsSegs) {
        std::string nameHisto = specificPlot + "_" + chambTag + "_" + labelTag;
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        name = nameHisto;
        m_plots[name] = (TH1F*) data1.Get(name.c_str());
        m_plots[name]->SetTitle("");
        m_plots[name]->Draw();
        m_plots_res[specificPlot + "_res_" + chambTag + "_" + categories.at(0)]->SetBinContent(j+1, m_plots[name]->GetRMS(1));
        m_plots_mean[specificPlot + "_mean_" + chambTag + "_" + categories.at(0)]->SetBinContent(j+1, m_plots[name]->GetMean(1));
	m_plots_peak[specificPlot + "_peak_" + chambTag + "_" + categories.at(0)]->SetBinContent(j+1, m_plots[name]->GetBinCenter(m_plots[name]->GetMaximumBin()));
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	// gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
      
      for (auto & specificPlot : moreSpecific2DPlotsSegs) {
        std::string nameHisto = specificPlot + "_" + chambTag + "_" + labelTag;
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        name = nameHisto;
        m_plots2[name] = (TH2F*) data1.Get(name.c_str());
        m_plots2[name]-> SetTitle("");
        m_plots2[name]->GetZaxis()->SetLabelSize(.03);
        m_plots2[name]->GetYaxis()->SetTitleOffset(1.6);
        m_plots2[name]->Draw("colz");
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	// gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
      if (labelTag == "All" || labelTag == "Correlated") continue;
      for (auto  slTag : slTags) {
	int k = &slTag - &slTags[0];

        for (auto & specificPlot : moreSpecific1DPlots) {
          if (specificPlot == "hPsi" || specificPlot == "hTime" || specificPlot == "hPos") continue;
          std::string nameHisto = specificPlot + "_" + chambTag + "_" + labelTag + "_" + slTag;
          TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
          name = nameHisto;
          m_plots[name] = (TH1F*) data1.Get(name.c_str());
          m_plots[name]->SetTitle("");
          m_plots[name]->Draw();
          m_plots_res[specificPlot + "_res_" + chambTag + "_" + categories.at(0)]->SetBinContent(3+1+k, m_plots[name]->GetRMS(1));
          m_plots_mean[specificPlot + "_mean_" + chambTag + "_" + categories.at(0)]->SetBinContent(3+1+k, m_plots[name]->GetMean(1));
	  m_plots_peak[specificPlot + "_peak_" + chambTag + "_" + categories.at(0)]->SetBinContent(
												   3+1+k, m_plots[name]->GetBinCenter(m_plots[name]->GetMaximumBin()));
	  outdir.str("");
	  outdir.clear();
	  outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	  // gPad->SaveAs(name);
          DrawPrelimLabel(myCanvas, chambTag);
          DrawLabel(myCanvas);
          SaveCanvas(myCanvas, outdir.str());
          delete myCanvas;
        }
      
        for (auto & specificPlot : moreSpecific2DPlots) {
          if (specificPlot == "hPsi2D" || specificPlot == "hTime2D" || specificPlot == "hPos2D") continue;
          //if (specificPlot == "hPsi2DTM" ||specificPlot == "hPos2DTM" || specificPlot == "hPhi2DTM" ||specificPlot == "hPhiB2DTM" ) continue;
          std::string nameHisto = specificPlot + "_" + chambTag + "_" + labelTag + "_" + slTag;
          TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
          name = nameHisto;
          m_plots2[name] = (TH2F*) data1.Get(name.c_str());
          m_plots2[name]->SetTitle("");
          m_plots2[name]->GetZaxis()->SetLabelSize(.03);
          m_plots2[name]->GetYaxis()->SetTitleOffset(1.6);
          m_plots2[name]->Draw("colz");
	  outdir.str("");
	  outdir.clear();
	  outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	  // gPad->SaveAs(name);
          DrawPrelimLabel(myCanvas, chambTag);
          DrawLabel(myCanvas);
          SaveCanvas(myCanvas, outdir.str());
          delete myCanvas;
        }
        for (auto & specificPlot : moreSpecific1DPlotsSegs) {
          if (specificPlot == "hPsi" || specificPlot == "hTime" || specificPlot == "hPos") continue;
          std::string nameHisto = specificPlot + "_" + chambTag + "_" + labelTag + "_" + slTag;
          TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
          name = nameHisto;
          m_plots[name] = (TH1F*) data1.Get(name.c_str());
          m_plots[name]->SetTitle("");
          m_plots[name]->Draw();
          m_plots_res[specificPlot + "_res_" + chambTag + "_" + categories.at(0)]->SetBinContent(3+1+k, m_plots[name]->GetRMS(1));
          m_plots_mean[specificPlot + "_mean_" + chambTag + "_" + categories.at(0)]->SetBinContent(3+1+k, m_plots[name]->GetMean(1));
	  m_plots_peak[specificPlot + "_peak_" + chambTag + "_" + categories.at(0)]->SetBinContent(3+1+k, m_plots[name]->GetBinCenter(m_plots[name]->GetMaximumBin()));
	  outdir.str("");
	  outdir.clear();
	  outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	  // gPad->SaveAs(name);
          DrawPrelimLabel(myCanvas, chambTag);
          DrawLabel(myCanvas);
          SaveCanvas(myCanvas, outdir.str());
          delete myCanvas;
        }
      
        for (auto & specificPlot : moreSpecific2DPlotsSegs) {
          if (specificPlot == "hPsi2D" || specificPlot == "hTime2D" || specificPlot == "hPos2D") continue;
          //if (specificPlot == "hPsi2DTM" ||specificPlot == "hPos2DTM" || specificPlot == "hPhi2DTM" ||specificPlot == "hPhiB2DTM" ) continue;
          std::string nameHisto = specificPlot + "_" + chambTag + "_" + labelTag + "_" + slTag;
          TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
          name = nameHisto;
          m_plots2[name] = (TH2F*) data1.Get(name.c_str());
          m_plots2[name]->SetTitle("");
          m_plots2[name]->GetZaxis()->SetLabelSize(.03);
          m_plots2[name]->GetYaxis()->SetTitleOffset(1.6);
          m_plots2[name]->Draw("colz");
	  outdir.str("");
	  outdir.clear();
	  outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	  // gPad->SaveAs(name);
          DrawPrelimLabel(myCanvas, chambTag);
          DrawLabel(myCanvas);
          SaveCanvas(myCanvas, outdir.str());
          delete myCanvas;
        }
      } // sl
    } // label
    for (auto & quTag : quTags) {
      int j = &quTag - &quTags[0];

      for (auto & specificPlot : moreSpecific1DPlots) {
        std::string nameHisto = specificPlot + "_" + chambTag + "_" + quTag;
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        name = nameHisto;
        m_plots[name] = (TH1F*) data1.Get(name.c_str());
        m_plots[name]->SetTitle("");
        m_plots[name]->Draw();
        m_plots_res[specificPlot + "_res_" + chambTag + "_" + categories.at(1)]->SetBinContent(j+1, m_plots[name]->GetRMS(1));
        m_plots_mean[specificPlot + "_mean_" + chambTag + "_" + categories.at(1)]->SetBinContent(j+1, m_plots[name]->GetMean(1));
	m_plots_peak[specificPlot + "_peak_" + chambTag + "_" + categories.at(1)]->SetBinContent(j+1, m_plots[name]->GetBinCenter(m_plots[name]->GetMaximumBin()));
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	// gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
      
      for (auto & specificPlot : moreSpecific2DPlots) {
        std::string nameHisto = specificPlot + "_" + chambTag + "_" + quTag;
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        name = nameHisto;
        m_plots2[name] = (TH2F*) data1.Get(name.c_str());
        m_plots2[name]->SetTitle("");
        m_plots2[name]->GetZaxis()->SetLabelSize(.03);
        m_plots2[name]->GetYaxis()->SetTitleOffset(1.6);
        m_plots2[name]->Draw("colz");
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	// gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
    }
    for (auto & quTag: quTagsSegs) {
      int j = &quTag - &quTagsSegs[0];

      for (auto & specificPlot : moreSpecific1DPlotsSegs) {
        std::string nameHisto = specificPlot + "_" + chambTag + "_" + quTag;
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        name = nameHisto;
        m_plots[name] = (TH1F*) data1.Get(name.c_str());
        m_plots[name]->SetTitle("");
        m_plots[name]->Draw();
        m_plots_res[specificPlot + "_res_" + chambTag + "_" + categories.at(1)]->SetBinContent(j+1, m_plots[name]->GetRMS(1));
        m_plots_mean[specificPlot + "_mean_" + chambTag + "_" + categories.at(1)]->SetBinContent(j+1, m_plots[name]->GetMean(1));
	m_plots_peak[specificPlot + "_peak_" + chambTag + "_" + categories.at(1)]->SetBinContent(j+1, m_plots[name]->GetBinCenter(m_plots[name]->GetMaximumBin()));
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	// gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
      
      for (auto & specificPlot : moreSpecific2DPlotsSegs) {
        std::string nameHisto = specificPlot + "_" + chambTag + "_" + quTag;
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        name = nameHisto;
        m_plots2[name] = (TH2F*) data1.Get(name.c_str());
        m_plots2[name]->SetTitle("");
        m_plots2[name]->GetZaxis()->SetLabelSize(.03);
        m_plots2[name]->GetYaxis()->SetTitleOffset(1.6);
        m_plots2[name]->Draw("colz");
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specificPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	// gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
    } //qu
  } // chamber


  // PLOT RESOLS
 
  for (auto & chambTag : chambTags) {
    for (auto & specific1DPlot : moreSpecific1DPlots) {
      for (int i = 0; i<2; ++i){
        std::string nameHisto = specific1DPlot + "_res_" + chambTag + "_" + categories.at(i);
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        m_plots_res[nameHisto]->Draw();
        m_plots_res[nameHisto]->SetTitle("");
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specific1DPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	// gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
      for (int i = 0; i<2; ++i){
        std::string nameHisto = specific1DPlot + "_mean_" + chambTag + "_" + categories.at(i);
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        m_plots_mean[nameHisto]->Draw();
        m_plots_mean[nameHisto]->SetTitle("");
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specific1DPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	// gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
      for (int i = 0; i<2; ++i){
        std::string nameHisto = specific1DPlot + "_peak_" + chambTag + "_" + categories.at(i);
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        m_plots_peak[nameHisto]->Draw();
        m_plots_peak[nameHisto]->SetTitle("");
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specific1DPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	// gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
    }
    for (auto & specific1DPlot : moreSpecific1DPlotsSegs) {
      for (int i = 0; i<2; ++i){
        std::string nameHisto = specific1DPlot + "_res_" + chambTag + "_" + categories.at(i);
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        m_plots_res[nameHisto]->Draw();
        m_plots_res[nameHisto]->SetTitle("");
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specific1DPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	//gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
      for (int i = 0; i<2; ++i){
        std::string nameHisto = specific1DPlot + "_mean_" + chambTag + "_" + categories.at(i);
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        m_plots_mean[nameHisto]->Draw();
        m_plots_mean[nameHisto]->SetTitle("");
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specific1DPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	// gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
      for (int i = 0; i<2; ++i){
        std::string nameHisto = specific1DPlot + "_peak_" + chambTag + "_" + categories.at(i);
        TCanvas* myCanvas = CreateCanvas(nameHisto, false, false);
        m_plots_peak[nameHisto]->Draw();
        m_plots_peak[nameHisto]->SetTitle("");
	outdir.str("");
	outdir.clear();
	outdir << run.c_str() << "/emulator/" << specific1DPlot.c_str() << "/" << nameHisto.c_str() << ".png";
	// gPad->SaveAs(name);
        DrawPrelimLabel(myCanvas, chambTag);
        DrawLabel(myCanvas);
        SaveCanvas(myCanvas, outdir.str());
        delete myCanvas;
      }
    }
  }
  

} // end macro
