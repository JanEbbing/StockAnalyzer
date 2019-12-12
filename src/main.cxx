#include <cstdlib>
#include <cmath>
#include <boost/program_options.hpp>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "app.h"
#include "Coordinator.h"
#include "RandomStrategy.h"
#include "BuyAndHoldRandomStrategy.h"

#include <QtCharts>

using namespace QtCharts;

namespace po = boost::program_options;
std::unordered_map<std::string, StockData> Coordinator::stocks;

po::options_description getDescription() {

  po::options_description desc("Options");
  desc.add_options()
    ("help,h", "Display this message")
    ("version,v", "Display the version number");

  return desc;
}

/*
int main(int argc, char *argv[]) {

    po::options_description desc = getDescription();
    po::variables_map varmap;

    try {
        store(po::command_line_parser(argc, argv).options(desc).run(), varmap);
        notify(varmap);
    } catch (po::error &error) {
        std::cerr << error.what() << std::endl;
        return EXIT_FAILURE;
    }

    if (varmap.count("help")) {
        std::cout << desc << std::endl;
    }

    if (varmap.count("version")) {
        App app;
        std::cout << app.getProjectVersion() << std::endl;
    }

    return EXIT_SUCCESS;
}
*/

int test_trading_strategy(int argc, char **argv, Strategy &cur_strat, int investment_money) {
  std::string RESOURCES_PATH = "/home/jan/StockAnalyzer/resources/";
  Coordinator::read_stock_data(RESOURCES_PATH);
  for (const std::string& stocks : Coordinator::get_all_stocks()) {
    std::cout << stocks << std::endl;
  }

  std::string first_date_string("19500103");
  boost::gregorian::date first_date(boost::gregorian::from_undelimited_string(first_date_string));
  boost::gregorian::date current_date(boost::gregorian::from_undelimited_string(first_date_string));
  std::string last_date_string("20180827");
  boost::gregorian::date last_date(boost::gregorian::from_undelimited_string(last_date_string));

  std::cout << "Started on " << boost::gregorian::to_simple_string(first_date);
  auto *series = new QLineSeries();
  series->setName(QString::fromStdString(cur_strat.getName()));
  auto *dax_series = new QLineSeries();
  dax_series->setName("DAX Series");
  auto *sp_series = new QLineSeries();
  sp_series->setName("S&P 500 Series");
  auto *inflation_series2 = new QLineSeries();
  inflation_series2->setName("Inflation @ 2%");
  auto *inflation_series3 = new QLineSeries();
  inflation_series3->setName("Inflation @ 3%");
  int i = 0;
  while (current_date <= last_date) {
    cur_strat.rebalance(current_date);
    series->append(i, cur_strat.get_current_networth(current_date));
    dax_series->append(i, 1000 / 16 * Coordinator::get_price_at("DAX_30_historical_data_fake.csv", current_date).value().get_open());
    sp_series->append(i, 1000 / 16 * Coordinator::get_price_at("SnP500_historical_data.csv", current_date).value().get_open());
    boost::gregorian::date_duration passed_duration = current_date - first_date;
    float exponent = passed_duration.days() / 365.0f;
    inflation_series2->append(i, investment_money * std::pow(1.02, exponent));
    inflation_series3->append(i, investment_money * std::pow(1.03, exponent));

    do {
      boost::gregorian::days next_day(1);
      current_date = current_date + next_day;
      i++;
    } while (!Coordinator::date_valid(current_date) && current_date <= last_date);
  }

  std::cout << "\nEnded up with " << cur_strat.get_current_networth(last_date) << " on "
            << boost::gregorian::to_simple_string(last_date) << std::endl;
  QApplication app(argc, argv);

  QChart *chart = new QChart();
  chart->legend()->setVisible(true);
  chart->addSeries(series);
  chart->addSeries(dax_series);
  chart->addSeries(sp_series);
  chart->addSeries(inflation_series2);
  chart->addSeries(inflation_series3);
  chart->createDefaultAxes();
  chart->setTitle("Portfolio performance over time");

  QChartView *chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);
  QMainWindow window;
  window.setCentralWidget(chartView);
  window.resize(400, 300);
  window.show();

  return app.exec();
}


int main(int argc, char **argv) {
  long double investment_money = 1000;
  RNGType rng;
  boost::uniform_real<> zero_to_one(0, 1);
  BuyAndHoldRandomStrategy r(investment_money, rng, zero_to_one);

  std::cout << "Starting with " << investment_money << " money." << std::endl;
  return test_trading_strategy(argc, argv, r, investment_money);
}