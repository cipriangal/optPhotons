// Make this appear first!
#include "G4Timer.hh"

#include "RunAction.hh"
#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "B5EventAction.hh"

#include "Run.hh"
#include "G4Run.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(PrimaryGeneratorAction* prim, B5EventAction *evtAct)
  : G4UserRunAction(),
    fTimer(nullptr),
    fRun(nullptr),
    fHistoManager(nullptr),
    fPrimary(prim),
    fEventAction(evtAct)
{
  fTimer = new G4Timer;
  fHistoManager = new HistoManager();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete fTimer;
  delete fHistoManager;
}

G4Run* RunAction::GenerateRun()
{
  fRun = new Run();
  return fRun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  if (fPrimary) {
    G4ParticleDefinition* particle = 
      fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->OpenFile();

  // if (analysisManager->IsActive()) {
  //   analysisManager->OpenFile();
  //   G4cout<<"openenddd file"<<G4endl;
  //   std::cin.ignore();
  // }else{
  //   G4cout<<"something is wrong here"<<G4endl;
  //   std::cin.ignore();
  // }
  fTimer->Start();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;

  if (isMaster) fRun->EndOfRun();

  // save histograms
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
