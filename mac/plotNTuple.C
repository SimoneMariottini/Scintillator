
void plotNTuple(){
  TFile* f1 = new TFile("scintillation_config2_coating_guide10x13.root", "READ");
  TFile* f2 = new TFile("scintillation.root", "READ");

  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1000, 1000);
  c1->cd();

  TH1D* hist1 = new TH1D("hist1", "hist1", 101, -0.5, 100.5);
  TH1D* hist2 = new TH1D("hist2", "hist2", 101, -0.5, 100.5);

  TTree* ntuple1 = (TTree*)f1->Get("SiPM");
  TTree* ntuple2 = (TTree*)f2->Get("SiPM");
  
  ntuple1->Draw("NumberOfIncPhotons >> hist1");
  ntuple2->Draw("NumberOfIncPhotons >> hist2");

  hist1->Draw();

  hist2->SetLineColor(kRed);
  hist2->Draw("same");

}
