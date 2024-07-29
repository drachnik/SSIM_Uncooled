#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <numeric>

// Function to read PGM image
std::vector<double> read_pgm(const std::string& filename, int& width, int& height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    std::getline(file, line);
    if (line != "P5") {
        throw std::runtime_error("Invalid PGM file format: " + filename);
    }

    // Skip comments
    do {
        std::getline(file, line);
    } while (line[0] == '#');

    std::istringstream ss(line);
    ss >> width >> height;

    int max_val;
    file >> max_val;
    file.ignore(1); // Skip the newline character after max_val

    std::vector<double> data(width * height);
    for (int i = 0; i < width * height; ++i) {
        data[i] = static_cast<unsigned char>(file.get());
    }

    return data;
}

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

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <image1.pgm> <image2.pgm>" << std::endl;
        return 1;
    }

    try {
        int width1, height1, width2, height2;
        std::vector<double> img1 = read_pgm(argv[1], width1, height1);
        std::vector<double> img2 = read_pgm(argv[2], width2, height2);

        if (width1 != width2 || height1 != height2) {
            throw std::runtime_error("Images must have the same dimensions");
        }

        double result = ssim(img1, img2);
        std::cout << "SSIM: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
