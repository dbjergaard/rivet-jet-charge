#!/usr/bin/python

import sys
from ROOT import * #TCanvas, TROOT, THStack, TLegend, TSystem
from ROOT import *
ROOT.gROOT.LoadMacro("AtlasStyle.C") 
SetAtlasStyle()
# Canonical ATLAS style doesn't display the titles... for papers, but
# since these are for a presentation,and I don't want to deal with
# copying the titles AGAIN, I'm breaking the rules...
# gStyle.SetOptTitle(1)
# Or not, this is going to take too much tweaking to make it look
# right... Very Clever ATLAS style

ratioHistos = [
    ['QuarkJetPt','Ratio of Quark-p_{T} vs total p_{T};p_{T} [GeV];#int f(x) dx #equiv 1',False],
    ['ChargeSignPurity','Ratio of Right-Signed Charge-p_{T} vs total p_{T};p_{T} [GeV];#int f(x) dx #equiv 1',False],
    ['JetPt','Transverse momentum of all Jets;p_{T} [GeV];#int f(x) dx #equiv 1',False]
]
stackHistos =[
    ['GluonJetCharge',' Truth-Gluon',False, kGreen-3],
    ['QuarkJetCharge',' Truth-Quark',False,kAzure-6],
    ['WJetCharge','Measured Charge',False,kBlue+3]
]
canonHistos =[
    ['JetPt','Transverse momentum of all Jets;p_{T} [GeV];#int f(x) dx #equiv 1',False],
    #['JetEta','#eta distribution of Jets;#eta;#int f(x) dx #equiv 1 '],
    ['JetMult','Jet Multiplicity;n;#int f(x) dx #equiv 1 ',False],
    #['TruthDeltaR','#Delta R of truth parton to jet;sterad;#int f(x) dx #equiv 1 ',False],
    ['WJetCharge',' Jet charge #times W charge  (Q_{j} Q_{W});e^{2};#int f(x) dx #equiv 1 ',False],
    ['JetPullTheta',' Jet Pull #theta_{t} ;#theta_{t};#int f(x) dx #equiv 1 ',False],
    ['JetPullMag',' Jet Pull Magnitude |t| ;|t|;#int f(x) dx #equiv 1 ',False]
    #['TruthPdgID',' PdgID of truth quark;n;#int f(x) dx #equiv 1 ',False],
]
generators = [
    ['Herwig++','Herwig++',kBlue+2],
    ['Pythia6.CTEQ6L1','Pythia 6, CTEQ6L1',kCyan+2],
    ['Pythia6.MSTWLO','Pythia 6, MSTWLO**',kCyan-3],
    ['Pythia8.4C.MSTW2008','Pythia 8, 4C, MSTW 2008',kMagenta+2],
    ['Pythia8.4C','Pythia 8, 4C',kMagenta-3],
    ['Sherpa','Sherpa',kRed+2]
]
#hlist   = []

# Open the .root files
for gen in generators:
    gen.append(TFile('rootfiles/'+gen[0]+'.root'));
# Print the 'canonical' histograms
for histo in canonHistos:
    hs = THStack(histo[0],histo[1])
    c = TCanvas(histo[0],histo[1])
    leg = TLegend(0.75,0.55,.95,0.95)
    leg.SetFillColor(0)
    leg.SetBorderSize(0)
    for gen in generators:
        #print "Getting ",histo[0], "from", gen[1]
        h = gen[-1].Get(histo[0])
        h.SetLineColor(gen[2])
        h.SetLineWidth(3)
        #hlist.append(h)
        hs.Add(h)
        leg.AddEntry(h,gen[1],"f")
    hs.Draw('nostack')
    leg.Draw()
    
    c.SetLogy(histo[2])
    c.Print(histo[0]+'.png')
# Stack Quark and Gluon charge to show relative fractions
hs = THStack('JetChargeStacked','Jet charge #times W charge  (Q_{j} Q_{W});e^{2};#int f(x) dx #equiv 1 ')
c = TCanvas('JetChargeStacked','Jet charge #times W charge  (Q_{j} Q_{W});e^{2};#int f(x) dx #equiv 1 ')
leg = TLegend(0.6,0.4,.85,0.9)
leg.SetFillColor(0)
leg.SetBorderSize(0)
for histo in stackHistos:
    h = generators[1][-1].Get(histo[0])
    h.SetLineColor(histo[3])
    h.SetLineWidth(3)
    hs.Add(h)
    leg.AddEntry(h,)
hs.Draw('nostack')
leg.Draw()
c.Print(hs.GetName()+'.png')
# Produce ratio plots
