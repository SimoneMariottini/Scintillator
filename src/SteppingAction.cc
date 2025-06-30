#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(const DetectorConstruction* detConstruction,
                               EventAction* eventAction, RunAction* runAction)
  : fDetConstruction(detConstruction),
    fEventAction(eventAction), 
    fRunAction(runAction)
{

  auto particleTable = G4ParticleTable::GetParticleTable();
  fOpticalPhoton = particleTable->FindParticle("opticalphoton")->GetParticleDefinitionID();
  fMuPlus = particleTable->FindParticle("mu+")->GetParticleDefinitionID();
  fPositron = particleTable->FindParticle("e+")->GetParticleDefinitionID();
  fElectron = particleTable->FindParticle("e-")->GetParticleDefinitionID();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  #if ENERGY
  auto postVolume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
  auto preVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  //Check that particle is in scintillator volume and is not a photon
  auto particle = step->GetTrack()->GetParticleDefinition()->GetParticleDefinitionID();

  auto particleMother = step->GetTrack()->GetParentID();

  if(postVolume == fDetConstruction->GetScintPV()){
    #if MUON
    if(particle == fMuPlus){
    fEventAction->AddDepositedEnergy(step->GetTotalEnergyDeposit(), 0);
    }
    #endif
    #if POSITRON
    if(particle == fPositron || particle == fElectron){
      fEventAction->AddDepositedEnergy(step->GetTotalEnergyDeposit(), 0 + MUON);
      }
    #endif
  }
  else if(preVolume == fDetConstruction->GetScintPV()){
    #if MUON
    if(particle == fMuPlus){
      fEventAction->SetParticleEscpaped();
    }
    #endif
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