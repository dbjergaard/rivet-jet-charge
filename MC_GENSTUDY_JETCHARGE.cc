// -*- C++ -*-
//System includes
//#include <map>
#include <algorithm>

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
      WFinder muWFinder(fs, muonEtaRanges, 25*GeV, MUON, 
			40*GeV,1000*GeV,25*GeV,0.6,true,false,80.4,true);


      // Tag a W in the event, focus on jets that don't come from the W decay.
      addProjection(muWFinder,"muWFinder");

      FastJets JetProjection(muWFinder.remainingFinalState(),FastJets::ANTIKT, 0.6); //FastJets::KT,0.7
      addProjection(JetProjection,"Jets");

      // Histograms

      _histJetMult	= bookHistogram1D("JetMult", 6, -0.5, 5.5);
      //Jet Kinematics
      _histJetPt	= bookHistogram1D("JetPt"	, 50, 24, 70);
      _histJetE		= bookHistogram1D("JetE"	, 25, 20, 200);
      _histJetEta	= bookHistogram1D("JetEta"	, 25, -2, 2);
      _histJetRapidity	= bookHistogram1D("JetRapidity"	, 25, -2, 2);
      //_histJetPhi	= bookHistogram1D("JetPhi"	, 25, 0, TWOPI);
      _histJetMass      = bookHistogram1D("JetMass"	, 100, 0, 40);
      // Jet theoreticals
      //_histJet2Mass	= bookHistogram1D("Jet2Mass"	, 100, 0, 100);
      //_histJet3Mass	= bookHistogram1D("Jet3Mass"	, 100, 0, 100);
			
      _histSubJetMult	= bookHistogram1D("SubJetMult", 15, -0.5, 29.5);
      _histSubJetSkew   = bookHistogram1D("SubJetSkew", 25, 0, 0.1); 
      _histSubJetStddev = bookHistogram1D("SubJetStddev", 25, 0, 0.2); 

      _histSubJet2Mass	= bookHistogram1D("SubJet2Mass"	, 100, 0, 35);
      _histSubJet3Mass	= bookHistogram1D("SubJet3Mass"	, 100, 0, 45);
      _histSubJetDeltaR	= bookHistogram1D("SubJetDeltaR", 50, 0, 1.0);
      _histSubJetMass	= bookHistogram1D("SubJetMass"	, 100, 0, 12);
      _histSubJetSumEt	= bookHistogram1D("SubJetSumEt", 20, 0, 175);

      //_histJetDipolarity= bookHistogram1D("JetDipolarity", 50, -2, 2);
      _histWJetCharge    =  bookHistogram1D("WJetCharge", 50, -0.3, 0.3);
      _histWCharge    =  bookHistogram1D("WCharge", 3, -1.5, 1.5);
    }
    /// quickly calculate standard deviation of pt distribution in jets
    virtual void pt_stddev(const PseudoJets& jets, double& mean,double& stddev,const double N)
    {
      foreach(const fastjet::PseudoJet& jet, jets)
	  mean+=jet.pt();
      mean=mean/N;
      
      foreach(const fastjet::PseudoJet& jet, jets)
	  stddev+=((jet.pt()-mean)*(jet.pt()-mean));
      stddev=stddev/N;
    }

    virtual void analyzeSubJets(const fastjet::PseudoJet& jet,const double weight)
    {
      //double dcut=0.4;
      double ptstddev	= 0.0;
      double ptmean	= 0.0;
      double ptskew	= 0.0;


      double ptmin=0.5*GeV;
      double sumEt=0.0;
      fastjet::ClusterSequence clusterSeq(jet.validated_cs()->constituents(jet),fastjet::JetDefinition(fastjet::kt_algorithm,0.6)); 

      PseudoJets subJets=clusterSeq.exclusive_jets_up_to(3);
      /*
      PseudoJets testJet=clusterSeq.exclusive_jets_up_to(1);
      printf("Original jet: eta %f, phi %f, pt %f, E %f\n",
	     jet.eta(),jet.phi(),jet.pt(),jet.E());
      printf("     New Jet: eta %f, phi %f, pt %f, E %f\n",
	     testJet[0].eta(),testJet[0].phi(),testJet[0].pt(),testJet[0].E());
      */
 
     fastjet::ClusterSequence antiKTClusterSeq(jet.validated_cs()->constituents(jet),fastjet::JetDefinition(fastjet::antikt_algorithm,0.1));
      PseudoJets smallSubJets=antiKTClusterSeq.inclusive_jets(ptmin);
      int smallJetMult = smallSubJets.size();
      _histSubJetMult->fill(smallJetMult,weight);
      /// may not be statistically correct to set stddev and skew to
      /// zero for a sample size of 1
      if(smallJetMult > 1)
	{
	  pt_stddev(smallSubJets,ptmean,ptstddev,static_cast<double>(smallJetMult));

	  foreach(const fastjet::PseudoJet& jeta, smallSubJets)
	    ptskew+=pow(jeta.pt()-ptmean,3);
	  ptskew=(ptskew/(static_cast<double>(smallJetMult)))/(pow(ptstddev,1.5)*jet.pt());
	  ptstddev=ptstddev/jet.pt();
	}

      _histSubJetSkew->fill(ptskew,weight);
      _histSubJetStddev->fill(ptskew,weight);

      unsigned int nSubJets=subJets.size();

      if(nSubJets==3)
	_histSubJet3Mass->fill((subJets.at(0)+subJets.at(1)+subJets.at(2)).m(),weight);
      
      for(unsigned int j=0;j!=nSubJets;++j)
	{
	  sumEt+=subJets.at(j).Et();
	  _histSubJetMass->fill(subJets.at(j).m());
	  for(unsigned int k=(j+1); k!=nSubJets;++k)
	    {
	      _histSubJetDeltaR->fill(subJets.at(j).delta_R(subJets.at(k)),weight);
	      _histSubJet2Mass->fill((subJets.at(j)+subJets.at(k)).m(),weight);
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
      const PseudoJets& jets = JetProjection.pseudoJetsByPt(25.0*GeV);

      if (jets.size() > 0) 
	{
	  unsigned int jetMult=jets.size();
	  _histJetMult->fill(jetMult);
	  /// Rather than loop over all jets, just take the first hard
	  /// one, Make sure entire jet is within fiducial volume
	  if(jets.front().eta() > -(2.5-0.6) && jets.front().eta() < (2.5-0.6))
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
      // was 1/sumOfWeights()
      normalize(_histJetMult ,1.0);
      normalize(_histJetPt ,1.0);
      normalize(_histJetE ,1.0);
      normalize(_histJetEta ,1.0);
      normalize(_histJetRapidity ,1.0);
      //normalize(_histJetPhi ,1.0);
      normalize(_histJetMass ,1.0);

      //normalize(_histJet2Mass ,1.0);
      //normalize(_histJet3Mass ,1.0);

      normalize(_histSubJet2Mass ,1.0);
      normalize(_histSubJet3Mass ,1.0);
      normalize(_histSubJetDeltaR ,1.0);
      normalize(_histSubJetMass ,1.0);
      normalize(_histSubJetSumEt ,1.0);

      normalize(_histSubJetSkew ,1.0);
      normalize(_histSubJetStddev ,1.0);

      //normalize(_histJetDipolarity,1.0);
      normalize(_histWJetCharge ,1.0);
      normalize(_histWCharge ,1.0);
      normalize(_histSubJetMult ,1.0);

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

    AIDA::IHistogram1D *_histSubJetSkew;
    AIDA::IHistogram1D *_histSubJetStddev;

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
/// Garbage bin
/*
  	      for(unsigned int l=(k+1); l!=nSubJets; ++l)
	      _histSubJet3Mass->fill((subJets.at(j)+subJets.at(k)+subJets.at(l)).m(),weight);
 */
      /*
      scale( _histJetMult , 1/(_histJetMult->sumAllBinHeights()));
      scale( _histJetPt , 1/(_histJetPt->sumAllBinHeights()));
      scale( _histJetE , 1/(_histJetE->sumAllBinHeights()));
      scale( _histJetEta , 1/(_histJetEta->sumAllBinHeights()));
      scale( _histJetRapidity , 1/(_histJetRapidity->sumAllBinHeights()));
      //scale( _histJetPhi , 1/(_histJetPhi->sumAllBinHeights()));
      scale( _histJetMass , 1/(_histJetMass->sumAllBinHeights()));
	                                            
      // Jet Theoreticals Theoreticals 	
      //scale( _histJet2Mass , 1/(_histJet2Mass->sumAllBinHeights()));
      //scale( _histJet3Mass , 1/(_histJet3Mass->sumAllBinHeights()));
	                                            
      scale( _histSubJet2Mass , 1/(_histSubJet2Mass->sumAllBinHeights()));
      scale( _histSubJet3Mass , 1/(_histSubJet3Mass->sumAllBinHeights()));
      scale( _histSubJetDeltaR , 1/(_histSubJetDeltaR->sumAllBinHeights()));
      scale( _histSubJetMass , 1/(_histSubJetMass->sumAllBinHeights()));
      scale( _histSubJetSumEt , 1/(_histSubJetSumEt->sumAllBinHeights()));
	                                            
      scale( _histSubJetSkew , 1/(_histSubJetSkew->sumAllBinHeights()));
      scale( _histSubJetStddev , 1/(_histSubJetStddev->sumAllBinHeights()));
      	                                            
      //scale( _histJetDipolarity , 1/(_histJetDipolarity->sumAllBinHeights()));
      scale( _histWJetCharge , 1/(_histWJetCharge->sumAllBinHeights()));
      scale( _histWCharge , 1/(_histWCharge->sumAllBinHeights()));
      //cout<<"Mean Charge: "<<_histWCharge->mean()<<"\u00B1"<<_histWCharge->rms()<<endl;
      scale( _histSubJetMult , 1/(_histSubJetMult->sumAllBinHeights()));
      */

