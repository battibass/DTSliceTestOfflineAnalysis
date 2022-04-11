// Drawing template (originally used for TRG-17-001)
// Author: O. Davignon (CERN)
#include <TCanvas.h>
#include <TF1.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TPaveText.h>
#include <TMarker.h>
#include <TLine.h>
#include <TAxis.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TH1.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGraphAsymmErrors.h>

TCanvas* CreateCanvas(TString CanvasName = "myPlot", bool LogY = false, bool Grid = true)
{
  TCanvas* c = new TCanvas(CanvasName.Data(),CanvasName.Data(),800,800);
  c->SetLeftMargin(0.11);
  // c->SetRightMargin(0.16);
  if(Grid)
    {
      c->SetGrid();
    }
  if(LogY)
    {
      c->SetLogy();
    }
  return c;
}

void DrawPrelimLabel(TCanvas* c, TString station = "MB4")
{
  c->cd();

  TLatex tex;
  tex.SetTextSize(0.045);
  tex.DrawLatexNDC(0.11,0.91,"CMS #font[12]{Preliminary}");
  tex.Draw("same");

  TLatex tex2;
  tex2.SetTextSize(0.03);
  // tex2.SetTextColor(0);
  tex2.DrawLatexNDC(0.14,0.86, "Wh+2 Se12 " + station );
  tex2.Draw("same");

  return;
}

void DrawLumiLabel(TCanvas* c, TString Lumi = "35.9")
{
  c->cd();

  TLatex tex;
  tex.SetTextSize(0.0315);
  tex.SetTextAlign(31);
  //sil  TString toDisplay = "14 TeV, 3000 fb^{-1}, 200 PU";//typically for Phase-2
  TString toDisplay = "DT SliceTest, Cosmic Data (2021)";//typically for Phase-2
  //TString toDisplay = Lumi + " fb^{-1} (13 TeV)";//typically for Phase-1
  tex.DrawLatexNDC(0.90,0.91,toDisplay.Data());
  tex.Draw("same");

  return;
}

void SaveCanvas(TCanvas* c, TString PlotName = "myPlotName")
{
  c->cd();
  c->SaveAs(PlotName );

  return;
}
