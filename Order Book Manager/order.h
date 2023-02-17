#include <string>
#include <ostream>

struct Order {
    Order(int price, int open_qty, const std::string& order_id, bool is_sell)
        : price(price), open_qty(open_qty), order_id(order_id), is_sell(is_sell) {}

    int price;               // the order's price, and int type used instead of double type for simplicity
    int open_qty;            // the open qty of the order
    std::string order_id;    // an unique order ID to identify the order 
    bool is_sell;            // order side, true - buy order, false - sell order
};

std::ostream& operator<<(std::ostream& os, const Order& order);