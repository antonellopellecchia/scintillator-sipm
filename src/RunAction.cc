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
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include <iostream>
#include <fstream>
#include <sstream>

#include <sys/stat.h>

#include <TCanvas.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TH2F.h>
#include <TFile.h> 
#include <TStyle.h>
#include <TF1.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TTree.h>

#include "style.cc"

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(G4bool headless, string outFilePath): G4UserRunAction() {
  this->headless = headless;
  //this->heedSimulation = new HeedSimulation(this);

  runFile = new TFile(outFilePath.c_str(), "RECREATE", "File containing simulation output ntuples");

  runTree = new TTree("runTree", "");
  runTree->Branch("hitEnergy", &hitEnergy, "hitEnergy/D");
  runTree->Branch("hitTime", &hitTime, "hitTime/D");
  runTree->Branch("hitParentID", &hitParentID, "hitParentID/D");
  runTree->Branch("hitEventID", &hitEventID, "hitEventID/D");
  runTree->Branch("hitPositionX", &hitPositionX, "hitPositionX/D");
  runTree->Branch("hitPositionY", &hitPositionY, "hitPositionY/D");
  runTree->Branch("hitPositionZ", &hitPositionZ, "hitPositionZ/D");
  runTree->Branch("vertexPositionX", &vertexPositionX, "vertexPositionX/D");
  runTree->Branch("vertexPositionY", &vertexPositionY, "vertexPositionY/D");
  runTree->Branch("vertexPositionZ", &vertexPositionZ, "vertexPositionZ/D");

  scintillationTree = new TTree("scintillationTree", "");
  scintillationTree->Branch("nPhotons", &scintillationCount, "nPhotons/I");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run) {
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  nOfEvents = run->GetNumberOfEventToBeProcessed();
  G4cout << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run) {
  /*G4String out_dir = G4String("./out/");
  G4String root_out_dir = G4String(out_dir+"root/");
  G4String eps_out_dir = G4String(out_dir+"eps/");
  mkdir(out_dir.c_str(), 0700);
  mkdir(root_out_dir.c_str(), 0700);
  mkdir(eps_out_dir.c_str(), 0700);*/

  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  if (this->headless) {
    G4cout << G4endl;

    runTree->Print();
    scintillationTree->Print();
    runFile->Write();
    runFile->Close();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::FillNtuples(G4int eventID, G4double energy, G4double time, G4ThreeVector position, G4int parentID, G4ThreeVector vertexPosition) {
  hitEnergy = energy;
  hitTime = time;
  hitParentID = parentID;
  hitEventID = eventID;

  hitPositionX = position.getX();
  hitPositionY = position.getY();
  hitPositionZ = position.getZ();

  vertexPositionX = vertexPosition.getX();
  vertexPositionY = vertexPosition.getY();
  vertexPositionZ = vertexPosition.getZ();

  runTree->Fill();
}

void RunAction::FillScintillationTree(G4int scintillationCount) {
  this->scintillationCount = scintillationCount;

  scintillationTree->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

