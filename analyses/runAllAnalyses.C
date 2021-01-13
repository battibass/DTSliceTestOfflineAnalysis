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
  segmentAnalysis.PreLoop();
  segmentAnalysis.Loop();

  DTNtupleTriggerAnalyzer triggerAnalysis(inputFile, runName + "/trigger/results_trigger.root");
  triggerAnalysis.Loop();

  gSystem->Exec("rm *d *pcm *so");

}

void runAllAnalyses(TString xmlFile)
{

  TString calibFolder = "/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2020_data/calib/";

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
	       && !nodeContent.EqualTo("")
	       && !nodeContent.Contains(calibFolder))
	{
	  filesToCopy.push_back(nodeContent);
	}

      childNode = xml.GetNext(childNode);
    }

  xml.FreeDoc(xmlDoc);
  
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
