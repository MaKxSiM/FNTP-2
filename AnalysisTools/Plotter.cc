{
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetOptFit(0);
  gStyle->SetLabelFont(42);
  gStyle->SetTitleFont(42);

  //gStyle->SetPadLeftMargin(0.16);
  //gStyle->SetPadRightMargin(0.03);
  gStyle->SetPadTopMargin(0.04);//!!
  //gStyle->SetPadBottomMargin(0.12);//!!
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  //gPad->SetLogy();
  TGaxis::SetMaxDigits(3);

  struct DataPointTemplate {
    float Concentration;
    string Units;
    float Dose;
    float SurvivedFraction;
    float Uncertainty;
  };

  struct NanoParicleInfoTemplate {
    string Material;
    float Size;
    string Coating;
    string Vector;
  };

  struct FiledInfoTemplate {
    string Type;
    string Spectrum;
  };

  vector<double> Concentration;
  vector<DataPointTemplate> vDataPoint;
  int NbinsX, NbinsY;
  float Dose_min, Dose_max, C_min, C_max;
  string CellLine;
  NanoParicleInfoTemplate NanoParicleInfo;
  FiledInfoTemplate  RadiationFieldInfo;

  //== transient variables to ses assess histrgem limits
   vector<float> vDoses;
   vector<float> vConcentrations;

  //std::ifstream inputFile("/home/maxim/Programs/MedPhysProjects/FNTP-2/Init/FNTP-2/Data/LaB6_EMT6P_Editted.txt");
  //std::ifstream inputFile("/home/maxim/Programs/MedPhysProjects/FNTP-2/Iteration2/FNTP-2/Data/Au-DARPin_BT-474.txt");
  //std::ifstream inputFile("/home/maxim/Programs/MedPhysProjects/FNTP-2/Iteration2/FNTP-2/Data/Au_SK-OV-3.txt");
  //std::ifstream inputFile("/home/maxim/Programs/MedPhysProjects/FNTP-2/Iteration2/FNTP-2/Data/HfN_BT-474.txt");
  std::ifstream inputFile("/home/maxim/Programs/MedPhysProjects/FNTP-2/Iteration2/FNTP-2/Data/Au_EMT6P.txt");
  //std::ifstream inputFile("/home/maxim/Programs/MedPhysProjects/FNTP-2/Iteration2/FNTP-2/Data/Bi_EMT6P.txt");
  
    if (!inputFile.is_open()) {
      cerr << "Error: Could not open the file!" << endl;
    }

    string line;
    bool fDataClonAssay =false;
    bool fCellLine=false;
    bool fNanoParticle=false;
    bool fRadiationField=false;

    while (getline(inputFile, line)) {
        stringstream ss_(line);
        string item_;
        vector<string> Row_;
        while (ss_ >> item_) {
               Row_.push_back(item_);
        }

        if(fDataClonAssay){
            DataPointTemplate DataPoint_;
          //  cout<<"item size:  " <<Row_.size()<<"     items: "<<Row_[0]<<"  "<< Row_[1]<<"  " <<Row_[2]<<"  " <<Row_[3]<<"  " <<Row_[4]<<endl;
            DataPoint_.Concentration = stof(Row_[0]);
            DataPoint_.Units = Row_[1];
            DataPoint_.Dose = stof(Row_[2]);
            DataPoint_.SurvivedFraction = stof(Row_[3]);
            DataPoint_.Uncertainty = stof(Row_[4])+2.0; //+2% should give some regularizationo for the fitting procedure

            vDataPoint.push_back(DataPoint_);
            vDoses.push_back(DataPoint_.Dose);
            vConcentrations.push_back(DataPoint_.Concentration);
        }
        if(line[1] == 'G') fDataClonAssay = true; //cout << line << endl;

//======= reading  cell line info ==============================================
        if(fCellLine){
          CellLine = Row_[0];
          fCellLine = false;
        }
        if(line[1] == 'B') fCellLine = true; //cout << line << endl;
//------------------------------------------------------------------------------

//======= reading  nanoparticle info ===========================================
        if(fNanoParticle){
          //cout<<"!!!!!!!!!!!!!!!!:  "<<Row_[0]<<endl;
          NanoParicleInfo.Material = Row_[0];
          NanoParicleInfo.Size = stof(Row_[1]);
          NanoParicleInfo.Coating  = Row_[2];
          NanoParicleInfo.Vector  = Row_[3];
          fNanoParticle = false;
        }
        if(line[1] == 'C') fNanoParticle = true; //cout << line << endl;
//------------------------------------------------------------------------------

//======= reading  rations field info  =========================================
        if(fRadiationField){
          RadiationFieldInfo.Type = Row_[0];
          RadiationFieldInfo.Spectrum  = Row_[1];
          fRadiationField = false;
        }
        if(line[1] == 'D')  fRadiationField = true; //cout << line << endl;
//------------------------------------------------------------------------------


    }
      inputFile.close();

// plotting snippet

std::sort(vDoses.begin(), vDoses.end());
std::sort(vConcentrations.begin(), vConcentrations.end());

auto last = std::unique(vDoses.begin(), vDoses.end());
vDoses.erase(last, vDoses.end());

auto last2 = std::unique(vConcentrations.begin(), vConcentrations.end());
vConcentrations.erase(last2, vConcentrations.end());

cout<< " N doses:  "<< vDoses.size()  << "   "  << vDoses[0]<<"  "<<vDoses[1]<<"  "<<vDoses[2]<< endl;
cout<< " N concentrations:  "<< vConcentrations.size()  << "   "  << vConcentrations[0]<<"  "<<vConcentrations[1]<<"  "<<vConcentrations[2]<< endl;

NbinsX = vDoses.size();
NbinsY = vConcentrations.size();

Dose_min = vDoses[0];
Dose_max = vDoses[NbinsX-1];
C_min = vConcentrations[0];
C_max = vConcentrations[NbinsY-1];


TH2Poly *hp = new TH2Poly();
for(int i = 0; i < NbinsX; i++ ){
  for(int j = 0; j < NbinsY; j++ ){
      hp->AddBin(vDoses[i]-0.8,vConcentrations[j]-4.5, vDoses[i]+0.8,vConcentrations[j]+4.5);
    //if(j==0)hp->AddBin(vDoses[i]-0.5,vConcentrations[j]-1.0, vDoses[i]+0.4,vConcentrations[j]+1.0);//for bismuth
    //if(j==1)hp->AddBin(vDoses[i]-0.5,vConcentrations[j]-0.8, vDoses[i]+0.4,vConcentrations[j]+0.8);//for bismuth
    //if(j>1)hp->AddBin(vDoses[i]-0.5,vConcentrations[j]-4.0, vDoses[i]+0.4,vConcentrations[j]+4.0);//for bismuth
  }
}

cout<< "vDataPoint size:  "<< vDataPoint.size()<<endl;
for(int j = 0; j < vDataPoint.size(); j++ ){
    cout << vDataPoint[j].Dose <<"   " << vDataPoint[j].Concentration <<"   "<<vDataPoint[j].SurvivedFraction <<endl;
    hp->Fill(vDataPoint[j].Dose, vDataPoint[j].Concentration, vDataPoint[j].SurvivedFraction );
}

TCanvas* c1 = new TCanvas("c1", "c1", 960, 720);
c1->GetPad(0)->SetRightMargin(0.12);
c1->GetPad(0)->SetLeftMargin(0.12);
c1->GetPad(0)->SetBottomMargin(0.12);
  hp->GetXaxis()->CenterTitle();
  hp->GetYaxis()->CenterTitle();
  hp->GetXaxis()->SetTitleSize(0.05);
  hp->GetYaxis()->SetTitleSize(0.05);
  hp->GetXaxis()->SetLabelSize(0.05);
  hp->GetYaxis()->SetLabelSize(0.05);
  hp->GetZaxis()->SetLabelSize(0.05);
  hp->GetXaxis()->SetTitle("Dose (Gy)");
  hp->GetYaxis()->SetTitle("Concentration (mg/L)");
  hp->Draw("colz");


// 1D slice at fixed concentration with  LQ fit
TCanvas* c2 = new TCanvas("c2", "c2", 960, 720);
c2->GetPad(0)->SetRightMargin(0.04);


int CS = 5; //Concentrtion  sclice, numbering sterts from 1. Dev: It should be taken from the file with paprameters in future devs.

// ======  filling vectors for 1D TGraph =======================================
vector<float> vDose_err; // vetor with error of doses
vector<float> SF;
vector<float> SF_err;
for(int i=0; i<NbinsX; i++){
  vDose_err.push_back(0);
  SF.push_back(vDataPoint[(CS-1)*NbinsX+i].SurvivedFraction);
  SF_err.push_back(vDataPoint[(CS-1)*NbinsX+i].Uncertainty);
}
//------------------------------------------------------------------------------


// ==== LQ fit template ========================================================
TF1 *LQ_func = new TF1("LQ_Fit", "[0]*exp(-([1]*x+[2]*x^2))",Dose_min, Dose_max);
LQ_func->SetParameter(0, 100);
LQ_func->SetParameter(1, 0.3);
LQ_func->SetParameter(1, 0.03);
LQ_func->SetLineWidth(4);

LQ_func->SetParLimits(1, 0.01, 5.0);
LQ_func->SetParLimits(2, 0.001, 5.0);

//------------------------------------------------------------------------------

 TGraphErrors* gr =  new TGraphErrors(NbinsX, &vDoses[0], &SF[0], &vDose_err[0], &SF_err[0]);
 gr->GetYaxis()->SetTickLength(0.02);
 gr->GetYaxis()->SetNdivisions(505);
 gr->GetXaxis()->CenterTitle();
 gr->GetYaxis()->CenterTitle();
 gr->GetXaxis()->SetTitle("Dose (Gy)");
 gr->GetYaxis()->SetTitle("Cell Viability (%)");
 gr->GetYaxis()->SetRangeUser(0, 160);
 gr->GetXaxis()->SetRangeUser(Dose_min - 0.5, Dose_max + 0.5);
 gr->GetYaxis()->SetTitleSize(0.045);
 gr->GetYaxis()->SetTitleOffset(1.0);
 gr->GetXaxis()->SetTitleSize(0.045);
 gr->GetXaxis()->SetTitleOffset(1.0);
 gr->GetYaxis()->SetLabelSize(0.045);
 gr->GetXaxis()->SetLabelSize(0.045);
 gr->SetMarkerStyle(20);
 gr->SetMarkerSize(3);
 gr->SetLineWidth(3);
 gr->Draw("AP");

 gr->Fit(LQ_func);


// === forcing  alpha to be greater than beta for the ill fits of  the ill data  ==
 if(LQ_func->GetParameter(1)< LQ_func->GetParameter(2)){
   LQ_func->SetParLimits(1, LQ_func->GetParameter(2), 5.0);
   gr->Fit(LQ_func);
 }
//------------------------------------------------------------------------------


 double  LQ_alpha, LQ_beta;
 LQ_alpha = LQ_func->GetParameter(1);
 LQ_beta = LQ_func->GetParameter(2);

char LQ_legend[50];
char ConcentrationLegend[50];
char CellLineLegend[50];
char NanoParticleLegend[50];
char RadiatioFieldLegend[50];

sprintf(LQ_legend, " LQ fit: #alpha = %4.3f, #beta = %4.3f", LQ_alpha, LQ_beta);
sprintf(ConcentrationLegend, "Concetration: %4.1f mg/L", vConcentrations[CS-1]);
sprintf(CellLineLegend, "Cell line: %s", CellLine.c_str());
sprintf(NanoParticleLegend, "Nanoparticle: %s+%s, D = %3.1f nm", NanoParicleInfo.Material.c_str(), NanoParicleInfo.Coating.c_str(), NanoParicleInfo.Size);
sprintf(RadiatioFieldLegend, "Radiation type: %s, %s", RadiationFieldInfo.Type.c_str(), RadiationFieldInfo.Spectrum.c_str());

TLegend* legend1 = new TLegend(0.48, 0.59, 0.80, 0.70);//
legend1->SetTextSize(0.032);
legend1->SetTextFont(42);
//legend1->SetHeader(ConcentrationLegend);
legend1->AddEntry(gr, "Data", "p");
legend1->AddEntry(LQ_func, LQ_legend, "l");
legend1->SetFillColor(kWhite);
legend1->SetLineColor(kWhite);
legend1->Draw();

TLegend* legend2 = new TLegend(0.45, 0.87, 0.80, 0.92);//
legend2->SetTextSize(0.032);
legend2->SetTextFont(42);
legend2->SetHeader(CellLineLegend);
legend2->SetFillColor(kWhite);
legend2->SetLineColor(kWhite);
legend2->Draw();

TLegend* legend3 = new TLegend(0.45, 0.82, 0.80, 0.87);//
legend3->SetTextSize(0.032);
legend3->SetTextFont(42);
legend3->SetHeader(NanoParticleLegend);
legend3->SetFillColor(kWhite);
legend3->SetLineColor(kWhite);
legend3->Draw();

TLegend* legend4 = new TLegend(0.45, 0.77, 0.80, 0.82);//
legend4->SetTextSize(0.032);
legend4->SetTextFont(42);
legend4->SetHeader(ConcentrationLegend);
legend4->SetFillColor(kWhite);
legend4->SetLineColor(kWhite);
legend4->Draw();

TLegend* legend5 = new TLegend(0.45, 0.72, 0.80, 0.77);//
legend5->SetTextSize(0.032);
legend5->SetTextFont(42);
legend5->SetHeader(RadiatioFieldLegend);
legend5->SetFillColor(kWhite);
legend5->SetLineColor(kWhite);
legend5->Draw();



}
