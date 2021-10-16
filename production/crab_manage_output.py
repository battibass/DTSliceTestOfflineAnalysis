#!/usr/bin/env python3
"""
This program takes care of skimming of DTNtuples:
- it checks for the presence of already skimmed files
- it allows parallel skimming using HTCondor
- it allows sequential interactive skimming
- it provides a status summary of the skim process
"""

import argparse
from datetime import datetime
from os import path, makedirs 
from sys import exit
import subprocess
import glob

#----------------
# Variables
#----------------

FILES_PER_JOB = 10

#-----------------
# Helper functions
#-----------------

def non_skimmed_files(input_folder, version):
    
    results = []
    
    paths_orig = glob.glob(path.join(input_folder, "*.root"))
    paths_skim = glob.glob(path.join(input_folder, "skim_" + version, "*.root"))
    files_orig = [path.basename(path_orig) for path_orig in paths_orig]
    files_skim = [path.basename(path_skim).replace("_skim", "") for path_skim in paths_skim]

    for file_orig in files_orig:
        if file_orig not in files_skim:
            results.append(file_orig)

    return results

def interactive_skim_command(input_folder, input_file, cut_string, version):

    output_file = input_file.replace(".root", "_skim.root")

    command = ["skimTree", 
               "-c '{}'".format(cut_string),
               path.join(input_folder, input_file),
               path.join(input_folder, "skim_" + version, output_file)]

    process = subprocess.Popen(command, stdout=subprocess.PIPE)
    output, error = process.communicate()

    print("[skimTree OUTPUT]:")
    print(output)

    if error:
        print("[skimTree ERROR]:")
        print(error)

    return

def interactive_skim(input_folder, cut_string, version):

    for input_file in non_skimmed_files(input_folder, version):
        print("[{}] running interactive skim for folder : {} and input file : {} .".format(__file__, input_folder, input_file))
        interactive_skim_command(input_folder, input_file, cut_string, version) 

    return

def condor_create_sh(input_folder, job_folder, input_files, cut_string, version, i_file):

    abs_folder = path.abspath(input_folder)
    sh_file_name = path.join(job_folder, "run_skim_{}.sh".format(i_file))
    sh_file = open(sh_file_name,"w")

    sh_file.write("#! /usr/bin/bash\n")
    sh_file.write("cd /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DT/OfflineCode/SliceTest/v9p7p1/CMSSW_11_2_3/src/\n")
    sh_file.write("eval `scramv1 runtime -sh`\n")
    sh_file.write("cd ..\n")

    for input_file in input_files:
        output_file = input_file.replace(".root", "_skim.root")

        command = ["skimTree", 
                   "-c '{}'".format(cut_string),
                   path.join(abs_folder, input_file),
                   path.join(abs_folder, "skim_" + version, output_file),
                   "\n"]

        sh_file.write(" ".join(command))

    sh_file.close()

    return sh_file_name

def condor_create_jdl(input_folder, job_folder, sh_file_name, version, i_file):

    jdl_file_name = path.join(job_folder, "run_skim_{}.jdl".format(i_file))
    jdl_file = open(jdl_file_name, "w")

    jdl_file.write("executable = {}\n".format(sh_file_name))
    jdl_file.write("universe = vanilla\n")
    jdl_file.write("output = {}/condor_{}_$(Cluster)_$(Process).out\n".format(job_folder, i_file))
    jdl_file.write("error  = {}/condor_{}_$(Cluster)_$(Process).err\n".format(job_folder, i_file))
    jdl_file.write("log    = {}/condor_{}_$(Cluster)_$(Process).log\n".format(job_folder, i_file))
    jdl_file.write("queue 1\n")

    jdl_file.close()

    return jdl_file_name

def condor_skim_command(jdl_file_name):

    command = ["condor_submit", jdl_file_name]

    process = subprocess.Popen(command, stdout=subprocess.PIPE)
    output, error = process.communicate()

    print("[condor OUTPUT]:")
    print(output)

    if error:
        print("[condor ERROR]:")
        print(error)

    return


