/**
 * @brief 
 * 
 * @file neuron.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#include "config.h"
#include "neuron/neuron.hpp"
#include "net/net.hpp"
#include "operator/functions.hpp"

neuron::neuron(unsigned num_outputs) {
    for (unsigned connection = 0; connection < num_outputs; ++connection) {
		m_output_weights.push_back(Connection());
		m_output_weights.back().weight = random_weight();
    }
}

void neuron::set_output_val(double output_val) {
    m_output_val = output_val;
}

double neuron::get_output_val() {
    return m_output_val;
}

void neuron::set_output_weight(Connection &connection, unsigned index) {
    m_output_weights[index] = connection;
}

std::vector<Connection> *neuron::get_output_weights() {
	return &m_output_weights;
}