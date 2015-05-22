
/* **************************************************
 *   Run StPicoMixedEventMaker in different modes
 * --------------------------------------------------
 *   
 * Much work still, maker to produce mixed event background
 * for particle decays as used in the rest of the LBL Pico
 * Analysis Library
 *
 * --------------------------------------------------
 *  Authors:  Xin Dong        (xdong@lbl.gov)
 *            Michael Lomnitz (mrlomnitz@lbl.gov)
 *            Mustafa Mustafa (mmustafa@lbl.gov)
 *
 * **************************************************
 */

#include "TROOT.h"
#include "TSystem.h"
#include "TChain.h"


# ifndef __CINT__

#include "StMaker.h"
#include "StChain.h"

#include "StMuDSTMaker/COMMON/macros/loadSharedLibraries.C"

#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoHFMaker/StPicoHFEvent.h"

#include "StPicoMixedEventMaker/StPicoMixedEventMaker.h"
#include "StPicoHFMaker/StHFCuts.h"
#include "StMemStat.h"

#else
class StChain;
#endif

class StMaker;
class StChain;
class StPicoDstMaker;
StChain *chain;

void runPicoMixedEvent(const Char_t *inputFile="test.list", const Char_t *outputFile="outputBaseName", 
			 const Char_t *badRunListFileName = "picoList_bad_MB.list") { 
  // -- Check STAR Library. Please set SL_version to the original star library used in the production 
  //    from http://www.star.bnl.gov/devcgi/dbProdOptionRetrv.pl
  StMemStat mem;
  string SL_version = "SL15c";
  string env_SL = getenv ("STAR");
  if (env_SL.find(SL_version)==string::npos) {
      cout<<"Environment Star Library does not match the requested library in runPicoMixedEventMaker.C. Exiting..."<<endl;
      exit(1);
  }
  // ========================================================================================
  //   Testing 
  // ========================================================================================
  Int_t nEvents = 10000000;
	
  gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
  loadSharedLibraries();

  gSystem->Load("StBTofUtil");
  gSystem->Load("StPicoDstMaker");
  gSystem->Load("StPicoPrescales");
  gSystem->Load("StPicoHFMaker");
  gSystem->Load("StPicoMixedEventMaker");
  
  chain = new StChain();

  // ========================================================================================
  // Still bone dry
  // ========================================================================================


  TString sInputFile(inputFile);
  TString sInputListHF("");  

  // ========================================================================================
  StPicoDstMaker* picoDstMaker = new StPicoDstMaker(0, sInputFile, "picoDstMaker");
  StPicoMixedEventMaker* picoMixedEventMaker = new StPicoMixedEventMaker("picoMixedEventMaker", picoDstMaker, outputFile, sInputListHF);

  StHFCuts* hfCuts = new StHFCuts("hfBaseCuts");
  picoMixedEventMaker -> setHFBaseCuts(hfCuts);

  // ---------------------------------------------------
  // -- Set Base cuts for HF analysis

  // -- File name of bad run list
  //hfCuts->setBadRunListFileName(badRunListFileName);

  // -- Event and track cuts  ----------------------------
  hfCuts->setCutVzMax(6.);
  hfCuts->setCutVzVpdVzMax(3.);
  //hfCuts->setCutTriggerWord(0x1F);

  hfCuts->setCutNHitsFitMax(15); 
  hfCuts->setCutRequireHFT(false);
  // hfCuts->setCutNHitsFitnHitsMax(0.52);

  // -- Topological and PID cuts ------------------------
  hfCuts->setCutSecondaryPair(0.75, 1.0, 7.0, 
			      0.9, 0,2.0 );
  //Single track pt
  hfCuts->setCutPtRange(0.6,2.0,StHFCuts::kPion);
  hfCuts->setCutPtRange(0.6,2.0,StHFCuts::kKaon);
  //TPC setters
  hfCuts->setCutTPCNSigmaPion(3.0);
  hfCuts->setCutTPCNSigmaKaon(3.0);
  //TOF setters, need to set pt range as well
  hfCuts->setCutTOFDeltaOneOverBeta(0.05, StHFCuts::kKaon);
  hfCuts->setCutPtotRangeHybridTOF(0.6,2.0,StHFCuts::kKaon);
  hfCuts->setCutTOFDeltaOneOverBeta(0.05, StHFCuts::kPion);
  hfCuts->setCutPtotRangeHybridTOF(0.6,2.0,StHFCuts::kPion);
  // ---------------------------------------------------

  chain->Init();
  cout << "chain->Init();" << endl;
  int total = picoDstMaker->chain()->GetEntries();
  cout << " Total entries = " << total << endl;
  if(nEvents>total) nEvents = total;
  for (Int_t i=0; i<nEvents; i++) {
    if(i%10==0)
      cout << "Working on eventNumber " << i << endl;
    
    chain->Clear();

    int iret = chain->Make(i);

    if (iret) { cout << "Bad return code!" << iret << endl; break;}
    
    total++;
  }
  
  cout << "****************************************** " << endl;
  cout << "Work done... now its time to close up shop!"<< endl;
  cout << "****************************************** " << endl;
  chain->Finish();
  cout << "****************************************** " << endl;
  cout << "total number of events  " << nEvents << endl;
  cout << "****************************************** " << endl;
  
  delete chain;
  
}

