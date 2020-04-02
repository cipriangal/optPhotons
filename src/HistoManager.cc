#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFileName("opnovice2")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);    // enable inactivation of histograms

  // Define histogram indices, titles
  G4int maxHisto = 12;
  G4String id[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                   "10","11","12" };
  
  G4String title[] = {
      "dummy",                                        // 0
      "Cerenkov spectrum",                            // 1
      "scintillation spectrum",                       // 2
      "boundary process status",                      // 3
      "X momentum dir of backward-going photons",     // 4
      "Y momentum dir of backward-going photons",     // 5
      "Z momentum dir of backward-going photons",     // 6
      "X momentum dir of forward-going photons",      // 7
      "Y momentum dir of forward-going photons",      // 8
      "Z momentum dir of forward-going photons",      // 9
      "X momentum dir of Fresnel-refracted photons",  //10
      "Y momentum dir of Fresnel-refracted photons",  //11
      "Z momentum dir of Fresnel-refracted photons",  //12
  };
 
  // Default values (to be reset via /analysis/h1/set command)               
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated 
  for (G4int k=0; k <= maxHisto; ++k) {
      G4int ih = analysisManager->CreateH1(id[k], title[k], nbins, vmin, vmax);
      analysisManager->SetH1Activation(ih, false);
  }
  
  analysisManager->CreateNtuple("t","some variables");
  analysisManager->CreateNtupleDColumn("x");//0
  analysisManager->CreateNtupleDColumn("y");//1
  analysisManager->CreateNtupleDColumn("z");//2
  analysisManager->CreateNtupleDColumn("px");//3
  analysisManager->CreateNtupleDColumn("py");//4
  analysisManager->CreateNtupleDColumn("pz");//5
  analysisManager->CreateNtupleIColumn("pid");//6
  analysisManager->CreateNtupleIColumn("tid");//7
  analysisManager->CreateNtupleIColumn("mid");//8
  analysisManager->CreateNtupleDColumn("e");//9
  analysisManager->CreateNtupleDColumn("ke");//10
  analysisManager->CreateNtupleIColumn("evNr");//11
  analysisManager->CreateNtupleDColumn("time");//12
  analysisManager->CreateNtupleIColumn("detID");//13
  analysisManager->FinishNtuple();
  // G4cout<<"Finished ntuple"<<G4endl;
  // std::cin.ignore();
}
