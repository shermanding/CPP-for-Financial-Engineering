#ifndef QF633_CODE_VOLSURFBUILDER_H
#define QF633_CODE_VOLSURFBUILDER_H

#include <map>
#include "Msg.h"
#include "Date.h"

template<class Smile>
class VolSurfBuilder {
public:
    void Process(const Msg& msg);  // process message
    void PrintInfo();
    std::map<datetime_t, std::pair<Smile, double> > FitSmiles();
protected:
    // we want to keep the best level information for all instruments
    // here we use a map from contract name to BestLevelInfo, the key is contract name
    std::map<std::string, TickData> currentSurfaceRaw;
};

template <class Smile>
void VolSurfBuilder<Smile>::Process(const Msg& msg) {
    if (msg.isSnap) {
        // discard currently maintained market snapshot, and construct a new copy based on the input Msg
    } else {
        // update the currently maintained market snapshot
    }
}

template <class Smile>
void VolSurfBuilder<Smile>::PrintInfo() {
    // TODO: you may print out information about VolSurfBuilder's currentSnapshot to test
}

template <class Smile>
std::map<datetime_t, std::pair<Smile, double> > VolSurfBuilder<Smile>::FitSmiles() {
    std::map<datetime_t, std::vector<TickData> > tickersByExpiry{};
    // TODO: group the tickers in the current market snapshot by expiry date, and construct tickersByExpiry
    // ...

    std::map<datetime_t, std::pair<Smile, double> > res{};
    // then create Smile instance for each expiry by calling FitSmile() of the Smile
    for (auto iter = tickersByExpiry.begin(); iter != tickersByExpiry.end(); iter++) {
        auto sm = Smile::FitSmile(iter->second);  // TODO: you need to implement FitSmile function in CubicSmile
        double fittingError = 0;
        // TODO: we need to measure the fitting error here
        res.insert(std::pair<datetime_t, std::pair<Smile, double> >(iter->first,std::pair<Smile, double>(sm, fittingError)));
    }
    return res;
}

#endif //QF633_CODE_VOLSURFBUILDER_H
