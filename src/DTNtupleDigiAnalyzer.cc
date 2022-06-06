#include "DTNtupleDigiAnalyzer.h"

#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>

DTNtupleDigiAnalyzer::DTNtupleDigiAnalyzer(const TString &inFileName, const TString &outFileName, std::string outFolder)
    : DTNtupleBaseAnalyzer(inFileName), m_outFile(outFileName, "RECREATE"), m_outFolder(outFolder) {
  // The list of chambers to monitor
  m_stations["Ph1"].push_back(1);
  m_stations["Ph1"].push_back(2);
  m_stations["Ph1"].push_back(3);
  m_stations["Ph1"].push_back(4);

  m_stations["Ph2"].push_back(1);
  m_stations["Ph2"].push_back(2);
  m_stations["Ph2"].push_back(3);
  m_stations["Ph2"].push_back(4);

  m_timeBoxMin["Ph1"] = -750.;
  m_timeBoxMax["Ph1"] = 4250.;
  m_timeBoxBins["Ph1"] = 1250;

  m_timeBoxMin["Ph2"] = -1250.;
  m_timeBoxMax["Ph2"] = 3750.;
  m_timeBoxBins["Ph2"] = 1250;

  m_timeBoxMinTP[1]["Ph1"] = 350.;
  m_timeBoxMaxTP[1]["Ph1"] = 750.;
  m_timeBoxMinTP[2]["Ph1"] = 350.;
  m_timeBoxMaxTP[2]["Ph1"] = 750.;
  m_timeBoxMinTP[3]["Ph1"] = 350.;
  m_timeBoxMaxTP[3]["Ph1"] = 750.;
  m_timeBoxMinTP[4]["Ph1"] = 350.;
  m_timeBoxMaxTP[4]["Ph1"] = 750.;

  m_timeBoxMinTP[1]["Ph2"] = 0.;
  m_timeBoxMaxTP[1]["Ph2"] = 400.;
  m_timeBoxMinTP[2]["Ph2"] = 0.;
  m_timeBoxMaxTP[2]["Ph2"] = 400.;
  m_timeBoxMinTP[3]["Ph2"] = 0.;
  m_timeBoxMaxTP[3]["Ph2"] = 400.;
  m_timeBoxMinTP[4]["Ph2"] = 0.;
  m_timeBoxMaxTP[4]["Ph2"] = 400.;

  // File with the wires to mask to be used. Its lines should be of the form
  // MB2 SL3 L4 2 6 31
  // where 2, 6 and 31 are the wires from the fourth layer of the third superlayer
  // that should be mask from the second station.

  std::string maskFile = "./maskFile.txt";

  if (maskFile != "") {
    std::ifstream myfile(maskFile);

    if (myfile.is_open()) {
      std::string line;
      while (getline(myfile, line)) {
        TString tmpline = line;
        TString tok;
        Ssiz_t from = 0;
        int theStation = 0;
        int theSL = 0;
        int theL = 0;
        while (tmpline.Tokenize(tok, from, " ")) {
          //             if      (tok.Contains("MB")) theStation = atoi(tok.Data()[2]);
          //             else if (tok.Contains("SL")) theSL      = atoi(tok.Data()[2]);
          //             else if (tok.Contains("L"))  theL       = atoi(tok.Data()[1]);
          if (tok.Contains("MB")) {
            TString tmpstr(tok(2, 2));
            theStation = tmpstr.Atoi();
          } else if (tok.Contains("SL")) {
            TString tmpstr(tok(2, 2));
            theSL = tmpstr.Atoi();
          } else if (tok.Contains("L")) {
            TString tmpstr(tok(1, 1));
            theL = tmpstr.Atoi();
          } else
            maskedWires[theStation][theSL][theL].push_back(tok.Atoi());
        }
      }
      myfile.close();
    } else
      std::cout << "[DTNtupleDigiAnalyzer::DTNtupleDigiAnalyzer] Unable to open mask wire file: NO WIRE WILL BE MASKED!"
                << std::endl;
  }
}

