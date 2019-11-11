#include "DTNtupleSegmentAnalyzer.h"
#include "TCanvas.h"
#include <fstream>
#include <iomanip>      // std::setprecision
#include "TMath.h"
#include "TF1.h"

DTNtupleSegmentAnalyzer::DTNtupleSegmentAnalyzer(const TString & inFileName,
						 const TString & outFileName) :
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)  
{ 


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



      //-----------------------------------------------------------//
      //-----------------------Efficiency--------------------------//
      //-----------------------------------------------------------//



      MeasureEfficiency("Ph1",jentry,
			NdeadPh1,
			deadPh1,
			seg_nSegments,seg_hasPhi, 
			seg_station, 
			seg_wheel, 
			seg_sector,  
			seg_hasZed,  
			seg_phi_nHits, 
			seg_z_nHits,
			seg_dirLoc_x, 
			seg_hitsExpWire,
			seg_phiHits_pos,
			seg_phiHits_posCh,
			seg_phiHits_posErr,
			seg_phiHits_side,
			seg_phiHits_wire,
			seg_phiHits_wirePos,
			seg_phiHits_layer,
			seg_phiHits_superLayer,
			seg_zHits_pos,
			seg_zHits_posCh,
			seg_zHits_posErr,
			seg_zHits_side,
			seg_zHits_wire,
			seg_zHits_wirePos,
			seg_zHits_layer,
			digi_nDigis,
			digi_wheel,
			digi_sector,
			digi_station,
			digi_superLayer,
			digi_layer,
			digi_wire,
			digi_time
			);

      MeasureEfficiency("Ph2",jentry,
			NdeadPh2,
			deadPh2,
			ph2Seg_nSegments,ph2Seg_hasPhi, 
			ph2Seg_station, 
			ph2Seg_wheel, 
			ph2Seg_sector,  
			ph2Seg_hasZed,  
			ph2Seg_phi_nHits, 
			ph2Seg_z_nHits,
			ph2Seg_dirLoc_x, 
			ph2Seg_hitsExpWire,
			ph2Seg_phiHits_pos,
			ph2Seg_phiHits_posCh,
			ph2Seg_phiHits_posErr,
			ph2Seg_phiHits_side,
			ph2Seg_phiHits_wire,
			ph2Seg_phiHits_wirePos,
			ph2Seg_phiHits_layer,
			ph2Seg_phiHits_superLayer,
			ph2Seg_zHits_pos,
			ph2Seg_zHits_posCh,
			ph2Seg_zHits_posErr,
			ph2Seg_zHits_side,
			ph2Seg_zHits_wire,
			ph2Seg_zHits_wirePos,
			ph2Seg_zHits_layer,
			ph2Digi_nDigis,
			ph2Digi_wheel,
			ph2Digi_sector,
			ph2Digi_station,
			ph2Digi_superLayer,
			ph2Digi_layer,
			ph2Digi_wire,
			ph2Digi_time
			);
     
      //-----------------------------------------------------------//
      //------------------------Residual---------------------------//
      //-----------------------------------------------------------//


      ComputeResidual("Ph1",jentry, 
		      seg_nSegments,
		      seg_hasPhi, 
		      seg_station, 
		      seg_wheel, 
		      seg_sector,  
		      seg_hasZed,  
		      seg_phi_nHits, 
		      seg_z_nHits,
		      seg_phi_normChi2,
		      seg_z_normChi2,
		      seg_hitsExpWire,
		      seg_hitsExpPos,
		      seg_hitsExpPosCh,
		      seg_phiHits_pos,
		      seg_phiHits_posCh,
		      seg_phiHits_posErr,
		      seg_phiHits_side,
		      seg_phiHits_wire,
		      seg_phiHits_wirePos,
		      seg_phiHits_layer,
		      seg_phiHits_superLayer,
		      seg_zHits_pos,
		      seg_zHits_posCh,
		      seg_zHits_posErr,
		      seg_zHits_side,
		      seg_zHits_wire,
		      seg_zHits_wirePos,
		      seg_zHits_layer
		      );


      ComputeResidual("Ph2",jentry, 
		      ph2Seg_nSegments,
		      ph2Seg_hasPhi, 
		      ph2Seg_station, 
		      ph2Seg_wheel, 
		      ph2Seg_sector,  
		      ph2Seg_hasZed,  
		      ph2Seg_phi_nHits, 
		      ph2Seg_z_nHits,
		      ph2Seg_phi_normChi2,
		      ph2Seg_z_normChi2,
		      ph2Seg_hitsExpWire,
		      ph2Seg_hitsExpPos,
		      ph2Seg_hitsExpPosCh,
		      ph2Seg_phiHits_pos,
		      ph2Seg_phiHits_posCh,
		      ph2Seg_phiHits_posErr,
		      ph2Seg_phiHits_side,
		      ph2Seg_phiHits_wire,
		      ph2Seg_phiHits_wirePos,
		      ph2Seg_phiHits_layer,
		      ph2Seg_phiHits_superLayer,
		      ph2Seg_zHits_pos,
		      ph2Seg_zHits_posCh,
		      ph2Seg_zHits_posErr,
		      ph2Seg_zHits_side,
		      ph2Seg_zHits_wire,
		      ph2Seg_zHits_wirePos,
		      ph2Seg_zHits_layer
		      );
          
     fill();
     
    }

  endJob();

}

