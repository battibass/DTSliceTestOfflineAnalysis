import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import os

########################################
# Configuration
########################################

options = VarParsing.VarParsing()

options.register('runNumber',
                 1, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Run number")

options.register('inputFile',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT tTrigs")

options.register('label',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "label '' = collisions, 'cosmics' = cosmics")

options.parseArguments()

########################################

process = cms.Process("DumpFileToDB")

process.load("CondCore.CondDB.CondDB_cfi")

process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(1),
    firstRun = cms.untracked.uint32(options.runNumber)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)

print(f"INPUTFILE : {options.inputFile}")

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
                                          process.CondDB,
                                          toPut = cms.VPSet(cms.PSet(record = cms.string("DTTtrigRcd"),
                                                                     label = cms.string(options.label),
                                                                     tag = cms.string("ttrig")))
                                          )

OUTPUTFILE = options.inputFile.replace(".txt",".db")

try:
    os.remove(OUTPUTFILE)
except OSError:
    pass

process.PoolDBOutputService.connect = cms.string("sqlite_file:" + OUTPUTFILE)
                                          
process.dumpToDB = cms.EDAnalyzer("DumpFileToDB",
                                  calibFileConfig = cms.untracked.PSet(
                                      calibConstFileName = cms.untracked.string(options.inputFile),
                                      calibConstGranularity = cms.untracked.string("bySL"),
                                  ),
                                  dbFormat = cms.untracked.string("Legacy"),
                                  dbToDump = cms.untracked.string("TTrigDB"),
                              )

process.p = cms.Path(process.dumpToDB)
    

