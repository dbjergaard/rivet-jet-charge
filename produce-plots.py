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
    ['QuarkJetPt','Quark-p_{T} vs total p_{T};p_{T} [GeV];#scale[0.8]{#int} f(x) dx #equiv 1',False,0.0],
    ['QuarkJetEta','Quark-#eta vs total #eta;#eta;#scale[0.8]{#int} f(x) dx #equiv 1',False,0.0],
    ['ChargeSignPurity','Ratio of Right-Signed Charge-p_{T} vs total p_{T};p_{T} [GeV];#scale[0.8]{#int} f(x) dx #equiv 1',False,0.0],
]
stackHistos =[
    ['GluonJetCharge','Truth-Gluon',False, kCyan-7,0.0],
    ["QuarkNegTwoThirds",'Truth-qQ=-2/3',True,kYellow-7,0.0],
    ["QuarkNegOneThird",'Truth-qQ=-1/3',True,kGreen-7,0.0],
    ["QuarkOneThird",'Truth-qQ=1/3',True,kMagenta-7,0.0],
    ["QuarkTwoThirds",'Truth-qQ=2/3',True,kBlue-7,0.0],
    #['WJetCharge','Measured Charge',False,kRed+3,0.0]
]
canonHistos =[
    #['JetPt','Transverse momentum of all Jets;p_{T} [GeV];#scale[0.8]{#int} f(x) dx #equiv 1',False,1.0],
    #['JetEta','#eta distribution of Jets;#eta;#scale[0.8]{#int} f(x) dx #equiv 1 ',False,1.0],
    #['QuarkJetPt','Quark-p_{T} vs total p_{T};p_{T} [GeV];#scale[0.8]{#int} f(x) dx #equiv 1',False,0.0],
    #['ChargeSignPurity','Ratio of Right-Signed Charge-p_{T} vs total p_{T};p_{T} [GeV];#scale[0.8]{#int} f(x) dx #equiv 1',False,0.0],
    ['JetMult','Jet Multiplicity;n;#scale[0.8]{#int} f(x) dx #equiv 1 ',False,1.0],
    ['TruthDeltaR','#Delta R of truth parton to jet;sterad;#scale[0.8]{#int} f(x) dx #equiv 1 ',False,1.0],
    ['WJetChargeK5',' Jet charge #times W charge  (Q_{j} Q_{W}) (k=0.5);e^{2};#scale[0.8]{#int} f(x) dx #equiv 1 ',False,1.0],
    ['WJetChargeK3',' Jet charge #times W charge  (Q_{j} Q_{W}) (k=0.3);e^{2};#scale[0.8]{#int} f(x) dx #equiv 1 ',False,1.0],
    #['JetPullTheta',' Jet Pull #theta_{t} ;#theta_{t};#scale[0.8]{#int} f(x) dx #equiv 1 ',False,1.0],
    #['JetPullMag',' Jet Pull Magnitude |t| ;|t|;#scale[0.8]{#int} f(x) dx #equiv 1 ',False,1.0],
    ['TruthPdgID',' PdgID of truth quark;n;#scale[0.8]{#int} f(x) dx #equiv 1 ',False,1.0]
]
boost_generators = [
    ['Pythia6.Perugia.2011','Pythia 6, Perugia 2011',kCyan+3],
    ['Pythia6.NOcr','Pythia 6, NOcr',kCyan+1],
    ['Herwig++','Herwig++',kBlue+2],
    ['Pythia8.MSTW2008','Pythia 8, 4C, MSTW 2008',kMagenta+2],
    #['Pythia8.4C','Pythia 8, 4C',kMagenta+2],
    ['Sherpa','Sherpa',kRed+2]
]
pythia_generators = [
    ['Pythia6.CTEQ6L1','Pythia 6, CTEQ6L1',kCyan+1],
    ['Pythia6.MSTWLO','Pythia 6, MSTWLO**',kCyan+3],
    ['Pythia8.MSTW2008','Pythia 8, 4C, MSTW 2008',kMagenta+2],
    ['Pythia8.CTEQ6L1','Pythia 8, 4C, CTEQ6L1',kMagenta]
]
def set_hist_opts(hist, color):
    hist.SetLineColor(color)
    hist.SetMarkerColor(color) 
    hist.SetLineWidth(3)
    hist.SetMarkerStyle(1)
def print_histo(canvas,hist,prefix):
    canvas.RedrawAxis()
    canvas.SetLogy(hist[2])
    canvas.Print(prefix + hist[0]+'.eps')
    canvas.Print(prefix + hist[0]+'.png')
    
