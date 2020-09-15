from WMCore.Configuration import Configuration
config = Configuration()

##### Configuration parameters ################################

runNumber = 337240
inputDataset = "/Cosmics/Commissioning2020-v1/RAW"

# These are the cfg parameters used to configure the 
# dtDpgNtuples_slicetest_cfg.py configuration file
configParams = ['ntupleName=DTDPGNtuple.root',
                'tTrigFilePh2=./ttrig_timeboxes_Run337240_v1.db',
                't0FilePh2=./t0_run337214.db'] 
# E.g. use dedicated tTrigs
# configParams = ['ntupleName=DTDPGNtuple.root', \
#                 'tTrigFile=calib/TTrigDB_cosmics_ttrig.db'] 

# These are the additional input files (e.g. sqlite .db) 
# needed by dtDpgNtuples_slicetest_cfg.py to run
inputFiles = ['./ttrig_timeboxes_Run337240_v1.db',
              './t0_run337214.db'] 
# E.g. use dedicated tTrigs
# inputFiles = ['./calib/TTrigDB_cosmics_ttrig.db'] 

###############################################################

config.section_('General')
config.General.workArea = 'crab_jobs'
config.General.requestName = 'DTDPGNtuples_SliceTest_run' + str(runNumber)
config.General.transferOutputs = True

config.section_('JobType')
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = 'dtDpgNtuples_slicetest_cfg.py'

config.JobType.pyCfgParams = configParams
config.JobType.inputFiles  = inputFiles

config.section_('Data')
config.Data.inputDataset = inputDataset

config.Data.splitting    = 'LumiBased'
config.Data.unitsPerJob  = 5  
config.Data.runRange     = str(runNumber)
config.Data.inputDBS     = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
config.Data.outLFNDirBase  = '/store/group/dpg_dt/comm_dt/commissioning_2020_data/crab/'

config.section_('Site')
config.Site.storageSite = 'T2_CH_CERN'

