#include "EventAction.hh"
#include "RunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
  fNDetectedPhotons = 0;
  fSumDeltaTime = 0.;
  fSumDeltaTimeSq = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  auto meanDeltaTime = fSumDeltaTime/double(fNDetectedPhotons);
  auto meanDeltaTimeSq = fSumDeltaTimeSq/double(fNDetectedPhotons);

  auto sigmaDeltaTime = sqrt(meanDeltaTimeSq - meanDeltaTime*meanDeltaTime);

  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( printModulo >= 0 && eventID % printModulo == 0) {
    G4cout << "----------> Number of detected photons = " << fNDetectedPhotons;
    G4cout << "----------> Time resolution = " << sigmaDeltaTime;
  }
  
  auto fAnalysisManager = G4AnalysisManager::Instance();
  
  fAnalysisManager->FillNtupleDColumn(0, fNDetectedPhotons);
  fAnalysisManager->FillNtupleDColumn(1, meanDeltaTime);
  fAnalysisManager->FillNtupleDColumn(2, sigmaDeltaTime);
  
  fAnalysisManager->AddNtupleRow();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
