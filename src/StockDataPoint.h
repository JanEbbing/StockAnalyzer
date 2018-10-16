#ifndef STOCK_DATA_POINT_H_
#define STOCK_DATA_POINT_H_

#include "boost/date_time/gregorian/gregorian.hpp"

class StockDataPoint {
    boost::gregorian::date date;

    double open;
    double close;
    double high;
    double low;

    double adj_close;
    uint32_t volume;

public:
    StockDataPoint() {}

    StockDataPoint(std::vector<std::string> input_data) : open(std::stod(input_data[1])), close(std::stod(input_data[4])), high(std::stod(input_data[2])), low(std::stod(input_data[3])), adj_close(std::stod(input_data[1])), volume(std::stoi(input_data[5])) {
        date = boost::gregorian::from_simple_string(input_data[0]);
    }

    boost::gregorian::date get_date() {
        return date;
    }

    double get_open() {
        return open;
    }

};

#endif