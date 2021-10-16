#!/usr/bin/env python3
"""
This program generates a JSON that is then processed by the crab
configuration scripts for global run ntuple production.
"""

import argparse
import json
import os

import slice_test_autocond as autocond

#----------------
# Variables
#----------------

OUT_FOLDER = "crab_jsons"
CRAB_DEFAULT_FILE = "crab.json"

#----------------
# Helper function
#----------------

def add_files(file_path, param, input_files, config_params):

    """
    Add files to JSON lists
    """

    if file_path != "":
        input_files.append(file_path)
        config_params.append(param + "=" + os.path.basename(file_path))

    return input_files, config_params

if __name__ == '__main__':

    #----------------------
    # Setup argument parser
    #----------------------

    PARSER = argparse.ArgumentParser(description=__doc__)

    PARSER.add_argument("runNumber",
                        help="Number of the run to be analyzed")

    PARSER.add_argument("-d", "--dataset",
                        default="/ExpressCosmics/Commissioning2021-Express-v1/FEVT",
                        help="Dataset to be processed")

    PARSER.add_argument("-o", "--outLFNBase",
                        default="/store/group/dpg_dt/comm_dt/commissioning_2021_data/crab/",
                        help="Output LFN base directory")

    PARSER.add_argument("-v", "--version",
                        default="v1",
                        help="version of the processing")

    PARSER.add_argument("--tTrigPh1",
                        default="",
                        help="Phase-1 tTrig file")

    PARSER.add_argument("--t0Ph1",
                        default="",
                        help="Phase-1 t0 file")

    PARSER.add_argument("--tTrigPh2",
                        default="",
                        help="Phase-2 tTrig file")

    PARSER.add_argument("--t0Ph2",
                        default="",
                        help="Phase-2 t0 file")

    PARSER.add_argument("--autocond",
                        default="",
                        help="Autocond label: valid ones are {}".format(autocond.labels()))

    ARGS = PARSER.parse_args()

    #---------------------------------
    # Generate JSON configuration file
    #---------------------------------

    JSON = {}

    JSON["runNumber"] = ARGS.runNumber

    JSON["dataset"] = ARGS.dataset
    JSON["outLFNBase"] = ARGS.outLFNBase
    JSON["version"] = ARGS.version

    INPUT_FILES = []
    CONFIG_PARAMS = ['ntupleName=DTDPGNtuple.root']

    TTRIG_PH1 = autocond.get_ttrig("phase1",ARGS.autocond) \
        if (ARGS.autocond and not ARGS.tTrigPh1) else ARGS.tTrigPh1
    TTRIG_PH2 = autocond.get_ttrig("phase2",ARGS.autocond) \
        if (ARGS.autocond and not ARGS.tTrigPh2) else ARGS.tTrigPh2
    T0_PH1 = autocond.get_t0i("phase1") if (ARGS.autocond and not ARGS.t0Ph1) else ARGS.t0Ph1
    T0_PH2 = autocond.get_t0i("phase2") if (ARGS.autocond and not ARGS.t0Ph2) else ARGS.t0Ph2

    INPUT_FILES, CONFIG_PARAMS = add_files(TTRIG_PH1, "tTrigFile", INPUT_FILES, CONFIG_PARAMS)
    INPUT_FILES, CONFIG_PARAMS = add_files(TTRIG_PH2, "tTrigFilePh2", INPUT_FILES, CONFIG_PARAMS)
    INPUT_FILES, CONFIG_PARAMS = add_files(T0_PH1, "t0File", INPUT_FILES, CONFIG_PARAMS)
    INPUT_FILES, CONFIG_PARAMS = add_files(T0_PH2, "t0FilePh2", INPUT_FILES, CONFIG_PARAMS)

    JSON["input_files"] = INPUT_FILES
    JSON["config_params"] = CONFIG_PARAMS

    if not os.path.exists(OUT_FOLDER):
        os.makedirs(OUT_FOLDER)

    OUT_FILE_NAME = "crab_run" + str(ARGS.runNumber) + ".json"

    OUT_FILE_PATH = os.path.join(OUT_FOLDER, OUT_FILE_NAME)

    with open(OUT_FILE_PATH, 'w') as out_file:
        json.dump(JSON, out_file, indent=4, ensure_ascii=False)

    CRAB_DEFAULT_PATH = os.path.join(OUT_FOLDER, CRAB_DEFAULT_FILE)

    if os.path.exists(CRAB_DEFAULT_PATH):
        os.unlink(CRAB_DEFAULT_PATH)

    os.symlink(OUT_FILE_NAME, CRAB_DEFAULT_PATH)
