import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras

import subprocess
import sys

options = VarParsing.VarParsing()

options.register('globalTag',
                 '106X_dataRun2_v10', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Global Tag")

options.register('nEvents',
                 -1, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Maximum number of processed events")

options.register('inputFile',
                 '/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/root/run329614_streamDQM_fu-c2f13-09-03.root', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "The input file to be processed")

options.register('inputFolder',
                 '', #default value
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                 "EOS folder with input files")

options.register('tTrigFile',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT tTrigs, used only if non ''")

options.register('ntupleName',
                 './DTDPGNtuple_10_6_0_SX5.root', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Folder and name ame for output ntuple")


options.parseArguments()

process = cms.Process("DTNTUPLES",eras.Run2_2018)

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.nEvents))

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.GlobalTag.globaltag = cms.string(options.globalTag)

if options.tTrigFile != '' :

    process.GlobalTag.toGet = cms.VPSet(
        cms.PSet(record = cms.string("DTTtrigRcd"),
                 tag = cms.string("ttrig"),
                 connect = cms.string("sqlite_file:" + options.tTrigFile),
                 label = cms.untracked.string("cosmics")
                 )
        )

process.source = cms.Source("PoolSource",
                            
        fileNames = cms.untracked.vstring(""),
        secondaryFileNames = cms.untracked.vstring()

)

if (options.inputFile == '' and options.inputFolder == '') or \
   (options.inputFile != '' and options.inputFolder != '') :

    print "[dtDpgNtuples_slicetest_cfg.py]: inputFile and inputFolder can be non-null only one at a time. quitting."
    sys.exit(999)
    
if options.inputFile != "" :
    process.source.fileNames = cms.untracked.vstring("file://" + options.inputFile)

if options.inputFolder != "" :
    files = subprocess.check_output(["ls", options.inputFolder])
    process.source.fileNames = ["file://" + options.inputFolder + "/" + f for f in files.split()]

process.TFileService = cms.Service('TFileService',
        fileName = cms.string(options.ntupleName)
    )

process.load('Configuration/StandardSequences/GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('EventFilter.DTRawToDigi.dtab7unpacker_cfi')

process.dtAB7unpacker.channelMapping = cms.untracked.string("july2019")

process.load('RecoLocalMuon.Configuration.RecoLocalMuonCosmics_cff')

process.load('DTDPGAnalysis.DTNtuples.dtNtupleProducer_slicetest_cfi')

process.p = cms.Path(process.muonDTDigis
                     + process.dtAB7unpacker
                     + process.dtlocalrecoT0Seg
                     + process.dtNtupleProducer)



