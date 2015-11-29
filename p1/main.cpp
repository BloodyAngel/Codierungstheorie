




#include "BinaryHuffmanTree.hpp"



int main(int argc, char** argv){
    std::string inputString1 = "Hello Coding!";
    if (argc > 1)
        inputString1 = argv[1];
    std::vector<int> inputString2 = { 0, 0, 0, 0, 1, 1, 1, 2, 2, 3};

    std::string outputString1;
    std::vector<int> outputString2;

    typedef char MY_TYPE;
    const auto& inputString = inputString1;
    auto& outputString = outputString1;

    // generate header/coded file
    {
        std::cout<< "Creating (Binary-)Huffman tree\n\n";
        BinaryHuffmanTree<MY_TYPE> tree = BinaryHuffmanTree<>::generateHuffmanTree(inputString);
        tree.printLeaves();
        std::cout << "\nEntropie: " << tree.calculateEntropie()
                  << std::endl
                  << std::endl;

        std::cout << "\n\nwriting header file\n";
        tree.writeHeaderToFile("header.txt");

        std::cout << "\nstart coding string\n";
        tree.saveCodeString("coding.txt", inputString);
    }

    // read header and regenerate coded file
    {
        std::cout << "\nreading header file\n";
        BinaryHuffmanTree<MY_TYPE> tree1 = BinaryHuffmanTree<MY_TYPE>::getHuffmanTreeFromHeader("header.txt");

        tree1.printLeaves();
        std::cout << "\nparsing code\n";
        tree1.parseCode("coding.txt", outputString);
    }

    std::cout << "encoded text:\n\n";
    for (auto& e: outputString)
        std::cout << e;
    std::cout << std::endl << std::endl;
    return 0;
}

