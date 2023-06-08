////////////////////////////////////////////////////////////////////////////////
//   LSDetCon.cc
//
//   Definitions of LSDetCon class's member functions.
// And it describes geometry of simulation.
//
//     - Hoyong Jeong (hoyong5419@korea.ac.kr), Eunil Won (eunilwon@korea.ac.kr)
////////////////////////////////////////////////////////////////////////////////

#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "LSDetCon.hh"

//////////////////////////////////////////////////
//   Constructor and destructor
//////////////////////////////////////////////////
LSDetCon::LSDetCon()
{
	ConstructMaterials();
	DefineDimensions();
}

LSDetCon::~LSDetCon()
{
	DestructMaterials();
}

//////////////////////////////////////////////////
//   Define dimensions
//////////////////////////////////////////////////
void LSDetCon::DefineDimensions()
{

	// World dimensions
	m_WorldX = 15. * m;
	m_WorldY = 15. * m;
	m_WorldZ = 15. * m;

	// Lab dimensions
	m_LabX = 10. * m; 
	m_LabY = 10. * m;
	m_LabZ = 10. * m;

	// LS box
	m_LSX =  2. * m; 
	m_LSY =  2. * m;
	m_LSZ =  2. * m;

	// PMT
	m_PMTD = .2032 * m; // Equivalent to 8 inchs;
	m_PMTT = .2032 * m;
}

//////////////////////////////////////////////////
//   Construct everything
//////////////////////////////////////////////////
G4VPhysicalVolume* LSDetCon::Construct()
{
	//------------------------------------------------
	//   Volumes
	//------------------------------------------------
	// World volume
	m_WorldSolid = new G4Box("WorldSolid", m_WorldX / 2., m_WorldY / 2., m_WorldZ / 2.);
	m_WorldLV = new G4LogicalVolume(m_WorldSolid, m_AirMat, "WorldLV");
	m_WorldPV = new G4PVPlacement(0, G4ThreeVector(), m_WorldLV, "WorldPV", 0, false, 0, true);

	// Lab volume
	m_LabSolid = new G4Box("LabSolid", m_LabX / 2., m_LabY / 2., m_LabZ / 2.);
	m_LabLV = new G4LogicalVolume(m_LabSolid, m_AirMat, "LabLV");
	m_LabPV = new G4PVPlacement(0, G4ThreeVector(), m_LabLV, "LabPV", m_WorldLV, false, 0);

	// LS volume
	m_LSSolid1 = new G4Box("LSSolid1", m_LSX / 2., m_LSY / 2., m_LSZ / 2.);
	m_LSSolid2 = new G4Tubs("LSSolid2", 0., m_PMTD / 2., m_PMTT / 2., 0. * degree, 360. * degree);
	m_LSSolid = new G4UnionSolid("LSSolid", m_LSSolid1, m_LSSolid2, 0, G4ThreeVector(0., 0., m_LSZ / 2. + m_PMTT / 2.));
	m_LSLV = new G4LogicalVolume(m_LSSolid, m_LSMat, "LSLV");
	m_LSLV -> SetVisAttributes(new G4VisAttributes(G4Colour::White()));
	m_LSPV = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), m_LSLV, "LSPV", m_LabLV, false, 0);

	// PMT volume
	m_PMTSolid = new G4Tubs("PMTSolid", 0., m_PMTD / 2., m_PMTT / 2., 0. * degree, 360. * degree);
	m_PMTLV = new G4LogicalVolume(m_PMTSolid, m_PMTMat, "PMTLV");
	m_PMTLV -> SetVisAttributes(new G4VisAttributes(G4Colour::Gray()));
	m_PMTPV = new G4PVPlacement(0, G4ThreeVector(0., 0., m_LSZ / 2. + m_PMTT / 2.), m_PMTLV, "PMTPV", m_LSLV, false, 0);

	//------------------------------------------------
	//   Surfaces
	//------------------------------------------------
	// LS box
	m_LSOpS = new G4OpticalSurface("LSOpS");
	m_LSOpS -> SetType(dielectric_LUTDAVIS);
	m_LSOpS -> SetFinish(RoughTeflon_LUT); // Surface property
	m_LSOpS -> SetModel(DAVIS);

	m_LSLBS = new G4LogicalBorderSurface("LSLBS", m_LSPV, m_LabPV, m_LSOpS);

	G4OpticalSurface* opS = dynamic_cast<G4OpticalSurface*>(m_LSLBS -> GetSurface(m_LSPV, m_LabPV) -> GetSurfaceProperty());
	if ( opS ) opS -> DumpInfo();

	// Air
	m_AirOpS = new G4OpticalSurface("AirOpS");
	m_AirOpS -> SetType(dielectric_dielectric);
	m_AirOpS -> SetFinish(polished);
	m_AirOpS -> SetModel(glisur);

