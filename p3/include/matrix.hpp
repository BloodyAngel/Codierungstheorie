
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
    };
    CodingMatrix(std::initializer_list<std::array<_ValueType, _Cols>> list){
        if (list.size() != _Rows)
            throw std::runtime_error("Error in function: 'CodingMatrix(std::initializer_list<std::array<_ValueType, _Cols>>)', list contains invalid number of arguments");

        int iter = 0;
        for (auto& e : list)
            this->m_Data[iter++] = e;
    }

    ~CodingMatrix()= default;

    static constexpr int ROWS(){ return _Rows; }
    static constexpr int COLS(){ return _Cols; }

    CodingMatrix(const CodingMatrix& m) = default;
    CodingMatrix& operator= (const CodingMatrix& m) = default;

    CodingMatrix<_ValueType, _Base, _Cols, _Rows> transpose() const{
        /// TODO
        CodingMatrix<_ValueType, _Base, _Cols, _Rows> m;
        for (int row = 0; row < _Rows; ++row){
        for (int col = 0; col < _Cols; ++col){
            m[row][col];
        }
        }
    }
    constexpr Vector<_ValueType, _Base, _Cols>& operator[] (int row){ return this->m_Data[row]; }


    friend std::ostream& operator<<(std::ostream& os, const MY_TYPE& rhs){
        for (auto& e : rhs.m_Data)
            os << e << std::endl;
        return os;
    }


    template<int _RhsRows, int _RhsCols>
    CodingMatrix<_ValueType, _Base, _RhsRows, _Cols> operator* (const CodingMatrix<_ValueType, _Base, _RhsRows, _RhsCols>& rhs){
        typedef CodingMatrix<_ValueType, _Base, _RhsRows, _Cols>  RETURN_TYPE;
        RETURN_TYPE toReturn;
        for (int row = 0; row < RETURN_TYPE::ROWS(); ++row){
        for (int col = 0; col < RETURN_TYPE::COLS(); ++col){
            for (int i = 0; i < _Cols; ++i){
                toReturn[row][col] += this->m_Data[row][i] + rhs.m_Data[i][col];
            }
            toReturn[row][col] %= _Base;
        }
        }
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
        for (int i = 0; i < _Rows; ++i)
            toReturn[i][i + _Rows] = -1;

        toReturn.truncateToBase();
        return toReturn;
    }


    void solveGauss(){
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
            if (allZeros)
                throw std::runtime_error("Error: rows are not linear indipendent, reached column: " + std::to_string(col));

            for (int i = col + 1; !this->m_Data[col][col] && i < _Rows; ++i){
                if (i >= _Rows)
                    throw std::runtime_error("Error: failed to solve gaussian");

                if (this->m_Data[i][col]){
                    std::swap(this->m_Data[col], this->m_Data[i]);
                    break;
                }
            }
            /// possible to swap cols to have at a non zero value at 'm_Data[col][col]'

            // diagonal value has to be 1
            // (m_Data[col][col] = 1)
            Vector<_ValueType, _Base, _Cols> tmp = this->m_Data[col];
            for (int i = 0; i < _Base &&  this->m_Data[col][col] != 1; ++i){
                this->m_Data[col] += tmp;
                this->m_Data[col].truncateToBase();
            }
            if (this->m_Data[col][col] != 1)
                throw std::runtime_error("Error in function: 'solveGauss', cannot generate diagonal, reached column: " + std::to_string(col));

            // all values below Matrix[col][col] has to be 0
            // => row = col + 1, no need to change the others
            for (int row = col + 1; row < _Rows; ++row){
                if (!this->m_Data[row][col])
                    continue; // already 0

                _ValueType factor = _Base - this->m_Data[row][col];
                this->m_Data[row] += this->m_Data[col] * factor; // NOTE: full addition isn't neseccary (a lot of 0), might be enhanced :)
                this->m_Data[row].truncateToBase();
            }
        }
    }

    void truncateToBase(){
        for (auto& e : this->m_Data)
            e.truncateToBase();
    }

private:
    std::array<Vector<_ValueType, _Base, _Cols>, _Rows> m_Data;
};


#endif

