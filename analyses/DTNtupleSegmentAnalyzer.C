#include "DTNtupleSegmentAnalyzer.h"
#include "TProfile.h"
#include "TCanvas.h"
#include <algorithm>
#include <fstream>
#include <iomanip>      // std::setprecision
#include "TMath.h"
#include "TF1.h"

DTNtupleSegmentAnalyzer::DTNtupleSegmentAnalyzer(const TString & inFileName,
						 const TString & outFileName) :
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)  
{ 

  m_timeBoxMin["Ph1"]  =  -750.;
  m_timeBoxMax["Ph1"]  =  4250.;

  m_timeBoxMin["Ph2"]  = -1250.;
  m_timeBoxMax["Ph2"]  =  3750.;

  TObjArray *tx = outFileName.Tokenize("/");
  m_deadFileName = (((TObjString *)(tx->At(0)))->String()).Data();
  cout<<"m_deadFileName "<<m_deadFileName<<endl;
}

DTNtupleSegmentAnalyzer::~DTNtupleSegmentAnalyzer() 
{ 

}


int deadPh1[1000000][6];
int NdeadPh1=0;

int deadPh2[1000000][6];
int NdeadPh2=0;

int nrequiredhit=7;
int MaxDead=0;


//Add preloop for both ph1 and ph2 //del
void DTNtupleSegmentAnalyzer::PreLoop(string Tag){

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t nRealEntries = fChain->GetEntries();

  //nentries=50000;

  Long64_t nbytes = 0, nb = 0;

  ofstream DeadList;
  std::string deadname;

  deadname.append("DeadList/DeadList_"+m_deadFileName+Tag);
  cout<<("Preloop on "+Tag+" digis to save dead channels. Info will be saved in DeadList/DeadList_"+m_deadFileName+Tag).c_str()<<endl;
  DeadList.open(deadname.c_str());
  //Create a Histogram per layer 
  char go;
  char hname[50];
  TH1F* occupancy[4][3][4];
      for (int ist=0; ist<4; ist++) {
	for (int isl=0; isl<3; isl++) {
	  if (isl==1 && ist==3) continue;
	  for (int ilay=0; ilay<4; ilay++) {
	    sprintf (hname,"MB%u_SL%u_Lay%u",ist+1,isl+1,ilay+1);
	    occupancy[ist][isl][ilay] = new TH1F (hname,"",92,1.,93.);
	  }
	}
      }

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if (jentry%50000 == 0) cout<<setprecision (2)<<" Pre-Loop event "<<jentry<<" "<<jentry/float(nRealEntries)*100.<<"%"<<endl;
    if(Tag=="Ph2")
      for (uint idigi=0; idigi<ph2Digi_nDigis; idigi++) {
	occupancy[ph2Digi_station->at(idigi)-1][ph2Digi_superLayer->at(idigi)-1][ph2Digi_layer->at(idigi)-1]->Fill(float(ph2Digi_wire->at(idigi)));
      }
    else
      for (uint idigi=0; idigi<digi_nDigis; idigi++) {
	occupancy[digi_station->at(idigi)-1][digi_superLayer->at(idigi)-1][digi_layer->at(idigi)-1]->Fill(float(digi_wire->at(idigi)));
      }
  }
  
  // analyze occupancy histos and fill dead channel table
  
  int nwire=0; int NwireTot=0;

  if(Tag=="Ph1")  for (int iw=0; iw<5000; iw++) for (int geo=0; geo<6; geo++) deadPh1[iw][geo]=0;
  else for (int iw=0; iw<5000; iw++) for (int geo=0; geo<6; geo++) deadPh2[iw][geo]=0;

  for (int ist=0; ist<4; ist++) {
    //	if (ist!=3 && ise>11) continue;
    //if (ist!=3) continue;
    for (int isl=0; isl<3; isl++) {
      if (isl==1 && ist==3) continue;
      if (isl==1) nwire=57; 
      else if (ist==0) nwire = 49;
      else if (ist==1) nwire = 60;
      else if (ist==2) nwire = 72;
      else if (ist==3) nwire = 92;	  	   
      
      NwireTot+=(nwire*4);
      for (int ilay=0; ilay<4; ilay++) {
	for (int iw=1; iw<nwire+1; iw++) {
	  if (occupancy[ist][isl][ilay]->GetBinContent(iw)==0){

	    if(Tag=="Ph1"){
	    deadPh1[NdeadPh1][0]=ist+1;
	    deadPh1[NdeadPh1][1]=isl+1;
	    deadPh1[NdeadPh1][2]=ilay+1;
	    deadPh1[NdeadPh1][3]=iw; 

	    DeadList <<NdeadPh1+1<<" ";
	    for (int ip=0; ip<6; ip++){
	      DeadList<< deadPh1[NdeadPh1][ip]<<" "; DeadList<<endl;
	    }
	    NdeadPh1++; 
	  }
	    else{
	    deadPh2[NdeadPh2][0]=ist+1;
	    deadPh2[NdeadPh2][1]=isl+1;
	    deadPh2[NdeadPh2][2]=ilay+1;
	    deadPh2[NdeadPh2][3]=iw; 

	    DeadList <<NdeadPh2+1<<" ";
	    for (int ip=0; ip<6; ip++){
	      DeadList<< deadPh2[NdeadPh2][ip]<<" "; DeadList<<endl;
	    }
	    NdeadPh2++; 
	  }
	    }
	    //if(Ndead==10000){cout<<"Error. Too many dead channels. "<<endl; abort();}
	  }
	}
      }
    }



  //Delete histrograms not to be saved in output file  
  for (int ist=0; ist<4; ist++) 
    for (int isl=0; isl<3; isl++){
      if (isl==1 && ist==3) continue;
      for (int ilay=0; ilay<4; ilay++){
	delete occupancy[ist][isl][ilay];
      }
    }
  

  if(Tag=="Ph1"){  cout<<NdeadPh1<<" dead wires out of "<<NwireTot<<endl;
  DeadList<<0<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<" "
          <<endl<<NdeadPh1<<" dead wires of out "<<NwireTot<<endl;
  }

  else{
    cout<<NdeadPh2<<" dead wires out of "<<NwireTot<<endl;
    DeadList<<0<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<" "
	    <<endl<<NdeadPh2<<" dead wires of out "<<NwireTot<<endl;
  }
  //Uncomment to see list of dead channels.

  // for (int idead=0; idead<Ndead; idead++) {
  //   cout<<"MB"<<dead[idead][0]<<"_SL"<<dead[idead][1]
  //       <<"_layer"<<dead[idead][2]<<"_cell"<<dead[idead][3]<<endl;
  // }
   
  DeadList.close();
}



