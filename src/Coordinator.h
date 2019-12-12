#ifndef SRC_COORDINATOR_H_
#define SRC_COORDINATOR_H_

#include <vector>
#include <unordered_map>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/optional.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include "StockData.h"
#include "Utility.h"

namespace fs = std::filesystem;

class Coordinator {
private:
  static std::unordered_map<std::string, StockData> stocks;

  /* static void read_data(const std::string& path) {
    std::string raw_line;
    ifstream stock_file (path);
    if (stock_file.is_open()) {
        while(getline (stock_file, raw_line)) {
            std::vector<std::string> parsed_line;
            boost::tokenizer< boost::escaped_list_separator<char> > tk(raw_line, boost::escaped_list_separator<char>('\\', ',', '\"'));
            for (boost::tokenizer<boost::escaped_list_separator<char> >::iterator i(tk.begin()); i!=tk.end(); ++i) {
               parsed_line.push_back(*i);
            }
            StockDataPoint data_point(parsed_line);
        }
        stock_file.close();
    }*/
public:
  static StockData get_price_history(const std::string& stock_name) {
    return Coordinator::stocks.at(stock_name);
  }

  static boost::optional<StockDataPoint> get_price_at(const std::string& stock_name, boost::gregorian::date date) {
    StockData sd = Coordinator::stocks.at(stock_name);
    for(StockDataPoint sdp : sd.get_price_history()) {
      if(sdp.get_date() == date) {
        return boost::optional<StockDataPoint>(sdp);
      }
    }
    return boost::optional<StockDataPoint>();
  }

  static std::vector<std::string> get_all_stocks() {
    return get_keys(Coordinator::stocks);
  }

  static void read_stock_data(const std::string& path_name) {
    for (auto& file_name : fs::directory_iterator(path_name)) {
      std::string file_name_string = file_name.path().string();
      std::string stock_name = file_name.path().filename().string();
      // Öffne file <file_name>
      // Iteriere zeilenweise durch
      // Splitte zeilen nach , gib den Vector mit der Zeile an StockDataPoint Konstruktor, baue daraus StockData 
      StockData cur_data;
      std::vector<StockDataPoint> price_history;
      std::ifstream stock_file(file_name_string);
      std::string line;
      while (std::getline(stock_file, line)) {
        std::vector<std::string> split_input;
        boost::split(split_input, line, [](char c){return c == ',';});
        price_history.push_back(StockDataPoint(split_input));
      }
      cur_data.price_history = std::move(price_history);
      Coordinator::stocks[stock_name] = cur_data;
    }
  }

  static bool date_valid(const boost::gregorian::date& to_check) {
    StockData random_stock = Coordinator::stocks.at("SnP500_historical_data.csv");
    return random_stock.defined_for_date(to_check);
  }
};

#endif
