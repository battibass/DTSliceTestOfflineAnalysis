setenv CMSSW_FOLDER /afs/cern.ch/cms/CAF/CMSCOMM/COMM_DT/OfflineCode/SliceTest/v9p2/CMSSW_11_0_2/
setenv CRAB_CONFIG_SCRIPT /cvmfs/cms.cern.ch/crab3/crab.sh
 
cd ${CMSSW_FOLDER}/src
echo "[${0}]: Configuring CMSSW from directory ${CMSSW_FOLDER}/src"
cmsenv
cd -

echo "[${0}]: Configuring CRAB using ${CRAB_CONFIG_SCRIPT}"
source ${CRAB_CONFIG_SCRIPT}

echo "[${0}]: Initializing proxy"
voms-proxy-init -voms cms -valid 192:0
