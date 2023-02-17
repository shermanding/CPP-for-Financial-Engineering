#include "util/incremental_csv_reader.h"

namespace {
    
void Split(const std::string& line, std::vector<std::string>& values) {
    auto start = 0u;
    for (auto curr = 0u; curr < line.size(); ++curr) {
        if (line[curr] == ',') {
            values.push_back(line.substr(start, curr - start));
            start = curr + 1;
        }
    }
    values.push_back(line.substr(start, line.size() - start));
}
}

IncrementalCSVReader::IncrementalCSVReader(std::string filename, Listener listener)
    : file_(filename), listener_(std::move(listener)) {
    if (!file_.is_open()) {
        throw std::runtime_error("failed to open csv file");
    }
    std::string header_line;
    if (!getline(file_, header_line)) {
        throw std::runtime_error("failed to get header");
    }
    Split(header_line, header_);
}

bool IncrementalCSVReader::Step() {
    std::string line;
    if (!getline(file_, line)) {
        return false;
    }

    Row row;
    Split(line, row);
    if (row.size() != header_.size()) {
        throw std::runtime_error("inconsistent header and row length");
    }

    listener_(header_, row);
    return true;
}