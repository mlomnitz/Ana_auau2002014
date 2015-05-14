#ifndef StPicoMixedEvent_hh
#define StPicoMixedEvent_hh

#include "TObject.h"
#include <vector>
#include <limits>

class StPicoEvent;
class StPicoTrack;
class StMixedEventTriplet;

class StPicoMixedEvent : public TObject {
 public: 
  StPicoMixedEvent();
  ~StPicoMixedEvent(){;}
  void addPicoEvent(StPicoEvent* picoEvent);
  void addPicoTrack(StPicoTrack* picoTrack, float bField);
  void setEventBuffer(Int_t buffer);
  void mixEvents();
 private:
  void InitMixedEvent();
  void FinishMixedEvent();
  unsigned short int mEventsBuffer; //Nevents in buffers
  std::vector<StPicoEvent* > * mEvents;
  
  ClassDef(StPicoMixedEvent,1)
};

inline void StPicoMixedEvent::setEventBuffer(Int_t buffer){ mEventsBuffer = buffer;}
#endif
