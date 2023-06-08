#ifndef LSDETCON_h
#define LSDETCON_h 1

////////////////////////////////////////////////////////////////////////////////
//   LSDetCon.hh
//
//   This file is a header for LSDetCon class. It's for construction of whole
// geometry of simulation, which includes target geometry and detector geometry.
//
//     - Hoyong Jeong (hoyong5419@korea.ac.kr), Eunil Won (eunilwon@korea.ac.kr)
////////////////////////////////////////////////////////////////////////////////

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4UnionSolid.hh"

class G4VPhysicalVolume;

class LSDetCon: public G4VUserDetectorConstruction
{
  public:
	LSDetCon();
	virtual ~LSDetCon();
	virtual G4VPhysicalVolume* Construct();

  private:
	void DefineDimensions();
	void ConstructMaterials();
	void DestructMaterials();

  private:
	// Dimensions and detector setup
	G4double m_WorldX, m_WorldY, m_WorldZ;
	G4double m_LabX, m_LabY, m_LabZ;
	G4double m_LSX, m_LSY, m_LSZ;
	G4double m_PMTD, m_PMTT;

	// Geometry objects: World
	G4Box* m_WorldSolid;
	G4LogicalVolume* m_WorldLV;
	G4VPhysicalVolume* m_WorldPV;

	// Geometry objects: Lab
	G4Box* m_LabSolid;
	G4LogicalVolume* m_LabLV;
	G4VPhysicalVolume* m_LabPV;

	// Geometry objects: LS box
	G4Box* m_LSSolid1;
	G4Tubs* m_LSSolid2;
	G4UnionSolid* m_LSSolid;
	G4LogicalVolume* m_LSLV;
	G4VPhysicalVolume* m_LSPV;

	// Geometry objects: PMT
	G4Tubs* m_PMTSolid;
	G4LogicalVolume* m_PMTLV;
	G4VPhysicalVolume* m_PMTPV;

	// Surface objects: LS box
	G4OpticalSurface* m_LSOpS;
	G4LogicalBorderSurface* m_LSLBS;

	// Surface objects: Air
	G4OpticalSurface* m_AirOpS;
	G4LogicalSkinSurface* m_AirLSS;

	// Materials
	G4Material* m_AirMat;
	G4Material* m_LSMat;
	G4Material* m_PMTMat;
	G4MaterialPropertiesTable* m_LSMPT;
	G4MaterialPropertiesTable* m_AirMPT;
	G4MaterialPropertiesTable* m_AirST;

};
#endif
