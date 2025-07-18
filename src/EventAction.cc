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
  #if ENERGY
  for (int i = 0; i < TRACKEDPATICLES; i ++){
    fDepositedEnergy[i] = 0.;
    fParticleEscaped[i] = false;
  }
  #endif
  #if SCINTILLATION*COLLECTED_PHOTONS
  fNDetectedPhotons = 0;
  fSumDeltaTime = 0.;
  fSumDeltaTimeSq = 0.;
  #endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  #if SCINTILLATION*COLLECTED_PHOTONS
  auto meanDeltaTime = fSumDeltaTime/double(fNDetectedPhotons);
  auto meanDeltaTimeSq = fSumDeltaTimeSq/double(fNDetectedPhotons);

  auto sigmaDeltaTime = sqrt(meanDeltaTimeSq - meanDeltaTime*meanDeltaTime);
  #endif
  auto eventID = event->GetEventID();
  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( printModulo >= 0 && eventID % printModulo == 0) {
    #if ENERGY
    G4cout << "----------> Deposited energy = " << fDepositedEnergy;
    #endif
    #if SCINTILLATION*COLLECTED_PHOTONS
    G4cout << "----------> Number of detected photons = " << fNDetectedPhotons;
    G4cout << "----------> Time resolution = " << sigmaDeltaTime;
    #endif
  }
  
  auto fAnalysisManager = G4AnalysisManager::Instance();

  #if ENERGY
  for(int i = 0; i < TRACKEDPATICLES; i++){
  fAnalysisManager->FillNtupleDColumn(i, fDepositedEnergy[i]);
  }
  for(int i = 0; i < TRACKEDPATICLES; i++){
  fAnalysisManager->FillNtupleDColumn(TRACKEDPATICLES + i, int(fParticleEscaped[i]));
  }
  #endif
  
  #if SCINTILLATION*COLLECTED_PHOTONS
  fAnalysisManager->FillNtupleDColumn(ENERGY*TRACKEDPATICLES + 0, fNDetectedPhotons);
  fAnalysisManager->FillNtupleDColumn(ENERGY*TRACKEDPATICLES + 1, meanDeltaTime);
  fAnalysisManager->FillNtupleDColumn(ENERGY*TRACKEDPATICLES + 2, sigmaDeltaTime);
  #endif
  
  fAnalysisManager->AddNtupleRow();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
