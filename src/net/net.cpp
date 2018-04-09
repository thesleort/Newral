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
    m_topology.layers = (layer *)malloc(sizeof(layer()) * m_net_config.layer_feature_map.size());

    for (int layer = 0; layer < m_net_config.layer_feature_map.size(); ++layer) {
        m_topology.layers[layer].fmc = &m_net_config.layer_feature_map[0];

        m_topology.layers[layer].fmc = &m_net_config.layer_feature_map[0];
        m_topology.layers[layer].filter = (filter *)malloc(sizeof(filter) * m_net_config.filters);

        for (int color = 0; color < m_net_config.layer_feature_map[layer].filters; ++color) {
            m_topology.layers[layer].filter[color].neuron = (neuron **)malloc(sizeof(neuron) * m_topology);

            if (layer == 0) {
				m_topology.layers[layer].filter[color].width = m_net_config.width;
				m_topology.layers[layer].filter[color].height = m_net_config.height;

                for (int width = 0; width < m_net_config.width; ++width) {
                    m_topology.layers[layer].filter[color].neuron[width] = (neuron *)malloc(sizeof(neuron) * m_net_config.height);

                    for (int height = 0; height < m_net_config.height; ++height) {
                        m_topology.layers[layer].filter[color].neuron[width][height] = neuron(&m_net_config.layer_feature_map[layer]);
                    }
                }
            } else {
				unsigned width = filter_size(m_topology.layers[layer-1].filter[0].width, m_net_config.layer_feature_map[layer]);
				unsigned height = filter_size(m_topology.layers[layer-1].filter[0].height, m_net_config.layer_feature_map[layer]);
				m_topology.layers[layer].filter[color].width = width;
				m_topology.layers[layer].filter[color].height = height;

                for (int x = 0; x < width; ++x) {
                    m_topology.layers[layer].filter[color].neuron[x] = (neuron *)malloc(sizeof(neuron) * height);

                    for (int y = 0; y < height; ++y) {
                        m_topology.layers[layer].filter[color].neuron[width][height] = neuron(&m_net_config.layer_feature_map[layer]);
                    }
                }
            }
        }
    }
}

unsigned filter_size_calc(unsigned length, unsigned filter, int padding, unsigned stride) {
    return (length - filter + 2 * padding) / stride + 1;
}

unsigned filter_size(unsigned length, feature_map_config fmc) {
	return filter_size_calc(length, fmc.filters, fmc.padding, fmc.stride);
}


/*
net::net(net_config nc) {

	m_topology.layers = (layer*) malloc(sizeof(layer()));
	m_topology.layers[0].filter = (filter*) malloc(sizeof(filter) * nc.filters);
    for (int color = 0; color < nc.filters; ++color) {
		m_topology.layers[0].filter[color].feature_map = (feature_map**) malloc(sizeof(feature_map) * nc.width);
        for (int width = 0; width < nc.width; ++width) {
			m_topology.layers[0].filter[color].feature_map[width] = (feature_map*) malloc(sizeof(feature_map) * nc.height);
            for (int height = 0; height < nc.height; ++height) {
				feature_map new_feature_map;
				new_feature_map.map
                m_topology.layers[0].filter[color].feature_map[width][height] = neuron();
            }
        }
    }
}
*/