void DTNtupleSegmentAnalyzer::book()
{
  
  m_outFile.cd();

  std::vector<std::string> typeTags = { "Ph1", "Ph2" };

  for (const auto typeTag : typeTags){
    
    m_plots[(typeTag+"hNsegment").c_str()] = new TH1F((typeTag+"hNsegment").c_str(),
							 "# segments;# segments; entries",
							 20,0.,20.); 
    
    for (int st=1; st<5; st++)
      {

	m_plots[Form("%shPhiNhits_st%d",typeTag.c_str(),st)] = new TH1F(Form("%shPhiNhits_st%d",typeTag.c_str(),st),
							 "# hits;# hits; entries",
							 8,1,9); 
	

	m_plots[Form("%shThetaNhits_st%d",typeTag.c_str(),st)] = new TH1F(Form("%shThetaNhits_st%d",typeTag.c_str(),st),
							 "# hits;# hits; entries",
							 4,1,5); 
	
		
	m_plots[Form("%shNhits_st%d",typeTag.c_str(),st)] = new TH1F(Form("%shNhits_st%d",typeTag.c_str(),st),
						   "# hits;# hits; entries",
						   12,1,13);  
	
	m_plots[Form("%shT0_st%d",typeTag.c_str(),st)] = new TH1F(Form("%shT0_st%d",typeTag.c_str(),st),
						"Segment t0;t0 (ns); entries",
						200,-100.,100.);  
	
	m_plots[Form("%shPhiProbChi2_st%d",typeTag.c_str(),st)] = new TH1F(Form("%shPhiProbChi2_st%d",typeTag.c_str(),st), 
							 Form("Segment #phi prob(#chi^2) st%d; prob(#chi^2); entries",st), 
							 100,0.,1.);
	
	m_plots[Form("%shThetaProbChi2_st%d",typeTag.c_str(),st)] = new TH1F(Form("%shThetaProbChi2_st%d",typeTag.c_str(),st), 
							   Form("Segment #theta Prob. #chi^2 st%d; prob(#chi^2); entries",st), 
							   100,0.,1.);
	
	m_plots[Form("%shPhiChi2_st%d",typeTag.c_str(),st)] = new TH1F(Form("%shPhiChi2_st%d",typeTag.c_str(),st), 
						     Form("Segment #phi #chi^2 st%d;  #chi^2/ndof; entries",st), 
						     100,0.,50.);
	
	m_plots[Form("%shThetaChi2_st%d",typeTag.c_str(),st)] = new TH1F(Form("%shThetaChi2_st%d",typeTag.c_str(),st), 
						       Form("Segment #theta #chi^2 st%d; #chi^2/ndof; entries",st), 
						       100,0.,50.);

	m_effs[Form("%shEffHitVsX_st%d",typeTag.c_str(),st)] = new TEfficiency(Form("%shEffHitVsX_st%d",typeTag.c_str(),st), 
							     Form("Fraction of hits in layer st%d; x (cm); SL / layer",st), 
									       100,-250.,250., 12, 0.5,12.5);



	m_plots[Form("%shResPerSl_st%d",typeTag.c_str(),st)] = new TH1F(Form("%shResPerSl_st%d",typeTag.c_str(),st),
									     Form("Residual per Sl st%d; Sl ; Residual [#mu m]",st),
									     3,0.5,3.5);

      }
    
    
    m_effs[(typeTag+"effPhi_station").c_str()]   = new TEfficiency((typeTag+"effPhi").c_str(),"#phi efficiency per station;station;eff.",4,0.5,4.5);
    m_effs[(typeTag+"effTheta_station").c_str()] = new TEfficiency((typeTag+"effTheta").c_str(),"#theta efficiency per station;station;eff.",4,0.5,4.5);
    
    
    m_plots[(typeTag+"hitResZ").c_str()] = new TH1F((typeTag+"hitResZ").c_str(),
						    "Hit residual z view;|d_hit|-|d_extr|(cm)",
						    250,-1.,1.); 
    
    m_plots[(typeTag+"hitResX").c_str()] = new TH1F((typeTag+"hitResX").c_str(),
						    "Hit residual X view;|d_hit|-|d_extr|(cm)",
						    250,-.8,.8); 
    
    
    m_2Dplots[(typeTag+"hitResX_vsWireDist").c_str()] = new TH2F((typeTag+"hitResX_vsWireDist").c_str(),
								 "Hit residual X view;dist wire;|d_hit|-|d_extr|(cm)",
								 100,0,2.5,
								 200,-.8,.8); 
    
    m_2Dplots[(typeTag+"hitResZ_vsWireDist").c_str()] = new TH2F((typeTag+"hitResZ_vsWireDist").c_str(),
								 "Hit residual Z view;dist wire;|d_hit|-|d_extr|(cm)",
								 100,0,2.5,
								 200,-.8,.8); 
    
    for (int st=1; st<5; st++){
      for (int sl=1; sl<4; sl++){
	
	if(sl==2 && st!=4){
	  m_plots[Form("%shitResZ_st%d",typeTag.c_str(),st)]  = new TH1F(Form("%shitResZ_st%d" ,typeTag.c_str(),st),
									 "Hit residual #theta view;|d_hit|-|d_extr|(cm)",
						       250,-.8,.8); 	
	  
	  
	  m_2Dplots[Form("%shitResZvsWireDist_st%d",typeTag.c_str(),st)]  = new TH2F(Form("%shitResZvsWireDist_st%d",typeTag.c_str(),st),
								   "Hit residual #theta view;|d_wire|[cm];|d_hit|-|d_extr|(cm)",
								     100,0,2.5,
								     250,-.8,.8); 	
	 }
	 
	 else {
	   m_plots[Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl)]  = new TH1F(Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl),
								"Hit residual #phi view;|d_hit|-|d_extr|(cm)",
								250,-.8,.8); 
	   
	   m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d",typeTag.c_str(),st,sl)]  = new TH2F(Form("%shitResXvsWireDist_st%d_sl%d",typeTag.c_str(),st,sl),
									    "Hit residual #phi view;|d_wire|[cm];|d_hit|-|d_extr|(cm)",
									    100,0,2.5,
									    250,-.8,.8);
	 }
	
	for (int l=1; l<5; l++){
	  if(sl==2 && st!=4){
	    m_plots[Form("%shitResZ_st%d_l%d",typeTag.c_str(),st,l)]  = new TH1F(Form("%shitResZ_st%d_l%d" ,typeTag.c_str(),st,l),
							       "Hit residual #theta view;|d_hit|-|d_extr|(cm)",
							       250,-.8,.8); 	

	    
	    m_2Dplots[Form("%shitResZvsWireDist_st%d_l%d",typeTag.c_str(),st,l)]  = new TH2F(Form("%shitResZvsWireDist_st%d_l%d",typeTag.c_str(),st,l),
									   "Hit residual #theta view;|d_wire|[cm];|d_hit|-|d_extr|(cm)",
									   100,0,2.5,
									   250,-.8,.8); 	
	  }
	  
	  else {
	    m_plots[Form("%shitResX_st%d_sl%d_l%d",typeTag.c_str(),st,sl,l)]  = new TH1F(Form("%shitResX_st%d_sl%d_l%d",typeTag.c_str(),st,sl,l),
								       "Hit residual #phi view;|d_hit|-|d_extr|(cm)",
								       250,-.8,.8); 
	    
	    m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d_l%d",typeTag.c_str(),st,sl,l)]  = new TH2F(Form("%shitResXvsWireDist_st%d_sl%d_l%d",typeTag.c_str(),st,sl,l),
										   "Hit residual #phi view;|d_wire|[cm];|d_hit|-|d_extr|(cm)",
										   100,0,2.5,
										   250,-.8,.8);
	  }
	}
      } 
    }
    
    }
     
    
}

