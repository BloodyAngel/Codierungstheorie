#pragma once

#include <iostream>
#include <ostream>
#include <array>


template<int _MaxDegree, int _BaseValue>
class Polynom{
    typedef Polynom<_MaxDegree, _BaseValue> MY_TYPE;
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
        if (list.size() != _MaxDegree)
            throw "Error: Initializer list has to many arguments (in function Polynom(std::initializer_list<int>)";

        int counter = list.size() - 1;
        for (auto& e : list)
            this->m_Data[counter--] = e;
    }

    static const MY_TYPE* DIVISION_POLYNOM;


    MY_TYPE& operator+=(const MY_TYPE& poly){
        for (unsigned int i = 0; i < _MaxDegree; ++i)
            this->m_Data[i] += poly.m_Data[i];

        this->_truncateToBaseValue();
        return *this;
    }
    MY_TYPE operator+(const MY_TYPE& poly){
        MY_TYPE toReturn;
        for (unsigned int i = 0; i < _MaxDegree; ++i)
            toReturn.m_Data[i] = this->m_Data[i] + poly.m_Data[i];

        toReturn._truncateToBaseValue();
        return toReturn;
    }

    MY_TYPE operator*(const MY_TYPE& poly){
        if (!DIVISION_POLYNOM)
            throw "Error: Polynomdivision not possible, DIVISION_POLYNOM not set!";

        // create temporary polynom with 2*_MaxLength
        // this is the maximum grad the *-Operation can create
        Polynom<_MaxDegree * 2, _BaseValue> tmp;

        // calculate "multiplication" into tmp poly
        for (int i = 0, myDegree = this->degree(), polyDegree = poly.degree(); i <= myDegree; ++i){
            for (int k = 0; k <= polyDegree; ++k)
                tmp.m_Data[i + k] += poly.m_Data[k] * this->m_Data[i];
        }
        tmp._truncateToBaseValue();

        // calculate poly division until tmp.degree() <= _MaxDegree
        const int divisionDegree = DIVISION_POLYNOM->degree();

        while(tmp > *DIVISION_POLYNOM) {
            int tmpDegree = tmp.degree();
            int shift = tmpDegree - divisionDegree;
            int factor= 1;

            // NOTE:
            // if factor is always 1 -> factor = tmp.m_Data[tmpDegree];    is faster!
            while (tmp.m_Data[tmpDegree] != ((factor * DIVISION_POLYNOM->m_Data[divisionDegree]) % _BaseValue) )
                ++factor;

            for (int i = 0; i < _MaxDegree; ++i)
                tmp.m_Data[shift + i] -= factor * DIVISION_POLYNOM->m_Data[i];

            tmp._truncateToBaseValue();
        }
        MY_TYPE toReturn;
        for (int i = 0; i < _MaxDegree; ++i)
            toReturn.m_Data[i] = tmp.m_Data[i];

        return toReturn;
    }

    friend std::ostream& operator<<(std::ostream& os, const MY_TYPE& poly){
        bool alreadyWrote = false;
        for (int i = poly.m_Data.size() - 1; i >= 0; --i){
			
            if (!poly.m_Data[i]){
                if (alreadyWrote || i)
                    continue;
            }

            if (alreadyWrote)
                os << '+';

            alreadyWrote = true;
            if (poly.m_Data[i] > 1 || i == 0)
                os << poly.m_Data[i];
            if (i == 1)
                os << "x";
            else if (i > 1)
                os << "x" << i;
            else{
            }
        }
        return os;
    }

    MY_TYPE& operator=(MY_TYPE&& poly){
        this->m_Data = std::move(poly.m_Data);
        this->m_Degree = std::move(poly.m_Degree);
        return *this;
    }

    int degree() const {
        if (this->m_Degree < 0){
            // degree has been adjusted
            int degree = -1;
            for (int i = this->m_Data.size() - 1; i >= 0; --i){
                if (this->m_Data[i]){
                    degree = i;
                    break;
                }
            }
            this->m_Degree = degree;
        }
        return this->m_Degree;
    }

//private:
    template<int v, int k>
    friend class Polynom;

    template<int _RhsMaxDegree>
    int compare(const Polynom<_RhsMaxDegree, _BaseValue>& rhs){
        int degreeIter = this->degree();
        if (degreeIter < rhs.degree())
            return -1;
        if (degreeIter > rhs.degree())
            return +1;

        while (degreeIter >= 0){
            if (this->m_Data[degreeIter] < rhs.m_Data[degreeIter])
                return -1;
            if (this->m_Data[degreeIter] > rhs.m_Data[degreeIter])
                return +1;

            --degreeIter;
        }
        return 0;
    }

    template<int _RhsMaxDegree>
    bool operator< (const Polynom<_RhsMaxDegree, _BaseValue>& rhs){
        if (this->compare(rhs) == -1)
            return true;
        return false;
    }
    template<int _RhsMaxDegree>
    bool operator> (const Polynom<_RhsMaxDegree, _BaseValue>& rhs){
        if (this->compare(rhs) == 1)
            return true;
        return false;
    }

    template<int _RhsMaxDegree>
    bool operator>=(const Polynom<_RhsMaxDegree, _BaseValue>& rhs){
        if (this->compare(rhs) >= 0)
            return true;
        return false;
    }
    template<int _RhsMaxDegree>
    bool operator<=(const Polynom<_RhsMaxDegree, _BaseValue>& rhs){
        if (this->compare(rhs) <= 0)
            return true;
        return false;
    }

    template<int _RhsMaxDegree>
    bool operator==(const Polynom<_RhsMaxDegree, _BaseValue>& rhs){
        if (this->compare(rhs) == 0)
            return true;
        return false;
    }
    template<int _RhsMaxDegree>
    bool operator!=(const Polynom<_RhsMaxDegree, _BaseValue>& rhs){
        if (this->compare(rhs) != 0)
            return true;
        return false;
    }


    void _truncateToBaseValue(){
        for (auto& e : this->m_Data){
            e %= _BaseValue;
            if (e < 0)
                e += _BaseValue;
        }
        this->m_Degree = -1;
    }
    mutable int m_Degree = -1;
    std::array<int, _MaxDegree> m_Data;
};


template<int _MaxDegree, int _BaseValue>
const Polynom<_MaxDegree, _BaseValue>* Polynom<_MaxDegree, _BaseValue>::DIVISION_POLYNOM = nullptr;
