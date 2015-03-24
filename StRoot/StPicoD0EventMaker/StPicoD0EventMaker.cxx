#include <vector>
#include <cmath>

#include "TTree.h"
#include "TFile.h"
#include "StThreeVectorF.hh"
#include "StLorentzVectorF.hh"
#include "../StPicoDstMaker/StPicoDst.h"
#include "../StPicoDstMaker/StPicoDstMaker.h"
#include "../StPicoDstMaker/StPicoEvent.h"
#include "../StPicoDstMaker/StPicoTrack.h"
#include "../StPicoDstMaker/StPicoBTofPidTraits.h"
#include "StPicoD0Event.h"
#include "StPicoD0EventMaker.h"
#include "StCuts.h"

ClassImp(StPicoD0EventMaker)

//-----------------------------------------------------------------------------
StPicoD0EventMaker::StPicoD0EventMaker(char const* name, StPicoDstMaker* picoMaker, char const* outName)
   : StMaker(name), mPicoDstMaker(picoMaker), mPicoDst(NULL), mPicoEvent(NULL)
{
   mPicoD0Event = new StPicoD0Event();

   mOutputFile = new TFile(outName, "RECREATE");
   mOutputFile->SetCompressionLevel(1);
   int BufSize = (int)pow(2., 16.);
   int Split = 1;
   mTree = new TTree("T", "T", BufSize);
   mTree->SetAutoSave(1000000); // autosave every 1 Mbytes
   mTree->Branch("dEvent", "StPicoD0Event", &mPicoD0Event, BufSize, Split);
}

//-----------------------------------------------------------------------------
StPicoD0EventMaker::~StPicoD0EventMaker()
{
   /* mTree is owned by mOutputFile directory, it will be destructed once
    * the file is closed in ::Finish() */
}

//-----------------------------------------------------------------------------
Int_t StPicoD0EventMaker::Init()
{
   return kStOK;
}

//-----------------------------------------------------------------------------
Int_t StPicoD0EventMaker::Finish()
{
   mOutputFile->cd();
   mOutputFile->Write();
   mOutputFile->Close();
   return kStOK;
}
//-----------------------------------------------------------------------------
void StPicoD0EventMaker::Clear(Option_t *opt)
{
   mPicoD0Event->clear("C");
}

//-----------------------------------------------------------------------------
Int_t StPicoD0EventMaker::Make()
{
   if (!mPicoDstMaker)
   {
      LOG_WARN << " No PicoDstMaker! Skip! " << endm;
      return kStWarn;
   }

   mPicoDst = mPicoDstMaker->picoDst();
   if (!mPicoDst)
   {
      LOG_WARN << " No PicoDst! Skip! " << endm;
      return kStWarn;
   }

   mPicoEvent = mPicoDst->event();
   mPicoD0Event->addPicoEvent(*mPicoEvent);

   if (isGoodEvent())
   {
      UInt_t nTracks = mPicoDst->numberOfTracks();

      std::vector<unsigned short> idxPicoKaons;
      std::vector<unsigned short> idxPicoPions;

      for (unsigned short iTrack = 0; iTrack < nTracks; ++iTrack)
      {
         StPicoTrack* trk = mPicoDst->track(iTrack);

         if (!trk || !isGoodTrack(trk)) continue;

         if (isPion(trk)) idxPicoPions.push_back(iTrack);
         if (isKaon(trk)) idxPicoKaons.push_back(iTrack);

      } // .. end tracks loop

      float const bField = mPicoEvent->bField();
      StThreeVectorF const pVtx = mPicoEvent->primaryVertex();

      for (unsigned short ik = 0; ik < idxPicoKaons.size(); ++ik)
      {
         StPicoTrack const * kaon = mPicoDst->track(idxPicoKaons[ik]);

         // make Kπ pairs
         for (unsigned short ip = 0; ip < idxPicoPions.size(); ++ip)
         {
            if (idxPicoKaons[ik] == idxPicoPions[ip]) continue;

            StPicoTrack const * pion = mPicoDst->track(idxPicoPions[ip]);

            StKaonPion kaonPion(kaon, pion, idxPicoKaons[ik], idxPicoPions[ip], pVtx, bField);

            if (!isGoodPair(kaonPion)) continue;

            mPicoD0Event->addKaonPion(&kaonPion);
         } // .. end make Kπ pairs
      } // .. end of kaons loop

      idxPicoKaons.clear();
      idxPicoPions.clear();
   } //.. end of good event fill

   // This should never be inside the good event block
   // because we want to save header information about all events, good or bad
   mTree->Fill();
   mPicoD0Event->clear("C");

   return kStOK;
}

//-----------------------------------------------------------------------------
bool StPicoD0EventMaker::isGoodEvent()
{
   return (mPicoEvent->triggerWord() & cuts::triggerWord) &&
          fabs(mPicoEvent->primaryVertex().z()) < cuts::vz &&
          fabs(mPicoEvent->primaryVertex().z() - mPicoEvent->vzVpd()) < cuts::vzVpdVz;
}
//-----------------------------------------------------------------------------
bool StPicoD0EventMaker::isGoodTrack(StPicoTrack const * const trk) const
{
   // Require at least one hit on every layer of PXL and IST.
   // It is done here for tests on the preview II data.
   // The new StPicoTrack which is used in official production has a method to check this
   return (!cuts::requireHFT || trk->nHitsMapHFT() & 0xB) && 
          trk->nHitsFit() >= cuts::nHitsFit;
}
//-----------------------------------------------------------------------------
bool StPicoD0EventMaker::isPion(StPicoTrack const * const trk) const
{
   return fabs(trk->nSigmaPion()) < cuts::nSigmaPion;
}
//-----------------------------------------------------------------------------
bool StPicoD0EventMaker::isKaon(StPicoTrack const * const trk) const
{
   return fabs(trk->nSigmaKaon()) < cuts::nSigmaKaon;
}
//-----------------------------------------------------------------------------
bool StPicoD0EventMaker::isGoodPair(StKaonPion const & kp) const
{
   return kp.m() > cuts::minMass && kp.m() < cuts::maxMass &&
          std::cos(kp.pointingAngle()) > cuts::cosTheta &&
          kp.decayLength() > cuts::decayLength &&
          kp.dcaDaughters() < cuts::dcaDaughters;
}