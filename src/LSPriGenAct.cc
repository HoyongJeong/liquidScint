////////////////////////////////////////////////////////////////////////////////
//   LSPriGenAct.cc
//
//   Definitions of LSPriGenAct class's member functions.
//
//     - Hoyong Jeong (hoyong5419@korea.ac.kr), Eunil Won (eunilwon@korea.ac.kr)
////////////////////////////////////////////////////////////////////////////////

#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

#include "LSPriGenAct.hh"

//////////////////////////////////////////////////
//   Constructor and destructor
//////////////////////////////////////////////////
LSPriGenAct::LSPriGenAct()
{
	m_PG = new G4ParticleGun();

	// Gun position
	m_WorldZ = 15. * m;
//	m_BeamPX = - m_WorldZ / 2.;
	m_BeamPX = 0.;
	m_BeamPY = 0.;
	m_BeamPZ = 0.;

	// Set particle definition
	m_PT = G4ParticleTable::GetParticleTable();
	m_Par = m_PT -> FindParticle("e-");
	m_PG -> SetParticleDefinition(m_Par);

	// Position
	m_PG -> SetParticlePosition(G4ThreeVector(m_BeamPX, m_BeamPY, m_BeamPZ));
//	m_PG -> SetParticlePosition(G4ThreeVector(0., 0., 0.));

	// Momentum
	m_MomDir = G4ThreeVector(1., 0., 0.);
	m_PG -> SetParticleMomentumDirection(m_MomDir);

	// Kinetic energy
	m_PG -> SetParticleEnergy(2. * MeV);
}

LSPriGenAct::~LSPriGenAct()
{
	delete m_PG;
}

//////////////////////////////////////////////////
//   Shoot!
//////////////////////////////////////////////////
void LSPriGenAct::GeneratePrimaries(G4Event* anEvent)
{
	m_PG -> GeneratePrimaryVertex(anEvent);
}
