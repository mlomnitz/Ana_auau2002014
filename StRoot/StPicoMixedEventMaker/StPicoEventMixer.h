#ifndef StPicoEventMixer_hh
#define StPicoEventMixer_hh

/* **************************************************
 * Class stores event buffer used in event mixing. Mixing
 * is done automatically once buffer reaches defined maximum.
 * User should rpesonalize mixEvent() method to cosntruct 
 * desired background.
 *
 * **************************************************
 * 
 * Initial Authors:
 *          **Michael Lomnitz (mrlomnitz@lbl.gov)
 *          Musta Mustafa   (mmustafa@lbl.gov)
 *
 *  ** Code maintainer 
 *
 * **************************************************
 */

#include <vector>
#include <limits>
#include <iostream>
#include "StMixerEvent.h"

class StPicoEvent;
class StPicoTrack;
class StPicoDst;
class StMixerTrack;
class StMixerEvent;

class StPicoEventMixer {
 public: 
  StPicoEventMixer();
  ~StPicoEventMixer(){;};
  bool addPicoEvent(const StPicoDst *picoDst);
  void addPicoTrack(StMixerEvent ,StPicoTrack const * );
  void setEventBuffer(int buffer);
  void mixEvents();
 private:
  void InitMixedEvent();
  void FinishMixedEvent();
  bool isPion(StMixerTrack);
  bool isKaon(StMixerTrack);
  
  std::vector < StMixerEvent > mEvents; //Needs to be generalized to more categories Using TCLones array

  unsigned short int mEventsBuffer; 
  unsigned short int filledBuffer;

};

inline void StPicoEventMixer::setEventBuffer(int buffer){ mEventsBuffer = buffer;}
			    
    
#endif
