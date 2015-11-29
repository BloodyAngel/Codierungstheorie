
#include "Polynom.hpp"
#include <iostream>

int main(int argc, char** argv){
    Polynom<2 + 1, 3> poly1 = { 1, 2, 2 };
    std::cout << "First   polynom: \r\t\t\t" << poly1 << std::endl;

    Polynom<2 + 1, 3> poly2 = { 0, 0, 1 };
    std::cout << "Second  polynom: \r\t\t\t" << poly2 << std::endl;

    Polynom<2 + 1, 3> divisionPoly = { 1, 1, 1 };
    Polynom<2 + 1, 3>::DIVISION_POLYNOM = &divisionPoly;
    std::cout << "\nDivisor polynom: \r\t\t\t" << divisionPoly << std::endl;

    poly1 = poly1 * poly2;
    std::cout << std::endl << std::endl << "Poly1 * Poly2: \r\t\t\t" << poly1 << std::endl << std::endl;
	return 0;
}

