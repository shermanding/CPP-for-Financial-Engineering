#ifndef QF633_CODE_VOLSURFBUILDER_H
#define QF633_CODE_VOLSURFBUILDER_H

#include <sstream>
#include <map>
#include "Msg.h"
#include "Date.h"
#include "CubicSmile.h"
#include <chrono>


template<class Smile>
class VolSurfBuilder {
public:
    void Process(const Msg& msg);  // process message
    void PrintInfo();
    std::map<std::string, std::pair<Smile, double> > FitSmiles();
private:
    std::map<std::string, int> volMonths = {
        { "JAN", 1 },
        { "FEB", 2 },
        { "MAR", 3 },
        { "APR", 4 },
        { "MAY", 5 },
        { "JUN", 6 },
        { "JUL", 7 },
        { "AUG", 8 },
        { "SEP", 9 },
        { "OCT", 10 },
        { "NOV", 11 },
        { "DEC", 12 }
    };

protected:
    // we want to keep the best level information for all instruments
    // here we use a map from contract name to BestLevelInfo, the key is contract name
    std::map<std::string, TickData> currentSurfaceRaw;
};

template <class Smile>
void VolSurfBuilder<Smile>::Process(const Msg& msg) {
    std::vector<TickData> updates = msg.Updates;
    if (msg.isSnap) {
        // discard currently maintained market snapshot, and construct a new copy based on the inPut Msg
        currentSurfaceRaw = std::map<std::string, TickData>();
        for ( auto itr = updates.begin(); itr != updates.end(); ++ itr ){
            TickData ticks = *itr;
            currentSurfaceRaw.insert({ticks.ContractName,ticks});
        }
    } else {
        // update the currently maintained market snapshot
        for ( auto itr = updates.begin(); itr != updates.end(); ++ itr ){
            TickData ticks = *itr;
            // checking to see if entry doesn't exist, then insert. else update.
            if (currentSurfaceRaw.count(ticks.ContractName)==1) {
                currentSurfaceRaw.at(ticks.ContractName) = ticks;
            } else if (currentSurfaceRaw.count(ticks.ContractName)==0 && currentSurfaceRaw.empty()){
                continue;
            }else if (currentSurfaceRaw.count(ticks.ContractName)==0){
                currentSurfaceRaw.insert({ticks.ContractName,ticks});
            }
        }
    }
}

template <class Smile>
void VolSurfBuilder<Smile>::PrintInfo() {
    // TODO: you may print out information about VolSurfBuilder's currentSnapshot to test
    // for (auto const& x : currentSurfaceRaw){
    //     std::cout << x.second.LastUpdateTimeStamp << std::endl;
    //     std::cout << x.first << ":" << x.second.ContractName << std::endl;
    // }
}

template <class Smile>
std::map<std::string, std::pair<Smile, double> > VolSurfBuilder<Smile>::FitSmiles() {
    std::map<std::string, std::vector<TickData>> tickersByExpiry;
    
    // TODO: group the tickers in the current market snapshot by expiry date, and construct tickersByExpiry
    for (auto mapIterator = currentSurfaceRaw.begin(); mapIterator!=currentSurfaceRaw.end();mapIterator++){
        std::vector<TickData> tempTickDataVector;
        std::string contractNameDate;
        std::stringstream line(mapIterator->first);
        std::string ss_dates;
        std::string y,m,d;
        int line_counter = 1;
        while (getline(line, ss_dates, '-')){
            line_counter ++;
            if (line_counter == 3){
                if (ss_dates.length()==7){
                    d = ss_dates.substr(0,2);
                    m = ss_dates.substr(2,3);
                    y = "20"+ss_dates.substr(5,2);
                }else{
                    d = ss_dates.substr(0,1);
                    m = ss_dates.substr(1,3);
                    y = "20"+ss_dates.substr(4,2);
                }
            }

        }
        // std::cout<<mapIterator->second.ContractName<<endl;
        contractNameDate = d+"-"+m+"-"+y;

        if (tickersByExpiry.count(contractNameDate) == 0){
            tempTickDataVector.push_back(mapIterator->second);
            tickersByExpiry.insert({contractNameDate,tempTickDataVector});
        } else {
            tempTickDataVector = tickersByExpiry.at(contractNameDate);
            tempTickDataVector.push_back(mapIterator->second);
            tickersByExpiry.erase(contractNameDate);
            tickersByExpiry.insert({contractNameDate, tempTickDataVector});
        }
    }

    std::map<std::string, std::pair<Smile, double> > res{};
    // then create Smile instance for each expiry by Puting FitSmile() of the Smile
    for (auto iter = tickersByExpiry.begin(); iter != tickersByExpiry.end(); iter++) {
        // TODO: you need to implement FitSmile function in CubicSmile
        auto sm = Smile::FitSmile(iter->second);
        double fittingError = 0;
        double fwd;
        // TODO: we need to measure the fitting error here
        map<double,double> PutStrikeVolMapper;
        auto volTickerSnap = iter->second;
        datetime_t latestTimeStamp = datetime_t(0);
        TickData LatestTick;
        for (auto itr = volTickerSnap.begin(); itr != volTickerSnap.end(); itr++){
            TickData sameExpiryTick = *itr;
            if (latestTimeStamp - sameExpiryTick.LastUpdateTimeStamp < 0){
                latestTimeStamp = sameExpiryTick.LastUpdateTimeStamp;
                LatestTick = *itr;
            }
        }

        fwd = LatestTick.UnderlyingPrice;

        for (auto itrMap = volTickerSnap.begin(); itrMap != volTickerSnap.end(); itrMap++){
            TickData sameExpiryTick = *itrMap;
            int counter = 0;
            std::stringstream eachLine(sameExpiryTick.ContractName);
            std::string ss_item;
            double strike, price, midiv, underlyingprice;
            while (getline(eachLine, ss_item, '-')){
                counter++;
                if (counter == 3) {
                strike = std::stod(ss_item);
                }}
                price = sameExpiryTick.MarkPrice;
                midiv = (sameExpiryTick.BestBidIV + sameExpiryTick.BestAskIV) / 2 / 100;
                auto tempPair2 = std::pair<double,double>(strike,midiv);
                auto tempPair3 = std::pair<double,double>(strike,fwd);
                if (counter == 4){
                    if (ss_item=="P" && price != 0) {
                        // add to PutStrikeVolmapper only if, midiv != 0 and Put options are OTM
                        if (tempPair2.second != 0) {
                            PutStrikeVolMapper.insert(tempPair2);
                        }         
                    }
                }
            }


        double SmileCalibration(CubicSmile sm, map<double,double> PutStrikeVolMapper);
        fittingError = SmileCalibration(sm, PutStrikeVolMapper);
        res.insert(std::pair<std::string, std::pair<Smile, double>>(iter->first,std::pair<Smile, double>(sm, fittingError)));
    }
    return res;
}

#endif //QF633_CODE_VOLSURFBUILDER_H
