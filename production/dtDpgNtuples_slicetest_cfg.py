import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras

import lxml.etree as etree
import subprocess
import sys
import os

XML_FOLDER = "./cmsrun_xml/"
HAS_AUTOCOND = os.path.isfile("./slice_test_autocond.py")

if HAS_AUTOCOND:
    import slice_test_autocond as autocond

def appendToGlobalTag(process, rcd, tag, fileName, label) :

    if  not fileName :
        return process

    if not hasattr(process.GlobalTag,"toGet") :
        process.GlobalTag.toGet = cms.VPSet()

    process.GlobalTag.toGet.append(
        cms.PSet(tag = cms.string(tag),
                 record = cms.string(rcd),
                 connect = cms.string("sqlite_file:" + fileName),
                 label = cms.untracked.string(label)
             )
    )

    return process

options = VarParsing.VarParsing()

options.register('globalTag',
                 '122X_dataRun3_Prompt_v3', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Global Tag")

options.register('nEvents',
                 -1, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Maximum number of processed events")

options.register('runNumber',
                 '347683', #default value
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.int,
                 "Run number to be looked for in either 'inputFolderCentral' or 'inputFolderDT' folders")

options.register('inputFile',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "The input file to be processed, if non null overrides runNumber based input file selection")

options.register('inputFolderCentral',
                 '/eos/cms/store/data/Commissioning2022/MiniDaq/RAW/v1/', #default value
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                 "Base EOS folder with input files from MiniDAQ/Global runs with central tier0 transfer")

options.register('inputFolderDT',
                 '/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2022_data/root/', #default value
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                 "Base EOS folder with input files from MiniDAQ runs with DT 'private' tier0 transfer")

options.register('tTrigFile',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT legacy tTrigs, used only if non ''")

options.register('t0File',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT legacy t0is, used only if non ''")

options.register('tTrigFilePh2',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT phase-2 tTrigs, used only if non ''")

options.register('t0FilePh2',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT phase-2 t0is, used only if non ''")

options.register('vDriftFile',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT vDrifts, used only if non ''")

options.register('runOnDat',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "If set to True switches source from 'PoolSource' to 'NewEventStreamFileReader'")

options.register('runOnTestPulse',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "If set to True switches off the filling of all collections but digis")

options.register('ntupleName',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Folder and name ame for output ntuple, if non null overrides 'standard' naming based on runNumber option")

if HAS_AUTOCOND:
    options.register('autocond',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Autocond label: valid ones are {}".format(autocond.labels()))

options.parseArguments()

process = cms.Process("DTNTUPLES",eras.Run3)

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.nEvents))

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = cms.string(options.globalTag)

goodAutocond = HAS_AUTOCOND and options.autocond

tTrigFile = autocond.get_ttrig("phase1",options.autocond) \
            if (goodAutocond and not options.tTrigFile) else options.tTrigFile
tTrigFilePh2 = autocond.get_ttrig("phase2",options.autocond) \
               if (goodAutocond and not options.tTrigFilePh2) else options.tTrigFilePh2
t0File = autocond.get_t0i("phase1") if (goodAutocond and not options.t0File) else options.t0File
t0FilePh2 = autocond.get_t0i("phase2") if (goodAutocond and not options.t0FilePh2) else options.t0FilePh2
    
process = appendToGlobalTag(process, "DTTtrigRcd", "ttrig", tTrigFile, "cosmics")
process = appendToGlobalTag(process, "DTT0Rcd", "t0", t0File, "")

process = appendToGlobalTag(process, "DTTtrigRcd", "ttrig", tTrigFilePh2, "cosmics_ph2")
process = appendToGlobalTag(process, "DTT0Rcd", "t0", t0FilePh2, "ph2")

process = appendToGlobalTag(process, "DTMtimeRcd", "vDrift", options.vDriftFile, "")

process.source = cms.Source("NewEventStreamFileReader" if options.runOnDat else "PoolSource",
                            
        fileNames = cms.untracked.vstring()
)

if options.inputFile :

    print('[dtDpgNtuples_slicetest_cfg.py]: inputFile parameter is non-null running on file:\n\t\t\t{}'.format(options.inputFile))
    process.source.fileNames = cms.untracked.vstring("file://" + options.inputFile)

else :

    runStr = str(options.runNumber).zfill(9)
    runFolder = options.inputFolderCentral + "/" + runStr[0:3] + "/" + runStr[3:6] + "/" + runStr[6:] 
    if not options.runOnDat:
        runFolder = runFolder + "/00000"
    
    print('[dtDpgNtuples_slicetest_cfg.py]: looking for files under:\n\t\t\t{}'.format(runFolder))
    
    if os.path.exists(runFolder) :
        files = subprocess.check_output(["ls", runFolder])
        process.source.fileNames = ["file://" + runFolder + "/" + f.decode("utf-8") for f in files.split()]

    else :
        print('[dtDpgNtuples_slicetest_cfg.py]: files not found there, looking under:\n\t\t\t{}'.format(options.inputFolderDT))

        files = subprocess.check_output(["ls", options.inputFolderDT])
        filesFromRun = [f.decode("utf-8") for f in files.split() if f.find(runStr[3:]) > -1]

        if len(filesFromRun) == 1 :
            process.source.fileNames.append("file://" + options.inputFolderDT + "/" + filesFromRun[0])

        else :
            print('[dtDpgNtuples_slicetest_cfg.py]: {} files found, can\'t run!'.format(len(filesFromRun)))
            sys.exit(999)

print(process.source.fileNames)

ntupleName = options.ntupleName if options.ntupleName else "./DTDPGNtuple_run" + str(options.runNumber) + ".root"  

process.TFileService = cms.Service('TFileService',
        fileName = cms.string(ntupleName)
    )

process.load('Configuration/StandardSequences/GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('EventFilter.DTRawToDigi.dtab7unpacker_cfi')

process.load('DTDPGAnalysis.DTNtuples.dtUpgradeFedL1AProducer_cfi')

process.load('RecoLocalMuon.Configuration.RecoLocalMuonCosmics_cff')

process.load('DTDPGAnalysis.DTNtuples.dtNtupleProducer_slicetest_cfi')

process.p = cms.Path(process.muonDTDigis
                     + process.dtAB7unpacker
                     + process.dtUpgradeFedL1AProducer
                     + process.twinMuxStage2Digis
                     + process.bmtfDigis
                     + process.dtlocalrecoT0Seg
                     + process.dtNtupleProducer)

if tTrigFilePh2 and t0FilePh2 :
    from DTDPGAnalysis.DTNtuples.customiseDtPhase2Reco_cff import customiseForPhase2Reco
    process = customiseForPhase2Reco(process,"p", tTrigFilePh2, t0FilePh2)

    from DTDPGAnalysis.DTNtuples.customiseDtPhase2Emulator_cff import customiseForPhase2Emulator
    process = customiseForPhase2Emulator(process,"p")

if options.runOnTestPulse :
    from DTDPGAnalysis.DTNtuples.customiseDtNtuples_cff import customiseForTestPulseRun
    process = customiseForTestPulseRun(process)

xml_base = etree.Element("options") 

for var, val in options._singletons.items():
    if var == "ntupleName":
        etree.SubElement(xml_base, var).text = os.path.abspath(ntupleName)
    elif var == "vDriftFile" and val != "":
        etree.SubElement(xml_base, var).text = os.path.abspath(val)
    elif "File" in var:
        continue
    else:
        etree.SubElement(xml_base, var).text = str(val)
        
if t0File != "":
    etree.SubElement(xml_base, "t0File").text = os.path.abspath(t0File)
if t0FilePh2 != "":
    etree.SubElement(xml_base, "t0FilePh2").text = os.path.abspath(t0FilePh2)
if tTrigFile != "":
    etree.SubElement(xml_base, "tTrigFile").text = os.path.abspath(tTrigFile)
if tTrigFilePh2 != "":
    etree.SubElement(xml_base, "tTrigFilePh2").text = os.path.abspath(tTrigFilePh2)

if not os.path.exists(XML_FOLDER):
    os.makedirs(XML_FOLDER)

xml_string = etree.tostring(xml_base, pretty_print=True)

out_file_name = "ntuple_cfg_run" + str(options.runNumber) + ".xml"
out_file_path = os.path.join(XML_FOLDER, out_file_name)

with open(out_file_path, 'w') as file: file.write(xml_string.decode("utf-8"))
