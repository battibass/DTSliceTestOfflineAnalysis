#!/usr/bin/env python3
"""
This program generates an XML file to be processed by
the offline DTSliceTestOfflineAnalysis workflow which
combines information from the JSON file produced for
ntuple production with CRAB and the actual location of
the (skimmed and merged) DTDPGNtuple
"""

import argparse
import json
import sys
import os

import lxml.etree as etree

#----------------
# Variables
#----------------

OUT_FOLDER = "crab_xml"

if __name__ == '__main__':

    #----------------------
    # Setup argument parser
    #----------------------

    PARSER = argparse.ArgumentParser(description=__doc__)

    PARSER.add_argument("crabJSONFile",
                        help="JSON file used for processing with CRAB")

    PARSER.add_argument("dtNtuple",
                        help="The (skimmed and merged) DTNtuple after CRAB processing")

    ARGS = PARSER.parse_args()

    #---------------------------------
    # Generate XML configuration file
    #---------------------------------

    if not os.path.isfile(ARGS.crabJSONFile):
        print(f"[crab_json_to_xml] file : {ARGS.crabJSONFile} does not exist. Quitting")
        sys.exit(100)

    if not os.path.isfile(ARGS.dtNtuple):
        print(f"[crab_json_to_xml] file : {ARGS.dtNtuple} does not exist. Quitting")
        sys.exit(100)

    # READ and parse JSON
    with open(ARGS.crabJSONFile) as json_file:
        JSON = json.load(json_file)

    # default params
    PARAMS = {"t0File" : "",
              "t0FilePh2" : "",
              "tTrigFile" : "",
              "tTrigFilePh2" : "",
              "vDriftFile" : "",
              "inputFolderCentral" : "dataset {}".format(JSON["dataset"]),
              "inputFolderDT" : "NONE",
              "globalTag" : "NONE",
              "runNumber" : JSON["runNumber"],
              "ntupleName" : os.path.abspath(ARGS.dtNtuple),
              "nEvents" : "-1",
              "runOnTestPulse" : "False",
              "runOnDat" : "False",
              "inputFile" : "NONE"}

    for param_string in JSON["config_params"]:

        var, param = param_string.split("=")

        if param == "ntupleName":
            continue

        for path in JSON["input_files"]:
            if param in path:
                PARAMS[var] = path

    # fill and write XML
    XML_BASE = etree.Element("options")

    for var, param in PARAMS.items():
        etree.SubElement(XML_BASE, var).text = param

    if not os.path.exists(OUT_FOLDER):
        os.makedirs(OUT_FOLDER)

    xml_string = etree.tostring(XML_BASE, pretty_print=True)

    out_file_name = "ntuple_cfg_run" + PARAMS["runNumber"] + ".xml"
    out_file_path = os.path.join(OUT_FOLDER, out_file_name)

    with open(out_file_path, 'w') as file:
        file.write(xml_string.decode("utf-8"))
