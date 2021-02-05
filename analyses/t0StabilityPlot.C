#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>

#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TFile.h>
#include <TLatex.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TProfile2D.h>
#include <TH1F.h>

//  run as root 't0s_stability.C("runNumber1 runNumber2 runNumber3 ...")

void t0StabilityPlot(std::vector<std::string> fileNames,
		     std::vector<std::string> labels,
		     TString baseOutFolder)
{

  gSystem->Exec("mkdir -p " + baseOutFolder + "/"); 

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  std::vector<int> colorArray = {kBlue+1, kGreen+1, kOrange+1, kMagenta+1, kYellow+1};

  if (fileNames.size() != labels.size()+1)
    {
      std::cout << "[t0StabilityPlot] : size of labels + 1 should be == size fileNames. Vectors are not compatible. quitting." << std::endl;
      return;
    }

  if (fileNames.size() < 2)
    {
      std::cout << "[t0StabilityPlot] : must provide at least 2 input files. quitting." << std::endl;
      return;
    }

  int iFile = 0;
  std::vector<std::string> typeTags = { "Ph1", "Ph2" };
  std::map<std::string, std::map<int, std::vector<TProfile2D *>>> profiles; // [typeTag, station, one plot per file]
  std::map<std::string, std::map<int, std::vector<TH1F *>>> plots; // [typeTag, station, one plot per file from 2nd file onwards] 

  std::cout << "[t0StabilityPlot] : running using " << std::endl;

  for (auto & fileName : fileNames)
    {
      std::cout << "\t" << fileName 
		<< (iFile ? " as comparison file" : " as refernce file") <<std::endl;
      auto file = TFile::Open(fileName.c_str());

      for (const auto & typeTag : typeTags)
	{
	  
	  for (int iSt=1; iSt<=4; ++iSt)
	    {
	      profiles[typeTag][iSt].push_back(static_cast<TProfile2D*>(file->Get(Form("hAvgDigiTime%sSt%i",typeTag.c_str(),iSt))));

	      if (iFile > 0)
		{ 
		  auto result = new TH1F(Form("hTimeDiff%sSt%i",typeTag.c_str(),iSt),";time difference (ns); # wires",51,-25.5,25.5);
		  plots[typeTag][iSt].push_back(result);

		  auto ref = profiles[typeTag][iSt].at(0);
		  auto target = profiles[typeTag][iSt].at(iFile);

		  int nBinsX = target->GetNbinsX();
		  int nBinsY = target->GetNbinsY();

		  for (int iBinX = 1; iBinX <= nBinsX; ++ iBinX)
		    {
		      for (int iBinY = 1; iBinY <= nBinsY; ++ iBinY)
			{
			  
			  int iBin = target->GetBin(iBinX, iBinY);
			  int refNEntries = ref->GetBinEntries(iBin);
			  int targetNEntries = target->GetBinEntries(iBin);

			  if (refNEntries > 10 && targetNEntries > 10)
			    {
			      
			      double refTime = ref->GetBinContent(iBin);
			      double targetTime = target->GetBinContent(iBin);
			      double timeDiff = std::max(-25.,std::min(25.,targetTime - refTime));			      
			      
			      if (std::abs(timeDiff) > 2.)
				{
				  int layer = (iBinY - 1) % 4;
				  int sl = (iBinY - 1) / 4;
				  std::cout << "[t0StabilityPlot] : |time diff.| > 2 ns for (" 
					    << typeTag << " , MB: " << iSt 
					    << " , wire: " << std::setw(3) << iBinX
					    << " , layer: " << layer 
					    << " , superlayer: " << sl
					    << std::setprecision(5)
					    << "):  ref ( " << std::setw(7) << refTime << " , " << std::setw(7) << refNEntries
					    << " )  target ( " << std::setw(7) << targetTime << " , " << std::setw(7) << targetNEntries
					    << " )" << std::endl;
			   	}

			      result->Fill(timeDiff);
			    }
			}
		    }	  
		}
	    }
	}
      
      iFile++;
      
    }

  for (auto & plotsByTag : plots)
    {
      auto typeTag = plotsByTag.first;

      for (auto & plotsBySt : plotsByTag.second)
	{
	  auto iSt = plotsBySt.first;

	  auto cName = Form("t0 stability %s MB%i",typeTag.c_str(),iSt);
	  auto canvas = new TCanvas(cName,cName,1000,800);
	  canvas->SetGridx();
	  canvas->cd();

	  TLegend *leg = new TLegend(0.62, 0.77, 0.87, 0.89);
	  leg->SetTextSize(0.02);
	  leg->SetHeader(Form("Wheel 2, Sector 12, MB%i %s", iSt, typeTag.c_str()), "C");
	  
	  int iHisto = 0;
	  float yRange = 0.;
	  for(auto & plot : plotsBySt.second)
	    {
	      float yRangeHisto = plot->GetMaximum();
	      if (yRangeHisto > yRange)
		yRange = yRangeHisto;
	    }

	  yRange *= 1.25;
	  
	  for(auto & plot : plotsBySt.second)
	    {
	      plot->GetYaxis()->SetTitle("Number of wires");
	      plot->GetXaxis()->SetTitle("Target t0i - Reference t0i (ns)");
	      plot->GetXaxis()->SetTitleOffset(1.4);
	      plot->GetYaxis()->SetRangeUser(0.,yRange);

	      plot->GetXaxis()->SetTitleOffset(1.4);

	      plot->Draw(iHisto ? "histsame" : "hist");
	      plot->SetLineColor(colorArray.at(iHisto));
	      plot->SetLineWidth(2);

	      leg->AddEntry(plot, labels.at(iHisto).c_str(), "l");	      
	    }

	  leg->Draw();

	  TLatex *latex = new TLatex();
	  latex->SetNDC();
	  latex->SetTextFont(61);
	  latex->SetTextSize(0.050);
	  latex->SetTextAlign(11);
	  latex->DrawLatex(0.100, 0.94, "CMS");
	  
	  latex->SetTextFont(52);
	  latex->SetTextSize(0.047);
	  latex->SetTextAlign(11);
	  latex->DrawLatex(0.190, 0.94, "Preliminary");
	  
	  latex->SetTextFont(42);
	  latex->SetTextSize(0.050);
	  latex->SetTextAlign(31);
	  latex->DrawLatex(0.90, 0.94, "DT SliceTest");
	  
	  canvas->SaveAs(Form("%s/tp_time_diff_MB%i_%s.png",baseOutFolder.Data(), iSt, typeTag.c_str()));

	}
    }

  gSystem->Exec("python plotAndPublish/publishDir.py " + baseOutFolder + " ./plotAndPublish/index.php"); 
  gSystem->Exec("cp -r " + baseOutFolder + " /eos/user/b/battilan/www/DTDPG/SliceTest/2021/stability/"); 

}