DTNtupleDigiAnalyzer::DTNtupleDigiAnalyzer(const std::vector<TString> &inFileNames,
                                           const TString &outFileName,
                                           std::string outFolder)
    : DTNtupleBaseAnalyzer(inFileNames), m_outFile(outFileName, "RECREATE"), m_outFolder(outFolder) {
  // The list of chambers to monitor
  m_stations["Ph1"].push_back(1);
  m_stations["Ph1"].push_back(2);
  m_stations["Ph1"].push_back(3);
  m_stations["Ph1"].push_back(4);

  m_stations["Ph2"].push_back(1);
  m_stations["Ph2"].push_back(2);
  m_stations["Ph2"].push_back(3);
  m_stations["Ph2"].push_back(4);

  m_timeBoxMin["Ph1"] = -750.;
  m_timeBoxMax["Ph1"] = 4250.;
  m_timeBoxBins["Ph1"] = 1250;

  m_timeBoxMin["Ph2"] = -1250.;
  m_timeBoxMax["Ph2"] = 3750.;
  m_timeBoxBins["Ph2"] = 1250;

  m_timeBoxMinTP[1]["Ph1"] = 350.;
  m_timeBoxMaxTP[1]["Ph1"] = 750.;
  m_timeBoxMinTP[2]["Ph1"] = 350.;
  m_timeBoxMaxTP[2]["Ph1"] = 750.;
  m_timeBoxMinTP[3]["Ph1"] = 350.;
  m_timeBoxMaxTP[3]["Ph1"] = 750.;
  m_timeBoxMinTP[4]["Ph1"] = 350.;
  m_timeBoxMaxTP[4]["Ph1"] = 750.;

  m_timeBoxMinTP[1]["Ph2"] = 0.;
  m_timeBoxMaxTP[1]["Ph2"] = 400.;
  m_timeBoxMinTP[2]["Ph2"] = 0.;
  m_timeBoxMaxTP[2]["Ph2"] = 400.;
  m_timeBoxMinTP[3]["Ph2"] = 0.;
  m_timeBoxMaxTP[3]["Ph2"] = 400.;
  m_timeBoxMinTP[4]["Ph2"] = 0.;
  m_timeBoxMaxTP[4]["Ph2"] = 400.;

  // File with the wires to mask to be used. Its lines should be of the form
  // MB2 SL3 L4 2 6 31
  // where 2, 6 and 31 are the wires from the fourth layer of the third superlayer
  // that should be mask from the second station.

  std::string maskFile = "./maskFile.txt";

  if (maskFile != "") {
    std::ifstream myfile(maskFile);

    if (myfile.is_open()) {
      std::string line;
      while (getline(myfile, line)) {
        TString tmpline = line;
        TString tok;
        Ssiz_t from = 0;
        int theStation = 0;
        int theSL = 0;
        int theL = 0;
        while (tmpline.Tokenize(tok, from, " ")) {
          //             if      (tok.Contains("MB")) theStation = atoi(tok.Data()[2]);
          //             else if (tok.Contains("SL")) theSL      = atoi(tok.Data()[2]);
          //             else if (tok.Contains("L"))  theL       = atoi(tok.Data()[1]);
          if (tok.Contains("MB")) {
            TString tmpstr(tok(2, 2));
            theStation = tmpstr.Atoi();
          } else if (tok.Contains("SL")) {
            TString tmpstr(tok(2, 2));
            theSL = tmpstr.Atoi();
          } else if (tok.Contains("L")) {
            TString tmpstr(tok(1, 1));
            theL = tmpstr.Atoi();
          } else
            maskedWires[theStation][theSL][theL].push_back(tok.Atoi());
        }
      }
      myfile.close();
    } else
      std::cout << "[DTNtupleDigiAnalyzer::DTNtupleDigiAnalyzer] Unable to open mask wire file: NO WIRE WILL BE MASKED!"
                << std::endl;
  }
}

DTNtupleDigiAnalyzer::~DTNtupleDigiAnalyzer() {}

void DTNtupleDigiAnalyzer::Loop() {
  book();

  if (fChain == 0)
    return;

  Long64_t nentries = fChain->GetEntries();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0)
      break;
    nb = fChain->GetEvent(jentry);
    nbytes += nb;

    if (jentry % 100 == 0)
      std::cout << "[DTNtupleDigiAnalyzer::Loop] processed : " << jentry << " entries\r" << std::flush;

    DTNtupleBaseAnalyzer::LoadObjs();

    fill();
  }

  std::cout << std::endl;

  endJob();
}

