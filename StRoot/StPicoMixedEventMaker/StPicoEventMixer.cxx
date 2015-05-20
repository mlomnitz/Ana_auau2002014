#include "StPicoEventMixer.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoDst.h"

#include "StPicoHFMaker/StHFCuts.h"

#include "StMixerTrack.h"
#include "StMixerEvent.h"
#include "StMixerPair.h"
#include "StMixerTriplet.h"
#include <vector>

StPicoEventMixer::StPicoEventMixer(): mEvents(0), mEventsBuffer(std::numeric_limits<int>::min()), filledBuffer(0)
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
  bool isTpcPi = false;
  bool isTofPi = false;
  bool isTpcK = false;
  bool isTofK = false;
  StMixerTrack *metrk = new StMixerTrack(picoTrack, isTpcPi, isTofPi, isTpcK, isTofK);
  a.addTrack(metrk);
  return;
}
void StPicoEventMixer::mixEvents(){
  short int const nEvent = mEvents.size();
  int const nTracksEvt1 = mEvents.at(0).getNoTracks();

  for( int iEvt2 = 1; iEvt2 < nEvent; iEvt2++){
    int const nTracksEvt2 = mEvents.at(iEvt2).getNoTracks();

    for( int iTrk1 = 0; iTrk1 < nTracksEvt1; iTrk1++){
      for( int iTrk2 = 0; iTrk2 < nTracksEvt2; iTrk2++){
	StMixerTrack *pion = mEvents.at(0).trackAt(iTrk1);
	//Select pions from first event
	if( !isPion(mEvents.at(0).trackAt(iTrk1)) ) continue;
	StMixerTrack *kaon = mEvents.at(iEvt2).trackAt(iTrk2);
	//Select Kaons from other events
	if ( !isKaon(mEvents.at(iEvt2).trackAt(iTrk2)) ) continue;
	StMixerPair *pair = new StMixerPair(mEvents.at(0).trackAt(iTrk1), mEvents.at(iEvt2).trackAt(iTrk2),
					    StHFCuts::kPion, StHFCuts::kPion,
					    mEvents.at(0).vertex(), mEvents.at(0).field() );
	//And now? Need to determine what and how it is going to be saved
      } //second event track loop
    } //first event track loop 
  } //loop over second events
  mEvents.erase(mEvents.begin());
  //mEvents.erase(mEvents.begin());
  return;
}
bool StPicoEventMixer::isPion(StMixerTrack track){
  short info = track.getTrackInfo();
  //TPC pion
  if( (info & 2) >> 1 != 1) return false;
  //TOF pion
  if( (info & 4) >> 2 != 1) return false;
  return true;
}

bool StPicoEventMixer::isKaon(StMixerTrack track){
  short info = track.getTrackInfo();
  //TPC Kaon
  if( (info & 8) >> 3 != 1) return false;
  //TOF Kaon
  if( (info & 16) >> 4 != 1) return false;
  return true;
}
