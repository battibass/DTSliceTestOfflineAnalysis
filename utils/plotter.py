#!/usr/bin/env python
# pylint: disable=C0103

"""
This program takes an input JSON config and
extracts plots from ROOT files. The output
consists of a canvas with superimposed
plots.
"""

import argparse
import sys
import os
import array as arr
import json

#########################
# Custom plotting options
#########################

optionFlags = ['logY', 'nEntries', 'verbLeg', 'verbRes', 'xRangeFromHisto']


#########################
# Function to define color
# plaette for 2D efficiency
# plots
#########################

def efficiencyPalette():

    """Palette for color plotting of 2D efficieny histos"""

    from ROOT import TColor

    pcol = []

    for iBin in range(0,100):

        rgb  = []

        if iBin<70:
            rgb = [0.70+0.007*iBin, 0.00+0.0069*iBin, 0.00]

        elif iBin<90:
            rgb = [0.70+0.007*iBin, 0.00+0.0069*iBin+0.10+0.01*(iBin-70), 0.00]
        else:
            rgb = [0.98-0.098*(iBin-90), 0.80, 0.00]

        pcol.append(TColor.GetColor(rgb[0], rgb[1], rgb[2]))

    return pcol


#########################
# Setup argument parser
#########################

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('inputJsonConfig', help='Path to the input JSON config file')
parser.add_argument('inputFile', help='The input file to be processed')
parser.add_argument('plotFolder', help='Plot output folder')
parser.add_argument('-f', '--fast', default=0, action='count',
                    help='Skip fetching and saving the fit canvases for each plot')
parser.add_argument('-v', '--verbosity', default=0,
                    help='Increase or decrease output verbosity')
args = parser.parse_args()

#########################
# Parse JSON file
#########################

with open(args.inputJsonConfig, 'r') as f:
    config = json.loads(f.read())

#########################
# Go through plots
# defined in config JSON
#########################

from ROOT import * # import this here, otherwise it overwrites the argparse stuff
gROOT.SetBatch(True) # set ROOT to batch mode, this suppresses printing canvases
gROOT.ProcessLine('gErrorIgnoreLevel = 1001;') # suppress stdout pollution of canvas.Print(...)

TH1.AddDirectory(False)
gStyle.SetOptTitle(0)
gStyle.SetPaintTextFormat('1.3f')
gStyle.SetHistMinimumZero()

inputFile = TFile.Open(args.inputFile)

if not inputFile:
    print('[ERROR] File {} not found'.format(args.inputFile))
    sys.exit()

# Setup canvas with all elements
canvas = TCanvas('canvas', 'canvas', 800, 800)