def process_gens(generators, histName, legend, stackHist,norm):
    legend.SetFillColor(0)
    legend.SetBorderSize(0)
    for gen in generators:
        #print "Getting ", histName, "from",gen[0]
        h = gen[-1].Get(histName)
        if h.Integral() != 0 and norm != 0:
            h.Scale(norm/h.Integral())
        set_hist_opts(h,gen[2])
        stackHist.Add(h)
        legend.AddEntry(h,gen[1])
    stackHist.Draw('Hnostack')
    legend.Draw()
def print_canon_hists(outPrefix, generators):
    for histo in canonHistos:
        hs = THStack(histo[0],histo[1])
        c = TCanvas(histo[0],histo[1],800,600)
        if histo[0]=='JetPullTheta':
            leg = TLegend(0.2,0.25,.45,0.55)
        else:
            leg = TLegend(0.72,0.65,.95,0.95)
        process_gens(generators,histo[0],leg,hs,histo[3])
        print_histo(c,histo,outPrefix)
def rebin_ratio_hists(generators,histList):
    for histo in histList:
        for gen in generators:
            numHist = gen[-1].Get(histo)
            numHist.Rebin(5)

def print_ratio_hists(outPrefix,generators,histo,denHistName,sumDen):
    hs = THStack(histo[0],histo[1])
    c = TCanvas(histo[0],histo[1],800,600)
    leg = TLegend(0.2,0.25,.45,0.45)
    leg.SetFillColor(0)
    leg.SetBorderSize(0)
    for gen in generators:
        #print "Getting ", denHistName, "and", histo[0], "from", gen[1]
        numHist = gen[-1].Get(histo[0])
        denHist = gen[-1].Get(denHistName)
        if sumDen:
            denHist.Add(numHist,denHist)
        numHist.Divide(numHist,denHist)
        set_hist_opts(numHist,gen[2])
        hs.Add(numHist)
        leg.AddEntry(numHist,gen[1])
    hs.Draw('Hnostack')
    leg.Draw()
    print_histo(c,histo,outPrefix)

# Open the .root files
for gen in boost_generators:
    gen.append(TFile('rootfiles/'+gen[0]+'.root'));
for gen in pythia_generators:
    gen.append(TFile('rootfiles/'+gen[0]+'.root'));
# Stack Quark and Gluon charge to show relative fractions
klist=['K3','K5']
for k in klist:
    hs = THStack('JetChargeStacked'+k,'Jet charge #times W charge  (Q_{j} Q_{W});e^{2};#scale[0.8]{#int} f(x) dx #equiv 1 ')
    c = TCanvas('JetChargeStacked'+k,'Jet charge #times W charge  (Q_{j} Q_{W});e^{2};#scale[0.8]{#int} f(x) dx #equiv 1 ',800,600)
    leg = TLegend(0.72,0.75,.95,0.95)
    leg.SetFillColor(0)
    leg.SetBorderSize(0)
    for histo in stackHistos:
        h = boost_generators[0][-1].Get(histo[0]+k)
        set_hist_opts(h,histo[3]+3)
        h.SetFillColor(histo[3])
        h.SetFillStyle(1001)
        hs.Add(h)
        leg.AddEntry(h,histo[1])
    total = boost_generators[0][-1].Get('WJetCharge'+k)
    hs.Draw('H')
    total.SetMarkerStyle(1)
    total.SetLineWidth(2)
    total.SetLineColor(1)
    total.Draw('Hsame')
    leg.AddEntry(total,'Measured Charge')
    leg.Draw()
    c.RedrawAxis()
    c.Print(hs.GetName()+'.png')
    c.Print(hs.GetName()+'.eps')
# Produce ratio plots
rebin_ratio_hists(pythia_generators,['QuarkJetPt','GluonJetPt','ChargeSignPurity'])
print_ratio_hists('PDFComparison_',pythia_generators,ratioHistos[2],'QuarkJetPt',False)
print_ratio_hists('PDFComparison_',pythia_generators,ratioHistos[1],'GluonJetEta',True)
print_ratio_hists('PDFComparison_',pythia_generators,ratioHistos[0],'GluonJetPt',True)

rebin_ratio_hists(boost_generators,['QuarkJetPt','GluonJetPt','ChargeSignPurity'])
print_ratio_hists('BOOST_',boost_generators,ratioHistos[2],'QuarkJetPt',False)
print_ratio_hists('BOOST_',boost_generators,ratioHistos[1],'GluonJetEta',True)
print_ratio_hists('BOOST_',boost_generators,ratioHistos[0],'GluonJetPt',True)
# Print the 'canonical' histograms
print_canon_hists('BOOST_',boost_generators)
print_canon_hists('PDFComparison_',pythia_generators)
