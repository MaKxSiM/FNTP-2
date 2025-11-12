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
      cerr << "Error: Could not open the file with neutron energy & angle distribution!" << endl;
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

//TH2F* hClonogenicAssay2D = new TH2F("hClonogenicAssay2D","Clonogenic Assay 2D", NbinsX, Dose_min, Dose_max, NbinsY, C_min, C_max);

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

  //hp->Fill(0.5, 2.1, 5.1 );

  hp->GetXaxis()->CenterTitle();
  hp->GetYaxis()->CenterTitle();
  hp->GetXaxis()->SetTitle("Dose (Gy)");
  hp->GetYaxis()->SetTitle("Concentration (mg/L)");

hp->Draw("colz");


}
