#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4Material.hh"
#include "globals.hh"
//#include "G4OpticalSurface.hh"

class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction() = default;
    ~DetectorConstruction() override = default;

  public:

    G4VPhysicalVolume* Construct() override;
    //void ConstructSDandField() override;

    const G4VPhysicalVolume* GetSiPMPV() const;
    const G4VPhysicalVolume* GetWorldPV() const;
    const G4VPhysicalVolume* GetScintPV() const;

  private:

    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    G4VPhysicalVolume* fWorldPV = nullptr;
    G4VPhysicalVolume* fScintPV = nullptr;
    G4VPhysicalVolume* fSiPMPV = nullptr;

    G4Material* fPolystyrene;
    G4Material* fAir;
    G4Material* fPlexiglass;
    G4Material* fAlluminum;
    //G4OpticalSurface* fAlluminumOpticalSurface;

    G4bool fCheckOverlaps = true; // option to activate checking of volumes overlaps
};

//
// Inline functions
//

inline const G4VPhysicalVolume* DetectorConstruction::GetSiPMPV() const { return fSiPMPV; }
inline const G4VPhysicalVolume* DetectorConstruction::GetWorldPV() const { return fWorldPV; }
inline const G4VPhysicalVolume* DetectorConstruction::GetScintPV() const { return fScintPV; }

#endif

