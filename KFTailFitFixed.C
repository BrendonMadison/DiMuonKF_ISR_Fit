void KFTailFitFixed(){
  //
  // Primary purpose: Uses the Kuraev-Fadin ISR (Initial State Radiation) distribution to fit for DiMuon sqrt(s) distribution of ILC 250 GeV e-e+ collider.
  //
  // Secondary purpose: Since this likely poorly fits the sqrt(s) distribution, as ISR is only 1 component that contributes to the sqrt(s), this gives an idea of how the parameters need to be changed to better fit the sqrt(s) as simply using lepton mass and fine structure constant, while make sense in theory, don't work perfectly in implementation. 
  //
  // To dump the text output to a file type ".> mylog.txt" in the prompt before the script
  gSystem->Load("libRooFit");
  using namespace RooFit; 

  //The main variable in this case is the sqrt(s) energy
  RooRealVar bsx("bsx","sqrt(s) [GeV]",150,249.9,"GeV");

  //Fit parameter fine structure and lepton mass
  RooRealVar alpha("alpha","Fine Str.",1.0/137,0.1/137,10/137);
  RooRealVar me("me","Lepton Mass",105.7e-3,100e-3,110e-3);
  //Sets lepton mass to constant
  me.setConstant(kTRUE);
  me.removeError();
  //Mean beam energy. Fit for but should be ~250 GeV
  RooRealVar be("be","Mean Beam Energy",250,240,260);
  //define the beta parameter of the isr fit according to the Kuraev&Fadin paper
  RooGenericPdf beta("beta","beta","2*alpha/3.1415926 * (log(bsx*bsx/(me*me)) - 1)",RooArgSet(alpha,bsx,me));
  //The PDF to be fit for
  RooGenericPdf fbeta("fbeta","fbeta","beta/16 * ((8 + 3*beta)*(1-bsx/be)^(beta/2 - 1) - 4*(1+bsx/be))",RooArgSet(beta,bsx,be));

  // Access the histogram and tree stored in the root file
  TFile hfile("../../gdev/Test-DiMuon/DiMuons_LR.root");
  TTree* tree=(TTree*)hfile.Get("DiMuonTree");

  //Draw the histogram that is to be fitted for. You can change the options here. The range for bsx should be the same as used below in htemp.
  tree->Draw("(pfoSqrtsPlus)>>htemp(200,150.0,249.9)","","");

  //Grabs htemp off the directory
  TH1F *h1 = (TH1F*) gDirectory->Get("htemp");

  //Normalizes htemp and changes it name and draws with error bars.
  h1->SetTitle("#sqrt{s}");
  h1->Scale(1.0/(h1->Integral()));
  h1->Draw("E");

  // Create a binned dataset that imports contents of h1 and associates its contents to observable 'sqrts' aka the beam energy
  RooDataHist db("db", "db", bsx,Import(*h1));

  //create a RooAddPdf for passing the model to the plotting program.
  RooRealVar addfrac("% Signal","addfrac",1.0,0.0,1.0);
  addfrac.setConstant(kTRUE);
  addfrac.removeError();
  RooAddPdf sigAdd("sigAdd","Muon Fixed Kuraev-Fadin Tail Fit #sqrt{s}",RooArgList(fbeta),addfrac);

  //Temporary file to pass the PDF
  TFile f("temp.root","recreate");

  db.Write();
  sigAdd.Write();
  bsx.Write();

  //Check that the PDF and data were saved.
  if(gDirectory->Get("db") == NULL) {
    cout << "Unable to return database." << endl;
  }

  //Formulates and executes the command call to the plotting program
  char *cmd;
  cmd = Form(".x FitPlotMacro.C(\"%s\",\"%s\",\"%s\",\"%s\")",db.GetName(),sigAdd.GetName(),bsx.GetName(),"#sqrt{s} From DiMuon");
  gROOT->ProcessLine(cmd);

  //Closes the temp file
  f.Close();

  //Removes the temp file
  gSystem->Exec("rm temp.root");

  //Close the histogram file
  hfile.Close();

}
