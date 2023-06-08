////////////////////////////////////////////////////////////////////////////////
//   liquidScint.cc
//
//   This is main function of the liquidScint simulation.
//
//     - Hoyong Jeong (hoyong5419@korea.ac.kr), Eunil Won (eunilwon@korea.ac.kr)
////////////////////////////////////////////////////////////////////////////////

#include <unistd.h>

#include "LSDetCon.hh"
#include "LSActIni.hh"

#include "G4RunManagerFactory.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4UIExecutive.hh"
#include "G4String.hh"
#include "Randomize.hh"

#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"

// Declaration of PrintHelp()
void PrintHelp();

//////////////////////////////////////////////////
//   Main function                              //
//////////////////////////////////////////////////
int main(int argc, char** argv)
{
	// Read options
	int flag_b = 0, flag_g = 0, flag_h = 0, flag_m = 0;
	const char* optDic = "bghm:"; // Option dictionary
	int option;
	char* macro;
	while ( (option = getopt(argc, argv, optDic)) != -1 ) // -1 means getopt() parses all options.
	{
		switch ( option )
		{
			case 'b' :
				flag_b = 1;
				break;
			case 'g' :
				flag_g = 1;
				break;
			case 'h' :
				flag_h = 1;
				break;
			case 'm' :
				flag_m = 1;
				macro = optarg;
				break;
			case '?' :
				flag_h = 1;
				break;
		}
	}

	// In case of '-h' option activated
	if ( flag_h )
	{
		PrintHelp();
		return 0;
	}

	// Randomizer
	CLHEP::RanluxEngine defaultEngine(1234567, 4);
	G4Random::setTheEngine(&defaultEngine);
	G4int seed = time(NULL);
	G4Random::setTheSeed(seed);

	// Detect interactive mode (if flag_g) and define UI session
	G4UIExecutive* UI = 0;
	if ( flag_g ) UI = new G4UIExecutive(argc, argv);

	// Run manager
	auto RM = G4RunManagerFactory::CreateRunManager();
	RM -> SetNumberOfThreads(4);

	// Detector construction
	// We define everything about geomtrical setup in this class.
	RM -> SetUserInitialization(new LSDetCon());

	// Physics list to be used
	G4VModularPhysicsList* PL = new FTFP_BERT;
	PL -> SetVerboseLevel(0);
	PL -> ReplacePhysics(new G4EmStandardPhysics_option4());
	G4OpticalPhysics* OP = new G4OpticalPhysics();
	PL -> RegisterPhysics(OP);
	RM -> SetUserInitialization(PL);

	// User actions
	RM -> SetUserInitialization(new LSActIni());

	// Initialize
	RM -> Initialize();

	// Visualization manger
	G4VisManager* VM = new G4VisExecutive();
	VM -> Initialize();

	// Get the pointer to the user interface manager
	G4UImanager* UM = G4UImanager::GetUIpointer();

	// Process macro or start UI session
	G4String command = "/control/execute ";
	G4String command2;
	if ( !UI )
	{
		// terminal mode
		G4UIsession* US = new G4UIterminal(new G4UItcsh);
		if ( flag_m )
		{
			command2 = macro;
			UM -> ApplyCommand(command + command2);
		}
		if ( !flag_b ) US -> SessionStart();

		delete US;
	}
	else
	{
		// Interactive mode
		command2 = "init_vis.mac";
		UM -> ApplyCommand(command + command2);
		if ( flag_m )
		{
			command2 = macro;
			UM -> ApplyCommand(command + command2);
		}
		UI -> SessionStart();

		delete UI;
	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted 
	// in the main() program.
	delete VM;
	delete RM;

	std::cout << "bye bye :)" << std::endl;

	return 0;
}

//////////////////////////////////////////////////
//   Print help message                         //
//////////////////////////////////////////////////
void PrintHelp()
{
	std::cout << "usage: liquidScint [-b] [-g] [-m macrofile]" << std::endl;
	std::cout << std::endl;
	std::cout << "Examples:" << std::endl;
	std::cout << "  liquidScint -b -m myRun.mac  # Run in batch mode with macro." << std::endl;
	std::cout << "  liquidScint -g               # Run in graphical mode." << std::endl;
	std::cout << std::endl;
	std::cout << "Options:" << std::endl;
	std::cout << "  -b  Execute in batch mode"         << std::endl;
	std::cout << "  -g  Execute in graphical mode"     << std::endl;
	std::cout << "      Note: Default is command mode" << std::endl;
	std::cout << "  -h  Show help message"             << std::endl;
	std::cout << "  -m  Run with macro"                << std::endl;
	std::cout << std::endl;
	std::cout << "bye bye :)" << std::endl;
	std::cout << std::endl;
}
