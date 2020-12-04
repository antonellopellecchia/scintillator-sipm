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
//
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
//#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4OpticalPhoton.hh"

#include <TMath.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
  :G4UserSteppingAction(),
   scintillatorVolume(0),
   sipmVolume(0) {
  this->eventAction = eventAction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  const G4Event *event = G4RunManager::GetRunManager()->GetCurrentEvent();
  //G4int eventID = event->GetEventID();
  //G4int eventID = this->eventAction->eventID;
  G4Track *track = step->GetTrack();
  G4int trackID = track->GetTrackID();

  if (!scintillatorVolume || !sipmVolume ) {
    // In order to avoid dependence of SteppingAction
    // on DetectorConstruction class we get Envelope volume
    // from G4LogicalVolumeStore.
    scintillatorVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("ScintillatorLogical");
    sipmVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("SiPMLogical");
  }
  
  G4LogicalVolume* volume = step->GetPreStepPoint()-> GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  G4String particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();

  if (volume==scintillatorVolume) {
    //G4cout << particleName << G4endl;
    if (G4String("mu-")==particleName) {
      const vector<const G4Track *> *secondaries = step->GetSecondaryInCurrentStep();
      for (int i=0; i<secondaries->size(); i++) {
        const G4Track *secondary = secondaries->at(i);
        //G4cout << secondary->GetCreatorProcess()->GetProcessName() << " ";
        //G4cout << secondary->GetDynamicParticle()->GetParticleDefinition()->GetParticleName() << G4endl;
        if (secondary->GetParentID()>0 &&
          secondary->GetDynamicParticle()->GetParticleDefinition() ==G4OpticalPhoton::OpticalPhotonDefinition()) {
           if (secondary->GetCreatorProcess()->GetProcessName()=="Scintillation") this->eventAction->scintillationCount++;
          }
      }
    }
  } else if (volume==sipmVolume) {
    if (step->IsFirstStepInVolume() && G4String("opticalphoton")==particleName) {
      //cout << "SiPM" << endl;
      //G4cout << step->GetPreStepPoint() << G4endl;
      G4StepPoint *point = step->GetPostStepPoint();
      //G4cout << particleName << G4endl;
      //G4cout << eventID << G4endl;
      /*G4cout << track->GetCreatorProcess()->GetProcessName() << G4endl;
      G4cout << track->GetOriginTouchable()->GetVolume()->GetName() << G4endl;
      G4cout << point->GetProcessDefinedStep()->GetProcessName() << G4endl;*/
      this->eventAction->AddSipmHit(
        point->GetTotalEnergy()*1.e6,
        point->GetGlobalTime(),
        point->GetPosition(),
        step->GetTrack()->GetParentID(),
        step->GetTrack()->GetVertexPosition()
      );
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

