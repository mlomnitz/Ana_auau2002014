#ifndef StPicoMixedEvent_hh
#define StPicoMixedEvent_hh

#include "TObject.h"

class StPicoEvent;

class StPicoMixedEvent : public TObject {
 public: 
  StPicoMixedEvent();
  ~StPicoMixedEvent(){;}
  void AddPicoEvent(StPicoEvent* picoEvent);
 private:

  ClassDef(StPicoMixedEvent,1)
};
#endif
