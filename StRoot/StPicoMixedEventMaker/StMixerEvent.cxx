#include "StMixerEvent.h"
#include "StMixerTrack.h"

#include <limits>
StMixerEvent::StMixerEvent() :  mVtx(StThreeVectorF()), mEventTracks(0),
				mBField(std::numeric_limits<float>::quiet_NaN()),mNoTracks(std::numeric_limits<int>::quiet_NaN())
{
}
StMixerEvent::StMixerEvent(StMixerEvent *t) : mVtx(t->mVtx), mEventTracks(t->mEventTracks),
				mBField(t->mBField), mNoTracks(t->mNoTracks)
{
}
void StMixerEvent::addTrack(StMixerTrack *t)
{
  mEventTracks.push_back(t);
  return;
}