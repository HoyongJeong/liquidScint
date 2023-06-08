////////////////////////////////////////////////////////////////////////////////
//   LSRunAct.cc
//
//   Definitions of LSRunAct class's member functions. Details of user
// actions are here.
//
//     - Hoyong Jeong (hoyong5419@korea.ac.kr), Eunil Won (eunilwon@korea.ac.kr)
////////////////////////////////////////////////////////////////////////////////

#include <ctime>

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4RootAnalysisManager.hh"

#include "LSRunAct.hh"

//////////////////////////////////////////////////
//   Constructor
//////////////////////////////////////////////////
LSRunAct::LSRunAct(): G4UserRunAction()
{
	// Create analysis manager
	auto AM = G4RootAnalysisManager::Instance();

	// Default settings
	// I don't like chatterbox...
	AM -> SetVerboseLevel(0);

	// Creating ntuple
	AM -> CreateNtuple("liquidScint", "Cerenkov and Scintillation");
	AM -> CreateNtupleIColumn("eventID");   // Column ID = 0
	AM -> CreateNtupleIColumn("creProcID"); // Column ID = 1
	AM -> CreateNtupleDColumn("timing");    // Column ID = 2
	AM -> CreateNtupleDColumn("energy");    // Column ID = 3
	AM -> FinishNtuple();
}

//////////////////////////////////////////////////
//   Destructor
//////////////////////////////////////////////////
LSRunAct::~LSRunAct()
{
}

//////////////////////////////////////////////////
//   Begin of run action
//////////////////////////////////////////////////
void LSRunAct::BeginOfRunAction(const G4Run* /*run*/)
{
	// All actions defined here will be excuted at the beginning of every run.
	// What is a run? You may type "/run/beamOn [someNumber]".
	// Whenever you do this, "one run" runs.

	// Get analysis manager
	auto AM = G4RootAnalysisManager::Instance();

	// Get current time to include it to file name
	// This time info is going to be used to generate output file name.
	time_t rawTime;
	struct tm* timeInfo;
	char buffer[80];
	time(&rawTime);
	timeInfo = localtime(&rawTime);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", timeInfo);
	std::string sTime(buffer);

	// Open an output file
	// The default file name is set in LSRunAct::LSRunAct(),
	// and it can be overwritten in a macro
	G4String fileName = "liquidScint_";
	fileName += sTime;
	fileName += ".root";
	G4cout << fileName << G4endl;

	AM -> OpenFile(fileName);
	G4cout << "Using " << AM -> GetType() << G4endl;
}

//////////////////////////////////////////////////
//   End of run action
//////////////////////////////////////////////////
void LSRunAct::EndOfRunAction(const G4Run* /*run*/)
{
	// save histograms & ntuple
	auto AM = G4RootAnalysisManager::Instance();
	// You must save. Otherwise, file will be just empty.
	AM -> Write();
	// You must close the file. Otherwise, file will be crahsed.
	AM -> CloseFile();
}
