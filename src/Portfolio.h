#ifndef SRC_PORTFOLIO_H_
#define SRC_PORTFOLIO_H_

#include <unordered_map>
#include <boost/optional.hpp>
#include <limits>
#include "Coordinator.h"
#include "StockOrder.h"
#include "Utility.h"

class Portfolio {
private:
  long double current_cash;
  std::unordered_map<std::string, long double> current_investments;

public:
  Portfolio() {}

  Portfolio(long double starting_cash) {
      current_cash = starting_cash;
  }

  Portfolio(const Portfolio& rhs) {
    current_cash = rhs.current_cash;
    current_investments.reserve(rhs.current_investments.size());
    for(auto kv : rhs.current_investments) {
        current_investments[kv.first] = kv.second;
    }
  }

  Portfolio& operator=(const Portfolio& rhs) {
    // check for self-assignment
    if(&rhs == this) {
        return *this;
    }
    Portfolio tmp(rhs);
    std::swap(current_cash, tmp.current_cash);
    std::swap(current_investments, tmp.current_investments);
  }

  std::vector<std::string> get_all_stocks() {
      return get_keys(current_investments);
  }

  void remove_empty_stocks() {
      std::vector<std::string> to_delete;
      for(auto it = current_investments.begin(); it != current_investments.end(); ) {
          if(abs(it->second) < std::numeric_limits<long double>::epsilon()) {
              it = current_investments.erase(it);
          } else {
              ++it;
          }
      }
  }

  long double get_current_cash() {
      return current_cash;
  }

  long double get_stock_amount(const std::string& stock_name) {
      if(current_investments.find(stock_name) != current_investments.end()) {
        return current_investments[stock_name];
      } else {
        return 0;
      }
  }

  long double get_current_networth(boost::gregorian::date current_date) {
    long double result = current_cash;
    for(std::string stock : get_keys(current_investments)) {
        try {
            result += current_investments[stock] * Coordinator::get_price_at(stock, current_date).value().get_open();
        } catch(const boost::bad_optional_access& ex) {
            std::cout << "Error during get networth execution! Date " << current_date.year() << ", " << current_date.month() << ", " << current_date.day() << " does not exist for stock " << stock << std::endl;
        }
    }
    return result;
  }

  void apply_order(StockOrder order) {
    // Sanity check: Is enough money present (buy)/Do we have the stock we want to sell
    switch(order.t) {
        case Buy:
          {
            boost::optional<StockDataPoint> buy_price_point = Coordinator::get_price_at(order.stock_name, order.date);
            if(buy_price_point && (*buy_price_point).get_open() * order.amount > current_cash) {
                std::cout << "Error during apply order! Executing order " << order.to_string() << ", had not enough cash (" << current_cash << ")";
            }
            break;
          }
        case Sell:
          {
            if(current_investments[order.stock_name] < order.amount) {
                std::cout << "Error during apply order! Executing order " << order.to_string() << ", had not enough stocks of type (" << order.stock_name << ")";
            }
            break;
          }
        default:
          {
            std::cout << "Fatal error during apply order! Order had no valid type.";
            break;
          }
    }
    boost::optional<StockDataPoint> price_point = Coordinator::get_price_at(order.stock_name, order.date);

    switch(order.t) {
        case Buy:
          {
            if(price_point) {
                current_cash = current_cash - (*price_point).get_open() * order.amount;
                if(current_investments.find(order.stock_name) != current_investments.end()) {
                        current_investments[order.stock_name] += order.amount;
                } else {
                        current_investments[order.stock_name] = order.amount;
                }
            } else {
                std::cout << "Fatal error in apply_order, stock suddenly does not exist any more.";
            }
            break;
          }
        case Sell:
          {
            if(price_point) {
                current_investments[order.stock_name] -= order.amount;
                current_cash += (*price_point).get_open() * order.amount;
            } else {
                std::cout << "Fatal error in apply_order, stock suddenly does not exist any more.";
            }
            break;
          }
        default:
          {
            std::cout << "Fatal error during apply order! Order had no valid type.";
            break;
          }
    }
  }
};


#endif /* end of include guard: SRC_PORTFOLIO_H_ */
