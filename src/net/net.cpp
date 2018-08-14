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
#include "neuron/neuron.hpp"

net::net(net_config nc) {
    m_net_config = nc;

    m_net_config.layer_num = (layer *)malloc(sizeof(layer) * m_net_config.layer_config.size());
    m_net_config.num_layers = m_net_config.layer_config.size();

    for (unsigned layer_num = 0; layer_num < m_net_config.num_layers; ++layer_num) {
        layer *this_layer = &m_net_config.layer_num[layer_num];
        this_layer->layer_config = m_net_config.layer_config[layer_num];

        //Allocate input data structure

        switch (this_layer->layer_config.layer_type) {
        case INPUT:
            add_layer(this_layer->layer_config, INPUT);
            break;
        case MAXPOOL:
            add_layer(this_layer->layer_config, MAXPOOL);
            break;
        case CONVOLUTION:
            add_layer(this_layer->layer_config, CONVOLUTION);
            break;
        case FULLY:
            add_layer(this_layer->layer_config, FULLY);
            break;
        case OUTPUT:
            add_layer(this_layer->layer_config, OUTPUT);
            break;
        }

        // Connect neurons to neurons in previous layer amd vice versa.
    }
}

// void net::feed_forward(std::vector<std::vector<std::vector<float>>> &input) {
//     if (&m_net_config != NULL) {

//         if (input.size() == m_net_config.filters && input[0].size() == m_net_config.width && input[0][0].size() == m_net_config.height) {

//             // Assign (latch) the input values into the input neurons
//             for (unsigned filter_num = 0; filter_num < m_net_config.filters; ++filter_num) {
//                 for (unsigned x = 0; x < m_net_config.width; ++x) {
//                     for (unsigned y = 0; y < m_net_config.height; ++y) {
//                         m_topology.layers[0].filters[filter_num].neurons[x][y].set_output_val(input[filter_num][x][y]);
//                     }
//                 }
//             }

//             // forward propagate
//             for (unsigned layer_num = 1; layer_num < m_net_config.layer_feature_map.size(); ++layer_num) {
//                 layer &prev_layer = m_topology.layers[layer_num - 1];
//                 for (unsigned filter_num = 0; filter_num < m_topology.layers[layer_num].fmc->filters; ++filter_num) {
//                     for (unsigned x = 0; x < m_topology.layers[layer_num].fmc->size; ++x) {
//                         for (unsigned y = 0; y < m_topology.layers[layer_num].fmc->size; ++y) {
// 							m_topology.layers[layer_num].filters[filter_num].neurons[x][y].feed_forward(prev_layer);
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

unsigned filter_size_calc(unsigned &length, unsigned &filter, int &padding, unsigned &stride) {
    return (length - filter + 2 * padding) / stride + 1;
}

unsigned filter_size(unsigned &length, feature_map_config *fmc) {
    return filter_size_calc(length, fmc->filters, fmc->padding, fmc->stride);
}

/**
 * @brief To add the volumes containing the actual neurons.
 * 
 * @param config 
 */
void net::add_layer(layer_config &config, enum type type) {
    config.layer_this->neurons = (neuron ***)malloc(sizeof(neuron **) * config.width);

    for (unsigned x = 0; x < config.width; ++x) {
        config.layer_this->neurons[x] = (neuron **)malloc(sizeof(neuron *) * config.height);

        for (unsigned y = 0; y < config.height; ++y) {
            config.layer_this->neurons[x][y] = (neuron *)malloc(sizeof(neuron) * config.depth);

            for (unsigned z = 0; z < config.depth; ++z) {
                // Init neuron
                config.layer_this->neurons[x][y][z] = neuron(config, x, y, z);
                // if (type == CONVOLUTION) {
                //     config.layer_this->neurons[x][y][z].set_input_weights(config.layer_prev->layer_config.filter_configs[z]);
                // } else {
				// 	config.layer_this->neurons[x][y][z].set_input_weights(config.layer_prev->layer_config);
				// }
            }
        }
    }
}

/**
 * @brief To add the filters that will be applied. These filters are stored 
 * separately to allow for easier parameter/weight sharing.
 * 
 * @param config 
 */
// void net::add_filter(filter_config &config) {
//     config.filter->neurons = (neuron ***)malloc(sizeof(neuron **) * config.width);

//     for (unsigned x = 0; x < config.width; ++x) {
//         config.filter->neurons[x] = (neuron **)malloc(sizeof(neuron *) * config.height);

//         for (unsigned y = 0; y < config.height; ++y) {
//             config.filter->neurons[x][y] = (neuron *)malloc(sizeof(neuron) * config.depth);

//             for (unsigned z = 0; z < config.depth; ++z) {
//                 // Init filter neuron
//                 config.filter->neurons[x][y][z] = neuron(*config.layer_config, x, y, z, FILTER);
//                 config.filter->neurons[x][y][z].set_input_weights(config, config.layer_config->layer_prev->layer_config);
//             }
//         }
//     }
// }

/**
 * @brief Connects the neurons in the current layer with the respective 
 * neurons in the filter before it.
 * 
 * @param lc 
 * @param fc 
 */
// TODO: Remove, since unnecessary ?
void net::connect_neurons(layer_config &lc, filter_config &fc) {
    for (unsigned x = 0; x < lc.width; ++x) {
        for (unsigned y = 0; y < lc.height; ++y) {
            for (unsigned filter_z = 0; filter_z < lc.depth; ++filter_z) {
            }
        }
    }
}