/**
 * @brief 
 * 
 * @file net.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#ifndef __net_hpp
#define __net_hpp

#include "neuron/neuron.hpp"

typedef std::vector<neuron> Layer;

class net {
public:
private:
    double m_error;
    double m_recentAverageError;
    static double m_recentAverageSmoothingFactor;
};

#endif