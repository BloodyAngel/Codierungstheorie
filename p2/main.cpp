
#include "Polynom.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char** argv){
    Polynom<2 + 1, 3> poly1 = {0, 1, 0}; //{ 1, 1, 1 };
    std::cout << "First   polynom: \r\t\t\t" << poly1 << std::endl;

    Polynom<2 + 1, 3> poly2 = {1, 1, 1};//{ 0, 0, 1 };
    std::cout << "Second  polynom: \r\t\t\t" << poly2 << std::endl;

    Polynom<2 + 1, 3> divisionPoly = { 1, 1, 1 };
    Polynom<2 + 1, 3>::DIVISION_POLYNOM = &divisionPoly;
    std::cout << "\nDivisor polynom: \r\t\t\t" << divisionPoly << std::endl;

    poly1 = poly1 * poly2;
    std::cout << std::endl << std::endl << "Poly1 * Poly2: \r\t\t\t" << poly1 << std::endl << std::endl;

    std::cout << "Press enter to continue" << std::endl;
    getchar();
    system("clear");


    // F_8, aka F_(2³)
    constexpr int F23_value = 8;
    constexpr int F23_space = 9;
    // check for error
    auto generatePolynom23 = [](int value) -> Polynom<3, 2>{
        constexpr int base = 2;
        Polynom<3, 2> toReturn;
        for (int i = 0; value; ++i){
            toReturn.m_Data[i] = value % base;
            value /= base;
        }
        return toReturn;
    };
    Polynom<3, 2> div23 = { 1, 1, 1 };
    Polynom<3, 2>::DIVISION_POLYNOM = &div23;

    for (int iter1 = 0; iter1 < F23_value; ++iter1){
        auto poly_a = generatePolynom23(iter1);

    for (int iter2 = 0; iter2 < F23_value; ++iter2){
        auto poly_b = generatePolynom23(iter2);

    for (int iter3 = 0; iter3 < F23_value; ++iter3){
        auto poly_c = generatePolynom23(iter3);
        if (((poly_a * poly_b) * poly_c) != (poly_a * (poly_b * poly_c)) )
            throw "Error: wrong implementation";
    }}}

    std::cout << "multiplication found no errors" << std::endl << "Press enter to continue";
    getchar();
    system("clear");


    // addition TABLE
    // create table header
    std::cout << "Addition table: F_(2³)" << std::endl;
    for (int iter = 0; iter < F23_value; ++iter){
        auto poly = generatePolynom23(iter);
        std::cout << '\r' << std::string(iter + 1, '\t') << '|' << poly;
    }
    std::cout << std::endl << std::string(80 - F23_space, '-') << std::endl;

    // table itself
    for (int iter1 = 0; iter1 < F23_value; ++iter1){

        // create table line
        std::cout << std::string(80 - F23_space, '-') << std::endl;

        auto poly_a = generatePolynom23(iter1);
        std::cout << poly_a << "\r\t\b|";

    for (int iter2 = 0; iter2 < F23_value; ++iter2){
        auto poly_b = generatePolynom23(iter2);

        std::cout << '\r' << std::string(iter2 + 1, '\t') << '|' << poly_a + poly_b;
    }
    std::cout << std::endl;
    }
    std::cout << std::endl << std::endl << std::endl;
    std::cout << "Press enter to continue";
    getchar();
    system("clear");


    // multiplication TABLE
    // create table header
    std::cout << "Multiplication table: F_(2³)" << std::endl;
    for (int iter = 0; iter < F23_value; ++iter){
        auto poly = generatePolynom23(iter);
        std::cout << '\r' << std::string(iter + 1, '\t') << '|' << poly;
    }
    std::cout << std::endl << std::string(80 - F23_space, '-') << std::endl;

    // table itself
    for (int iter1 = 0; iter1 < F23_value; ++iter1){

        // create table line
        std::cout << std::string(80 - F23_space, '-') << std::endl;

        auto poly_a = generatePolynom23(iter1);
        std::cout << poly_a << "\r\t\b|";

    for (int iter2 = 0; iter2 < F23_value; ++iter2){
        auto poly_b = generatePolynom23(iter2);

        std::cout << '\r' << std::string(iter2 + 1, '\t') << '|' << poly_a * poly_b;
    }
    std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
    std::cout << "Press enter to continue";
    getchar();
    system("clear");


    // F_9, aka F_(3²)
    constexpr int F32_value = 9;
    constexpr int F32_space = 11;
    // check for error
    auto generatePolynom32 = [](int value) -> Polynom<2, 3>{
        constexpr int base = 3;
        Polynom<2, 3> toReturn;
        for (int i = 0; value; ++i){
            toReturn.m_Data[i] = value % base;
            value /= base;
        }
        return toReturn;
    };
    Polynom<2, 3> div32 = { 2, 2 };
    Polynom<2, 3>::DIVISION_POLYNOM = &div32;

    // addition TABLE
    // create table header
    // this table requires more space -> ignore first column
    std::cout << "Addition table: F_(3²)" << std::endl;
    for (int iter = 0; iter < F32_value; ++iter){
        auto poly = generatePolynom32(iter);
        if (iter == 0)
            std::cout << poly;
        else
            std::cout << '\r' << std::string(iter, '\t') << '|' << poly;
    }
    std::cout << std::endl << std::string(80 - F32_space, '-') << std::endl;

    // table itself
    for (int iter1 = 0; iter1 < F32_value; ++iter1){

        // create table line
        std::cout << std::string(80 - F32_space, '-') << std::endl;

        auto poly_a = generatePolynom32(iter1);
        //std::cout << poly_a << "\r\t\b|";

    for (int iter2 = 0; iter2 < F32_value; ++iter2){
        auto poly_b = generatePolynom32(iter2);
        if (iter2 == 0)
            std::cout << '\r' << poly_a + poly_b;
        else
            std::cout << '\r' << std::string(iter2, '\t') << '|' << poly_a + poly_b;
    }
    std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
    std::cout << "Press enter to continue";
    getchar();


    // multiplication TABLE
    // create table header
    // this table requires more space -> ignore first column
    std::cout << "Multiplication table: F_(3²)" << std::endl;
    for (int iter = 0; iter < F32_value; ++iter){
        auto poly = generatePolynom32(iter);
        if (iter == 0)
            std::cout << poly;
        else
            std::cout << '\r' << std::string(iter, '\t') << '|' << poly;
    }
    std::cout << std::endl << std::string(80 - F32_space, '-') << std::endl;

    // table itself
    for (int iter1 = 0; iter1 < F32_value; ++iter1){

        // create table line
        std::cout << std::string(80 - F32_space, '-') << std::endl;

        auto poly_a = generatePolynom32(iter1);
        //std::cout << poly_a << "\r\t\b|";

    for (int iter2 = 0; iter2 < F32_value; ++iter2){
        auto poly_b = generatePolynom32(iter2);
        if (iter2 == 0)
            std::cout << '\r' << poly_a * poly_b;
        else
            std::cout << '\r' << std::string(iter2, '\t') << '|' << poly_a * poly_b;
    }
    std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
    std::cout << "Press enter end program";
    getchar();

	return 0;
}
