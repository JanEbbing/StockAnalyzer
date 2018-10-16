#ifndef SRC_STOCKORDER_H_
#define SRC_STOCKORDER_H_

#include <sstream>
#include "boost/date_time/gregorian/gregorian.hpp"

enum OrderType { Buy, Sell };

class StockOrder {
private:
    std::string order_type_to_string(OrderType type) {
        switch(type) {
            case Buy: return "Buy";
            case Sell: return "Sell";
            default: return "Error";
        }
    }
public:
    OrderType t;
    long double amount;
    std::string stock_name;
    boost::gregorian::date date;

    StockOrder(const OrderType order_type, const long double money_amount, const std::string& name, const boost::gregorian::date& order_date) {
        t = order_type;
        amount = money_amount;
        stock_name = name;
        date = order_date;
    }

    std::string to_string() {
        std::ostringstream os;
        os << "Oder: Type: " << order_type_to_string(t) << ", amount: " << amount << ", name: " << stock_name << ", date " << date.day() << date.month() << date.year();
        return os.str();
    }
};

#endif /* end of include guard: SRC_STOCKORDER_H_ */