void DTNtupleSegmentAnalyzer::Loop()
{

  book();

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();
  //nentries = 50000;

  //Read file with dead cells Ph1
  if (NdeadPh1==0) {
    cout<<" READING FROM FILE !! "<<endl;
    ifstream txtin;
    std::string deadname;

    deadname.append("DeadList/DeadList_"+m_deadFileName+"Ph1");
     
    cout<<" reading from "<<deadname<<endl;
    txtin.open(deadname.c_str(),std::ifstream::in);
     
    int idead    = 0;
    int prevlay  = 0; int prevSL = 0;
    int ndeadlay = 0;

    do {
      txtin>>idead>>deadPh1[NdeadPh1][0]>>deadPh1[NdeadPh1][1]>>deadPh1[NdeadPh1][2]>>deadPh1[NdeadPh1][3];
      if (deadPh1[NdeadPh1][1] == prevSL && deadPh1[NdeadPh1][2] == prevlay ) {
	ndeadlay++;
      }
      else {
	if (ndeadlay>20) cout<<"/MB"<<deadPh1[NdeadPh1][0]<<" SL"
			     <<deadPh1[NdeadPh1][1]<<" L"
			     <<deadPh1[NdeadPh1][2]<<" "<<ndeadlay+1<<" deads!!"<<endl;
	prevSL=deadPh1[NdeadPh1][1]; 
	prevlay=deadPh1[NdeadPh1][2]; 
	ndeadlay=0;
      }
      NdeadPh1++;
    }
    while (idead!=0);
  }


  //Read file with dead cells Ph2
  if (NdeadPh2==0) {
    cout<<" READING FROM FILE !! "<<endl;
    ifstream txtin;
    std::string deadname;

    deadname.append("DeadList/DeadList_"+m_deadFileName+"Ph2");
     
    cout<<" reading from "<<deadname<<endl;
    txtin.open(deadname.c_str(),std::ifstream::in);
     
    int idead    = 0;
    int prevlay  = 0; int prevSL = 0;
    int ndeadlay = 0;

    do {
      txtin>>idead>>deadPh2[NdeadPh2][0]>>deadPh2[NdeadPh2][1]>>deadPh2[NdeadPh2][2]>>deadPh2[NdeadPh2][3];
      if (deadPh2[NdeadPh2][1] == prevSL && deadPh2[NdeadPh2][2] == prevlay ) {
	ndeadlay++;
      }
      else {
	if (ndeadlay>20) cout<<"/MB"<<deadPh2[NdeadPh2][0]<<" SL"
			     <<deadPh2[NdeadPh2][1]<<" L"
			     <<deadPh2[NdeadPh2][2]<<" "<<ndeadlay+1<<" deads!!"<<endl;
	prevSL=deadPh2[NdeadPh2][1]; 
	prevlay=deadPh2[NdeadPh2][2]; 
	ndeadlay=0;
      }
      NdeadPh2++;
    }
    while (idead!=0);
  }



  //Main loop over entries
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEvent(jentry);   nbytes += nb;

      if(jentry % 100 == 0) 
	std::cout << "[DTNtupleSegmentAnalyzer::Loop] processed : " 
		  << jentry << " entries\r" << std::flush;

      DTNtupleBaseAnalyzer::LoadObjs();

      fill();
     
    }

  endJob();

}

