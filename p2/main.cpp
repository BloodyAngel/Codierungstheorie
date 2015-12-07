
#include "Polynom.hpp"
#include <iostream>
#include <fstream>
#include <exception>
#include <math.h>

//#define INDEX_OUTPUT
#define NUMBER_OUTPUT


template<int _ArraySize, int _BaseValue>
void multiplicationTest(const Polynom<_ArraySize, _BaseValue>& div, int _TestLength){
    Polynom<_ArraySize, _BaseValue>::DIVISION_POLYNOM = &div;

    // check for error
    auto generatePolynom = [](int value) -> Polynom<_ArraySize, _BaseValue>{
        Polynom<_ArraySize, _BaseValue> toReturn;
        for (int i = 0; value; ++i){
            toReturn.m_Data.at(i) = value % _BaseValue;
            value /= _BaseValue;
        }
        return toReturn;
    };

    for (int iter1 = 0; iter1 < _TestLength; ++iter1){
        std::cout << "\rProgress: " << iter1 << std::flush;
        auto poly_a = generatePolynom(iter1);

    #pragma omp parallel for
    for (int iter2 = 0; iter2 < _TestLength; ++iter2){
        auto poly_b = generatePolynom(iter2);

    for (int iter3 = 0; iter3 < _TestLength; ++iter3){
        auto poly_c = generatePolynom(iter3);
        auto abc = (poly_a * poly_b) * poly_c;
        auto bca = poly_a * (poly_b  * poly_c);
        if (abc != bca)
            throw std::runtime_error("Error: wrong implementation");
    }}}

    std::cout << "\rmultiplication found no errors" << std::endl;
}


template<int _BaseValue, int _PolySize, typename _Function>
void createTable(const Polynom<_PolySize, _BaseValue>& divisionPoly, int tableSize, _Function foo){
    auto generatePolynom = [](int value) -> Polynom<_PolySize, _BaseValue>{
        Polynom<_PolySize, _BaseValue> toReturn;
        for (int i = 0; value; ++i){
            toReturn.m_Data[i] = value % _BaseValue;
            value /= _BaseValue;
        }
        return toReturn;
    };
    Polynom<_PolySize, _BaseValue>::DIVISION_POLYNOM = &divisionPoly;

    // addition TABLE
    // create table header
    for (int iter = 0; iter < tableSize; ++iter){
        auto poly = generatePolynom(iter);
        std::cout << '\r' << std::string(iter, '\t') << '|' << poly;
    }
    std::cout << std::endl << std::string(80, '-') << std::endl;

    // table itself
    for (int iter1 = 0; iter1 < tableSize; ++iter1){

        // create table line
        std::cout << std::string(80, '-') << std::endl;

        auto poly_a = generatePolynom(iter1);

        for (int iter2 = 0; iter2 < tableSize; ++iter2){
            auto poly_b = generatePolynom(iter2);
            auto solution = foo(poly_a, poly_b);
            int val = 0;
            for (int i = 0; i < solution.m_Data.size(); ++i)
                val += pow(_BaseValue, i) * solution.m_Data[i];

#if defined(INDEX_OUTPUT)
            std::cout << '\r' << std::string(iter2, '\t') << '|' << val;
#elif defined(NUMBER_OUTPUT)
            std::cout << '\r' << std::string(iter2, '\t') << '|' << solution;
#else
#error "No output set"
#endif
        }
        std::cout << std::endl;
    }
    std::cout << "\nPress enter to continue";
    getchar();
}