def condor_skim(input_folder, job_folder, cut_string, version):

    files_tbp = non_skimmed_files(input_folder, version)
    file_blocks = [files_tbp[i_file:i_file + FILES_PER_JOB] for i_file in range(0, len(files_tbp), FILES_PER_JOB)]

    for file_block, i_block in zip(file_blocks, range(1, len(file_blocks)+1)):
        print("[{}] creating .sh script for file block # {}.".format(__file__, i_block))
        sh_file_name = condor_create_sh(input_folder, job_folder, file_block, cut_string, version, i_block)
        print("[{}] creating condor .jdl file for .sh script # {}.".format(__file__, i_block))
        jdl_file_name = condor_create_jdl(input_folder, job_folder, sh_file_name, version, i_block)
        print("[{}] running condor_submit for .jdl file # {}.".format(__file__, i_block))
        condor_skim_command(jdl_file_name)
        
    return

def status(input_folder, version):

    n_files_to_process = len(non_skimmed_files(input_folder, version))

    if n_files_to_process:    
        print("[{}] there are {} files which are not yet skimmed.".format(__file__, n_files_to_process))
        print("[{}] if you have no running jobs, please re-run this macro using either the 'condor_skim' or the 'interactive_skim' command.".format(__file__))
    else:
        print("[{}] all files have been skimmed, you can now proceed running hadd, e.g.: ".format(__file__, n_files_to_process))
        print("[{}] hadd DTDPGNtuple_runRUN_skim_{}.root {}/skim_{}/DT*root ".format(__file__, version, input_folder, version))

    return

if __name__ == '__main__':

    #----------------------
    # Setup argument parser
    #----------------------

    PARSER = argparse.ArgumentParser(description=__doc__)
    
    PARSER.add_argument("command",
                        help="Either: 'condor_skim', 'interactive_skim' or 'status'")
    
    PARSER.add_argument("crabOutputFolder",
                        help="Base folder with CRAB ntuple root files")

    PARSER.add_argument("-c", "--cut",
                        default="(seg_wheel==2 && seg_sector==12) || (ph2Seg_wheel==2 && ph2Seg_sector==12)",
                        help="ROOT cut string to be used for the skim")

    PARSER.add_argument("-v", "--version",
                        default="seg_in_slicetest_v1",
                        help="Version name of the skim")

    ARGS = PARSER.parse_args()

    #----------------
    # Variables
    #----------------

    COMMANDS = ["condor_skim", "interactive_skim", "status"]

    if ARGS.command not in COMMANDS:
        print("[{}] command : {} must be either 'condor_skim', 'interactive_skim' or 'status'. Quitting".format(__file__, ARGS.command))
        exit(100)

    if not path.isdir(ARGS.crabOutputFolder):
        print("[{}] folder : {} does not exist. Quitting".format(__file__, ARGS.crabOutputFolder))
        exit(100)

    job_tag_name = datetime.now().strftime("%d%m%Y_%H%M%S")
    job_folder = path.join("./condor_jobs", "{}_{}".format(job_tag_name, ARGS.version))
    skim_folder = path.join(ARGS.crabOutputFolder, "skim_{}".format(ARGS.version))

    OUT_FOLDERS = []
    
    if "skim" in ARGS.command:
        OUT_FOLDERS.append(skim_folder)

    if "condor" in ARGS.command:
        OUT_FOLDERS.append(job_folder)

    for folder in OUT_FOLDERS:
        folder_path = path.join(folder)
        if not path.exists(folder_path):
            makedirs(folder_path)

    if ARGS.command == "condor_skim":
        condor_skim(ARGS.crabOutputFolder, job_folder, ARGS.cut, ARGS.version)
    elif ARGS.command == "interactive_skim":
        interactive_skim(ARGS.crabOutputFolder, ARGS.cut, ARGS.version)
    elif ARGS.command == "status":
        if not path.isdir(skim_folder):
            print("[{}] folder : {} does not exist. Quitting".format(__file__, skim_folder))
            exit(100)
        status(ARGS.crabOutputFolder, ARGS.version)
        