void DTNtupleSegmentAnalyzer::book()
{
  
  m_outFile.cd();

  for (int st=1; st<5; st++)
    {

      auto hName = Form("NSegPh1VsPh2_st%d",st); 
      m_plots[hName] = new TH2F(hName,
				"# segment ph1 vs ph2;# seg (phase-1); # seg (phase-2)",
				10,-0.5,9.5,10,-0.5,9.5); 

      hName = Form("NHitsPhiPh1VsPh2_st%d",st);
      m_plots[hName] = new TH2F(hName,
				"# phi hits ph1 vs ph2;# phi hits (phase-1); # phi hits (phase-2)",
				10,-0.5,9.5,10,-0.5,9.5);

      hName = Form("NPh2Ph1HitsDiffPhiVsX_st%d",st);
      m_plots[hName] = new TH2F(hName,
				"# ph2 - ph1 phi hits vs x;local x (cm); # ph2 - ph1 phi hits",
				20,-200.,200.,17,-8.5,8.5);

      hName = Form("EffPhiPh1VsPh2_st%d",st);
      m_plots[hName] = new TH2F(hName,
				"eff ph1 vs ph2;# efficiency (phase-1); efficiency (phase-2)",
				101,0.9,1.01,101,0.9,1.01);
    }
	
  std::vector<std::string> typeTags = { "Ph1", "Ph2" };

  for (const auto & typeTag : typeTags)
    {
      
      auto hName = Form("%seffPhi_station",typeTag.c_str()); 
      m_effs[hName] = new TEfficiency(hName,
				      "#phi efficiency per station;station;eff.",
				      4,0.5,4.5);
      
      hName = Form("%seffTheta_station",typeTag.c_str()); 
      m_effs[hName] = new TEfficiency(hName,
				      "#theta efficiency per station;station;eff.",
				      4,0.5,4.5);
      
      hName = Form("%shitResZ",typeTag.c_str()); 
      m_plots[hName] = new TH1F(hName,
				"Hit residual z view;|d_hit|-|d_extr|(cm)",
				250,-1.,1.); 
      
      hName = Form("%shitResX",typeTag.c_str()); 
      m_plots[hName] = new TH1F(hName,
				"Hit residual X view;|d_hit|-|d_extr|(cm)",
				250,-.8,.8); 
      
      hName = Form("%shitResX_vsWireDist",typeTag.c_str()); 
      m_2Dplots[hName] = new TH2F(hName,
				  "Hit residual X view;dist wire;|d_hit|-|d_extr|(cm)",
				  100,0,2.5,200,-.8,.8); 

      hName = Form("%shitResZ_vsWireDist",typeTag.c_str()); 
      m_2Dplots[hName] = new TH2F(hName,
				  "Hit residual Z view;dist wire;|d_hit|-|d_extr|(cm)",
				  100,0,2.5,200,-.8,.8); 
      
      for (int st=1; st<5; st++)
	{
	  
	  hName = Form("%shPhiNhits_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName,
				    "# hits;# hits; entries",
				    8,0.5,8.5); 	  
	  
	  hName = Form("%shNhits_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName,
				    "# hits;# hits; entries",
				    12,0.5,12.5);  

	  hName = Form("%shT0_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName,
				    "Segment t0;t0 (ns); entries",
				    200,-100.,100.);  

	  hName = Form("%shT0VsX_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TProfile(hName,
					"Segment t0 vs local x;local x (cm);t0 (ns)",
					20,-200.,200.,-100.,100.);  

	  hName = Form("%shPhiProbChi2_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName, 
				    "Segment #phi prob(#chi^2); prob(#chi^2); entries", 
				    100,0.,1.);

	  hName = Form("%shPhiChi2_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName, 
				    "Segment #phi #chi^2;  #chi^2/ndof; entries", 
				    100,0.,50.);
	  
	  if (st < 4)
	    {
	      hName = Form("%shThetaNhits_st%d",typeTag.c_str(),st);
	      m_plots[hName] = new TH1F(hName,
					"# hits;# hits; entries",
					4,0.5,4.5); 
	      
	      hName = Form("%shThetaProbChi2_st%d",typeTag.c_str(),st);
	      m_plots[hName] = new TH1F(hName, 
					"Segment #theta Prob. #chi^2; prob(#chi^2); entries", 
					100,0.,1.);
	      
	      hName = Form("%shThetaChi2_st%d",typeTag.c_str(),st);
	      m_plots[hName] = new TH1F(hName, 
					"Segment #theta #chi^2; #chi^2/ndof; entries", 
					100,0.,50.);
	    }
	  
	  hName = Form("%shResPerSl_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName,
				    "Residual per Sl; Sl ; Residual [#mu m]",
				    3,0.5,3.5);

	  hName = Form("%seffPhiByWire_st%d",typeTag.c_str(),st); 
	  m_effs[hName] = new TEfficiency(hName,
					  "Wire by wire efficiency; wire; layer / superlayer",
					  100,0.5,100.5, 12, -0.5, 11.5);

	  hName = Form("%seffPhiSummary_st%d",typeTag.c_str(),st);
	  m_plots[hName] = new TH1F(hName,"Efficiency summary;efficiency,# wires",110,0.5,1.05);

	}
            
      for (int st=1; st<5; st++)
	{
	  for (int sl=1; sl<4; sl++)
	    {
	  
	      if(sl==2 && st!=4)
		{
		  hName = Form("%shitResZ_st%d",typeTag.c_str(),st);
		  m_plots[hName] = new TH1F(hName,
					    "Hit residual #theta view;|d_hit|-|d_extr|(cm)",
					    250,-.8,.8); 	
		  
		  
		  hName = Form("%shitResZvsWireDist_st%d",typeTag.c_str(),st);
		  m_2Dplots[hName] = new TH2F(hName,
					      "Hit residual #theta view;|d_wire|[cm];|d_hit|-|d_extr|(cm)",
					      100,0,2.5,250,-.8,.8); 	
		}
	      else
		{
		  hName = Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl);
		  m_plots[hName] = new TH1F(hName,
					    "Hit residual #phi view;|d_hit|-|d_extr|(cm)",
					    250,-.8,.8); 

		  hName = Form("%shitResXvsWireDist_st%d_sl%d",typeTag.c_str(),st,sl);
		  m_2Dplots[hName] = new TH2F(hName,
					      "Hit residual #phi view;|d_wire|[cm];|d_hit|-|d_extr|(cm)",
					      100,0,2.5, 250,-.8,.8);
		}
	      
	      for (int l=1; l<5; l++)
		{
		  if(sl==2 && st!=4)
		    {
		      hName = Form("%shitResZ_st%d_l%d",typeTag.c_str(),st,l); 
		      m_plots[hName] = new TH1F(hName,
						"Hit residual #theta view;|d_hit|-|d_extr|(cm)",
						250,-.8,.8); 	
		      
		      
		      hName = Form("%shitResZvsWireDist_st%d_l%d",typeTag.c_str(),st,l);
		      m_2Dplots[hName]  = new TH2F(hName,
						   "Hit residual #theta view;|d_wire|[cm];|d_hit|-|d_extr|(cm)",
						   100,0,2.5,250,-.8,.8); 	
		    }
		  else
		    {
		      hName = Form("%shitResX_st%d_sl%d_l%d",typeTag.c_str(),st,sl,l);
		      m_plots[hName] = new TH1F(hName,
						"Hit residual #phi view;|d_hit|-|d_extr|(cm)",
						250,-.8,.8); 
		      
		      hName = Form("%shitResXvsWireDist_st%d_sl%d_l%d",typeTag.c_str(),st,sl,l);
		      m_2Dplots[hName]  = new TH2F(hName,
						   "Hit residual #phi view;|d_wire|[cm];|d_hit|-|d_extr|(cm)",
						   100,0,2.5,250,-.8,.8);
		    }
		}
	    } 
	  
	}
      
    }  
  
}

void DTNtupleSegmentAnalyzer::fill()
{

  baseAnalysis();
  comparisonAnalysis();

  measureEfficiency("Ph1",NdeadPh1,deadPh1,segmentObjs["Ph1"],digiObjs["Ph1"]);
  measureEfficiency("Ph2",NdeadPh2,deadPh2,segmentObjs["Ph2"],digiObjs["Ph2"]);
  
  computeResidual("Ph1",segmentObjs["Ph1"]);
  computeResidual("Ph2",segmentObjs["Ph2"]);

}