int main(int argc, char** argv){
    std::cout << R"abc(Perform "(a * b) * c == a * (b * c)" test)abc" << std::endl;

    std::cout << "\nStarting test in base 2\n";
    constexpr int TABLE_SIZE = 100;
    Polynom<7, 2> testDiv_2;
    testDiv_2 = { 1, 1, 1 };
    multiplicationTest(testDiv_2, TABLE_SIZE);
    testDiv_2 = { 1, 1, 0, 1 };
    multiplicationTest(testDiv_2, TABLE_SIZE);
    testDiv_2 = { 1, 1, 0, 0, 1 };
    multiplicationTest(testDiv_2, TABLE_SIZE);

    std::cout << "\nStarting test in base 3\n";
    Polynom<5, 3> testDiv_3;
    testDiv_3 = { 1, 1, 2 };
    multiplicationTest(testDiv_3, TABLE_SIZE);

    std::cout << "\nStarting test in base 5\n";
    Polynom<5, 5> testDiv_5;
    testDiv_5 = { 1, 1, 2 };
    multiplicationTest(testDiv_5, TABLE_SIZE);
    testDiv_5 = { 1, 0, 3, 2 };
    multiplicationTest(testDiv_5, TABLE_SIZE);

    std::cout << "\nAll tests finished without fails\ncontinue with enter" << std::endl;
    getchar();


    const auto addFoo = [](const auto& p1, const auto& p2) -> decltype(p1 + p2) { return p1 + p2; };
    const auto mulFoo = [](const auto& p1, const auto& p2) -> decltype(p1 * p2) { return p1 * p2; };

    system("clear");
    Polynom<10, 2> division4 = {1, 1, 1};
    std::cout << "Addition table: F_(2²), Division poly: " << division4 << std::endl << std::endl;
    createTable(division4, 4, addFoo);

    system("clear");
    std::cout << "Multiplication table: F_(2²), Division poly: " << division4 << std::endl << std::endl;
    createTable(division4, 4, mulFoo);


    system("clear");
    Polynom<10, 2> division8 = {1, 1, 1};
    std::cout << "Addition table: F_(2³), Division poly: " << division8 << std::endl << std::endl;
    createTable(division8, 8, addFoo);

    system("clear");
    std::cout << "Multiplication table: F_(2³), Division poly: " << division8 << std::endl << std::endl;
    createTable(division8, 8, mulFoo);


    system("clear");
    Polynom<10, 3> division9 = {1, 1, 2};
    std::cout << "Addition table: F_(3²), Division poly: " << division9 << std::endl << std::endl;
    createTable(division9, 9, addFoo);

    system("clear");
    std::cout << "Multiplication table: F_(3²), Division poly: " << division9 << std::endl << std::endl;
    createTable(division9, 9, mulFoo);
    return 0;
}


