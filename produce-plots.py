import sys
from ROOT import * #TCanvas, TROOT, THStack, TLegend, TSystem
#!/usr/bin/python
ratioHistos = [
    ['QuarkJetPt','Ratio of Quark-p_T vs total p_T;$p_T$ [GeV];$\int f(x) dx \equiv 1$',False],
    ['ChargeSignPurity','Ratio of Right-Signed Charge-p_T vs total p_T;$p_T$ [GeV];$\int f(x) dx \equiv 1$',False]
]
stackHistos =[
    ['GluonJetCharge',' Truth-Gluon Jet charge $\times$ W charge  ($Q_j Q_W$);$e^2$;$\int f(x) dx \equiv 1$ ',False],
    ['QuarkJetCharge',' Truth-Quark Jet charge $\times$ W charge  ($Q_j Q_W$);$e^2$;$\int f(x) dx \equiv 1$ ',False]
]
canonHistos =[
    ['JetPt','Transverse momentum of all Jets;$p_T$ [GeV];$\int f(x) dx \equiv 1$',False],
    #['JetEta','$\eta$ distribution of Jets;$\eta$;$\int f(x) dx \equiv 1$ '],
    ['JetMult','Jet Multiplicity;$n$;$\int f(x) dx \equiv 1$ ',False],
    #['TruthDeltaR','$\Delta R$ of truth parton to jet;sterad;$\int f(x) dx \equiv 1$ ',False],
    ['WJetCharge',' Jet charge $\times$ W charge  ($Q_j Q_W$);$e^2$;$\int f(x) dx \equiv 1$ ',False],
    ['JetPullTheta',' Jet Pull $\theta_t$ ;$\theta_t$;$\int f(x) dx \equiv 1$ ',False],
    ['JetPullMag',' Jet Pull Magnitude $|t|$ ;$|t|$;$\int f(x) dx \equiv 1$ ',False],
    #['TruthPdgID',' PdgID of truth quark;n;$\int f(x) dx \equiv 1$ ',False],
]
generators = [
    ['Herwig++','Herwig++',kBlue+2],
    ['Pythia6.CTEQ6L1','Pythia 6, CTEQ6L1',kCyan+2],
    ['Pythia6.MSTWLO','Pythia 6, MSTWLO**',kCyan-3],
    ['Pythia8.4C.MSTW2008','Pythia 8, 4C, MSTW 2008',kMagenta+2],
    ['Pythia8.4C','Pythia 8, 4C',kMagenta-3],
    ['Sherpa','Sherpa',kRed+2],
]
#hlist   = []

for gen in generators:
    gen.append(TFile('rootfiles/'+gen[0]+'.root'));
for histo in canonHistos:
    hs = THStack(histo[0],histo[1])
    c = TCanvas(histo[0],histo[1])
    leg = TLegend(0.86,0.4,1.0,0.8)
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
    c.SetLogy(histo[2])
    leg.Draw()
    c.Print(histo[0]+'.png')

