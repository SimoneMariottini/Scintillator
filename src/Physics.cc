#include "Physics.hh"

ModularPhysicsList::ModularPhysicsList(G4int verboseLevel) 
{
    RegisterPhysics (new G4EmStandardPhysics_option4()); //EM interactions
    #if SCINTILLATION
    auto opticalPhysics = new G4OpticalPhysics(verboseLevel);

    auto opticalParams = G4OpticalParameters::Instance();
    opticalParams->SetBoundaryInvokeSD(true);
    opticalParams->SetProcessActivation("OpAbsorption", true);
    opticalParams->SetProcessActivation("OpRayleigh", true);
    
    opticalParams->SetProcessActivation("OpScintillation", true);
    
    opticalParams->SetVerboseLevel(verboseLevel);

    RegisterPhysics (opticalPhysics); //Optical Photon processes
    #endif

    RegisterPhysics(new G4DecayPhysics(verboseLevel));                 // Decays for all particles
    RegisterPhysics(new G4RadioactiveDecayPhysics(verboseLevel));      // Radioactive decay processes
    RegisterPhysics(new G4StoppingPhysics(verboseLevel));              // Stopping physics for ions and muons
    RegisterPhysics(new G4IonPhysics(verboseLevel));                   // Ion interactions
    RegisterPhysics(new G4NeutronTrackingCut(verboseLevel));           // Tracking cuts for neutrons

}

ModularPhysicsList::~ModularPhysicsList()
{}