//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file optical/OpNovice2/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fDetectorMessenger(nullptr)
{
  fExpHall_x = fExpHall_y = fExpHall_z = 0.1*m;
  fTank_x    = fTank_y    = 5*cm;
  fTank_z    = 1.*cm;

  fTank = nullptr;

  fTankMPT    = new G4MaterialPropertiesTable();
  fWorldMPT   = new G4MaterialPropertiesTable();
  fSurfaceMPT = new G4MaterialPropertiesTable();

  fSurface = new G4OpticalSurface("Surface");
  fSurface->SetType(dielectric_dielectric);
  fSurface->SetFinish(ground);
  fSurface->SetModel(unified);
  fSurface->SetMaterialPropertiesTable(fSurfaceMPT);
  
  fTank_LV  = nullptr;
  fWorld_LV = nullptr;

  fTankMaterial  = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
  fWorldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  fDetMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");

  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fDetectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{

  /*eicdirc additions*/
  static const G4double LambdaE = 2.0 * 3.14159265358979323846 * 1.973269602e-16 * m * GeV;
  const G4int num = 36;
  G4double WaveLength[num];
  G4double PhotonEnergy[num]; // energy of photons which correspond to the given 

  // absorption of quartz per 1 m - from jjv
  G4double QuartzAbsorption[num] = 
    {0.999572036,0.999544661,0.999515062,0.999483019,0.999448285,
     0.999410586,0.999369611,0.999325013,0.999276402,0.999223336,
     0.999165317,0.999101778,0.999032079,0.998955488,0.998871172,
     0.998778177,0.99867541,0.998561611,0.998435332,0.998294892,0.998138345,
     0.997963425,0.997767484,0.997547418,
     0.99729958,0.99701966,0.99670255,0.996342167,0.995931242,0.995461041,
     0.994921022,0.994298396,0.993577567,0.992739402,0.991760297,0.990610945};

  // only phase refractive indexes are necessary -> g4 calculates group itself !!  
  G4double QuartzRefractiveIndex[num]={
    1.456535,1.456812,1.4571,1.457399,1.457712,1.458038,1.458378,
    1.458735,1.459108,1.4595,1.459911,1.460344,1.460799,1.46128,
    1.461789,1.462326,1.462897,1.463502,1.464146,1.464833,
    1.465566,1.46635,1.46719,1.468094,1.469066,1.470116,1.471252,1.472485,
    1.473826,1.475289,1.476891,1.478651,1.480592,1.482739,1.485127,1.487793};

  for(int i=0;i<num;i++){
    WaveLength[i]= (300 +i*10)*nanometer;
    PhotonEnergy[num-(i+1)]= LambdaE/WaveLength[i];

    /* as the absorption is given per length and G4 needs 
       mean free path length, calculate it here
       mean free path length - taken as probability equal 1/e
       that the photon will be absorbed */

    QuartzAbsorption[i] = (-1)/log(QuartzAbsorption[i])*100*cm;
  }

  // Quartz material => Si02
  G4MaterialPropertiesTable* QuartzMPT = new G4MaterialPropertiesTable();
  QuartzMPT->AddProperty("RINDEX",       PhotonEnergy, QuartzRefractiveIndex,num);
  QuartzMPT->AddProperty("ABSLENGTH",    PhotonEnergy, QuartzAbsorption,     num);
  
  G4String symbol;             //a=mass of a mole;
  G4double a, z, density;      //z=mean number of protons;  
  G4int ncomponents, natoms;

  G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);
  G4Element* Si = new G4Element("Silicon" ,symbol="Si", z= 14., a= 28.09*g/mole);
 // quartz material = SiO2
  G4Material* SiO2 = new G4Material("quartz",density= 2.200*g/cm3, ncomponents=2);
  SiO2->AddElement(Si, natoms=1);
  SiO2->AddElement(O , natoms=2);

  /*eicdirc additions*/

  fTankMaterial = SiO2;
  fTankMaterial->SetMaterialPropertiesTable(QuartzMPT);

  // fTankMaterial->SetMaterialPropertiesTable(fTankMPT);
  // fTankMaterial->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  fWorldMaterial->SetMaterialPropertiesTable(fWorldMPT);

  // ------------- Volumes --------------
  // The experimental Hall
  G4Box* world_box = new G4Box("World", fExpHall_x, fExpHall_y, fExpHall_z);

  fWorld_LV
    = new G4LogicalVolume(world_box, fWorldMaterial, "World", 0, 0, 0);

  G4VPhysicalVolume* world_PV
    = new G4PVPlacement(0, G4ThreeVector(), fWorld_LV, "World", 0, false, 0);

  // The tank
  G4Box* tank_box = new G4Box("Tank", fTank_x, fTank_y, fTank_z);

  fTank_LV
    = new G4LogicalVolume(tank_box, fTankMaterial, "Tank", 0, 0, 0);

  fTank
    = new G4PVPlacement(0, G4ThreeVector(), fTank_LV, "Tank",
                        fWorld_LV, false, 0);

  G4Box* detBox = new G4Box("detSolid", fTank_x, 1*mm, fTank_z);
  
  fdet1_LV = new G4LogicalVolume(detBox, fDetMaterial, "det1", 0,0, 0);
  fdet2_LV = new G4LogicalVolume(detBox, fDetMaterial, "det2", 0,0, 0);

  fdet1 = new G4PVPlacement(0, G4ThreeVector(0, fTank_y+1*mm,0),fdet1_LV, "det1",fWorld_LV,false,0);
  fdet2 = new G4PVPlacement(0, G4ThreeVector(0,-fTank_y-1*mm,0),fdet1_LV, "det2",fWorld_LV,false,0);


  /*
  // ------------- Surface --------------

  G4LogicalBorderSurface* surface =
          new G4LogicalBorderSurface("Surface",
                                 fTank, world_PV, fSurface);

  G4OpticalSurface* opticalSurface = dynamic_cast <G4OpticalSurface*>
        (surface->GetSurface(fTank,world_PV)->GetSurfaceProperty());
  G4cout << "******  opticalSurface->DumpInfo:" << G4endl;
  if (opticalSurface) { opticalSurface->DumpInfo(); }
  G4cout << "******  end of opticalSurface->DumpInfo" << G4endl;
  */

  return world_PV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetSurfaceSigmaAlpha(G4double v) {
  fSurface->SetSigmaAlpha(v);
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  G4cout << "Surface sigma alpha set to: " << fSurface->GetSigmaAlpha()
         << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddTankMPV(const char* c,
                                     G4MaterialPropertyVector* mpv) {
  mpv->SetSpline(true);
  fTankMPT->AddProperty(c, mpv);
  G4cout << "The MPT for the box is now: " << G4endl;
  fTankMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddWorldMPV(const char* c,
                                       G4MaterialPropertyVector* mpv) {
  mpv->SetSpline(true);
  fWorldMPT->AddProperty(c, mpv);
  G4cout << "The MPT for the world is now: " << G4endl;
  fWorldMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddSurfaceMPV(const char* c,
                                         G4MaterialPropertyVector* mpv) {
  mpv->SetSpline(true);
  fSurfaceMPT->AddProperty(c, mpv);
  G4cout << "The MPT for the surface is now: " << G4endl;
  fSurfaceMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddTankMPCV(const char* c, G4double v) {
  fTankMPT->AddConstProperty(c, v);
  G4cout << "The MPT for the box is now: " << G4endl;
  fTankMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::AddWorldMPCV(const char* c, G4double v) {
  fWorldMPT->AddConstProperty(c, v);
  G4cout << "The MPT for the world is now: " << G4endl;
  fWorldMPT->DumpTable();
  G4cout << "............." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetWorldMaterial(const G4String& mat) {
  G4Material* pmat = G4NistManager::Instance()->FindOrBuildMaterial(mat);
  if (pmat && fWorldMaterial != pmat) {
    fWorldMaterial = pmat;
    if (fWorld_LV) {
      fWorld_LV->SetMaterial(fWorldMaterial);
      fWorldMaterial->SetMaterialPropertiesTable(fWorldMPT);
    }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    G4cout << "World material set to " << fWorldMaterial->GetName()
           << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetTankMaterial(const G4String& mat) {
  G4Material* pmat = G4NistManager::Instance()->FindOrBuildMaterial(mat);
  if (pmat && fTankMaterial != pmat) {
    fTankMaterial = pmat;
    if (fTank_LV) {
      fTank_LV->SetMaterial(fTankMaterial);
      fTankMaterial->SetMaterialPropertiesTable(fTankMPT);
      fTankMaterial->GetIonisation()->SetBirksConstant(0.126*mm/MeV);
    }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    G4cout << "Tank material set to " << fTankMaterial->GetName()
           << G4endl;
  }
}
