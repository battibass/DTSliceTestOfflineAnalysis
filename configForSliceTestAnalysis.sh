CMSSW_FOLDER=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DT/OfflineCode/SliceTest/v9p3p1/CMSSW_11_1_3/
TTRIG_DATASET=/eos/cms/store/data/Commissioning2020/MiniDaq/RAW/v1/
CRAB_CONFIG_SCRIPT=/cvmfs/cms.cern.ch/crab3/crab.sh
 
cd $CMSSW_FOLDER/src
echo "[$0]: Configuring CMSSW from directory $CMSSW_FOLDER/src"
cmsenv
cd -

echo "[$0]: Configuring tTrig calibration to use $TTRIG_DATASET"
export DTSAMPLEDIR=$TTRIG_DATASET

echo "[$0]: Configuring CRAB using $CRAB_CONFIG_SCRIPT"
. $CRAB_CONFIG_SCRIPT

echo "[$0]: Initializing proxy"
voms-proxy-init -voms cms -valid 192:0
