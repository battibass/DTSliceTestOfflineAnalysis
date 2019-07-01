# DTSliceTestOfflineAnalysis
Package collecting the offline analysis program for the DT slice-test

### Running the analysis:
```
cd analyses
root -b

root [0] .x loadAllAnalyses.C
root [1] .L runAllAnalyses.C
root [2] runAllAnalyses("../../DTDPGNtuple_10_6_0_SX5.root",329614)

```
