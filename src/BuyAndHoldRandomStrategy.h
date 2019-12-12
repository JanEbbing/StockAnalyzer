//
// Created by jan on 26/11/19.
//

#ifndef PROJECTNAME_BUYANDHOLDRANDOMSTRATEGY_H
#define PROJECTNAME_BUYANDHOLDRANDOMSTRATEGY_H

#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include "Strategy.h"
#include "Portfolio.h"
#include "Utility.h"

typedef boost::mt19937 RNGType;

class BuyAndHoldRandomStrategy : public Strategy {
private:
  const double sell_probability = 0.4;
  const double buy_probability = 0.2;
  boost::variate_generator< RNGType, boost::uniform_real<> > random;
  std::unordered_map<std::string, double> worst_prices;
public:
  BuyAndHoldRandomStrategy(long double money, RNGType rng, boost::uniform_real<> zero_to_one) : Strategy(money, "Buy & Hold Random Strategy"), random(rng, zero_to_one) {}

  std::string getName() override {
    return "Buy & Hold Random Strategy";
  }

  std::vector<StockOrder> rebalance(boost::gregorian::date cur_date) override {
    std::vector<StockOrder> result;
    for(const std::string& stock_name : p.get_all_stocks()) {
      if(this->sells_with_profit(stock_name, cur_date) && random() < sell_probability) {
        StockOrder new_order(Sell, p.get_stock_amount(stock_name), stock_name, cur_date);
        result.push_back(new_order);
        p.apply_order(new_order);
        worst_prices[stock_name] = 0.0;
      }
    }
    for(const std::string& potential_stock : Coordinator::get_all_stocks()) {
      if(random() < buy_probability) {
        long double investment_sum = random() * p.get_current_cash();
        double share_price = Coordinator::get_price_at(potential_stock, cur_date).value().get_open();
        int num_shares = (int) (investment_sum / share_price);
        if (num_shares > 0) {
          StockOrder new_order(Buy, num_shares, potential_stock, cur_date);
          result.push_back(new_order);
          p.apply_order(new_order);
          worst_prices[potential_stock] = std::max(worst_prices[potential_stock], share_price);
        }
      }
    }
    if (false) { // TODO: replace with global flag
      for(auto el : result) {
        std::cout << el.to_string() << std::endl;
      }
    }
    p.remove_empty_stocks();
    return result;
  }
private:
  bool sells_with_profit(const std::string& stock_name, const boost::gregorian::date& cur_date) {
    if (this->worst_prices.find(stock_name) == this->worst_prices.end()) {
      return true;
    } else {
      return this->worst_prices.find(stock_name)->second <= Coordinator::get_price_at(stock_name, cur_date).value().get_open();
    }
  }
};

#endif //PROJECTNAME_BUYANDHOLDRANDOMSTRATEGY_H
