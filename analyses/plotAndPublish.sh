#! /usr/bin/bash

##########################
# Configuration parameters
##########################

RUN_FOLDER="./run329614"

UTILS_FOLDER="./plotAndPublish/"
PUBLUSH_FOLDER="/eos/user/b/battilan/www/DTDPG/proveSliceTest/"

PLOTTER_CFGS=("configDigi.json" "configTrigger.json")
PLOTTER_ROOTS=("results_digi.root" "results_trigger.root")
PLOTTER_FOLDERS=("digi/" "trigger/")

PLOTTER_CMD="plotter.py"
PUBLISH_CMD="publishDir.py"

INDEX_FILE="index.php"


##########################
# Commands
##########################

cmsenv

for index in ${!PLOTTER_CFGS[*]}
do :
    python $UTILS_FOLDER/$PLOTTER_CMD $UTILS_FOLDER/${PLOTTER_CFGS[$index]} $RUN_FOLDER/${PLOTTER_FOLDERS[$index]} ${PLOTTER_ROOTS[$index]}
done

python $UTILS_FOLDER/$PUBLISH_CMD $RUN_FOLDER $UTILS_FOLDER/$INDEX_FILE

scp -r $RUN_FOLDER battilan@lxplus:$PUBLUSH_FOLDER




