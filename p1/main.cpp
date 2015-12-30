



#include "BinaryHuffmanTree.hpp"

#define USE_CHAR_TYPE
//#define USE_INT_TYPE


int main(int argc, char** argv){
    std::string inputString1 = "aabc";
    if (argc > 1)
        inputString1 = argv[1];
    std::vector<int> inputString2 = { 0, 0, 7, 0, 1, 1, 1, 2, 2, 3 };

    std::string outputString1;
    std::vector<int> outputString2;

#if defined(USE_CHAR_TYPE) && defined(USE_INT_TYPE)
    #error "Please define either USE_CHAR_TYPE or USE_INT_TYPE, and not both!"
#elif defined(USE_CHAR_TYPE)
    typedef char USED_TYPE;
    const auto& inputString = inputString1;
    auto& outputString = outputString1;
#elif defined(USE_INT_TYPE)
    typedef int USED_TYPE;
    const auto& inputString = inputString2;
    auto& outputString = outputString2;
#else
    #error "Please define either USE_CHAR_TYPE or USE_INT_TYPE"
#endif
    // generate header/coded file
    {
        std::cout<< "Creating (Binary-)Huffman tree\n\n";
        BinaryHuffmanTree<USED_TYPE> tree = BinaryHuffmanTree<>::generateHuffmanTree(inputString);
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
        BinaryHuffmanTree<USED_TYPE> tree = BinaryHuffmanTree<USED_TYPE>::getHuffmanTreeFromHeader("header.txt");

        tree.printLeaves();
        std::cout << "\nparsing code\n";
        tree.parseCode("coding.txt", outputString);
    }

    std::cout << "encoded text:\n\n";
    for (auto& e: outputString)
        std::cout << e;
    std::cout << std::endl << std::endl;
    return 0;
}

