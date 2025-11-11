{

  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetOptFit(0);
  gStyle->SetLabelFont(42);
  gStyle->SetTitleFont(42);

  gStyle->SetPadLeftMargin(0.16);
  gStyle->SetPadRightMargin(0.03);
  gStyle->SetPadTopMargin(0.04);//!!
  gStyle->SetPadBottomMargin(0.12);//!!
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  //gPad->SetLogy();
  TGaxis::SetMaxDigits(3);

  struct DataPointTemplate {
    float Concentrtion;
    string Units;
    float Dose;
    float SurvivedFraction;
    float Uncertainty;
  };


  vector<double> Concentration;
  vector<DataPointTemplate> vDataPoint;



  std::ifstream inputFile("/home/maxim/Programs/MedPhysProjects/FNTP-2/Init/FNTP-2/Data/LaB6_EMT6P_Editted.txt");


    if (!inputFile.is_open()) {
      cerr << "Error: Could not open the file with neutron energy & angle distribution!" << endl;
    }


    string line;
    bool fDataClonAssay =false;
    while (getline(inputFile, line)) {
        //cout << line << endl;

        stringstream ss_(line);
        string item_;
        vector<string> Row_;
        while (ss_ >> item_) {
            //double itemf_  = stod(item_);
          // cout<<"imtes:  "<<item_<<endl;.
               Row_.push_back(item_);

        }


        if(fDataClonAssay){
        cout << line << endl;
        DataPointTemplate DataPoint_;
        cout<<"item size:  " <<Row_.size()<<"     items: "<<Row_[0]<<"  "<< Row_[1]<<"  " <<Row_[2]<<"  " <<Row_[3]<<"  " <<Row_[4]<<endl;
        DataPoint_.Concentrtion = stof(Row_[0]);
        DataPoint_.Units = Row_[1];
        DataPoint_.Dose = stof(Row_[2]);
        DataPoint_.SurvivedFraction = stof(Row_[3]);
        DataPoint_.Uncertainty = stof(Row_[4]);

         vDataPoint.push_back(DataPoint_);
        }

        if(line[1] == 'G') fDataClonAssay = true; //cout << line << endl;

    }
      inputFile.close();



// plotting snippet



}
