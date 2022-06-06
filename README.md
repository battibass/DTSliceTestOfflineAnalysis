# DTSliceTestOfflineAnalysis
Package collecting the offline analysis programs for the DT slice-test

## Downloading the package and setting up the environment:

From your favourite CMSSW area `src/` folder:
```bash
git clone https://github.com/battibass/DTSliceTestOfflineAnalysis.git DTDPGAnalysis/DTSliceTestOfflineAnalysis -b poa

cmsenv
scramv1 b -j 5
```

This makes the analysis command and the plotting one available "everywhere"
after a `cmsenv`

## Running the analysis and publishing plots

```bash
runSliceTestAnalysis 351176 /eos/cms/store/group/dpg_dt/comm_dt/commissioning_2022_data/ntuples/ST/DTDPGNtuple_run351176_skim_seg_in_slicetest_v1.root 

plotAndPublish.sh run351176
```
