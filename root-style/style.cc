#include <TROOT.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TFile.h>
#include <TLegend.h>
#include <TStyle.h>

#include "style.hh"

TStyle *setStyle() {
  TStyle *m_garfieldStyle = new TStyle("Garfield", "Garfield Style");

  m_garfieldStyle->Reset();
  m_garfieldStyle->SetFillColor(1);
  m_garfieldStyle->SetFillStyle(1001);
  m_garfieldStyle->SetCanvasBorderMode(0);
  m_garfieldStyle->SetCanvasColor(0);
  m_garfieldStyle->SetCanvasDefH(600);
  m_garfieldStyle->SetCanvasDefW(600);
  m_garfieldStyle->SetPadBorderMode(0);
  m_garfieldStyle->SetPadColor(0);
  m_garfieldStyle->SetPadLeftMargin(0.15);
  m_garfieldStyle->SetPadBottomMargin(0.1);
  m_garfieldStyle->SetPadRightMargin(0.05);
  m_garfieldStyle->SetPadTopMargin(0.05);
  m_garfieldStyle->SetPadTickX(1);
  m_garfieldStyle->SetPadTickY(1);
  m_garfieldStyle->SetFrameFillColor(0);
  m_garfieldStyle->SetFrameBorderMode(0);
  m_garfieldStyle->SetDrawBorder(0);
  m_garfieldStyle->SetLegendBorderSize(0);

  m_garfieldStyle->SetGridColor(kGray);
  m_garfieldStyle->SetGridStyle(3);
  m_garfieldStyle->SetGridWidth(1);
  m_garfieldStyle->SetPadGridX(kTRUE);
  m_garfieldStyle->SetPadGridY(kTRUE);

  const int font = 42;
  const double tsize = 0.035;
  m_garfieldStyle->SetTextFont(font);
  m_garfieldStyle->SetTextSize(tsize);
  m_garfieldStyle->SetTitleStyle(0);
  m_garfieldStyle->SetTitleBorderSize(0);
  m_garfieldStyle->SetTitleColor(923, "xyz");
  m_garfieldStyle->SetTitleColor(1, "t");
  m_garfieldStyle->SetTitleFillColor(0);
  m_garfieldStyle->SetTitleFont(font, "xyz");
  m_garfieldStyle->SetTitleFont(font, "t");
  m_garfieldStyle->SetTitleOffset(1.2, "x");
  m_garfieldStyle->SetTitleOffset(0, "y");
  m_garfieldStyle->SetTitleSize(tsize, "xyz");
  m_garfieldStyle->SetTitleSize(tsize, "t");

  m_garfieldStyle->SetLegendFont(font);
  m_garfieldStyle->SetStatStyle(0);
  m_garfieldStyle->SetStatBorderSize(0);
  m_garfieldStyle->SetStatColor(0);
  m_garfieldStyle->SetStatFont(font);
  m_garfieldStyle->SetStatFontSize(tsize);
  m_garfieldStyle->SetStatX(0.88);
  m_garfieldStyle->SetStatY(0.88);
  m_garfieldStyle->SetStatW(0.25);
  m_garfieldStyle->SetStatH(0.1);
  m_garfieldStyle->SetOptStat(111110);
  m_garfieldStyle->SetStatFormat("6.3g");
  m_garfieldStyle->SetLabelFont(font, "xyz");
  m_garfieldStyle->SetLabelColor(923, "xyz");
  m_garfieldStyle->SetLabelSize(tsize, "xyz");
  m_garfieldStyle->SetLabelOffset(0.01, "xyz");
  m_garfieldStyle->SetOptTitle(0);
  m_garfieldStyle->SetPaperSize(TStyle::kA4);
  m_garfieldStyle->SetFuncWidth(2);
  m_garfieldStyle->SetHistLineColor(kViolet - 2);
  m_garfieldStyle->SetPalette(1);
  m_garfieldStyle->SetAxisColor(kBlack, "X");
  m_garfieldStyle->SetAxisColor(kBlack, "Y");
  m_garfieldStyle->SetAxisColor(kBlack, "Z");
  m_garfieldStyle->SetNdivisions(505, "x");
  m_garfieldStyle->SetNdivisions(510, "y");
  m_garfieldStyle->SetMarkerStyle(20);
  m_garfieldStyle->SetMarkerSize(1.2);
  const double lw = 2;
  m_garfieldStyle->SetLineWidth(lw);
  m_garfieldStyle->SetLineStyleString(2, "[12 12]");
  m_garfieldStyle->SetFrameLineWidth(lw);
  m_garfieldStyle->SetHistLineWidth(lw);
  m_garfieldStyle->SetFuncWidth(lw);
  m_garfieldStyle->SetGridWidth(lw);
  m_garfieldStyle->cd();
  
  return m_garfieldStyle;
}
