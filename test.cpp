#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>

// Function to calculate mean of a vector
double mean(const std::vector<double>& vec) {
    double sum = std::accumulate(vec.begin(), vec.end(), 0.0);
    return sum / vec.size();
}

// Function to calculate variance of a vector
double variance(const std::vector<double>& vec, double mean) {
    double var = 0.0;
    for (const auto& v : vec) {
        var += (v - mean) * (v - mean);
    }
    return var / vec.size();
}

// Function to calculate covariance of two vectors
double covariance(const std::vector<double>& vec1, const std::vector<double>& vec2, double mean1, double mean2) {
    double covar = 0.0;
    for (size_t i = 0; i < vec1.size(); ++i) {
        covar += (vec1[i] - mean1) * (vec2[i] - mean2);
    }
    return covar / vec1.size();
}

// Function to calculate SSIM
double ssim(const std::vector<double>& img1, const std::vector<double>& img2) {
    if (img1.size() != img2.size()) {
        throw std::invalid_argument("Images must have the same size");
    }

    const double K1 = 0.01;
    const double K2 = 0.03;
    const double L = 255.0;
    const double C1 = (K1 * L) * (K1 * L);
    const double C2 = (K2 * L) * (K2 * L);

    double mean1 = mean(img1);
    double mean2 = mean(img2);
    double var1 = variance(img1, mean1);
    double var2 = variance(img2, mean2);
    double covar = covariance(img1, img2, mean1, mean2);

    double numerator = (2 * mean1 * mean2 + C1) * (2 * covar + C2);
    double denominator = (mean1 * mean1 + mean2 * mean2 + C1) * (var1 + var2 + C2);

    return numerator / denominator;
}

int main() {
    // Example usage
    std::vector<double> img1 = { /* Pixel values of the first image */ };
    std::vector<double> img2 = { /* Pixel values of the second image */ };

    try {
        double result = ssim(img1, img2);
        std::cout << "SSIM: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
