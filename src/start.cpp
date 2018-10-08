/**
 * @brief 
 * 
 * @file start.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-22
 */

#include "start.hpp"

// #include "config.h"
// #include "start.hpp"
// #include "net/net.hpp"
// #include "model/model.hpp"
// #include "text.hpp"

load_cfg::load_cfg(std::string &cfg_file) {

    std::string line;
    std::string previous_subject;

    m_cfg_file.open(cfg_file);

    unsigned num_layers = 0;
    enum type layer_type = INPUT;
    enum type previous_layer_type = INPUT;
    layer *current_layer;

    while (getline(m_cfg_file, line)) {
        std::vector<std::string> fields;

        //boost
        boost::split_regex(fields, line, boost::regex("="));

        if (fields.at(0).compare("[net]") == 0 || layer_type == INPUT) {
            if (fields.at(0).compare(previous_subject) != 0) {
                current_layer = (layer *)malloc(sizeof(layer));
                previous_subject = fields.at(0);
            }
            // Input
            layer_type = INPUT;
            current_layer->layer_type = INPUT;

            if (fields.at(0).compare("width")) {
                int width = stoi(fields.at(1));
                m_net_config.input_width = width;
                current_layer->width = width;

            } else if (fields.at(0).compare("height")) {
                int height = stoi(fields.at(1));
                m_net_config.input_height = height;
                current_layer->height = height;

            } else if (fields.at(0).compare("depth")) {
                int depth = stoi(fields.at(1));
                m_net_config.input_depth = depth;
                current_layer->depth = depth;
            }

        } else if (fields.at(0).compare("[convolution]") || layer_type == CONVOLUTION) {

            if (fields.at(0).compare(previous_subject) != 0) {
                m_net_config.m_layers.push_back(*current_layer);
                current_layer = (layer *)malloc(sizeof(layer));
                current_layer->filter_configs = (filter_config *)malloc(sizeof(filter_config));
                previous_subject = fields.at(0);
                ++num_layers;
            }

            layer_type = CONVOLUTION;
            current_layer->layer_type = CONVOLUTION;

            if (fields.at(0).compare("filters")) {
                int filters = stoi(fields.at(1));
                current_layer->filter_configs->filters = filters;
                // current_layer
            } else if (fields.at(0).compare("width")) {
                int width = stoi(fields.at(1));
                current_layer->filter_configs->width = width;

            } else if (fields.at(0).compare("height")) {
                int height = stoi(fields.at(1));
                current_layer->filter_configs->height = height;

            } else if (fields.at(0).compare("depth")) {
                int depth = stoi(fields.at(1));
                current_layer->filter_configs->depth = depth;

            } else if (fields.at(0).compare("stride")) {
                int stride = stoi(fields.at(1));
                current_layer->filter_configs->stride = stride;

            } else if (fields.at(0).compare("padding")) {
                int padding = stoi(fields.at(1));
                current_layer->filter_configs->padding = padding;
            }

        } else if (fields.at(0).compare("[end]")) {
            m_net_config.m_layers.push_back(*current_layer);
            ++num_layers;
            m_net_config.num_layers = num_layers;
        }
    }
}
// current_layer->neurons = (float *)malloc(sizeof(float) * current_layer->width * current_layer->height * current_layer->depth);
void load_cfg::allocator() {
    for (unsigned layer_num = 0; layer_num < m_net_config.m_layers.size(); ++layer_num) {

        layer &next_layer = (layer_num + 1 >= m_net_config.m_layers.size()) ? m_net_config.m_layers[layer_num + 1] : m_net_config.m_layers[layer_num];
        layer &previous_layer = (layer_num - 1 < 0) ? m_net_config.m_layers[layer_num] : m_net_config.m_layers[layer_num - 1];
        layer &current_layer = m_net_config.m_layers[layer_num];
        int height, width, depth;
        switch (current_layer.layer_type) {
        case INPUT:
            current_layer.neurons = (float *)malloc(sizeof(float) * current_layer.width * current_layer.height * current_layer.depth);
            break;
        case CONVOLUTION:
            // Allocate neurons on next layer
            height = (current_layer.height - current_layer.filter_configs->height + 2 * current_layer.filter_configs->padding) / current_layer.filter_configs->stride + 1;
            width = (current_layer.width - current_layer.filter_configs->width + 2 * current_layer.filter_configs->padding) / current_layer.filter_configs->stride + 1;
            height = current_layer.num_filters;
            current_layer.neurons = (float *)malloc(sizeof(float) * width * height * depth);

            // Set pointers between layers
            previous_layer.layer_next = &current_layer;
            current_layer.layer_prev = &previous_layer;
            current_layer.layer_this = &current_layer;

            // Allocate filters
            current_layer.filters = (filter *)malloc(sizeof(filter) * current_layer.num_filters);
            current_layer.filter_configs->m_layer = &current_layer;
            for (unsigned filter_num = 0; filter_num < current_layer.num_filters; ++filter_num) {
                current_layer.filters[filter_num].filter_weight = (float *)malloc(sizeof(float) * current_layer.filter_configs->width * current_layer.filter_configs->height * current_layer.filter_configs->depth);
                current_layer.filters[filter_num].filter_delta_weight = (float *)malloc(sizeof(float) * current_layer.filter_configs->width * current_layer.filter_configs->height * current_layer.filter_configs->depth);
                current_layer.filters[filter_num].m_filter_config = current_layer.filter_configs;
            }
            break;
        }
    }
}