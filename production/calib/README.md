# DTSliceTestOfflineAnalysis / production / calib
A few instructions about how to run calibrations

## Run t0i production for phase-2
```
# Print command line options
python dtT0WireCalibration_cfg.py --help 

# Get t0i out of a specific test-pulse run
cmsRun dtT0WireCalibration_cfg.py runNumber=336977
```

## Run tTrig production
```
# Run the tTrig production workflow
# using t0 from the previous step (./t0_run336977.db)
dtCalibration ttrig timeboxes all --run=336978 --trial=1 --label=ttrig_timebox \
	      	    	      	  --runselection=336978 --datasetpath=/Dummy/Dummy/RAW \
				  --globaltag=111X_dataRun3_Prompt_v2  --datasettype=Cosmics \
				  --run-on-RAW --phase2 --inputT0DB ./t0_run336977.db --input-files-local
```

## Run ntuples with the new t0i and tTrigs
```
cd .. # get back to production directory

cmsRun dtDpgNtuples_slicetest_cfg.py nEvents=50000 runNumber=336978 \
       				     tTrigFilePh2=./calib/Run336978-ttrig_timebox_v1/TimeBoxes/results/ttrig_timeboxes_Run336978_v1.db \
				     t0FilePh2=./calib/t0_run336977.db
```

