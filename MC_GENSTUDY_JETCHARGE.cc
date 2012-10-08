// -*- C++ -*-
//System includes
#include <map>

//Rivet framework
#include "Rivet/Analysis.hh"
#include "Rivet/RivetAIDA.hh"
#include "Rivet/Tools/Logging.hh"
#include "Rivet/Tools/ParticleIdUtils.hh"

//Projections
#include "Rivet/Projections/FinalState.hh"
#include "Rivet/Projections/WFinder.hh"
#include "Rivet/Projections/FastJets.hh"
//#include "Rivet/Projections/MissingMomentum.hh"

// Histogram booking
#include "LWH/Histogram1D.h"

namespace Rivet {

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
      std::vector<std::pair<double, double > > muonEtaRanges;
      muonEtaRanges.push_back(make_pair(-2.4,2.4));
      WFinder muWFinder(fs, muonEtaRanges, 20*GeV, MUON, 
			40*GeV,1000*GeV,25*GeV,0.4,true,false,80.4,true);


      // Tag a W in the event, focus on jets that don't come from the W decay.
      addProjection(muWFinder,"muWFinder");

      FastJets JetProjection(muWFinder.remainingFinalState(),FastJets::KT,0.7); //
      addProjection(JetProjection,"Jets");

      // @todo Implement the following histos properly
      // 1. Invariant mass of 2/3-jet system
      // 2. Inclusive jet spectrum


      // Histograms
      _histJetMult	= bookHistogram1D("JetMult", 7, -0.5, 6.5);
      //Jet Kinematics
      _histJetPt	= bookHistogram1D("JetPt"	, 100, 20, 50);
      _histJetE		= bookHistogram1D("JetE"	, 100, 0, 200);
      _histJetEta	= bookHistogram1D("JetEta"	, 50, -5, 5);
      _histJetRapidity	= bookHistogram1D("JetRapidity"	, 50, -5, 5);
      //_histJetPhi	= bookHistogram1D("JetPhi"	, 50, 0, TWOPI);
      _histJetMass      = bookHistogram1D("JetMass"	, 200, 0, 40);
      // Jet theoreticals
      //_histJet2Mass	= bookHistogram1D("Jet2Mass"	, 200, 0, 100);
      //_histJet3Mass	= bookHistogram1D("Jet3Mass"	, 200, 0, 100);
			
      _histSubJetMult	= bookHistogram1D("SubJetMult", 7, -0.5, 6.5);
      _histSubJet2Mass	= bookHistogram1D("SubJet2Mass"	, 200, 0, 25);
      _histSubJet3Mass	= bookHistogram1D("SubJet3Mass"	, 200, 0, 25);
      _histSubJetDeltaR	= bookHistogram1D("SubJetDeltaR", 200, 0, 3.0);
      _histSubJetMass	= bookHistogram1D("SubJetMass"	, 200, 0, 10);
      _histSubJetSumEt	= bookHistogram1D("SubJetSumEt", 200, 0, 100);

