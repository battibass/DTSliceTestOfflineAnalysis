import yaml
from WMCore.Configuration import Configuration
config = Configuration()

json_data = []

with open('./crab_jsons/crab.json', 'r') as json_file:
    json_data = yaml.safe_load(json_file)

config.section_('General')
config.General.workArea = 'crab_jobs'
config.General.requestName = 'DTDPGNtuples_SliceTest_run' \
                             + json_data["runNumber"] + "_" \
                             + json_data["version"]
config.General.transferOutputs = True

config.section_('JobType')
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = 'dtDpgNtuples_slicetest_cfg.py'

config.JobType.pyCfgParams = json_data["config_params"]
config.JobType.inputFiles  = json_data["input_files"]

config.section_('Data')
config.Data.inputDataset = json_data["dataset"]

config.Data.splitting    = 'LumiBased'
config.Data.unitsPerJob  = 10  
config.Data.runRange     = json_data["runNumber"]
config.Data.inputDBS     = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
config.Data.outLFNDirBase  = json_data["outLFNBase"]
 
config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'

print config
