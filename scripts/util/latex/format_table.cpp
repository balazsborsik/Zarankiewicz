#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib> // For std::stoi

void writeZeroRow(std::ofstream& out, int width) {
    for (int i = 0; i < width; ++i) {
        out << "0 ";
    }
    out << "\n";
}

int main(int argc, char* argv[]) {
    // 1. Validate Command Line Arguments
    // Expecting: [program] [input_number] [filename] [top_padding]
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <input_number> <filename> <top_padding>" << std::endl;
        std::cerr << "Example: " << argv[0] << " 5 matrix.txt 2" << std::endl;
        return 1;
    }

    int inputNumber;
    std::string filename;
    int topPadding;

    // 2. Parse Parameters
    try {
        inputNumber = std::stoi(argv[1]);
        filename = argv[2];
        topPadding = std::stoi(argv[3]);
    } catch (const std::exception& e) {
        std::cerr << "Error: Parameters must be valid types (int string int)." << std::endl;
        return 1;
    }

    if (inputNumber > 39 || inputNumber < 0) {
        std::cerr << "Error: Input number must be between 0 and 39." << std::endl;
        return 1;
    }

    // 3. Open Files
    std::ifstream inputFile(filename);
    std::ofstream outputFile("output.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return 1;
    }

    std::string line;
    bool isFirstRow = true;
    int totalRowsWritten = 0;
    int matrixWidth = 0; // Will be calculated based on the first line of data

    // 4. Process File
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::vector<int> rowData;
        int value;

        while (ss >> value) {
            rowData.push_back(value);
        }

        if (rowData.empty()) continue;

        // --- FIRST PASS SETUP ---
        if (isFirstRow) {
            int dataSize = rowData.size();
            
            // Calculate total width to keep matrix rectangular
            // Width = (Front Zeros) + (Data Columns) + (Back Zeros)
            // Width = (inputNumber) + (dataSize) + (39 - inputNumber)
            // This simplifies to: 39 + dataSize

            // A. Execute Top Padding (Write 'topPadding' number of zero rows)
            for (int i = 0; i < topPadding; ++i) {
                writeZeroRow(outputFile, 39);
                totalRowsWritten++;
            }
            
            isFirstRow = false;
        }

        // --- WRITE DATA ROW ---
        
        // 1. Front Zeros
        for (int i = 0; i < inputNumber; ++i) {
            outputFile << "0 ";
        }

        // 2. Data
        for (size_t i = 0; i < rowData.size(); ++i) {
            outputFile << rowData[i] << " ";
        }

        // 3. Back Zeros (39 - inputNumber)
        int backZeros = 39 - inputNumber - rowData.size();
        for (int i = 0; i < backZeros; ++i) {
            outputFile << "0 "; 
        }
        
        outputFile << "\n";
        totalRowsWritten++;
    }

    // 5. Execute Bottom Padding
    // Fill with zero rows until we reach 39 total rows
    while (totalRowsWritten < 39) {
        // Use the calculated width so the matrix stays rectangular
        // If the file was empty, we default to 39 zeros
        writeZeroRow(outputFile, 39);
        totalRowsWritten++;
    }

    std::cout << "Success! Written " << totalRowsWritten << " rows to 'output.txt'." << std::endl;

    inputFile.close();
    outputFile.close();

    std::ifstream infile("output.txt");
    std::string line2;
    std::ofstream ofs(filename, std::ofstream::trunc);
    while (std::getline(infile, line2))
    {
        ofs << line2 << '\n';
    }
    infile.close();
    ofs.close();
    return 0;
}