void DTNtupleSegmentAnalyzer::endJob()
{

  std::vector<std::string> typeTags = { "Ph1", "Ph2" };
  
  for (const auto & typeTag : typeTags)
    {
      for (int st=1; st<5; ++st)
	{

	  string hName = Form("%seffPhiByWire_st%d",typeTag.c_str(),st);
	  
	  int nBinsX = m_effs[hName]->GetTotalHistogram()->GetNbinsX();
	  int nBinsY = m_effs[hName]->GetTotalHistogram()->GetNbinsY();

	  for (int iBinX = 1; iBinX <= nBinsX; ++ iBinX)
	    {
	      for (int iBinY = 1; iBinY <= nBinsY; ++ iBinY)
		{
		  
		  int iBin  = m_effs[hName]->GetGlobalBin(iBinX, iBinY);
		  float eff = m_effs[hName]->GetEfficiency(iBin);
		  if ( eff > 0.01 )
		    {
		      m_plots[Form("%seffPhiSummary_st%d",typeTag.c_str(),st)]->Fill(eff);
		    }			
		}
	    }
	  
	  for (int sl=1; sl<4; ++sl)
	    {
	
	      if(sl==2 && st!=4)
		{  

		  string hName = Form("%shitResZ_st%d",typeTag.c_str(),st);

		  auto histo = m_plots[hName];

		  double center = histo->GetBinCenter(histo->GetMaximumBin());
		  double sigma  = histo->GetStdDev();
		  
		  cout << "Fit " << hName << endl << endl;
		  
		  TF1 *  fTheta = new TF1("fTheta","gaus",
					  center - 1.2 * sigma,
					  center + 1.2 * sigma);
		  
		  m_plots[hName]->Fit("fTheta","RM");
		  
		  fTheta->SetRange(fTheta->GetParameter(1) - 1.1 * fTheta->GetParameter(2),
				   fTheta->GetParameter(1) + 1.1 * fTheta->GetParameter(2));
		  
		  m_plots[hName]->Fit("fTheta","RM");
		  
		  m_plots[Form("%shResPerSl_st%d",typeTag.c_str(),st)]->SetBinContent(sl + 0.5, 1e4 * fTheta->GetParameter(2)); 	
		}
	      else if(sl!=2)
		{
	  
		  string hName = Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl);
		  
		  auto histo = m_plots[hName];

		  double center = histo->GetBinCenter(histo->GetMaximumBin());
		  double sigma  = histo->GetStdDev();

		  cout << "Fit " << hName << endl;
		  
		  TF1 *  fPhi = new TF1("fPhi","gaus",
					center - 1.2 * sigma,
					center + 1.2 * sigma);
	  
		  m_plots[hName]->Fit("fPhi","RM");

		  fPhi->SetRange(fPhi->GetParameter(1) - 1.1 * fPhi->GetParameter(2),
				 fPhi->GetParameter(1) + 1.1 * fPhi->GetParameter(2));

		  m_plots[hName]->Fit("fPhi","RM");
	  
		  m_plots[Form("%shResPerSl_st%d",typeTag.c_str(),st)]->SetBinContent(sl+0.5,1e4*fPhi->GetParameter(2)); 	
		}

	      cout << endl << endl;
	    }
	}
    }

  for (int st=1; st<5; ++st)
    {

      string hNamePh1 = Form("Ph1effPhiByWire_st%d",st);
      string hNamePh2 = Form("Ph2effPhiByWire_st%d",st);
      
      int nBinsX = m_effs[hNamePh1]->GetTotalHistogram()->GetNbinsX();
      int nBinsY = m_effs[hNamePh1]->GetTotalHistogram()->GetNbinsY();
      
      for (int iBinX = 1; iBinX <= nBinsX; ++ iBinX)
	{
	  for (int iBinY = 1; iBinY <= nBinsY; ++ iBinY)
	    {
	      
	      int iBinPh1  = m_effs[hNamePh1]->GetGlobalBin(iBinX, iBinY);
	      float effPh1 = m_effs[hNamePh1]->GetEfficiency(iBinPh1);
	      
	      int iBinPh2  = m_effs[hNamePh2]->GetGlobalBin(iBinX, iBinY);
	      float effPh2 = m_effs[hNamePh2]->GetEfficiency(iBinPh2);
	      if ( effPh1 > 0.01 && effPh2 > 0.01 )
		{
		  m_plots[Form("EffPhiPh1VsPh2_st%d",st)]->Fill(std::max(effPh1, float(0.901)),
								std::max(effPh2, float(0.901)));
		}			
	    }
	}
    }

  m_outFile.cd();
  m_outFile.Write();
  m_outFile.Close();
  
}

