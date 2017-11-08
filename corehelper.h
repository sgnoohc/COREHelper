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
#include "../CORE/CMS3.h"
#include "../CORE/Base.h"
#include "../CORE/TriggerSelections.h"
#include "../CORE/ElectronSelections.h"
#include "../CORE/MuonSelections.h"
#include "../CORE/JetSelections.h"
#include "../CORE/MetSelections.h"
#include "../CORE/IsolationTools.h"
#include "../CORE/IsoTrackVeto.h"
#include "../CORE/MCSelections.h"
#include "../CORE/Tools/goodrun.h"
#include "../CORE/Tools/JetCorrector.h"
#include "../CORE/Tools/jetcorr/FactorizedJetCorrector.h"
#include "../CORE/Tools/jetcorr/JetCorrectionUncertainty.h"
using namespace tas;

// RooUtil
#include "../rooutil/ttreex.h"
#include "../rooutil/printutil.h"
using namespace RooUtil;

//#define MAP std::unordered_map
//#define STRING std::string
#define MAP std::map
#define STRING TString

///////////////////////////////////////////////////////////////////////////////////////////////
// LorentzVector typedef that we use very often
///////////////////////////////////////////////////////////////////////////////////////////////
typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LV;
typedef std::vector<LV> LVs;

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

        bool isCMSX(int v) { if (cms3.evt_CMS3tag()[0].Contains(Form("CMS%d", v))) return true; else return false; }

        void initializeCORE(TString option);
        static int getCMS3Version();
        void setJetCorrector();
        void createEventBranches(RooUtil::TTreeX* ttree);
        void setEventBranches(RooUtil::TTreeX* ttree);
        void createPileUpBranches(RooUtil::TTreeX* ttree);
        void setPileUpBranches(RooUtil::TTreeX* ttree);
        void createMETBranches(RooUtil::TTreeX* ttree);
        void setMETBranches(RooUtil::TTreeX* ttree);
        void createLeptonBranches(RooUtil::TTreeX* ttree, std::vector<std::pair<id_level_t, TString>>);
        void setLeptonBranches(RooUtil::TTreeX* ttree);
        void setElectronBranches(RooUtil::TTreeX* ttree);
        void setMuonBranches(RooUtil::TTreeX* ttree);
        void createIsoTrackBranches(RooUtil::TTreeX* ttree);
        void setIsoTrackBranches(RooUtil::TTreeX* ttree);
        void createJetBranches(RooUtil::TTreeX* ttree);
        void setJetBranches(RooUtil::TTreeX* ttree);
        void createGenBranches(RooUtil::TTreeX* ttree);
        void setGenBranches(RooUtil::TTreeX* ttree);
        void createFatJetBranches(RooUtil::TTreeX* ttree);
        void setFatJetBranches(RooUtil::TTreeX* ttree);
        void createTrigBranches(RooUtil::TTreeX* ttree, std::vector<TString>);
        void setTrigBranches(RooUtil::TTreeX* ttree);

        int nCount(RooUtil::TTreeX* ttree, TString bn);
};

#endif
