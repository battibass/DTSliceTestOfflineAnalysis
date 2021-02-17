import argparse
import sys
import os
from array import *
import json

"""
Function to define color plaette for 2D efficiency plots
"""

def efficiencyPalette() :
    
    from ROOT import TColor
    
    pcol = []
    
    for iBin in range(0,100) :

        rgb  = []

        if iBin < 70 :
            rgb = [0.70+0.007*iBin, 0.00+0.0069*iBin, 0.00]
    
        elif iBin < 90 :
            rgb = [0.70+0.007*iBin, 0.00+0.0069*iBin+0.10+0.01*(iBin-70), 0.00]
      
        else :
            rgb = [0.98-0.098*(iBin-90), 0.80, 0.00]

        pcol.append(TColor.GetColor(rgb[0], rgb[1], rgb[2]))
        
    return pcol

                    
"""
Setup argument parser
"""

parser = argparse.ArgumentParser(description="This program takes an input JSON config and extracts plots from ROOT files. " \
                                             + "The output consists of a plot with superimposed plots from multiple files." )
parser.add_argument("inputJsonConfig", help="Path to the input JSON config file")
parser.add_argument("inputFile", help="The input file to be processed")
parser.add_argument("plotFolder", help="Plot output folder")
parser.add_argument("-f", "--fast", default=0, action="count", help="Skip fetching and saving the fit canvases for each plot")
parser.add_argument("-v", "--verbosity", default=1, help="Increase or decrease output verbosity")
args = parser.parse_args()

"""
Parse JSON file
"""

with open(args.inputJsonConfig, 'r') as f:
    config = json.loads(f.read())

"""
Go through plots defined in config JSON
"""

from ROOT import * # import this here, otherwise it overwrites the argparse stuff
gROOT.SetBatch(True) # set ROOT to batch mode, this suppresses printing canvases
gROOT.ProcessLine("gErrorIgnoreLevel = 1001;") # suppress stdout pollution of canvas.Print(...)

TH1.AddDirectory(False)
gStyle.SetOptTitle(0)
gStyle.SetPaintTextFormat("1.3f");
gStyle.SetHistMinimumZero()

inputFile = TFile.Open(args.inputFile)

if not inputFile:
    print('[ERROR] File {} not found'.format(args.inputFile))
    sys.exit()

