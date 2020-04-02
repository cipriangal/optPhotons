#ifndef B5EventAction_h
#define B5EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4Types.hh"

class B5EventAction : public G4UserEventAction
{
public:
    B5EventAction();
    virtual ~B5EventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    
  G4long GetEventID(){return eventId;}
private:
  G4long eventId;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
