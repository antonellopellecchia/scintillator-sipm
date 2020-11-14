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
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include <vector>

#include "EventAction.hh"
#include "RunAction.hh"

#include "G4ThreeVector.hh"
#include "G4String.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction): G4UserEventAction() {
  this->runAction = runAction;
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event *event) {
  this->eventID = event->GetEventID();
  if (eventID%100 == 0) G4cout << eventID << "/" << runAction->nOfEvents << "\t\t" << G4endl;

  this->scintillationCount = 0;
  this->hitEnergies = vector<G4double>(0);
  this->hitTimes = vector<G4double>(0);
  this->hitPositions = vector<G4ThreeVector>(0);
  this->hitParentIDs = vector<G4int>(0);
  this->vertexPositions = vector<G4ThreeVector>(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event) {
  for (int i=0; i<hitEnergies.size(); i++) {
    this->runAction->FillNtuples(this->eventID, hitEnergies[i], hitTimes[i], hitPositions[i], hitParentIDs[i], vertexPositions[i]);
  }
  this->runAction->FillScintillationTree(this->scintillationCount);
  //G4cout << "event " << event->GetEventID() << " " << this->scintillationCount << " photons" << G4endl;
}

void EventAction::AddSipmHit(G4double energy, G4double time, G4ThreeVector position, G4int parentID, G4ThreeVector vertexPosition) {
  hitEnergies.push_back(energy);
  hitTimes.push_back(time);
  hitPositions.push_back(position);
  hitParentIDs.push_back(parentID);
  vertexPositions.push_back(vertexPosition);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
