#include <iostream>
#include <vector>
#include <algorithm>

void distributeApple(const std::vector<int>& appleWeights) {
    int totalAmountPaid = 50 + 30 + 20;
    std::vector<int> amountsPaid = {50, 30, 20};
    std::vector<std::string> people = {"Ram", "Sham", "Rahim"};

    std::vector<int> distributedWeights(3, 0);
    int totalWeight = 0;

    for (int weight : appleWeights) {
        int index = std::distance(amountsPaid.begin(), std::max_element(amountsPaid.begin(), amountsPaid.end()));
        distributedWeights[index] += weight;
        totalWeight += weight;
        amountsPaid[index] = 0; 
    }

    std::cout << "Distribution Result:" << std::endl;
    for (int i = 0; i < 3; ++i) {
        std::cout << people[i] << " : ";
        for (int j = 0; j < distributedWeights[i]; ++j) {
            std::cout << "100 ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::vector<int> appleWeights;
    int weight;

    std::cout << "Enter apple weight in gram (-1 to stop) : ";
    std::cin >> weight;
    while (weight != -1) {
        appleWeights.push_back(weight);
        std::cout << "Enter apple weight in gram (-1 to stop) : ";
        std::cin >> weight;
    }

    distributeApple(appleWeights);

    return 0;
}
