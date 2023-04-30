#include <iostream>

#include "CsvFeeder.h"
#include "Msg.h"
#include "VolSurfBuilder.h"
#include "CubicSmile.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

std::map<int,std::string> reverse_months = {
    { 1,"JAN" },
    { 2,"FEB" },
    { 3,"MAR"},
    { 4,"APR"},
    { 5,"MAY"},
    { 6,"JUN" },
    { 7,"JUL"},
    { 8,"AUG"},
    { 9,"SEP" },
    { 10,"OCT" },
    { 11,"NOV" },
    { 12,"DEC" }
};

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: "
                  << argv[0] << " tick_data.csv" << " outputFile.csv" << std::endl;
        return 1;
    }
    const char* ticker_filename = argv[1];
    const char* output_filename = argv[2];
    std::ofstream myfile;

    VolSurfBuilder<CubicSmile> volBuilder;
    auto feeder_listener = [&volBuilder] (const Msg& msg) {
        if (msg.isSet) {
            volBuilder.Process(msg);
        }
    };

    auto timer_listener = [&] (uint64_t now_ms) {
        // fit smile
        auto smiles = volBuilder.FitSmiles();
        // TODO: stream the smiles and their fitting error to outputFile.csv
        if (!myfile.is_open()){
            myfile.open(output_filename);
            myfile << "TIME" << "," << "EXPIRY" <<"," << "FUT_PRICE" <<"," << "ATM" <<"," << "BF25" <<","  << "RR25" <<","  << "BF10" <<","  << "RR10" << "," << "FITTING_ERROR"<< ", STRIKE_VOL"<<std::endl;
        }
        if (myfile.is_open()){
            for (auto itr = smiles.begin(); itr!= smiles.end(); itr++){
                auto strikeMarks = itr->second.first.strikeMarks;
                int counter=1;
                double sigma_10,sigma_25,atmvol,sigma_75,sigma_90,fwd=0.0;
                for(auto strikes = strikeMarks.begin(); strikes!=strikeMarks.end(); strikes++){
                    if(counter==1){
                        sigma_10 = (*strikes).second;
                        counter++;
                    } else if(counter==2){
                        sigma_25 = (*strikes).second;
                        counter++;
                    } else if(counter==3){
                        atmvol = (*strikes).second;
                        fwd = (*strikes).first;
                        counter++;
                    } else if(counter==4){
                        sigma_75 = (*strikes).second;
                        counter++;
                    } else if(counter==5){
                        sigma_90 = (*strikes).second;
                        counter++;
                    }
                }
                double bf25 = (sigma_75+sigma_25)/2 - atmvol;
                double rr25 = sigma_25-sigma_75;
                double bf10 = (sigma_90+sigma_10)/2 - atmvol;
                double rr10 = sigma_10-sigma_90;
                auto expiry = itr->first;
                myfile << datetime_t(now_ms) << "," << expiry <<"," <<fwd <<"," << atmvol <<"," << bf25<<","  << rr25<<","  << bf10<<","  << rr10 << "," << itr->second.second << "";
                if (fwd > 10000){
                    for (int i = 1,n=180; i!=n; i++){
                    myfile<<","<<i*500<<"::"<<itr->second.first.Vol(i*500);
                        if (i==n-1){
                            myfile<<endl;
                        }
                    }
                }
                else{
                    for (int i = 1,n=100; i!=n; i++){
                        myfile<<","<<i*50<<"::"<<itr->second.first.Vol(i*50);
                        if (i==n-1){
                            myfile<<endl;
                        }
                    }
                }
            }
        }
    };

    const auto interval = std::chrono::minutes(1);  // we call timer_listener at 1 minute interval
    CsvFeeder csv_feeder(ticker_filename,
                         feeder_listener,
                         interval,
                         timer_listener);
    while (csv_feeder.Step()) {
    }
    return 0;
}