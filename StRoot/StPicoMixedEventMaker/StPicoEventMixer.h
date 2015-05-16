#ifndef StPicoEventMixer_hh
#define StPicoEventMixer_hh

#include "TObject.h"
#include <vector>
#include <limits>
#include <iostream>
#include "StMixerEvent.h"

class StPicoEvent;
class StPicoTrack;
class StPicoDst;
class StMixerTrack;
class StMixerEvent;
class StMixedEventTriplet;

class StPicoEventMixer {
 public: 
  StPicoEventMixer();
  ~StPicoEventMixer(){;};
  bool addPicoEvent(const StPicoDst *picoDst);
  void addPicoTrack(StMixerEvent ,StPicoTrack const * );
  void setEventBuffer(Int_t buffer);
  void mixEvents();
 private:
  void InitMixedEvent();
  void FinishMixedEvent();
  unsigned short int mEventsBuffer; //Nevents in buffers
  unsigned short int filledBuffer;
  std::vector < StMixerEvent > mEvents; //Needs to be generalized to more categories Using TCLones array
  ClassDef(StPicoEventMixer,1)
};

inline void StPicoEventMixer::setEventBuffer(Int_t buffer){ mEventsBuffer = buffer;}
#endif
