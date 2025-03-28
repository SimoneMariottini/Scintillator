#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(const DetectorConstruction* detConstruction,
                               EventAction* eventAction)
  : fDetConstruction(detConstruction),
    fEventAction(eventAction)
{
  auto particleTable = G4ParticleTable::GetParticleTable();
  fOpticalPhoton = particleTable->FindParticle("opticalphoton")->GetParticleDefinitionID();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  auto preVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  auto postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();

  if(postVolume == fDetConstruction->GetSiPMPV()){

    auto particle = step->GetTrack()->GetParticleDefinition()->GetParticleDefinitionID();
    if(particle == fOpticalPhoton){
      fEventAction->AddDetectedPhoton();
      step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......