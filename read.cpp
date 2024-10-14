#include <iostream>
#include <fstream>
#include <vector>

// Function to read a .fvecs file
std::vector<std::vector<float>> read_fvecs(const std::string& filename) {
    std::vector<std::vector<float>> vectors;

    // Open the file in binary mode
    std::ifstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
        return vectors; // Return empty if file can't be opened
    }

    while (!file.eof()) {
        int dim;

        // Read the dimension (an int)
        file.read(reinterpret_cast<char*>(&dim), sizeof(int));
        if (file.eof()) break; // End of file check after reading the dimension

        std::vector<float> vec(dim);

        // Read the float values
        file.read(reinterpret_cast<char*>(vec.data()), dim * sizeof(float));

        if (!file.eof()) {
            vectors.push_back(vec); // Add the vector to the list
        }
    }

    file.close();
    return vectors;
}

int main() {
    std::string filename = "siftsmall/siftsmall_base.fvecs"; // Path to the .fvecs file

    // Read the vectors from the file
    std::vector<std::vector<float>> vectors = read_fvecs(filename);

    // Output the read vectors (for demonstration)
    for (const auto& vec : vectors) {
        std::cout << "Vector (dim " << vec.size() << "): ";
        for (float v : vec) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
