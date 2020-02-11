#ifndef DTNtupleDigiAnalyzer_h
#define DTNtupleDigiAnalyzer_h

#include "DTNtupleBaseAnalyzer.h"

#include "TFile.h"
#include "TEfficiency.h"
#include "TH1F.h"
#include "TH2F.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <tuple>
#include <map>

class WireId
{

 public:

 WireId(int chamb, int sl, int layer, int wire) : 
  m_chamb(chamb), m_sl(sl), m_layer(layer), m_wire(wire) { };

  int m_chamb;
  int m_sl;
  int m_layer;
  int m_wire;

  bool operator<(const WireId& rhs) const
  {
    return 
      std::tie(m_chamb, m_sl, m_layer, m_wire) < 
      std::tie(rhs.m_chamb, rhs.m_sl, rhs.m_layer, rhs.m_wire);
  };

  bool operator==(const WireId& rhs) const
  {
    return 
      std::tie(m_chamb, m_sl, m_layer, m_wire) == 
      std::tie(rhs.m_chamb, rhs.m_sl, rhs.m_layer, rhs.m_wire);
  };


};

class DTNtupleDigiAnalyzer : public DTNtupleBaseAnalyzer 
{

 public:
  
  DTNtupleDigiAnalyzer(const TString & inFileName,
		       const TString & outFileName,
		       std::string outFolder);

  ~DTNtupleDigiAnalyzer();

  virtual void Loop() override;

 protected:

  virtual void book() override;
  virtual void fill() override;
  virtual void endJob() override;

 private:

  void fillBasic(std::string typeTag, std::map<WireId, std::vector<float>> & digisByWire);
  void fillEff(std::string typeTag, const std::set<WireId> & wireIdProbes, const std::set<WireId> & wireIdRefs);
  std::set<WireId> wiresWithInTimeDigis(std::string typeTag, const std::map<WireId, std::vector<float>> & digisByWire) const;

  TFile m_outFile;

  std::string m_outFolder;

  std::map<std::string,std::vector<int>> m_stations;

  std::map<std::string,float> m_timeBoxMin;
  std::map<std::string,float> m_timeBoxMax;
  std::map<std::string,int>   m_timeBoxBins;

  std::map<TString, TH1*> m_plots;
  std::map<TString, TEfficiency*> m_effs;
  
};

#endif
