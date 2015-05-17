#ifndef StMixedEventTriplet_hh
#define StMixedEventTriplet_hh

#include "TObject.h"

#include "StLorentzVectorF.hh"
#include "StThreeVectorF.hh"

class StPicoTrack;
class StPicoEvent;

class StMixedEventTriplet : public TObject
{
 public:
  StMixedEventTriplet();
  StMixedEventTriplet(StMixedEventTriplet const *);
  StMixedEventTriplet(StPicoTrack const * particle1, StPicoTrack const * particle2, StPicoTrack const * particle3, 
	      float p1MassHypo, float p2MassHypo, float p3MassHypo,
	      StThreeVectorF const & vtx, float bField);
  ~StMixedEventTriplet() {;}

  StPicoTrack const * particleTrack(int nTrack);
  float m()    const;
  float pt()   const;
  float eta()  const;
  float phi()  const;
  float pointingAngle() const;
  float decayLength() const;
  float particle1Dca() const;
  float particle2Dca() const;
  float particle3Dca() const;
  float dcaDaughters12() const;
  float dcaDaughters23() const;
  float dcaDaughters31() const;
  StThreeVectorF const & decayVertex() const;
    
  float px() const;
  float py() const;
  float pz() const;
    
 private:
  StMixedEventTriplet(StMixedEventTriplet const &);
  StMixedEventTriplet& operator=(StMixedEventTriplet const &);
  
  StPicoTrack* mParticle1;
  StPicoTrack* mParticle2;
  StPicoTrack* mParticle3;
  
  StLorentzVectorF mLorentzVector; 
  StThreeVectorF   mDecayVertex;

  float mPointingAngle;
  float mDecayLength;

  float mParticle1Dca;
  float mParticle2Dca;
  float mParticle3Dca;

  float mDcaDaughters12;
  float mDcaDaughters23;
  float mDcaDaughters31;

  ClassDef(StMixedEventTriplet,2)
};
inline float StMixedEventTriplet::m()    const { return mLorentzVector.m();}
inline float StMixedEventTriplet::pt()   const { return mLorentzVector.perp();}
inline float StMixedEventTriplet::eta()  const { return mLorentzVector.pseudoRapidity();}
inline float StMixedEventTriplet::phi()  const { return mLorentzVector.phi();}
inline float StMixedEventTriplet::px()   const { return mLorentzVector.px();}
inline float StMixedEventTriplet::py()   const { return mLorentzVector.py();}
inline float StMixedEventTriplet::pz()   const { return mLorentzVector.pz();}
inline float StMixedEventTriplet::pointingAngle() const { return mPointingAngle;}
inline float StMixedEventTriplet::decayLength()   const { return mDecayLength;}
inline float StMixedEventTriplet::particle1Dca()  const { return mParticle1Dca;}
inline float StMixedEventTriplet::particle2Dca()  const { return mParticle2Dca;}
inline float StMixedEventTriplet::particle3Dca()  const { return mParticle3Dca;}
inline float StMixedEventTriplet::dcaDaughters12() const { return mDcaDaughters12;}
inline float StMixedEventTriplet::dcaDaughters23() const { return mDcaDaughters23;}
inline float StMixedEventTriplet::dcaDaughters31() const { return mDcaDaughters31;}
inline StThreeVectorF const & StMixedEventTriplet::decayVertex() const { return mDecayVertex;}

#endif
