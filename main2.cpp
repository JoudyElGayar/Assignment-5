#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>
#include <filesystem>

struct ExchangeRateChange {
    std::string date;
    double change;

    ExchangeRateChange(const std::string& d, double c) : date(d), change(c) {}
};

struct MaxHeapComparator {
    bool operator()(const ExchangeRateChange& a, const ExchangeRateChange& b) {
        return a.change < b.change;
    }
};

struct MinHeapComparator {
    bool operator()(const ExchangeRateChange& a, const ExchangeRateChange& b) {
        return a.change > b.change;
    }
};

std::vector<double> readExchangeRatesFromCSV(const std::string& filename) {
    std::vector<double> exchangeRates;
    std::ifstream inputFile(filename);

    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            std::string value;
            while (std::getline(iss, value, ',')) {
                exchangeRates.push_back(std::stod(value));
            }
        }
        inputFile.close();
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }

    return exchangeRates;
}

int main() {
    std::vector<double> exchangeRates = readExchangeRatesFromCSV("/Users/joudy/Desktop/euro-dollar.csv");

    double averageRate = std::accumulate(exchangeRates.begin(), exchangeRates.end(), 0.0) / exchangeRates.size();

    const size_t N = 10;
    
    std::priority_queue<ExchangeRateChange, std::vector<ExchangeRateChange>, MaxHeapComparator> maxHeap;
    std::priority_queue<ExchangeRateChange, std::vector<ExchangeRateChange>, MinHeapComparator> minHeap;

    for (size_t i = 0; i < exchangeRates.size(); ++i) {
        double change = exchangeRates[i] - averageRate;
        ExchangeRateChange data(std::to_string(i + 1), change);

        maxHeap.push(data);
        minHeap.push(data);

        if (maxHeap.size() > N) {
            maxHeap.pop();
        }
        if (minHeap.size() > N) {
            minHeap.pop();
        }
    }

    std::cout << "Dates for N most positive changes:" << std::endl;
    while (!maxHeap.empty()) {
        std::cout << maxHeap.top().date << std::endl;
        maxHeap.pop();
    }

    std::cout << "Dates for N most negative changes:" << std::endl;
    while (!minHeap.empty()) {
        std::cout << minHeap.top().date << std::endl;
        minHeap.pop();
    }

    return 0;
}
