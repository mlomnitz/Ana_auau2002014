#ifndef StMixerTrack_hh
#define StMixerTrack_hh

#include <math.h>
#include "StThreeVectorF.hh"

class StMixerTrack{
 public:
  StMixerTrack();
  StMixerTrack(StMixerTrack const *);
  void setTrackInfo(bool const, bool const, bool const, bool const, bool const);
  void setTrackMom(float const , float const , float const);
  void setTrackMom(StThreeVectorF const & );
  short const getTrackInfo() const;
  int const charge() const ;
  StThreeVectorF const &gMom() const;
  ~StMixerTrack(){;};
 private:
  short mTrackInfo;
  StThreeVectorF mMom;
  //Removed origin, allt racks shoud me set to 0,0,0
};
inline short const StMixerTrack::getTrackInfo() const { return(mTrackInfo); }
inline StThreeVectorF const & StMixerTrack::gMom() const { return(mMom) ;}
inline int const StMixerTrack::charge() const { 
  int temp = (mTrackInfo & 1);
  if(temp ==1) return 1;
  else return -1;
}
#endif
