# DTSliceTestOfflineAnalysis
Package collecting the offline analysis programs for the DT slice-test

## Downloading the package and setting up the environment:
```
git clone https://github.com/battibass/DTSliceTestOfflineAnalysis.git

cd DTSliceTestOfflineAnalysis

# For BASH:
. configForSlcieTestAnalysis.sh
```

## Producing ntuples

### Running the ntuple production:
```
# From the DTSliceTestOfflineAnalysis "base" folder:
cd production

# Print ntuple production cfg configuration options:
python dtDpgNtuples_slicetest_cfg.py --help

# Run ntuple production:
cmsRun dtDpgNtuples_slicetest_cfg.py inputFile=/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/root/run329614_streamDQM_fu-c2f13-09-03.root ntupleName=./DTDPGNtuple_run329614.root 

# Copy the ntuple in the DT EOS area:
cp DTDPGNtuple_run329614.root /eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/ntuples/
```

### Using tools to produce rough ad-hoc pedestal calibrations:
```
# From the DTSliceTestOfflineAnalysis "base" folder:
cd production/calib

# Dump tTrigs from GlobalTag into txt file:
cmsRun DumpDBToFile_cfg.py

# Edit the txt:
# - You can use an existing ntuple to get tTrig values
#   from it, look either at the segment t0 or at the 
#   rising edge of the phase-1 timeboxes.
# - Once ready open the txt file and look for "2 2 12"
#   (these are wheel station sector), then edit the 
#   7th column (it corresponds to tTrigs) for all 3 
#   SLs.
# - 2475.00 is a reasonable tTrig for run 329614.
emacs -nw 106X_dataRun2_v10_cosmics_ttrig.txt

# Create an sqlite file from the edited txt:
cmsRun DumpFileToDB_cfg.py

# Run ntuple production using the new tTrigs:
cd ..
cmsRun dtDpgNtuples_slicetest_cfg.py inputFile=/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/root/run329614_streamDQM_fu-c2f13-09-03.root ntupleName=./DTDPGNtuple_run329614.root tTrigFile=calib/TTrigDB_cosmics_ttrig.db 

# If OK, copy again the file to the DT EOS area:
cp DTDPGNtuple_run329614.root /eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/ntuples/
```

## Running ntuple analysis and publishing in a webpage:

### Running the analysis:
```
# From the DTSliceTestOfflineAnalysis "base" folder:
cd analyses

root -b

root [0] .x loadAllAnalyses.C
root [1] .L runAllAnalyses.C
root [2] runAllAnalyses("/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/ntuples/DTDPGNtuple_run329614.root",329614)

```

### Publishing the plots:
```
. plotAndPublish.sh run329614/
# Type the dtdqm@lxplus ssh password to publish a directory
```
