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

  private:

    G4int fNDetectedPhotons;
};

//
// inline functions
//

inline const void EventAction::AddDetectedPhoton(){ fNDetectedPhotons++; }

#endif