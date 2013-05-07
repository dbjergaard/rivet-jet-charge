# Rivet Jet Substructure Study
## Obtaining and running
1. Clone this repository
```git clone https://github.com/dbjergaard/rivet-jet-charge.git```
2. Setup rivet with 'source rivet-env.sh'
3. Patch FastJets to include particles() method, recompile FastJets
```sh 
   patch  ~/rivet/build/rivet/include/Rivet/Projections/FastJets.hh < BOOSTFastJets.patch
   cd ~/rivet/build/rivet && make -j 4 && make install && cd -
```
4. Run 'make' in the repo directory
5. Have fun looking at substructure histograms!

## Physics Motivation
The big picture aim of this study is to provide an accurate picture of
how different Monte Carlo generators handle creation of jets.  In
order to fully assess different generators, we must extract as much
information as possible from the jets.
## Implemented theoreticals
### Jet Charge
Jet Charge is defined as the sum of the charge of the constituent particles
weighted by the particle's transverse momentum. 
### Jet Dipolarity
Jet dipolarity is a p_T weighted sum over the radii of sub*jets. It is
typically only defined for jets with two subjets. 
### Jet Pull
### Angular Correlation Functions
### Pruning, Filtering, Trimming
## Analysis Cuts
### W boson selection
* The W finder projection is configured as 
      std::vector<std::pair<double, double > > muonEtaRanges;
      muonEtaRanges.push_back(make_pair(*2.4,2.4));
      WFinder muWFinder(fs, muonEtaRanges, 20#GeV, MUON, 
			40#GeV,1000#GeV,25#GeV,0.4,true,false,80.4,true);
* In english:
  * Selections on the Muon:
* $|\eta|<2.4$
* $p_T > 20 GeV/c$
* $\Delta R >0.4$ (Maximum delta R between muon and a jet)
  * Selections on the W
  * $40 GeV/c^2 < m_T < 1000 GeV/c^2$
clusterPhotons is true, trackPhotons is false
## Histograms
   * Jet Multiplicity 
     * Histogram of number of Jets in events over all events
   * Inclusive p_T
   * Inclusive E
   * Inclusive Mass
   * Invariant mass of jet pairs
   * Invariant mass of jet triples
   * Invariant mass of sub*jet pairs
   * Invariant mass of sub*jet triples
   * DeltaR of sub*jets
   * Invariant mass of all subjets
   * Dipolarity of jet with two subjets
   * Charge of jet
