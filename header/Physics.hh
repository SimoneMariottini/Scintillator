#ifndef PHYSICS_HH
#define PHYSICS_HH 1

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4PhysListFactory.hh"
#include "G4ThermalNeutrons.hh"
#include "FTFP_BERT.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4NeutronTrackingCut.hh"

class ModularPhysicsList : public G4VModularPhysicsList
{
public:
    ModularPhysicsList();
    ~ModularPhysicsList();
};

#endif