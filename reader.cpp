#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>

// Function to read a 16-bit binary image file
std::vector<unsigned short> read_16bit_binary_image(const std::string& filename, int& width, int& height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open or find the file: " + filename);
    }

    // Read the width and height (assuming they are stored in the file header)
    file.read(reinterpret_cast<char*>(&width), sizeof(int));
    file.read(reinterpret_cast<char*>(&height), sizeof(int));

    if (width <= 0 || height <= 0) {
        throw std::runtime_error("Invalid width or height in file: " + filename);
    }

    int size = width * height;
    std::vector<unsigned short> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size * sizeof(unsigned short));

    if (!file) {
        throw std::runtime_error("Error reading the file: " + filename);
    }

    return buffer;
}

// Function to write the 16-bit image data to a PNG file
void write_image_to_png(const std::string& filename, const std::vector<unsigned short>& image, int width, int height) {
    // Convert to 8-bit for saving
    std::vector<unsigned char> image_8bit(width * height);
    for (int i = 0; i < width * height; ++i) {
        image_8bit[i] = static_cast<unsigned char>(image[i] / 256); // Convert 16-bit to 8-bit
    }

    if (!stbi_write_png(filename.c_str(), width, height, 1, image_8bit.data(), width)) {
        throw std::runtime_error("Error writing the PNG file: " + filename);
    }
}

int main() {
    try {
        int width, height;
        std::string input_filename = "path/to/your/16bit_binary_image.bin"; // Replace with your binary file path
        std::string output_filename = "output_image.png"; // Desired output PNG file path

        std::vector<unsigned short> image = read_16bit_binary_image(input_filename, width, height);
        write_image_to_png(output_filename, image, width, height);

        std::cout << "Image successfully written to " << output_filename << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
