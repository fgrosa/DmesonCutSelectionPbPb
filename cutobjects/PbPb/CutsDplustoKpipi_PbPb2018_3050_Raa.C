#if !defined (__CINT__) || defined (__CLING__)

#include <Riostream.h>
#include <TFile.h>
#include <TClonesArray.h>
#include <TParameter.h>

#include "AliESDtrackCuts.h"
#include "AliRDHFCutsDplustoKpipi.h"

#endif

//____________________________________________________________________________________________________//
// Methods:
// 1) MakeFileForCutsDplus3050_Central2018 --> central cuts of 2015 and 2018 analyses
// 2) MakeFileForCutsDplus3050_Loose2018 --> loose cuts of 2018 analysis (sparse)
// 3) MakeFileForCutsDplus3050_FiltTreeCreator2018 --> filtering cuts of 2018 analysis (tree creator)
//____________________________________________________________________________________________________//

//__________________________________________________________________________________________
AliRDHFCutsDplustoKpipi* MakeFileForCutsDplus3050_Central2018(bool fUseStrongPID=false, double maxPtstrongPID=3.0, bool fIsMC=false, int addTrackCut = 0){

    AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
    esdTrackCuts->SetRequireSigmaToVertex(false);
    esdTrackCuts->SetRequireTPCRefit(true);
    esdTrackCuts->SetRequireITSRefit(true);
    esdTrackCuts->SetMaxChi2PerClusterTPC(2.5);
    esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
    esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kAny);
    esdTrackCuts->SetMinDCAToVertexXY(0.);
    esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    esdTrackCuts->SetPtRange(0.4,1.e10);

    float minc=30.,maxc=50.;
    const int nptbins=15;
    float* ptbins;
    ptbins=new float[nptbins+1];

    ptbins[0]=2.;
    ptbins[1]=3.;
    ptbins[2]=4.;
    ptbins[3]=5.;
    ptbins[4]=6.;
    ptbins[5]=7.;
    ptbins[6]=8.;
    ptbins[7]=9.;
    ptbins[8]=10.;
    ptbins[9]=11.;
    ptbins[10]=12.;
    ptbins[11]=14.;
    ptbins[12]=16.;
    ptbins[13]=24.;
    ptbins[14]=36.;
    ptbins[15]=50.;

    const int nvars=14;
    float** anacutsval;
    anacutsval=new float*[nvars];
    for(int ic=0;ic<nvars;ic++){anacutsval[ic]=new float[nptbins];}

    /*

     Cut list
     0          "inv. mass [GeV]",
     1			"pTK [GeV/c]",
     2			"pTPi [GeV/c]",
     3			"d0K [cm]",
     4			"d0Pi [cm]",
     5			"dist12 [cm]",
     6			"sigmavert [cm]",
     7			"decLen [cm]",
     8			"ptMax [GeV/c]",
     9			"cosThetaPoint",
     10			"Sum d0^2 (cm^2)",
     11			"dca [cm]",
     12			"norm decay length XY",
     13			"cosThetaPointXY";
     */

    for(int ipt=0;ipt<nptbins;ipt++){
        anacutsval[0][ipt]=0.2; //minv
        anacutsval[1][ipt]=0.4; //ptK
        anacutsval[2][ipt]=0.4; //ptPi
        anacutsval[3][ipt]=0.; //d0K
        anacutsval[4][ipt]=0.; //d0Pi
        anacutsval[5][ipt]=0.; //dist12
        anacutsval[8][ipt]=0.0; //pM
        anacutsval[10][ipt]=0.0; //sumd02
        anacutsval[11][ipt]=10000000000.; //dca
    }

    int ic=6;//sigvert
    anacutsval[ic][0]=0.020;//2.0-3.0
    anacutsval[ic][1]=0.022;//3.0-4.0
    anacutsval[ic][2]=0.022;//4.0-5.0
    anacutsval[ic][3]=0.022;//5.0-6.0
    anacutsval[ic][4]=0.024;//6.0-7.0
    anacutsval[ic][5]=0.024;//7.0-8.0
    anacutsval[ic][6]=0.024;//8.0-9.0
    anacutsval[ic][7]=0.024;//9.0-10.0
    anacutsval[ic][8]=0.024;//10.0-11.0
    anacutsval[ic][9]=0.024;//11.0-12.0
    anacutsval[ic][10]=0.024;//12.0-14.0
    anacutsval[ic][11]=0.024;//14.0-16.0
    anacutsval[ic][12]=0.024;//16.0-24.0
    anacutsval[ic][13]=0.034;//24.0-36.0
    anacutsval[ic][14]=0.034;//36.0-50.0

    ic=7;//declen
    for(int ipt=0;ipt<nptbins;ipt++){
        anacutsval[ic][ipt]=0.12;
    }
    anacutsval[ic][0]=0.07;//2.0-3.0
    anacutsval[ic][1]=0.10;//3.0-4.0
    anacutsval[ic][2]=0.10;//4.0-5.0
    anacutsval[ic][3]=0.10;//5.0-6.0
    anacutsval[ic][12]=0.14;//16.0-24.0
    anacutsval[ic][13]=0.14;//24.0-36.0
    anacutsval[ic][14]=0.14;//36.0-50.0

    //cosp
    ic=9;
    anacutsval[ic][0]=0.996;//2.0-3.0
    anacutsval[ic][1]=0.996;//3.0-4.0
    anacutsval[ic][2]=0.995;//4.0-5.0
    anacutsval[ic][3]=0.995;//5.0-6.0
    anacutsval[ic][4]=0.995;//6.0-7.0
    anacutsval[ic][5]=0.995;//7.0-8.0
    anacutsval[ic][6]=0.990;//8.0-9.0
    anacutsval[ic][7]=0.990;//9.0-10.0
    anacutsval[ic][8]=0.990;//10.0-11.0
    anacutsval[ic][9]=0.990;//11.0-12.0
    anacutsval[ic][10]=0.990;//12.0-14.0
    anacutsval[ic][11]=0.990;//14.0-16.0
    anacutsval[ic][12]=0.980;//16.0-24.0
    anacutsval[ic][13]=0.970;//24.0-36.0
    anacutsval[ic][14]=0.950;//36.0-50.0

    ic=12;//ndlXY
    anacutsval[ic][0]=12.;//2.0-3.0
    anacutsval[ic][1]=12.;//3.0-4.0
    anacutsval[ic][2]=12.;//4.0-5.0
    anacutsval[ic][3]=12.;//5.0-6.0
    anacutsval[ic][4]=10.;//6.0-7.0
    anacutsval[ic][5]=10.;//7.0-8.0
    anacutsval[ic][6]=10.;//8.0-9.0
    anacutsval[ic][7]=10.;//9.0-10.0
    anacutsval[ic][8]=9.;//10.0-11.0
    anacutsval[ic][9]=9.;//11.0-12.0
    anacutsval[ic][10]=9.;//12.0-14.0
    anacutsval[ic][11]=9.;//14.0-16.0
    anacutsval[ic][12]=8.;//16.0-24.0
    anacutsval[ic][13]=8.;//24.0-36.0
    anacutsval[ic][14]=6.;//36.0-50.0

    ic=13;//cospXY
    anacutsval[ic][0]=0.996;//2.0-3.0
    anacutsval[ic][1]=0.996;//3.0-4.0
    anacutsval[ic][2]=0.995;//4.0-5.0
    anacutsval[ic][3]=0.995;//5.0-6.0
    anacutsval[ic][4]=0.995;//6.0-7.0
    anacutsval[ic][5]=0.995;//7.0-8.0
    anacutsval[ic][6]=0.990;//8.0-9.0
    anacutsval[ic][7]=0.990;//9.0-10.0
    anacutsval[ic][8]=0.990;//10.0-11.0
    anacutsval[ic][9]=0.990;//11.0-12.0
    anacutsval[ic][10]=0.990;//12.0-14.0
    anacutsval[ic][11]=0.990;//14.0-16.0
    anacutsval[ic][12]=0.980;//16.0-24.0
    anacutsval[ic][13]=0.970;//24.0-36.0
    anacutsval[ic][14]=0.950;//36.0-50.0

    float *d0cutsval=new float[nptbins];
    for(int ipt=0;ipt<nptbins;ipt++){ //d0
        d0cutsval[ipt]=60;
    }
    d0cutsval[0]=80;//2.0-3.0

    float *d0d0expcutsval=new float[nptbins];
    for(int ipt=0;ipt<nptbins;ipt++){ //d0d0exp
        d0d0expcutsval[ipt]=2.5;
    }
    d0d0expcutsval[0]=1.5;//2.0-3.0
    d0d0expcutsval[1]=1.5;//3.0-4.0
    d0d0expcutsval[2]=2.0;//4.0-5.0
    d0d0expcutsval[3]=2.0;//5.0-6.0
    d0d0expcutsval[14]=3.0;//36.0-50.0

    AliRDHFCutsDplustoKpipi* analysiscuts=new AliRDHFCutsDplustoKpipi();
    analysiscuts->SetName("AnalysisCuts");
    analysiscuts->SetTitle("Cuts for Dplus Analysis and CF");
    analysiscuts->SetPtBins(nptbins+1,ptbins);
    analysiscuts->SetCuts(nvars,nptbins,anacutsval);
    analysiscuts->Setd0Cut(nptbins,d0cutsval);
    analysiscuts->Setd0MeasMinusExpCut(nptbins,d0d0expcutsval);
    analysiscuts->AddTrackCuts(esdTrackCuts);
    analysiscuts->SetMinNumTPCClsForPID(50);
    analysiscuts->SetScaleNormDLxyBypOverPt(false);

    analysiscuts->SetUsePID(true);
    AliAODPidHF* PidHF = NULL;
    if(fUseStrongPID) {
        analysiscuts->SetUseStrongPid(3);
        analysiscuts->SetMaxPtStrongPid(maxPtstrongPID);
        analysiscuts->SetMaxPStrongPidK(1);
        analysiscuts->SetMaxPStrongPidpi(1);
    }

    analysiscuts->SetUseImpParProdCorrCut(false);
    analysiscuts->SetOptPileup(false);
    if(!fIsMC) {
        analysiscuts->SetMinCentrality(minc);
        analysiscuts->SetMaxCentrality(maxc);
    }

    analysiscuts->SetRemoveTrackletOutliers(true);//added on June 28
    analysiscuts->SetCutOnzVertexSPD(3);//needed for Pb-Pb 2015

    analysiscuts->SetUseCentrality(AliRDHFCuts::kCentV0M); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,kCentInvalid
    analysiscuts->SetTriggerClass("");//dont use for ppMB/ppMB_MC
    analysiscuts->ResetMaskAndEnableMBTrigger();//dont use for ppMB/ppMB_MC
    analysiscuts->SetTriggerMask(AliVEvent::kINT7 | AliVEvent::kSemiCentral);
    if(fIsMC) analysiscuts->SetTriggerMask(AliVEvent::kMB);

    if(!fIsMC)
        analysiscuts->SetUseTimeRangeCutForPbPb2018(true);
    if(!fIsMC)
      analysiscuts->EnableNsigmaDataDrivenCorrection(true,AliAODPidHF::kPbPb3050);

    TString trackCutName= "";
    switch (addTrackCut) {
      case 1:
        esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.9);
        trackCutName = "_addRowsOverClusterTPC";
        break;

      case 2:
        analysiscuts->SetMinCrossedRowsTPCPtDep("120-(5/pt)");
        trackCutName = "_addMinCrossedRowsTPC";
        break;

      case 3:
        analysiscuts->SetMinRatioClsOverCrossRowsTPC(0.65);
        trackCutName = "_addRatioClsOverRowsTPC";
        break;

      case 4:
        analysiscuts->SetUseCutGeoNcrNcl(true);
        trackCutName = "_addUseCutGeo";
        break;

      default:
        break;
    }

    analysiscuts->SetMinPtCandidate(2.);
    analysiscuts->SetMaxPtCandidate(50.);

    std::cout<<"This is the object I'm going to save:"<<nptbins<<std::endl;

    TString triggername="kINT7_kSemiCentral";
    if(fIsMC) triggername="kMB";
    TString PIDsuffix="";
    if(fUseStrongPID) PIDsuffix=Form("_strongPIDpt%0.f",maxPtstrongPID);

    analysiscuts->PrintAll();
    analysiscuts->PrintTrigger();
    TString filename=Form("DplustoKpipiCuts_3050_central%s_Raa_%s%s.root",PIDsuffix.Data(),triggername.Data(),trackCutName.Data());
    TFile* fout=new TFile(filename.Data(),"recreate");
    fout->cd();
    analysiscuts->Write();
    fout->Close();

    return analysiscuts;
}