void DTNtupleSegmentAnalyzer::fill()
{
  
  
  //Phase1


  int nSegPh1 = 0;
  
  for (uint iSeg = 0; iSeg < seg_nSegments; ++iSeg) 
    {

      if(seg_sector->at(iSeg) != 12 || seg_wheel->at(iSeg) != 2) continue;    

      ++nSegPh1;    

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
	    m_plots[Form("Ph1hT0_st%d",seg_station->at(iSeg))]->Fill(std::max(float(-99.9),(std::min(float(99.9),seg_phi_t0->at(iSeg)))));

	  
	  for (int iSL = 1; iSL <= 3; iSL+=2)
	    {
	      for (int iLayer = 1; iLayer <= 4; ++iLayer)
		{

		  bool hitInLayer = false;
	  
		  for (int iHit = 0; iHit < seg_phi_nHits->at(iSeg); ++iHit)
		    {
		      int layer = getXY<int>(seg_phiHits_layer,iSeg,iHit);	
		      int superLayer = getXY<int>(seg_phiHits_superLayer,iSeg,iHit);	
		  
		      if (layer == iLayer && superLayer == iSL)
			{
			  hitInLayer = true;
			  break;
			}
		    }
		  m_effs[Form("Ph1hEffHitVsX_st%d",seg_station->at(iSeg))]->Fill(hitInLayer, seg_posLoc_x->at(iSeg), iLayer + 4*(iSL - 1));
		}
	    }

	}

      if(seg_hasZed->at(iSeg))
	{

	  nHits += seg_z_nHits->at(iSeg);
	  m_plots[Form("Ph1hThetaNhits_st%d",seg_station->at(iSeg))]->Fill(seg_z_nHits->at(iSeg));

	  m_plots[Form("Ph1hThetaChi2_st%d",seg_station->at(iSeg))]->Fill(seg_z_normChi2->at(iSeg));

	  if(seg_z_nHits->at(iSeg) == 4)
	    m_plots[Form("Ph1hThetaProbChi2_st%d",seg_station->at(iSeg))]->Fill(TMath::Prob(seg_z_normChi2->at(iSeg)*2,2));

	  for (int iLayer = 1; iLayer <= 4; ++iLayer)
	    {

	      bool hitInLayer = false;
	  
	      for (int iHit = 0; iHit < seg_phi_nHits->at(iSeg); ++iHit)
		{
		  int layer = getXY<int>(seg_phiHits_layer,iSeg,iHit);	
		  
		  if (layer == iLayer)
		    {
		      hitInLayer = true;
		      break;
		    }
		}
		  
	      m_effs[Form("Ph1hEffHitVsX_st%d",seg_station->at(iSeg))]->Fill(hitInLayer, seg_posLoc_y->at(iSeg), iLayer + 4);
	      
	    }

	}
      m_plots[Form("Ph1hNhits_st%d",seg_station->at(iSeg))]->Fill(nHits);
    }

  if (nSegPh1)
    {
      m_plots["Ph1hNsegment"] ->Fill(nSegPh1);
    }


  //Phase2


  int nSegPh2 = 0;
  
  for (uint iSeg = 0; iSeg < ph2Seg_nSegments; ++iSeg) 
    {

      if(ph2Seg_sector->at(iSeg) != 12 || ph2Seg_wheel->at(iSeg) != 2) continue;    

      ++nSegPh2;    

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
	    m_plots[Form("Ph2hT0_st%d",ph2Seg_station->at(iSeg))]->Fill(std::max(float(-99.9),(std::min(float(99.9),ph2Seg_phi_t0->at(iSeg)))));

	  
	  for (int iSL = 1; iSL <= 3; iSL+=2)
	    {
	      for (int iLayer = 1; iLayer <= 4; ++iLayer)
		{

		  bool hitInLayer = false;
	  
		  for (int iHit = 0; iHit < ph2Seg_phi_nHits->at(iSeg); ++iHit)
		    {
		      int layer = getXY<int>(ph2Seg_phiHits_layer,iSeg,iHit);	
		      int superLayer = getXY<int>(ph2Seg_phiHits_superLayer,iSeg,iHit);	
		  
		      if (layer == iLayer && superLayer == iSL)
			{
			  hitInLayer = true;
			  break;
			}
		    }
		  m_effs[Form("Ph2hEffHitVsX_st%d",ph2Seg_station->at(iSeg))]->Fill(hitInLayer, ph2Seg_posLoc_x->at(iSeg), iLayer + 4*(iSL - 1));
		}
	    }

	}

      if(ph2Seg_hasZed->at(iSeg))
	{

	  nHits += ph2Seg_z_nHits->at(iSeg);
	  m_plots[Form("Ph2hThetaNhits_st%d",ph2Seg_station->at(iSeg))]->Fill(ph2Seg_z_nHits->at(iSeg));

	  m_plots[Form("Ph2hThetaChi2_st%d",ph2Seg_station->at(iSeg))]->Fill(ph2Seg_z_normChi2->at(iSeg));

	  if(ph2Seg_z_nHits->at(iSeg) == 4)
	    m_plots[Form("Ph2hThetaProbChi2_st%d",ph2Seg_station->at(iSeg))]->Fill(TMath::Prob(ph2Seg_z_normChi2->at(iSeg)*2,2));

	  for (int iLayer = 1; iLayer <= 4; ++iLayer)
	    {

	      bool hitInLayer = false;
	  
	      for (int iHit = 0; iHit < ph2Seg_phi_nHits->at(iSeg); ++iHit)
		{
		  int layer = getXY<int>(ph2Seg_phiHits_layer,iSeg,iHit);	
		  
		  if (layer == iLayer)
		    {
		      hitInLayer = true;
		      break;
		    }
		}
		  
	      m_effs[Form("Ph2hEffHitVsX_st%d",ph2Seg_station->at(iSeg))]->Fill(hitInLayer, ph2Seg_posLoc_y->at(iSeg), iLayer + 4);
	      
	    }

	}
      m_plots[Form("Ph2hNhits_st%d",ph2Seg_station->at(iSeg))]->Fill(nHits);
    }

  if (nSegPh2)
    {
      m_plots["Ph2hNsegment"] ->Fill(nSegPh2);
    }
  }

