using namespace RooFit;
 
void fitGaus1()
{
  RooRealVar x("x", "x", -10, 10);
  //RooRealVar mean("mean", "Mean of Gaussian", 0, -10, 10);
  //RooRealVar sigma("sigma", "Width of Gaussian", 1, -10, 10);

  RooRealVar a1("a1", "a1", -1, -10, 10);
 
  //RooGaussian gauss("gauss", "gauss(x, mean, sigma)", x, mean, sigma);
  RooExponential ex("Exponential", "Exponential(x, al)",x, a1 );

  RooPlot* xframe = x.frame();
  ex.plotOn(xframe);
  xframe->Draw();
}
