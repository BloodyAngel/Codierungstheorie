
#ifndef _MATRIX_HPP_HPP_
#define _MATRIX_HPP_HPP_

#include <array>
#include <iostream>
#include "vector.hpp"

template<typename _ValueType, int _Base, int _Rows, int _Cols>
class CodingMatrix{
    typedef CodingMatrix<_ValueType, _Base, _Rows, _Cols> MY_TYPE;
public:
    CodingMatrix() {
        for (auto& array : this->m_Data){
            for (auto& e : array)
                e = 0.0;
        }
    }
    CodingMatrix(std::initializer_list<std::array<_ValueType, _Cols>> list){
        if (list.size() != _Rows)
            throw std::runtime_error("Error in function: 'CodingMatrix(std::initializer_list<std::array<_ValueType, _Cols>>)', list contains invalid number of arguments");

        int iter = 0;
        for (auto& e : list)
            this->m_Data[iter++] = e;
    }
    ~CodingMatrix() = default;

    static constexpr int ROWS(){ return _Rows; }
    static constexpr int COLS(){ return _Cols; }
    constexpr Vector<_ValueType, _Base, _Cols>& operator[] (int row){ return this->m_Data[row]; }

    CodingMatrix(const CodingMatrix& m) = default;
    CodingMatrix& operator= (const CodingMatrix& m) = default;

    CodingMatrix<_ValueType, _Base, _Cols, _Rows> transpose() {
        CodingMatrix<_ValueType, _Base, _Cols, _Rows> toReturn;
        MY_TYPE& THIS = *this;
        for (int row = 0; row < _Rows; ++row){
        for (int col = 0; col < _Cols; ++col){
            toReturn[col][row] = THIS[row][col];
        }
        }
        return toReturn;
    }

    friend std::ostream& operator<<(std::ostream& os, const MY_TYPE& rhs){
        for(auto& e : rhs.m_Data)
            os << e << std::endl;
        return os;
    }


    template<int _RhsRows, int _RhsCols>
    CodingMatrix<_ValueType, _Base, _RhsRows, _Cols> operator* (const CodingMatrix<_ValueType, _Base, _RhsRows, _RhsCols>& rhs){
        typedef CodingMatrix<_ValueType, _Base, _RhsRows, _Cols>  RETURN_TYPE;
        RETURN_TYPE toReturn;
        for (int row = 0; row < RETURN_TYPE::ROWS(); ++row){
        for (int col = 0; col < RETURN_TYPE::COLS(); ++col){
            for (int i = 0; i < _Cols; ++i)
                toReturn[row][col] += this->m_Data[row][i] + rhs.m_Data[i][col];
        }
        }
        toReturn.truncateToBase();
        return toReturn;
    }

    // assumption 1: matrix has always more cols than rows!
    // assumption 2: matrix is in kanonian representation and in form: ("message in front")
    //      / 1  a1_1 a1_2 ... ...  \
    //      | 0   1   a2_1 ... ...  |
    //      \ 0   0    1   ... ...  /
    CodingMatrix<_ValueType, _Base, _Cols - _Rows, _Cols> generateControllMatrix(){
        CodingMatrix<_ValueType, _Base, _Cols - _Rows, _Cols> toReturn;
        for (int col = _Rows; col < _Cols; ++col){
            // write current row into matrix
            for (int row = 0; row < _Rows; ++row)
                toReturn[col - _Rows][row] = this->m_Data[row][col];
        }
        for (int i = 0; i < _Cols - _Rows; ++i)
            toReturn[i][i + _Rows] = -1;

        toReturn.truncateToBase();
        return toReturn;
    }


    void solveAdjustedGauss(){
        // actual solve gauss
        // adjust diagonal to have a non 0 value
        for (int col = 0; col < _Cols && col < _Rows; ++col){
            // check for row of only zeros (-> linear dependent row)
            bool allZeros = true;
            for (int i = 0; i < _Cols; ++i){
                if (this->m_Data[col][i]){
                    allZeros = false;
                    break;
                }
            }
            if (allZeros){
                std::cout << *this;
                throw std::runtime_error("Error: rows are not linear indipendent, reached column: " + std::to_string(col));
            }

            for (int i = col + 1; !this->m_Data[col][col] && i < _Rows; ++i){
                if (i >= _Rows)
                    throw std::runtime_error("Error: failed to solve gaussian");

                if (this->m_Data[i][col]){
                    std::swap(this->m_Data[col], this->m_Data[i]);
                    break;
                }
            }

            // diagonal value has to be 1
            // (m_Data[col][col] = 1)
            /// possible to swap cols to have at a non zero value at 'm_Data[col][col]'
            if (this->m_Data[col][col] == 0){
                bool swaped = false;
                for (int i = col + 1; i < _Cols; ++i){
                    if (this->m_Data[col /* row */][i]){
                        std::cout << "\nswaping column " << col << " and " << i << std::endl;
                        for (int row = 0; row < _Rows; ++row)
                            std::swap(this->m_Data[row][col], this->m_Data[row][i]);

                        swaped = true;
                        break;
                    }
                }
                if (!swaped)
                    throw std::runtime_error("provided matrix rows aren't linear independent");
            }

            Vector<_ValueType, _Base, _Cols> tmp = this->m_Data[col];
            for (int i = 0; i < _Base &&  this->m_Data[col][col] != 1; ++i){
                this->m_Data[col] += tmp;
                this->m_Data[col].truncateToBase();
            }

            if (this->m_Data[col][col] != 1)
                throw std::runtime_error("Error in function: 'solveGauss', cannot generate diagonal, reached column: " + std::to_string(col));


            /// all values below Matrix[col][col] has to be 0
            // => row = col + 1, no need to change the others
            //for (int row = col + 1; row < _Rows; ++row){
            for (int row = 0; row < _Rows; ++row){
                if (!this->m_Data[row][col] || row == col)
                    continue; // already 0

                _ValueType factor = _Base - this->m_Data[row][col];
                this->m_Data[row] += this->m_Data[col] * factor; // NOTE: full addition isn't neseccary (a lot of 0), might be enhanced :)
                this->m_Data[row].truncateToBase();
            }
        }
        this->truncateToBase();
    }

    void truncateToBase(){
        for (auto& e : this->m_Data)
            e.truncateToBase();
    }

    Vector<_ValueType, _Base, _Rows> operator* (const Vector<_ValueType, _Base, _Cols>& rhs) {
        Vector<_ValueType, _Base, _Rows> toReturn;
        for (int i = 0; i < _Rows; ++i){
            toReturn.m_Data[i] = 0;
            for (int k = 0; k < _Cols; ++k)
                toReturn.m_Data[i] += rhs.m_Data[k] * this->m_Data[i][k];
        }
        toReturn.truncateToBase ();
        return toReturn;
    }

private:
    std::array<Vector<_ValueType, _Base, _Cols>, _Rows> m_Data;
};


#endif

