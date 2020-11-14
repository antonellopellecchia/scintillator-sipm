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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction():G4VUserDetectorConstruction() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  ConstructMaterials();

  G4Material* air = G4Material::GetMaterial("G4_AIR");
  G4Material* polyvinyltoluene = G4Material::GetMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  // teflon wrapping?

  //G4Element* Cl = new G4Element("Chlorine", "Cl", 17., 35.5*g/mole);
  //G4Element* C = new G4Element("Carbon", "C", 6., 12.0*g/mole);
  //G4Element* H = new G4Element("Hydrozen", "H", 1., 1.00794*g/mole);
  //G4Element* O  = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
  //G4Element *Al  = new G4Element("Aluminum", "Al", 13., 26.98*g/mole);

  G4MaterialPropertiesTable *scintillatorProperties = new G4MaterialPropertiesTable();

  scintillatorProperties->AddConstProperty("SCINTILLATIONYIELD", 9000./MeV);
  scintillatorProperties->AddConstProperty("FASTTIMECONSTANT", 0.7*ns);
  scintillatorProperties->AddConstProperty("RESOLUTIONSCALE", 1.0);
  scintillatorProperties->AddConstProperty("YIELDRATIO", 1.0);
  
  const G4int NPHOTONENERGIES = 22;
  G4double scintResponseWavelengths[NPHOTONENERGIES] = {3.26*eV,3.22*eV,3.17*eV,3.13*eV,3.10*eV,3.06*eV,3.02*eV,2.98*eV,2.95*eV,2.91*eV,2.88*eV,2.83*eV,2.78*eV,2.75*eV,2.72*eV,2.69*eV,2.66*eV,2.63*eV,2.61*eV,2.58*eV,2.53*eV,2.50*eV};
  G4double scintResponseSpectrum[NPHOTONENERGIES] = {1.69, 3.13, 4.21, 4.92, 7.8, 9.59, 11.75, 15.16, 18.93, 25.22, 35.11, 43.9, 48.57, 53.6, 60.78, 85.93, 99.4, 91.32, 75.69, 35.28, 17.5, 4.38};
  G4double scintRefractiveIndex[NPHOTONENERGIES];
  G4double scintAbsLength[NPHOTONENERGIES];
  for (int i=0; i<NPHOTONENERGIES; i++) {
    scintRefractiveIndex[i] = 1.58;
    scintAbsLength[i] = 140.*cm;
  }
  scintillatorProperties->AddProperty("FASTCOMPONENT", scintResponseWavelengths, scintResponseSpectrum, NPHOTONENERGIES);
  scintillatorProperties->AddProperty("RINDEX", scintResponseWavelengths, scintRefractiveIndex, NPHOTONENERGIES);
  scintillatorProperties->AddProperty("ABSLENGTH", scintResponseWavelengths, scintAbsLength, NPHOTONENERGIES);

  polyvinyltoluene->SetMaterialPropertiesTable(scintillatorProperties);

  /*const G4int nCherenkovMomenta = 101;
  G4double pCherenkov[nCherenkovMomenta] = {0.24796837486619944*eV, 0.25605986665241576*eV, 0.26447138957572464*eV, 0.2730929238614531*eV, 0.2820386429324379*eV, 0.29124779758773717*eV, 0.3007864809148465*eV, 0.310659452350538*eV, 0.32078703087477284*eV, 0.3313313400136283*eV, 0.34211972249751577*eV, 0.35333196760643976*eV, 0.3648740065718061*eV, 0.37685163353525747*eV, 0.38915313067514035*eV, 0.40189363835688724*eV, 0.41507930175125446*eV, 0.42856615082302013*eV, 0.44264258276722496*eV, 0.45716883271791925*eV, 0.47196112460258743*eV, 0.4875508746877692*eV, 0.5033868754896456*eV, 0.5198498424867913*eV, 0.5369605345738402*eV, 0.5544909992535765*eV, 0.572675230637874*eV, 0.5912455290085823*eV, 0.6107595440054173*eV, 0.630641848591555*eV, 0.6511774550057758*eV, 0.6727302627948981*eV, 0.6945892853394943*eV, 0.71750108468229*eV, 0.740646280962364*eV, 0.7648623530727928*eV, 0.7902115196500938*eV, 0.8156854436388139*eV, 0.842856474732153*eV, 0.8700644732147348*eV, 0.8984361408195632*eV, 0.9280253550381714*eV, 0.9581467344134444*eV, 0.9894987025786092*eV, 1.0221285031582827*eV, 1.055184573898721*eV, 1.090450197300789*eV, 1.1261052446239757*eV, 1.1630786813611604*eV, 1.2002341474646634*eV, 1.2398418743309971*eV, 1.280431554612204*eV, 1.3222159265553985*eV, 1.3656150174369392*eV, 1.4101932146621896*eV, 1.456410048550449*eV, 1.5039324045742324*eV, 1.5531026861217552*eV, 1.6039351543738642*eV, 1.6564353698476915*eV, 1.7105986124875787*eV, 1.766659838032199*eV, 1.8243700328590307*eV, 1.8839718497659885*eV, 1.9457656533757017*eV, 2.0094681917844364*eV, 2.075049162060246*eV, 2.1428307541151006*eV, 2.2132129138361245*eV, 2.285422809826723*eV, 2.360254853095369*eV, 2.437275160862978*eV, 2.5169343774482282*eV, 2.5992492124339566*eV, 2.684802672869201*eV, 2.7724549962678826*eV, 2.8633761531893698*eV, 2.956932683832571*eV, 3.0537977200270863*eV, 3.153209242957775*eV, 3.2567425120330897*eV, 3.362739013645232*eV, 3.472946426697471*eV, 3.586467672348849*eV, 3.7043378378577745*eV, 3.8254917443103893*eV, 3.9497988987926*eV, 4.079769247551817*eV, 4.212850405473996*eV, 4.350322366073675*eV, 4.493808895726702*eV, 4.640126775190858*eV, 4.792585521186692*eV, 4.949468560203582*eV, 5.110642515791414*eV, 5.278168898812249*eV, 5.449854392663723*eV, 5.627970378261448*eV, 5.8126670151476665*eV, 6.004076873273594*eV, 6.199209371654986*eV};
  G4double refractiveIndex[nCherenkovMomenta] = {1.6240317502663,1.6332058556533,1.6417334442682,1.6495581507485,1.6568352356965,1.663555792699,1.6698063350772,1.6756189316007,1.6809814756374,1.6860063236089,1.6906368746825,1.6949749939614,1.6990024419669,1.7027737381029,1.7062709711188,1.709543722848,1.7126051747702,1.715438147562,1.7181145792243,1.7206148427182,1.7229226773359,1.7251280028472,1.7271600353641,1.7290784375914,1.730889247653,1.7325755211079,1.7341669205685,1.7356472403196,1.7370650289788,1.7383827319917,1.7396263439883,1.7408195373476,1.7419273828406,1.742992131424,1.7439802356172,1.7449319592359,1.7458498390026,1.7467019118788,1.7475432140301,1.7483249348844,1.7490839326181,1.7498225628591,1.7505264589423,1.7512147150232,1.751889641765,1.7525364667588,1.7531915603611,1.7538227008788,1.7544492215126,1.7550547342523,1.7556780777757,1.7562973768044,1.7569182191283,1.7575488420438,1.7581849835841,1.7588353492177,1.7594974153468,1.7601781977445,1.7608801757554,1.7616058134504,1.7623575309256,1.7631412029905,1.7639560666816,1.7648083776856,1.7657055418929,1.7666467187031,1.7676347594555,1.768678092594,1.7697870034397,1.7709534510436,1.7721946418157,1.7735082113872,1.7749069668851,1.7763969806669,1.7779954669163,1.7796876970942,1.7815032886078,1.7834376078764,1.7855131978094,1.7877228296252,1.7901123299044,1.792654904549,1.7954050494599,1.7983552384705,1.8015488081449,1.8049747310816,1.8086465749755,1.812662211763,1.8169687643807,1.8216335887885,1.8267479542496,1.8322345043326,1.8382585951889,1.8448039892896,1.8519174110987,1.859757418441,1.8682985747984,1.8777446762354,1.8882235599463,1.8998877983536,1.9127094080587};
  G4MaterialPropertiesTable *sapphireProperties = new G4MaterialPropertiesTable();
  sapphireProperties->AddProperty("RINDEX", pCherenkov, refractiveIndex, nCherenkovMomenta)->SetSpline(true);
  sapphire->SetMaterialPropertiesTable(sapphireProperties);*/

  double copperDensity = 8.960*g/cm3;
  double copperAtomicWeight = 63.55*g/mole;
  G4Material *copper = new G4Material("Copper", 29., copperAtomicWeight, copperDensity);
  const G4int k_ = 1;
  G4double pCherenkovCopper[k_] = {2.*eV};
  G4double copperRefractiveIndex[k_] = {1.6};
  G4MaterialPropertiesTable *copperProperties = new G4MaterialPropertiesTable();
  copperProperties->AddProperty("RINDEX", pCherenkovCopper, copperRefractiveIndex, k_)->SetSpline(true);
  copper->SetMaterialPropertiesTable(copperProperties);
  
  G4double scintillatorSizeX = 100*mm;
  G4double scintillatorSizeY = 10*mm;
  G4double sipmCathodeLength = 6*mm;
  
  G4double scintillatorThickness = 100*mm;
  G4double sipmCathodeThickness = 0.5*mm;

  G4double scintillatorZ = 0.*mm;
  G4double sipmCathodeZ = scintillatorZ + 0.5*scintillatorThickness + 0.5*sipmCathodeThickness;
  
  // maximum object size in xy
  G4double sizeXY = scintillatorSizeX*1.2;
  // envelope parameters
  G4double envSizeXY = 2*sizeXY;
  G4double envSizeZ = 3*(scintillatorThickness+sipmCathodeThickness);
  
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;
  
  //     
  // World
  //
  G4double worldSizeXY = 1.2*envSizeXY;
  G4double worldSizeZ  = 1.2*envSizeZ;
  G4Material* worldMaterial = air;

  G4Box* solidWorld = new G4Box("World", 0.5*worldSizeXY, 0.5*worldSizeXY, 0.5*worldSizeZ);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMaterial, "World");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, checkOverlaps);
  
  //
  // Envelope
  //
  G4Material* envMaterial = air;
  G4Box* solidEnv = new G4Box("Envelope", 0.5*envSizeXY, 0.5*envSizeXY, 0.5*envSizeZ);
  G4LogicalVolume* logicEnv = new G4LogicalVolume(solidEnv, envMaterial, "Envelope");
  new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

  //
  // Scintillator
  //
  G4Box *scintillatorSolid = new G4Box("ScintillatorBox", 0.5*scintillatorSizeX, 0.5*scintillatorSizeY, 0.5*scintillatorThickness);
  fScintillatorLogical = new G4LogicalVolume(scintillatorSolid, polyvinyltoluene, "ScintillatorLogical");
  new G4PVPlacement(0, G4ThreeVector(0.,0.,scintillatorZ), fScintillatorLogical, "ScintillatorPhysical", logicEnv, false, 0, checkOverlaps);

  //
  // SiPM window
  //
  G4Box *sipmSolid = new G4Box("SiPMBox", 0.5*sipmCathodeLength, 0.5*sipmCathodeLength, 0.5*sipmCathodeThickness);
  fSipmLogical = new G4LogicalVolume(sipmSolid, copper, "SiPMLogical");
  new G4PVPlacement(0, G4ThreeVector(0.,0.,sipmCathodeZ), fSipmLogical, "SiPMPhysical", logicEnv, false, 0, checkOverlaps);

  return physWorld;
}

void DetectorConstruction::ConstructMaterials() {
   auto nistManager = G4NistManager::Instance();

   // Air 
   nistManager->FindOrBuildMaterial("G4_AIR");

   // Argon gas
   nistManager->FindOrBuildMaterial("G4_Ar");
   // With a density different from the one defined in NIST
   // G4double density = 1.782e-03*g/cm3; 
   // nistManager->BuildMaterialWithNewDensity("B5_Ar","G4_Ar",density);
   // !! cases segmentation fault

   // Scintillator
   // (PolyVinylToluene, C_9H_10)
   nistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

   // Vacuum "Galactic"
   // nistManager->FindOrBuildMaterial("G4_Galactic");

   // Vacuum "Air with low density"
   // auto air = G4Material::GetMaterial("G4_AIR");
   // G4double density = 1.0e-5*air->GetDensity();
   // nistManager
   //   ->BuildMaterialWithNewDensity("Air_lowDensity", "G4_AIR", density);

   G4cout << G4endl << "The materials defined are: " << G4endl << G4endl;
   G4cout << *(G4Material::GetMaterialTable()) << G4endl;
 }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
