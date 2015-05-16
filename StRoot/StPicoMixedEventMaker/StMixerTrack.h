#ifndef StMixerTrack_hh
#define StMixerTrack_hh

#include <math.h>

class StMixerTrack{
 public:
  StMixerTrack();
  StMixerTrack(StMixerTrack const *);
  void setTrackInfo(bool const, bool const, bool const, bool const, bool const);
  void setTrackMom(float const , float const , float const);
  short getTrackInfo();
  ~StMixerTrack(){;};
 private:
  short mTrackInfo;
  float mMomX, mMomY, mMomZ;
  //Removed origin, allt racks shoud me set to 0,0,0
};
inline short StMixerTrack::getTrackInfo(){ return(mTrackInfo); }
#endif