void DTNtupleDigiAnalyzer::book() {
  m_outFile.cd();

  std::vector<std::string> typeTags = {"Ph1", "Ph2"};

  for (const auto &typeTag : typeTags) {
    TString hName = ("hNDigis" + typeTag).c_str();
    m_plots[hName] = new TH1F(hName, "# of digis ;# of digis,entries", 101, -0.5, 100.5);

    for (const auto iSt : m_stations[typeTag]) {
      std::stringstream stTagS;
      stTagS << typeTag << "St" << iSt;

      std::string stTag = stTagS.str();

      hName = ("hOccupancy" + stTag).c_str();
      m_plots[hName] = new TH2F(hName, "Occupancy;wire;layer / superlayer", 100, 0.5, 100.5, 12, 0.5, 12.5);

      hName = ("hOccupancyMultiple" + stTag).c_str();
      m_plots[hName] = new TH2F(hName, "Occupancy;wire;layer / superlayer", 100, 0.5, 100.5, 12, 0.5, 12.5);

      hName = ("hAvgDigiTime" + stTag).c_str();
      m_plots[hName] = new TProfile2D(hName,
                                      "Average digi time;wire;layer / superlayer",
                                      100,
                                      0.5,
                                      100.5,
                                      12,
                                      0.5,
                                      12.5,
                                      m_timeBoxMinTP[iSt][typeTag],
                                      m_timeBoxMaxTP[iSt][typeTag]);

      hName = ("hWireByWireEff" + stTag).c_str();
      m_effs[hName] = new TEfficiency(
          hName, "Wire by wire matching efficiency;wire;layer / superlayer", 100, 0.5, 100.5, 12, 0.5, 12.5);

      hName = ("hEffSummary" + stTag).c_str();
      m_plots[hName] = new TH1F(hName, "Efficiency summary;efficiency,# wires", 110, 0.5, 1.05);

      hName = ("hTimeDiffPhiSL1" + stTag).c_str();
      m_plots[hName] = new TH1F(hName, "Wire by wire digi time difference;time difference;entries", 125, 0., 500.);
      hName = ("hTimeDiffPhiSL3" + stTag).c_str();
      m_plots[hName] = new TH1F(hName, "Wire by wire digi time difference;time difference;entries", 125, 0., 500.);

      hName = ("hTimeDiffTheta" + stTag).c_str();
      m_plots[hName] = new TH1F(hName, "Wire by wire digi time difference;time difference;entries", 125, 0., 500.);

      for (int iSl = 1; iSl <= 3; ++iSl) {
        for (int iLayer = 1; iLayer <= 4; ++iLayer) {
          std::stringstream layerTagS;
          layerTagS << typeTag << "St" << iSt << "SuperLayer" << iSl << "Layer" << iLayer;

          std::string layerTag = layerTagS.str();

          hName = ("hTimeBox" + layerTag).c_str();
          m_plots[hName] = new TH1F(hName,
                                    "Digi time box;time (ns);entries",
                                    m_timeBoxBins[typeTag],
                                    m_timeBoxMin[typeTag],
                                    m_timeBoxMax[typeTag]);
        }

        std::stringstream slTagS;
        slTagS << typeTag << "St" << iSt << "SuperLayer" << iSl;

        std::string slTag = slTagS.str();

        hName = ("hIneffWires" + slTag).c_str();
        m_plots[hName] = new TH1F(hName, "# ineff wires per SL;# inefficient wires;entries", 10, 0.5, 10.5);

        hName = ("hTimeIneffWires" + slTag).c_str();
        m_plots[hName] =
            new TH1F(hName, "time for cases wit >= 4 ineff wires per SL;lumisection;entries", 2000, 0.5, 2000.5);
      }
    }

    hName = ("hFracEffWires" + typeTag).c_str();
    m_effs[hName] = new TEfficiency(hName, "Fraction of wires with eff in [0.90:1.00] range", 4, 0.5, 4.5);
  }
}

void DTNtupleDigiAnalyzer::fill() {
  std::map<WireId, std::vector<float>> digisByWire;
  std::map<WireId, std::vector<float>> ph2DigisByWire;

  fillBasic("Ph1", digisByWire);
  fillBasic("Ph2", ph2DigisByWire);

  auto wireIds = wiresWithInTimeDigis("Ph1", digisByWire);
  auto ph2WireIds = wiresWithInTimeDigis("Ph2", ph2DigisByWire);

  fillEff("Ph1", wireIds, ph2WireIds);
  fillEff("Ph2", ph2WireIds, wireIds);
}

