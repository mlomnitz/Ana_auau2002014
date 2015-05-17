#include "StMixerEvent.h"
#include "StMixerTrack.h"
#include <limits>
StMixerEvent::StMixerEvent() :  mBField(std::numeric_limits<float>::quiet_NaN()), mVx(std::numeric_limits<float>::quiet_NaN()),
				mVy(std::numeric_limits<float>::quiet_NaN()),  mVz(std::numeric_limits<float>::quiet_NaN()),
				mEventTracks(0),mNoTracks(std::numeric_limits<int>::quiet_NaN())
{
}
StMixerEvent::StMixerEvent(StMixerEvent *t) : mBField(t->mBField),
					      mVx(t->mVx),  mVy(t->mVy),  mVz(t->mVz),
					      mEventTracks(t->mEventTracks),mNoTracks(t->mNoTracks)
{
}
void StMixerEvent::addTrack(StMixerTrack *t)
{
  mEventTracks.push_back(t);
  return;
}
