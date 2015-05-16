#include "StMixerTrack.h"
#include "StThreeVectorF.hh"
#include <limits>

StMixerTrack::StMixerTrack() : mTrackInfo(std::numeric_limits<short>::min()), mMom(StThreeVectorF())
{
}
StMixerTrack::StMixerTrack(StMixerTrack const * t) : mTrackInfo(t->mTrackInfo), mMom(t->mMom)
{
}
void StMixerTrack::setTrackInfo(bool charge, bool isTpcPi, bool isTofPi, bool isTpcK, bool isTofK)
{
  if( charge == true ) mTrackInfo = mTrackInfo | (1 << 1);
  //Pi
  if( isTpcPi == true ) mTrackInfo = mTrackInfo | (1 << 2);
  if( isTofPi == true ) mTrackInfo = mTrackInfo | (1 << 3);
  //K
  if( isTpcK == true ) mTrackInfo = mTrackInfo | (1 << 4);
  if( isTofK == true ) mTrackInfo = mTrackInfo | (1 << 5);
  
}
void StMixerTrack::setTrackMom(float const px, float const py, float const pz){
  StThreeVectorF p(px, py, pz);
  mMom = p;
}
void StMixerTrack::setTrackMom(StThreeVectorF const & p ){
  mMom = p;
}
