
#ifndef _HPP_NEBENKLASSEN_HPP_HPP_
#define _HPP_NEBENKLASSEN_HPP_HPP_

#include "klasse.hpp"
#include "matrix.hpp"

#include <algorithm>
#include <vector>

template<typename _ValueType, int _Base, int _Rows, int _Cols>
class NebenKlasse{
    CodingMatrix<_ValueType, _Base, _Rows, _Cols>& m_ControlMatrix;

    struct Result{
        Vector<_ValueType, _Base, _Cols> result;
        bool isSingleResult;
    };
    std::vector<Result> m_Syndroms;
public:
    NebenKlasse(CodingMatrix<_ValueType, _Base, _Rows, _Cols>& controlMatrix) : m_ControlMatrix(controlMatrix){
        const int maxNumElements = std::pow(_Base, _Rows);
        const int numSyndroms = std::pow(_Base, _Cols);

        std::vector<Klasse<_ValueType, _Base, _Cols>> tmpKlassen(maxNumElements);
        this->m_Syndroms.resize(numSyndroms);

        for (int i = 0; i < numSyndroms; ++i){
            Vector<_ValueType, _Base, _Cols> vector;
            for (int value = i, iter = 0; value >= 0 && iter < _Cols; ++iter, value /= _Base)
                vector[iter] = value % _Base;

            auto syndrom = controlMatrix * vector;
            int idx = 0;
            int adjustFactor = 1;
            for (int k = 0; k < syndrom.m_Data.size(); ++k, adjustFactor *= _Base)
                idx += adjustFactor * syndrom.m_Data[k];

            tmpKlassen.at(idx).addElement(vector);
        }

        for (int i = 0; i < tmpKlassen.size(); ++i){
            this->m_Syndroms[i].isSingleResult = tmpKlassen[i].calculateSmallestWeight();
            this->m_Syndroms[i].result = *tmpKlassen[i].getSmallestWeight();
        }
    }

    bool getMaximumLikely(Vector<_ValueType, _Base, _Cols>& received){
        // calculate syndrom
        auto syndrom = this->m_ControlMatrix * received;
        int synIdx = 0;
        int adjustmentFactor = 1;
        for (int i = 0; i < syndrom.m_Data.size(); ++i, adjustmentFactor *= _Base)
            synIdx += adjustmentFactor * syndrom.m_Data[i];

        received -= this->m_Syndroms.at(synIdx).result;
        received.truncateToBase();
        return this->m_Syndroms[synIdx].isSingleResult;
    }
};

#endif
