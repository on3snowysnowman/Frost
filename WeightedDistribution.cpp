#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <unordered_map>

int weighted_random_selection(const std::vector<int>& values, const std::vector<int>& weights) {
    // Create a cumulative distribution from the weights
    std::vector<int> cumulative_weights(weights.size());
    std::partial_sum(weights.begin(), weights.end(), cumulative_weights.begin());

    // Generate a random number between 0 and the total weight - 1
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, cumulative_weights.back() - 1);

    int random_number = dist(gen);

    // Find the index corresponding to the random number
    auto it = std::lower_bound(cumulative_weights.begin(), cumulative_weights.end(), random_number + 1); // Add 1 to handle upper boundary
    return values[it - cumulative_weights.begin()];
}



int main() {
    // Define numbers and weights
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> weights = {1, 3, 7, 14, 30, 14, 7, 3, 1};

    // Output weights
    std::cout << "Weights: ";
    for (int w : weights) {
        std::cout << w << " ";
    }
    std::cout << std::endl;

    // Run the simulation
    std::unordered_map<int, int> generated_values;
    const int trials = 10000;

    for (int i = 0; i < trials; ++i) {
        ++generated_values[weighted_random_selection(numbers, weights)];
    }

    // Output the distribution
    std::cout << "Distribution (out of " << trials << " samples):\n";
    for (size_t i = 0; i < numbers.size(); ++i) {
        int number = numbers[i];
        int count = generated_values[number];
        double percentage = (static_cast<double>(count) / trials) * 100;
        std::cout << number << ": " << count << " -> " << percentage << "%\n";
    }

    return 0;
}