void DTNtupleSegmentAnalyzer::baseAnalysis()
{

  //Phase1

  for (uint iSeg = 0; iSeg < seg_nSegments; ++iSeg) 
    {

      if(seg_sector->at(iSeg) != 12 || seg_wheel->at(iSeg) != 2) continue;    

      //Chi2 cut if needed
      
      //if(seg_hasPhi->at(iSeg)    && seg_phi_normChi2->at(iSeg) > 3.84) continue;
      //if(seg_hasZed->at(iSeg) && seg_z_normChi2->at(iSeg) > 3.84) continue;

      //Select segment with t0 < of a specific time  to avoid out of time events
      //if(abs(seg_phi_t0->at(iSeg)) > 15) continue;

      int nHits = 0;
      if(seg_hasPhi->at(iSeg))
	{
	  
	  nHits += seg_phi_nHits->at(iSeg);
	  m_plots[Form("Ph1hPhiNhits_st%d",seg_station->at(iSeg))]->Fill(seg_phi_nHits->at(iSeg));
	  m_plots[Form("Ph1hPhiChi2_st%d",seg_station->at(iSeg))]->Fill(seg_phi_normChi2->at(iSeg));

	  if(seg_phi_nHits->at(iSeg) == 8)
	    m_plots[Form("Ph1hPhiProbChi2_st%d",seg_station->at(iSeg))]->Fill(TMath::Prob(seg_phi_normChi2->at(iSeg)*6,6));

	  if (seg_phi_t0->at(iSeg) > -900)
	    {
	      float t0 = seg_phi_t0->at(iSeg);
	      float t0InRange = std::max(float(-99.9),(std::min(float(99.9),t0)));

	      m_plots[Form("Ph1hT0_st%d",seg_station->at(iSeg))]->Fill(t0InRange);
	      m_plots[Form("Ph1hT0VsX_st%d",seg_station->at(iSeg))]->Fill(seg_posLoc_x->at(iSeg), t0);
	    }
	  
	}

      if(seg_hasZed->at(iSeg))
	{

	  nHits += seg_z_nHits->at(iSeg);
	  m_plots[Form("Ph1hThetaNhits_st%d",seg_station->at(iSeg))]->Fill(seg_z_nHits->at(iSeg));

	  m_plots[Form("Ph1hThetaChi2_st%d",seg_station->at(iSeg))]->Fill(seg_z_normChi2->at(iSeg));

	  if(seg_z_nHits->at(iSeg) == 4)
	    m_plots[Form("Ph1hThetaProbChi2_st%d",seg_station->at(iSeg))]->Fill(TMath::Prob(seg_z_normChi2->at(iSeg)*2,2));

	}
      
      m_plots[Form("Ph1hNhits_st%d",seg_station->at(iSeg))]->Fill(nHits);

    }

  //Phase2

  for (uint iSeg = 0; iSeg < ph2Seg_nSegments; ++iSeg) 
    {
      
      if(ph2Seg_sector->at(iSeg) != 12 || ph2Seg_wheel->at(iSeg) != 2) continue;    

      //Chi2 cut if needed
      
      //if(ph2Seg_hasPhi->at(iSeg)    && ph2Seg_phi_normChi2->at(iSeg) > 3.84) continue;
      //if(ph2Seg_hasZed->at(iSeg) && ph2Seg_z_normChi2->at(iSeg) > 3.84) continue;
      
      //Select segment with t0 < of a specific time  to avoid out of time events
      //if(abs(ph2Seg_phi_t0->at(iSeg)) > 15) continue;
      
      int nHits = 0;
      if(ph2Seg_hasPhi->at(iSeg))
	{
	  
	  nHits += ph2Seg_phi_nHits->at(iSeg);
	  m_plots[Form("Ph2hPhiNhits_st%d",ph2Seg_station->at(iSeg))]->Fill(ph2Seg_phi_nHits->at(iSeg));
	  m_plots[Form("Ph2hPhiChi2_st%d",ph2Seg_station->at(iSeg))]->Fill(ph2Seg_phi_normChi2->at(iSeg));

	  if(ph2Seg_phi_nHits->at(iSeg) == 8)
	    m_plots[Form("Ph2hPhiProbChi2_st%d",ph2Seg_station->at(iSeg))]->Fill(TMath::Prob(ph2Seg_phi_normChi2->at(iSeg)*6,6));

	  if (ph2Seg_phi_t0->at(iSeg) > -900)
	    {
	      float t0 = ph2Seg_phi_t0->at(iSeg);
	      float t0InRange = std::max(float(-99.9),(std::min(float(99.9),t0)));

	      m_plots[Form("Ph2hT0_st%d",ph2Seg_station->at(iSeg))]->Fill(t0InRange);
	      m_plots[Form("Ph2hT0VsX_st%d",ph2Seg_station->at(iSeg))]->Fill(ph2Seg_posLoc_x->at(iSeg), t0);
	    }
	  
	}

      if(ph2Seg_hasZed->at(iSeg))
	{

	  nHits += ph2Seg_z_nHits->at(iSeg);
	  m_plots[Form("Ph2hThetaNhits_st%d",ph2Seg_station->at(iSeg))]->Fill(ph2Seg_z_nHits->at(iSeg));

	  m_plots[Form("Ph2hThetaChi2_st%d",ph2Seg_station->at(iSeg))]->Fill(ph2Seg_z_normChi2->at(iSeg));

	  if(ph2Seg_z_nHits->at(iSeg) == 4)
	    m_plots[Form("Ph2hThetaProbChi2_st%d",ph2Seg_station->at(iSeg))]->Fill(TMath::Prob(ph2Seg_z_normChi2->at(iSeg)*2,2));

	}

      m_plots[Form("Ph2hNhits_st%d",ph2Seg_station->at(iSeg))]->Fill(nHits);

    }

}

void DTNtupleSegmentAnalyzer::comparisonAnalysis()
{

  std::array<int, 4> nSegPerStPh1 = {0, 0, 0, 0};
  std::array<int, 4> nSegPerStPh2 = {0, 0, 0, 0};

  std::array<int, 4> iBestSegPerStPh1 = {-1, -1, -1, -1};
  std::array<int, 4> iBestSegPerStPh2 = {-1, -1, -1, -1};

  for (std::size_t iSeg = 0; iSeg < seg_nSegments; ++iSeg) 
    {

      if(seg_sector->at(iSeg) != 12 || seg_wheel->at(iSeg) != 2) continue;    

      if(seg_hasPhi->at(iSeg))
	{
	  
	  int stat  = seg_station->at(iSeg);
	  int nHits = seg_phi_nHits->at(iSeg);
	  int iBestSeg = iBestSegPerStPh1[stat - 1];
	  int nHitsBestSeg = iBestSeg > -1 ? seg_phi_nHits->at(iBestSeg) : 0;
	  
	  nSegPerStPh1[stat - 1]++;
	  
	  if (nHits > nHitsBestSeg)
	    iBestSegPerStPh1[stat - 1] = iSeg;

	}

    }

    for (std::size_t iSeg = 0; iSeg < ph2Seg_nSegments; ++iSeg) 
    {

      if(ph2Seg_sector->at(iSeg) != 12 || ph2Seg_wheel->at(iSeg) != 2) continue;    

      if(ph2Seg_hasPhi->at(iSeg))
	{
	  
	  int stat  = ph2Seg_station->at(iSeg);
	  int nHits = ph2Seg_phi_nHits->at(iSeg);
	  int iBestSeg = iBestSegPerStPh2[stat - 1];
	  int nHitsBestSeg = iBestSeg > -1 ? ph2Seg_phi_nHits->at(iBestSeg) : 0;
	  
	  nSegPerStPh2[stat - 1]++;
	  
	  if (nHits > nHitsBestSeg)
	    iBestSegPerStPh2[stat - 1] = iSeg;

	}
      
    }

    for (int iSt = 1; iSt <= 4; ++iSt)
      {

	int nSegPh1 = nSegPerStPh1[iSt -1];
	int nSegPh2 = nSegPerStPh2[iSt -1];
	
	if (nSegPh1 > 0 || nSegPh2 > 0)
	  {
	    m_plots[Form("NSegPh1VsPh2_st%d",iSt)]->Fill(nSegPh1,nSegPh2);

	    if (nSegPh1 > 0 && nSegPh2 > 0)
	      {
		int nHitsPh1 = seg_phi_nHits->at(iBestSegPerStPh1[iSt - 1]);
		int nHitsPh2 = ph2Seg_phi_nHits->at(iBestSegPerStPh2[iSt - 1]);

		m_plots[Form("NHitsPhiPh1VsPh2_st%d",iSt)]->Fill(nHitsPh1,nHitsPh2);

		float posXPh1 = seg_posLoc_x->at(iBestSegPerStPh1[iSt - 1]);
		float posXPh2 = ph2Seg_posLoc_x->at(iBestSegPerStPh2[iSt - 1]);

		if (std::abs(posXPh1 - posXPh2) < 10.)
		  m_plots[Form("NPh2Ph1HitsDiffPhiVsX_st%d",iSt)]->Fill(posXPh1,nHitsPh2 - nHitsPh1);		
	      }
	  }
	
      }

}

