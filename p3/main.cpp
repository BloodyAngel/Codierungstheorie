
#include "matrix.hpp"
#include "nebenklassen.hpp"

#include <iostream>

#ifdef __cpp_constexpr
#define CONSTEXPRESSION constexpr
#else
#define CONSTEXPRESSION
#endif


namespace CONSTEXPR{
constexpr int pow(int base, int power){
    int toReturn = 1;
    for (int i = 0; i < power; ++i)
        toReturn *= base;
    return toReturn;
}
}

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}



int main(){
    const Color::Modifier red(Color::Code::FG_RED);
    const Color::Modifier blue(Color::Code::FG_BLUE);
    const Color::Modifier green(Color::Code::FG_GREEN);
    const Color::Modifier _default(Color::Code::FG_DEFAULT);

    CodingMatrix<int, 3, 4, 5> m1 = {
        std::array<int, 5>{1, 1, 1, 2, 1},
        std::array<int, 5>{1, 2, 2, 1, 2},
        std::array<int, 5>{1, 1, 0, 1, 0},
        std::array<int, 5>{2, 0, 1, 1, 0},
    };
    std::cout << "input matrix: \n" << m1 << std::endl << std::endl;
    m1.solveGauss();
    std::cout << "gaussian solved matrix: \n" << m1 << std::flush;
    getchar();
    system("clear");


    CodingMatrix<int, 5, 3, 5> m2 = {
        std::array<int, 5>{1, 0, 0, 4, 1},
        std::array<int, 5>{0, 1, 0, 1, 1},
        std::array<int, 5>{0, 0, 1, 0, 2}
    };
    auto control = m2.generateControllMatrix();
    auto transpose = control.transpose();

    m2.truncateToBase();
    std::cout << "Generator matrix:\n\n" << m2      << std::endl << std::endl
              << "Control   matrix:\n\n" << control << std::flush;
    getchar();
    system("clear");

// EXAMPLE F2
    std::cout << "Example F_2" << std::endl;
    constexpr int base_f2 = 2;
    constexpr int rows_f2 = 2;
    constexpr int cols_f2 = 6;
    constexpr int syndromSize_f2 = cols_f2 - rows_f2;
    constexpr int numElements_f2 = CONSTEXPR::pow(base_f2, cols_f2);
    CodingMatrix<int, base_f2, rows_f2, cols_f2> generator_f2 = {
        std::array<int, cols_f2>{1, 0, 1, 0, 1, 0},
        std::array<int, cols_f2>{0, 1, 0, 1, 0, 1}
    };
    generator_f2.solveGauss();
    auto controll_f2 = generator_f2.generateControllMatrix();

    std::cout << "generator matrix:\n" << generator_f2 << std::endl << "controll matrix:\n" << controll_f2 << std::endl;
    NebenKlasse<int, base_f2, syndromSize_f2, cols_f2> nk_f2 = NebenKlasse<int, base_f2, syndromSize_f2, cols_f2>(controll_f2);

    for (int i = 0; i < numElements_f2; ++i){
        Vector<int, base_f2, cols_f2> received;
        for (int tmp = i, iter = 0; tmp && iter < cols_f2; tmp /= base_f2, ++iter)
            received[iter] = tmp % base_f2;

        auto syndrom = controll_f2 * received;
        std::cout << received << "\tSyndrom: " << syndrom << std::endl;

        Vector<int, base_f2, cols_f2> result = received;
        bool confidence = nk_f2.getMaximumLikely(result);

        std::cout << blue << result << _default << "\tmax likeley" << '\t' << (confidence ? green : red) << (confidence ? "correct result" : "possibly wrong result") << _default
                  << std::endl << std::endl;
    }
    getchar();
    system("clear");




// EXAMPLE F3
    std::cout << "Example F_3" << std::endl;
    constexpr int base_f3 = 3;
    constexpr int rows_f3 = 2;
    constexpr int cols_f3 = 3;
    constexpr int syndromSize_f3 = cols_f3 - rows_f3;
    constexpr int numElements_f3 = CONSTEXPR::pow(base_f3, cols_f3);
    CodingMatrix<int, base_f3, rows_f3, cols_f3> generator_f3 = {
        std::array<int, cols_f3>{1, 1, 1},
        std::array<int, cols_f3>{2, 2, 1}
    };
    generator_f3.solveGauss();
    auto controll_f3 = generator_f3.generateControllMatrix();

    std::cout << "generator matrix:\n" << generator_f3 << std::endl << "controll matrix:\n" << controll_f3 << std::endl;
    NebenKlasse<int, base_f3, syndromSize_f3, cols_f3> nk_f3 = NebenKlasse<int, base_f3, syndromSize_f3, cols_f3>(controll_f3);

    for (int i = 0; i < numElements_f3; ++i){
        Vector<int, base_f3, cols_f3> received;
        for (int tmp = i, iter = 0; tmp && iter < cols_f3; tmp /= base_f3, ++iter)
            received[iter] = tmp % base_f3;

        auto syndrom = controll_f3 * received;
        std::cout << received << "\tSyndrom: " << syndrom << std::endl;

        Vector<int, base_f3, cols_f3> result = received;
        bool confidence = nk_f3.getMaximumLikely(result);

        std::cout << blue << result << _default << "\tmax likeley" << '\t' << (confidence ? green : red) << (confidence ? "correct result" : "possibly wrong result")
                  << _default << std::endl << std::endl;
    }
    getchar();
    system("clear");



// EXAMPLE F5
    std::cout << "Example F_5" << std::endl;
    constexpr int base_f5 = 5;
    constexpr int rows_f5 = 1;
    constexpr int cols_f5 = 3;
    constexpr int syndromSize_f5 = cols_f5 - rows_f5;
    constexpr int numElements_f5 = CONSTEXPR::pow(base_f5, cols_f5);
    CodingMatrix<int, base_f5, rows_f5, cols_f5> generator_f5 = {
        std::array<int, cols_f5>{1, 2, 3},
    };
    generator_f5.solveGauss();
    auto controll_f5 = generator_f5.generateControllMatrix();

    std::cout << "generator matrix:\n" << generator_f5 << std::endl << "controll matrix:\n" << controll_f5 << std::endl;
    NebenKlasse<int, base_f5, syndromSize_f5, cols_f5> nk_f5 = NebenKlasse<int, base_f5, syndromSize_f5, cols_f5>(controll_f5);

    for (int i = 0; i < numElements_f5; ++i){
        Vector<int, base_f5, cols_f5> received;
        for (int tmp = i, iter = 0; tmp && iter < cols_f5; tmp /= base_f5, ++iter)
            received[iter] = tmp % base_f5;

        auto syndrom = controll_f5 * received;
        std::cout << received << "\tSyndrom: " << syndrom << std::endl;

        Vector<int, base_f5, cols_f5> result = received;
        bool confidence = nk_f5.getMaximumLikely(result);

        std::cout << blue << result << _default << "\tmax likeley" << '\t' << (confidence ? green : red) << (confidence ? "correct result" : "possibly wrong result")
                  << _default << std::endl << std::endl;
    }

	return 0;
}
