#pragma once

#include <functional>
#include <string>
#include <fstream>

class IncrementalCSVReader {
 public:
    using Header = std::vector<std::string>;
    using Row = std::vector<std::string>;
    using Listener = std::function<void(const Header& header, const Row& row)>;

 public:
    IncrementalCSVReader(std::string filename, Listener listener);

    bool Step();

 private:
    std::ifstream file_;
    Header header_;
    Listener listener_;
};