#include "StPicoEventMixer.h"
#include "StPicoDstMaker/StPicoEvent.h"
#include "StPicoDstMaker/StPicoTrack.h"
#include "StPicoDstMaker/StPicoDst.h"

#include "StPicoHFMaker/StHFCuts.h"
#include "StPicoMixedEventMaker.h"

#include "StMixerEvent.h"
#include "StMixerPair.h"
#include "StMixerTriplet.h"
#include <vector>

StPicoEventMixer::StPicoEventMixer(): mEvents(), mEventsBuffer(std::numeric_limits<int>::min()), filledBuffer(0)
{
  StMemStat mem;
  InitMixedEvent();
  return;
}
void StPicoEventMixer::InitMixedEvent(){
  setEventBuffer(3);
  //mBackground = new TH1F("bgMass","Mixed Event Invariant mass",500,0,5);
  int BufSize = (int)pow(2., 16.);
  //ntp_ME = new TNtuple("ntp_ME","MixedEvent Tree","dca1:dca2:dcaDaughters:"		       
  //"theta_hs:decayL_hs:pt_hs:mass_hs:eta_hs:phi_hs:",BufSize);
  ntp_ME = new TTree("ntp_ME","MixedEvent",BufSize);
  ntp_ME ->SetAutoSave(1000000);
  ntp_ME->Branch("dca1",&dca1,"dca1/F");
  ntp_ME->Branch("dca1",&dca2,"dca2/F");
  ntp_ME->Branch("dcaDaughters",&dcaDaughters,"dcaDaughters/F");
  ntp_ME->Branch("theta_hs",&theta_hs,"theta_hs/F");
  ntp_ME->Branch("decayL_hs",&decayL_hs,"decayL_hs/F");
  ntp_ME->Branch("pt_hs",&pt_hs,"pt_hs/F");
  ntp_ME->Branch("mass_hs",&mass_hs,"mass_hs/F");
  ntp_ME->Branch("eta_hs",&eta_hs,"eta_hs/F");
  ntp_ME->Branch("phi_hs",&phi_hs,"phi_hs/F");

  return;
}
void StPicoEventMixer::FinishMixedEvent(){
  //mBackground -> Write();
  //ntp_ME->Write("anyName",TObject::kSingleKey);
  ntp_ME->Write();
  return;
}
bool StPicoEventMixer::addPicoEvent(const StPicoDst * picoDst, StHFCuts *mHFCuts)
{
  int nTracks = picoDst->numberOfTracks();
  StThreeVectorF pVertex = picoDst->event()->primaryVertex();
  StMixerEvent *Event = new StMixerEvent(pVertex, picoDst->event()->bField());

  bool isTpcPi = false;
  bool isTofPi = false;
  bool isTpcK = false;
  bool isTofK = false;
  //Event.setNoTracks( nTracks );
  for( int iTrk = 0; iTrk < nTracks; iTrk++ ){
    StPicoTrack * trk = picoDst->track(iTrk);
    if( mHFCuts->isTPCPion(trk) && mHFCuts->isGoodTrack(trk)){
      isTpcPi = true;
      isTofPi = true;
    }
    if( mHFCuts->isTPCKaon(trk) && mHFCuts->isGoodTrack(trk)){
      isTpcK = true;
      isTofK = true;
    }
    if(!isTpcK && !isTpcPi) continue;
    StMixerTrack *mTrack = new StMixerTrack(trk, isTpcPi, isTofPi, isTpcK, isTofK);
    Event->addTrack(mTrack);
    delete mTrack;
  } 
  if ( nTracks > 0 ){
    mEvents.push_back(Event);
    filledBuffer+=1;
  }
  else {
    delete Event;
    return false;
  }
  //Returns true if need to do mixing, false if buffer has space still
  if ( filledBuffer == mEventsBuffer - 1 )
    return true;
  return false;
}  
void StPicoEventMixer::mixEvents(StHFCuts *mHFCuts){
  short int const nEvent = mEvents.size();
  int const nTracksEvt1 = mEvents.at(0)->getNoTracks();
  for( int iEvt2 = 1; iEvt2 < nEvent; iEvt2++){
    int const nTracksEvt2 = mEvents.at(iEvt2)->getNoTracks();
    for( int iTrk1 = 0; iTrk1 < nTracksEvt1; iTrk1++){
      StMixerTrack *pion = new StMixerTrack(mEvents.at(0)->trackAt(iTrk1));

      if( !isMixerPion(pion) ) {
	delete pion;
	continue;
      }
      for( int iTrk2 = 0; iTrk2 < nTracksEvt2; iTrk2++){
	StMixerTrack *pion2 = new StMixerTrack(mEvents.at(iEvt2)->trackAt(iTrk2));
	if ( !isMixerPion(pion2) ) {
	  delete pion2;
	  continue;
	}
	if( pion2->charge() == pion->charge() ) {
	  delete pion2;
	  continue;
	}

	StMixerPair *pair = new StMixerPair(pion, pion2,
					    StHFCuts::kPion, StHFCuts::kPion,
					    mEvents.at(0)->vertex(), mEvents.at(iEvt2)->vertex(),
					    mEvents.at(0)->field() );
	if( !mHFCuts->isGoodMixerPair(pair) )
	  fill(pair);
	delete pair;
	delete pion2;

      } //second event track loop
      delete pion;
    } //first event track loop 
  } //loop over second events
  filledBuffer--;
  delete mEvents.at(0)	;
  mEvents.erase(mEvents.begin());
  return;
} 
// _________________________________________________________
bool StPicoEventMixer::isMixerPion(StMixerTrack track){
  short info = track.getTrackInfo();
  //TPC pion
  if( (info & 2) >> 1 != 1) return false;
  //TOF pion
  if( (info & 4) >> 2 != 1) return false;
  return true;
}
// _________________________________________________________
bool StPicoEventMixer::isMixerKaon(StMixerTrack track){
  short info = track.getTrackInfo();
  //TPC Kaon
  if( (info & 8) >> 3 != 1) return false;
  //TOF Kaon
  if( (info & 16) >> 4 != 1) return false;
  return true;
}
// _________________________________________________________
void StPicoEventMixer::fill(StMixerPair const * const pair){
  //mBackground -> Fill(pair->m());
  
  dca1 = pair->particle1Dca();
  dca2 = pair->particle1Dca();
  dcaDaughters = pair->dcaDaughters();
  theta_hs = pair->pointingAngle();
  decayL_hs = pair->decayLength();
  pt_hs = pair->pt();
  mass_hs = pair->m();
  eta_hs = pair->eta();
  phi_hs = pair->phi();
  ntp_ME->Fill();
  return;
}
