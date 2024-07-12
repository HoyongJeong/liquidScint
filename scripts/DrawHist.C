void DrawHist()
{
	//------------------------------------------------
	// Open root files
	//------------------------------------------------
	TChain* chain = new TChain("liquidScint");
	chain -> Add("*.root");


	//------------------------------------------------
	// Get branches from the files
	//------------------------------------------------
	int procID;
	double t, egy;
	chain -> SetBranchAddress("creProcID", &procID);
	chain -> SetBranchAddress("timing"   , &t     );
	chain -> SetBranchAddress("energy"   , &egy   );


	//------------------------------------------------
	// Define timing histograms
	//------------------------------------------------
	TH1D* hTC = new TH1D("hTC", "Cerenkov Photons Timing"     , 500, 0, 500);
	TH1D* hTS = new TH1D("hTS", "Scintillation Photons Timing", 500, 0, 500);
	TH1D* hTA = new TH1D("hTA", "All Photons Timing"          , 500, 0, 500);


	//------------------------------------------------
	// Looping over entries and fill histograms
	//------------------------------------------------
	for ( int i = 0; i < chain -> GetEntries(); i++ )
	{
		chain -> GetEntry(i);
		
		if      ( procID == 0 ) hTC -> Fill(t);
		else if ( procID == 1 ) hTS -> Fill(t);

		hTA -> Fill(t);
	}


	//------------------------------------------------
	// Drawing style
	//------------------------------------------------
	hTC -> SetFillColor(kCyan);
	hTS -> SetFillColor(kGreen);
	hTA -> SetFillColor(kMagenta);


	//------------------------------------------------
	// Draw histograms on a canvas
	//------------------------------------------------
	TCanvas* cT = new TCanvas("cT", "Timing", 900, 600);
	cT -> Divide(2, 2);
	cT -> cd(1);
	hTC -> Draw();
	cT -> cd(2);
	hTS -> Draw();
	cT -> cd(3);
	hTA -> Draw();
	cT -> cd(4);
	hTA -> Draw();
	hTC -> Draw("same");


	//------------------------------------------------
	// Save canvas as an image
	//------------------------------------------------
	cT -> SaveAs("hists.png");


	//------------------------------------------------
	// Bye bye
	//------------------------------------------------
	return;
}
