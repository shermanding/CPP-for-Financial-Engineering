#include <iostream>
#include "CsvFeeder.h"
#include "date/date.h"
#include <string>
#include <cstring>
#include <thread>
#include <chrono>

using std::cout;
using std::endl;
using std::copy;
using std::this_thread::sleep_for;
using namespace std::chrono_literals;

uint64_t TimeToUnixMS(std::string ts) {
    std::istringstream in{ts};
    std::chrono::system_clock::time_point tp;
    in >> date::parse("%FT%T", tp);
    const auto timestamp = std::chrono::time_point_cast<std::chrono::milliseconds>(tp).time_since_epoch().count();
    return timestamp;
}

bool ReadNextMsg(std::ifstream& file, Msg& msg, Msg& tempMsg_) {
    int counter = 0;

    // Checking if any message was sent by previous call
    if (tempMsg_.isSet == true){
        msg = tempMsg_;
        tempMsg_ = Msg();
        counter++;
    }

    // Checking if it's the end of file yet
    if (file.eof()) {
        return false;
    }
    // TODO: your implementation to read file and create the next Msg into the variable msg
    
    // declaring the variables
    std::string line,row,snap;
    uint64_t currentTimestamp,lastUpdateTimeStamp;
    
    if(file.is_open()){
        // Looping through each row
        while (getline(file, line)){
            // Streaming row into string and checking if it is header row
            std::stringstream str(line);
            std::string tempWord;

            if (std::strcmp(str.str().substr(0,8).c_str(),"contract")==0){
                continue;
            }
            // Declaring a new TickData for each row
            TickData tickData;

            // Splitting row into strings
            int col = 0;
            while (getline(str,tempWord,',')){
                if (col == 0){
                    tickData.ContractName = tempWord.c_str();
                    col++;
                    continue;
                }
                if (col == 1){
                    lastUpdateTimeStamp = TimeToUnixMS(tempWord);
                    tickData.LastUpdateTimeStamp = lastUpdateTimeStamp;
                    col++;
                    continue;
                }
                if (col == 2){
                    snap = tempWord.c_str();
                    col++;
                    continue;
                }
                if (col == 4){
                    tickData.BestBidPrice = (tempWord.empty())? std::numeric_limits<double>::quiet_NaN() : stod(tempWord);
                    col++;
                    continue;
                }
                if (col == 5){
                    tickData.BestBidAmount = (tempWord.empty())? std::numeric_limits<double>::quiet_NaN() : stod(tempWord);
                    col++;
                    continue;
                }
                if (col == 6){
                    tickData.BestBidIV = (tempWord.empty())? std::numeric_limits<double>::quiet_NaN() : stod(tempWord);
                    col++;
                    continue;
                }
                if (col == 7){
                    tickData.BestAskPrice = (tempWord.empty())? std::numeric_limits<double>::quiet_NaN() : stod(tempWord);
                    col++;
                    continue;
                }
                if (col == 8){
                    tickData.BestAskAmount = (tempWord.empty())? std::numeric_limits<double>::quiet_NaN() : stod(tempWord);
                    col++;
                    continue;
                }
                if (col == 9){
                    tickData.BestAskIV= (tempWord.empty())? std::numeric_limits<double>::quiet_NaN() : stod(tempWord);
                    col++;
                    continue;
                }
                if (col == 10){
                    tickData.MarkPrice = (tempWord.empty())? std::numeric_limits<double>::quiet_NaN() : stod(tempWord);
                    col++;
                    continue;
                }
                if (col == 11){
                    tickData.MarkIV = (tempWord.empty())? std::numeric_limits<double>::quiet_NaN() : stod(tempWord);
                    col++;
                    continue;
                }
                if (col == 12){
                    tickData.UnderlyingIndex = tempWord.c_str();
                    col++;
                    continue;
                }
                if (col == 13){
                    tickData.UnderlyingPrice = (tempWord.empty())? std::numeric_limits<double>::quiet_NaN() : stod(tempWord);
                    col++;
                    continue;
                }
                if (col == 15){
                    tickData.LastPrice = (tempWord.empty())? std::numeric_limits<double>::quiet_NaN() : stod(tempWord);
                    col++;
                    continue;
                }
                if (col == 16){
                    tickData.OpenInterest = (tempWord.empty())? std::numeric_limits<double>::quiet_NaN() : stod(tempWord);
                    col++;
                    continue;
                }
                col++;
            }
            if (msg.timestamp != lastUpdateTimeStamp && counter>0){
                tempMsg_.timestamp = lastUpdateTimeStamp;
                if (snap == "snap"){
                    tempMsg_.isSnap = true;
                }else{
                    tempMsg_.isSnap = false;
                }
                tempMsg_.Updates.push_back(tickData);
                tempMsg_.isSet = true;
                msg.isSet = true;
                return true;
            } else if (counter == 0) {
                msg.timestamp=lastUpdateTimeStamp;
                if (snap == "snap"){
                    msg.isSnap = true;
                }else{
                    msg.isSnap = false;
                }
                msg.isSet=true;
                msg.Updates.push_back(tickData);
            } else {
                msg.Updates.push_back(tickData);
            }
            counter++;
        }
    } else{
        std::cout << "Could not open the file" << std::endl;
    }
    return true;
}


CsvFeeder::CsvFeeder(const std::string ticker_filename,
                     FeedListener feed_listener,
                     std::chrono::minutes interval,
                     TimerListener timer_listener)
        : ticker_file_(ticker_filename),
          feed_listener_(feed_listener),
          interval_(interval),
          timer_listener_(timer_listener) {
    // initialize member variables with input information, prepare for Step() processing

    ReadNextMsg(ticker_file_, msg_, tempMsg_);
    if (msg_.isSet) {
        // initialize interval timer now_ms_
        now_ms_ = msg_.timestamp;
    } else {
        throw std::invalid_argument("empty message at initialization");
    }
}

bool CsvFeeder::Step() {
    if (msg_.isSet) {
        // call feed_listener with the loaded Msg
        feed_listener_(msg_);

        // if current message's timestamp is crossing the given interval, call time_listener, change now_ms_ to the next interval cutoff
        if (now_ms_ < msg_.timestamp) {
            timer_listener_(now_ms_);
            now_ms_ += interval_.count();
        }
        // load tick data into Msg
        // if there is no more message from the csv file, return false, otherwise true
        return ReadNextMsg(ticker_file_, msg_, tempMsg_);
    }
    return false;
}

CsvFeeder::~CsvFeeder() {
    // release resource allocated in constructor, if any
}