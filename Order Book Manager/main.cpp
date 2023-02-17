#include <iostream>

#include "order_manager.h"

int main() {
    OrderManager order_manager;

    order_manager.Add(100, 2, "o1", true);
    order_manager.Add(99, 1, "o2", true);
    order_manager.Add(100, 3, "o3", true);
    order_manager.Add(101, 4, "o4", true);

    std::cout << "step 1" << std::endl;
    order_manager.PrintAllOrders();

    order_manager.Update("o3", 1);
    order_manager.Update("o2", 0);
    order_manager.Add(98, 5, "o5", false);
    order_manager.Add(101, 1, "o6", true);
    order_manager.Add(100, 2, "o7", true);
    std::cout << "step 2" << std::endl;
    order_manager.PrintAllOrders();
    return 0;
}