#ifndef LSACTINI_h
#define LSACTINI_h 1

////////////////////////////////////////////////////////////////////////////////
//   LSActIni.hh
//
//   This file is a header for LSActIni class. Every actions are initialized
// through this class.
//
//     - Hoyong Jeong (hoyong5419@korea.ac.kr), Eunil Won (eunilwon@korea.ac.kr)
////////////////////////////////////////////////////////////////////////////////

#include "G4VUserActionInitialization.hh"

class LSActIni: public G4VUserActionInitialization
{
  public:
	LSActIni();
	virtual ~LSActIni();

	virtual void BuildForMaster() const;
	virtual void Build() const;
};

#endif
