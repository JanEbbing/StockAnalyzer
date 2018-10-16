#ifndef SRC_EVALUATOR_H_
#define SRC_EVALUATOR_H_

#include <vector>
#include "RandomStrategy.h"
#include "Portfolio.h"

class Evaluator {
public:
    static long double evaluate(const Portfolio& p, const std::vector<StockOrder>& orders, const boost::gregorian::date& closing_date) {
        Portfolio tmp = p;
        for(StockOrder so : orders) {
            tmp.apply_order(so);
        }
        long double result = tmp.get_current_cash();
        for(const std::string remaining_stock : tmp.get_all_stocks()) {
            result += tmp.get_stock_amount(remaining_stock) * Coordinator::get_price_at(remaining_stock, closing_date).value().get_open();
        }
        return result;
    }
};

#endif /* end of include guard: SRC_EVALUATOR_H_ */
