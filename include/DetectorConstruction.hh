#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4OpticalSurface.hh"

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RunManager.hh"


class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  virtual ~DetectorConstruction();

  G4VPhysicalVolume* GetTank() {return fTank;}
  G4double GetTankXSize() {return fTank_x;}

  G4OpticalSurface* GetSurface(void) {return fSurface;}

  void SetSurfaceFinish(const G4OpticalSurfaceFinish finish) {
    fSurface->SetFinish(finish);
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
  }
  G4OpticalSurfaceFinish GetSurfaceFinish(void) 
  {return fSurface->GetFinish();}

  void SetSurfaceType(const G4SurfaceType type) {
    fSurface->SetType(type);
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
  }
    
  void SetSurfaceModel(const G4OpticalSurfaceModel model) {
    fSurface->SetModel(model);
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
  }
  G4OpticalSurfaceModel GetSurfaceModel(void) 
  {return fSurface->GetModel();}

  void SetSurfaceSigmaAlpha(G4double v);

  void AddTankMPV(const char* c, G4MaterialPropertyVector* mpv);
  void AddTankMPCV(const char* c, G4double v);
  G4MaterialPropertiesTable* GetTankMaterialPropertiesTable() 
  {return fTankMPT;}

  void AddWorldMPV(const char* c, G4MaterialPropertyVector* mpv);
  void AddWorldMPCV(const char* c, G4double v);
  G4MaterialPropertiesTable* GetWorldMaterialPropertiesTable() 
  {return fWorldMPT;}

  void AddSurfaceMPV(const char* c, G4MaterialPropertyVector* mpv);
  G4MaterialPropertiesTable* GetSurfaceMaterialPropertiesTable() 
  {return fSurfaceMPT;}

  void        SetWorldMaterial(const G4String&);
  G4Material* GetWorldMaterial() const {return fWorldMaterial;}
  void        SetTankMaterial(const G4String&);
  G4Material* GetTankMaterial() const {return fTankMaterial;}

  virtual G4VPhysicalVolume* Construct();

private:
  G4double fExpHall_x;
  G4double fExpHall_y;
  G4double fExpHall_z;

  G4VPhysicalVolume* fTank;

  G4double fTank_x;
  G4double fTank_y;
  G4double fTank_z;

  G4LogicalVolume* fWorld_LV;
  G4LogicalVolume* fTank_LV;

  G4VPhysicalVolume* fdet1;
  G4LogicalVolume* fdet1_LV;

  G4VPhysicalVolume* fdet2;
  G4LogicalVolume* fdet2_LV;

  G4Material* fWorldMaterial;
  G4Material* fTankMaterial;
  G4Material* fDetMaterial;

  G4OpticalSurface* fSurface;

  DetectorMessenger* fDetectorMessenger;

  G4MaterialPropertiesTable* fTankMPT;
  G4MaterialPropertiesTable* fWorldMPT;
  G4MaterialPropertiesTable* fSurfaceMPT;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*DetectorConstruction_h*/
