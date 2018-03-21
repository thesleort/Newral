/**
 * @brief 
 * 
 * @file neuron.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#ifndef __neuron_hpp
#define __neuron_hpp

#include <vector>
#include <cstdlib>

struct connction{
    double weight;
    double delta_weight;
} connection;

class neuron {
public:
    neuron(unsigned num_outputs);

private:
    
};

#endif