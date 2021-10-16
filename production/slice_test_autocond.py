from os import path

base_folders = { "phase1" : "/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2021_data/calib/phase1/",
                 "phase2" : "/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2021_data/calib/phase2/" }

t0is = { "phase1" : "t0_run340746_phase1.db",
         "phase2" : "t0_run340746_phase2.db" }

phase1_ttrigs = { "minidaq_S12_MB3" : "ttrig_timeboxes_run340748_t0_run340746_phase1_trigMB3_corrected.db",
                  "minidaq_S12_MB2" : "ttrig_timeboxes_run341429_t0_run340746_phase1_trigMB2_corrected.db",
                  "minidaq_S12_MB1" : "ttrig_timeboxes_run341432_t0_run340746_phase1_trigMB1_corrected.db",
                  "minidaq_S06_MB1MB4" : "ttrig_timeboxes_run341789_t0_run340746_phase1_trigMB1MB4_S06_corrected.db",
                  "global_BOTTOM_ONLY" : "ttrig_timeboxes_run343498_t0_run340746_phase1_trigBMTF_BOTTOM_ONLY_corrected.db",
                  "global_TOP_ONLY_S06" : "ttrig_timeboxes_run341758_t0_run340746_phase1_trigBMTF_S06_corrected.db" }

phase2_ttrigs = { "minidaq_S12_MB3" : "ttrig_timeboxes_run340748_t0_run340746_phase2_trigMB3_corrected.db",
                  "minidaq_S12_MB2" : "ttrig_timeboxes_run341429_t0_run340746_phase2_trigMB2_corrected.db",
                  "minidaq_S12_MB1" : "ttrig_timeboxes_run341432_t0_run340746_phase2_trigMB1_corrected.db",
                  "minidaq_S06_MB1MB4" : "ttrig_timeboxes_run341789_t0_run340746_phase2_trigMB1MB4_S06_corrected.db",
                  "global_BOTTOM_ONLY" : "ttrig_timeboxes_run343498_t0_run340746_phase2_trigBMTF_BOTTOM_ONLY_corrected.db",
                  "global_TOP_ONLY_S06" : "ttrig_timeboxes_run341758_t0_run340746_phase2_trigBMTF_S06_corrected.db" }

def get_t0i(phase):

    if phase in base_folders and phase in t0is:
        return path.join(base_folders[phase],t0is[phase])

    print("Phase: \'{}\' not in available phases, which are {}".format(phase,t0is.keys()))
    return ""

def get_ttrig(phase,label):

    if phase in base_folders and label in phase1_ttrigs if phase=="phase1" else phase2_ttrigs:
        return path.join(base_folders[phase],(phase1_ttrigs[label] if phase=="phase1" else phase2_ttrigs[label]))

    print("Either phase: \'{}\' or label: \'{}\' not available.".format(phase,label))
    print("  - Available phases: {}".format(base_folders.keys()))
    print("  - Available labels: {}".format(phase1_ttrigs.keys() if phase=="phase1" else phase2_ttrigs.keys()))
    return ""

def labels():
    return phase1_ttrigs.keys() & phase2_ttrigs.keys()

if __name__ == '__main__':

    print(get_t0i("phase1"))
    print(get_t0i("wrong"))

    print(get_ttrig("phase2","minidaq_S12_MB3"))
    print(get_ttrig("wrong","minidaq_S12_MB3"))
    print(get_ttrig("phase2","wrong"))

    print(labels())
