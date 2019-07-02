# DTSliceTestOfflineAnalysis
Package collecting the offline analysis program for the DT slice-test

### Downloading the package:
```
# From your DTNtuple CMSSW setup:
git clone https://github.com/battibass/DTSliceTestOfflineAnalysis.git
```

### Running the analysis:
```
cd analyses
root -b

root [0] .x loadAllAnalyses.C
root [1] .L runAllAnalyses.C
root [2] runAllAnalyses("/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/ntuples/DTDPGNtuple_SX5_run328798.root",328798)

```

### Publishing the plots:
```
. plotAndPublish.sh run328798/
# Type the dtdqm@lxplus ssh password to publish a directory
```
