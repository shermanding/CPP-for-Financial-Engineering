#include <string.h>
#include <iostream>
#include <vector>

#include "util/incremental_csv_reader.h"
#include "book/l3_book.h"

void PrintLevel2Book(const L3Book& l3_book) {
    printf("%10s %10s %10s %10s %10s %10s\n", "bid_count", "bid_qty", "bid_price", "ask_price", "ask_qty", "ask_count");
    l3_book.ForEachLevel([=](
        double bid_price, int64_t bid_qty, int64_t bid_count, 
        double ask_price, int64_t ask_qty, int64_t ask_count) {
        printf("%10zu %10zu %10.2f %10.2f %10zu %10zu\n", bid_count, bid_qty, bid_price, ask_price, ask_qty, ask_count);
        return true;
    });
}

void PrintLevel3Book(const L3Book& l3_book) {
    printf("%5s %10s %10s %10s", "side", "price", "qty", "orders");
    double last_price = -123456;
    auto print_level = [&last_price] (bool is_buy, double level_price, int64_t level_qty, 
                                      int64_t qty, int64_t order_id) {
        if (level_price != last_price) {
            printf("\n%5s %10.2f %10zu %4s", is_buy ? "B" : "S", level_price, level_qty, "");
        }
        printf("%zu(%zu), ", qty, order_id);
        last_price = level_price;
        return true;
    };
    l3_book.ForEachOrder(false, false, print_level);
    last_price = -123456;
    l3_book.ForEachOrder(true, true, print_level);
    printf("\n");
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: %s filename l2/l3\n", argv[0]);
        return 1;
    }

    const std::string filename = argv[1];
    const bool print_l2 = !strcmp(argv[2], "l2");

    const std::vector<std::string> expected_header{
        "action",
        "side",
        "order_id",
        "price",
        "qty"
    };

    L3Book l3_book;
    auto handle_row = [&expected_header, &l3_book, print_l2](const auto& header, const auto& row) {
        if (header != expected_header) {
            throw std::runtime_error("unexpected header");
        }

        const auto& action = row[0];
        const auto is_buy = row[1][0] == 'B';
        const auto order_id = std::stoll(row[2]);
        const auto price = std::stod(row[3]);
        const auto qty = std::stoll(row[4]);

        if (!strcmp(action.c_str(), "add")) {
            l3_book.ProcessAdd(order_id, is_buy, price, qty);
        } else if (!strcmp(action.c_str(), "replace")) {
            l3_book.ProcessReplace(order_id, is_buy, price, qty);
        } else if (!strcmp(action.c_str(), "cancel")) {
            l3_book.ProcessDelete(order_id);
        } else if (!strcmp(action.c_str(), "exec")) {
            l3_book.ProcessExec(order_id, qty);
        } else {
            throw std::runtime_error("unexpected action");
        }

        printf("\nprocess msg action %s, side: %s, order_id: %s, price: %s, qty: %s\n", 
               row[0].c_str(), row[1].c_str(), row[2].c_str(), row[3].c_str(), row[4].c_str());
        if (print_l2) {
            PrintLevel2Book(l3_book);
        } else {
            PrintLevel3Book(l3_book);
        }
    };

    IncrementalCSVReader reader(filename, handle_row); 

    while (reader.Step()) {
    }

    return 0;
}