void DTNtupleSegmentAnalyzer::measureEfficiency(string Tag, int Ndead, int dead[][6],
						DTNtupleSegment & seg, DTNtupleDigi & digi)
{

  // ***********************
  // CB to be cleaned up yet
  // ***********************
  
  int ChambCross[100][3];
  for (int i=0; i<100; i++) for (int geo=0; geo<3; geo++) ChambCross[i][geo]=-999;
  int NChambCross=0;
  
  // First search for Phi segments
  
  for (uint iSeg=0; iSeg<(*seg.nSegments); iSeg++)
    {
      //selection
      if (!seg.hasPhi->at(iSeg)) continue;
      // In chambers 1,2,3 select only segments with also Z (theta) contribution.
      if(seg.sector->at(iSeg) != 12 || seg.wheel->at(iSeg) != 2) continue;     
      
      // if (station->at(iSeg)!=4 && !hasZed->at(iSeg)) continue;
    
      int seg_phinhits = seg.phi_nHits->at(iSeg);
      
      if (fabs(seg.dirLoc_x->at(iSeg))>0.7) continue; // angle
      
      
      auto expWire = (TVectorF*)seg.hitsExpWire->At(iSeg);
      
      // If a hit is missing, let us check that the extrapolation doesn't fall beyond layer or cross a dead cell!
      int NexpDead=0; bool OutOfLayer=false;
      
      if (seg_phinhits < 8 )
	{
	  for (int iex=0; iex<12; iex++)
	    {
	      int expSL = 1;
	      int expLay = iex+1;
	      //associate layer with right super layer
	      if (seg.station->at(iSeg) != 4)
		{
		  if (iex > 3 && iex<8)
		    {
		      expSL=2;
		      expLay-=4;
		    }
		  else if (iex>7)
		    {
		      expSL=3;
		      expLay-=8;
		    }
		}
	      else
		{
		  if (iex > 3 && iex<8) continue;
		  else if (iex > 7)
		    {
		      expSL=3;
		      expLay-=8;
		    }
		}
	      
	      int nwire=0;
	      
	      if (expSL==2)
		{
		  nwire=57; 
		}
	      else if (seg.station->at(iSeg)==1) nwire = 49;
	      else if (seg.station->at(iSeg)==2) nwire = 60;
	      else if (seg.station->at(iSeg)==3) nwire = 72;
	      else if (seg.station->at(iSeg)==4) nwire = 92;
	      
	      Float_t expW = (*expWire)(iex);
	      
	      if (expW>nwire)
		{
		  OutOfLayer=true;
		  break;
		}
	      
	      for (int idead=0; idead<Ndead; idead++)
		{
		  if (dead[idead][0] != seg.station->at(iSeg)) continue;
		  if (dead[idead][1] != expSL)  continue;  
		  if (dead[idead][2] != expLay) continue;
		  if (dead[idead][3] != expW)   continue; 
		  NexpDead++;
		  
		  break;
		}
	      
	      if (NexpDead>MaxDead) break;
	      
	    }
	  
	  if (OutOfLayer)       continue; // this segment goes out of layer boundary
	  if (NexpDead>MaxDead) continue; // this segment crosses dead cell(s): drop it!
	  
	}
      
      int NHits=0;
      int missingLayer[3][2];
      for (int imi=0; imi<3;imi++)
	{
	  missingLayer[imi][0]=0;
	  missingLayer[imi][1]=0;
	}
      int nmissing=0;
      
      auto hitSuperLayerPhi =(TVectorF*)seg.phiHits_superLayer->At(iSeg);
      auto hitLayerPhi      =(TVectorF*)seg.phiHits_layer->At(iSeg);
      auto hitWirePhi       =(TVectorF*)seg.phiHits_wire->At(iSeg);
      
      for (int ilay=1; ilay<9; ilay++)
	{
	  // Search for associated hits
	  bool foundh = false;
	  for (int kk=0; kk<seg_phinhits; kk++)
	    {
	      int sl1  = (*hitSuperLayerPhi)(kk);
	      int lay1 = (sl1==1) ? (*hitLayerPhi)(kk) : (*hitLayerPhi)(kk)+4; // hit layer 1-8
	  
	      if (lay1==ilay)
		{
		  NHits++;
		  foundh=true;
		  break;
		}
	    }
	  
	  if (!foundh)
	    {
	      if (nmissing<3)
		missingLayer[nmissing][0]=ilay;
	      nmissing++;
	    }
	}
      
      if (nmissing != 8-NHits)
	{
	  cout << NHits << " hits and " << nmissing << " missing!!" << endl;
	  return;
	}
      
      if (NHits<nrequiredhit)
	continue;
      else if (NHits==8)
	{
	  for (int sl=0; sl<2; sl++)
	    {
	      for (int lay=0; lay<4; lay++)
		{
		  //variables, points, stations, wheels 
		  m_effs[(Tag+"effPhi_station").c_str()]->Fill(1,seg.station->at(iSeg)-0.5);    
		  
		  int slAndLay = (lay) + (sl * 2) * 4;
		  //int layExp   = sl == 0 ? lay : lay + 4;
		  Float_t expW = (*expWire)(slAndLay);
		  m_effs[Form("%seffPhiByWire_st%d",Tag.c_str(),seg.station->at(iSeg))]->Fill(1,expW,slAndLay);
		}
	    }
	}
      else
	{ // let's see how to treat missing layers
	  for (int imiss=0; imiss<nmissing; imiss++)
	    {
	      int sl  = missingLayer[imiss][0] < 5 ? 0 : 1;
	      int lay = sl==0 ? missingLayer[imiss][0]-1 : missingLayer[imiss][0]-5;
	      // is there a digi within the expected tube?
	      
	      float digiW  = -1.;
	      float d      = 1000000; //just a very big number
	      int iex      = missingLayer[imiss][0] < 5 ? missingLayer[imiss][0]-1 : missingLayer[imiss][0]+3;
	      Float_t expW = (*expWire)(iex);  //perche` float?
	      
	      for (uint iDigi=0; iDigi < (*digi.nDigis); iDigi++)
		{
		  
		  if (digi.time->at(iDigi) < m_timeBoxMin.at(Tag) +  750. ||
		      digi.time->at(iDigi) > m_timeBoxMin.at(Tag) + 1650.)
		    continue;

		  if (digi.wheel->at(iDigi)   != seg.wheel->at(iSeg))   continue;
		  if (digi.sector->at(iDigi)  != seg.sector->at(iSeg))  continue;
		  if (digi.station->at(iDigi) != seg.station->at(iSeg)) continue;
		  
		  if (digi.superLayer->at(iDigi) == 2)            continue;
		  if (digi.superLayer->at(iDigi) == 1 && sl != 0) continue;
		  if (digi.superLayer->at(iDigi) == 3 && sl != 1) continue;
		  if (digi.layer->at(iDigi) != lay+1)     continue;
		  
		  // let's loop all over the digis and take the closest digis to the extrapolated wire. 
		  // Think about an extra condition on time.
		  
		  if (fabs(expW - digi.wire->at(iDigi))<fabs(d))
		    {
		      digiW = digi.wire->at(iDigi);
		      d=expW-digiW; 
		    }
		}
	      
	      if (fabs(d)< 1.1)
		{
		  missingLayer[imiss][1]=1;
		} //non dovrebbe essere sempre un intero d?

	    }
	  
	  if (NHits==nrequiredhit)
	    {
	      for (int imiss=0; imiss<nmissing; imiss++)
		{
		  int sl = missingLayer[imiss][0] < 5 ? 0 : 1;
		  int lay = sl==0 ? missingLayer[imiss][0]-1 : missingLayer[imiss][0]-5;
		  m_effs[(Tag+"effPhi_station").c_str()]->Fill(missingLayer[imiss][1],seg.station->at(iSeg)-0.5);
		  
		  int slAndLay = (lay) + (sl*2) * 4;
		  Float_t expW = (*expWire)(slAndLay);
		  m_effs[Form("%seffPhiByWire_st%d",Tag.c_str(),seg.station->at(iSeg))]->Fill(missingLayer[imiss][1],expW,slAndLay); 
		}
	    }
	  else
	    {
	      for (int sl=0; sl<2; sl++)
		{
		  for (int lay=0; lay<4; lay++)
		    {
		      bool missAss=false; bool missDigi=false;
		      for (int imiss=0; imiss<nmissing; imiss++)
			{
			  if (missingLayer[imiss][0]==sl*4+lay+1)
			    {
			      missAss=true;
			      if (!missingLayer[imiss][1])
				missDigi=true;
			    }
			}
		      m_effs[(Tag+"effPhi_station").c_str()]->Fill(!(missAss&&missDigi),seg.station->at(iSeg)-0.5);
		      
		      int slAndLay = (lay) + (sl * 2) * 4;
		      //int iex      = sl == 0 ? lay : lay + 4;
		      Float_t expW = (*expWire)(slAndLay);
		      m_effs[Form("%seffPhiByWire_st%d",Tag.c_str(),seg.station->at(iSeg))]->Fill(!(missAss&&missDigi),expW,slAndLay);
		    }
		}
	    }
	}
    }
  
  // Then search for Zed segments
  
  for (uint iSeg=0; iSeg<(*seg.nSegments); iSeg++)
    {
      if(seg.sector->at(iSeg) != 12 || seg.wheel->at(iSeg) != 2) continue;     
      if (!seg.hasZed->at(iSeg) || seg.phi_nHits->at(iSeg)<nrequiredhit) continue; 
      
      int seg_znhits = seg.z_nHits->at(iSeg);
      
      //if (fabs(dirLoc_y->at(iSeg))>0.7) continue; // angle WARNING!!! try and disable this for theta layers!
      if (seg_znhits < 3) continue; // piuttosto ovvio!!!  :-)
      
      auto expWire = (TVectorF*)seg.hitsExpWire->At(iSeg);

      // If a hit is missing, let us check that the extrapolation doesn't fall out of layer or cross a dead cell!
      int NexpDead=0;
      bool OutOfLayer=false;
      
      if (seg_znhits < 4)
	{
	  for (int iex=4; iex<8; iex++)
	    {
	      int expSL = 2;
	      int expLay = iex-3;		
	  
	      Float_t expW = (*expWire)(iex);
	      if (expW>58) {
		OutOfLayer=true;
		break;
	      }
	      for (int idead=0; idead<Ndead; idead++)
		{
		  if (dead[idead][1] != seg.station->at(iSeg)) continue;
		  if (dead[idead][2] != expSL) continue;  
		  if (dead[idead][3] != expLay) continue;
		  if (dead[idead][4] != expW) continue; 
		  NexpDead++;
		  break;   
		}
	      if (OutOfLayer) break;
	      if (NexpDead>MaxDead) break; 
	    }
	
	  if (NexpDead>MaxDead)
	      continue;
	}
      
      int NHits=0;
      int missingLayer=-1;
      
      auto hitLayerZ = (TVectorF*)seg.zHits_layer->At(iSeg);
      auto hitWireZ  = (TVectorF*)seg.zHits_wire->At(iSeg);
      
      for (int ilay=1; ilay<5; ilay++)
	{
	  // Search for associated hits
	  bool foundh = false;
	  for (int kk=0; kk<seg_znhits; kk++)
	    {
	      int lay1 = (*hitLayerZ)(kk);
	  
	      if (lay1==ilay)
		{
		  NHits++;
		  foundh=true;
		  break;
		}
	    }
	  if (!foundh)
	    missingLayer=ilay;
	}
      
      if (NHits<3)
	continue;
      else if (NHits==4)
	{
	  for (int lay=0; lay<4; lay++)
	    {
	      m_effs[(Tag+"effTheta_station").c_str()]->Fill(1,seg.station->at(iSeg)-0.5); 
	    }
	}
      else if (NHits==3)
	{
	  int lay = missingLayer-1;
	
	  // is there a digi within the expected tube?
	  float digiW=-1.;
	  float d =1000000;
	  int iex = missingLayer+3;
	  Float_t expW = (*expWire)(iex); 

	  for (uint iDigi=0; iDigi < (*digi.nDigis); iDigi++)
	    {	    
	      if (digi.time->at(iDigi) < m_timeBoxMin.at(Tag) +  750. ||
		  digi.time->at(iDigi) > m_timeBoxMin.at(Tag) + 1650.)
		continue;
	      
	      if (digi.wheel->at(iDigi)   != seg.wheel->at(iSeg))   continue;
	      if (digi.sector->at(iDigi)  != seg.sector->at(iSeg))  continue;
	      if (digi.station->at(iDigi) != seg.station->at(iSeg)) continue;
	      
	      if (digi.superLayer->at(iDigi) != 2) continue;
	      if (digi.layer->at(iDigi) != lay+1) continue;
	      
	      if (fabs(expW - digi.wire->at(iDigi))<fabs(d))
		{
		  digiW=digi.wire->at(iDigi);
		  d=expW-digiW;
		}
	    }
	  m_effs[(Tag+"effTheta_station").c_str()]->Fill((fabs(d)< 1.1),seg.station->at(iSeg)-0.5); 
	}
      else
	{
	  cout<<" what do you want?? NHits (z) = "<<NHits<<endl;
	  return;
	}
    }
}

