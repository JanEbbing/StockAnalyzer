#ifndef SRC_STRATEGY_H_
#define SRC_STRATEGY_H_

#include <vector>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "Portfolio.h"

class Strategy {
protected:
    const long double starting_money;
    Portfolio p;
public:
    Strategy(long double money) : starting_money(money), p(money) {}
    long double get_current_networth(boost::gregorian::date current_date) {
        return p.get_current_networth(current_date);
    }
    virtual std::vector<StockOrder> rebalance(boost::gregorian::date cur_date) = 0;
};

#endif