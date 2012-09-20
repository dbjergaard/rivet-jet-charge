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
      //std::vector<std::pair<double, double > > electronEtaRanges;
      std::vector<std::pair<double, double > > muonEtaRanges;
      //electronEtaRanges.push_back(make_pair(-2.47,-1.37));
      //electronEtaRanges.push_back(make_pair(1.52,2.47));
      muonEtaRanges.push_back(make_pair(-2.4,2.4));

      WFinder muWFinder(fs, muonEtaRanges, 20*GeV, MUON, 40*GeV,1000*GeV,25*GeV,0.4,true,false,80.4,true);


      addProjection(muWFinder,"muWFinder");

      FastJets JetProjection(fs,FastJets::KT,0.7); //muWFinder.remainingFinalState()
      addProjection(JetProjection,"Jets");

      // @todo Implement the following histos properly
      // 1. Invariant mass of 2/3-jet system
      // 2. Inclusive jet spectrum
      // 3. \DeltaR between subjets
      // 4. jet mass
      // 5. Sum ET of the subjets

      // Tag a W in the event, focus on jets that don't come from the W decay.

      // Histograms

      _histJetMult	= bookHistogram1D("Mult", 100, -0.5, 199.5);
      //Jet Kinematics
      _histJetPt	= bookHistogram1D("JetPt"	, 300, 0, 30);
      _histJetE		= bookHistogram1D("JetE"	, 100, 0, 200);
      _histJetEta	= bookHistogram1D("JetEta"	, 50, -5, 5);
      _histJetRapidity	= bookHistogram1D("JetRapidity"	, 50, -5, 5);
      _histJetPhi	= bookHistogram1D("JetPhi"	, 50, 0, TWOPI);
      // Jet theoreticals
      /*
      _histJet2Mass	= bookHistogram1D("Jet2Mass"	, 200, 0, 100);
      _histJet3Mass	= bookHistogram1D("Jet3Mass"	, 200, 0, 100);
      _histSubJetDeltaR	= bookHistogram1D("Jet3Mass"	, 200, 0, 3.0);
      _histSubJetMass	= bookHistogram1D("SubJetMass"	, 200, 0, 100);
      _histSubJetSumEt	= bookHistogram1D("SubJetSumEt", 200, 0, 100);
      */
    }
    /// Perform the per-event analysis
    void analyze(const Event& event) 
    {
      /*
      const WFinder& muWFinder = applyProjection<WFinder>(event,"muWFinder");
      if (muWFinder.bosons().size() != 1)
	vetoEvent;
      */
      const double weight = event.weight();
      const FastJets& JetProjection = applyProjection<FastJets>(event, "Jets");
      const Jets& jets = JetProjection.jetsByPt(20.0*GeV);

      //const FinalState& fs = applyProjection<FinalState>(event, "FS");      
      /*
      _histMult->fill(fs.size(),weight);
      foreach(const Particle& p, fs.particles())
	{
	  _histEta->fill(p.momentum().eta(),weight);
	  _histPt->fill(p.momentum().pT(),weight);
	}
      */
      if (jets.size() > 0) 
	{
	  
	  //foreach(Jets::const_iterator jet,jets)
	  unsigned int jetMult=jets.size();
	  //std::cout<<"Jet Mult: "<<jetMult<<endl;
	  _histJetMult->fill(jetMult);
	  foreach(const Jet& jet, jets)
	    {
	      const FourMomentum& jetP=jet.momentum();
	      /*
	      printf("pT: %'.3f eta: %'.3f rapidity: %'.3f phi: %'.3f \n",
		     jetP.pT(),jetP.eta(),jetP.rapidity(), jetP.phi());
	      */

	      _histJetPt->fill(jetP.pT(),weight);	
	      _histJetE->fill(jetP.E(),weight);
	      _histJetEta->fill(jetP.eta(),weight);	
	      _histJetRapidity->fill(jetP.rapidity(),weight); 
	      _histJetPhi->fill(jetP.phi(),weight);	

	    }
	  /*
	  for(unsigned int i=0; i < jetMult; ++i)//figure out the foreach macro for jets
	    {
	    }
	  */
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

      // Jet Kinematics
      /*
      scale( _histMult ,1/sumOfWeights());
      scale( _histPt ,1/sumOfWeights());
      scale( _histEta ,1/sumOfWeights());
      */

      scale( _histJetMult ,1/sumOfWeights());
      scale( _histJetPt ,1/sumOfWeights());
      scale( _histJetE ,1/sumOfWeights());
      scale( _histJetEta ,1/sumOfWeights());
      scale( _histJetRapidity ,1/sumOfWeights());
      scale( _histJetPhi ,1/sumOfWeights());

      // Jet Theoreticals 	
      /*
      scale( _histJet2Mass ,1/sumOfWeights());
      scale( _histJet3Mass ,1/sumOfWeights());
      scale( _histSubJetDeltaR ,1/sumOfWeights());
      scale( _histSubJetMass ,1/sumOfWeights());
      scale( _histSubJetSumEt ,1/sumOfWeights());
      */
    }
    //@}
  private:

    ///@param Histograms Indexed by histogram name for easy management
    ///until Rivet Autobooking takes over, allows any number of
    ///histograms to be added "on the fly" in the init() method.
    //@{
    AIDA::IHistogram1D *_histJetMult;

    //Jet Kinematics	
    AIDA::IHistogram1D *_histJetPt;
    AIDA::IHistogram1D *_histJetE;
    AIDA::IHistogram1D *_histJetEta;
    AIDA::IHistogram1D *_histJetRapidity;
    AIDA::IHistogram1D *_histJetPhi;
    // Jet theoretical
    /*
    AIDA::IHistogram1D *_histJet2Mass;
    AIDA::IHistogram1D *_histJet3Mass;
    AIDA::IHistogram1D *_histSubJetDeltaR;
    AIDA::IHistogram1D *_histSubJetMass;
    AIDA::IHistogram1D *_histSubJetSumEt;
    */
    //@}

  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_GENSTUDY_JETCHARGE);

}
