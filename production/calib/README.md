# DTSliceTestOfflineAnalysis / production / calib
A few instructions about how to run calibrations

## Run t0i production for phase-2
```
# Print command line options
python dtT0WireCalibration_cfg.py --help 

# Get t0i out of a specific test-pulse run
cmsRun dtT0WireCalibration_cfg.py runNumber=333514
```

## Run tTrig production
```
# Run the tTrig production workflow
# using t0 from the previous step (./t0_run333364.db)
dtCalibration ttrig timeboxes all --run=333510 --trial=1 --label=ttrig_timebox \
	      	    	      	  --runselection=333510 --datasetpath=/Dummy/Dummy/RAW \
				  --globaltag=106X_dataRun3_Express_v2  --datasettype=Cosmics \
				  --run-on-RAW --phase2 --inputT0DB ./t0_run333514.db --input-files-local
```

## Run ntuples with the new t0i and tTrigs
```
cd .. # get back to production directory

cmsRun dtDpgNtuples_slicetest_cfg.py nEvents=50000 runNumber=333510 \
       				     tTrigFilePh2=./calib/Run333510-ttrig_timebox_v1/TimeBoxes/results/ttrig_timeboxes_Run333510_v1.db \
				     t0FilePh2=./calib/t0_run333514.db
```

