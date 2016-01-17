
#ifndef _HPP_KLASSE_HPP_
#define _HPP_KLASSE_HPP_

#include <vector>
#include <iostream>

#include "vector.hpp"


template<typename _ValueType, int _Base, int _Rows>
class Klasse{
    typedef Vector<_ValueType, _Base, _Rows> MY_VECTOR;
public:
    Klasse(const std::initializer_list<std::array<_ValueType, _Rows>>& lst){
        MY_VECTOR v;
        for (const auto& e : lst){
            v = e;
            this->m_Data.push_back(v);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Klasse& k){
        int idx = 0;
        for (const auto& e : k.m_Data)
            os << idx++ << ":\r\t" << e << std::endl << std::endl;
        return os;
    }

    bool addElement(const auto& vector){
        for (auto& e : this->m_Data){
            if (e == vector)
                return false;
        }
        MY_VECTOR v;
        v = vector;
        this->m_Data.push_back(v);
        return true;
    }

    bool calculateSmallestWeight(){
        bool isSingleResult = true;
        if (!this->m_Data.size()){
            this->m_SmallestWeight = nullptr;
            return false;
        }

        // count number of non 0's
        auto count = [](const MY_VECTOR& v) -> int{
            int count = 0;
            for (auto iter = v.cbegin(); iter != v.cend(); ++iter){
                if (*iter)
                    ++count;
            }
            return count;
        };
        auto smaller = [&count, &isSingleResult](const MY_VECTOR& lhs, const MY_VECTOR& rhs) -> bool{
            int count_lhs = count(lhs);
            int count_rhs = count(rhs);

            if (count_lhs == count_rhs)
                isSingleResult = false;

            if (count_lhs < count_rhs)
                return true;
            return false;
        };

        this->m_SmallestWeight = &this->m_Data[0];
        for (int i = 1; i < this->m_Data.size(); ++i){
            if (smaller(this->m_Data[i], *this->m_SmallestWeight))
                this->m_SmallestWeight = &this->m_Data[i];
        }
        return isSingleResult;
    }

    const MY_VECTOR* getSmallestWeight(){
        if (!this->m_SmallestWeight)
            this->calculateSmallestWeight();

        return this->m_SmallestWeight;
    }

    Klasse() = default; // nebenklasse is allowed to use this function
private:
    template<typename vtype, int, int, int>
    friend class NebenKlasse;

    std::vector<MY_VECTOR> m_Data;
    const MY_VECTOR* m_SmallestWeight = nullptr;
};

#endif
