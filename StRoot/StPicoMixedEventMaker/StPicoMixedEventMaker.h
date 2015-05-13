#ifndef StPicoMixedEventMaker_h
#define StPicoMixedEventMaker_h

#include "StMaker.h"

/* **************************************************
 *  Base class for Mixed Event cosntructions
 *  details to come
 *
 * **************************************************
 *
 *  Initial Authors:
 *            Michael Lomnitz  (mrlomnitz@lbl.gov)
 *            Mustaga Mustafa  (mmustafa@lbl.gov) 
 *
 *  ** Code Maintainer
 * 
 *
 * **************************************************  
 */

class TTree;
class TFile;
class TChain;

class StPicoDst;
class StPicoDstMaker;
class StPicoEvent;
class StPicoTrack;
class StPicoMixedEvent;

class StPicoMixedEventMaker : public StMaker 
{
  public:
    StPicoMixedEventMaker(char const* name, StPicoDstMaker* picoMaker, char const* outputBaseFileName,  
		       char const* inputHFListHFtree);
    virtual ~StPicoMixedEventMaker();
    Int_t SetCategories();
 private:
    // -- Inhertited from StMaker 
    //    NOT TO BE OVERWRITTEN by daughter class
    //    daughter class should implement xxxHF()
    Int_t Init();
    Int_t Make();
    bool LoadEventPlaneCorr(Int_t const run);
    void  Clear(Option_t *opt="");
    Int_t Finish();

    // -- private members  ------------------------
    StPicoDst      *mPicoDst;
    StPicoDstMaker* mPicoDstMaker;      // ptr to picoDst maker
    StPicoEvent*    mPicoEvent;         // ptr to picoDstEvent
    StPicoMixedEvent* mPicoMixedEvent;

    TString         mOuputFileBaseName; // base name for output files
    TString         mInputFileName;     // filename of input list of HF trees (needs to be in the 
                                        // same order as the picoDstList    
    TTree*          mTree;              // tree holding "mPicoHFEvent" for writing only

    int             mRunId;
    int             mEventCounter;      // n Processed events in chain

    TFile*          mOutputFileTree;    // ptr to file saving the HFtree
    //TFile*          mOutputFileList;    // ptr to file saving the list of histograms
    // Vector of vectors to save categories
    //vector<TObject *> mCategories;
    ClassDef(StPicoMixedEventMaker, 1)
};
#endif
