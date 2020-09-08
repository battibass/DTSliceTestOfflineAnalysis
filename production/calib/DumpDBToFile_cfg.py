########################################
# Configuration
########################################

GLOBALTAG = "111X_dataRun3_Prompt_v2"

INPUTFILE = "./Run337261-ttrig_timebox_v1/TimeBoxes/results/ttrig_timeboxes_Run337261_v1.db"
# INPUTFILE = "TTrigDB_Legacy.db"

INPUTTAG=""
#INPUTTAG = "DT_t0_cosmic2009_V01_ModCorr_express"

# LABEL = ""
LABEL = "cosmics"

RUN=337261

OUTPUTFILE = "{}_{}_ttrig.txt".format(RUN,LABEL)

if INPUTTAG!="" and INPUTFILE!="" :
    print 'ERROR!'
    exit

########################################

import FWCore.ParameterSet.Config as cms

process = cms.Process("DumpDBToFile")
process.load("CondCore.CondDB.CondDB_cfi")

process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(1),
    firstRun = cms.untracked.uint32(RUN)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)


print "GT:", GLOBALTAG

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, GLOBALTAG, '')

if INPUTFILE!="" :
    print "Read from: ", INPUTFILE
    process.GlobalTag.toGet = cms.VPSet(
        cms.PSet(record = cms.string("DTTtrigRcd"),
                 tag = cms.string("ttrig"),
                 connect = cms.string("sqlite_file:"+INPUTFILE),
                 label = cms.untracked.string(LABEL)
                 )
        )

if INPUTTAG!="" :
    print "Read from tag: ", INPUTTAG
    process.GlobalTag.toGet = cms.VPSet(
        cms.PSet(record = cms.string("DTTtrigRcd"),
                 tag = cms.string(INPUTTAG),
                 # connect = cms.string('frontier://FrontierProd/CMS_CONDITIONS'),
                 )
        )

process.dumpTTrigToFile = cms.EDAnalyzer("DumpDBToFile",
    dbToDump = cms.untracked.string('TTrigDB'),
    dbLabel = cms.untracked.string(LABEL),
    dbFormat = cms.untracked.string("Legacy"),
    calibFileConfig = cms.untracked.PSet(
        nFields = cms.untracked.int32(8),
        calibConstGranularity = cms.untracked.string('bySL')
    ),
    outputFileName = cms.untracked.string(OUTPUTFILE)
)

process.p = cms.Path(process.dumpTTrigToFile)
