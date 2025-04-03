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

  private:

    G4int fNDetectedPhotons;
    G4double fSumDeltaTime;
    G4double fSumDeltaTimeSq;
};

//
// inline functions
//

inline const void EventAction::AddDetectedPhoton(){ fNDetectedPhotons++; }
inline const void EventAction::AddDeltaTime(G4double deltaTime){ fSumDeltaTime += deltaTime; fSumDeltaTimeSq += deltaTime*deltaTime;}

#endif