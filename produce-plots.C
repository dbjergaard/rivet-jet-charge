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
using std::endl;
using std::string;
using std::vector;
using std::map;
typedef std::map<std::string,std::string> NameTitleMap;
static void print_histo(TH1* plot, string outname) {
  TCanvas genericCanvas;
  //plot->Sumw2();
  plot->Draw("E1P0");
  outname = outname + ".eps";
  genericCanvas.SaveAs(outname.c_str());
  outname.replace(outname.size()-3,3,"gif");
  genericCanvas.SaveAs(outname.c_str());
}
void usage(const char* name) {
  printf("Usage: %s [rootfiles]\n",name);
}
void mapTGraphAsymmErrorToTH1F(const TGraphAsymmErrors* aidaHist, TH1F* rootHist)  {
  rootHist = aidaHist->GetHistogram();
}
int main(int argc,const char* argv[]) {
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
    foreach(string& gen, genNames) {
      TH1F* histo=nullptr;
      //TGraphAsymmErrors* graph = static_cast<TGraphAsymmErrors*>(genRootfiles[gen]->Get(plot.first.c_str()));
      cout<<"before histo: "<<histo;
      mapTGraphAsymmErrorToTH1F(static_cast<TGraphAsymmErrors*>(genRootfiles[gen]->Get(plot.first.c_str())),histo);
      cout<<"after histo: "<<histo<<endl;
      
      }
  }
  
  return 0;
}
