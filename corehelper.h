//  .
// ..: P. Chang, philip@physics.ucsd.edu

#ifndef corehelper_h
#define corehelper_h

// C/C++
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <stdarg.h>
#include <functional>
#include <cmath>

// ROOT
#include "TBenchmark.h"
#include "TBits.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TChainElement.h"
#include "TTreeCache.h"
#include "TTreePerfStats.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include "TString.h"
#include "TLorentzVector.h"
#include "Math/LorentzVector.h"

// CORE tools
#include "CORE/CMS3.h"
#include "CORE/Base.h"
#include "CORE/TriggerSelections.h"
#include "CORE/ElectronSelections.h"
#include "CORE/MuonSelections.h"
#include "CORE/JetSelections.h"
#include "CORE/MetSelections.h"
#include "CORE/IsolationTools.h"
#include "CORE/MCSelections.h"
#include "CORE/Tools/goodrun.h"
#include "CORE/Tools/JetCorrector.h"
#include "CORE/Tools/jetcorr/FactorizedJetCorrector.h"
#include "CORE/Tools/jetcorr/JetCorrectionUncertainty.h"
using namespace tas;

// RooUtil
#include "../rooutil/ttreex.h"

//#define MAP std::unordered_map
//#define STRING std::string
#define MAP std::map
#define STRING TString

///////////////////////////////////////////////////////////////////////////////////////////////
// LorentzVector typedef that we use very often
///////////////////////////////////////////////////////////////////////////////////////////////
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LV;
typedef std::vector<LV> LVs;

namespace RooUtil
{

    class CORE2016
    {

        public:
        CORE2016();
        ~CORE2016();

        TString option;

        // stores current corrections for a given event
        FactorizedJetCorrector   * jet_corrector_pfL1FastJetL2L3_current;
        JetCorrectionUncertainty * jecUnc_current;

        // default corrections
        std::vector<std::string> jetcorr_filenames_pfL1FastJetL2L3;
        FactorizedJetCorrector   * jet_corrector_pfL1FastJetL2L3;
        JetCorrectionUncertainty * jecUnc;

        // corrections for later runs in 2016F
        std::vector<std::string> jetcorr_filenames_pfL1FastJetL2L3_postrun278802;
        FactorizedJetCorrector   * jet_corrector_pfL1FastJetL2L3_postrun278802;
        JetCorrectionUncertainty * jecUnc_postrun278802;

        // lepton ids
        std::vector<std::pair<id_level_t, TString>> lepton_ids;

        // triggers
        std::vector<TString> trigger_patterns;
        std::vector<std::pair<TString, TString>> triggers;

        void initializeCORE(TString option);
        static int getCMS3Version();
        void setJetCorrector();
        void createEventBranches(TTreeX* ttree);
        void setEventBranches(TTreeX* ttree);
        void createPileUpBranches(TTreeX* ttree);
        void setPileUpBranches(TTreeX* ttree);
        void createMETBranches(TTreeX* ttree);
        void setMETBranches(TTreeX* ttree);
        void createLeptonBranches(TTreeX* ttree, std::vector<std::pair<id_level_t, TString>>);
        void setLeptonBranches(TTreeX* ttree);
        void setElectronBranches(TTreeX* ttree);
        void setMuonBranches(TTreeX* ttree);
        void createJetBranches(TTreeX* ttree);
        void setJetBranches(TTreeX* ttree);
        void createGenBranches(TTreeX* ttree);
        void setGenBranches(TTreeX* ttree);
        void createFatJetBranches(TTreeX* ttree);
        void setFatJetBranches(TTreeX* ttree);
        void createTrigBranches(TTreeX* ttree, std::vector<TString>);
        void setTrigBranches(TTreeX* ttree);
    };

}

#endif
