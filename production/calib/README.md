# DTSliceTestOfflineAnalysis / production / calib
A few instructions about how to run calibrations

## Run t0i production for phase-2
```
# Print command line options
python dtT0WireCalibration_cfg.py --help 

# Get t0i out of a specific test-pulse run
cmsRun dtT0WireCalibration_cfg.py runNumber=333364 \
       				  runOnDat=True    \
				  inputFolderCentral=/eos/cms/store/t0streamer/Minidaq/A/
```

## Run tTrig production
```
# Run the tTrig production workflow
# using t0 from the previous step (./t0_run333364.db)
dtCalibration ttrig timeboxes all --run=333369 --trial=1 --label=ttrig_timebox \
	      	    	      	  --runselection=333369 --datasetpath=/Dummy/Dummy/RAW \
				  --globaltag=106X_dataRun3_Express_v2  --datasettype=Cosmics \
				  --run-on-RAW --phase2 --inputT0DB ./t0_run333364.db --input-files-local 
```

## Run ntuples with the new t0i and tTrigs
```
cd .. # get back to production directory

cmsRun dtDpgNtuples_slicetest_cfg.py runNumber=333369 \
       				     tTrigFilePh2=./calib/Run333369-ttrig_timebox_v1/TimeBoxes/results/ttrig_timeboxes_Run333369_v1.db \
				     t0FilePh2=./calib/t0_run333364.db
```

