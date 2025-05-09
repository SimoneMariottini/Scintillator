#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class EventAction : public G4UserEventAction
{
  public:
    EventAction() = default;
    ~EventAction() override = default;

    void   BeginOfEventAction(const G4Event* event) override;
    void   EndOfEventAction(const G4Event* event) override;

    const void AddDetectedPhoton();
    const void AddDeltaTime(G4double deltaTime);
    const void AddDepositedEnergy(G4double depositedEnergy);

  private:

    G4int fNDetectedPhotons;
    G4double fSumDeltaTime;
    G4double fSumDeltaTimeSq;
    G4double fDepositedEnergy;
};

//
// inline functions
//

inline const void EventAction::AddDetectedPhoton(){ fNDetectedPhotons++; }
inline const void EventAction::AddDeltaTime(G4double deltaTime){ fSumDeltaTime += deltaTime; fSumDeltaTimeSq += deltaTime*deltaTime;}
inline const void EventAction::AddDepositedEnergy(G4double depositedEnergy){ fDepositedEnergy += depositedEnergy; }

#endif