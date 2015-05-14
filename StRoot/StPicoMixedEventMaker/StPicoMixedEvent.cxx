#include "StPicoMixedEvent.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StMixedEventTriplet.h"

ClassImp(StPicoMixedEvent)

StPicoMixedEvent::StPicoMixedEvent(): mEventsBuffer(std::numeric_limits<int>::min()), mEvents(NULL)
{
  InitMixedEvent();
  return;
}

void StPicoMixedEvent::addPicoEvent(StPicoEvent* picoEvent)
{
  mEvents->push_back(picoEvent);
  return;
}
void StPicoMixedEvent::addPicoTrack(StPicoTrack* picoTrack, float bField)
{
  return;
}
void StPicoMixedEvent::InitMixedEvent(){
  setEventBuffer(11);
  mEvents = new vector< StPicoEvent* >;
  return;
}
void StPicoMixedEvent::FinishMixedEvent(){
  return;
}
void StPicoMixedEvent::mixEvents(){
  return;
}
