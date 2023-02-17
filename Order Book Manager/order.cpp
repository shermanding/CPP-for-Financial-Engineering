#include "order.h"

std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << "price=" << order.price
        << " open_qty=" << order.open_qty
        << " order_id=" << order.order_id
        << " is_sell=" << static_cast<int>(order.is_sell);
    return os;
}