//__________________________________________________________________________________________
AliRDHFCutsDplustoKpipi* MakeFileForCutsDplus3050_Loose2018(bool fUseStrongPID=false, double maxPtstrongPID=3.0, bool fIsMC=false){

    AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
    esdTrackCuts->SetRequireSigmaToVertex(false);
    esdTrackCuts->SetRequireTPCRefit(true);
    esdTrackCuts->SetRequireITSRefit(true);
    esdTrackCuts->SetMaxChi2PerClusterTPC(2.5);
    esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
    esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kAny);
    esdTrackCuts->SetMinDCAToVertexXY(0.);
    esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    esdTrackCuts->SetPtRange(0.4,1.e10);

    float minc=30.,maxc=50.;
    const int nptbins=15;
    float* ptbins;
    ptbins=new float[nptbins+1];

    ptbins[0]=2.;
    ptbins[1]=3.;
    ptbins[2]=4.;
    ptbins[3]=5.;
    ptbins[4]=6.;
    ptbins[5]=7.;
    ptbins[6]=8.;
    ptbins[7]=9.;
    ptbins[8]=10.;
    ptbins[9]=11.;
    ptbins[10]=12.;
    ptbins[11]=14.;
    ptbins[12]=16.;
    ptbins[13]=24.;
    ptbins[14]=36.;
    ptbins[15]=50.;

    const int nvars=14;
    float** anacutsval;
    anacutsval=new float*[nvars];
    for(int ic=0;ic<nvars;ic++){anacutsval[ic]=new float[nptbins];}

    /*

     Cut list
     0          "inv. mass [GeV]",
     1			"pTK [GeV/c]",
     2			"pTPi [GeV/c]",
     3			"d0K [cm]",
     4			"d0Pi [cm]",
     5			"dist12 [cm]",
     6			"sigmavert [cm]",
     7			"decLen [cm]",
     8			"ptMax [GeV/c]",
     9			"cosThetaPoint",
     10			"Sum d0^2 (cm^2)",
     11			"dca [cm]",
     12			"norm decay length XY",
     13			"cosThetaPointXY";
     */

    for(int ipt=0;ipt<nptbins;ipt++){
        anacutsval[0][ipt]=0.2; //minv
        anacutsval[1][ipt]=0.4; //ptK
        anacutsval[2][ipt]=0.4; //ptPi
        anacutsval[3][ipt]=0.; //d0K
        anacutsval[4][ipt]=0.; //d0Pi
        anacutsval[5][ipt]=0.; //dist12
        anacutsval[8][ipt]=0.0; //pM
        anacutsval[10][ipt]=0.0; //sumd02
        anacutsval[11][ipt]=10000000000.; //dca
    }

    int ic=6;//sigvert
    anacutsval[ic][0]=0.022;//2.0-3.0
    anacutsval[ic][1]=0.024;//3.0-4.0
    anacutsval[ic][2]=0.024;//4.0-5.0
    anacutsval[ic][3]=0.024;//5.0-6.0
    anacutsval[ic][4]=0.026;//6.0-7.0
    anacutsval[ic][5]=0.026;//7.0-8.0
    anacutsval[ic][6]=0.026;//8.0-9.0
    anacutsval[ic][7]=0.026;//9.0-10.0
    anacutsval[ic][8]=0.026;//10.0-11.0
    anacutsval[ic][9]=0.026;//11.0-12.0
    anacutsval[ic][10]=0.026;//12.0-14.0
    anacutsval[ic][11]=0.026;//14.0-16.0
    anacutsval[ic][12]=0.026;//16.0-24.0
    anacutsval[ic][13]=0.036;//24.0-36.0
    anacutsval[ic][14]=0.036;//36.0-50.0

    ic=7;//declen
    for(int ipt=0;ipt<nptbins;ipt++){
        anacutsval[ic][ipt]=0.11;
    }
    anacutsval[ic][0]=0.07;//2.0-3.0
    anacutsval[ic][12]=0.15;//16.0-24.0
    anacutsval[ic][13]=0.15;//24.0-36.0
    anacutsval[ic][14]=0.15;//36.0-50.0

    //cosp
    ic=9;
    anacutsval[ic][0]=0.996;//2.0-3.0
    anacutsval[ic][1]=0.995;//3.0-4.0
    anacutsval[ic][2]=0.994;//4.0-5.0
    anacutsval[ic][3]=0.994;//5.0-6.0
    anacutsval[ic][4]=0.994;//6.0-7.0
    anacutsval[ic][5]=0.994;//7.0-8.0
    anacutsval[ic][6]=0.988;//8.0-9.0
    anacutsval[ic][7]=0.988;//9.0-10.0
    anacutsval[ic][8]=0.988;//10.0-11.0
    anacutsval[ic][9]=0.988;//11.0-12.0
    anacutsval[ic][10]=0.988;//12.0-14.0
    anacutsval[ic][11]=0.988;//14.0-16.0
    anacutsval[ic][12]=0.975;//16.0-24.0
    anacutsval[ic][13]=0.950;//24.0-36.0
    anacutsval[ic][14]=0.950;//36.0-50.0

    ic=12;//ndlXY
    anacutsval[ic][0]=11.;//2.0-3.0
    anacutsval[ic][1]=11.;//3.0-4.0
    anacutsval[ic][2]=11.;//4.0-5.0
    anacutsval[ic][3]=11.;//5.0-6.0
    anacutsval[ic][4]=9.;//6.0-7.0
    anacutsval[ic][5]=9.;//7.0-8.0
    anacutsval[ic][6]=9.;//8.0-9.0
    anacutsval[ic][7]=9.;//9.0-10.0
    anacutsval[ic][8]=8.;//10.0-11.0
    anacutsval[ic][9]=8.;//11.0-12.0
    anacutsval[ic][10]=8.;//12.0-14.0
    anacutsval[ic][11]=8.;//14.0-16.0
    anacutsval[ic][12]=6.;//16.0-24.0
    anacutsval[ic][13]=6.;//24.0-36.0
    anacutsval[ic][14]=5.;//36.0-50.0

    ic=13;//cospXY
    anacutsval[ic][0]=0.996;//2.0-3.0
    anacutsval[ic][1]=0.995;//3.0-4.0
    anacutsval[ic][2]=0.994;//4.0-5.0
    anacutsval[ic][3]=0.994;//5.0-6.0
    anacutsval[ic][4]=0.994;//6.0-7.0
    anacutsval[ic][5]=0.994;//7.0-8.0
    anacutsval[ic][6]=0.988;//8.0-9.0
    anacutsval[ic][7]=0.988;//9.0-10.0
    anacutsval[ic][8]=0.988;//10.0-11.0
    anacutsval[ic][9]=0.988;//11.0-12.0
    anacutsval[ic][10]=0.988;//12.0-14.0
    anacutsval[ic][11]=0.988;//14.0-16.0
    anacutsval[ic][12]=0.975;//16.0-24.0
    anacutsval[ic][13]=0.950;//24.0-36.0
    anacutsval[ic][14]=0.950;//36.0-50.0

    float *d0cutsval=new float[nptbins];
    for(int ipt=0;ipt<nptbins;ipt++){ //d0
        d0cutsval[ipt]=150;
    }

    float *d0d0expcutsval=new float[nptbins];
    for(int ipt=0;ipt<nptbins;ipt++){ //d0d0exp
        d0d0expcutsval[ipt]=4.5;
    }

    AliRDHFCutsDplustoKpipi* analysiscuts=new AliRDHFCutsDplustoKpipi();
    analysiscuts->SetName("AnalysisCuts");
    analysiscuts->SetTitle("Cuts for Dplus Analysis and CF");
    analysiscuts->SetPtBins(nptbins+1,ptbins);
    analysiscuts->SetCuts(nvars,nptbins,anacutsval);
    analysiscuts->Setd0Cut(nptbins,d0cutsval);
    analysiscuts->Setd0MeasMinusExpCut(nptbins,d0d0expcutsval);
    analysiscuts->AddTrackCuts(esdTrackCuts);
    analysiscuts->SetMinNumTPCClsForPID(50);
    analysiscuts->SetScaleNormDLxyBypOverPt(false);
    analysiscuts->SetUsePreSelect(1);

    analysiscuts->SetUsePID(true);
    AliAODPidHF* PidHF = NULL;
    if(fUseStrongPID) {
        analysiscuts->SetUseStrongPid(3);
        analysiscuts->SetMaxPtStrongPid(maxPtstrongPID);
        analysiscuts->SetMaxPStrongPidK(1);
        analysiscuts->SetMaxPStrongPidpi(1);
    }

    analysiscuts->SetUseImpParProdCorrCut(false);
    analysiscuts->SetOptPileup(false);
    if(!fIsMC) {
        analysiscuts->SetMinCentrality(minc);
        analysiscuts->SetMaxCentrality(maxc);
    }

    analysiscuts->SetRemoveTrackletOutliers(true);//added on June 28
    analysiscuts->SetCutOnzVertexSPD(3);//needed for Pb-Pb 2015

    analysiscuts->SetUseCentrality(AliRDHFCuts::kCentV0M); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,kCentInvalid
    analysiscuts->SetTriggerClass("");//dont use for ppMB/ppMB_MC
    analysiscuts->ResetMaskAndEnableMBTrigger();//dont use for ppMB/ppMB_MC
    analysiscuts->SetTriggerMask(AliVEvent::kINT7 | AliVEvent::kSemiCentral);
    if(fIsMC) analysiscuts->SetTriggerMask(AliVEvent::kMB);

    if(!fIsMC)
        analysiscuts->SetUseTimeRangeCutForPbPb2018(true);
    if(!fIsMC)
      analysiscuts->EnableNsigmaDataDrivenCorrection(true,AliAODPidHF::kPbPb3050);

    analysiscuts->SetMinPtCandidate(2.);
    analysiscuts->SetMaxPtCandidate(50.);

    std::cout<<"This is the object I'm going to save:"<<nptbins<<std::endl;

    TString triggername="kINT7_kSemiCentral";
    if(fIsMC) triggername="kMB";
    TString PIDsuffix="";
    if(fUseStrongPID) PIDsuffix=Form("_strongPIDpt%0.f",maxPtstrongPID);

    analysiscuts->PrintAll();
    analysiscuts->PrintTrigger();
    TString filename=Form("DplustoKpipiCuts_3050_loose%s_Raa_%s.root",PIDsuffix.Data(),triggername.Data());
    TFile* fout=new TFile(filename.Data(),"recreate");
    fout->cd();
    analysiscuts->Write();
    fout->Close();

    return analysiscuts;
}

