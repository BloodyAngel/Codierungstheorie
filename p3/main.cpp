
#include "matrix.hpp"

#include <iostream>

int main(int argc, char** argv){
    CodingMatrix<int, 3, 4, 5> m1 = {
        std::array<int, 5>{1, 1, 1, 2, 1},
        std::array<int, 5>{1, 2, 2, 1, 2},
        std::array<int, 5>{1, 1, 0, 1, 0},
        std::array<int, 5>{2, 0, 1, 0, 1},
    };

    std::cout << "input matrix: \n" << m1 << std::endl << std::endl;
    m1.solveGauss();
    std::cout << "gaussian solved matrix: \n" << m1 << std::endl;


    CodingMatrix<int, 5, 3, 5> m2 = {
        std::array<int, 5>{1, 0, 0, 4, 1},
        std::array<int, 5>{0, 1, 0, 1, 1},
        std::array<int, 5>{0, 0, 1, 0, 2}
    };
    auto controll = m2.generateControllMatrix();
    m2.truncateToBase();
    std::cout << std::endl << std::endl << "Generator matrix:\n\n" << m2        << std::endl << std::endl
                                        << "Controll  matrix:\n\n" << controll  << std::endl << std::endl;


	return 0;
}

