#!/usr/bin/env python3
# pylint: disable=C0103

"""
This program apply rough <t0>-based corrections to the DT slice-test Phase-1 and
Phase-2 dbs in order to improve agreement between the two.
"""

import argparse
import subprocess
import os
from ROOT import *

#########################
# Helper function
#########################

def process_row(row, corrections):
    '''
    Apply tTrig corrections where relevant
    '''
    entries = row.split()
    assert len(entries) == 18 # entries in a row for a proper dump format
    for i_chamb, t0 in enumerate(corrections):
        if entries[0] == '2'  \
           and entries[2] == '12' \
           and entries[1] == str(i_chamb + 1) \
           and (entries[3] == '1' or entries[3] == '3'): # slice-test wheel/sector
            entries[6] = str(float(entries[6]) + t0)
            entries[-1] = str(float(entries[-1]) + t0)
    return ' '.join(entries) + '\n' 


def compute_correction(root_file, ph, ch):
    '''
    Compute tTrig corrections from segment t0 histograms
    '''
    t0_histogram = root_file.Get(f'Ph{ph}hT0_st{ch}')
    mean = t0_histogram.GetMean()
    f_gaus = t0_histogram.Fit("gaus", "S", "", mean - 20.0, mean + 20.0)
    return f_gaus.GetParams()[1]

#########################
# Setup argument parser
#########################

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("inputDBFile", help="Path to the input DB file")
parser.add_argument("inputROOTFile", help="Path to the input ROOT file")
parser.add_argument("-v", "--verbosity", default=0,
                    help="Increase or decrease output verbosity")
args = parser.parse_args()

#########################
# Go through directories
# from given toplevel
# directory and put the
# given PHP file in each
# directory
#########################

gROOT.SetBatch(True) # set ROOT to batch mode, this suppresses printing canvases
gROOT.ProcessLine('gErrorIgnoreLevel = 1001;') # suppress stdout pollution

input_root_file = TFile.Open(args.inputROOTFile)

if not input_root_file:
    print(f'[ERROR] File {args.inputROOTFile} not found')
    sys.exit()

t0_corrections = []

for chamber in range(1,5):
    t0_shifts = [compute_correction(input_root_file, phase, chamber) for phase in [1, 2]]
    t0_corrections.append(t0_shifts[1] - t0_shifts[0]) # phase-2 - phase-1

print(f'Corrections: {t0_corrections}')

assert len(t0_corrections) == 4

UTILS_FOLDER = os.path.join(os.environ['CMSSW_BASE'],
                            'src/DTDPGAnalysis/DTSliceTestOfflineAnalysis/utils/')
DB_INPUT_FILE = os.path.basename(args.inputDBFile)
TXT_INPUT_FILE = DB_INPUT_FILE.replace('.db','.txt')
TXT_OUTPUT_FILE = DB_INPUT_FILE.replace('.db','_corrected.txt')

print(os.environ['CMSSW_BASE'])

subprocess.run(f'cp {args.inputDBFile} ./{DB_INPUT_FILE}', shell=True, check=True)
subprocess.run(f'cmsRun {UTILS_FOLDER}/DumpDBToFile_cfg.py inputFile={DB_INPUT_FILE}',
               shell=True, check=True)

with open(TXT_INPUT_FILE, encoding='utf-8') as f_in, \
     open(TXT_OUTPUT_FILE,'w',encoding='utf-8') as f_out:
    for line in f_in.readlines():
        f_out.write(process_row(line, t0_corrections))

subprocess.run(f'cmsRun {UTILS_FOLDER}/DumpFileToDB_cfg.py inputFile={TXT_OUTPUT_FILE}',
               shell=True, check=True)
