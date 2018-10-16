#ifndef SRC_RANDOMSTRATEGY_H_
#define SRC_RANDOMSTRATEGY_H_

#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include "Strategy.h"
#include "Portfolio.h"
#include "Utility.h"

typedef boost::mt19937 RNGType;

class RandomStrategy : public Strategy {
private:
    const double sell_probability = 0.02;
    const double buy_probability = 0.1;
    boost::variate_generator< RNGType, boost::uniform_real<> > random;
public:
    RandomStrategy(long double money, RNGType rng, boost::uniform_real<> zero_to_one) : Strategy(money), random(rng, zero_to_one) {}

    std::vector<StockOrder> rebalance(boost::gregorian::date cur_date) override {
        std::vector<StockOrder> result;
        for(std::string stock_name : p.get_all_stocks()) {
            if(random() < sell_probability) {
                StockOrder new_order(Sell, p.get_stock_amount(stock_name), stock_name, cur_date);
                result.push_back(new_order);
                p.apply_order(new_order);
            }
        }
        for(std::string potential_stock : Coordinator::get_all_stocks()) {
            if(random() < buy_probability) {
                long double investment_sum = random() * p.get_current_cash();
                int num_shares = (int) (investment_sum / Coordinator::get_price_at(potential_stock, cur_date).value().get_open());
                if (num_shares > 0) {
                    StockOrder new_order(Buy, num_shares, potential_stock, cur_date);
                    result.push_back(new_order);
                    p.apply_order(new_order);
                }
            }
        }
        for(auto el : result) {
            std::cout << el.to_string() << std::endl;
        }
        p.remove_empty_stocks();
        return result;
    }
};

#endif