void DTNtupleDigiAnalyzer::fillBasic(std::string typeTag, std::map<WireId, std::vector<float>> &digisByWire) {
  DTNtupleDigi &digi = digiObjs[typeTag];

  int nDigis = 0;

  for (std::size_t iDigi = 0; iDigi < (*digi.nDigis); ++iDigi) {
    // The slice test sector
    if (digi.sector->at(iDigi) != 12 || digi.wheel->at(iDigi) != 2)
      continue;

    int st = digi.station->at(iDigi);
    int sl = digi.superLayer->at(iDigi);
    int lay = digi.layer->at(iDigi);
    int wire = digi.wire->at(iDigi);

    if (maskedWires.count(st)) {
      if (maskedWires[st].count(sl)) {
        if (maskedWires[st][sl].count(lay)) {
          if (std::count(maskedWires[st][sl][lay].begin(), maskedWires[st][sl][lay].end(), wire))
            //cout << "masked MB" << st << " SL" << sl << " L" << lay << " w" << wire << std::endl;
            continue;
        }
      }
    }

    ++nDigis;

    int slAndLay = lay + (sl - 1) * 4;

    double time = digi.time->at(iDigi);

    std::stringstream stTagS;
    stTagS << typeTag << "St" << st;

    std::string stTag = stTagS.str();

    m_plots[("hOccupancy" + stTag).c_str()]->Fill(wire, slAndLay);
    static_cast<TProfile2D *>(m_plots[("hAvgDigiTime" + stTag).c_str()])->Fill(wire, slAndLay, time);

    WireId wireId(st, sl, lay, wire);
    digisByWire[wireId].push_back(time);

    std::stringstream layerTagS;
    layerTagS << typeTag << "St" << st << "SuperLayer" << sl << "Layer" << lay;

    std::string layerTag = layerTagS.str();

    m_plots[("hTimeBox" + layerTag).c_str()]->Fill(time);
  }

  m_plots[("hNDigis" + typeTag).c_str()]->Fill(nDigis);

  for (auto &wireAndDigis : digisByWire) {
    auto &wireId = wireAndDigis.first;
    std::stringstream stTagS;
    stTagS << typeTag << "St" << wireId.m_chamb;

    std::string stTag = stTagS.str();
    /*
      if (maskedWires.count(wireId.m_chamb))
        {
          if (maskedWires[wireId.m_chamb].count(wireId.m_sl))
            {
              if (maskedWires[wireId.m_chamb][wireId.m_sl].count(wireId.m_layer))
                {
                  if (std::count(maskedWires[wireId.m_chamb][wireId.m_sl][wireId.m_layer].begin(), maskedWires[wireId.m_chamb][wireId.m_sl][wireId.m_layer].end(), wireId.m_wire)) continue;
                }
            }
        }
*/
    auto &digiTimes = wireAndDigis.second;

    if (digiTimes.size() > 1) {
      std::sort(digiTimes.begin(), digiTimes.end());

      auto digiIt = digiTimes.begin();
      auto digiEnd = digiTimes.end();

      double timePrev = (*digiIt);
      ++digiIt;
      m_plots[("hOccupancyMultiple" + stTag).c_str()]->Fill(wireId.m_wire, wireId.m_layer + (wireId.m_sl - 1) * 4);

      for (; digiIt != digiEnd; ++digiIt) {
        if (wireId.m_sl == 2)
          m_plots[("hTimeDiffTheta" + stTag).c_str()]->Fill((*digiIt) - timePrev);
        else if (wireId.m_sl == 1)
          m_plots[("hTimeDiffPhiSL1" + stTag).c_str()]->Fill((*digiIt) - timePrev);
        else if (wireId.m_sl == 3)
          m_plots[("hTimeDiffPhiSL3" + stTag).c_str()]->Fill((*digiIt) - timePrev);
        timePrev = (*digiIt);
      }
    }
  }
}

std::set<WireId> DTNtupleDigiAnalyzer::wiresWithInTimeDigis(
    std::string typeTag, const std::map<WireId, std::vector<float>> &digisByWire) const {
  std::set<WireId> wireIds;

  for (const auto &wireAndDigis : digisByWire) {
    const auto &wireId = wireAndDigis.first;
    const auto &digiTimes = wireAndDigis.second;

    for (const auto &digiTime : digiTimes) {
      if (digiTime > m_timeBoxMin.at(typeTag) + 750. && digiTime < m_timeBoxMin.at(typeTag) + 2250.) {
        wireIds.insert(wireId);
        break;
      }
    }
  }

  return wireIds;
}

