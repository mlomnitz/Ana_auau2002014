#include "StMixerTrack.h"
#include <limits>

StMixerTrack::StMixerTrack() : mTrackInfo(std::numeric_limits<short>::min()), mMomX(std::numeric_limits<float>::max()),
			      mMomY(std::numeric_limits<float>::max()), mMomZ(std::numeric_limits<float>::max())
{
}
StMixerTrack::StMixerTrack(StMixerTrack const * t) : mTrackInfo(t->mTrackInfo), mMomX(t->mMomX),
			      mMomY(t->mMomY), mMomZ(t->mMomZ)
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
  mMomX = px;
  mMomY = py;
  mMomZ = pz;
}

