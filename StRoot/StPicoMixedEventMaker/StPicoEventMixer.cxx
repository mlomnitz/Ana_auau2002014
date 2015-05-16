#include "StPicoEventMixer.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoDst.h"
#include "StMixedEventTriplet.h"
#include "StMixerTrack.h"
#include "StMixerEvent.h"
#include <vector>

ClassImp(StPicoEventMixer)

StPicoEventMixer::StPicoEventMixer(): mEventsBuffer(std::numeric_limits<int>::min()), filledBuffer(0),mEvents(0)
{
  InitMixedEvent();
  return;
}
void StPicoEventMixer::InitMixedEvent(){
  setEventBuffer(11);
  return;
}
void StPicoEventMixer::FinishMixedEvent(){
  return;
}
bool StPicoEventMixer::addPicoEvent(const StPicoDst * picoDst)
{
  //Should also set categories, still pending
  StMixerEvent Event;
  StPicoEvent * picoEvent = picoDst->event();
  int nTracks = picoEvent->numberOfGlobalTracks();
  StThreeVectorF pVertex = picoEvent->primaryVertex();
  
  Event.setPos( pVertex.x(), pVertex.y(), pVertex.z() );
  Event.setField( picoEvent->bField() );
  Event.setNoTracks( nTracks );
  for( int iTrk = 0; iTrk < nTracks; iTrk++ ){
    StPicoTrack * trk = picoDst->track(iTrk);
    addPicoTrack(Event,trk);
  } 
  if ( nTracks > 0 ){
    mEvents.push_back(Event);
    filledBuffer+=1;
  }
  //Returns true if need to do mixing, false if buffer has space still
  if ( filledBuffer == mEventsBuffer )
    return true;
  return false;
}  
void StPicoEventMixer::addPicoTrack(StMixerEvent a, StPicoTrack const * picoTrack)
{
  StThreeVectorF p = picoTrack -> gMom();
  StMixerTrack *metrk = new StMixerTrack();
  metrk->setTrackMom(p.x(), p.y(), p.z());
  //Lomnitz :: Get track TOF, TPC and charge 
  bool isTpcPi = 0;
  bool isTofPi = 0;
  bool isTpcK = 0;
  bool isTofK = 0;
  metrk->setTrackInfo(picoTrack->charge(), isTpcPi, isTofPi, isTpcK, isTofK);
  a.addTrack(metrk);
  return;
}
void StPicoEventMixer::mixEvents(){
  //Should this go here?
  short int const nEvent = mEvents.size();

      
  return;
}
