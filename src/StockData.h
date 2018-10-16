#ifndef SRC_STOCK_DATA_H_
#define SRC_STOCK_DATA_H_

#include "StockDataPoint.h"
#include <boost/date_time/gregorian/gregorian.hpp>

class StockData {
//private:
//  std::vector<StockDataPoint> price_history;
public:
  std::vector<StockDataPoint> price_history;
  std::vector<StockDataPoint> get_price_history() {
    return price_history;
  }
  bool defined_for_date(const boost::gregorian::date& to_check) {
    for(auto sdp : price_history) {
      if(sdp.get_date() == to_check) {
        return true;
      }
    }
    return false;
  }
};

#endif /* end of include guard: SRC_STOCK_DATA_H_ */
