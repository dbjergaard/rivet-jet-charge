// -*- C++ -*-
//System includes
#include <map>

//Rivet framework
#include "Rivet/Analysis.hh"
#include "Rivet/RivetAIDA.hh"
#include "Rivet/Tools/Logging.hh"

//Projections
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/WFinder.hh"
#include "Rivet/Projections/FastJets.hh"
//#include "Rivet/Projections/MissingMomentum.hh"

// Histogram booking
#include "LWH/Histogram1D.h"

namespace Rivet {
  typedef std::map <std::string,AIDA::IHistogram1D*> BookedHistos;

  /// Generic analysis looking at various distributions of final state particles
  class MC_GENSTUDY_JETCHARGE : public Analysis {
  public:

    /// Constructor
    MC_GENSTUDY_JETCHARGE()
      : Analysis("MC_GENSTUDY_JETCHARGE")
    {    }


  public:

    /// @name Analysis methods
    //@{

    /// Book histograms and initialise projections before the run
    void init() {

      // Projections
      const FinalState fs;
      addProjection(fs, "FS");
      std::vector<std::pair<double, double > > electronEtaRanges;
      std::vector<std::pair<double, double > > muonEtaRanges;
      electronEtaRanges.push_back(make_pair(-2.47,-1.37));
      electronEtaRanges.push_back(make_pair(1.52,2.47));
      muonEtaRanges.push_back(make_pair(-2.4,2.4));

      WFinder muWFinder(fs, muonEtaRanges, 20*GeV, MUON, 40*GeV,1000*GeV,25*GeV,0.4,true,false,80.4,true);


      addProjection(muWFinder,"muWFinder");

      FastJets JetProjection(muWFinder.remainingFinalState(),FastJets::KT,0.7);
      addProjection(JetProjection,"Jets");

      // @todo Implement the following histos properly
      // 1. Invariant mass of 2/3-jet system
      // 2. Inclusive jet spectrum
      // 3. \DeltaR between subjets
      // 4. jet mass
      // 5. Sum ET of the subjets

      // Tag a W in the event, focus on jets that don't come from the W decay.

      // Histograms

      // Jet Kinematics
      Histograms["JetPt"]	= bookHistogram1D("JetPt"	, 300, 0, 30);
      Histograms["JetE"]	= bookHistogram1D("JetE"	, 100, 0, 200);
      Histograms["JetEta"]	= bookHistogram1D("JetEta"	, 50, -5, 5);
      Histograms["JetRapidity"] = bookHistogram1D("JetRapidity"	, 50, -5, 5);
      Histograms["JetPhi"]	= bookHistogram1D("JetPhi"	, 50, 0, TWOPI);
      // Jet Theoreticals 
      Histograms["Jet2Mass"]     = bookHistogram1D("Jet2Mass"	, 200, 0, 100);
      Histograms["Jet3Mass"]     = bookHistogram1D("Jet3Mass"	, 200, 0, 100);
      Histograms["SubJetDeltaR"] = bookHistogram1D("Jet3Mass"	, 200, 0, 3.0);
      Histograms["SubJetMass"]   = bookHistogram1D("SubJetMass"	, 200, 0, 100);
      Histograms["SubJetSumEt"]	 = bookHistogram1D("SubJetSumEt", 200, 0, 100);

    }
    /// Perform the per-event analysis
    void analyze(const Event& event) 
    {

      const WFinder& muWFinder = applyProjection<WFinder>(event,"muWFinder");
      if (muWFinder.bosons().size() != 1)
	vetoEvent;

      const double weight = event.weight();
      const FastJets& JetProjection = applyProjection<FastJets>(event, "Jets");
      const Jets& jets = JetProjection.jetsByPt(20.0*GeV);

      if (jets.size() > 0) 
	{
	  //foreach(Jets::const_iterator jet,jets)
	  unsigned int jetMult=jets.size();
	  std::cout<<"Jet Mult: "<<jetMult<<endl;
	  
	  for(unsigned int i=0; i < jetMult; ++i)//figure out the foreach macro for jets
	    {
	      Histograms["JetPt"]->fill(jets[i].momentum().pT(),weight);	
	      Histograms["JetE"]->fill(jets[i].momentum().E(),weight);
	      Histograms["JetEta"]->fill(jets[i].eta(),weight);	
	      //Histograms["JetRapidity"]->fill(jet.y(),weight); 
	      Histograms["JetPhi"]->fill(jets[i].phi(),weight);	
	    }
	}
      else
	vetoEvent;

    }
    /// Finalize
    void finalize() 
    {

      /*
      foreach(BookedHistos::value_type H,Histograms)
	scale(H.second,1/sumOfWeights());
      */
      // for some reason looping is broken, scale histograms manually =/
      // Jet Kinematics

      scale( Histograms["JetPt"] ,1/sumOfWeights());
      scale( Histograms["JetE"] ,1/sumOfWeights());
      scale( Histograms["JetEta"] ,1/sumOfWeights());
      //scale( Histograms["JetRapidity"] ,1/sumOfWeights());
      scale( Histograms["JetPhi"] ,1/sumOfWeights());
      // Jet Theoreticals 	
      /*
      scale( Histograms["Jet2Mass"] ,1/sumOfWeights());
      scale( Histograms["Jet3Mass"] ,1/sumOfWeights());
      scale( Histograms["SubJetDeltaR"] ,1/sumOfWeights());
      scale( Histograms["SubJetMass"] ,1/sumOfWeights());
      scale( Histograms["SubJetSumEt"] ,1/sumOfWeights());
      */
    }
    //@}
private:

    ///@param Histograms Indexed by histogram name for easy management
    ///until Rivet Autobooking takes over, allows any number of
    ///histograms to be added "on the fly" in the init() method.
    //@{
    BookedHistos Histograms;
    //@}

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_GENSTUDY_JETCHARGE);

}
