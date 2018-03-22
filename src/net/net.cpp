/**
 * @brief 
 * 
 * @file net.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#include <iostream>
#include <cmath>

#include "config.h"
#include "net/net.hpp"


double net::m_recent_average_smoothing_factor = RECENT_AVERAGE_SMOOTHING_FACTOR; // Number of training samples to average over


void net::init(const std::vector<unsigned> &topology) {
    unsigned num_layers = topology.size();

    std::cout << " Layer size: " << sizeof(m_layers) << std::endl;
    for (unsigned layer_num = 0; layer_num < num_layers; ++layer_num) {
        m_layers.push_back(Layer());
        unsigned num_outputs = layer_num == topology.size() - 1 ? 0 : topology[layer_num + 1];

        for (unsigned neuron_num = 0; neuron_num <= topology[layer_num]; ++neuron_num) {
            m_layers.back().push_back(neuron(num_outputs, neuron_num));

            //std::cout << "Made a neuron" << std::endl;
        }

        // Force the bias node's output value to 1.0. It's the last neuron created above
        m_layers.back().back().set_output_val(1.0);
    }
}

void net::feed_forward(const std::vector<double> &input_vals) {
//    std::cout << "layers_ " << m_layers[0].size() << std::endl;
    //assert(inputVals.size() == m_layers[0].size() - 1);

    // Assign (latch) the input values into the input neurons
    for (unsigned i = 0; i < input_vals.size(); ++i) {
        m_layers[0][i].set_output_val(input_vals[i]);
    }

    // forward propagate
    for (unsigned layer_num = 1; layer_num < m_layers.size(); ++layer_num) {
        Layer &prev_layer = m_layers[layer_num - 1];
        for (unsigned n = 0; n < m_layers[layer_num].size() - 1; ++n) {
            m_layers[layer_num][n].feed_forward(prev_layer);
        }
    }
}

void net::back_prop(const std::vector<double> &target_vals) {

    // Calculate overall net error (RMS of output neuron errors) *Round Mean Square Error
    Layer &output_layer = m_layers.back();
    m_error = 0.0;

    for (unsigned neuron = 0; neuron < output_layer.size() - 1; ++neuron) {
        double delta = target_vals[neuron] - output_layer[neuron].get_output_val();
        m_error += delta * delta;
    }

    m_error /= output_layer.size() - 1;  // Get average squared
    m_error = sqrt(m_error);            // RMS


    // Implement a recent average measurement
    m_recent_average_error =
            (m_recent_average_error * m_recent_average_smoothing_factor + m_error)
            / (m_recent_average_smoothing_factor + 1.0);


    // Calculate output layer gradients
    for (unsigned neuron = 0; neuron < output_layer.size() - 1; ++neuron) {
        output_layer[neuron].calc_output_gradients(target_vals[neuron]);
    }

    // Calculate gradients on hidden layers
    for (unsigned layer_num = m_layers.size() - 2; layer_num > 0; --layer_num) {
        Layer &hidden_layer = m_layers[layer_num];
        Layer &next_layer = m_layers[layer_num + 1];

        for (unsigned neuron = 0; neuron < hidden_layer.size(); ++neuron) {
            hidden_layer[neuron].calc_hidden_gradients(next_layer);
        }
    }

    // For all layers from outputs to first hidden layer,
    // update connection weights
    for (unsigned layer_num = m_layers.size() - 1; layer_num > 0; --layer_num) {
        Layer &layer = m_layers[layer_num];
        Layer &prev_layer = m_layers[layer_num - 1];

        for (int neuron = 0; neuron < layer.size() - 1; ++neuron) {
            layer[neuron].update_input_weights(prev_layer);
        }
    }
}

void net::get_results(std::vector<double> &result_vals) const {
    result_vals.clear();

    for (int neuron = 0; neuron < m_layers.back().size() - 1; ++neuron) {
        // Take current neuron in the last layer and get output value
        result_vals.push_back(m_layers.back()[neuron].get_output_val());
    }
}

double net::get_recent_average_error() {
    return m_recent_average_error;
}

std::vector<Layer> net::get_net() {
    return m_layers;
}

void net::get_net(std::vector<Layer> &layers) {
    layers = m_layers;
}

void net::net_info(std::vector<Layer> **layers) {
    std::cout << "m_layers address:  " << &m_layers << std::endl;
    std::cout << "  layers address:  " << layers << std::endl;
    *layers = &m_layers;
    std::cout << "m_layers address2: " << &m_layers << std::endl;
    std::cout << "  layers address2: " << layers << std::endl;
//    std::cout << "  weight address: " << &m_layers[0][0].getOutputWeights()[0].weight << std::endl;
//    std::cout << " Layer size: " << sizeof(m_layers) << std::endl;
}