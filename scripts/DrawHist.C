void DrawHist()
{
	// Open files
	TChain* chain = new TChain("liquidScint");
	chain -> Add("*.root");

	// Branches
	int procID;
	double t, egy;
	chain -> SetBranchAddress("creProcID", &procID);
	chain -> SetBranchAddress("timing"   , &t     );
	chain -> SetBranchAddress("energy"   , &egy   );

	// Hists
	TH1D* hTC = new TH1D("hTC", "Cerenkov Photons Timing"     , 500, 0, 500);
	TH1D* hTS = new TH1D("hTS", "Scintillation Photons Timing", 500, 0, 500);
	TH1D* hTA = new TH1D("hTA", "All Photons Timing"          , 500, 0, 500);

	// Loop
	for ( int i = 0; i < chain -> GetEntries(); i++ )
	{
		chain -> GetEntry(i);
		
		if      ( procID == 0 ) hTC -> Fill(t);
		else if ( procID == 1 ) hTS -> Fill(t);

		hTA -> Fill(t);
	}

	// Style
	hTC -> SetFillColor(kCyan);
	hTS -> SetFillColor(kGreen);
	hTA -> SetFillColor(kMagenta);

	// Draw
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
}
