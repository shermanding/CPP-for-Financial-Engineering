#include <iostream>
#include "CsvFeeder.h"
#include "date/date.h"

uint64_t TimeToUnixMS(std::string ts) {
    std::istringstream in{ts};
    std::chrono::system_clock::time_point tp;
    in >> date::parse("%FT%T", tp);
    const auto timestamp = std::chrono::time_point_cast<std::chrono::milliseconds>(tp).time_since_epoch().count();
    return timestamp;
}

bool ReadNextMsg(std::ifstream& file, Msg& msg) {
    if (file.eof()) {
        return false;
    }
    // TODO: your implementation to read file and create the next Msg into the variable msg
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

    ReadNextMsg(ticker_file_, msg_);
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
        return ReadNextMsg(ticker_file_, msg_);
    }
    return false;
}

CsvFeeder::~CsvFeeder() {
    // release resource allocated in constructor, if any
}