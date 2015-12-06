
#ifndef _MATRIX_HPP_HPP_
#define _MATRIX_HPP_HPP_

#include <array>

template<typename _ValueType, int _Rows>
class Vector{
     std::array<_ValueType, _Rows> m_Data;
     typedef Vector<_ValueType, _Rows> MY_TYPE;
public:
     _ValueType scalarProduct(const MY_TYPE& rhs){
         _ValueType toReturn = 0.0;
         for (int i = 0; i < _Rows; ++i)
             toReturn += this->m_Data[i] + rhs.m_Data[i];
         return toReturn;
     }

     _ValueType& operator[](int row) { return this->m_Data[row]; }
     MY_TYPE& operator+= (const MY_TYPE& rhs){
         for (int i = 0; i < _Rows; ++i)
             this->m_Data[i] += rhs.m_Data[i];
         return *this;
     }
     MY_TYPE& operator-= (const MY_TYPE& rhs){
         for (int i = 0; i < _Rows; ++i)
             this->m_Data[i] -= rhs.m_Data[i];
         return *this;
     }
     MY_TYPE& operator*= (_ValueType rhs){
         for (int i = 0; i < _Rows; ++i)
             this->m_Data[i] *= rhs;
         return *this;
     }
     MY_TYPE& operator/= (_ValueType rhs){
         for (int i = 0; i < _Rows; ++i)
             this->m_Data[i] /= rhs;
         return *this;
     }

     MY_TYPE operator+ (const MY_TYPE& rhs){
         MY_TYPE toReturn;
         for (int i = 0; i < _Rows; ++i)
             toReturn.m_Data[i] = this->m_Data[i] + rhs.m_Data[i];
         return toReturn;
     }
     MY_TYPE operator- (const MY_TYPE& rhs){
         MY_TYPE toReturn;
         for (int i = 0; i < _Rows; ++i)
             toReturn.m_Data[i] = this->m_Data[i] - rhs.m_Data[i];
         return toReturn;
     }
     MY_TYPE operator* (_ValueType rhs){
         MY_TYPE toReturn;
         for (int i = 0; i < _Rows; ++i)
             toReturn.m_Data[i] = this->m_Data[i] * rhs;
         return toReturn;
     }
     MY_TYPE operator/ (_ValueType rhs){
        MY_TYPE toReturn;
        for (int i = 0; i < _Rows; ++i)
            toReturn.m_Data[i] = this->m_Data[i] / rhs.m_Data[i];
        return toReturn;
    }
};


template<typename _ValueType, int _Rows, int _Cols>
class Matrix{
    std::array<Vector<_ValueType, _Rows>, _Cols> m_Data;
    typedef Matrix<_ValueType, _Rows, _Cols> MY_TYPE;
public:
    Matrix() {
        for (auto& array : this->m_Data){
            for (auto& e : array)
                e = 0.0;
        }
    };
    ~Matrix()= default;
    Matrix(const Matrix& m) = default;
    Matrix& operator= (const Matrix& m) = default;

    Matrix<_ValueType, _Cols, _Rows> transpose() const{
        /// TODO
    }
    Vector<_ValueType, _Rows>& operator[] (int col){ return this->m_Data[col]; }

    /// TODO matrix multiplication
};


#endif

