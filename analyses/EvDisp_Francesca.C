#define EvDisp_cxx
#include "EvDisp.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void EvDisp::Loop()
{
//   In a ROOT session, you can do:
//      root> .L EvDisp.C
//      root> EvDisp t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   char go;

   TH2F* display1 = new TH2F ("display1","digi display lagacy",60,0.5,60.5,12,0.5,12.5);
   TH2F* display2 = new TH2F ("display2","digi display Phase2",60,0.5,60.5,12,0.5,12.5);

   TH2F* timecomp = new TH2F ("timecomp","ph2 time vs legacy",500,2200,3200,500,82000,83000);

   TCanvas* c1 = new TCanvas();
   c1->Divide(2,1);
   gStyle->SetOptStat(0);
   gStyle->SetPalette(1);
   int ipad=0;

   int start=0;
   cout<<" start from? "<<endl; 
   cin>>start;

   int stop=nentries;
   cout<<" stop at? "<<endl; 
   cin>>stop;

   for (Long64_t jentry=start; jentry<stop;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if (jentry%1000 == 0) cout<<"entry "<<jentry<<endl;

      display1->Reset();
      display2->Reset();

      for (int idigi=0; idigi<digi_nDigis; idigi++) {
        float x=digi_wire->at(idigi);
        float y=digi_layer->at(idigi) + 4*(digi_superLayer->at(idigi)-1);
        display1->Fill(x,y);

        for (int idigi2=0; idigi2<ph2Digi_nDigis; idigi2++) {
          if (ph2Digi_superLayer->at(idigi2)==digi_superLayer->at(idigi) &&
              ph2Digi_layer->at(idigi2)==digi_layer->at(idigi) &&
              ph2Digi_wire->at(idigi2)==digi_wire->at(idigi) )
              timecomp->Fill(digi_time->at(idigi),ph2Digi_time->at(idigi2));
            }
      }
      for (int idigi=0; idigi<ph2Digi_nDigis; idigi++) {
        float x=ph2Digi_wire->at(idigi);
        float y=ph2Digi_layer->at(idigi) + 4*(ph2Digi_superLayer->at(idigi)-1);
        display2->Fill(x,y);
      }
      
      c1->cd(1);
      display1->Draw("colz");

      c1->cd(2);
      display2->Draw("colz");

      c1->Update();
      cout<<" event "<<jentry<<" "<<event_eventNumber<<" "<<digi_nDigis<<" "<<ph2Digi_nDigis<<" 0 to stop"<<endl;
      cin>>go;
      if (go == '0') return;
      
   }
}
