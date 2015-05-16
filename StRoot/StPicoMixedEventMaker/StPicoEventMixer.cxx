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
  short int const nEvent = mEvents.size();
  int const nTracksEvt1 = mEvents.at(0).getNoTracks();

  for( int iEvt2 = 1; iEvt2 < nEvent; iEvt2++){
    int const nTracksEvt2 = mEvents.at(iEvt2).getNoTracks();

    for( int iTrk1 = 0; iTrk1 < nTracksEvt1; iTrk1++){
      for( int iTrk2 = 0; iTrk2 < nTracksEvt2; iTrk2++){
	StMixerTrack *pion = mEvents.at(0).trackAt(iTrk1);
	StMixerTrack *kaon = mEvents.at(iEvt2).trackAt(iTrk2);
	//Select pions from first event
	if( !isPion(mEvents.at(0).trackAt(iTrk1)) ) continue;
	//Select Kaons from other events
	if ( !isKaon(mEvents.at(iEvt2).trackAt(iTrk2)) ) continue;
	//Lomnitz make pair here
	
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
