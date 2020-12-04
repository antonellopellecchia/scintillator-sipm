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
/// \file DetectorConstructionSmall.hh
/// \brief Definition of the DetectorConstructionSmall class

#ifndef DetectorConstructionSmall_h
#define DetectorConstructionSmall_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

// 10 x 1 x 0.5 cm scintillator with single SiPM on one short face

class DetectorConstructionSmall : public G4VUserDetectorConstruction
{
public:
  DetectorConstructionSmall();
  virtual ~DetectorConstructionSmall();

  virtual G4VPhysicalVolume* Construct();

  void ConstructMaterials();
    
  G4LogicalVolume* GetScintillator() const { return fScintillatorLogical; }
  G4LogicalVolume* GetSipm() const { return fSipmLogical; }

protected:
  G4LogicalVolume *fScintillatorLogical;
  G4LogicalVolume *fSipmLogical;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

