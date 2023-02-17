#include "book/l3_book.h"
#include <iostream>
#include <algorithm>

void L3Book::UncrossBookSide(bool is_buy) {
    // uncross the book by removing the offensive orders from the given book side
    // is_buy: true, remove any buy orders which could cross with the sell side 
    // is_buy: false, remove any sell orders which could cross with the buy side

    // list of stale orders
    std::list<int64_t> stale_orders;

    if (is_buy)
    {
        for (auto& buyItr : buy_side_)
        {
            Price P = buyItr.first;
            // When an order is added to the buy side with price P , any sell orders with prices less or equal to P are stale orders and should be purged.            
            for (auto innerItr = sell_side_.rbegin(); innerItr != sell_side_.rend(); innerItr++ )
            {   
                auto innerBid = innerItr->first;
                auto innerLevel = innerItr->second;
                if (innerBid <= P)
                {
                    for(auto& order : innerLevel.orders) {
                        stale_orders.push_back(order->order_id);
                    }
                }
            }
        } 
    }
    else
    {
        for (auto& sellItr : sell_side_)
        {
            Price P = sellItr.first;
            // when an order is added to the sell side with price P , any buy orders with prices greater or equal to P are considered to be stale orders.
            for (auto innerItr = buy_side_.begin(); innerItr != buy_side_.end(); innerItr++ )
            {   
                auto innerAsk = innerItr->first;
                auto innerLevel = innerItr->second;
                if (innerAsk >= P)
                {
                    for (auto& order : innerLevel.orders) {
                        stale_orders.push_back(order->order_id);
                    }
                }
            }
        }
    }
    // process delete each order in stale order list
    for (auto& stale_itr : stale_orders) {
        ProcessDelete(stale_itr);
    }
}

void L3Book::ProcessAdd(int64_t order_id, bool is_buy, double price, int64_t qty) {
    auto add_to_side = [=](auto& side) {
        // find or create the price level  
        const auto [level_itr, new_level] = side.emplace(Price(price), Level(price));
        Level* level = &(level_itr->second);

        // save the order to orders_
        const auto [order_itr, new_order] = orders_.emplace(
            order_id, Order(order_id, is_buy, price, qty)
        );
        if (!new_order) {
            throw std::runtime_error("add order with duplicated order_id");
        }
        Order* order = &(order_itr->second);

        // update the level's summary data
        level->qty += qty;
        level->count += 1;

        // link Level with Order, note:
        // 1. list::push_front: O(1) operation while push_back is O(N)
        // 2. iterator invalidatio rule: 
        //       a) reference to element in std::unordered_map is stable, i.e. level
        //       b) reference to element in std::list is stable, i.e. order
        level->orders.push_front(&order_itr->second);  
        order->level = level;
        order->pos = level->orders.begin();
    };

    if (is_buy) {
        add_to_side(buy_side_);
    } else {
        add_to_side(sell_side_);
    }
    UncrossBookSide(!is_buy);
}

void L3Book::ProcessReplace(int64_t order_id, bool is_buy, double price, int64_t qty) {
    auto order_itr = orders_.find(order_id);
    if (order_itr == orders_.end()) {
        throw std::runtime_error("replace non-existing order");
    }
    auto& orig_order = order_itr->second;
    const bool inplace = Price(price) == Price(orig_order.price) && qty < orig_order.qty;
    if (inplace) {
        orig_order.level->qty -= (orig_order.qty - qty);
        orig_order.qty = qty;
    } else {
        ProcessDelete(order_id);
        ProcessAdd(order_id, is_buy, price, qty);
    }
}

void L3Book::ProcessDelete(int64_t order_id) {
    auto order_itr = orders_.find(order_id);
    if (order_itr == orders_.end()) {
        throw std::runtime_error("delete non-existing order");
    }
    const Order& order = order_itr->second;
    Level* level = order.level;

    // unlink Level with Order
    level->orders.erase(order.pos);

    // update the Level summary data
    level->qty -= order.qty;
    level->count -= 1;
    if (level->qty < 0 || level->count < 0) {
        throw std::runtime_error("delete more qty than available");
    }

    // delete the Level if no more orders
    auto delete_level = [] (auto& side, double price) {
        auto removed = side.erase(Price(price));
        if (removed == 0) {
            throw std::runtime_error("delete non-existing level");
        }
    };
    if (level->qty == 0) {
        if (order.is_buy) {
            delete_level(buy_side_, order.price);
        } else {
            delete_level(sell_side_, order.price);
        }
    }

    // delete the order
    orders_.erase(order_itr);
}

void L3Book::ProcessExec(int64_t order_id, int64_t exec_qty) {
    // TODO handle the execution message
    auto order_itr = orders_.find(order_id);
    if (order_itr == orders_.end()) {
        throw std::runtime_error("exec non-existing order");
    }
    auto& order = order_itr->second;
    int64_t order_qty = order.qty;
    // handle partial execution by reducing order qty and level qty
    if (exec_qty < order_qty) {
        ProcessReplace(order_id, order.is_buy, order.price, (order_qty - exec_qty));
    }
    // handle full execution and can be dropped
    else if (exec_qty == order_qty) {
        ProcessDelete(order_id);
    }
    // exception handling
    else
        throw std::runtime_error("NO SUCH ORDER PRESENT OR ORDER EXECUTION AMOUNT EXCEEDS");

}