for keyPlot in config:
    if args.verbosity==1:
        print('Processing plot config: {}'.format(keyPlot))

    # Get result plots from fit canvases (in dir fit_eff_plots)

    # Get input parameters
    inputFilenames = []
    inputPlotNames = []
    inputFolders = []
    inputLegendEntries = []
    inputLabels = []
 
    for keyInputs in sorted(config[keyPlot]['inputs']):
        inputPlotNames.append(config[keyPlot]['inputs'][keyInputs]['plot'])
        inputFolders.append(config[keyPlot]['inputs'][keyInputs]['folder'])
        inputLegendEntries.append(config[keyPlot]['inputs'][keyInputs]['legendEntry'])
        inputLabels.append(config[keyPlot]['inputs'][keyInputs]['label'])


    # Get input graphs from files
    inputHistos = []
    for iHisto in range(len(inputPlotNames)):
        
        inputDir = inputFile.GetDirectory(inputFolders[iHisto])

        for keys in inputDir.GetListOfKeys():
            if keys.GetName() == inputPlotNames[iHisto]:
                inputName = keys.GetName()

                print('Load plot \'{}\': {}'.format(inputFolders[iHisto], inputName))
                print inputDir.Get(inputName).ClassName()

                histo = inputDir.Get(inputName).Clone(str(iHisto))
                inputHistos.append(histo)

    # Set line color and marker style for each graph using given maps from config
    colorMap    = config[keyPlot]['plot']['colorMap']
    markerMap   = config[keyPlot]['plot']['markerMap']
    legendRange = config[keyPlot]['plot']['legendRange']

    if args.verbosity==1:
        print('Using colormap: {}'.format(colorMap))
        print('Using markermap: {}'.format(markerMap))
    if len(colorMap)<len(inputFolders):
        print('[ERROR] The defined colormap has not enough entries for the number of defined input folders.')
        sys.exit()
    if len(markerMap)<len(inputFolders):
        print('[ERROR] The defined markermap has not enough entries for the number of defined input folders.')
        sys.exit()

    for iHisto in range(len(inputHistos)):
        
        inputHistos[iHisto].SetLineColor(colorMap[iHisto])
        inputHistos[iHisto].SetLineWidth(2)
        inputHistos[iHisto].SetMarkerStyle(markerMap[iHisto])
        inputHistos[iHisto].SetMarkerColor(colorMap[iHisto])

    # Setup canvas with all elements
    canvas = TCanvas('canvas', 'canvas', 800, 800)
    
    pad = TPad('pad', 'pad', 0.01, 0.00, 1.00, 1.00)

    option = config[keyPlot]['plot']['option']
             
    hasLogY = option.find("logY") > -1
    if hasLogY :
        option = option.replace("logY","")

    hasVerbLeg = option.find("verbLeg") > -1
    if hasVerbLeg :
        option = option.replace("verbLeg","")

    hasVerbRes = option.find("verbRes") > -1
    if hasVerbRes :
        option = option.replace("verbRes","")

    hasXRangeFromHisto = option.find("xRangeFromHisto") > -1
    if hasXRangeFromHisto :
        option = option.replace("xRangeFromHisto","")



    pad.SetGrid()
    pad.Draw()

    if hasLogY :
        pad.SetLogy()

    pad.cd()

    plotX  = config[keyPlot]['plot']['x']
    plotY  = config[keyPlot]['plot']['y']
 
    gStyle.SetOptStat(0)
        
    if config[keyPlot]['plot'].has_key("z") :
        plotZ = config[keyPlot]['plot']['z']

    rangeY = plotY[:2]

    for iHisto in range(len(inputHistos)):

        if inputHistos[iHisto].ClassName() == "TH1F" :

            if not hasLogY :
                rangeY[0] = 0.0

            rangeYFromHisto = inputHistos[iHisto].GetMaximum() * 1.5

            if rangeY[1] < rangeYFromHisto:
                rangeY[1] = rangeYFromHisto
    
    for iHisto in range(len(inputHistos)):
        
        histoDim   = inputHistos[iHisto].GetDimension()
        histoClass = inputHistos[iHisto].ClassName()

        inputHistos[iHisto].SetTitle(";"+plotX[2]+";"+plotY[2])
            
        if iHisto == 0 :
                inputHistos[iHisto].Draw(option)
        else :
            inputHistos[iHisto].Draw('same' + option)

        canvas.Update()

        if histoClass == "TEfficiency" and histoDim == 1:
            histo = inputHistos[iHisto].GetPaintedGraph()
        elif histoClass == "TEfficiency" and histoDim == 2:
            histo = inputHistos[iHisto].GetPaintedHistogram()
        else :
            histo = inputHistos[iHisto]
            
        if not hasXRangeFromHisto:
            if histoClass == "TEfficiency" and histoDim == 1 :
                histo.GetXaxis().SetLimits(plotX[0], plotX[1])
            else :
                histo.GetXaxis().SetRangeUser(plotX[0], plotX[1])
            
        histo.GetYaxis().SetRangeUser(rangeY[0], rangeY[1])

        if histoClass == "TEfficiency" and histoDim == 2 : 
            palette = efficiencyPalette()
            nBins = len(palette)
            gStyle.SetPalette(nBins,array('i',palette))
            histo.SetMinimum(plotZ[0])
            histo.SetMaximum(plotZ[1])
            histo.SetContour(nBins)
            histo.Draw(option)
                                
        if histoClass == "TProfile2D" : 
            histo.SetMinimum(plotZ[0])
            histo.SetMaximum(plotZ[1])
            gStyle.SetPalette(1)
            histo.Draw(option)

        elif histoClass == "TH2F" :
            gStyle.SetPalette(1)
            histo.Draw(option)

        else :
            histo.SetLineWidth(2)
            histo.GetXaxis().SetTitle(plotX[2])
            histo.GetYaxis().SetTitle(plotY[2])
            
        canvas.Update()

        histo.GetXaxis().SetLabelSize(22)
        histo.GetXaxis().SetTitleFont(63)
        histo.GetXaxis().SetLabelFont(43)
        histo.GetXaxis().SetTitleSize(22)
        histo.GetXaxis().SetLabelSize(20)
        histo.GetXaxis().SetTitleOffset(1.2)
        if plotX[2] == "sector" and histo.GetXaxis().GetNbins() <= 14 :
            for iBinX in range(1,histo.GetXaxis().GetNbins() +1) :
                histo.GetXaxis().SetBinLabel(iBinX,str(iBinX))
        
        histo.GetYaxis().SetLabelSize(22)
        histo.GetYaxis().SetTitleFont(63)
        histo.GetYaxis().SetLabelFont(43)
        histo.GetYaxis().SetTitleSize(22)
        histo.GetYaxis().SetLabelSize(20)
        histo.GetYaxis().SetTitleOffset(1.5)
        if plotY[2] == "wheel" and histo.GetYaxis().GetNbins() == 5 :
            for iBinY in range(1,histo.GetYaxis().GetNbins() +1) :
                histo.GetYaxis().SetBinLabel(iBinY,str(iBinY - 3))
                    
        canvas.Update()

    canvas.Update()

    canvas.cd()

    if len(inputHistos) > 1 :
    
        leg = TLegend(legendRange[0], legendRange[1], legendRange[2], legendRange[3])

        for iHisto in range(len(inputHistos)):
            legEntry = inputLegendEntries[iHisto]
            if hasVerbLeg :
                nUnderFlow = int(inputHistos[iHisto].GetBinContent(0))
                nOverFlow  = int(inputHistos[iHisto].GetBinContent(inputHistos[iHisto].GetNbinsX()+1))
                legEntry = legEntry + "   [uf/of: " + str(nUnderFlow) + "/" +str(nOverFlow) + "]" 

            if hasVerbRes :
                fitFunc = inputHistos[iHisto].GetFunction("fPhi")
                if fitFunc :
                    sigma    = int(fitFunc.GetParameter(2) * 10000.)
                    sigmaErr = int(fitFunc.GetParError(2) * 10000.)
                    legEntry = legEntry + "  [#sigma of gaussian fit = " + str(sigma) + " ]"

            leg.AddEntry(inputHistos[iHisto], legEntry, 'LP')

        leg.SetBorderSize(1)
        leg.SetTextFont(43)
        leg.SetTextSize(20)
        leg.Draw()

    canvas.cd()
    latex = TLatex()
    latex.SetNDC()
    latex.SetTextFont(61)
    latex.SetTextSize(0.030)
    latex.SetTextAlign(11);
    latex.DrawLatex(0.115, 0.91, config[keyPlot]['plot']['logo'][0])
    latex.SetTextFont(52)
    latex.SetTextSize(0.027)
    latex.SetTextAlign(11);
    latex.DrawLatex(0.179, 0.91, config[keyPlot]['plot']['logo'][1])
    latex.SetTextFont(42)
    latex.SetTextSize(0.030)
    latex.SetTextAlign(31);
    latex.DrawLatex(0.90, 0.91, config[keyPlot]['plot']['caption'])
    latex.SetTextAlign(11);
    latex.SetTextColor(1)
    latex.SetTextFont(61)
    latex.SetTextSize(0.032)
    latex.DrawLatex(legendRange[0], legendRange[3] + 0.02, config[keyPlot]['plot']['legendTitle'])
    canvas.Update()

    # Save plot
    if args.verbosity==1:
        print('Output directory: {}'.format(args.plotFolder))
    if not os.path.exists(args.plotFolder):
        os.makedirs(args.plotFolder)
    for fileType in config[keyPlot]['output']['fileType']:

        fileNamePlot = config[keyPlot]['output']['filenamePlot']
        canvas.SaveAs(os.path.join(args.plotFolder,fileNamePlot+'.'+fileType))

    if args.verbosity==1:
        print('')
