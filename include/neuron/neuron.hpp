/**
 * @brief 
 * 
 * @file neuron.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#ifndef __NEURON_HPP
#define __NEURON_HPP

#include <vector>
#include <cstdlib>

typedef struct connection {
    double weight;
    double delta_weight;
} connection;

class neuron {
public:
    neuron(unsigned num_outputs);

private:
};

#endif