void DTNtupleDigiAnalyzer::fillEff(std::string typeTag,
                                   const std::set<WireId> &wireIdProbes,
                                   const std::set<WireId> &wireIdRefs) {
  std::map<std::string, int> ineffWiresPerSL;

  for (const auto &wireIdRef : wireIdRefs) {
    std::stringstream stTagS;
    stTagS << typeTag << "St" << wireIdRef.m_chamb;

    std::string stTag = stTagS.str();

    std::stringstream slTagS;
    slTagS << stTag << "SuperLayer" << wireIdRef.m_sl;

    std::string slTag = slTagS.str();

    bool hasWireMatch = false;

    for (const auto &wireIdProbe : wireIdProbes) {
      if (wireIdRef == wireIdProbe) {
        hasWireMatch = true;
        break;
      }
    }

    int slAndLay = wireIdRef.m_layer + (wireIdRef.m_sl - 1) * 4;
    m_effs[("hWireByWireEff" + stTag).c_str()]->Fill(hasWireMatch, wireIdRef.m_wire, slAndLay);

    if (!hasWireMatch) {
      if (ineffWiresPerSL.find(slTag) == ineffWiresPerSL.end())
        ineffWiresPerSL[slTag] = 0;

      ineffWiresPerSL[slTag]++;
    }
  }

  for (auto &ineffWiresPerSLPair : ineffWiresPerSL) {
    auto slTag = ineffWiresPerSLPair.first;
    auto nIneffWires = ineffWiresPerSLPair.second;

    m_plots[("hIneffWires" + slTag).c_str()]->Fill(nIneffWires);

    if (nIneffWires >= 4)
      m_plots[("hTimeIneffWires" + slTag).c_str()]->Fill(event_lumiBlock);
  }
}

void DTNtupleDigiAnalyzer::endJob() {
  std::vector<std::string> typeTags = {"Ph1", "Ph2"};

  for (const auto &typeTag : typeTags) {
    for (const auto &iSt : m_stations[typeTag]) {
      std::ofstream outTxtFile;

      std::string tag = typeTag + "St" + std::to_string(iSt);

      outTxtFile.open(m_outFolder + "/" + "ineffCh" + tag + ".txt");
      outTxtFile << "+--------------------------------+\n";
      outTxtFile << "| SL | layer | wire | efficiency |\n";

      TString hName = ("hWireByWireEff" + tag).c_str();

      int nBinsX = m_effs[hName]->GetTotalHistogram()->GetNbinsX();
      int nBinsY = m_effs[hName]->GetTotalHistogram()->GetNbinsY();

      for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
          int iBin = m_effs[hName]->GetGlobalBin(iBinX, iBinY);
          float eff = m_effs[hName]->GetEfficiency(iBin);

          if (eff > 0.01) {
            m_plots[("hEffSummary" + tag).c_str()]->Fill(eff);
            m_effs[("hFracEffWires" + typeTag).c_str()]->Fill(eff >= 0.9, iSt);
            if (eff < 0.9) {
              outTxtFile << "| " << std::setw(2) << (iBinY / 4 + 1) << " | " << std::setw(5) << (iBinY % 4) << " | "
                         << std::setw(4) << iBinX << " | " << std::setw(10) << std::setprecision(3) << eff << " |\n";
            }
          }
        }
      }

      outTxtFile.close();

      auto hAvgDigiTime = static_cast<TProfile2D *>(m_plots[("hAvgDigiTime" + tag).c_str()]);
      float avgDigiTimeChamb = hAvgDigiTime->GetMean(3);  // z axis

      hName = ("hAvgDigiTimeSummary" + tag).c_str();
      auto hAvgDigiTimeSummary = new TH1F(hName,
                                          "Average digi time summary;average digi time;# wires",
                                          50,
                                          avgDigiTimeChamb - 25.,
                                          avgDigiTimeChamb + 25.);

      nBinsX = hAvgDigiTime->GetNbinsX();
      nBinsY = hAvgDigiTime->GetNbinsY();

      for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
          int iBin = hAvgDigiTime->GetBin(iBinX, iBinY);
          if (hAvgDigiTime->GetBinEntries(iBin) > 10) {
            hAvgDigiTimeSummary->Fill(hAvgDigiTime->GetBinContent(iBin));
          }
        }
      }
    }
  }

  std::for_each(m_effs.begin(), m_effs.end(), [](auto p) { p.second->Write(); });
  m_outFile.Write();
  m_outFile.Close();
}
