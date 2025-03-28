#include "Physics.hh"

ModularPhysicsList::ModularPhysicsList() 
{
    RegisterPhysics (new G4EmStandardPhysics_option4()); //EM interactions

    auto opticalPhysics = new G4OpticalPhysics();
    opticalPhysics->SetVerboseLevel(1);

    auto opticalParams = G4OpticalParameters::Instance();
    opticalParams->SetBoundaryInvokeSD(true);
    opticalParams->SetProcessActivation("OpAbsorption", true);
    opticalParams->SetProcessActivation("OpRayleigh", true);
    opticalParams->SetProcessActivation("OpScintillation", true);
    opticalParams->SetVerboseLevel(1);

    RegisterPhysics (opticalPhysics); //Optical Photon processes

    RegisterPhysics(new G4DecayPhysics());                 // Decays for all particles
    RegisterPhysics(new G4RadioactiveDecayPhysics());      // Radioactive decay processes
    RegisterPhysics(new G4StoppingPhysics());              // Stopping physics for ions and muons
    RegisterPhysics(new G4IonPhysics());                   // Ion interactions
    RegisterPhysics(new G4NeutronTrackingCut());           // Tracking cuts for neutrons

}

ModularPhysicsList::~ModularPhysicsList()
{}