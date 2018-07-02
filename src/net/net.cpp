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

    m_topology.layer_num = (layer *)malloc(sizeof(layer) * m_net_config.layer_config.size());
    m_topology.num_layers = m_net_config.layer_config.size();

    for (unsigned layer_num = 0; layer_num < m_topology.num_layers; ++layer_num) {
        layer *this_layer = &m_topology.layer_num[layer_num];
        this_layer->layer_config = &m_net_config.layer_config[layer_num];
        this_layer->filter_num = (filter *)malloc(sizeof(filter) * this_layer->layer_config->num_filters);

        // Allocate input data structure
        if (this_layer->layer_config->layer_type == INPUT) {
            
        } else if (this_layer->layer_config->layer_type == MAXPOOL || // Allocate maxpool and convolution data structures
                   this_layer->layer_config->layer_type == CONVOLUTION) {
            
        }
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
