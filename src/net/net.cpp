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

net::net(net_config nc) {
    m_net_config = nc;

    m_topology.layers = (layer *)malloc(sizeof(layer) * m_net_config.layer_feature_map.size);
    m_topology.num_layers = m_net_config.layer_feature_map.size;

    for (int layer_num = 0; layer_num < m_topology.num_layers; ++layer_num) {
        layer *this_layer = &m_topology.layers[layer_num];
        this_layer->fmc = &m_net_config.layer_feature_map[layer_num];
        this_layer->filter = (filter *)malloc(sizeof(filter) * this_layer->fmc->filters);

        // Allocate input data structure
        if (this_layer->fmc->layer_type == INPUT) {
            for (int filter_num = 0; filter_num < this_layer->fmc->size; ++filter_num) {
                filter *this_filter = this_layer[layer_num].filter;

                this_filter->width = m_net_config.width;
                this_filter->height = m_net_config.height;
                this_filter->neuron = (neuron **)malloc(sizeof(neuron *) * this_filter->width);

                for (int x = 0; x < m_net_config.width; ++x) {
                    this_filter->neuron[x] = (neuron *)malloc(sizeof(neuron) * this_filter->height);

                    for (int y = 0; y < m_net_config.height; ++y) {
                        this_filter->neuron[x][y] = neuron(this_layer->fmc);
                    }
                }
            }
        } else if (this_layer->fmc->layer_type == MAXPOOL || // Allocate maxpool and convolution data structures
                   this_layer->fmc->layer_type == CONVOLUTION) {
            layer *prev_layer = &m_topology.layers[layer_num - 1];

			this_layer->fmc->layer_prev = &m_topology.layers[layer_num - 1];

            for (int filter_num = 0; filter_num < this_layer->fmc->size; ++filter_num) {
                filter *this_filter = this_layer[layer_num].filter;

                this_filter->width = filter_size(prev_layer->filter[0].width, this_layer->fmc);
                this_filter->height = filter_size(prev_layer->filter[0].height, this_layer->fmc);

                this_filter->neuron = (neuron **)malloc(sizeof(neuron *) * filter_size(this_filter->width, this_layer->fmc));
                for (int x = 0; x < this_filter->width; ++x) {
                    this_filter->neuron[x] = (neuron *)malloc(sizeof(neuron *) * filter_size(this_filter->height, this_layer->fmc));

                    for (int y = 0; y < this_filter->height; ++y) {
                        this_filter->neuron[x][y] = neuron(this_layer->fmc);
                    }
                }
            }
        }
    }
}

unsigned filter_size_calc(unsigned length, unsigned filter, int padding, unsigned stride) {
    return (length - filter + 2 * padding) / stride + 1;
}

unsigned filter_size(unsigned *length, feature_map_config *fmc) {
    return filter_size_calc(*length, fmc->filters, fmc->padding, fmc->stride);
}
