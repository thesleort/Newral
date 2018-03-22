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

double neuron::eta = ETA;  // Overall net training rate [0.0..1.0]
double neuron::alpha = ALPHA; // Momentum, multiplier of last deltaWeight [0.0..n]

neuron::neuron(unsigned num_outputs, unsigned my_index) {
    for (unsigned connections = 0; connections < num_outputs; ++connections) {
        m_output_weights.push_back(Connection());
        m_output_weights.back().weight = random_weight();

    }

    m_my_index = my_index;
}

void neuron::set_output_val(double val) {
    m_output_val = val;
}

double neuron::get_output_val() const {
    return m_output_val;
}

void neuron::feed_forward(const Layer &prev_layer) {
    double sum = 0.0;

    // Sum previous layers outputs, which are inputs
    // Include bias node from same previous layer.

    for (unsigned neuron = 0; neuron < prev_layer.size(); ++neuron) {
        sum += prev_layer[neuron].get_output_val() *
                prev_layer[neuron].m_output_weights[m_my_index].weight;
    }

    m_output_val = tanh_transfer_function(sum);
}

void neuron::calc_output_gradients(double targetVal) {
    double delta = targetVal - m_output_val;
    m_gradient = delta * tanh_transfer_function_derivative(m_output_val);
}

void neuron::calc_hidden_gradients(const Layer &next_layer) {
    double dow = sum_DOW(next_layer);
    m_gradient = dow * tanh_transfer_function_derivative(m_output_val);
}

void neuron::update_input_weights(Layer &prev_layer) {
    // The weights to be updated are in the Connection container
    // in the neurons in the preceding layer.

    for (unsigned neuron_num = 0; neuron_num < prev_layer.size(); ++neuron_num) {
        neuron &neuron = prev_layer[neuron_num];
        double old_delta_weight = neuron.m_output_weights[m_my_index].delta_weight;

        double new_delta_weight =
                // Individual input, magnified by the gradient and training rate.
                eta
                * neuron.get_output_val()
                * m_gradient
                // Also add momentum = a fraction of the previous delta weight
                + alpha
                * old_delta_weight;

        neuron.m_output_weights[m_my_index].delta_weight = new_delta_weight;
        neuron.m_output_weights[m_my_index].weight += new_delta_weight;
    }
}

std::vector<Connection> neuron::get_output_weights() {
    return m_output_weights;
}

void neuron::set_output_weight(Layer &prevLayer) {
    for (unsigned neuronNum = 0; neuronNum < prevLayer.size(); ++neuronNum) {

    }
}

void neuron::set_output_weight(Connection &connection, unsigned index) {
    m_output_weights[index] = connection;
}

void neuron::set_output_weights(double weight, double delta_weight) {
    m_output_weights[m_output_index].weight = weight;
    m_output_weights[m_output_index].delta_weight = delta_weight;
    ++m_output_index;
}

double neuron::sum_DOW(const Layer &next_layer) const {
    double sum = 0.0;

    // Sum our contributions of the errors at the nodes that we feed

    for (unsigned neuron = 0; neuron < next_layer.size() - 1; ++neuron) {
        sum += m_output_weights[neuron].weight * next_layer[neuron].m_gradient;
    }

    return sum;
}