void DTNtupleSegmentAnalyzer::endJob()
{
  
  
  std::vector<std::string> typeTags = { "Ph1", "Ph2" };
  
  for (const auto typeTag : typeTags){
    for (int st=1; st<5; st++){
      for (int sl=1; sl<4; sl++){
	
	if(sl==2 && st!=4){  
	  
	  cout<<"Fit "<<Form("%shitResZ_st%d",typeTag.c_str(),st)<<endl<<endl;;
	  TF1 *  fTheta = new TF1("fTheta","gaus",m_plots[Form("%shitResZ_st%d",typeTag.c_str(),st)]->GetBinCenter(m_plots[Form("%shitResZ_st%d",typeTag.c_str(),st)]->GetMaximumBin())-1.2*m_plots[Form("%shitResZ_st%d",typeTag.c_str(),st)]->GetStdDev(),m_plots[Form("%shitResZ_st%d",typeTag.c_str(),st)]->GetBinCenter(m_plots[Form("%shitResZ_st%d",typeTag.c_str(),st)]->GetMaximumBin())+1.2*m_plots[Form("%shitResZ_st%d",typeTag.c_str(),st)]->GetStdDev());
	  
	  m_plots[Form("%shitResZ_st%d",typeTag.c_str(),st)]->Fit("fTheta","RM");
	  fTheta->SetRange(fTheta->GetParameter(1)-1.1*fTheta->GetParameter(2),fTheta->GetParameter(1)+1.1*fTheta->GetParameter(2));
	  m_plots[Form("%shitResZ_st%d",typeTag.c_str(),st)]->Fit("fTheta","RM");
	  
	  m_plots[Form("%shResPerSl_st%d",typeTag.c_str(),st)]->SetBinContent(sl+0.5,1e4*fTheta->GetParameter(2)); 	
	}
	else if(sl!=2){
	  
	  cout<<"Fit "<<Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl)<<endl;	  
	  TF1 *  fPhi = new TF1("fPhi","gaus",m_plots[Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl)]->GetBinCenter(m_plots[Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl)]->GetMaximumBin())-1.2*m_plots[Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl)]->GetStdDev(),m_plots[Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl)]->GetBinCenter(m_plots[Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl)]->GetMaximumBin())+1.2*m_plots[Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl)]->GetStdDev());
	  
	  m_plots[Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl)]->Fit("fPhi","RM");
	  fPhi->SetRange(fPhi->GetParameter(1)-1.1*fPhi->GetParameter(2),fPhi->GetParameter(1)+1.1*fPhi->GetParameter(2));
	  m_plots[Form("%shitResX_st%d_sl%d",typeTag.c_str(),st,sl)]->Fit("fPhi","RM");
	  
	  m_plots[Form("%shResPerSl_st%d",typeTag.c_str(),st)]->SetBinContent(sl+0.5,1e4*fPhi->GetParameter(2)); 	
	}
	cout<<"\n\n";
      }
    }
  }  
  
  m_outFile.cd();
  m_outFile.Write();
  m_outFile.Close();
  
}



