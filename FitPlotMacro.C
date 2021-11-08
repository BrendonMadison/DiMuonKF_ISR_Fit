//Macro for plotting fits. Call by fitting macros or from the command line

//void FitPlotMacro(const string argdb , const string argmodel, const string argx){
void FitPlotMacro(const string argdb,const string argmodel, const string argx, const string argtit){


  gSystem->Load("libRooFit");
  gSystem->Load("RooCrystalBall.cxx");
  using namespace RooFit;

  cout << "Using the database " << argdb << " and model " << argmodel <<  endl;

  gDirectory->ls();
  RooDataHist *ldb = (RooDataHist*)gDirectory->Get(argdb.c_str());
  
  //gDirectory->GetObject(argdb.c_str(),ldb);
  //  auto ldb = gDirectory->Get(argdb.c_str());
  RooAddPdf *lmodel = (RooAddPdf*)gDirectory->Get(argmodel.c_str());
  //gDirectory->GetObject(argmodel.c_str(),lmodel);
  RooRealVar *lx = (RooRealVar*)gDirectory->Get(argx.c_str());
  // gDirectory->GetObject(argx.c_str(),lx);
  //RooAbsPdf  lmodel = gDirectory->Get(argmodel.c_str());
  //auto lx = gDirectory->Get(argx.c_str());
  //Fit the data (db) to the model (crysball)


  //char *cmd;
  //cmd = Form("%s lmodel = gDirectory->Get(\"%s\");",argtype.c_str(),argmodel.c_str());
  //gROOT->ProcessLine(cmd);

  //char *cmd2;
  //cmd2 = Form("lmodel->fitTo(*ldb);");
  //gROOT->ProcessLine(cmd2);

  //check if the pointers are null
  if(ldb == NULL){
    cout << "Database returned null. Check that you are passing correctly, saving locally." << endl;
    return;
  }

  if(lmodel == NULL){
    cout << "Model returned null. Check that you are passing correctly, saving locally." << endl;
    return;
  }

  if(lx == NULL){
    cout << "X axis returned null. Check that you are passing correctly, saving locally." << endl;
    return;
  }

  lmodel->chi2FitTo(*ldb);

  // Plot data and results
  RooPlot *xframe=lx->frame(Title(Form("%s fit of \%s",lmodel->GetTitle(),argtit.c_str())));
  ldb->plotOn(xframe);

  //char *cmd3;
  //cmd3 = Form("lmodel->plotOn(xframe,LineColor(kBlue));");
  //gROOT->ProcessLine(cmd3);

  lmodel->plotOn(xframe,LineColor(kBlue));
  
  //  lmodel->paramOn(xframe, Layout(0.25,0.25,0.75));
  //ldb->statOn(xframe, Layout(0.25,0.25,0.30));
  //  xframe->SetTitle("Title Argument");
  //  xframe->Draw();

  //Construct a histogram with pulls
  RooHist* hpull = xframe->pullHist();
  Double_t pmean = hpull->GetMean(2);
  Double_t prms = hpull->GetRMS(2);
  RooPlot* pframe = lx->frame(Title(Form("Pull of %s fit",argtit.c_str())));
  pframe->addPlotable(hpull,"P");

  Int_t pt = 15;

  //Add pull text box
  TText *mtxt = new TText(hpull->GetXaxis()->GetXmin(),pmean-2.0*prms,Form("Mean: %f",pmean));
  TText *rtxt = new TText(hpull->GetXaxis()->GetXmin(),pmean-4.0*prms,Form("RMS: %f",prms));
  pframe->addObject(mtxt);
  pframe->addObject(rtxt);

  // Check goodness of fit ...
  //For some reason the below returns ChiSquare per bin...
  Double_t pChi = xframe->chiSquare();
  auto hdat = ldb->createHistogram(argx.c_str());
  RooArgSet asx(*lx);
  auto fmod = lmodel->asTF(asx,RooArgSet(),asx);

  
  //Double_t pChi = fmod->GetChisquare();

  Double_t nPars = lmodel->getParameters(*ldb)->getSize();
  Int_t ndof = hdat->GetNbinsX() - nPars;

  pChi = pChi * hdat->GetNbinsX();

  cout << "Reduced ChiSq: " << pChi << " / " << ndof << endl;

  lmodel->paramOn(xframe, Label(Form("#chi ^{2} / nDoF = %3.2lf / %i",pChi,ndof)),Layout(0.15,0.35,0.8));

  TCanvas* c = new TCanvas("c",argtit.c_str(),1200,600);
  c->Divide(1,2);
  c->cd(1); gPad->SetBottomMargin(0.1); xframe->GetXaxis()->SetTitle(lx->GetTitle()); xframe->GetXaxis()->SetRangeUser(xframe->GetXaxis()->GetXmin()*0.5,xframe->GetXaxis()->GetXmax()); xframe->GetYaxis()->SetTitle("Normalized Units"); xframe->Draw();
  c->cd(2); gPad->SetBottomMargin(0.1); pframe->GetXaxis()->SetTitle(lx->GetTitle()); pframe->Draw();

  TString filename = Form("%s_%s.pdf",argtit.c_str(),lmodel->GetTitle());

  filename.ReplaceAll(" ","_");
  filename.ReplaceAll("#","");

  c->SaveAs(filename);

  delete c;

}
