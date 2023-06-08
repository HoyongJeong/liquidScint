#ifndef LSPRIGENACT_h
#define LSPRIGENACT_h 1

////////////////////////////////////////////////////////////////////////////////
//   LSPriGenAct.hh
//
//   This file is a header for LSPriGenAct class. You can set primary beam
// options in this class.
//
//     - Hoyong Jeong (hoyong5419@korea.ac.kr), Eunil Won (eunilwon@korea.ac.kr)
////////////////////////////////////////////////////////////////////////////////

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"

class LSPriGenAct: public G4VUserPrimaryGeneratorAction
{
  public:
	LSPriGenAct();
	~LSPriGenAct();

	virtual void GeneratePrimaries(G4Event* anEvent);

  private:
	G4ParticleGun*   m_PG;
	G4ParticleTable* m_PT;

	G4double m_BeamPX, m_BeamPY, m_BeamPZ;
	G4double m_WorldZ;
	G4ThreeVector m_GunPos;
	G4ParticleDefinition* m_Par;
	G4ThreeVector m_MomDir;
	G4double m_Mom;
	G4double m_KinEgy;
};

#endif
