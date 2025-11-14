{
/*
  #include <iostream>
  #include <vector>
  #include <algorithm> // Required for std::sort
  #include <set>
*/
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetOptFit(0);
  gStyle->SetLabelFont(42);
  gStyle->SetTitleFont(42);

  //gStyle->SetPadLeftMargin(0.16);
  //gStyle->SetPadRightMargin(0.03);
  //gStyle->SetPadTopMargin(0.04);//!!
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

  vector<double> Concentration;
  vector<DataPointTemplate> vDataPoint;
  int NbinsX, NbinsY;
  float Dose_min, Dose_max, C_min, C_max;
  //== transient variables to ses assess histrgem limits
   vector<float> vDoses;
   vector<float> vConcentrations;

  std::ifstream inputFile("/home/maxim/Programs/MedPhysProjects/FNTP-2/Init/FNTP-2/Data/LaB6_EMT6P_Editted.txt");
    if (!inputFile.is_open()) {
      cerr << "Error: Could not open the file!" << endl;
    }

    string line;
    bool fDataClonAssay =false;
    while (getline(inputFile, line)) {
        stringstream ss_(line);
        string item_;
        vector<string> Row_;
        while (ss_ >> item_) {
               Row_.push_back(item_);
        }

        if(fDataClonAssay){
            DataPointTemplate DataPoint_;
            cout<<"item size:  " <<Row_.size()<<"     items: "<<Row_[0]<<"  "<< Row_[1]<<"  " <<Row_[2]<<"  " <<Row_[3]<<"  " <<Row_[4]<<endl;
            DataPoint_.Concentration = stof(Row_[0]);
            DataPoint_.Units = Row_[1];
            DataPoint_.Dose = stof(Row_[2]);
            DataPoint_.SurvivedFraction = stof(Row_[3]);
            DataPoint_.Uncertainty = stof(Row_[4]);

            vDataPoint.push_back(DataPoint_);
            vDoses.push_back(DataPoint_.Dose);
            vConcentrations.push_back(DataPoint_.Concentration);
        }
        if(line[1] == 'G') fDataClonAssay = true; //cout << line << endl;

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
      hp->AddBin(vDoses[i]-0.9,vConcentrations[j]-4.9, vDoses[i]+0.9,vConcentrations[j]+4.9);
  }
}

cout<< "vDataPoint size:  "<< vDataPoint.size()<<endl;
for(int j = 0; j < vDataPoint.size(); j++ ){
    cout << vDataPoint[j].Dose <<"   " << vDataPoint[j].Concentration <<"   "<<vDataPoint[j].SurvivedFraction <<endl;
    hp->Fill(vDataPoint[j].Dose, vDataPoint[j].Concentration, vDataPoint[j].SurvivedFraction );
}

TCanvas* c1 = new TCanvas("c1", "c1", 960, 720);
  hp->GetXaxis()->CenterTitle();
  hp->GetYaxis()->CenterTitle();
  hp->GetXaxis()->SetTitle("Dose (Gy)");
  hp->GetYaxis()->SetTitle("Concentration (mg/L)");
  hp->Draw("colz");


// 1D slice at fixed concentration with  LQ fit
TCanvas* c2 = new TCanvas("c2", "c2", 960, 720);

int CS = 1; //Concentrtion  sclice, numbering sterts from 1. Dev: It should be taken from the file with paprameters in future devs.

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
//------------------------------------------------------------------------------

 TGraphErrors* gr =  new TGraphErrors(NbinsX, &vDoses[0], &SF[0], &vDose_err[0], &SF_err[0]);
 gr->GetYaxis()->SetTickLength(0.02);
 gr->GetYaxis()->SetNdivisions(505);
 gr->GetXaxis()->CenterTitle();
 gr->GetYaxis()->CenterTitle();
 gr->GetXaxis()->SetTitle("Dose (Gy)");
 gr->GetYaxis()->SetTitle("Cell viability (%)");
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

 double  LQ_alpha, LQ_beta;
 LQ_alpha = LQ_func->GetParameter(1);
 LQ_beta = LQ_func->GetParameter(2);

char LQ_legend[50];
char Condtions[50];

sprintf(LQ_legend, " LQ fit: #alpha = %4.3f, #beta = %4.3f", LQ_alpha, LQ_beta);
sprintf(Condtions, " Concetration: %4.1f mg/L", vConcentrations[CS-1]);

 TLegend* legend1 = new TLegend(0.50, 0.60, 0.85, 0.80);//
 legend1->SetTextSize(0.032);
 legend1->SetTextFont(42);
 legend1->SetHeader(Condtions,"C");
 legend1->AddEntry(gr, "Data", "p");
 legend1->AddEntry(LQ_func, LQ_legend, "l");
 legend1->SetFillColor(kWhite);
 legend1->SetLineColor(kWhite);
 legend1->Draw();

}
