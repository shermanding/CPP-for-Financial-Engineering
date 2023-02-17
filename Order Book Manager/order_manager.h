#include <vector>

#include "order.h"

class OrderManager {
public:
    // add a new record to the order manager
    void Add(int price, int qty, const std::string& order_id, bool is_sell);

    // update the open_qty for an existing order record and delete the record if updated_open_qty== 0
    void Update(const std::string& order_id, int updated_open_qty);

    // print all orders following the order specified, i.e. 
    // - from lower prices to high prices 
    // - following FIFO if same price
    void PrintAllOrders() const;

private:
    std::vector<Order> orders_;
};
