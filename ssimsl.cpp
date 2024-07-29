#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <numeric>

// Function to read image using stb_image
std::vector<double> read_image(const std::string& filename, int& width, int& height, int& channels, int& bit_depth) {
    // Check the bit depth of the image
    unsigned char* data_8bit = stbi_load(filename.c_str(), &width, &height, &channels, 1); // Force to load as grayscale
    if (data_8bit) {
        bit_depth = 8;
        int size = width * height;
        std::vector<double> img_data(size);
        for (int i = 0; i < size; ++i) {
            img_data[i] = static_cast<double>(data_8bit[i]); // Load pixel values as they are
        }
        stbi_image_free(data_8bit);
        return img_data;
    }

    // If the 8-bit load fails, try 16-bit load
    unsigned short* data_16bit = stbi_load_16(filename.c_str(), &width, &height, &channels, 1); // Force to load as grayscale
    if (data_16bit) {
        bit_depth = 16;
        int size = width * height;
        std::vector<double> img_data(size);
        for (int i = 0; i < size; ++i) {
            img_data[i] = static_cast<double>(data_16bit[i]); // Load pixel values as they are
        }
        stbi_image_free(data_16bit);
        return img_data;
    }

    // If neither load works, throw an error
    throw std::runtime_error("Cannot open or find the image: " + filename);
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
    const double L = 255.0; // Since pixel values are in the range [0, 255]
    const double C1 = (K1 * L) * (K1 * L);
    const double C2 = (K2 * L) * (K2 * L);

    double mean1 = mean(img1);
    double mean2 = mean(img2);
    double var1 = variance(img1, mean1);
    double var2 = variance(img2, mean2);
    double covar = covariance(img1, img2, mean1, mean2);

    /*
    // Debug prints
    std::cout << "mean1: " << mean1 << ", mean2: " << mean2 << std::endl;
    std::cout << "var1: " << var1 << ", var2: " << var2 << std::endl;
    std::cout << "covar: " << covar << std::endl;
    */

    double numerator = (2 * mean1 * mean2 + C1) * (2 * covar + C2);
    double denominator = (mean1 * mean1 + mean2 * mean2 + C1) * (var1 + var2 + C2);

    return numerator / denominator;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <image1.jpg> <image2.jpg>" << std::endl;
        return 1;
    }

    try {
        int width1, height1, channels1, bit_depth1;
        int width2, height2, channels2, bit_depth2;

        std::vector<double> img1 = read_image(argv[1], width1, height1, channels1, bit_depth1);
        std::vector<double> img2 = read_image(argv[2], width2, height2, channels2, bit_depth2);

        // Print bit depth of the images
        std::cout << "Bit depth of image 1: " << bit_depth1 << std::endl;
        std::cout << "Bit depth of image 2: " << bit_depth2 << std::endl;

        if (width1 != width2 || height1 != height2) {
            throw std::runtime_error("Images must have the same dimensions");
        }

        double result = ssim(img1, img2);

        std::cout << "SSIM: " << result << std::endl;

        // Add your custom code here
        if (result > 0.9) {
            std::cout << "The images are very similar!" << std::endl;
        } else {
            std::cout << "The images are not very similar." << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
