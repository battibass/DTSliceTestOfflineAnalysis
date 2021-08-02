#include "TROOT.h"
#include "TString.h"
#include "TXMLEngine.h"

#include <string>
#include <iostream>

#include "DTNtupleDigiAnalyzer.h"
#include "DTNtupleTriggerAnalyzer.h"
#include "DTNtupleSegmentAnalyzer.h"

void runAllAnalyses(TString inputFile, Int_t runNumber)
{

  TString runName;
  runName.Form("run%d", runNumber);

  gSystem->Exec("mkdir -p " + runName + "/digi/");
  gSystem->Exec("mkdir -p " + runName + "/trigger/");
  gSystem->Exec("mkdir -p " + runName + "/segment/");

  //CB find a more elegant solution to pass directory
  DTNtupleDigiAnalyzer digiAnalysis(inputFile, runName + "/digi/results_digi.root", (runName + "/digi/").Data());
  digiAnalysis.Loop();

  DTNtupleSegmentAnalyzer segmentAnalysis(inputFile, runName + "/segment/results_segment.root");
  segmentAnalysis.Loop();

  DTNtupleTriggerAnalyzer triggerAnalysis(inputFile, runName + "/trigger/results_trigger.root");
  triggerAnalysis.Loop();

  gSystem->Exec("rm *d *pcm *so");

}

void runAllAnalyses(std::vector<TString> inputFiles, TString runNumbers)
{

  TString runsName = "runs_" + runNumbers;

  gSystem->Exec("mkdir -p " + runsName + "/digi/");
  gSystem->Exec("mkdir -p " + runsName + "/trigger/");
  gSystem->Exec("mkdir -p " + runsName + "/segment/");

  //CB find a more elegant solution to pass directory
  DTNtupleDigiAnalyzer digiAnalysis(inputFiles, runsName + "/digi/results_digi.root", (runsName + "/digi/").Data());
  digiAnalysis.Loop();

  DTNtupleSegmentAnalyzer segmentAnalysis(inputFiles, runsName + "/segment/results_segment.root");
  segmentAnalysis.Loop();

  DTNtupleTriggerAnalyzer triggerAnalysis(inputFiles, runsName + "/trigger/results_trigger.root");
  triggerAnalysis.Loop();

  gSystem->Exec("rm *d *pcm *so");

}

void runAllAnalyses(TString xmlFile)
{

  TString runNumber = "";
  TString ntupleName = "";

  std::vector<TString> filesToCopy = {xmlFile};

  TXMLEngine xml;
  XMLDocPointer_t xmlDoc = xml.ParseFile(xmlFile);
  XMLNodePointer_t mainNode = xml.DocGetRootElement(xmlDoc);
 
  XMLNodePointer_t childNode = xml.GetChild(mainNode);
  while (childNode != 0) 
    {
      TString nodeName(xml.GetNodeName(childNode));
      TString nodeContent(xml.GetNodeContent(childNode));

      if (nodeName.EqualTo("runNumber"))
	{
	  runNumber = nodeContent;	  
	}
      else if (nodeName.EqualTo("ntupleName"))
	{
	  ntupleName = nodeContent;
	}
      else if (nodeName.Contains("File") 
	       && !nodeName.Contains("input")
	       && !nodeContent.EqualTo(""))
	{
	  filesToCopy.push_back(nodeContent);
	}

      childNode = xml.GetNext(childNode);
    }

  xml.FreeDoc(xmlDoc);

  TString maskFile = "./maskFile.txt";

  if (!gSystem->AccessPathName(maskFile))
    filesToCopy.push_back(maskFile);

  if (maskFile != "") gSystem->Exec("cp " + maskFile + " run" + runNumber  + "/cfg/");
  
  if (runNumber == "" || ntupleName == "" || !runNumber.IsDec())
    {
      std::cout << "[runAllAnalyses]: either runNumber or ntupleName not available in input xml or valid. quitting." << std::endl;
      return;
    } 
	
  gSystem->Exec("mkdir -p run" + runNumber + "/cfg/");
  
  for (const auto & fileToCopy : filesToCopy)
    gSystem->Exec("cp " + fileToCopy + " run" + runNumber + "/cfg/");

  runAllAnalyses(ntupleName, atoi(runNumber));

}
