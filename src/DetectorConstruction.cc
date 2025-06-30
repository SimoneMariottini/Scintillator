#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Polyhedra.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4RotationMatrix.hh"
//#include "G4LogicalSkinSurface.hh"

#include "CLHEP/Units/SystemOfUnits.h"

#include "ConfigFile.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  // Some materials defined using NIST Manager
  auto nistManager = G4NistManager::Instance();

  G4double energy[2] = {1.239841939*eV/0.7, 1.239841939*eV/0.4}; //conversion factor divided by blue and red wavelength (in micrometers)

  //
  // Vacuum
  //

  fVacuum = nistManager->FindOrBuildMaterial("G4_Galactic");

  /*G4double rindexVacuum[2] = {1.0, 1.0};

  G4MaterialPropertiesTable *mptVacuum = new G4MaterialPropertiesTable();
  mptVacuum->AddProperty("RINDEX", energy, rindexVacuum, 2);

  fVacuum->SetMaterialPropertiesTable(mptVacuum);*/

  //
  // Air
  //

  fAir = nistManager->FindOrBuildMaterial("G4_AIR");

  /*G4double rindexAir[2] = {1.0, 1.0};

  G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
  mptAir->AddProperty("RINDEX", energy, rindexAir, 2);

  fAir->SetMaterialPropertiesTable(mptAir);*/

  //
  // Polystyrene
  //

  fPolystyrene = nistManager->FindOrBuildMaterial("G4_POLYSTYRENE"); //G4_PLASTIC_SC_VINYLTOLUENE
  
  #if SCINTILLATION

  std::vector<G4double> energyLong = {
        2.25*eV, 2.38*eV, 2.43*eV, 2.47*eV, 2.53*eV, 2.695*eV, 2.755*eV, 2.82*eV, 2.91*eV, 2.95*eV
  };
  std::vector<G4double> scintEmission = {
        0.0, 0.23, 0.35, 0.35, 0.44, 0.94, 1.0, 0.84, 0.28, 0.0
  };

  G4double rindexPolystyrene[2] = {1.59, 1.59};
  G4double absLenPolystyrene[2] = {200.*cm, 200.*cm};

  G4MaterialPropertiesTable *mptPolystyrene = new G4MaterialPropertiesTable();

  mptPolystyrene->AddConstProperty("SCINTILLATIONYIELD", 8./keV);
  mptPolystyrene->AddProperty("SCINTILLATIONCOMPONENT1", energyLong, scintEmission, energyLong.size());
  mptPolystyrene->AddProperty("RINDEX", energy, rindexPolystyrene, 2);
  mptPolystyrene->AddProperty("ABSLENGTH", energy, absLenPolystyrene, 2);
  mptPolystyrene->AddConstProperty("RESOLUTIONSCALE", 1.0);
  mptPolystyrene->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.8 * ns);
    
  fPolystyrene->SetMaterialPropertiesTable(mptPolystyrene);
  fPolystyrene->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  #endif

  //
  // Plexiglass
  //

  fPlexiglass = nistManager->FindOrBuildMaterial("G4_PLEXIGLASS");

  //
  // Alluminum
  //

  fAlluminum = nistManager->FindOrBuildMaterial("G4_Al");

  /*G4double rindexAlluminum[2] = {0.48023, 0.48023};
  G4double absLenAlluminum[2] = {1/(1.2035e+6)*cm, 1/(1.2035e+6)*cm};
  G4double reflectivityAlluminum[2] = {0.99711, 0.99711};

  G4MaterialPropertiesTable *mptAlluminum = new G4MaterialPropertiesTable();
  
  mptAlluminum->AddProperty("RINDEX", energy, rindexAlluminum, 2);
  mptAlluminum->AddProperty("ABSLENGTH", energy, absLenAlluminum, 2);

  fAlluminum->SetMaterialPropertiesTable(mptAlluminum);*/

  //
  // Mylar
  //

  fMylar = nistManager->FindOrBuildMaterial("G4_MYLAR");

  /*// Alluminum optical surface
  fAlluminumOpticalSurface = new G4OpticalSurface("AlluminumSurface");
  fAlluminumOpticalSurface->SetType(dielectric_metal);
  fAlluminumOpticalSurface->SetFinish(ground);
  fAlluminumOpticalSurface->SetModel(unified);

  G4MaterialPropertiesTable *mptOpticalAlluminum = new G4MaterialPropertiesTable();

  mptOpticalAlluminum->AddProperty("REFLECTIVITY", energy, reflectivityAlluminum, sizeof(energy)/sizeof(G4double));

  fAlluminumOpticalSurface->SetMaterialPropertiesTable(mptOpticalAlluminum);*/

  //
  // Print materials
  //

  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // Geometry parameters
  //
  #if design == 0
  auto scintillatorX = 10*mm;
  auto scintillatorY = 10*mm;
  auto scintillatorThickness = 3*mm;
  auto scintillatorOuterRadius = 3.7/2.*cm;

  auto worldSizeX = 10*cm;
  auto worldSizeY = 10*cm;
  auto worldSizeZ = 5.2*cm;

  //
  // World
  //

  auto worldS = new G4Box( "World",
    worldSizeX/2., worldSizeY/2., worldSizeZ/2.);

  auto worldLV = new G4LogicalVolume( worldS,           
    fAir,
    "World");

  auto worldPV = new G4PVPlacement(nullptr,
    G4ThreeVector(),
    worldLV,
    "World",                         
    nullptr,                            
    false,                                
    0,                          
    fCheckOverlaps);             

  //
  // Scintillator
  //

  G4double coatingZPlane[] = {-scintillatorThickness/2. - 0.100*mm, scintillatorThickness/2. + 0.100*mm};  // The height of the hexagonal prism (from z = 0 to z = 10)
  G4double coatingRInner[] = {0.0*cm, 0.0*cm};   // Inner radius of the hexagon (0 for regular hexagon)
  G4double coatingROuter[] = {scintillatorOuterRadius + 0.100*mm, scintillatorOuterRadius + 0.100*mm};   // Outer radius of the hexagon (5 units)

  auto coatingFullS = new G4Polyhedra("Coating Hexagonal prism",
    0., 360., 6, 2, coatingZPlane, coatingRInner, coatingROuter); // StartAngle, EndAngle, numSides, numFaces, zPlane, rInner, rOuter

  //G4SubtractionSolid* coatingS = new G4SubtractionSolid("Collimator", coatingFullS, scintillatorS, 0, G4ThreeVector(0, 0, 0));

  auto coatingLV
    = new G4LogicalVolume(
                 coatingFullS,     // its solid
                 fAlluminum,  // its material
                 "Coating");   // its name

  //new G4LogicalSkinSurface("AlluminumSkinSurface", coatingLV, fAlluminumOpticalSurface);

  new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(0., 0., -scintillatorThickness/2),          // at (0,0,0)
    coatingLV,                  // its logical volume
    "Coating",            // its name
    worldLV,                  // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    fCheckOverlaps);          // checking overlaps

  G4double scintZPlane[] = {-scintillatorThickness/2., scintillatorThickness/2.};  // The height of the hexagonal prism (from z = 0 to z = 10)
  G4double scintRInner[] = {0.0*cm, 0.0*cm};   // Inner radius of the hexagon (0 for regular hexagon)
  G4double scintROuter[] = {scintillatorOuterRadius, scintillatorOuterRadius};   // Outer radius of the hexagon (5 units)

  auto scintillatorS = new G4Polyhedra("Scintillator Hexagonal prism",
    0., 360., 6, 2, scintZPlane, scintRInner, scintROuter); // StartAngle, EndAngle, numSides, numFaces, zPlane, rInner, rOuter

  auto scintillatorLV = new G4LogicalVolume(
    scintillatorS,     
    fPolystyrene,  
    "Scintillator");  

  new G4PVPlacement(nullptr,
    G4ThreeVector(), 
    scintillatorLV,    
    "Scintillator",   
    coatingLV,         
    false,                    
    0,                        
    fCheckOverlaps);      
    
    //
    // Collimator
    // 

    auto collimatorX = 35.*mm;
    auto collimatorY = 30.*mm;
    auto collimatorZ = 20.*mm;
    auto holeSize = 1.*mm;
    
    auto collimatorBoxS = new G4Box("Collimator box",collimatorX/2., collimatorY/2., collimatorZ/2);
    auto collimatorHoleS = new G4Box("Collimator hole",holeSize/2., holeSize/2., collimatorZ/2);
    G4SubtractionSolid* collimatorS = new G4SubtractionSolid("Collimator", collimatorBoxS, collimatorHoleS, 0, G4ThreeVector(0, holeSize/2., 0));
    
    auto collimatorLV
      = new G4LogicalVolume(
                   collimatorS,
                   fPlexiglass, 
                   "Collimator");  
  
    new G4PVPlacement(nullptr, 
      G4ThreeVector(0., 0., -19*mm + scintillatorThickness),    
      collimatorLV,      
      "Collimator",   
      worldLV,  
      false,
      0,  
      fCheckOverlaps); 

  //
  // Visualization attributes
  //

  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  scintillatorLV->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));

  coatingLV->SetVisAttributes(G4VisAttributes(G4Colour::Red()));

  #endif

  #if design == 1

  auto scintFilmThickness = 0.025*mm;
  auto scintFilmLength = 15*mm;
  auto guideWidth= 2*mm;
  auto guideHeight = 2.625*mm;
  
  auto SiPMHeight = 1.3*mm;
  auto SiPMWidth = 1*mm;
  auto SiPMLength = 1.3*mm;

  auto worldSizeX = 10*cm;
  auto worldSizeY = 10*cm;
  auto worldSizeZ = 10*cm;
  
  //
  // World
  //

  auto worldS = new G4Box( "World",
    worldSizeX/2., worldSizeY/2., worldSizeZ/2.);

  auto worldLV = new G4LogicalVolume( worldS,           
    fAir,
    "World");

  fWorldPV = new G4PVPlacement(nullptr,
    G4ThreeVector(),
    worldLV,
    "World",                         
    nullptr,                            
    false,                                
    0,                          
    fCheckOverlaps);  
    
  
    
  auto scintFilmS = new G4Box("Scintillating Film",
    scintFilmLength/2., scintFilmLength/2., scintFilmThickness/2.);

  auto guideS = new G4Box("Light Guide",
    scintFilmLength/2., guideWidth/2., guideHeight/2.);

  auto SiPMS = new G4Box("SiPM",
    SiPMLength/2., SiPMWidth/2., SiPMHeight/2.);

  auto unionSolid1 = new G4UnionSolid("Union Solid 1", scintFilmS, guideS, nullptr, G4ThreeVector(0., scintFilmLength/2. - guideWidth/2., guideHeight/2. + scintFilmThickness/2.));
  auto unionSolid2 = new G4UnionSolid("Union Solid 2", unionSolid1, SiPMS, nullptr, G4ThreeVector(0., scintFilmLength/2. + SiPMWidth/2., guideHeight/2. + scintFilmThickness/2.));

  auto scintillatorLV = new G4LogicalVolume(
    scintFilmS,     
    fPolystyrene,  
    "Scintillator");  

  fScintPV = new G4PVPlacement(nullptr,
    G4ThreeVector(), 
    scintillatorLV,    
    "Scintillator",   
    worldLV,        
    false,                    
    0,                        
    fCheckOverlaps);
    
  auto guideLV = new G4LogicalVolume(
    guideS,     
    fPolystyrene,  
    "Light Guide");  
  
  new G4PVPlacement(nullptr,
    G4ThreeVector(0., scintFilmLength/2. - guideWidth/2., guideHeight/2. + scintFilmThickness/2.), 
    guideLV,    
    "Light Guide",   
    worldLV,      
    false,                    
    0,                        
    fCheckOverlaps);

  auto SiPMLV = new G4LogicalVolume(
    SiPMS,     
    fPolystyrene,  
    "SiPM");  

  fSiPMPV = new G4PVPlacement(nullptr,
    G4ThreeVector(0., scintFilmLength/2. + SiPMWidth/2., guideHeight/2. + scintFilmThickness/2.), 
    SiPMLV,    
    "SiPM",   
    worldLV,        
    false,                    
    0,                        
    fCheckOverlaps);

  //
  // Vis attributes
  //

  
  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  scintillatorLV->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));

  guideLV->SetVisAttributes(G4VisAttributes(G4Colour::Red()));

  SiPMLV->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));
  

  #endif

  #if design == 2

  auto scintFilmThickness = 0.025*mm;
  auto scintFilmLength = 24*mm;
  auto guideWidth= 1.3*mm;
  auto guideHeight = 1*mm;
  
  auto SiPMHeight = 1.3*mm;
  auto SiPMWidth = 1*mm;
  auto SiPMLength = 1.3*mm;

  auto coatingThickness = 0.100*mm;

  auto worldSizeX = 10*cm;
  auto worldSizeY = 10*cm;
  auto worldSizeZ = 10*cm;
  
  //
  // World
  //

  auto worldS = new G4Box( "World",
    worldSizeX/2., worldSizeY/2., worldSizeZ/2.);

  auto worldLV = new G4LogicalVolume( worldS,           
    fAir,
    "World");

  auto worldPV = new G4PVPlacement(nullptr,
    G4ThreeVector(),
    worldLV,
    "World",                         
    nullptr,                            
    false,                                
    0,                          
    fCheckOverlaps);  
    
  
    
  auto scintFilmS = new G4Box("Scintillating Film",
    scintFilmLength/2., scintFilmLength/2., scintFilmThickness/2.);

  auto guideS = new G4Box("Light Guide",
    scintFilmLength/2., guideWidth/2., guideHeight/2.);

  auto SiPMS = new G4Box("SiPM",
    SiPMLength/2., SiPMWidth/2., SiPMHeight/2.);

  auto scintillatorLV = new G4LogicalVolume(
    scintFilmS,     
    fPolystyrene,  
    "Scintillator");  

  new G4PVPlacement(nullptr,
    G4ThreeVector(), 
    scintillatorLV,    
    "Scintillator",   
    worldLV,        
    false,                    
    0,                        
    fCheckOverlaps);
    
  auto guideLV = new G4LogicalVolume(
    guideS,     
    fPolystyrene,  
    "Light Guide");  
  
  new G4PVPlacement(nullptr,
    G4ThreeVector(0., scintFilmLength/2. - guideWidth/2., guideHeight/2. + scintFilmThickness/2.), 
    guideLV,    
    "Light Guide",   
    worldLV,      
    false,                    
    0,                        
    fCheckOverlaps);

  auto SiPMLV = new G4LogicalVolume(
    SiPMS,     
    fPolystyrene,  
    "SiPM");  

  auto SiPMRotation = new G4RotationMatrix(G4ThreeVector(1., 0., 0.), pi/2.);

  fSiPMPV = new G4PVPlacement(SiPMRotation,
    G4ThreeVector(0., scintFilmLength/2. -guideWidth/2., guideHeight + scintFilmThickness/2. + SiPMWidth/2.), 
    SiPMLV,    
    "SiPM",   
    worldLV,        
    false,                    
    0,                        
    fCheckOverlaps);

  #if coating
    auto coatingFullS = new G4Box("Alluminum coating full block", scintFilmLength/2. + coatingThickness, guideWidth/2. + coatingThickness, guideHeight/2. + coatingThickness);
    auto unionSolid1 = new G4UnionSolid("Union Solid 1", scintFilmS, guideS, nullptr, G4ThreeVector(0., scintFilmLength/2. - guideWidth/2., guideHeight/2. + scintFilmThickness/2.));
    auto unionSolid2 = new G4UnionSolid("Union Solid 2", unionSolid1, SiPMS, SiPMRotation, G4ThreeVector(0., scintFilmLength/2. -guideWidth/2., guideHeight + scintFilmThickness/2. + SiPMWidth/2.));
    auto coatingS = new G4SubtractionSolid("Alluminum coating", coatingFullS, unionSolid2, nullptr, - G4ThreeVector(0., scintFilmLength/2. - guideWidth/2., guideHeight/2. + scintFilmThickness/2.));

    auto coatingLV = new G4LogicalVolume(
      coatingS,     
      fAlluminum,  
      "Alluminum coating");

    new G4PVPlacement(nullptr,
      G4ThreeVector(0., scintFilmLength/2. - guideWidth/2., guideHeight/2. + scintFilmThickness/2.), 
      coatingLV,    
      "Alluminum coating",   
      worldLV,        
      false,                    
      0,                        
      fCheckOverlaps);
    
  #endif

  //
  // Vis attributes
  //

  
  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  scintillatorLV->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));

  guideLV->SetVisAttributes(G4VisAttributes(G4Colour::Red()));

  SiPMLV->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));
  

  #endif

  #if design == 3

  auto scintFilmThickness = 0.025*mm;
  auto scintFilmLength = 24*mm;
  auto guideWidth= 1.3*mm;
  auto guideHeight = 5*mm;
  auto guideAngle = pi/2. - 0.67;
  
  auto SiPMHeight = 1.3*mm;
  auto SiPMWidth = 1*mm;
  auto SiPMLength = 1.3*mm;

  auto coatingThickness = 0.100*mm;

  auto worldSizeX = 10*cm;
  auto worldSizeY = 10*cm;
  auto worldSizeZ = 10*cm;

  G4ThreeVector scintFilmPosition = G4ThreeVector(0., 0., 0.);
  G4ThreeVector guidePosition = G4ThreeVector(0., scintFilmLength/2., guideHeight/2. - guideWidth*tan(guideAngle)/2.) + scintFilmPosition;
  G4ThreeVector SiPMPosition = G4ThreeVector(0., 0., guideHeight/2. + SiPMWidth/2.) + guidePosition;
  
  //
  // World
  //

  auto worldS = new G4Box( "World",
    worldSizeX/2., worldSizeY/2., worldSizeZ/2.);

  auto worldLV = new G4LogicalVolume( worldS,           
    fAir,
    "World");

  auto worldPV = new G4PVPlacement(nullptr,
    G4ThreeVector(),
    worldLV,
    "World",                         
    nullptr,                            
    false,                                
    0,                          
    fCheckOverlaps);  
    
  
    
  auto scintFilmS = new G4Box("Scintillating Film",
    scintFilmLength/2., scintFilmLength/2., scintFilmThickness/2.);

  auto SiPMS = new G4Box("SiPM",
    SiPMLength/2., SiPMWidth/2., SiPMHeight/2.);

  auto scintillatorLV = new G4LogicalVolume(
    scintFilmS,     
    fPolystyrene,  
    "Scintillator");  

  new G4PVPlacement(nullptr,
    scintFilmPosition, 
    scintillatorLV,    
    "Scintillator",   
    worldLV,        
    false,                    
    0,                        
    fCheckOverlaps);

  auto guideBodyS = new G4Box("Light Guide Body",
    scintFilmLength/2., guideWidth/2., guideHeight/2.);

  auto guideSubtractantS = new G4Box("Light Guide Subtractant",
    scintFilmLength/2. + 1*mm, guideWidth*sin(guideAngle), (guideWidth/cos(guideAngle))/2.);

  G4RotationMatrix* guideSubtractantRotation = new G4RotationMatrix(G4ThreeVector(1., 0., 0.), guideAngle);

  auto guideSubtractedS = new G4SubtractionSolid("Light Guide", guideBodyS, guideSubtractantS, guideSubtractantRotation, G4ThreeVector(0., guideWidth/2., -guideHeight/2.));
    
  auto guideS = new G4SubtractionSolid("Light Guide", guideSubtractedS, scintFilmS, nullptr,scintFilmPosition - guidePosition);

  auto guideLV = new G4LogicalVolume(
    guideS,     
    fPolystyrene,  
    "Light Guide");  
  
  new G4PVPlacement(nullptr,
    guidePosition, 
    guideLV,    
    "Light Guide",   
    worldLV,      
    false,                    
    0,                        
    fCheckOverlaps);

  auto SiPMLV = new G4LogicalVolume(
    SiPMS,     
    fPolystyrene,  
    "SiPM");  

  auto SiPMRotation = new G4RotationMatrix(G4ThreeVector(1., 0., 0.), pi/2.);

  fSiPMPV = new G4PVPlacement(SiPMRotation,
    SiPMPosition, 
    SiPMLV,    
    "SiPM",   
    worldLV,        
    false,                    
    0,                        
    fCheckOverlaps);

  #if coating

    auto coatingBodyS = new G4Box("Coating Body",
    (scintFilmLength + 2*coatingThickness)/2., (guideWidth + 2*coatingThickness)/2., (guideHeight + 2*coatingThickness)/2.);

    auto coatingSubtractantS = new G4Box("Coating Subtractant",
      (scintFilmLength + 2*coatingThickness)/2. + 1*mm, (guideWidth + 2*coatingThickness)*sin(guideAngle) - coatingThickness, ((guideWidth + 2*coatingThickness)/cos(guideAngle))/2.);

    G4RotationMatrix* coatingSubtractantRotation = new G4RotationMatrix(G4ThreeVector(1., 0., 0.), guideAngle);

    auto coatingFullS = new G4SubtractionSolid("Coating", coatingBodyS, coatingSubtractantS, coatingSubtractantRotation, G4ThreeVector(0., (guideWidth + 2*coatingThickness)/2., -(guideHeight + 2*coatingThickness)/2.));

    auto coatingSubtractionS1 = new G4SubtractionSolid("Coating subtraction 1", coatingFullS, guideSubtractedS, nullptr, G4ThreeVector(0., 0., 0.));
    auto coatingSubtractionS2 = new G4SubtractionSolid("Coating subtraction 2", coatingSubtractionS1, scintFilmS, nullptr, scintFilmPosition - guidePosition);
    auto coatingSubtractionS3 = new G4SubtractionSolid("Coating subtraction 3", coatingSubtractionS2, SiPMS, SiPMRotation, SiPMPosition - guidePosition);

    auto coatingLV = new G4LogicalVolume(
      coatingSubtractionS3,     
      fAlluminum,  
      "Alluminum coating");

    new G4PVPlacement(nullptr,
      guidePosition, 
      coatingLV,    
      "Alluminum coating",   
      worldLV,        
      false,                    
      0,                        
      fCheckOverlaps);
    
  #endif

  //
  // Vis attributes
  //

  
  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  scintillatorLV->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));

  guideLV->SetVisAttributes(G4VisAttributes(G4Colour::Red()));

  SiPMLV->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));
  

  #endif

  #if design == 4

  auto scintFilmThickness = 0.025*mm;
  auto scintFilmLength = 24*mm;

  auto coatingThickness = 0.100*mm;

  auto guideWidth= 1.3*mm;
  auto guideHeight = 2*mm;
  auto guideAngle = pi/2. - pi/4.;//-pi/4.;//- 0.67;
  auto guideLength = scintFilmLength - coatingThickness*4;
  
  auto SiPMHeight = 1.3*mm;
  auto SiPMWidth = 1*mm;
  auto SiPMLength = 1.3*mm;
  G4int nSides = 4;
  G4int nSiPM = 4;

  auto worldSizeX = 10*cm;
  auto worldSizeY = 10*cm;
  auto worldSizeZ = 10*cm;

  G4ThreeVector scintFilmPosition = G4ThreeVector(0., 0., 0.);
  G4ThreeVector guidePosition = G4ThreeVector(0., scintFilmLength/2. + guideWidth/2., guideHeight/2. - guideWidth*tan(guideAngle)/2.) + scintFilmPosition;
  G4ThreeVector SiPMPosition = G4ThreeVector(0., 0., guideHeight/2. + SiPMWidth/2.) + guidePosition;
  
  //
  // World
  //

  auto worldS = new G4Box( "World",
    worldSizeX/2., worldSizeY/2., worldSizeZ/2.);

  auto worldLV = new G4LogicalVolume( worldS,           
    fAir,
    "World");

  fWorldPV = new G4PVPlacement(nullptr,
    G4ThreeVector(),
    worldLV,
    "World",                         
    nullptr,                            
    false,                                
    0,                          
    fCheckOverlaps);  
    
  
    
  auto scintFilmS = new G4Box("Scintillating Film",
    scintFilmLength/2., scintFilmLength/2., scintFilmThickness/2.);

  auto SiPMS = new G4Box("SiPM",
    SiPMLength/2., SiPMWidth/2., SiPMHeight/2.);

  auto scintillatorLV = new G4LogicalVolume(
    scintFilmS,     
    fPolystyrene,  
    "Scintillator");  

  fScintPV = new G4PVPlacement(nullptr,
    scintFilmPosition, 
    scintillatorLV,    
    "Scintillator",   
    worldLV,        
    false,                    
    0,                        
    fCheckOverlaps);

  auto guideBodyS = new G4Box("Light Guide Body",
    guideLength/2., guideWidth/2., guideHeight/2.);

  auto guideSubtractantS = new G4Box("Light Guide Subtractant",
    guideLength/2. + 1*mm, guideWidth*sin(guideAngle), (guideWidth/cos(guideAngle))/2.);

  G4RotationMatrix* guideSubtractantRotation = new G4RotationMatrix(G4ThreeVector(1., 0., 0.), guideAngle);

  auto guideS = new G4SubtractionSolid("Light Guide", guideBodyS, guideSubtractantS, guideSubtractantRotation, G4ThreeVector(0., guideWidth/2., -guideHeight/2.));

  auto guideLV = new G4LogicalVolume(
    guideS,     
    fPolystyrene,  
    "Light Guide");  

  for (int i = 0; i < nSides; i++){
    auto rot = G4RotationMatrix(G4ThreeVector(0., 0., 1.), i*pi/2.);
    new G4PVPlacement(G4Transform3D(rot,
      rot*guidePosition), 
      guideLV,    
      "Light Guide",   
      worldLV,      
      false,                    
      i,                        
      fCheckOverlaps);
  }

  auto SiPMLV = new G4LogicalVolume(
    SiPMS,     
    fPolystyrene,  
    "SiPM");  

  auto SiPMRotation = new G4RotationMatrix(G4ThreeVector(1., 0., 0.), pi/2.);

  for (int i = 0; i < nSides; i++){
    for (int j = 0; j < nSiPM; j++){
      auto newSiPMPos = SiPMPosition + G4ThreeVector(scintFilmLength/double(nSiPM + 1)*(j - (nSiPM - 1)/2.), 0., 0.);
    
      auto rot = G4RotationMatrix(G4ThreeVector(0., 0., 1.), i*pi/2.);
      fSiPMPV[i + j*nSides] = new G4PVPlacement(G4Transform3D(SiPMRotation->rotateZ(i*pi/2.),
        rot*newSiPMPos), 
        SiPMLV,    
        "SiPM",   
        worldLV,
        false,                    
        i + j*nSides,                        
        fCheckOverlaps);
    }
  }

  #if coating

    auto coatingBodyS = new G4Box("Coating Body",
    (guideLength + 2*coatingThickness)/2., (guideWidth + 2*coatingThickness)/2., (guideHeight + 2*coatingThickness)/2.);

    auto coatingSubtractantS1 = new G4Box("Coating Subtractant 1",
      (guideLength + 2*coatingThickness)/2. + 1*mm, (guideWidth + 2*coatingThickness)*sin(guideAngle) - coatingThickness, ((guideWidth + 2*coatingThickness)/cos(guideAngle))/2.);

    G4RotationMatrix* coatingSubtractantRotation = new G4RotationMatrix(G4ThreeVector(1., 0., 0.), guideAngle);

    auto coatingFullS = new G4SubtractionSolid("Coating", coatingBodyS, coatingSubtractantS1, coatingSubtractantRotation, G4ThreeVector(0., (guideWidth + 2*coatingThickness)/2., -(guideHeight + 2*coatingThickness)/2.));

    auto coatingSubtractedS= new G4SubtractionSolid("coatingSubtracted 2", coatingFullS, scintFilmS, nullptr, scintFilmPosition - guidePosition);
    coatingSubtractedS = new G4SubtractionSolid("coatingSubtracted 2", coatingSubtractedS, guideS, nullptr, G4ThreeVector(0., 0., 0.));
    
    //This is reapeated for readibility purposes
    for (int j = 0; j < nSiPM; j++){
      auto newSiPMPos = SiPMPosition - guidePosition + G4ThreeVector(scintFilmLength/double(nSiPM + 1)*(j - (nSiPM - 1)/2.), 0., 0.);

      coatingSubtractedS = new G4SubtractionSolid("coatingSubtracted 2", coatingSubtractedS, SiPMS, SiPMRotation, newSiPMPos);
    }

    auto coatingLV = new G4LogicalVolume(
      coatingSubtractedS,     
      fAlluminum,  
      "Alluminum coating");

    for (int i = 0; i < nSides; i++){
      auto rot = G4RotationMatrix(G4ThreeVector(0., 0., 1.), i*pi/2.);
      new G4PVPlacement(G4Transform3D(rot,
        rot*guidePosition), 
        coatingLV,    
        "Light Guide",   
        worldLV,      
        false,                    
        i,                        
        fCheckOverlaps);
    }
    
  #endif

  //
  // Vis attributes
  //

  
  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  scintillatorLV->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));

  guideLV->SetVisAttributes(G4VisAttributes(G4Colour::Red()));

  SiPMLV->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));
  

  #endif

  #if design == 5

  auto scintFilmThickness = 0.025*mm;
  auto scintFilmLength = 24*mm;
  
  auto SiPMHeight = 1.3*mm;
  auto SiPMWidth = 1*mm;
  auto SiPMLength = 1.3*mm;

  auto worldSizeX = 3*cm;
  auto worldSizeY = 3*cm;
  auto worldSizeZ = 3*cm;

  auto guideWidth= 1.3*mm;
  auto guideHeight = 1*mm;

  G4ThreeVector scintFilmPosition = G4ThreeVector(0., 0., 0.);
  G4ThreeVector SiPMPosition = G4ThreeVector(0., scintFilmLength/2. + SiPMWidth/2., 0.) + scintFilmPosition;
  
  //
  // World
  //

  auto worldS = new G4Box( "World",
    worldSizeX/2., worldSizeY/2., worldSizeZ/2.);

  auto worldLV = new G4LogicalVolume( worldS,           
    fAir,
    "World");

  fWorldPV = new G4PVPlacement(nullptr,
    G4ThreeVector(),
    worldLV,
    "World",                         
    nullptr,                            
    false,                                
    0,                          
    fCheckOverlaps);  
    
  
    
  auto scintFilmS = new G4Box("Scintillating Film",
    scintFilmLength/2., scintFilmLength/2., scintFilmThickness/2.);

  auto SiPMS = new G4Box("SiPM",
    SiPMLength/2., SiPMWidth/2., SiPMHeight/2.);

  auto scintillatorLV = new G4LogicalVolume(
    scintFilmS,     
    fPolystyrene,  
    "Scintillator");  

  fScintPV = new G4PVPlacement(nullptr,
    scintFilmPosition, 
    scintillatorLV,    
    "Scintillator",   
    worldLV,        
    false,                    
    0,                        
    fCheckOverlaps);

  auto SiPMLV = new G4LogicalVolume(
    SiPMS,     
    fPolystyrene,  
    "SiPM");  

  //G4RotationMatrix* SiPMRotation = nullptr;

  fSiPMPV = new G4PVPlacement(nullptr,
    SiPMPosition, 
    SiPMLV,    
    "SiPM",   
    worldLV,        
    false,                    
    0,                        
    fCheckOverlaps);

  /*auto guideS = new G4Box("Light Guide",
    scintFilmLength/2., guideWidth/2., guideHeight/2.);

  auto guideLV = new G4LogicalVolume(
      guideS,     
      fPolystyrene,  
      "Light Guide");  
    
    new G4PVPlacement(nullptr,
      G4ThreeVector(0., scintFilmLength/2. - guideWidth/2., guideHeight/2. + scintFilmThickness/2.), 
      guideLV,    
      "Light Guide",   
      worldLV,      
      false,                    
      0,                        
      fCheckOverlaps);*/

  //
  // Vis attributes
  //

  
  worldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  scintillatorLV->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));

  SiPMLV->SetVisAttributes(G4VisAttributes(G4Colour::Blue()));
  

  #endif

  #if design == 6

  auto scintillatorX = 10*mm;
  auto scintillatorY = 10*mm;
  auto scintillatorThickness = 1*mm;

  auto worldSizeX = scintillatorX + 10*mm;
  auto worldSizeY = scintillatorY + 10*mm;
  auto worldSizeZ = 7*cm;

  //
  // World
  //

  auto worldS = new G4Box( "World",
    worldSizeX/2., worldSizeY/2., worldSizeZ/2.);

  fWorldLV = new G4LogicalVolume( worldS,           
    fVacuum,
    "World");

  fWorldPV = new G4PVPlacement(nullptr,
    G4ThreeVector(),
    fWorldLV,
    "World",                         
    nullptr,                            
    false,                                
    0,                          
    fCheckOverlaps);             

  //
  // Scintillator
  //

  auto scintS = new G4Box( "Scintillator",
    scintillatorX/2., scintillatorY/2.,scintillatorThickness/2.);

  auto scintLV = new G4LogicalVolume( scintS,           
    fPolystyrene,
    "Scintillator");

  fScintPV = new G4PVPlacement(nullptr,
    G4ThreeVector(),
    scintLV,
    "Scintillator",                         
    fWorldLV,                            
    false,                                
    0,                          
    fCheckOverlaps);             

  //
  // Air box
  //

  G4double airBoxThickness = 20*mm;

  auto airS = new G4Box("Air box",
    scintillatorX/2., scintillatorY/2., airBoxThickness/2.);

  auto airLV = new G4LogicalVolume( airS,           
    fVacuum,
    "Air box");
  
  auto airPV = new G4PVPlacement(nullptr,
    G4ThreeVector(0., 0., -airBoxThickness/2. - 10*mm),
    airLV,
    "Air box",                         
    fWorldLV,                            
    false,                                
    0,                          
    fCheckOverlaps);    

  //
  // Mylar sheet
  //

  G4double mylarThickness = 0.01*mm;
  
  auto mylarS = new G4Box("Mylar sheet",
    scintillatorX/2., scintillatorY/2., mylarThickness/2.);
    
  auto mylarLV = new G4LogicalVolume( mylarS,           
    fVacuum,
    "Mylar sheet");

  auto mylarPV = new G4PVPlacement(nullptr,
    G4ThreeVector(0., 0., -mylarThickness/2. - 5*mm),
    mylarLV,
    "Mylar sheet",                         
    fWorldLV,                            
    false,                                
    0,                          
    fCheckOverlaps);   

  //
  // Visualization attributes
  //

  fWorldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  scintLV->SetVisAttributes(G4VisAttributes(G4Colour::Grey()));

  airLV->SetVisAttributes(G4VisAttributes(G4Colour::Red()));

  mylarLV->SetVisAttributes(G4VisAttributes(G4Colour::Green()));

  #endif

  //
  //
  //

  return fWorldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......