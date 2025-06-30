
void plotNTuple(){
  TFile* f1 = new TFile("scintillation_500mum_muon.root", "READ");
  TFile* f2 = new TFile("scintillation_500mum_positron.root", "READ");

  TCanvas* c1 = new TCanvas("c1", "", 20, 20, 1500, 1000);
  c1->cd();

  TH1D* hist1 = new TH1D("hist1", "hist1", 100, 0., 5.);
  TH1D* hist2 = new TH1D("hist2", "hist2", 100, 0., 5.);
  TH1D* hist3 = new TH1D("hist3", "Energy deposition on 0.5mm scintillator", 100, 0., 5.);

  TLegend* legend = new TLegend(0.65, 0.7, 0.88, 0.88);

  TTree* ntuple1 = (TTree*)f1->Get("SiPM");
  TTree* ntuple2 = (TTree*)f2->Get("SiPM");
  
  ntuple1->Draw("DepositedMuonEnergy >> hist1");
  ntuple1->Draw("DepositedPositronEnergy >> hist2");
  ntuple2->Draw("DepositedPositronEnergy >> hist3");

  hist3->SetLineColor(kBlue);
  hist3->GetYaxis()->SetTitle("Counts");
  hist3->GetXaxis()->SetTitle("Energy [MeV]");
  legend->AddEntry(hist3, "Beam Positrons", "l");
  hist3->Draw();

  hist1->SetLineColor(kGreen);
  legend->AddEntry(hist1, "Muons", "l");
  hist1->Draw("same");
  
  hist2->SetLineColor(kRed);
  legend->AddEntry(hist2, "Decay Positrons", "l");
  hist2->Draw("same");

  legend->Draw();

}
