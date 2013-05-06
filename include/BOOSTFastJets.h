//-*- C++ -*-

#ifndef RIVET_BOOSTFastJets_HH
#define RIVET_BOOSTFastJets_HH
#include "Rivet/Projections/FastJets.hh"

namespace Rivet{
  class BOOSTFastJets : public FastJets{
  public:
    BOOSTFastJets(const FinalState& fsp, JetAlgName alg,
             double rparameter, double seed_threshold=1.0)
      : FastJets(fsp,alg,rparameter,seed_threshold){};
    ~BOOSTFastJets(){};
    /// structs used in angular correlation calculations
    struct ACFparticlepair {
      double deltaR;
      double weight;
    };

    struct ACFpeak {
      double Rval;
      double height;
      double prominence;
      double partialmass;
      int index;
    };

    struct ppsortfunction { bool operator()
    (ACFparticlepair a, ACFparticlepair b) { return (a.deltaR < b.deltaR); } };

    struct ACFfunction {
      double Rval;
      double value;
    };

    /// Calculate Dipolarity of Jet
    double Dipolarity(const fastjet::PseudoJet &j) const;
    // Calculate Pull of Jet
    std::pair<double,double> JetPull(const fastjet::PseudoJet &j, const double ptmin=-1*GeV) const;
    /// Calculate JetCharge
    double JetCharge(const fastjet::PseudoJet &j, const double k=0.5, const double ptmin=-1*GeV) const;

    fastjet::JetAlgorithm setJetAlgorithm(JetAlgName subJetAlgorithm) const;
    /// Create a filter, run it over specified jet
    /// Butterworth, Davison, Rubin and Salam, arXiv:0802.2470
    fastjet::PseudoJet Filter(fastjet::PseudoJet jet, JetAlgName subjet_def, 
				      int hardest,double subjet_R) const;

    /// Create a trimmer, run it over specified jet
    /// Krohn, Thaler and Wang, arXiv:0912.1342
    fastjet::PseudoJet Trimmer(fastjet::PseudoJet jet, JetAlgName subjet_def, 
				       double percentage, double subjet_R) const;

    /// Create a pruner, run it over specified jet
    /// Ellis, Vermilion and Walsh, arXiv:0903.5081
    fastjet::PseudoJet Pruner(fastjet::PseudoJet jet, JetAlgName subjet_def, 
						double zcut, double Rcut_factor) const;

    /// Get N=n_jets subjets to be used for finding N-subjettiness
    /// Thaler, Van Tilburg, arXiv:1011.2268
    PseudoJets GetAxes(unsigned int n_jets, 
			PseudoJets& inputJets, JetAlgName subjet_def, double subR) const;

    /// Get the N-subjettiness with respect to the subjet axes.
    /// Thaler, Van Tilburg, arXiv:1011.2268
    double TauValue(double beta, double jet_rad, 
	PseudoJets& particles, PseudoJets& axes) const;

    /// Update axes towards Tau(y, phi) minimum.
    /// Thaler, Van Tilburg, arxiv:1108.2701
    void UpdateAxes(double beta,
	PseudoJets& particles, PseudoJets& axes) const;

    /// Find peaks in Angular Structure Function for the given particles
    /// Jankowiak, Larkowski, arxiv:1104.1646
    /// Based on code by Jankowiak and Larkowski
    vector<ACFpeak> ASFPeaks(PseudoJets& particles,
		unsigned int most_prominent=0, double minprominence=0.0) const;
  private:
    double KeyColToRight(int p, vector<ACFpeak> peaks, vector<double> ASF_erf) const;

    double KeyColToLeft(int p, vector<ACFpeak> peaks, vector<double> ASF_erf) const;

  };
}

#endif
