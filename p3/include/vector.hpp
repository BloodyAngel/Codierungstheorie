

#ifndef _VECOTR_HPP_HPP_
#define _VECOTR_HPP_HPP_

#include <array>
#include <iostream>

template<typename _ValueType, int _Base, int _Rows>
class Vector{
    template<typename vtype, int, int, int>
    friend class CodingMatrix;
    template<typename vtype, int, int>
    friend class Klasse;
    template<typename vtype, int, int, int>
    friend class NebenKlasse;

    std::array<_ValueType, _Rows> m_Data;
    typedef Vector<_ValueType, _Base, _Rows> MY_TYPE;
public:
    Vector(){
        for (auto& e : this->m_Data)
            e = 0;
    }

    _ValueType scalarProduct(const MY_TYPE& rhs){
        _ValueType toReturn = 0.0;
        for (int i = 0; i < _Rows; ++i)
            toReturn += this->m_Data[i] + rhs.m_Data[i];
        return toReturn;
    }

    auto begin() { return std::begin (this->m_Data); }
    auto end()   { return std::end   (this->m_Data); }

    auto cbegin()const { return std::cbegin(this->m_Data); }
    auto cend()  const { return std::cend  (this->m_Data); }

    auto& data() { return this->m_Data; }

    void truncateToBase(){
        for (auto& e : this->m_Data){
            e %= _Base;
            if (e < 0)
                e += _Base;
        }
    }
    MY_TYPE& operator= (const std::array<_ValueType, _Rows>& rhs){
        this->m_Data = rhs;
        return *this;
    }

    bool operator== (const MY_TYPE& rhs){
        return *this == rhs.m_Data;
    }

    bool operator== (const std::array<_ValueType, _Rows> rhs){
        if (this->m_Data == rhs)
            return true;
        return false;
    }

    friend std::ostream& operator<<(std::ostream& os, const MY_TYPE& rhs){
        bool first = true;
        for (auto& e : rhs.m_Data){
            if (first)
                first = false;
            else
                os << '|';
            os << e;
        }
        return os;
    }

    constexpr _ValueType& operator[](int row) { return this->m_Data[row]; }
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

    MY_TYPE operator+ (const MY_TYPE& rhs) const{
        MY_TYPE toReturn;
        for (int i = 0; i < _Rows; ++i)
            toReturn.m_Data[i] = this->m_Data[i] + rhs.m_Data[i];
        return toReturn;
    }
    MY_TYPE operator- (const MY_TYPE& rhs) const{
        MY_TYPE toReturn;
        for (int i = 0; i < _Rows; ++i)
            toReturn.m_Data[i] = this->m_Data[i] - rhs.m_Data[i];
        return toReturn;
    }
    MY_TYPE operator* (_ValueType rhs) const{
        MY_TYPE toReturn;
        for (int i = 0; i < _Rows; ++i)
            toReturn.m_Data[i] = this->m_Data[i] * rhs;
        return toReturn;
    }
    MY_TYPE operator/ (_ValueType rhs) const{
        MY_TYPE toReturn;
        for (int i = 0; i < _Rows; ++i)
            toReturn.m_Data[i] = this->m_Data[i] / rhs.m_Data[i];
        return toReturn;
    }

    friend MY_TYPE& operator*=(const _ValueType& lhs, MY_TYPE& rhs){
        for (auto& e : rhs.m_Data)
            e *= lhs;
        return rhs;
    }
    friend MY_TYPE  operator* (const _ValueType& lhs, const MY_TYPE& rhs) {
        MY_TYPE toReturn;
        toReturn *= lhs;
        return toReturn;
    }

    friend MY_TYPE& operator/=(const _ValueType& lhs, MY_TYPE& rhs){
        for (auto& e : rhs.m_Data)
            e /= lhs;
        return rhs;
    }
    friend MY_TYPE  operator/ (const _ValueType& lhs, const MY_TYPE& rhs) {
        MY_TYPE toReturn;
        toReturn /= lhs;
        return toReturn;
    }
};


#endif
