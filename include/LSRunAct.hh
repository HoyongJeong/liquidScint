#ifndef LSRUNACT_h
#define LSRUNACT_h 1

////////////////////////////////////////////////////////////////////////////////
//   LSRunAct.hh
//
//   This file is a header for liquidScintRunAct class. In addition to the basic
// actions, user can add additional user-defined actions.
//
//     - Hoyong Jeong (hoyong5419@korea.ac.kr), Eunil Won (eunilwon@korea.ac.kr)
////////////////////////////////////////////////////////////////////////////////

#include "G4UserRunAction.hh"

class G4Run;

class LSRunAct: public G4UserRunAction
{
  public:
	LSRunAct();
	virtual ~LSRunAct();

	virtual void BeginOfRunAction(const G4Run*); 
	virtual void   EndOfRunAction(const G4Run*);
};

#endif
