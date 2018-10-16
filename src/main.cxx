#include <cstdlib>
#include <boost/program_options.hpp>
//#include <QApplication>
//#include <QPushButton>
#include "app.h"
#include "Coordinator.h"
#include "RandomStrategy.h"

namespace po = boost::program_options;
std::unordered_map<std::string, StockData> Coordinator::stocks;

po::options_description getDescription() {
    
    po::options_description desc("Options");
    desc.add_options()
        ("help,h",      "Display this message")
        ("version,v",   "Display the version number");

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

void start() {
    std::string RESOURCES_PATH = "/home/jan/StockAnalyzer/resources/";
    long double investment_money = 50000;
    std::cout << "Starting with " << investment_money << " money." << std::endl;
    Coordinator::read_stock_data(RESOURCES_PATH);
    RNGType rng;
    boost::uniform_real<> zero_to_one(0, 1);

    RandomStrategy r(investment_money, rng, zero_to_one);
    Strategy* cur_strat = &r;
    
    std::string first_date_string("19500103");
    boost::gregorian::date first_date(boost::gregorian::from_undelimited_string(first_date_string));
    boost::gregorian::date current_date(boost::gregorian::from_undelimited_string(first_date_string));
    std::string last_date_string("20180827");
    boost::gregorian::date last_date(boost::gregorian::from_undelimited_string(last_date_string));

    std::cout << "Started on " <<  boost::gregorian::to_simple_string(first_date);
    while(current_date <= last_date) {
        (*cur_strat).rebalance(current_date);

        do {
            boost::gregorian::days next_day(1);
            current_date = current_date + next_day;
        } while(!Coordinator::date_valid(current_date) && current_date <= last_date);
    }

    std::cout << "\nEnded up with " << (*cur_strat).get_current_networth(last_date) << " on " << boost::gregorian::to_simple_string(last_date) << std::endl;
}


int main(int argc, char **argv) {
    start();
/*    QApplication app (argc, argv);

    QPushButton button ("Hello world !");
    button.show();

    return app.exec(); */
    return 0;
}