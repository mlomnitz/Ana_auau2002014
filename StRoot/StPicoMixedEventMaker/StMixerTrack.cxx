#include "StMixerTrack.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StThreeVectorF.hh"
#include <limits>

StMixerTrack::StMixerTrack() : mOrigin(StThreeVectorF()), mMom(StThreeVectorF()), mTrackInfo(std::numeric_limits<short>::min())
{
}
StMixerTrack::StMixerTrack(StPicoTrack const *picoTrack, bool isTpcPi, bool isTofPi, bool isTpcK, bool isTofK) : mOrigin(StThreeVectorF()), mMom(StThreeVectorF()), mTrackInfo(std::numeric_limits<short>::min())
{

  mOrigin = picoTrack->helix().origin();
  mMom = picoTrack->gMom();
  
  if( picoTrack->charge() > 0 ) mTrackInfo = mTrackInfo | (1 << 1);
  //Pi
  if( isTpcPi == true ) mTrackInfo = mTrackInfo | (1 << 2);
  if( isTofPi == true ) mTrackInfo = mTrackInfo | (1 << 3);
  //K
  if( isTpcK == true ) mTrackInfo = mTrackInfo | (1 << 4);
  if( isTofK == true ) mTrackInfo = mTrackInfo | (1 << 5);

}
StMixerTrack::StMixerTrack(StMixerTrack const * t) : mOrigin(t->mOrigin), mMom(t->mMom), mTrackInfo(t->mTrackInfo)
{
}

