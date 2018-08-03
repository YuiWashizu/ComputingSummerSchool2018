using namespace RooFit;

void exercise2() {

  // create exponential model as two components
  RooWorkspace w("w");
  w.factory("Exponential:bkg1_pdf(x[0,10], a1[-0.5,-2,-0.2])");
  w.factory("Gaussian:sig_pdf(x, mass[2], sigma[0.3])"); 
  w.factory("prod:nsig1(mu[1,0,5],xsec1[50])");
  w.factory("SUM:model1(nsig1*sig_pdf, nbkg1[1000,0,10000]*bkg1_pdf)");

  w.factory("Exponential:bkg2_pdf(x, a2[-0.2,-2.0,-0.15])");
  //w.factory("Poisson:sig_pdf_p(x, )"); 
  w.factory("prod:nsig2(mu,xsec2[30])");
  w.factory("SUM:model2(nsig2*sig_pdf, nbkg2[100,0,10000]*bkg2_pdf)");

  w.factory("index[channel1,channel2]");
  w.factory("SIMUL:jointModel(index,channel1=model1,channel2=model2)");

  RooRealVar* x = w.var("x");
  RooCategory* index = w.cat("index");
  RooAbsPdf* pdf = w.pdf("jointModel");

  TFile *file = TFile::Open("tree.root", "RECREATE");

  TTree *tree1 = new TTree("tree1","tree1");
  tree1->ReadFile("dataset1.txt","x");
  RooDataSet data1("data1", "data1", tree1, *x);
  
  TTree *tree2 = new TTree("tree2","tree2");
  tree2->ReadFile("dataset2.txt","x");
  RooDataSet data2("data2", "data2", tree2, *x);
  
  //RooDataSet data("data", "data", *x, Index(*index),
  // 		  　　　　　　　Import("channel1", data1),
  // 		  　　　　　　　Import("channel2", data2));

  RooDataSet* data = pdf->generate(RooArgSet(*x, *index));

  RooPlot* plot1 = x->frame(Title("Channel 1"));
  RooPlot* plot2 = x->frame(Title("Channel 2"));

  data->plotOn(plot1,RooFit::Cut("index==index::channel1"));
  data->plotOn(plot2,RooFit::Cut("index==index::channel2"));
  
  RooFitResult * r = pdf->fitTo(*data, RooFit::Save(true));
  RooFit::Minimizer("Minuit2", "Migrad");

  pdf->plotOn(plot1,RooFit::ProjWData(*data),RooFit::Slice(*w.cat("index"),"channel1"));
  pdf->plotOn(plot2,RooFit::ProjWData(*data),RooFit::Slice(*w.cat("index"),"channel2"));

  TCanvas* c1 = new TCanvas();
  c1->Divide(1,2);
  c1->cd(1);plot1->Draw();
  c1->cd(2);plot2->Draw();
  c1->Write();
  c1->SaveAs("exercise2.png");
  //  file->Close();
}   