void DTNtupleSegmentAnalyzer::computeResidual(string Tag, DTNtupleSegment & seg)
{
  
  for (std::size_t iSeg = 0; iSeg < (*seg.nSegments); ++iSeg)
    {

      if(seg.sector->at(iSeg) != 12 || seg.wheel->at(iSeg) != 2)
	continue;

      int stat = seg.station->at(iSeg);

      int segPhiNHits = seg.phi_nHits->at(iSeg);
      int segZNHits   = seg.z_nHits->at(iSeg);

      if (seg.hasPhi->at(iSeg) && segPhiNHits == 8)
	{

	  if(seg.phi_normChi2->at(iSeg) > 3.84) continue;

	  auto hitSuperLayerPhi = (TVectorF*)seg.phiHits_superLayer->At(iSeg);
	  auto hitLayerPhi      = (TVectorF*)seg.phiHits_layer->At(iSeg);
	  auto hitWirePhi       = (TVectorF*)seg.phiHits_wire->At(iSeg);
	  auto hitPosPhi        = (TVectorF*)seg.phiHits_pos->At(iSeg);     
	  auto hitPosPhiCh      = (TVectorF*)seg.phiHits_posCh->At(iSeg);	 
	  auto hitPosPhiErr     = (TVectorF*)seg.phiHits_posErr->At(iSeg);	 

	  auto hitPhiSide       = (TVectorF*)seg.phiHits_side->At(iSeg);	 
	  
	  auto hitExpPos        = (TVectorF*)seg.hitsExpPos->At(iSeg);     
	  auto hitExpPosCh      = (TVectorF*)seg.hitsExpPosCh->At(iSeg);     
	  auto hitExpWire       = (TVectorF*)seg.hitsExpWire->At(iSeg);     

	  auto hitWirePosPhi    = (TVectorF*)seg.phiHits_wirePos->At(iSeg);
	  
	  for (int iHit=0; iHit < segPhiNHits; ++iHit)
	    {

	      double wireDist = std::abs((*hitWirePosPhi)(iHit) - (*hitPosPhi)(iHit));
	      
	      if(iHit<4)
		{
		  
 		  double res = ((*hitPosPhi)(iHit) - (*hitExpPos)(iHit)) * ((*hitPhiSide)(iHit) == 2 ? 1 : -1);
		  
		  m_plots[Form("%shitResX",Tag.c_str())]->Fill(res);
		  m_plots[Form("%shitResX_st%d_sl%d",Tag.c_str(),stat,1)]->Fill(res);
		  m_plots[Form("%shitResX_st%d_sl%d_l%d",Tag.c_str(),stat,1,iHit+1)]->Fill(res);
		  
		  m_2Dplots[Form("%shitResX_vsWireDist",Tag.c_str())]->Fill(wireDist,res);
		  m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d",Tag.c_str(),stat,1)]->Fill(wireDist,res);
		  m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d_l%d",Tag.c_str(),stat,1,iHit+1)]->Fill(wireDist,res);
		  
		}
	      else
		{
		  
 		  double res = ((*hitPosPhi)(iHit) - (*hitExpPos)(iHit+4)) * ((*hitPhiSide)(iHit) == 2 ? 1 : -1);
	
		  m_plots[Form("%shitResX",Tag.c_str())]->Fill(res);
		  m_plots[Form("%shitResX_st%d_sl%d",Tag.c_str(),stat,3)]->Fill(res);
		  m_plots[Form("%shitResX_st%d_sl%d_l%d",Tag.c_str(),stat,3,iHit-3)]->Fill(res);	      
		  
		  m_2Dplots[Form("%shitResX_vsWireDist",Tag.c_str())]->Fill(wireDist,res);
		  m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d",Tag.c_str(),stat,3)]->Fill(wireDist,res);
		  m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d_l%d",Tag.c_str(),stat,3,iHit-3)]->Fill(wireDist,res);
		  
		}
	    }
	}
      
      if (seg.hasZed->at(iSeg) && segZNHits == 4)
	{

	  if(seg.z_normChi2->at(iSeg) > 3.84) continue;

	  auto hitZSide    = (TVectorF*)seg.zHits_side->At(iSeg);	 

	  auto hitExpPos   = (TVectorF*)seg.hitsExpPos->At(iSeg);     
	  auto hitExpPosCh = (TVectorF*)seg.hitsExpPosCh->At(iSeg);     
	  auto hitExpWire  = (TVectorF*)seg.hitsExpWire->At(iSeg);     

	  auto hitWirePosZ = (TVectorF*)seg.zHits_wirePos->At(iSeg);

	  auto hitLayerZ   = (TVectorF*)seg.zHits_layer->At(iSeg);
	  auto hitWireZ    = (TVectorF*)seg.zHits_wire->At(iSeg);
	  auto hitPosZ     = (TVectorF*)seg.zHits_pos->At(iSeg);     
	  auto hitPosZCh   = (TVectorF*)seg.zHits_posCh->At(iSeg);	 
	  auto hitPosZErr  = (TVectorF*)seg.zHits_posErr->At(iSeg);

	  for (int iHit=0; iHit<segZNHits; ++iHit)
	    {

	      double wirePos = std::abs((*hitWirePosZ)(iHit) - (*hitPosZ)(iHit));
	      double res     = ((*hitPosZ)(iHit) - (*hitExpPos)(iHit+4)) * ((*hitZSide)(iHit) == 2 ? 1 : -1);
	      
	      m_plots[Form("%shitResZ",Tag.c_str())]->Fill(res);
	      m_plots[Form("%shitResZ_st%d",Tag.c_str(),stat)]->Fill(res);
	      m_plots[Form("%shitResZ_st%d_l%d",Tag.c_str(),stat,iHit+1)]->Fill(res);	    
	      
	      m_2Dplots[Form("%shitResZ_vsWireDist",Tag.c_str())]->Fill(wirePos,res);
	      m_2Dplots[Form("%shitResZvsWireDist_st%d",Tag.c_str(),stat)]->Fill(wirePos,res);
	      m_2Dplots[Form("%shitResZvsWireDist_st%d_l%d",Tag.c_str(),stat,iHit+1)]->Fill(wirePos,res);
	      
	    }
	}
    }
}
