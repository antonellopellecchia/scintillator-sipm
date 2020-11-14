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
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "RunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(EventAction *eventAction)
  : G4VUserPrimaryGeneratorAction(),
    fParticleGun(0), 
    fEnvelopeBox(0),
    fScintillatorBox(0),
    fEventAction(eventAction)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition *muon = particleTable->FindParticle("mu-");
  fParticleGun->SetParticleDefinition(muon);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.));
  fParticleGun->SetParticleEnergy(4.*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of ecah event
  //

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get Envelope volume
  // from G4LogicalVolumeStore.
  
  G4double envSizeXY = 0;
  G4double envSizeZ = 0;

  if (!fRunAction) fRunAction = (RunAction *)G4RunManager::GetRunManager()->GetUserRunAction();

  if (!fEnvelopeBox)
    {
      G4LogicalVolume* envLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
      if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
    }

  if ( fEnvelopeBox ) {
    envSizeXY = fEnvelopeBox->GetXHalfLength()*2.;
    envSizeZ = fEnvelopeBox->GetZHalfLength()*2.;
  }  
  else  {
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
		"MyCode0002",JustWarning,msg);
  }

  G4double size = 0.8;
  G4double x0 = size * envSizeXY * (G4UniformRand()-0.5);
  G4double y0 = size * envSizeXY * (G4UniformRand()-0.5);
  G4double z0 = -0.5 * envSizeZ;

  /*
    Define cherenkov tape to position
    the beam right in front of it
  */ 

  G4double scintillatorSizeX = 0;
  G4double scintillatorSizeY = 0;
  G4double scintillatorSizeZ = 0;
  if (!fScintillatorBox) {
    G4LogicalVolume *scintillatorLV = G4LogicalVolumeStore::GetInstance()->GetVolume("ScintillatorLogical");
    if (scintillatorLV) fScintillatorBox = dynamic_cast<G4Box*>(scintillatorLV->GetSolid());
  }

  if (fScintillatorBox) {
    scintillatorSizeX = fScintillatorBox->GetXHalfLength()*2.;
    scintillatorSizeY = fScintillatorBox->GetYHalfLength()*2.;
    scintillatorSizeZ = fScintillatorBox->GetZHalfLength()*2.;
  } else {
    G4ExceptionDescription msg;
    msg << "Cherenkov logic box not found.\n";
    msg << "The gun will be place at the center.";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
		"MyCode0002",JustWarning,msg);
  }
  
  // uniform beam position
  size = 0.99;
  x0 = size * scintillatorSizeX * (G4UniformRand()-0.5);
  y0 = size * scintillatorSizeY * (G4UniformRand()-0.5);
  z0 = size * scintillatorSizeZ * (G4UniformRand()-0.5);
  y0 = scintillatorSizeY * 10;

  // gaussian positioning, center in (0,0), sigma 1 mm
  //G4double r0 = G4RandGauss::shoot(0, beamSigma);
  //G4double theta0 = G4UniformRand()*CLHEP::pi;
  //x0 = r0*cos(theta0);
  //y0 = r0*sin(theta0);
  //std::tuple<G4double, G4double> posBeginning = std::make_tuple(x0, y0);
  //fEventAction->AddBeginningPosition(posBeginning);

  /*x0 = 0;
  y0 = 0;
  z0 = -0.5*envSizeZ;*/
  //x0 = G4RandGauss::shoot(0, beamSigma);
  //y0 = G4RandGauss::shoot(0, beamSigma);
  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.));

  // gaussian positioning, center in (0,0), sigma 1 mm
  //G4double beamSigma = 1.*mm;
  //G4double r0 = G4RandGauss::shoot(0, beamSigma);
  //G4double theta0 = G4UniformRand()*CLHEP::pi;
  //x0 = r0*cos(theta0);
  //y0 = r0*sin(theta0);

  //x0 = G4RandGauss::shoot(0, beamSigma);
  //y0 = G4RandGauss::shoot(0, beamSigma);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

