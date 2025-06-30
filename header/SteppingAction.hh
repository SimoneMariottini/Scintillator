#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "RunAction.hh"
#include "ConfigFile.hh"

class SteppingAction : public G4UserSteppingAction
{
public:

  SteppingAction(const DetectorConstruction* detConstruction,
                 EventAction* eventAction, RunAction* runAction);
  ~SteppingAction() override = default;

  void UserSteppingAction(const G4Step* step) override;

private:

  const DetectorConstruction* fDetConstruction = nullptr;
  RunAction* fRunAction = nullptr;
  EventAction* fEventAction = nullptr;

  G4int fOpticalPhoton;
  G4int fMuPlus;
  G4int fPositron;
  G4int fElectron;

};

#endif