//	m_AirLSS = new G4LogicalSkinSurface("AirLSS", m_PMTLV, m_AirOpS);

//	opS = dynamic_cast<G4OpticalSurface*>(m_AirLSS -> GetSurface(m_PMTLV) -> GetSurfaceProperty());
//	if( opS ) opS -> DumpInfo();

	// Generate & Add Material Properties Table attached to the optical surfaces
	std::vector<G4double> ephoton = { 2.034 * eV, 4.136 * eV };

	// OpticalAirSurface
	std::vector<G4double> reflectivity = { 0.3, 0.5 };
	std::vector<G4double> efficiency   = { 0.8, 1.0 };

	m_AirST = new G4MaterialPropertiesTable();

	m_AirST -> AddProperty("REFLECTIVITY", ephoton, reflectivity);
	m_AirST -> AddProperty("EFFICIENCY", ephoton, efficiency);

	G4cout << "Air Surface G4MaterialPropertiesTable:" << G4endl;
	m_AirST -> DumpTable();

	m_AirOpS -> SetMaterialPropertiesTable(m_AirST);


	return m_LabPV;
}

void LSDetCon::ConstructMaterials()
{
	// Basics of the materials
	m_AirMat = G4NistManager::Instance() -> FindOrBuildMaterial("G4_AIR"  );
	m_LSMat  = G4NistManager::Instance() -> FindOrBuildMaterial("G4_WATER");
	m_PMTMat = G4NistManager::Instance() -> FindOrBuildMaterial("G4_WATER");

	//------------------------------------------------
	// Generate & add material properties table
	// From here, the things have been copied from the OpNovice example
	//------------------------------------------------
	std::vector<G4double> photonEnergy = {
		2.034 * eV, 2.068 * eV, 2.103 * eV, 2.139 * eV, 2.177 * eV, 2.216 * eV,
		2.256 * eV, 2.298 * eV, 2.341 * eV, 2.386 * eV, 2.433 * eV, 2.481 * eV,
		2.532 * eV, 2.585 * eV, 2.640 * eV, 2.697 * eV, 2.757 * eV, 2.820 * eV,
		2.885 * eV, 2.954 * eV, 3.026 * eV, 3.102 * eV, 3.181 * eV, 3.265 * eV,
		3.353 * eV, 3.446 * eV, 3.545 * eV, 3.649 * eV, 3.760 * eV, 3.877 * eV,
		4.002 * eV, 4.136 * eV};

	// Water
	std::vector<G4double> refractiveIndex1 = {
		1.3435, 1.344,  1.3445, 1.345,  1.3455, 1.346,  1.3465, 1.347,
		1.3475, 1.348,  1.3485, 1.3492, 1.35,   1.3505, 1.351,  1.3518,
		1.3522, 1.3530, 1.3535, 1.354,  1.3545, 1.355,  1.3555, 1.356,
		1.3568, 1.3572, 1.358,  1.3585, 1.359,  1.3595, 1.36,   1.3608};
	std::vector<G4double> absorption = {
		 3.448 * m,  4.082 * m,  6.329 * m,  9.174 * m, 12.346 * m, 13.889 * m,
		15.152 * m, 17.241 * m, 18.868 * m, 20.000 * m, 26.316 * m, 35.714 * m,
		45.455 * m, 47.619 * m, 52.632 * m, 52.632 * m, 55.556 * m, 52.632 * m,
		52.632 * m, 47.619 * m, 45.455 * m, 41.667 * m, 37.037 * m, 33.333 * m,
		30.000 * m, 28.500 * m, 27.000 * m, 24.500 * m, 22.000 * m, 19.500 * m,
		17.500 * m, 14.500 * m};

	// Material properties can be added as arrays. However, in this case it is
	// up to the user to make sure both arrays have the same number of elements.
	G4double scintilFastArray[]{ 1.0, 1.0 };
	G4double energyArray[]{ 2.034 * eV, 4.136 * eV };
	G4int lenArray = 2;

	std::vector<G4double> scintilSlow = {
		0.01, 1.00, 2.00, 3.00, 4.00, 5.00, 6.00, 7.00, 8.00, 9.00, 8.00,
		7.00, 6.00, 4.00, 3.00, 2.00, 1.00, 0.01, 1.00, 2.00, 3.00, 4.00,
		5.00, 6.00, 7.00, 8.00, 9.00, 8.00, 7.00, 6.00, 5.00, 4.00};

	m_LSMPT = new G4MaterialPropertiesTable();

	// Values can be added to the material property table individually.
	// With this method, spline interpolation cannot be set. Arguments
	// createNewKey and spline both take their default values of false.
	// Need to specify the number of entries (1) in the arrays, as an argument
	// to AddProperty.
	G4int numEntries = 1;
	m_LSMPT -> AddProperty("RINDEX", &photonEnergy[0], &refractiveIndex1[0], numEntries);
	
	for ( size_t i = 1; i < photonEnergy . size(); ++i )
	{
		m_LSMPT -> AddEntry("RINDEX", photonEnergy[i], refractiveIndex1[i]);
	}

	// Check that group velocity is calculated from RINDEX
	if ( m_LSMPT -> GetProperty("RINDEX") -> GetVectorLength() != m_LSMPT -> GetProperty("GROUPVEL") -> GetVectorLength())
	{
		G4ExceptionDescription ed;
		ed << "Error calculating group velocities. Incorrect number of entries "
		      "in group velocity material property vector.";
		G4Exception("liquidScint::LSDetCon", "liquidScint001", FatalException, ed);
	}

	// Adding a property from two std::vectors. Argument createNewKey is false and spline is true.
	m_LSMPT -> AddProperty("ABSLENGTH", photonEnergy, absorption, false, true);

	// Adding a property using a C-style array.
	// Spline interpolation isn't used for scintillation.
	// Arguments spline and createNewKey both take default value false.
	m_LSMPT -> AddProperty("SCINTILLATIONCOMPONENT1", energyArray, scintilFastArray, lenArray);
	m_LSMPT -> AddProperty("SCINTILLATIONCOMPONENT2", photonEnergy, scintilSlow, false, true);
	m_LSMPT -> AddConstProperty("SCINTILLATIONYIELD", 50. / MeV);
	m_LSMPT -> AddConstProperty("RESOLUTIONSCALE", 1.0);
	m_LSMPT -> AddConstProperty("SCINTILLATIONTIMECONSTANT1",  1. * ns);
	m_LSMPT -> AddConstProperty("SCINTILLATIONTIMECONSTANT2", 10. * ns);
	m_LSMPT -> AddConstProperty("SCINTILLATIONYIELD1", 0.8);
	m_LSMPT -> AddConstProperty("SCINTILLATIONYIELD2", 0.2);
	std::vector<G4double> energy_water = {
		1.56962 * eV, 1.58974 * eV, 1.61039 * eV, 1.63157 * eV, 1.65333 * eV,
		1.67567 * eV, 1.69863 * eV, 1.72222 * eV, 1.74647 * eV, 1.77142 * eV,
		1.7971  * eV, 1.82352 * eV, 1.85074 * eV, 1.87878 * eV, 1.90769 * eV,
		1.93749 * eV, 1.96825 * eV, 1.99999 * eV, 2.03278 * eV, 2.06666 * eV,
		2.10169 * eV, 2.13793 * eV, 2.17543 * eV, 2.21428 * eV, 2.25454 * eV,
		2.29629 * eV, 2.33962 * eV, 2.38461 * eV, 2.43137 * eV, 2.47999 * eV,
		2.53061 * eV, 2.58333 * eV, 2.63829 * eV, 2.69565 * eV, 2.75555 * eV,
		2.81817 * eV, 2.88371 * eV, 2.95237 * eV, 3.02438 * eV, 3.09999 * eV,
		3.17948 * eV, 3.26315 * eV, 3.35134 * eV, 3.44444 * eV, 3.54285 * eV,
		3.64705 * eV, 3.75757 * eV, 3.87499 * eV, 3.99999 * eV, 4.13332 * eV,
		4.27585 * eV, 4.42856 * eV, 4.59258 * eV, 4.76922 * eV, 4.95999 * eV,
		5.16665 * eV, 5.39129 * eV, 5.63635 * eV, 5.90475 * eV, 6.19998 * eV};

	// Rayleigh scattering length is calculated by G4OpRayleigh

	// Mie: assume 100 times larger than the rayleigh scattering
	std::vector<G4double> mie_water = {
		167024.4 * m, 158726.7 * m, 150742 * m,   143062.5 * m, 135680.2 * m,
		128587.4 * m, 121776.3 * m, 115239.5 * m, 108969.5 * m, 102958.8 * m,
		97200.35 * m, 91686.86 * m, 86411.33 * m, 81366.79 * m, 76546.42 * m,
		71943.46 * m, 67551.29 * m, 63363.36 * m, 59373.25 * m, 55574.61 * m,
		51961.24 * m, 48527.00 * m, 45265.87 * m, 42171.94 * m, 39239.39 * m,
		36462.50 * m, 33835.68 * m, 31353.41 * m, 29010.30 * m, 26801.03 * m,
		24720.42 * m, 22763.36 * m, 20924.88 * m, 19200.07 * m, 17584.16 * m,
		16072.45 * m, 14660.38 * m, 13343.46 * m, 12117.33 * m, 10977.70 * m,
		9920.416 * m, 8941.407 * m, 8036.711 * m, 7202.470 * m, 6434.927 * m,
		5730.429 * m, 5085.425 * m, 4496.467 * m, 3960.210 * m, 3473.413 * m,
		3032.937 * m, 2635.746 * m, 2278.907 * m, 1959.588 * m, 1675.064 * m,
		1422.710 * m, 1200.004 * m, 1004.528 * m, 833.9666 * m, 686.1063 * m};

	// Mie: gforward, gbackward, forward backward ratio
	G4double mie_water_const[3] = { 0.99, 0.99, 0.8 };

	m_LSMPT -> AddProperty("MIEHG", energy_water, mie_water, false, true);
	m_LSMPT -> AddConstProperty("MIEHG_FORWARD", mie_water_const[0]);
	m_LSMPT -> AddConstProperty("MIEHG_BACKWARD", mie_water_const[1]);
	m_LSMPT -> AddConstProperty("MIEHG_FORWARD_RATIO", mie_water_const[2]);

	G4cout << "Water G4MaterialPropertiesTable:" << G4endl;
	m_LSMPT -> DumpTable();

	m_LSMat  -> SetMaterialPropertiesTable(m_LSMPT);
	m_PMTMat -> SetMaterialPropertiesTable(m_LSMPT);

	// Set the Birks Constant for the Water scintillator
	m_LSMat -> GetIonisation() -> SetBirksConstant(0.126 * mm / MeV);

	// Air
	std::vector<G4double> refractiveIndex2 = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
	                                          1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
	                                          1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
	                                          1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
	                                          1.0, 1.0, 1.0, 1.0 };
	
	m_AirMPT = new G4MaterialPropertiesTable();
	m_AirMPT -> AddProperty("RINDEX", photonEnergy, refractiveIndex2);

	G4cout << "Air G4MaterialPropertiesTable:" << G4endl;
	m_AirMPT -> DumpTable();

	m_AirMat -> SetMaterialPropertiesTable(m_AirMPT);
}

void LSDetCon::DestructMaterials()
{
}