void DTNtupleSegmentAnalyzer::MeasureEfficiency(string Tag,Long64_t jEntry,
						int Ndead,
						int dead[][6],
						 UInt_t nSegments,
						vector<short> *hasPhi,vector<short> *station,vector<short> *wheel,vector<short> *sector, vector<short> *hasZed,
						vector<short> *phi_nHits,vector<short> *z_nHits,vector<float> *dirLoc_x,
						TClonesArray  *hitsExpWire, TClonesArray  *phiHits_pos,TClonesArray  *phiHits_posCh,TClonesArray  *phiHits_posErr,
						TClonesArray  *phiHits_side,TClonesArray  *phiHits_wire,TClonesArray  *phiHits_wirePos,TClonesArray  *phiHits_layer,
						TClonesArray  *phiHits_superLayer,
						TClonesArray *zHits_pos, TClonesArray  *zHits_posCh,TClonesArray  *zHits_posErr,TClonesArray  *zHits_side,
						TClonesArray  *zHits_wire,TClonesArray  *zHits_wirePos,TClonesArray  *zHits_layer,
						UInt_t Digi_nDigis,vector<short> *Digi_wheel,vector<short> *Digi_sector,vector<short> *Digi_station,
						vector<short> *Digi_superLayer,vector<short> *Digi_layer,vector<short> *Digi_wire,vector<float> *Digi_time){
  
  
  int ChambCross[100][3];
  for (int i=0; i<100; i++) for (int geo=0; geo<3; geo++) ChambCross[i][geo]=-999;
  int NChambCross=0;
  
  // First search for Phi segments
  
  for (uint iseg=0; iseg<nSegments; iseg++) {
    //selection
    if (!hasPhi->at(iseg)) continue;
    // In chambers 1,2,3 select only segments with also Z (theta) contribution.
    if(sector->at(iseg) != 12 || wheel->at(iseg) != 2) continue;     
    
    if (station->at(iseg)!=4 && !hasZed->at(iseg)) continue;
    
    int seg_phinhits = phi_nHits->at(iseg);
    
    if (fabs(dirLoc_x->at(iseg))>0.7) continue; // angle
    
  
    TVectorF *expWire=(TVectorF*)hitsExpWire->At(iseg);
  
    // If a hit is missing, let us check that the extrapolation doesn't fall beyond layer or cross a dead cell!
    int NexpDead=0; bool OutOfLayer=false;
  
    if (seg_phinhits < 8 ) {
      for (int iex=0; iex<12; iex++) {
  
	int expSL = 1;
	int expLay = iex+1;
	//associate layer with right super layer
	if (station->at(iseg) != 4){
	  if (iex > 3 && iex<8) {expSL=2; expLay-=4;}
	  else if (iex>7) {expSL=3; expLay-=8;}
	}
	else {
	  if (iex > 3 && iex<8) continue;
	  else if (iex > 7) {expSL=3; expLay-=8;}
	}
	int nwire=0;
	if (expSL==2){
	  nwire=57; 
	}
	else if (station->at(iseg)==1) nwire = 49;
	else if (station->at(iseg)==2) nwire = 60;
	else if (station->at(iseg)==3) nwire = 72;
	else if (station->at(iseg)==4) nwire = 92;
	    
	Float_t expW = (*expWire)(iex);

	if (expW>nwire) {
	  OutOfLayer=true;
	  break;
	}

	for (int idead=0; idead<Ndead; idead++) {
	  if (dead[idead][0] != station->at(iseg)) continue;
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
    int NHits=0; int missingLayer[3][2]; for (int imi=0; imi<3;imi++) {missingLayer[imi][0]=0;missingLayer[imi][1]=0;}
    int nmissing=0;
       
    TVectorF *hitSuperLayerPhi =(TVectorF*)phiHits_superLayer->At(iseg);
    TVectorF *hitLayerPhi      =(TVectorF*)phiHits_layer->At(iseg);
    TVectorF *hitWirePhi       =(TVectorF*)phiHits_wire->At(iseg);
       
    for (int ilay=1; ilay<9; ilay++) {
      // Search for associated hits
      bool foundh = false;
      for (int kk=0; kk<seg_phinhits; kk++) {
	int sl1  = (*hitSuperLayerPhi)(kk);
	int lay1 = (sl1==1) ? (*hitLayerPhi)(kk) : (*hitLayerPhi)(kk)+4; // hit layer 1-8
	   
	if (lay1==ilay) {
	  NHits++;
	  foundh=true;
	  break;
	}
      }

      if (!foundh) {
	if (nmissing<3) missingLayer[nmissing][0]=ilay;
	nmissing++;
      }
    }
    if (nmissing != 8-NHits) {cout<<NHits<<" hits and "<<nmissing<<" missing!!"<<endl; return;}


    if (NHits<nrequiredhit) continue;
    else if (NHits==8) {
      for (int sl=0; sl<2; sl++) for (int lay=0; lay<4; lay++) {
	  //variables, points, stations, wheels 
	  m_effs[(Tag+"effPhi_station").c_str()]->Fill(1,station->at(iseg)-0.5);    
	}
    }
    else { // let's see how to treat missing layers
      for (int imiss=0; imiss<nmissing; imiss++) {
	int sl  = missingLayer[imiss][0] < 5 ? 0 : 1;
	int lay = sl==0 ? missingLayer[imiss][0]-1 : missingLayer[imiss][0]-5;
	// is there a digi within the expected tube?

	float digiW  = -1.;
	float d      = 1000000; //just a very big number
	int iex      = missingLayer[imiss][0] < 5 ? missingLayer[imiss][0]-1 : missingLayer[imiss][0]+3;
	Float_t expW = (*expWire)(iex);  //perche` float?
	   
	for (uint idigi=0; idigi<Digi_nDigis; idigi++) {
	     
	  if (Digi_time->at(idigi)<320 || Digi_time->at(idigi)>700)  continue; //require only digis time inside time box
	  if (Digi_wheel->at(idigi)   != wheel->at(iseg))   continue;
	  if (Digi_sector->at(idigi)  != sector->at(iseg))  continue;
	  if (Digi_station->at(idigi) != station->at(iseg)) continue;
	     
	  if (Digi_superLayer->at(idigi) == 2)            continue;
	  if (Digi_superLayer->at(idigi) == 1 && sl != 0) continue;
	  if (Digi_superLayer->at(idigi) == 3 && sl != 1) continue;
	  if (Digi_layer->at(idigi) != lay+1)     continue;

	  // let's loop all over the digis and take the closest digis to the extrapolated wire. 
	  // Think about an extra condition on time.

	  if (fabs(expW-Digi_wire->at(idigi))<fabs(d)) {
	    digiW=Digi_wire->at(idigi);
	    d=expW-digiW; 
	  }
	}
	if ( fabs(d)< 1.1) {missingLayer[imiss][1]=1; } //non dovrebbe essere sempre un intero d?
      }
      if (NHits==nrequiredhit) {
	for (int imiss=0; imiss<nmissing; imiss++) {
	  int sl = missingLayer[imiss][0] < 5 ? 0 : 1;
	  int lay = sl==0 ? missingLayer[imiss][0]-1 : missingLayer[imiss][0]-5;
	  m_effs[(Tag+"effPhi_station").c_str()]->Fill(missingLayer[imiss][1],station->at(iseg)-0.5); 
	}
      }
	 
      else {
	for (int sl=0; sl<2; sl++) for (int lay=0; lay<4; lay++) {
	    bool missAss=false; bool missDigi=false;
	    for (int imiss=0; imiss<nmissing; imiss++) {
	      if (missingLayer[imiss][0]==sl*4+lay+1) {
		missAss=true;
		if (!missingLayer[imiss][1]) missDigi=true;
	      }
	    }
	    m_effs[(Tag+"effPhi_station").c_str()]->Fill(!(missAss&&missDigi),station->at(iseg)-0.5);
	  }
      }
    }
  }

  // Then search for Zed segments
     
  for (uint iseg=0; iseg<nSegments; iseg++) {
    if(sector->at(iseg) != 12 || wheel->at(iseg) != 2) continue;     
    //selection
    //if (!hasZed->at(iseg) || !hasPhi->at(iseg)) continue; //check
    if (!hasZed->at(iseg) || phi_nHits->at(iseg)<nrequiredhit) continue; 

    int seg_znhits = z_nHits->at(iseg);
       
    //if (fabs(dirLoc_y->at(iseg))>0.7) continue; // angle WARNING!!! try and disable this for theta layers!
    if (seg_znhits < 3) continue; // piuttosto ovvio!!!  :-)

    TVectorF *expWire=(TVectorF*)hitsExpWire->At(iseg);
    //	expWire->Print();
       
    // If a hit is missing, let us check that the extrapolation doesn't fall out of layer or cross a dead cell!
    int NexpDead=0; bool OutOfLayer=false;

    if (seg_znhits < 4) {
      for (int iex=4; iex<8; iex++) {
	int expSL = 2;
	int expLay = iex-3;		
	   
	Float_t expW = (*expWire)(iex);
	if (expW>58) {
	  OutOfLayer=true;
	  break;
	}
	for (int idead=0; idead<Ndead; idead++) {
	  if (dead[idead][1] != station->at(iseg)) continue;
	  if (dead[idead][2] != expSL) continue;  
	  if (dead[idead][3] != expLay) continue;
	  if (dead[idead][4] != expW) continue; 
	  NexpDead++;
	  break;   
	}
	if (OutOfLayer) break;
	if (NexpDead>MaxDead) break; 
      }
	 
      if (NexpDead>MaxDead) {
	continue;
	// this segment crosses at least 1 dead cell: drop it!
      }
    }
    int NHits=0; int missingLayer=-1;
       
    TVectorF *hitLayerZ = (TVectorF*)zHits_layer->At(iseg);
    TVectorF *hitWireZ  = (TVectorF*)zHits_wire->At(iseg);
       
    for (int ilay=1; ilay<5; ilay++) {
      // Search for associated hits
      bool foundh = false;
      for (int kk=0; kk<seg_znhits; kk++) {
	   
	int lay1 = (*hitLayerZ)(kk);
	   
	if (lay1==ilay) {
	  NHits++;
	  foundh=true;
	  break;
	}
      }
      if (!foundh) missingLayer=ilay;
    }
    if (NHits<3) continue;
    else if (NHits==4) {
	 
      for (int lay=0; lay<4; lay++) {
	m_effs[(Tag+"effTheta_station").c_str()]->Fill(1,station->at(iseg)-0.5); 
      }
    }
    else if (NHits==3) {
      int lay = missingLayer-1;
	 
      // is there a digi within the expected tube?
      float digiW=-1.;
      float d =1000000;
      int iex = missingLayer+3;
      Float_t expW = (*expWire)(iex); 
      for (uint idigi=0; idigi<Digi_nDigis; idigi++) {
	   
	if (Digi_time->at(idigi)<320 || Digi_time->at(idigi)>700)  continue;
	if (Digi_wheel->at(idigi)   != wheel->at(iseg))   continue;
	if (Digi_sector->at(idigi)  != sector->at(iseg))  continue;
	if (Digi_station->at(idigi) != station->at(iseg)) continue;
	   
	if (Digi_superLayer->at(idigi) != 2) continue;
	if (Digi_layer->at(idigi) != lay+1) continue;
           
	if (fabs(expW-Digi_wire->at(idigi))<fabs(d)) {
	  digiW=Digi_wire->at(idigi);
	  d=expW-digiW;
	}
      }
      m_effs[(Tag+"effTheta_station").c_str()]->Fill((fabs(d)< 1.1),station->at(iseg)-0.5); 
    }
    else {
      cout<<" what do you want?? NHits (z) = "<<NHits<<endl;
      return;
    }
  }
}

void DTNtupleSegmentAnalyzer::ComputeResidual(string Tag,
					      Long64_t jEntry, 
					      UInt_t nSegments,
					      vector<short> *hasPhi,
					      vector<short> *station,
					      vector<short> *wheel, 
					      vector<short> *sector,  
					      vector<short> *hasZed,  
					      vector<short> *phi_nHits, 
					      vector<short> *z_nHits,
					      vector<float> *phi_normChi2,
					      vector<float> *z_normChi2,
					      TClonesArray  *hitsExpWire,
					      TClonesArray  *hitsExpPos,
					      TClonesArray  *hitsExpPosCh,				
					      TClonesArray  *phiHits_pos,
					      TClonesArray  *phiHits_posCh,
					      TClonesArray  *phiHits_posErr,
					      TClonesArray  *phiHits_side,
					      TClonesArray  *phiHits_wire,
					      TClonesArray  *phiHits_wirePos,
					      TClonesArray  *phiHits_layer,
					      TClonesArray  *phiHits_superLayer,
					      TClonesArray  *zHits_pos,
					      TClonesArray  *zHits_posCh,
					      TClonesArray  *zHits_posErr,
					      TClonesArray  *zHits_side,
					      TClonesArray  *zHits_wire,
					      TClonesArray  *zHits_wirePos,
					      TClonesArray  *zHits_layer
					      ){

  
  //Add whell and sector selection     
  
  int phiSeg = -1; int zSeg = -1;
  
  for (uint iseg=0; iseg<nSegments; iseg++) {
    if(sector->at(iseg) != 12 || wheel->at(iseg) != 2) continue;     
    int seg_phinhits = phi_nHits->at(iseg);
    int seg_znhits = z_nHits->at(iseg);
    if(hasPhi->at(iseg)) phiSeg++;
    if(hasZed->at(iseg)) zSeg++;

    if (seg_phinhits==8 && ( seg_znhits == 4 || station->at(iseg)==4 )){
      // Select segment with Phi and also Theta if station is different from 4
      if (!hasPhi->at(iseg) ||  (station->at(iseg)!=4 && !hasZed->at(iseg) ) ) continue;

      //cout<<"StationStep2 "<<station->at(iseg)<<" ";	//del
      //Chi2 cut if needed

      if(phi_normChi2->at(phiSeg) > 3.84) continue;
      if(hasZed->at(iseg) && z_normChi2->at(zSeg) > 3.84) continue;

      //Select segment with t0 < 20 to avoid out of time events
      //if(abs(phi_t0->at(phiSeg)) > 15) continue;

      TVectorF *hitSuperLayerPhi =(TVectorF*)phiHits_superLayer->At(iseg);
      TVectorF *hitLayerPhi      =(TVectorF*)phiHits_layer->At(iseg);
      TVectorF *hitWirePhi       =(TVectorF*)phiHits_wire->At(iseg);
      TVectorF *hitPosPhi        =(TVectorF*)phiHits_pos->At(iseg);     
      TVectorF *hitPosPhiCh      =(TVectorF*)phiHits_posCh->At(iseg);	 
      TVectorF *hitPosPhiErr     =(TVectorF*)phiHits_posErr->At(iseg);	 

      TVectorF *hitPhiSide       =(TVectorF*)phiHits_side->At(iseg);	 
      TVectorF *hitZSide         =(TVectorF*)zHits_side->At(iseg);	 

      TVectorF *hitExpPos        =(TVectorF*)hitsExpPos->At(iseg);     
      TVectorF *hitExpPosCh      =(TVectorF*)hitsExpPosCh->At(iseg);     
      TVectorF *hitExpWire       =(TVectorF*)hitsExpWire->At(iseg);     

      TVectorF *hitWirePosPhi    =(TVectorF*)phiHits_wirePos->At(iseg);
      TVectorF *hitWirePosZ      =(TVectorF*)zHits_wirePos->At(iseg);

      TVectorF *hitLayerZ        =(TVectorF*)zHits_layer->At(iseg);
      TVectorF *hitWireZ         =(TVectorF*)zHits_wire->At(iseg);
      TVectorF *hitPosZ          =(TVectorF*)zHits_pos->At(iseg);     
      TVectorF *hitPosZCh        =(TVectorF*)zHits_posCh->At(iseg);	 
      TVectorF *hitPosZErr       =(TVectorF*)zHits_posErr->At(iseg);


      for (int kk=0; kk<seg_phinhits; kk++) {

	if(kk<4){
	  // cout<<"k "<<kk<<" sl "<<(*hitSuperLayerPhi)(kk)<<" l  "<<(*hitLayerPhi)(kk)<<" w "<<(*hitWirePhi)(kk)<<" pos "<<(*hitPosPhi)(kk)<<" exp "<<(*hitExpPos)(kk)<<"  diff "<< (*hitPosPhi)(kk) - (*hitExpPos)(kk)<<endl;
	      
	  m_plots[Form("%shitResX",Tag.c_str())]->Fill(( (*hitPosPhi)(kk) - (*hitExpPos)(kk) )*( (*hitPhiSide)(kk)==2? 1:-1) );
	  m_plots[Form("%shitResX_st%d_sl%d",Tag.c_str(),station->at(iseg),1)]->Fill( ((*hitPosPhi)(kk) - (*hitExpPos)(kk) )*( (*hitPhiSide)(kk)==2? 1:-1));
	  m_plots[Form("%shitResX_st%d_sl%d_l%d",Tag.c_str(),station->at(iseg),1,kk+1)]->Fill( ((*hitPosPhi)(kk) - (*hitExpPos)(kk) )*( (*hitPhiSide)(kk)==2? 1:-1));
	      

	  m_2Dplots[Form("%shitResX_vsWireDist",Tag.c_str())]->Fill(abs((*hitWirePosPhi)(kk)-(*hitPosPhi)(kk)),((*hitPosPhi)(kk) - (*hitExpPos)(kk) )*( (*hitPhiSide)(kk)==2? 1:-1));
	  m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d",Tag.c_str(),station->at(iseg),1)]->Fill(abs((*hitWirePosPhi)(kk)-(*hitPosPhi)(kk)),
											  ((*hitPosPhi)(kk) - (*hitExpPos)(kk))*( (*hitPhiSide)(kk)==2? 1:-1));
	  m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d_l%d",Tag.c_str(),station->at(iseg),1,kk+1)]->Fill(abs((*hitWirePosPhi)(kk)-(*hitPosPhi)(kk)),
												   ((*hitPosPhi)(kk) - (*hitExpPos)(kk))*( (*hitPhiSide)(kk)==2? 1:-1));
	}
	else{
	      
	  m_plots[Form("%shitResX",Tag.c_str())]->Fill( ((*hitPosPhi)(kk) - (*hitExpPos)(kk+4))*( (*hitPhiSide)(kk)==2? 1:-1));
	  m_plots[Form("%shitResX_st%d_sl%d",Tag.c_str(),station->at(iseg),3)]->Fill( ( (*hitPosPhi)(kk) - (*hitExpPos)(kk+4))*( (*hitPhiSide)(kk)==2? 1:-1));
	  m_plots[Form("%shitResX_st%d_sl%d_l%d",Tag.c_str(),station->at(iseg),3,kk-3)]->Fill( ( (*hitPosPhi)(kk) - (*hitExpPos)(kk+4))*( (*hitPhiSide)(kk)==2? 1:-1));	      

	  m_2Dplots[Form("%shitResX_vsWireDist",Tag.c_str())]->Fill(abs((*hitWirePosPhi)(kk)-(*hitPosPhi)(kk)),((*hitPosPhi)(kk) - (*hitExpPos)(kk+4) )*( (*hitPhiSide)(kk)==2? 1:-1));
	  m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d",Tag.c_str(),station->at(iseg),3)]->Fill(abs((*hitWirePosPhi)(kk)-(*hitPosPhi)(kk)), 
											  ((*hitPosPhi)(kk) - (*hitExpPos)(kk+4))*( (*hitPhiSide)(kk)==2? 1:-1));
	  m_2Dplots[Form("%shitResXvsWireDist_st%d_sl%d_l%d",Tag.c_str(),station->at(iseg),3,kk-3)]->Fill(abs((*hitWirePosPhi)(kk)-(*hitPosPhi)(kk)), 
												   ((*hitPosPhi)(kk) - (*hitExpPos)(kk+4))*( (*hitPhiSide)(kk)==2? 1:-1));
	      
	}
      }
 
      for (int kk=0; kk<seg_znhits; kk++) {
	m_plots[Form("%shitResZ",Tag.c_str())]->Fill( ( (*hitPosZ)(kk) - (*hitExpPos)(kk+4) )*( (*hitZSide)(kk)==2? 1:-1));
	m_plots[Form("%shitResZ_st%d",Tag.c_str(),station->at(iseg))]->Fill( ((*hitPosZ)(kk) - (*hitExpPos)(kk+4))*( (*hitZSide)(kk)==2? 1:-1));
	m_plots[Form("%shitResZ_st%d_l%d",Tag.c_str(),station->at(iseg),kk+1)]->Fill( ((*hitPosZ)(kk) - (*hitExpPos)(kk+4))*( (*hitZSide)(kk)==2? 1:-1));	    

	m_2Dplots[Form("%shitResZ_vsWireDist",Tag.c_str())]->Fill(abs((*hitWirePosZ)(kk)-(*hitPosZ)(kk)),((*hitPosZ)(kk) - (*hitExpPos)(kk+4) )*( (*hitZSide)(kk)==2? 1:-1));
	m_2Dplots[Form("%shitResZvsWireDist_st%d",Tag.c_str(),station->at(iseg))]->Fill(abs((*hitWirePosZ)(kk)-(*hitPosZ)(kk)),((*hitPosZ)(kk) - (*hitExpPos)(kk+4))*( (*hitZSide)(kk)==2? 1:-1));
	m_2Dplots[Form("%shitResZvsWireDist_st%d_l%d",Tag.c_str(),station->at(iseg),kk+1)]->Fill(abs((*hitWirePosZ)(kk)-(*hitPosZ)(kk)),((*hitPosZ)(kk) - (*hitExpPos)(kk+4))*( (*hitZSide)(kk)==2? 1:-1));
      }
    }
  }
}