      //_histJetDipolarity= bookHistogram1D("JetDipolarity", 50, -2, 2);
      _histWJetCharge    =  bookHistogram1D("WJetCharge", 50, -0.5, 0.5);
      _histWCharge    =  bookHistogram1D("WCharge", 3, -1.5, 1.5);
    }
    virtual void analyzeSubJets(const fastjet::PseudoJet& jet,const double weight)
    {
      //check dcut==0.4 is appropriate
      double dcut=0.4;
      double sumEt=0.0;

      PseudoJets subJets=jet.validated_cs()->exclusive_subjets(jet,dcut);
      unsigned int nSubJets=jet.validated_cs()->n_exclusive_subjets(jet,dcut);
      _histSubJetMult->fill(nSubJets,weight);
      for(unsigned int j=0;j!=nSubJets;++j)
	{
	  sumEt+=subJets.at(j).Et();
	  _histSubJetMass->fill(subJets.at(j).m());
	  for(unsigned int k=(j+1); k!=nSubJets;++k)
	    {
	      _histSubJetDeltaR->fill(subJets.at(j).delta_R(subJets.at(k)),weight);
	      _histSubJet2Mass->fill((subJets.at(j)+subJets.at(k)).m(),weight);
	      for(unsigned int l=(k+1); l!=nSubJets; ++l)
		_histSubJet3Mass->fill((subJets.at(j)+subJets.at(k)+subJets.at(l)).m(),weight);
	    }
	}
      _histSubJetSumEt->fill(sumEt,weight);
    }
    /// Perform the per-event analysis

    void analyze(const Event& event) 
    {

      const WFinder& muWFinder = applyProjection<WFinder>(event,"muWFinder");
      if (muWFinder.bosons().size() != 1)
      {
	vetoEvent;
      }

      const double weight = event.weight();
      const FastJets& JetProjection = applyProjection<FastJets>(event, "Jets");
      const PseudoJets& jets = JetProjection.pseudoJetsByPt(20.0*GeV);

      if (jets.size() > 0) 
	{
	  unsigned int jetMult=jets.size();
	  _histJetMult->fill(jetMult);
	  //foreach(const fastjet::PseudoJet &jet, jets)

	  // rather than loop over all jets, just take the first hard one 
	  //for(PseudoJets::const_iterator jet=jets.begin();jet!=jets.end();++jet)
	  //{
	  //const FourMomentum& jetP=Rivet::momentum(jet);
	  
	  if(jets.front().eta() > -2.5 && jets.front().eta() < 2.5)
	    {
	      if(jets.front().has_valid_cs())
		analyzeSubJets(jets.front(),weight);

	      _histJetMass->fill(jets.front().m(),weight);
	      _histJetPt->fill(jets.front().pt(),weight);	
	      _histJetE->fill(jets.front().E(),weight);
	      _histJetEta->fill(jets.front().eta(),weight);	
	      _histJetRapidity->fill(jets.front().rapidity(),weight); 
	      //histJetPhi->fill(jets.front().phi(),weight);	
	      double wCharge=PID::charge(muWFinder.bosons().front().pdgId())+0.0;//dirty cast 
	  
	      _histWJetCharge->fill(wCharge*JetProjection.JetCharge(jets.front(),0.5,1*GeV),weight);
	      _histWCharge->fill(wCharge,weight);
	      //_histJetDipolarity->fill(JetProjection.Dipolarity(jets.front(),0.4),weight);
	    }	      
	  /*
	    for(PseudoJets::const_iterator jet2=jet; jet2!=jets.end(); jet2++)
	    {
	    _histJet2Mass->fill((*jet+*jet2).m(),weight);
	    for(PseudoJets::const_iterator jet3=jet2; jet3!=jets.end(); jet3++)
	    _histJet3Mass->fill((*jet+*jet2+*jet3).m(),weight);
	    }
	  */
	  //}
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
      scale( _histJetMult ,1/sumOfWeights());
      scale( _histJetPt ,1/sumOfWeights());
      scale( _histJetE ,1/sumOfWeights());
      scale( _histJetEta ,1/sumOfWeights());
      scale( _histJetRapidity ,1/sumOfWeights());
      //scale( _histJetPhi ,1/sumOfWeights());
      scale( _histJetMass ,1/sumOfWeights());

      // Jet Theoreticals 	
      //scale( _histJet2Mass ,1/sumOfWeights());
      //scale( _histJet3Mass ,1/sumOfWeights());

      scale( _histSubJet2Mass ,1/sumOfWeights());
      scale( _histSubJet3Mass ,1/sumOfWeights());
      scale( _histSubJetDeltaR ,1/sumOfWeights());
      scale( _histSubJetMass ,1/sumOfWeights());
      scale( _histSubJetSumEt ,1/sumOfWeights());
      //scale(_histJetDipolarity,1/sumOfWeights());
      scale(_histWJetCharge ,1/sumOfWeights());
      scale(_histWCharge ,1/sumOfWeights());
      scale(_histSubJetMult,1/sumOfWeights());
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
    //AIDA::IHistogram1D *_histJetPhi;
    AIDA::IHistogram1D *_histJetMass;

    // Jet theoreticals
    //AIDA::IHistogram1D *_histJet2Mass;
    //AIDA::IHistogram1D *_histJet3Mass;

    AIDA::IHistogram1D *_histSubJet2Mass;
    AIDA::IHistogram1D *_histSubJet3Mass;
    AIDA::IHistogram1D *_histSubJetDeltaR;
    AIDA::IHistogram1D *_histSubJetMass;
    AIDA::IHistogram1D *_histSubJetSumEt;

    AIDA::IHistogram1D *_histJetDipolarity;
    AIDA::IHistogram1D *_histWJetCharge;    
    AIDA::IHistogram1D *_histWCharge;
    AIDA::IHistogram1D *_histSubJetMult;
    //@}
			


  };


  // The hook for the plugin system
  DECLARE_RIVET_PLUGIN(MC_GENSTUDY_JETCHARGE);

}
