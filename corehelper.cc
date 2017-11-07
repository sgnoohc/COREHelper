//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "corehelper.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// CORE Helper
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// This is meant to be passed as the third argument, the predicate, of the standard library sort algorithm
inline bool sortByValue( const std::pair<int, float>& pair1, const std::pair<int, float>& pair2 )
{
    return pair1.second > pair2.second;
}


//_________________________________________________________________________________________________
CORE2016::CORE2016()
{
    jet_corrector_pfL1FastJetL2L3_current       = NULL;
    jecUnc_current                              = NULL;
    jet_corrector_pfL1FastJetL2L3               = NULL;
    jecUnc                                      = NULL;
    jet_corrector_pfL1FastJetL2L3_postrun278802 = NULL;
    jecUnc_postrun278802                        = NULL;
}

//_________________________________________________________________________________________________
CORE2016::~CORE2016()
{
}

//_________________________________________________________________________________________________
void CORE2016::initializeCORE(TString option_)
{

    // -~-~-~-~-~-
    // Set options
    // -~-~-~-~-~-
    option = option_;

    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    // If ANALYSIS_BASE is not set set it to current directory
    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    if (TString(gSystem->Getenv("ANALYSIS_BASE")).IsNull())
        gSystem->Setenv("ANALYSIS_BASE", ".");

    // -~-~-~-~-~-~-~-~
    // Electron ID tool
    // -~-~-~-~-~-~-~-~
    RooUtil::print( "Creating MVA input for electrons.", __FUNCTION__ );
    createAndInitMVA((TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "CORE").Data(), true, false, 80); // for electrons

    // -~-~-~-~-~-~-~
    // Good Runs List
    // -~-~-~-~-~-~-~
    TString json_file = TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/goodRunsList/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_snt.txt"; // 26p4 fb
    RooUtil::print( "Setting grl:" + json_file, __FUNCTION__ );
    set_goodrun_file( json_file.Data() );

    // -~-~-~-~-~-~
    // b-tagging SF
    // -~-~-~-~-~-~
    // TODO

    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    // retrieve JEC from files, if using
    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

    if ( option.Contains("applyJEC") )
    {
        jetcorr_filenames_pfL1FastJetL2L3.clear();

        //JECs for 76X
        if ( option.Contains( "16Dec2015" ) || option.Contains( "76X_mcRun2" ) )
        {
            if ( option.Contains( "Run2015C" ) || option.Contains( "Run2015D" ) )
            {
                jetcorr_filenames_pfL1FastJetL2L3.clear();
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_76X/DATA/Fall15_25nsV2_DATA_L1FastJet_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_76X/DATA/Fall15_25nsV2_DATA_L2Relative_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_76X/DATA/Fall15_25nsV2_DATA_L3Absolute_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_76X/DATA/Fall15_25nsV2_DATA_L2L3Residual_AK4PFchs.txt").Data() );
                jecUnc = new JetCorrectionUncertainty( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_76X/DATA/Fall15_25nsV2_DATA_Uncertainty_AK4PFchs.txt").Data() );
            }
            else
            {
                // files for 76X MC
                jetcorr_filenames_pfL1FastJetL2L3.clear();
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_76X/MC/Fall15_25nsV2_MC_L1FastJet_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_76X/MC/Fall15_25nsV2_MC_L2Relative_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_76X/MC/Fall15_25nsV2_MC_L3Absolute_AK4PFchs.txt").Data() );
                jecUnc = new JetCorrectionUncertainty( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_76X/MC/Fall15_25nsV2_MC_Uncertainty_AK4PFchs.txt").Data() );
            }
        }
        else if ( option.Contains( "80MiniAODv" ) || option.Contains( "RelVal" ) )
        {
            // files for 80X MC, ICHEP production
            jetcorr_filenames_pfL1FastJetL2L3.clear();
            jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/MC/Spring16_25nsV1_MC_L1FastJet_AK4PFchs.txt").Data() );
            jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/MC/Spring16_25nsV1_MC_L2Relative_AK4PFchs.txt").Data() );
            jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/MC/Spring16_25nsV1_MC_L3Absolute_AK4PFchs.txt").Data() );
            jecUnc = new JetCorrectionUncertainty( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/MC/Spring16_25nsV1_MC_Uncertainty_AK4PFchs.txt").Data() );
        }
        else if ( option.Contains( "Summer16" ) || option.Contains( "TEST" ) )
        {
            // files for 80X MC, Summer16 (Moriond17) production
            jetcorr_filenames_pfL1FastJetL2L3.clear();
            jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/MC/Summer16_23Sep2016V3_MC_L1FastJet_AK4PFchs.txt").Data() );
            jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/MC/Summer16_23Sep2016V3_MC_L2Relative_AK4PFchs.txt").Data() );
            jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/MC/Summer16_23Sep2016V3_MC_L3Absolute_AK4PFchs.txt").Data() );
            jecUnc = new
                JetCorrectionUncertainty( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/MC/Summer16_23Sep2016V3_MC_Uncertainty_AK4PFchs.txt").Data() );
        }
        else if ( option.Contains( "Run2016" ) || option.Contains( "CMSSW_8_0_11_V08-00-06" ) )
        {

            // // This scheme will eventually be used for Moriond. For now just use JECs from ICHEP
            if ( option.Contains( "Run2016B" ) || option.Contains( "Run2016C" ) || option.Contains( "Run2016D" ) )
            {
                // files for 80X Data
                jetcorr_filenames_pfL1FastJetL2L3.clear();
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016BCDV3_DATA_L1FastJet_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016BCDV3_DATA_L2Relative_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016BCDV3_DATA_L3Absolute_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016BCDV3_DATA_L2L3Residual_AK4PFchs.txt").Data() );
                jecUnc = new
                    JetCorrectionUncertainty( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016BCDV3_DATA_Uncertainty_AK4PFchs.txt").Data() );
            }

            if ( option.Contains( "Run2016E" ) || option.Contains( "Run2016F" ) )
            {
                // files for 80X Data
                jetcorr_filenames_pfL1FastJetL2L3.clear();
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016EFV3_DATA_L1FastJet_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016EFV3_DATA_L2Relative_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016EFV3_DATA_L3Absolute_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016EFV3_DATA_L2L3Residual_AK4PFchs.txt").Data() );
                jecUnc = new
                    JetCorrectionUncertainty( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016EFV3_DATA_Uncertainty_AK4PFchs.txt").Data() );

                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.clear();
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L1FastJet_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L2Relative_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L3Absolute_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L2L3Residual_AK4PFchs.txt").Data() );
                jecUnc_postrun278802 = new
                    JetCorrectionUncertainty( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_Uncertainty_AK4PFchs.txt").Data() );
                jet_corrector_pfL1FastJetL2L3_postrun278802  = makeJetCorrector( jetcorr_filenames_pfL1FastJetL2L3_postrun278802 );
            }

            if ( option.Contains( "Run2016G" ) )
            {
                // files for 80X Data
                jetcorr_filenames_pfL1FastJetL2L3.clear();
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L1FastJet_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L2Relative_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L3Absolute_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L2L3Residual_AK4PFchs.txt").Data() );
                jecUnc = new
                    JetCorrectionUncertainty( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_Uncertainty_AK4PFchs.txt").Data() );

                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.clear();
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L1FastJet_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L2Relative_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L3Absolute_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L2L3Residual_AK4PFchs.txt").Data() );
                jecUnc_postrun278802 = new
                    JetCorrectionUncertainty( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_Uncertainty_AK4PFchs.txt").Data() );
                jet_corrector_pfL1FastJetL2L3_postrun278802  = makeJetCorrector( jetcorr_filenames_pfL1FastJetL2L3_postrun278802 );
            }

            if ( option.Contains( "Run2016H" ) )
            {
                jetcorr_filenames_pfL1FastJetL2L3.clear();
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L1FastJet_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L2Relative_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L3Absolute_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L2L3Residual_AK4PFchs.txt").Data() );
                jecUnc = new
                    JetCorrectionUncertainty( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_Uncertainty_AK4PFchs.txt").Data() );

                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.clear();
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L1FastJet_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L2Relative_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L3Absolute_AK4PFchs.txt").Data() );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L2L3Residual_AK4PFchs.txt").Data() );
                jecUnc_postrun278802 = new
                    JetCorrectionUncertainty( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_Uncertainty_AK4PFchs.txt").Data() );
                jet_corrector_pfL1FastJetL2L3_postrun278802  = makeJetCorrector( jetcorr_filenames_pfL1FastJetL2L3_postrun278802 );
            }
        }

        if ( option.Contains("isFastSim") )
        {
            // files for 25ns fastsim samples
            jetcorr_filenames_pfL1FastJetL2L3.clear();
            jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/FASTSIM/Spring16_FastSimV1_L1FastJet_AK4PFchs.txt").Data() );
            jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/FASTSIM/Spring16_FastSimV1_L2Relative_AK4PFchs.txt").Data() );
            jetcorr_filenames_pfL1FastJetL2L3.push_back( (TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" + "COREHelper/configs/jetCorrections/source_80X/FASTSIM/Spring16_FastSimV1_L3Absolute_AK4PFchs.txt").Data() );
            jecUnc = new
                JetCorrectionUncertainty((TString(gSystem->Getenv("ANALYSIS_BASE")) + "/" +  "COREHelper/configs/jetCorrections/source_80X/FASTSIM/Spring16_FastSimV1_Uncertainty_AK4PFchs.txt").Data() );
        }

        if ( jetcorr_filenames_pfL1FastJetL2L3.size() == 0 )
            error("JECs are not set properly. Check the JECs.", __FUNCTION__);

        print("JECs used:", __FUNCTION__);

        for ( size_t jecind = 0; jecind < jetcorr_filenames_pfL1FastJetL2L3.size(); jecind++ )
            print( TString( jetcorr_filenames_pfL1FastJetL2L3.at( jecind ) ), __FUNCTION__ );

        jet_corrector_pfL1FastJetL2L3  = makeJetCorrector( jetcorr_filenames_pfL1FastJetL2L3 );
    }

}

//_________________________________________________________________________________________________
int CORE2016::getCMS3Version()
{
    TString cms3_version = cms3.evt_CMS3tag()[0];
    // convert last two digits of version number to int
    int small_cms3_version = TString(cms3_version(cms3_version.Length()-2,cms3_version.Length())).Atoi();
    return small_cms3_version;
}

//_________________________________________________________________________________________________
void CORE2016::setJetCorrector()
{
    // set jet corrector based on run number for data
    if (cms3.evt_isRealData() && cms3.evt_run() >= 278802 && cms3.evt_run() <= 278808) {
        jet_corrector_pfL1FastJetL2L3_current = jet_corrector_pfL1FastJetL2L3_postrun278802;
        jecUnc_current = jecUnc_postrun278802;
    }
    else {
        jet_corrector_pfL1FastJetL2L3_current = jet_corrector_pfL1FastJetL2L3;
        jecUnc_current = jecUnc;
    }
}

//_________________________________________________________________________________________________
void CORE2016::createEventBranches(RooUtil::TTreeX* ttree)
{
    // Event info related
    ttree->createBranch<Int_t   >( "evt_run" );
    ttree->createBranch<Int_t   >( "evt_lumiBlock" );
    ttree->createBranch<Int_t   >( "evt_event" );
    ttree->createBranch<Int_t   >( "evt_isRealData" );
    ttree->createBranch<Int_t   >( "evt_passgoodrunlist" );
    ttree->createBranch<Float_t >( "genps_weight" );
//    ttree->createBranch<Float_t >( "evt_scale1fb" );
//    ttree->createBranch<Float_t >( "evt_xsec" );
//    ttree->createBranch<Float_t >( "evt_kfactor" );
//    ttree->createBranch<Float_t >( "evt_filt_eff" );
}

//_________________________________________________________________________________________________
void CORE2016::setEventBranches(RooUtil::TTreeX* ttree)
{
    // Event info related
    ttree->setBranch<Int_t   >( "evt_run", cms3.evt_run() );
    ttree->setBranch<Int_t   >( "evt_lumiBlock", cms3.evt_lumiBlock() );
    ttree->setBranch<Int_t   >( "evt_event", cms3.evt_event() );
    ttree->setBranch<Int_t   >( "evt_isRealData", cms3.evt_isRealData() );
    ttree->setBranch<Int_t   >( "evt_passgoodrunlist", cms3.evt_isRealData() ? goodrun(cms3.evt_run(), cms3.evt_lumiBlock()) : 1. );
    ttree->setBranch<Float_t >( "genps_weight", cms3.genps_weight() );
//    ttree->setBranch<Float_t >( "evt_scale1fb", cms3.evt_scale1fb() );
//    ttree->setBranch<Float_t >( "evt_xsec", cms3.evt_xsec_incl() );
//    ttree->setBranch<Float_t >( "evt_kfactor", cms3.evt_kfactor() );
//    ttree->setBranch<Float_t >( "evt_filt_eff", cms3.evt_filt_eff() );
}

//_________________________________________________________________________________________________
void CORE2016::createPileUpBranches(RooUtil::TTreeX* ttree)
{
    // PileUp info related
    ttree->createBranch<Float_t >( "evt_rho" );
}

//_________________________________________________________________________________________________
void CORE2016::setPileUpBranches(RooUtil::TTreeX* ttree)
{
    // PileUp info related
    ttree->setBranch<Float_t >( "evt_rho", cms3.evt_fixgridfastjet_all_rho() );
}

//_________________________________________________________________________________________________
void CORE2016::createMETBranches(RooUtil::TTreeX* ttree)
{
    // MET variables
    ttree->createBranch<Float_t >( "gen_met" );
    ttree->createBranch<Float_t >( "gen_metPhi" );
    ttree->createBranch<Float_t >( "met_pt" );
    ttree->createBranch<Float_t >( "met_phi" );
}

//_________________________________________________________________________________________________
void CORE2016::setMETBranches(RooUtil::TTreeX* ttree)
{
    // MET variables
    ttree->setBranch<Float_t >( "gen_met", cms3.gen_met() );
    ttree->setBranch<Float_t >( "gen_metPhi", cms3.gen_metPhi() );
    ttree->setBranch<Float_t >( "met_pt", cms3.evt_pfmet() );
    ttree->setBranch<Float_t >( "met_phi", cms3.evt_pfmetPhi() );

    if ( option.Contains( "applyJEC" ) )
    {
        setJetCorrector();

        if ( cms3.evt_isRealData() && getCMS3Version() >= 18 )
        {
            ttree->setBranch<Float_t >( "met_pt", cms3.evt_muegclean_pfmet() );
            ttree->setBranch<Float_t >( "met_phi", cms3.evt_muegclean_pfmetPhi() );
        }
        else
        {
            // met with no unc
            pair <float, float> met_T1CHS_miniAOD_CORE_p2 = getT1CHSMET_fromMINIAOD( jet_corrector_pfL1FastJetL2L3_current );
            ttree->setBranch<Float_t >( "met_pt", met_T1CHS_miniAOD_CORE_p2.first );
            ttree->setBranch<Float_t >( "met_phi", met_T1CHS_miniAOD_CORE_p2.second );
        }
    }
}

//_________________________________________________________________________________________________
void CORE2016::createLeptonBranches(RooUtil::TTreeX* ttree, std::vector<std::pair<id_level_t, TString>> ids)
{

    // four vectors of the leptons
    // pdgid
    // and pass_X
    // Auxiliary variable will be an option

    // Set the lepton_ids
    lepton_ids = ids;

    ttree->createBranch<std::vector<LV     >>( "lep_p4"    );
    ttree->createBranch<std::vector<Int_t  >>( "lep_pdgId" );
    ttree->createBranch<std::vector<Float_t>>( "lep_bdt_muiso_var11" );
    ttree->createBranch<std::vector<Float_t>>( "lep_bdt_muiso_var8" );
    ttree->createBranch<std::vector<Float_t>>( "lep_bdt_muiso_var5" );

    for ( auto& id : lepton_ids )
        ttree->createBranch<std::vector<Int_t>>( "lep_pass_" + id.second );
}

//_________________________________________________________________________________________________
void CORE2016::setLeptonBranches(RooUtil::TTreeX* ttree)
{
    setElectronBranches(ttree);
    setMuonBranches(ttree);

    // Sorting leptons
    std::vector<TString> v_int_br_name;

    v_int_br_name.push_back( "lep_pdgId" );

    for ( auto& id : lepton_ids )
        v_int_br_name.push_back( id.second );

    // Sorting leptons float variables
    std::vector<TString> v_float_br_name;

    v_float_br_name.push_back( "lep_bdt_muiso_var11" );
    v_float_br_name.push_back( "lep_bdt_muiso_var8" );
    v_float_br_name.push_back( "lep_bdt_muiso_var5" );

    ttree->sortVecBranchesByPt( "lep_p4", v_float_br_name, v_int_br_name, {} );
}

//_________________________________________________________________________________________________
void CORE2016::setElectronBranches(RooUtil::TTreeX* ttree)
{

    // List of LVs of leptons for this event
    std::vector<LV> lvs;

    // List of pass_id_flag
    std::map<TString, std::vector<Int_t>> passids;

    // loop over electrons
    for ( unsigned int iel = 0; iel < cms3.els_p4().size(); ++iel )
    {
        // The following variable keeps track of pass/fail of the ids of interest.
        std::vector<bool> pass_or_fail_book_keeping;

        // Loop over ids, and create a pass_fail_book_keeping;
        for ( auto& id : lepton_ids )
        {
            if ( electronID( iel, id.first ) )
                pass_or_fail_book_keeping.push_back( 1 );
            else
                pass_or_fail_book_keeping.push_back( 0 );
        }

        // Check whether this lepton failed all of the ids of interest. If so, skip.
        if ( std::all_of(pass_or_fail_book_keeping.begin(),
                         pass_or_fail_book_keeping.end(), [](bool v) { return !v; }) )
            continue;

        // If you are here, the lepton is now accepted. Add to the collection
        ttree->pushbackToBranch<LV   >( "lep_p4", cms3.els_p4()[iel] );
        ttree->pushbackToBranch<Int_t>( "lep_pdgId", cms3.els_charge()[iel] * -11 );
        ttree->pushbackToBranch<Float_t>( "lep_bdt_muiso_var11", -999 );
        ttree->pushbackToBranch<Float_t>( "lep_bdt_muiso_var8", -999 );
        ttree->pushbackToBranch<Float_t>( "lep_bdt_muiso_var5", -999 );

        // Also loop over IDs and flag whether this lepton passed or failed
        for ( unsigned int ith_id = 0; ith_id < lepton_ids.size(); ++ith_id )
        {
            // Get the id name
            TString& idname = lepton_ids[ith_id].second;

            // Check whether this id passed
            if ( pass_or_fail_book_keeping[ith_id] )
                ttree->pushbackToBranch<Int_t>( "lep_pass_" + idname, 1 );
            else
                ttree->pushbackToBranch<Int_t>( "lep_pass_" + idname, 0 );
        }

    }

}

//_________________________________________________________________________________________________
void CORE2016::setMuonBranches(RooUtil::TTreeX* ttree)
{

    // List of LVs of leptons for this event
    std::vector<LV> lvs;

    // List of pass_id_flag
    std::map<TString, std::vector<Int_t>> passids;

    // loop over electrons
    for ( unsigned int imu = 0; imu < cms3.mus_p4().size(); ++imu )
    {
        // The following variable keeps track of pass/fail of the ids of interest.
        std::vector<bool> pass_or_fail_book_keeping;

        // Loop over ids, and create a pass_fail_book_keeping;
        for ( auto& id : lepton_ids )
        {
            if ( muonID( imu, id.first ) )
                pass_or_fail_book_keeping.push_back( 1 );
            else
                pass_or_fail_book_keeping.push_back( 0 );
        }

        // Check whether this lepton failed all of the ids of interest. If so, skip.
        if ( std::all_of(pass_or_fail_book_keeping.begin(),
                         pass_or_fail_book_keeping.end(), [](bool v) { return !v; }) )
            continue;

        // If you are here, the lepton is now accepted. Add to the collection
        ttree->pushbackToBranch<LV   >( "lep_p4", cms3.mus_p4()[imu] );
        ttree->pushbackToBranch<Int_t>( "lep_pdgId", cms3.mus_charge()[imu] * -11 );
        ttree->pushbackToBranch<Float_t>( "lep_bdt_muiso_var11", reader_muiso_var11.evaluate(imu) );
        ttree->pushbackToBranch<Float_t>( "lep_bdt_muiso_var8", reader_muiso_var8.evaluate(imu) );
        ttree->pushbackToBranch<Float_t>( "lep_bdt_muiso_var5", reader_muiso_var5.evaluate(imu) );

        // Also loop over IDs and flag whether this lepton passed or failed
        for ( unsigned int ith_id = 0; ith_id < lepton_ids.size(); ++ith_id )
        {
            // Get the id name
            TString& idname = lepton_ids[ith_id].second;

            // Check whether this id passed
            if ( pass_or_fail_book_keeping[ith_id] )
                ttree->pushbackToBranch<Int_t>( "lep_pass_" + idname, 1 );
            else
                ttree->pushbackToBranch<Int_t>( "lep_pass_" + idname, 0 );
        }

    }

}

//_________________________________________________________________________________________________
void CORE2016::createIsoTrackBranches( RooUtil::TTreeX* ttree )
{
    ttree->createBranch<std::vector<LV     >>( "isotrk_p4"    );
}

//_________________________________________________________________________________________________
void CORE2016::setIsoTrackBranches(RooUtil::TTreeX* ttree)
{
    for (size_t pfind = 0; pfind < cms3.pfcands_p4().size(); pfind++)
    {
        LorentzVector pfcand_p4 = cms3.pfcands_p4().at(pfind);
        if (cms3.pfcands_charge().at(pfind)  == 0) { continue; }
        if (fabs(cms3.pfcands_dz().at(pfind)) >  0.1) { continue; }
        if (cms3.pfcands_fromPV().at(pfind)  <= 1) { continue; }
        float cand_pt = pfcand_p4.pt();
        if (cand_pt < 5) { continue; }
        // isotrack a la MT2
        int pdgId = abs(cms3.pfcands_particleId().at(pfind));
        float absiso03 = cms3.pfcands_trackIso().at(pfind);
        if (absiso03 < min(0.2 * cand_pt, 8.0))
        {
            if ((cand_pt > 5.) && (pdgId == 11 || pdgId == 13))
            {
                if (absiso03 / cand_pt < 0.2)
                {
                    ttree->pushbackToBranch<LV>("isotrk_p4", pfcand_p4);
                }
            }
            if (cand_pt > 10. && pdgId == 211)
            {
                if (absiso03 / cand_pt < 0.1)
                {
                    ttree->pushbackToBranch<LV>("isotrk_p4", pfcand_p4);
                }
            }
        }
    }
}

//_________________________________________________________________________________________________
void CORE2016::createJetBranches( RooUtil::TTreeX* ttree )
{
    ttree->createBranch<std::vector<LV     >>( "jets_p4" );
    ttree->createBranch<std::vector<Float_t>>( "jets_csv" );
    if ( !option.Contains("isData") )
    {
        ttree->createBranch<std::vector<Int_t>>( "jets_mcFlavour" );
        ttree->createBranch<std::vector<Int_t>>( "jets_mcHadronFlav" );
    }
}

//_________________________________________________________________________________________________
void CORE2016::setJetBranches( RooUtil::TTreeX* ttree )
{
    //JETS
    //correct jets and check baseline selections
    std::vector<LV> p4sCorrJets; // store corrected p4 for ALL jets, so indices match CMS3 ntuple
    std::vector<LV> p4sCorrJets_up; // store corrected p4 for ALL jets, so indices match CMS3 ntuple
    std::vector<LV> p4sCorrJets_dn; // store corrected p4 for ALL jets, so indices match CMS3 ntuple
    std::vector<Float_t      > jet_corrfactor; // store correction for ALL jets, and indices match CMS3 ntuple
    std::vector<std::pair<int, Float_t> > passJets; //index of jets that pass baseline selections with their corrected pt
    std::vector<Float_t      > jet_corrfactor_up; // store correction for ALL jets, and vary by uncertainties
    std::vector<Float_t      > jet_corrfactor_dn; // store correction for ALL jets, and vary by uncertainties

    for ( unsigned int iJet = 0; iJet < cms3.pfjets_p4().size(); iJet++ )
    {

        LorentzVector pfjet_p4_cor = cms3.pfjets_p4()[iJet];

        double corr = 1.0;
        double shift = 0.0;

        if ( option.Contains( "applyJEC" ) )
        {

            // get uncorrected jet p4 to use as input for corrections
            LorentzVector pfjet_p4_uncor = cms3.pfjets_p4()[iJet] * cms3.pfjets_undoJEC()[iJet];

            std::vector<float> corr_vals;

            // get L1FastL2L3Residual total correction
            jet_corrector_pfL1FastJetL2L3_current->setRho( cms3.evt_fixgridfastjet_all_rho() );
            jet_corrector_pfL1FastJetL2L3_current->setJetA( cms3.pfjets_area()[iJet] );
            jet_corrector_pfL1FastJetL2L3_current->setJetPt( pfjet_p4_uncor.pt() );
            jet_corrector_pfL1FastJetL2L3_current->setJetEta( pfjet_p4_uncor.eta() );
            //get actual corrections
            corr_vals = jet_corrector_pfL1FastJetL2L3_current->getSubCorrections();
            corr      = corr_vals[corr_vals.size() - 1]; // All corrections
            shift = 0.0;

            if ( jecUnc_current != 0 )
            {
                jecUnc_current->setJetEta( pfjet_p4_uncor.eta() );
                jecUnc_current->setJetPt( pfjet_p4_uncor.pt()*corr );
                double unc = jecUnc_current->getUncertainty( true );

                // note that this always checks the "default" filename vector size, not the run-dependent for late 2016F
                if ( cms3.evt_isRealData() && jetcorr_filenames_pfL1FastJetL2L3.size() == 4 )
                    shift = sqrt(
                            unc * unc +
                            pow( ( corr_vals[corr_vals.size() - 1] / corr_vals[corr_vals.size() - 2] - 1. ), 2 ) );

                else
                    shift = unc;
            }

            // apply new JEC to p4
            pfjet_p4_cor = pfjet_p4_uncor * corr;
        }

        p4sCorrJets.push_back( pfjet_p4_cor );
        p4sCorrJets_up.push_back( pfjet_p4_cor * ( 1.0 + shift ) );
        p4sCorrJets_dn.push_back( pfjet_p4_cor * ( 1.0 - shift ) );
        jet_corrfactor.push_back( corr );
        jet_corrfactor_up.push_back( 1.0 + shift );
        jet_corrfactor_dn.push_back( 1.0 - shift );

        if ( p4sCorrJets[iJet].pt() < 15.0 )
            continue;

        if ( fabs( p4sCorrJets[iJet].eta() ) > 5.2 )
            continue;

        // note this uses the eta of the jet as stored in CMS3
        //  chance for small discrepancies if JEC changes direction slightly..
        if ( !option.Contains("isFastSim") && !isLoosePFJet_Summer16_v1( iJet ) )
            continue;

        if (  option.Contains("isFastSim") &&  isBadFastsimJet         ( iJet ) )
            continue;

        passJets.push_back( std::pair<int, float>( iJet, pfjet_p4_cor.pt() ) );

    }

    // sort passing jets by corrected pt
    std::sort( passJets.begin(), passJets.end(), sortByValue );

    //now fill variables for jets that pass baseline selections and don't overlap with a lepton
    for ( unsigned int passIdx = 0; passIdx < passJets.size(); passIdx++ )
    {

        int iJet = passJets[passIdx].first;

        float current_csv_val = getbtagvalue( "pfCombinedInclusiveSecondaryVertexV2BJetTags", iJet );
        float current_muf_val = cms3.pfjets_muonE()[iJet] /
                                ( cms3.pfjets_undoJEC()[iJet] * cms3.pfjets_p4()[iJet].energy() );

        ttree->pushbackToBranch<LV     >( "jets_p4" , p4sCorrJets[iJet] );
        ttree->pushbackToBranch<Float_t>( "jets_csv", current_csv_val );
        if ( !option.Contains( "isData" ) )
        {
            ttree->pushbackToBranch<Int_t>( "jets_mcFlavour"   , cms3.pfjets_partonFlavour()[iJet] );
            ttree->pushbackToBranch<Int_t>( "jets_mcHadronFlav", cms3.pfjets_hadronFlavour()[iJet] );
        }


//        if ( ( p4sCorrJets_up.at( iJet ).pt() > BJET_PT_MIN ) && abs( p4sCorrJets_up.at( iJet ).eta() ) < JET_ETA_MAX )
//        {
//            if ( p4sCorrJets_up.at( iJet ).pt() > JET_PT_MIN )
//            {
//                njets_up++;
//                jets_up_p4    .push_back( p4sCorrJets_up.at( iJet ) );
//                jets_up_csv   .push_back( current_csv_val );
//            }
//        }
//
//        if ( verbose )
//            cout << "Before filling jet dn branches" << endl;
//
//        if ( ( p4sCorrJets_dn.at( iJet ).pt() > BJET_PT_MIN ) && abs( p4sCorrJets_dn.at( iJet ).eta() ) < JET_ETA_MAX )
//        {
//            if ( p4sCorrJets_dn.at( iJet ).pt() > JET_PT_MIN )
//            {
//                njets_dn++;
//                jets_dn_p4    .push_back( p4sCorrJets_dn.at( iJet ) );
//                jets_dn_csv   .push_back( current_csv_val );
//            }
//        }
    }
}

//_________________________________________________________________________________________________
void CORE2016::createGenBranches( RooUtil::TTreeX* ttree )
{
    ttree->createBranch<std::vector<LV   >>( "gen_p4" );
    ttree->createBranch<std::vector<Int_t>>( "gen_pdgId" );
    ttree->createBranch<std::vector<Int_t>>( "gen_status" );
    ttree->createBranch<std::vector<Int_t>>( "gen_charge" );
    ttree->createBranch<std::vector<Int_t>>( "gen_motherId" );
    ttree->createBranch<std::vector<Int_t>>( "gen_grandmaId" );
}

//_________________________________________________________________________________________________
void CORE2016::setGenBranches( RooUtil::TTreeX* ttree )
{
    for ( unsigned int igen = 0; igen < cms3.genps_p4().size(); ++igen )
    {
        ttree->pushbackToBranch<LV   >( "gen_p4"        , cms3.genps_p4()[igen]               );
        ttree->pushbackToBranch<Int_t>( "gen_pdgId"     , cms3.genps_id()[igen]               );
        ttree->pushbackToBranch<Int_t>( "gen_status"    , cms3.genps_status()[igen]           );
        ttree->pushbackToBranch<Int_t>( "gen_charge"    , cms3.genps_charge()[igen]           );
        ttree->pushbackToBranch<Int_t>( "gen_motherId"  , cms3.genps_id_simplemother()[igen]  );
        ttree->pushbackToBranch<Int_t>( "gen_grandmaId" , cms3.genps_id_simplegrandma()[igen] );
    }
}

//_________________________________________________________________________________________________
void CORE2016::createFatJetBranches( RooUtil::TTreeX* ttree )
{
    // product of this EDProducer
    ttree->createBranch<std::vector<LV     >>( "ak8jets_p4"                  );
    ttree->createBranch<std::vector<Float_t>>( "ak8jets_mass"                );
    ttree->createBranch<std::vector<Float_t>>( "ak8jets_undoJEC"             );
    ttree->createBranch<std::vector<Float_t>>( "ak8jets_area"                );
    ttree->createBranch<std::vector<Int_t  >>( "ak8jets_partonFlavour"       );
    ttree->createBranch<std::vector<Float_t>>( "ak8jets_nJettinessTau1"      );
    ttree->createBranch<std::vector<Float_t>>( "ak8jets_nJettinessTau2"      );
    ttree->createBranch<std::vector<Float_t>>( "ak8jets_nJettinessTau3"      );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_topMass"             );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_minMass"             );
//    ttree->createBranch<std::vector<Int_t  >>( "ak8jets_nSubJets"            );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_prunedMass"          );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_trimmedMass"         );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_filteredMass"        );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_softdropMass"        );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_puppinJettinessTau1" );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_puppinJettinessTau2" );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_puppinJettinessTau3" );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_puppipt"             );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_puppimass"           );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_puppieta"            );
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_puppiphi"            );
//    ttree->createBranch<std::vector<LV     >>( "ak8jets_softdropPuppiSubjet1");
//    ttree->createBranch<std::vector<LV     >>( "ak8jets_softdropPuppiSubjet2");
//    ttree->createBranch<std::vector<Float_t>>( "ak8jets_puppisoftdropMass"   );
//    ttree->createBranch<std::vector<std::vector<int> >  > ( "ak8jetspfcandIndicies"                   ).setBranchAlias( "ak8jets_pfcandIndicies"            );

}

//_________________________________________________________________________________________________
void CORE2016::setFatJetBranches( RooUtil::TTreeX* ttree )
{
    for ( unsigned int ifatjet = 0; ifatjet < cms3.ak8jets_p4().size(); ++ifatjet )
    {
        ttree->pushbackToBranch<LV     >( "ak8jets_p4"                  , cms3.ak8jets_p4()[ifatjet]                  );
        ttree->pushbackToBranch<Float_t>( "ak8jets_mass"                , cms3.ak8jets_mass()[ifatjet]                );
        ttree->pushbackToBranch<Float_t>( "ak8jets_undoJEC"             , cms3.ak8jets_undoJEC()[ifatjet]             );
        ttree->pushbackToBranch<Float_t>( "ak8jets_area"                , cms3.ak8jets_area()[ifatjet]                );
        ttree->pushbackToBranch<Int_t  >( "ak8jets_partonFlavour"       , cms3.ak8jets_partonFlavour()[ifatjet]       );
        ttree->pushbackToBranch<Float_t>( "ak8jets_nJettinessTau1"      , cms3.ak8jets_nJettinessTau1()[ifatjet]      );
        ttree->pushbackToBranch<Float_t>( "ak8jets_nJettinessTau2"      , cms3.ak8jets_nJettinessTau2()[ifatjet]      );
        ttree->pushbackToBranch<Float_t>( "ak8jets_nJettinessTau3"      , cms3.ak8jets_nJettinessTau3()[ifatjet]      );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_topMass"             , cms3.ak8jets_topMass()[ifatjet]             );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_minMass"             , cms3.ak8jets_minMass()[ifatjet]             );
//        ttree->pushbackToBranch<Int_t  >( "ak8jets_nSubJets"            , cms3.ak8jets_nSubJets()[ifatjet]            );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_prunedMass"          , cms3.ak8jets_prunedMass()[ifatjet]          );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_trimmedMass"         , cms3.ak8jets_trimmedMass()[ifatjet]         );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_filteredMass"        , cms3.ak8jets_filteredMass()[ifatjet]        );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_softdropMass"        , cms3.ak8jets_softdropMass()[ifatjet]        );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_puppinJettinessTau1" , cms3.ak8jets_puppinJettinessTau1()[ifatjet] );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_puppinJettinessTau2" , cms3.ak8jets_puppinJettinessTau2()[ifatjet] );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_puppinJettinessTau3" , cms3.ak8jets_puppinJettinessTau3()[ifatjet] );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_puppipt"             , cms3.ak8jets_puppipt()[ifatjet]             );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_puppimass"           , cms3.ak8jets_puppimass()[ifatjet]           );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_puppieta"            , cms3.ak8jets_puppieta()[ifatjet]            );
//        ttree->pushbackToBranch<Float_t>( "ak8jets_puppiphi"            , cms3.ak8jets_puppiphi()[ifatjet]            );
//        ttree->pushbackToBranch<LV     >( "ak8jets_softdropPuppiSubjet1", cms3.ak8jets_softdropPuppiSubjet1()[ifatjet]);
//        ttree->pushbackToBranch<LV     >( "ak8jets_softdropPuppiSubjet2", cms3.ak8jets_softdropPuppiSubjet2()[ifatjet]);
//        ttree->pushbackToBranch<Float_t>( "ak8jets_puppisoftdropMass"   , cms3.ak8jets_puppisoftdropMass()[ifatjet]   );
    }
}

//_________________________________________________________________________________________________
void CORE2016::createTrigBranches( RooUtil::TTreeX* ttree, std::vector<TString> trigger_patterns_ )
{
//    ttree->createBranch<std::vector<TString>>( "hlt_trigNames" );
//    ttree->createBranch<TBits               >( "hlt_bits"      );

    // Set the triggers
    trigger_patterns = trigger_patterns_;

    cms3.GetEntry(0);

    for ( auto& triggerName : cms3.hlt_trigNames() )
    {
        for ( auto& trigger_pattern : trigger_patterns )
        {
            if ( triggerName.Contains( trigger_pattern ) )
            {
                TString tmpstr =
                    ((TObjString*) triggerName.Tokenize( "v" )->At(0))->GetString();
                TString version_sffx_rm_trig_name = tmpstr(0, tmpstr.Length() - 1);
                ttree->createBranch<Int_t>( "pass_trig_" + version_sffx_rm_trig_name );
                triggers.push_back( { version_sffx_rm_trig_name, triggerName } );
            }
        }
    }
}

//_________________________________________________________________________________________________
void CORE2016::setTrigBranches( RooUtil::TTreeX* ttree )
{
//    ttree->setBranch<std::vector<TString>>( "hlt_trigNames", cms3.hlt_trigNames() );
//    ttree->setBranch<TBits               >( "hlt_bits"     , cms3.hlt_bits()      );
    for ( auto& pair_trigname : triggers )
        ttree->setBranch<Int_t>( "pass_trig_" + pair_trigname.first, passHLTTriggerPattern( pair_trigname.second ) );
}

//_________________________________________________________________________________________________
int CORE2016::nCount( RooUtil::TTreeX* ttree, TString bn )
{
    const std::vector<Int_t> &v = ttree->getBranch<std::vector<Int_t>>(bn);
    return std::count_if(v.begin(), v.end(), [](Int_t i){return i == 1;});
}

//eof