for keyPlot in config:
    if args.verbosity>=0:
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

                if args.verbosity>=1:
                    print('Load plot \'{}\': {}'.format(inputFolders[iHisto], inputName))

                histo = inputDir.Get(inputName).Clone(str(iHisto))
                inputHistos.append(histo)

    # Set line color and marker style for each graph using given maps from config
    colorMap    = config[keyPlot]['plot']['colorMap']
    markerMap   = config[keyPlot]['plot']['markerMap']
    legendRange = config[keyPlot]['plot']['legendRange']

    if args.verbosity>=1:
        print('Using colormap: {}'.format(colorMap))
        print('Using markermap: {}'.format(markerMap))
    if len(colorMap)<len(inputFolders):
        print('[ERROR] The defined colormap has not enough entries \
              for the number of defined input folders.')
        sys.exit()
    if len(markerMap)<len(inputFolders):
        print('[ERROR] The defined markermap has not enough entries \
               for the number of defined input folders.')
        sys.exit()

    for iHisto in range(len(inputHistos)):

        inputHistos[iHisto].SetLineColor(colorMap[iHisto])
        inputHistos[iHisto].SetLineWidth(2)
        inputHistos[iHisto].SetMarkerStyle(markerMap[iHisto])
        inputHistos[iHisto].SetMarkerColor(colorMap[iHisto])
    
    canvas.Clear()
    pad = TPad('pad', 'pad', 0.01, 0.00, 1.00, 1.00)

    option = config[keyPlot]['plot']['option']
    optionPlotter = {}

    for flag in optionFlags:

        hasFlag = flag in option
        optionPlotter[flag] = hasFlag

        if hasFlag:
            option = option.replace(flag,'')

    if "colz" in option:
        pad.SetRightMargin(0.15)

    pad.SetGrid()
    pad.Draw()

    if optionPlotter['logY'] :
        pad.SetLogy()

    pad.cd()

    plotX  = config[keyPlot]['plot']['x']
    plotY  = config[keyPlot]['plot']['y']

    gStyle.SetOptStat(0)

    if 'z' in config[keyPlot]['plot'] :
        plotZ = config[keyPlot]['plot']['z']

    rangeY = plotY[:2]

    for iHisto in range(len(inputHistos)):

        if inputHistos[iHisto].ClassName() == 'TH1F':

            if not optionPlotter['logY']:
                rangeY[0] = 0.0

            rangeYFromHisto = inputHistos[iHisto].GetMaximum() * 1.5

            if rangeY[1] < rangeYFromHisto:
                rangeY[1] = rangeYFromHisto

    for iHisto in range(len(inputHistos)):

        histoDim   = inputHistos[iHisto].GetDimension()
        histoClass = inputHistos[iHisto].ClassName()

        inputHistos[iHisto].SetTitle(';'+plotX[2]+';'+plotY[2])

        if iHisto == 0:
            inputHistos[iHisto].Draw(option)
        else:
            inputHistos[iHisto].Draw('same' + option)

        canvas.Update()

        if histoClass == 'TEfficiency' and histoDim == 1:
            histo = inputHistos[iHisto].GetPaintedGraph()
        elif histoClass == 'TEfficiency' and histoDim == 2:
            histo = inputHistos[iHisto].GetPaintedHistogram()
        else:
            histo = inputHistos[iHisto]

        if not optionPlotter['xRangeFromHisto']:
            if histoClass == 'TEfficiency' and histoDim == 1:
                histo.GetXaxis().SetLimits(plotX[0], plotX[1])
            else:
                histo.GetXaxis().SetRangeUser(plotX[0], plotX[1])

        histo.GetYaxis().SetRangeUser(rangeY[0], rangeY[1])

        if histoClass == 'TEfficiency' and histoDim == 2:
            palette = efficiencyPalette()
            nBins = len(palette)
            gStyle.SetPalette(nBins,arr.array('i',palette))
            histo.SetMinimum(plotZ[0])
            histo.SetMaximum(plotZ[1])
            histo.SetContour(nBins)
            histo.Draw(option)

        if histoClass == 'TProfile2D':
            histo.SetMinimum(plotZ[0])
            histo.SetMaximum(plotZ[1])
            gStyle.SetPalette(1)
            histo.Draw(option)

        elif histoClass == 'TH2F':
            gStyle.SetPalette(1)
            histo.Draw(option)

        else:
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

        histo.GetYaxis().SetLabelSize(22)
        histo.GetYaxis().SetTitleFont(63)
        histo.GetYaxis().SetLabelFont(43)
        histo.GetYaxis().SetTitleSize(22)
        histo.GetYaxis().SetLabelSize(20)
        histo.GetYaxis().SetTitleOffset(2.0 if histoClass == 'TH1F' and histo.GetMaximum() > 1000 else 1.5)

        canvas.Update()

    canvas.Update()

    canvas.cd()

    if len(inputHistos) > 1:

        leg = TLegend(legendRange[0], legendRange[1], legendRange[2], legendRange[3])

        for iHisto in range(len(inputHistos)):
            legEntry = inputLegendEntries[iHisto]
            if optionPlotter['verbLeg']:
                histo = inputHistos[iHisto]
                nUnderFlow = int(histo.GetBinContent(0))
                nOverFlow  = int(histo.GetBinContent(histo.GetNbinsX()+1))
                legEntry = '{}   [uf/of: {}/{}]'.format(legEntry, nUnderFlow, nOverFlow)

            if optionPlotter['nEntries']:
                nEntries  = int(inputHistos[iHisto].GetEntries())
                legEntry = '{}   [# entries: {}]'.format(legEntry, nEntries)

            if optionPlotter['verbRes']:
                fitFunc = inputHistos[iHisto].GetFunction('fPhi')
                if fitFunc:
                    sigma    = fitFunc.GetParameter(2)
                    sigmaErr = fitFunc.GetParError(2)
                    legEntry = '{}  [#sigma of gaussian fit = {:4.3f} cm]'.format(legEntry, sigma)

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
    latex.SetTextAlign(11)
    latex.DrawLatex(0.115, 0.91, config[keyPlot]['plot']['logo'][0])
    latex.SetTextFont(52)
    latex.SetTextSize(0.027)
    latex.SetTextAlign(11)
    latex.DrawLatex(0.179, 0.91, config[keyPlot]['plot']['logo'][1])
    latex.SetTextFont(42)
    latex.SetTextSize(0.030)
    latex.SetTextAlign(31)
    latex.DrawLatex(0.90, 0.91, config[keyPlot]['plot']['caption'])
    latex.SetTextAlign(11)
    latex.SetTextColor(1)
    latex.SetTextFont(61)
    latex.SetTextSize(0.032)
    latex.DrawLatex(legendRange[0], legendRange[3] + 0.02, config[keyPlot]['plot']['legendTitle'])
    canvas.Update()

    # Save plot
    if args.verbosity>=1:
        print('Output directory: {}'.format(args.plotFolder))
    if not os.path.exists(args.plotFolder):
        os.makedirs(args.plotFolder)
    for fileType in config[keyPlot]['output']['fileType']:

        fileNamePlot = config[keyPlot]['output']['filenamePlot']
        canvas.SaveAs(os.path.join(args.plotFolder,fileNamePlot+'.'+fileType))

    if args.verbosity>=1:
        print('')
