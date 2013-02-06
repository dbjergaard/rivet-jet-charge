//System Includes
#include <iostream>
#include <map>
#include <string>
#include <boost/foreach.hpp>

// ROOT includes
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"

// Pre-processor defines
#define foreach BOOST_FOREACH

//Namespace slurps
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::map;
typedef std::map<std::string,std::string> NameTitleMap;

TH1F* mapTGraphAsymmErrorToTH1F(const TGraphAsymmErrors* aidaHist/*, TH1F* rootHist*/) {
  TH1F* rootHist=nullptr;
  if(aidaHist){
    rootHist = aidaHist->GetHistogram();
    rootHist->SetName(aidaHist->GetName());
    rootHist->SetTitle(aidaHist->GetTitle());
    Int_t N=aidaHist->GetN();
    Double_t* x=aidaHist->GetX(); 
    //Double_t* ex_hi=aidaHist->GetEXhigh();
    //Double_t* ex_lo=aidaHist->GetEXlow();
    Double_t* y=aidaHist->GetY(); 
    Double_t* ey_hi=aidaHist->GetEYhigh();
    Double_t* ey_lo=aidaHist->GetEYlow();
    
    if(N!=rootHist->GetNbinsX()){
      rootHist->Rebin(rootHist->GetNbinsX()/N);//DANGER WILL ROBINSON
    }
    for(Int_t i=0; i <N; ++i){
      rootHist->Fill(x[i],y[i]);
      //rootHist->SetBinContent(i,y[i]);
      //cout<<"ex_hi: "<<ex_hi[i]<<" ex_lo: "<<ex_lo[i]<<endl;
      //cout<<"ey_hi: "<<ey_hi[i]<<" ey_lo: "<<ey_lo[i]<<endl;
      //Over estimate error by taking max of hi or lo error on y
      rootHist->SetBinError(i,(ey_hi[i] > ey_lo[i]) ? ey_hi[i]:ey_lo[i]);
    }
  }
  return rootHist;
}
int main(/*int argc,const char* argv[]*/) {
  NameTitleMap canonPlots;
  string y_axis_title="#int f(x) dx #equiv 1";
  canonPlots["JetMult"]="Jet Multiplicity;n;"+y_axis_title;
  canonPlots["JetPt"]="Jet p_T;GeV/c;"+y_axis_title;
  canonPlots["JetEta"]="#eta distribution of jets;#eta;"+y_axis_title;
  canonPlots["WJetCharge"]="Jet charge #times W charge  (Q_j Q_W);e^2;"+y_axis_title;
  canonPlots["JetPullMag"]="Jet Pull Magnitude |t|;|t|;"+y_axis_title;
  canonPlots["JetPullTheta"]="Jet Pull #theta_t;#theta_t;"+y_axis_title;
  canonPlots["TruthPdgID"]="PdgID of truth quark;n;"+y_axis_title;
  canonPlots["TruthDeltaR"]="#Delta R of truth parton to jet;sterad;"+y_axis_title;
  map<string,string> ratioPlots;
  ratioPlots["ChargeSignPurity"]="Right-sign charge ratio";
  ratioPlots["QuarkJetPt"]="Quark purity ratio";
  
  vector<string> genNames={"Pythia8.4C",
    "Pythia8.4C.MSTW2008",
    "Pythia6.MSTWLO",
    "Pythia6.CTEQ6L1",
    "Sherpa",
    "Herwig++"};
  NameTitleMap genTitles;
  genTitles[genNames[0]]="Pythia 8, 4C";
  genTitles[genNames[1]]="Pythia 8, 4C, MSTW2008";
  genTitles[genNames[2]]="Pythia 6, MSTWLO";
  genTitles[genNames[3]]="Pythia 6, CTEQ6L1";
  genTitles[genNames[4]]="Sherpa";
  genTitles[genNames[5]]="Herwig++";
  map<string,int> genColors;
  map<string,TFile*> genRootfiles;
  foreach(string& gen, genNames){
    genRootfiles[gen]=new TFile(("rootfiles/"+gen+".root").c_str());
    if(genRootfiles[gen]==nullptr){
      genRootfiles.erase(gen);
    }
  }
  foreach(NameTitleMap::value_type plot, canonPlots){
    //map<string,TGraphAsymmErrors*> histos;
    TCanvas c(plot.first.c_str(), plot.second.c_str(),600,600);
    c.Draw();
    foreach(string& gen, genNames) {
      TH1F* histo=mapTGraphAsymmErrorToTH1F(static_cast<TGraphAsymmErrors*>(genRootfiles[gen]->Get(plot.first.c_str())));
      histo->Draw("same");
    }
    c.SaveAs((plot.first+".png").c_str());
  }
  return 0;
}
