#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "ConfigFile.hh"

class EventAction : public G4UserEventAction
{
  public:
    EventAction() = default;
    ~EventAction() override = default;

    void   BeginOfEventAction(const G4Event* event) override;
    void   EndOfEventAction(const G4Event* event) override;

    const void AddDetectedPhoton();
    const void AddDeltaTime(G4double deltaTime);
    const void AddDepositedEnergy(G4double depositedEnergy, G4int particleID);
    void SetParticleEscpaped();

  private:

    G4int fNDetectedPhotons;
    G4double fSumDeltaTime;
    G4double fSumDeltaTimeSq;
    G4double fDepositedEnergy[TRACKEDPATICLES] = {};
    G4bool fParticleEscaped[TRACKEDPATICLES] = {};

    //Particle definitions
};

//
// inline functions
//

inline const void EventAction::AddDetectedPhoton(){ fNDetectedPhotons++; }
inline const void EventAction::AddDeltaTime(G4double deltaTime){ fSumDeltaTime += deltaTime; fSumDeltaTimeSq += deltaTime*deltaTime;}
inline const void EventAction::AddDepositedEnergy(G4double depositedEnergy, G4int ID){ fDepositedEnergy[ID] += depositedEnergy; }

inline void EventAction::SetParticleEscpaped(){fParticleEscaped[0] = true;}

#endif 