//__________________________________________________________________________________________
AliRDHFCutsDplustoKpipi* MakeFileForCutsDplus3050_FiltTreeCreator2018(bool fUseStrongPID=false, double maxPtstrongPID=3.0, bool fIsMC=false){

    AliESDtrackCuts* esdTrackCuts=new AliESDtrackCuts();
    esdTrackCuts->SetRequireSigmaToVertex(false);
    esdTrackCuts->SetRequireTPCRefit(true);
    esdTrackCuts->SetRequireITSRefit(true);
    esdTrackCuts->SetMaxChi2PerClusterTPC(2.5);
    esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
    esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD, AliESDtrackCuts::kAny);
    esdTrackCuts->SetMinDCAToVertexXY(0.);
    esdTrackCuts->SetMinDCAToVertexXYPtDep("0.0025*TMath::Max(0.,(1-TMath::Floor(TMath::Abs(pt)/2.)))");
    esdTrackCuts->SetPtRange(0.4,1.e10);

    float minc=30.,maxc=50.;
    const int nptbins=2;
    float* ptbins;
    ptbins=new float[nptbins+1];

    ptbins[0]=2.;
    ptbins[1]=5.;
    ptbins[2]=50.;

    const int nvars=14;
    float** anacutsval;
    anacutsval=new float*[nvars];
    for(int ic=0;ic<nvars;ic++){anacutsval[ic]=new float[nptbins];}

    /*

     Cut list
     0          "inv. mass [GeV]",
     1			"pTK [GeV/c]",
     2			"pTPi [GeV/c]",
     3			"d0K [cm]",
     4			"d0Pi [cm]",
     5			"dist12 [cm]",
     6			"sigmavert [cm]",
     7			"decLen [cm]",
     8			"ptMax [GeV/c]",
     9			"cosThetaPoint",
     10			"Sum d0^2 (cm^2)",
     11			"dca [cm]",
     12			"norm decay length XY",
     13			"cosThetaPointXY";
     */

    for(int ipt=0;ipt<nptbins;ipt++){
        anacutsval[0][ipt]=0.2; //minv
        anacutsval[1][ipt]=0.4; //ptK
        anacutsval[2][ipt]=0.4; //ptPi
        anacutsval[3][ipt]=0.; //d0K
        anacutsval[4][ipt]=0.; //d0Pi
        anacutsval[5][ipt]=0.; //dist12
        anacutsval[8][ipt]=0.0; //pM
        anacutsval[10][ipt]=0.0; //sumd02
        anacutsval[11][ipt]=10000000000.; //dca
    }

    anacutsval[6][0]=0.04;
    anacutsval[7][0]=0.05;
    anacutsval[9][0]=0.99;
    anacutsval[12][0]=5.;
    anacutsval[13][0]=0.99;

    anacutsval[6][1]=0.06;
    anacutsval[7][1]=0.08;
    anacutsval[9][1]=0.97;
    anacutsval[12][1]=3.;
    anacutsval[13][1]=0.97;

    AliRDHFCutsDplustoKpipi* analysiscuts=new AliRDHFCutsDplustoKpipi();
    analysiscuts->SetName("AnalysisCuts");
    analysiscuts->SetTitle("Cuts for Dplus Analysis and CF");
    analysiscuts->SetPtBins(nptbins+1,ptbins);
    analysiscuts->SetCuts(nvars,nptbins,anacutsval);
    analysiscuts->AddTrackCuts(esdTrackCuts);
    analysiscuts->SetMinNumTPCClsForPID(50);
    analysiscuts->SetScaleNormDLxyBypOverPt(false);
    analysiscuts->SetUsePreSelect(1);

    analysiscuts->SetUsePID(true);
    AliAODPidHF* PidHF = NULL;
    if(fUseStrongPID) {
        analysiscuts->SetUseStrongPid(3);
        analysiscuts->SetMaxPtStrongPid(maxPtstrongPID);
        analysiscuts->SetMaxPStrongPidK(1);
        analysiscuts->SetMaxPStrongPidpi(1);
    }

    analysiscuts->SetUseImpParProdCorrCut(false);
    analysiscuts->SetOptPileup(false);
    if(!fIsMC) {
        analysiscuts->SetMinCentrality(minc);
        analysiscuts->SetMaxCentrality(maxc);
    }

    analysiscuts->SetRemoveTrackletOutliers(true);//added on June 28
    analysiscuts->SetCutOnzVertexSPD(3);//needed for Pb-Pb 2015

    analysiscuts->SetUseCentrality(AliRDHFCuts::kCentV0M); //kCentOff,kCentV0M,kCentTRK,kCentTKL,kCentCL1,kCentInvalid
    analysiscuts->SetTriggerClass("");//dont use for ppMB/ppMB_MC
    analysiscuts->ResetMaskAndEnableMBTrigger();//dont use for ppMB/ppMB_MC
    analysiscuts->SetTriggerMask(AliVEvent::kINT7 | AliVEvent::kSemiCentral);
    if(fIsMC) analysiscuts->SetTriggerMask(AliVEvent::kMB);

    if(!fIsMC)
        analysiscuts->SetUseTimeRangeCutForPbPb2018(true);
    if(!fIsMC)
      analysiscuts->EnableNsigmaDataDrivenCorrection(true,AliAODPidHF::kPbPb3050);

    analysiscuts->SetMinPtCandidate(2.);
    analysiscuts->SetMaxPtCandidate(50.);

    std::cout<<"This is the object I'm going to save:"<<nptbins<<std::endl;

    TString triggername="kINT7_kSemiCentral";
    if(fIsMC) triggername="kMB";
    TString PIDsuffix="";
    if(fUseStrongPID) PIDsuffix=Form("_strongPIDpt%0.f",maxPtstrongPID);

    analysiscuts->PrintAll();
    analysiscuts->PrintTrigger();
    TString filename=Form("DplustoKpipiCuts_3050_filttreecreator%s_Raa_%s.root",PIDsuffix.Data(),triggername.Data());
    TFile* fout=new TFile(filename.Data(),"recreate");
    fout->cd();
    analysiscuts->Write();
    fout->Close();

    return analysiscuts;
}
