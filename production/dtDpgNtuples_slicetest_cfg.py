import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras

import subprocess
import sys
import os

options = VarParsing.VarParsing()

options.register('globalTag',
                 '106X_dataRun3_Express_v2', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Global Tag")

options.register('nEvents',
                 -1, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Maximum number of processed events")

options.register('runNumber',
                 '329806', #default value
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.int,
                 "Run number to be looked for in either 'inputFolderCentral' or 'inputFolderDT' folders")

options.register('inputFile',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "The input file to be processed, if non null overrides runNumber based input file selection")

options.register('inputFolderCentral',
                 '/eos/cms/store/data/Commissioning2019/MiniDaq/RAW/v1/', #default value
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                 "Base EOS folder with input files from MiniDAQ runs with central tier0 transfer")

options.register('inputFolderDT',
                 '/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/root/', #default value
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                 "Base EOS folder with input files from MiniDAQ runs with DT 'private' tier0 transfer")

options.register('tTrigFile',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT tTrigs, used only if non ''")

options.register('t0File',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT t0is, used only if non ''")

options.register('vDriftFile',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT vDrifts, used only if non ''")

options.register('ntupleName',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Folder and name ame for output ntuple, if non null overrides 'standard' naming based on runNumber option")


options.parseArguments()

process = cms.Process("DTNTUPLES",eras.Run2_2018)

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.nEvents))

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.GlobalTag.globaltag = cms.string(options.globalTag)

if options.tTrigFile  != '' or \
   options.vDriftFile != '' or \
   options.t0File != '' :
    process.GlobalTag.toGet = cms.VPSet()

if options.tTrigFile != '' :
    process.GlobalTag.toGet.append(cms.PSet(record = cms.string("DTTtrigRcd"),
                                            tag = cms.string("ttrig"),
                                            connect = cms.string("sqlite_file:" + options.tTrigFile),
                                            label = cms.untracked.string("cosmics")
                                        )
                               )

if options.vDriftFile != '' :
    process.GlobalTag.toGet.append(cms.PSet(record = cms.string("DTMtimeRcd"),
                                            tag = cms.string("vDrift"),
                                            connect = cms.string("sqlite_file:" + options.vDriftFile)
                                        )
                                   )

if options.t0File != '' :
    process.GlobalTag.toGet.append(cms.PSet(record = cms.string("DTT0Rcd"),
                                            tag = cms.string("t0"),
                                            connect = cms.string("sqlite_file:" + options.t0File)
                                        )
                                   )
    

process.source = cms.Source("PoolSource",
                            
        fileNames = cms.untracked.vstring(),
        secondaryFileNames = cms.untracked.vstring()

)

if options.inputFile != '' :

    print "[dtDpgNtuples_slicetest_cfg.py]: inputFile parameter is non-null running on file:\n\t\t\t" + options.inputFile
    process.source.fileNames = cms.untracked.vstring("file://" + options.inputFile)

else :

    runStr = str(options.runNumber).zfill(9)
    runFolder = options.inputFolderCentral + "/" + runStr[0:3] + "/" + runStr[3:6] + "/" + runStr[6:] + "/00000"

    print "[dtDpgNtuples_slicetest_cfg.py]: looking for files under:\n\t\t\t" + runFolder
    
    if os.path.exists(runFolder) :
        files = subprocess.check_output(["ls", runFolder])
        process.source.fileNames = ["file://" + runFolder + "/" + f for f in files.split()]

    else :
        print "[dtDpgNtuples_slicetest_cfg.py]: files not found there, looking under:\n\t\t\t" + options.inputFolderDT

        files = subprocess.check_output(["ls", options.inputFolderDT])
        filesFromRun = []

        for f in files.split() :
            if f.find(runStr[3:]) > -1 :
                filesFromRun.append(f)

        if len(filesFromRun) == 1 :
            process.source.fileNames.append("file://" + options.inputFolderDT + "/" + filesFromRun[0])

        else :
            print "[dtDpgNtuples_slicetest_cfg.py]: " + str(len(filesFromRun)) + " files found, can't run!"
            sys.exit(999)

print process.source.fileNames

if options.ntupleName == '' :
    ntupleName = "./DTDPGNtuple_run" + str(options.runNumber) + ".root"
else :
    ntupleName = options.ntupleName

process.TFileService = cms.Service('TFileService',
        fileName = cms.string(ntupleName)
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
                     + process.twinMuxStage2Digis
                     + process.bmtfDigis
                     + process.dtlocalrecoT0Seg
                     + process.dtNtupleProducer)
