#include "root-style/style.cc"

#include <regex>

double planckConstant = 6.63e-34;
double speedOfLight = 3.e8;
double electronCharge = 1.6e-19;

void analysis(string rootFilePath="", bool withTts=false) {
  TStyle *style = setStyle();
  style->cd();
  //style->SetOpztStat(0);
  gROOT->SetStyle("Garfield");
  gROOT->ForceStyle();

  // read quantum efficiency csv spectrum
  TH1F *hQuantumEfficiency = new TH1F("hQuantumEfficiency", ";photon energy [eV]; quantum efficiency [%]", 50, 0., 7.);
  TNtuple *qeTree = new TNtuple("qeTree", "qeTree", "wavelength:qe");
  qeTree->ReadFile("../MCP_QE.csv");
  qeTree->Print();
  float photonWavelength, photonQe, photonEnergy;
  TBranch *wavelengthBranch = qeTree->GetBranch("wavelength");
  TBranch *qeBranch = qeTree->GetBranch("qe");
  wavelengthBranch->SetAddress(&photonWavelength);
  qeBranch->SetAddress(&photonQe);
  for (int i=0; i<qeTree->GetEntries(); i++) {
    qeTree->GetEvent(i);
    photonEnergy = planckConstant*speedOfLight/(photonWavelength*1e-9)/electronCharge;
    hQuantumEfficiency->SetBinContent(hQuantumEfficiency->FindBin(photonEnergy), photonQe);
  }
  double maxQe = hQuantumEfficiency->GetBinContent(hQuantumEfficiency->GetMaximumBin());
  cout << "Maximum quantum efficiency " << maxQe << endl;
  TCanvas *cMcpQe = new TCanvas("cMcpQe", "", 700, 600);
  hQuantumEfficiency->Draw();
  cMcpQe->SaveAs("out/mcp_qe.png");
  cMcpQe->SaveAs("out/mcp_qe.eps");

  const int nCherenkovMomenta = 101;
  double pCherenkov[nCherenkovMomenta] = {0.24796837486619944,0.25605986665241576,0.26447138957572464,0.2730929238614531,0.2820386429324379,0.29124779758773717,0.3007864809148465,0.310659452350538,0.32078703087477284,0.3313313400136283,0.34211972249751577,0.35333196760643976,0.3648740065718061,0.37685163353525747,0.38915313067514035,0.40189363835688724,0.41507930175125446,0.42856615082302013,0.44264258276722496,0.45716883271791925,0.47196112460258743,0.4875508746877692,0.5033868754896456,0.5198498424867913,0.5369605345738402,0.5544909992535765,0.572675230637874,0.5912455290085823,0.6107595440054173,0.630641848591555,0.6511774550057758,0.6727302627948981,0.6945892853394943,0.71750108468229,0.740646280962364,0.7648623530727928,0.7902115196500938,0.8156854436388139,0.842856474732153,0.8700644732147348,0.8984361408195632,0.9280253550381714,0.9581467344134444,0.9894987025786092,1.0221285031582827,1.055184573898721,1.090450197300789,1.1261052446239757,1.1630786813611604,1.2002341474646634,1.2398418743309971,1.280431554612204,1.3222159265553985,1.3656150174369392,1.4101932146621896,1.456410048550449,1.5039324045742324,1.5531026861217552,1.6039351543738642,1.6564353698476915,1.7105986124875787,1.766659838032199,1.8243700328590307,1.8839718497659885,1.9457656533757017,2.0094681917844364,2.075049162060246,2.1428307541151006,2.2132129138361245,2.285422809826723,2.360254853095369,2.437275160862978,2.5169343774482282,2.5992492124339566,2.684802672869201,2.7724549962678826,2.8633761531893698,2.956932683832571,3.0537977200270863,3.153209242957775,3.2567425120330897,3.362739013645232,3.472946426697471,3.586467672348849,3.7043378378577745,3.8254917443103893,3.9497988987926,4.079769247551817,4.212850405473996,4.350322366073675,4.493808895726702,4.640126775190858,4.792585521186692,4.949468560203582,5.110642515791414,5.278168898812249,5.449854392663723,5.627970378261448,5.8126670151476665,6.004076873273594,6.199209371654986};
  double refractiveIndex[nCherenkovMomenta] = {1.6240317502663,1.6332058556533,1.6417334442682,1.6495581507485,1.6568352356965,1.663555792699,1.6698063350772,1.6756189316007,1.6809814756374,1.6860063236089,1.6906368746825,1.6949749939614,1.6990024419669,1.7027737381029,1.7062709711188,1.709543722848,1.7126051747702,1.715438147562,1.7181145792243,1.7206148427182,1.7229226773359,1.7251280028472,1.7271600353641,1.7290784375914,1.730889247653,1.7325755211079,1.7341669205685,1.7356472403196,1.7370650289788,1.7383827319917,1.7396263439883,1.7408195373476,1.7419273828406,1.742992131424,1.7439802356172,1.7449319592359,1.7458498390026,1.7467019118788,1.7475432140301,1.7483249348844,1.7490839326181,1.7498225628591,1.7505264589423,1.7512147150232,1.751889641765,1.7525364667588,1.7531915603611,1.7538227008788,1.7544492215126,1.7550547342523,1.7556780777757,1.7562973768044,1.7569182191283,1.7575488420438,1.7581849835841,1.7588353492177,1.7594974153468,1.7601781977445,1.7608801757554,1.7616058134504,1.7623575309256,1.7631412029905,1.7639560666816,1.7648083776856,1.7657055418929,1.7666467187031,1.7676347594555,1.768678092594,1.7697870034397,1.7709534510436,1.7721946418157,1.7735082113872,1.7749069668851,1.7763969806669,1.7779954669163,1.7796876970942,1.7815032886078,1.7834376078764,1.7855131978094,1.7877228296252,1.7901123299044,1.792654904549,1.7954050494599,1.7983552384705,1.8015488081449,1.8049747310816,1.8086465749755,1.812662211763,1.8169687643807,1.8216335887885,1.8267479542496,1.8322345043326,1.8382585951889,1.8448039892896,1.8519174110987,1.859757418441,1.8682985747984,1.8777446762354,1.8882235599463,1.8998877983536,1.9127094080587};
  TGraph *gRefractiveIndex = new TGraph();
  gRefractiveIndex->SetTitle(";photon energy [eV]; sapphire refractive index");
  for (int i=0; i<nCherenkovMomenta; i++) gRefractiveIndex->SetPoint(i, pCherenkov[i], refractiveIndex[i]);
  TCanvas *cRefractiveIndex = new TCanvas("cRefractiveIndex", "", 700, 600);
  gRefractiveIndex->Draw("AC");
  cRefractiveIndex->SaveAs("out/refractiveIndex.png");
  cRefractiveIndex->SaveAs("out/refractiveIndex.eps");

  // read all trees from simulation
  TFile *rootFileIn = new TFile(rootFilePath.c_str());
  TTree *runTree = (TTree *)rootFileIn->Get("runTree");

  runTree->Print();
  int nHits = runTree->GetEntries();
    
  double hitEnergy, hitTime, hitPositionX, hitPositionY, hitPositionZ, hitParentID, hitEventID;
  
  TBranch *hitEnergyBranch = runTree->GetBranch("hitEnergy");
  TBranch *hitTimeBranch = runTree->GetBranch("hitTime");
  TBranch *hitPositionXBranch = runTree->GetBranch("hitPositionX");
  TBranch *hitPositionYBranch = runTree->GetBranch("hitPositionY");
  TBranch *hitPositionZBranch = runTree->GetBranch("hitPositionZ");
  TBranch *hitParentIDBranch = runTree->GetBranch("hitParentID");
  TBranch *hitEventIDBranch = runTree->GetBranch("hitEventID");
  
  hitEnergyBranch->SetAddress(&hitEnergy);
  hitTimeBranch->SetAddress(&hitTime);
  hitParentIDBranch->SetAddress(&hitParentID);
  hitEventIDBranch->SetAddress(&hitEventID);
  hitPositionXBranch->SetAddress(&hitPositionX);
  hitPositionYBranch->SetAddress(&hitPositionY);
  hitPositionZBranch->SetAddress(&hitPositionZ);

  double mcpTts = 35.; // ps TTS
  TH1F *hEnergy = new TH1F("hEnergy", ";photon energy [eV];", 50, 0., 7.);
  TH1F *hEnergyQe = new TH1F("hEnergyQe", ";photon energy [eV];", 50, 0., 7.);
  TH1F *hTime = new TH1F("hTime", ";photon arrival time [ps];", 100, 0., 8*mcpTts);
  TH1F *hTimePrimary = new TH1F("hTimePrimary", ";photon arrival time [ps];", 100, 0., 8*mcpTts);
  TH1F *hNphotonsAll = new TH1F("hNphotonsAll", ";number of photons per event;", 50, 50., 200);
  TH1F *hNphotonsPrimary = new TH1F("hNphotonsPrimary", ";number of photons per event;", 50, 50., 200);
  TH2F *hPosition = new TH2F("hPosition", ";hit position x [mm];hit position y [mm];", 60, -9., 9., 60, -9., 9.);
  TH2F *hPositionTime = new TH2F("hPositionTime", ";hit position x [mm];hit position y [mm];arrival time [ps]", 60, -9., 9., 60, -9., 9.);
  TH2F *hRadiusTime = new TH2F("hRadiusTime", ";r [mm];arrival time [ps];", 100, 0.005, 10., 50, 0.0, 8*mcpTts);
  TH2F *hRadiusTimePrimary = new TH2F("hRadiusTimePrimary", ";r [mm];arrival time [ps];", 100, 0.005, 10., 50, 0.0, 8*mcpTts);
  TH1I *hHitPerChannel = new TH1I("hHitPerChannel", ";channel;hits", 64, 0, 64);
  TH2F *hHitPerChannelTime = new TH2F("hHitPerChannelTime", ";channel;time [ps];hits", 64, 0, 64, 50, 0.0, 8*mcpTts);
  TH1F *hTimeEvent = new TH1F("hTimeEvent", ";signal time [ps];", 100, -mcpTts, 4*mcpTts);

  cout << endl;

  map<int, int> nPhotonPerEventAll, nPhotonPerEventPrimary;
  map<int, double> minTimePhotonPerEventAll, minTimePhotonPerEventPrimary;
  int hitChannelX, hitChannelY, hitChannel;
  int nMcpChannels = 8;
  double sizeMcpPmt = 53., sizeMcpChannel = sizeMcpPmt/nMcpChannels;

  TRandom *ttsRandomGenerator = new TRandom();

  //if (subsample) nHits = 100000; // subsample size
  TRandom *qeRandomGenerator = new TRandom();
  double hitQe, randomQe;
  for (int j=0; j<nHits; j++) {
    cout << "\rprocessing event " << j << " of " << nHits << ", " << (int)(100.0*j/nHits) << "\% completed...";

    runTree->GetEvent(j);
    hitTime *= 1.e3; // time in ps
    hEnergy->Fill(hitEnergy);

    // apply quantum efficiency effect
    randomQe = qeRandomGenerator->Uniform(0, maxQe);
    hitQe = hQuantumEfficiency->GetBinContent(hQuantumEfficiency->FindBin(hitEnergy));
    if (randomQe>hitQe) continue;
    //if (hitEnergy<2. || hitEnergy>6.) continue;

    if (withTts) // add random time from gaussian with sigma=TTS
      hitTime += ttsRandomGenerator->Gaus(3*mcpTts, mcpTts);

    hEnergyQe->Fill(hitEnergy);
    hTime->Fill(hitTime);
    hPosition->Fill(hitPositionX, hitPositionY);
    hRadiusTime->Fill(pow(pow(hitPositionX,2)+pow(hitPositionY,2), 0.5), hitTime);

    if (nPhotonPerEventAll.count(hitEventID)==0) nPhotonPerEventAll[hitEventID]=0;
    else nPhotonPerEventAll[hitEventID]++;

    // store first photon arrival time for each event
    if (minTimePhotonPerEventAll.count(hitEventID)==0 ||
      minTimePhotonPerEventAll[hitEventID]>hitTime)
      minTimePhotonPerEventAll[hitEventID]=hitTime;

    hitChannelX = (int)((hitPositionX+0.5*sizeMcpPmt)/sizeMcpChannel);
    hitChannelY = nMcpChannels-(int)((hitPositionY+0.5*sizeMcpPmt)/sizeMcpChannel);
    hitChannel = hitChannelX + nMcpChannels*hitChannelY;
    hHitPerChannel->Fill(hitChannel);
    hHitPerChannelTime->Fill(hitChannel, hitTime);

    if (hitParentID==1) {
      // plots only for photons from primary particle
      hTimePrimary->Fill(hitTime);
      hRadiusTimePrimary->Fill(pow(pow(hitPositionX,2)+pow(hitPositionY,2), 0.5), hitTime);
      hPositionTime->SetBinContent(hPositionTime->FindBin(hitPositionX, hitPositionY), hitTime);

      if (nPhotonPerEventPrimary.count(hitEventID)==0) nPhotonPerEventPrimary[hitEventID]=0;
      else nPhotonPerEventPrimary[hitEventID]++;

      if (minTimePhotonPerEventPrimary.count(hitEventID)==0 ||
        minTimePhotonPerEventPrimary[hitEventID]>hitTime)
        minTimePhotonPerEventPrimary[hitEventID]=hitTime;
    }
  }
  cout << endl;
  for (auto p:nPhotonPerEventAll) hNphotonsAll->Fill(p.second);
  for (auto p:nPhotonPerEventPrimary) hNphotonsPrimary->Fill(p.second);
  for (auto p:minTimePhotonPerEventAll) hTimeEvent->Fill(p.second);
  //for (int i=0; i<hEnergy->GetNbinsX(); i++) hEnergyQe->SetBinContent(i, hEnergy->GetBinContent(i)*hQuantumEfficiency->GetBinContent(i)/100.);

  TCanvas *cHitEnergy = new TCanvas("cHitEnergy", "", 700, 600);
  hEnergy->Draw();
  cHitEnergy->SaveAs("out/energy.png");
  cHitEnergy->SaveAs("out/energy.eps");

  TCanvas *cHitEnergyQe = new TCanvas("cHitEnergyQe", "", 700, 600);
  hEnergyQe->Draw();
  cHitEnergyQe->SaveAs("out/energyQe.png");
  cHitEnergyQe->SaveAs("out/energyQe.eps");

  TCanvas *cHitTime = new TCanvas("cHitTime", "", 700, 600);
  hTime->Draw();
  hTimePrimary->Draw("same");
  hTimePrimary->SetLineColor(kViolet+2);
  cHitTime->SaveAs("out/time.png");
  cHitTime->SaveAs("out/time.eps");

  TCanvas *cHitTimeEvent = new TCanvas("cHitTimeEvent", "", 700, 600);
  hTimeEvent->Draw();
  cHitTimeEvent->SaveAs("out/timeEvent.png");
  cHitTimeEvent->SaveAs("out/timeEvent.eps");

  TCanvas *cHitPosition = new TCanvas("cHitPosition", "", 800, 600);
  hPosition->SetStats(false);
  hPosition->Draw("COLZ");
  hPosition->GetZaxis()->SetTitleOffset(1.5);
  cHitPosition->SetRightMargin(.2);
  cHitPosition->SetLeftMargin(1.5);
  cHitPosition->SaveAs("out/position.png");
  cHitPosition->SaveAs("out/position.eps");

  TCanvas *cHitPositionTime = new TCanvas("cHitPositionTime", "", 800, 600);
  hPositionTime->SetStats(false);
  hPositionTime->Draw("COLZ");
  hPositionTime->GetZaxis()->SetTitleOffset(1.5);
  cHitPositionTime->SetRightMargin(.2);
  cHitPositionTime->SetLeftMargin(1.5);
  cHitPositionTime->SaveAs("out/positionTime.png");
  cHitPositionTime->SaveAs("out/positionTime.eps");

  TCanvas *cHitRadiusTime = new TCanvas("cHitRadiusTime", "", 800, 600);
  hRadiusTime->SetStats(false);
  hRadiusTime->Draw("COLZ");
  hRadiusTime->GetZaxis()->SetTitleOffset(1.5);
  cHitRadiusTime->SetRightMargin(.2);
  cHitRadiusTime->SetLeftMargin(1.5);
  cHitRadiusTime->SaveAs("out/radiusTime.png");
  cHitRadiusTime->SaveAs("out/radiusTime.eps");

  TCanvas *cHitRadiusTimePrimary = new TCanvas("cHitRadiusTimePrimary", "", 800, 600);
  hRadiusTimePrimary->SetStats(false);
  hRadiusTimePrimary->Draw("COLZ");
  hRadiusTimePrimary->GetZaxis()->SetTitleOffset(1.5);
  cHitRadiusTimePrimary->SetRightMargin(.2);
  cHitRadiusTimePrimary->SetLeftMargin(1.5);
  cHitRadiusTimePrimary->SaveAs("out/radiusTimePrimary.png");
  cHitRadiusTimePrimary->SaveAs("out/radiusTimePrimary.eps");

  TCanvas *cNphotons = new TCanvas("cNphotons", "", 700, 600);
  hNphotonsPrimary->Draw();
  hNphotonsAll->Draw("same");
  hNphotonsPrimary->SetLineColor(kViolet+2);
  cNphotons->SaveAs("out/nphotons.png");
  cNphotons->SaveAs("out/nphotons.eps");

  TCanvas *cHitPerChannel = new TCanvas("cHitPerChannel", "", 700, 600);
  cHitPerChannel->SetLogy();
  hHitPerChannel->SetStats(false);
  hHitPerChannel->Draw();
  cHitPerChannel->SaveAs("out/hitChannel.png");
  cHitPerChannel->SaveAs("out/hitChannel.eps");

  TCanvas *cHitPerChannelTime = new TCanvas("cHitPerChannelTime", "", 700, 600);
  hHitPerChannelTime->SetStats(false);
  hHitPerChannelTime->Draw("COLZ");
  hHitPerChannelTime->GetZaxis()->SetTitleOffset(1.5);
  cHitPerChannelTime->SetRightMargin(.15);
  cHitPerChannelTime->SetLeftMargin(.2);
  cHitPerChannelTime->SaveAs("out/hitChannelTime.png");
  cHitPerChannelTime->SaveAs("out/hitChannelTime.eps");
}
