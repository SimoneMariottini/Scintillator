#include "RunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
    G4RunManager::GetRunManager()->SetPrintProgress(1);

    // Create analysis manager
    auto analysisManager = G4AnalysisManager::Instance();
  
    // Create directories
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true);
  
    // Book histograms, ntuple
    //
    
    // Creating ntuple
    analysisManager->CreateNtuple("SiPM", "26MeV mu+");
    analysisManager->CreateNtupleDColumn("NumberOfIncPhotons");
    analysisManager->CreateNtupleDColumn("MeanDeltaTime");
    analysisManager->CreateNtupleDColumn("SigmaDeltaTime");
  
    analysisManager->FinishNtuple();
  }
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  void RunAction::BeginOfRunAction(const G4Run*)
  {
    
    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    G4String resultPath = std::getenv("RESULT");

    G4String fileName;

    if(resultPath){
      fileName = resultPath + "/scintillation.root";
    }
    else{
      fileName = "./scintillation.root";
    }
  
    analysisManager->OpenFile(fileName);
    
  }
  
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  void RunAction::EndOfRunAction(const G4Run*)
  { 
    // print histogram statistics
    auto analysisManager = G4AnalysisManager::Instance();
    
    // save histograms & ntuple
    analysisManager->Write();
    analysisManager->CloseFile();
  }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
