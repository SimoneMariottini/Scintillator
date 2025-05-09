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
  #if ENERGY
  auto postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
  //Check that particle is in scintillator volume and is not a photon
  if(postVolume == fDetConstruction->GetScintPV() && step->GetTrack()->GetParticleDefinition()->GetParticleDefinitionID() != fOpticalPhoton){
    fEventAction->AddDepositedEnergy(step->GetTotalEnergyDeposit());
  }

  #endif 
  #if SCINTILLATION*COLLECTED_PHOTONS
  auto preVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  auto postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
  for (int i = 0; i<4*4; i++){
  if(postVolume == fDetConstruction->GetSiPMPV(i)){

      auto particle = step->GetTrack()->GetParticleDefinition()->GetParticleDefinitionID();

      if(particle == fOpticalPhoton){
        fEventAction->AddDeltaTime(step->GetTrack()->GetGlobalTime());
        fEventAction->AddDetectedPhoton();
        step->GetTrack()->SetTrackStatus(fStopAndKill);
     }
    }
  }
  #endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......