#ifndef StMixerEvent_hh
#define StMixerEvent_hh
#include <math.h>
#include <vector>

class StMixerTrack;

class StMixerEvent{
 public:
  StMixerEvent();
  StMixerEvent(StMixerEvent *);
  ~StMixerEvent(){;};
  void addTrack(StMixerTrack *);
  void setPos( float const, float const, float const);
  void setField( float const );
  void setNoTracks( int const );
  int getNoTracks();
  StMixerTrack * trackAt(int const); 
 private:
  int mNoTracks;
  float mBField;
  float mVx, mVy, mVz;
  std::vector <StMixerTrack * > mEventTracks;
};
inline void StMixerEvent::setPos( float const vx, float const vy, float const vz){
  mVx = vx; mVy = vy; mVz = vz;
}
inline void StMixerEvent::setField( float const field ){ mBField = field; }
inline void StMixerEvent::setNoTracks( int const noTracks ){ mNoTracks = noTracks; }
inline int StMixerEvent::getNoTracks(){ return mEventTracks.size(); }
inline StMixerTrack * StMixerEvent::trackAt(int const counter) { return(mEventTracks.at(counter));} 
#endif
