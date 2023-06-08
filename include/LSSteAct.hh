#ifndef LSSTEACT_h
#define LSSTEACT_h 1

////////////////////////////////////////////////////////////////////////////////
//   LSSetAct.hh
//
//   This file is a header for LSSteAct class. User can add user-defined
// stepping action in this class. So this class works at every step.
// The most busiest class.
//
//     - Hoyong Jeong (hoyong5419@korea.ac.kr), Eunil Won (eunilwon@korea.ac.kr)
////////////////////////////////////////////////////////////////////////////////

#include "G4UserSteppingAction.hh"

class LSSteAct: public G4UserSteppingAction
{
  public:
	LSSteAct();
	virtual ~LSSteAct();

	virtual void UserSteppingAction(const G4Step*);
};

#endif
