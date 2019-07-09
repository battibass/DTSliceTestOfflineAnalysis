########################################
# Configuration
########################################

INPUTFILE = "106X_dataRun2_v10_cosmics_ttrig.txt"

OUTPUTFILE = "TTrigDB_cosmics_ttrig.db"

# LABEL = ""
LABEL = "cosmics"

########################################

import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("DumpFileToDB")

process.load("CondCore.CondDB.CondDB_cfi")

process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(1),
    firstRun = cms.untracked.uint32(1)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

try:
    os.remove(OUTPUTFILE)
except OSError:
    pass


print "INPUTFILE : ", INPUTFILE

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
                                          process.CondDB,
                                          toPut = cms.VPSet(cms.PSet(record = cms.string("DTTtrigRcd"),
                                                                     label = cms.string(LABEL),
                                                                     tag = cms.string("ttrig")))
                                          )

process.PoolDBOutputService.connect = cms.string("sqlite_file:" + OUTPUTFILE)
                                          
process.dumpToDB = cms.EDAnalyzer("DumpFileToDB",
                                  calibFileConfig = cms.untracked.PSet(
                                      calibConstFileName = cms.untracked.string(INPUTFILE),
                                      calibConstGranularity = cms.untracked.string("bySL"),
                                  ),
                                  dbFormat = cms.untracked.string("Legacy"),
                                  dbToDump = cms.untracked.string("TTrigDB"),
                              )

process.p = cms.Path(process.dumpToDB)
    

