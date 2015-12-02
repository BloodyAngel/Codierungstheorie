#pragma once


#include <ostream>
#include <array>

template<int _MaxGrad, int _BaseValue>
class Polynom{
    typedef Polynom<_MaxGrad, _BaseValue> MY_TYPE;
public:
    Polynom(){
        for (auto& e : this->m_Data)
            e = 0;
    }
    Polynom(const MY_TYPE& rhs){
        this->m_Data = rhs.m_Data;
    }
    Polynom(MY_TYPE&& rhs) {
        this->m_Data = std::move(rhs.m_Data);
    }
    Polynom(const std::initializer_list<int>& list) {
        if (list.size() != _MaxGrad)
            throw "Error: Initializer list has to many arguments (in function Polynom(std::initializer_list<int>)";

        int counter = list.size() - 1;
        for (auto& e : list)
            this->m_Data[counter--] = e;
    }

    static const MY_TYPE* DIVISION_POLYNOM;


    MY_TYPE& operator+=(const MY_TYPE& poly){
        for (unsigned int i = 0; i < _MaxGrad; ++i)
            this->m_Data[i] += poly.m_Data[i];

        this->_truncateToBaseValue();
        return *this;
    }
    MY_TYPE operator+(const MY_TYPE& poly){
        MY_TYPE toReturn;
        for (unsigned int i = 0; i < _MaxGrad; ++i)
            toReturn.m_Data[i] = this->m_Data[i] + poly.m_Data[i];

        toReturn._truncateToBaseValue();
        return toReturn;
    }

    MY_TYPE operator*(const MY_TYPE& poly){
        if (!DIVISION_POLYNOM)
            throw "Error: Polynomdivision not possible, DIVISION_POLYNOM not set!";

        // create temporary polynom with 2*_MaxLength
        // this is the maximum grad the *-Operation can create
        Polynom<_MaxGrad * 2, _BaseValue> tmp;

        // calculate "multiplication" into tmp poly
        for (int i = 0, myGrad = this->grad(), polyGrad = poly.grad(); i <= myGrad; ++i){
            for (int k = 0; k <= polyGrad; ++k)
                tmp.m_Data[i + k] += poly.m_Data[k] * this->m_Data[i];
        }
        tmp._truncateToBaseValue();

        // calculate poly division until tmp.grad() <= _MaxGrad
        const int divisionGrad      = DIVISION_POLYNOM->grad();
        const int divisionGradKoeff = DIVISION_POLYNOM->m_Data[divisionGrad];
        int tmpGrad;
        while( (tmpGrad = tmp.grad())  >  divisionGrad     ||
              tmp.m_Data[divisionGrad] >= divisionGradKoeff)
        {
            int shift = tmpGrad - divisionGrad;
            int factor= 1;

            // NOTE:
            // if factor is always 1 -> factor = tmp.m_Data[tmpGrad];    is faster!
            while (tmp.m_Data[tmpGrad] != ((factor * DIVISION_POLYNOM->m_Data[divisionGrad]) % _BaseValue) )
                ++factor;

            for (int i = 0; i < _MaxGrad; ++i)
                tmp.m_Data[shift + i] -= factor * DIVISION_POLYNOM->m_Data[i];

            tmp._truncateToBaseValue();
        }
        MY_TYPE toReturn;
        for (int i = 0; i < _MaxGrad; ++i)
            toReturn.m_Data[i] = tmp.m_Data[i];

        return toReturn;
    }

    friend std::ostream& operator<<(std::ostream& os, const MY_TYPE& poly){
        for (int i = poly.m_Data.size() - 1; i >= 0; --i){
            os << poly.m_Data[i];
            if (i)
                os << "*x^" << i << "  + ";
        }

        return os;
    }

    MY_TYPE& operator=(MY_TYPE&& poly){
        this->m_Data = std::move(poly.m_Data);
        return *this;
    }

    int grad() const {
        int grad = 0;
        for (int i = 0; i < this->m_Data.size(); ++i){
            if (this->m_Data[i])
                grad = i;
        }
        return grad;
    }

    void _truncateToBaseValue(){
        for (auto& e : this->m_Data){
            e %= _BaseValue;
            if (e < 0)
                e += _BaseValue;
        }
    }
    std::array<int, _MaxGrad> m_Data;
};


template<int _MaxGrad, int _BaseValue>
const Polynom<_MaxGrad, _BaseValue>* Polynom<_MaxGrad, _BaseValue>::DIVISION_POLYNOM = nullptr;