/// OLD CODE
///
/**
constexpr int F22_baseValue = 2;
constexpr int F22_value = 4;
Polynom<3, 2> div22 = {1, 1, 1};
Polynom<3, 2>::DIVISION_POLYNOM = &div22;
auto generatePolynom22 = [](int value) -> Polynom<3, 2>{
    constexpr int base = 2;
    Polynom<3, 2> toReturn;
    for (int i = 0; value; ++i){
        toReturn.m_Data[i] = value % base;
        value /= base;
    }
    return toReturn;
};

// addition TABLE F4
// create table header
std::cout << "Addition table: F_(2²)" << std::endl;
for (int iter = 0; iter < F22_value; ++iter){
    auto poly = generatePolynom22(iter);
    std::cout << '\r' << std::string(iter + 1, '\t') << '|' << poly;
}
std::cout << std::endl << std::string(80, '-') << std::endl;

// table itself
for (int iter1 = 0; iter1 < F22_value; ++iter1){
    // create table line
    std::cout << std::string(80, '-') << std::endl;

    auto poly_a = generatePolynom22(iter1);
    std::cout << poly_a << "\r\t\b|";

for (int iter2 = 0; iter2 < F22_value; ++iter2){
    auto poly_b = generatePolynom22(iter2);
    auto solution = poly_a + poly_b;
    int val = 0;
    for (int i = 0; i < solution.m_Data.size(); ++i)
        val += pow(F22_baseValue, i) * solution.m_Data[i];

#if defined(INDEX_OUTPUT)
    std::cout << '\r' << std::string(iter2 + 1, '\t') << '|' << val;
#elif defined(NUMBER_OUTPUT)
    std::cout << '\r' << std::string(iter2 + 1, '\t') << '|' << solution;
#else
#error "No output set"
#endif
}
std::cout << std::endl;
}
std::cout << std::endl << std::endl << std::endl;
std::cout << "Press enter to continue";
getchar();
system("clear");


// multiplication TABLE F4
// create table header
std::cout << "Multiplication table: F_(2²)" << std::endl;
for (int iter = 0; iter < F22_value; ++iter){
    auto poly = generatePolynom22(iter);
    std::cout << '\r' << std::string(iter + 1, '\t') << '|' << poly;
}
std::cout << std::endl << std::string(80, '-') << std::endl;

// table itself
for (int iter1 = 0; iter1 < F22_value; ++iter1){

    // create table line
    std::cout << std::string(80, '-') << std::endl;

    auto poly_a = generatePolynom22(iter1);
    std::cout << poly_a << "\r\t\b|";

for (int iter2 = 0; iter2 < F22_value; ++iter2){
    auto poly_b = generatePolynom22(iter2);
    auto solution = poly_a * poly_b;
    int val = 0;
    for (int i = 0; i < solution.m_Data.size(); ++i)
        val += pow(F22_baseValue, i) * solution.m_Data[i];

#if defined(INDEX_OUTPUT)
    std::cout << '\r' << std::string(iter2 + 1, '\t') << '|' << val;
#elif defined(NUMBER_OUTPUT)
    std::cout << '\r' << std::string(iter2 + 1, '\t') << '|' << solution;
#else
#error "No output set"
#endif
}
std::cout << std::endl;
}
std::cout << std::endl << std::endl;
std::cout << "Press enter to continue";
getchar();
system("clear");


multiplicationTest();


// F_8, aka F_(2³)
constexpr int F23_baseValue = 2;
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

// addition TABLE F8
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
    auto solution = poly_a + poly_b;
    int val = 0;
    for (int i = 0; i < solution.m_Data.size(); ++i)
        val += pow(F23_baseValue, i) * solution.m_Data[i];

#if defined(INDEX_OUTPUT)
    std::cout << '\r' << std::string(iter2 + 1, '\t') << '|' << val;
#elif defined(NUMBER_OUTPUT)
    std::cout << '\r' << std::string(iter2 + 1, '\t') << '|' << solution;
#else
#error "No output set"
#endif
}
std::cout << std::endl;
}
std::cout << std::endl << std::endl << std::endl;
std::cout << "Press enter to continue";
getchar();
system("clear");


// multiplication TABLE F8
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
    auto solution = poly_a * poly_b;
    int val = 0;
    for (int i = 0; i < solution.m_Data.size(); ++i)
        val += pow(F23_baseValue, i) * solution.m_Data[i];

#if defined(INDEX_OUTPUT)
    std::cout << '\r' << std::string(iter2 + 1, '\t') << '|' << val;
#elif defined(NUMBER_OUTPUT)
    std::cout << '\r' << std::string(iter2 + 1, '\t') << '|' << solution;
#else
#error "No output set"
#endif
}
std::cout << std::endl;
}
std::cout << std::endl << std::endl;
std::cout << "Press enter to continue";
getchar();
system("clear");


// F_9, aka F_(3²)
constexpr int F32_baseValue = 3;
constexpr int F32_value = 9;
constexpr int F32_space = 11;
// check for error
auto generatePolynom32 = [](int value) -> Polynom<3, 3>{
    constexpr int base = 3;
    Polynom<3, 3> toReturn;
    for (int i = 0; value; ++i){
        toReturn.m_Data[i] = value % base;
        value /= base;
    }
    return toReturn;
};
Polynom<3, 3> div32 = { 1, 1, 2 };
Polynom<3, 3>::DIVISION_POLYNOM = &div32;

// addition TABLE F9
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

#if defined(INDEX_OUTPUT)
    int solution = 0;
    auto solution1 = poly_a + poly_b;
    for (int i = 0; i < solution1.m_Data.size(); ++i)
        solution += pow(F32_baseValue, i) * solution1.m_Data[i];
#elif defined(NUMBER_OUTPUT)
    auto solution  = poly_a + poly_b;
#else
#error "No output set"
#endif

    if (iter2 == 0)
        std::cout << '\r' << solution;
    else
        std::cout << '\r' << std::string(iter2, '\t') << '|' << solution;
}
std::cout << std::endl;
}
std::cout << std::endl << std::endl;
std::cout << "Press enter to continue";
getchar();


// multiplication TABLE F9
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
#if defined(INDEX_OUTPUT)
    int solution = 0;
    auto result = poly_a * poly_b;
    for (int i = 0; i < result.m_Data.size(); ++i)
        solution += pow(F32_baseValue, i) * result.m_Data[i];
#elif defined(NUMBER_OUTPUT)
    auto solution  = poly_a * poly_b;
#else
#error "No output set"
#endif

    if (iter2 == 0)
        std::cout << '\r' << solution;
    else
        std::cout << '\r' << std::string(iter2, '\t') << '|' << solution;
}
std::cout << std::endl;
}
std::cout << std::endl << std::endl;
std::cout << "Press enter end program";
getchar();
return 0;
*/
