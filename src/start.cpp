/**
 * @brief 
 * 
 * @file start.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-22
 */

#include "start.hpp"
#include <json.hpp>

// #include "config.h"
// #include "start.hpp"
// #include "net/net.hpp"
// #include "model/model.hpp"
// #include "text.hpp"

setup::setup() {
}

/**
 * @brief Construct a new load cfg::load cfg object.
 * This loads the config, that defines the structure 
 * of the neural network.
 * 
 * @param cfg_file 
 */
void setup::load_cfg(std::string &cfg_file) {

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

        if (fields.at(0).compare("[net]") == 0) {
            layer_type = INPUT;
            current_layer = (layer *)malloc(sizeof(layer));
            previous_subject = fields.at(0);
        } else if (fields.at(0).compare("[convolution]") == 0) {
            layer_type = CONVOLUTION;
            m_net_config.m_layers.push_back(*current_layer);
            current_layer = (layer *)malloc(sizeof(layer));
            current_layer->filter_configs = (filter_config *)malloc(sizeof(filter_config));
            previous_subject = fields.at(0);
            ++num_layers;
        }

        if (layer_type == INPUT) {

            // Input
            layer_type = INPUT;
            current_layer->layer_type = INPUT;

            if (fields.at(0).compare("width") == 0) {
                int width = stoi(fields.at(1));
                m_net_config.input_width = width;
                current_layer->width = width;

            } else if (fields.at(0).compare("height") == 0) {
                int height = stoi(fields.at(1));
                m_net_config.input_height = height;
                current_layer->height = height;

            } else if (fields.at(0).compare("depth") == 0) {
                int depth = stoi(fields.at(1));
                m_net_config.input_depth = depth;
                current_layer->depth = depth;
            }

        } else if (layer_type == CONVOLUTION) {

            layer_type = CONVOLUTION;
            current_layer->layer_type = CONVOLUTION;

            if (fields.at(0).compare("filters") == 0) {
                int filters = stoi(fields.at(1));
                current_layer->filter_configs->filters = filters;

            } else if (fields.at(0).compare("width") == 0) {
                int width = stoi(fields.at(1));
                current_layer->filter_configs->width = width;

            } else if (fields.at(0).compare("height") == 0) {
                int height = stoi(fields.at(1));
                current_layer->filter_configs->height = height;

            } else if (fields.at(0).compare("depth") == 0) {
                int depth = stoi(fields.at(1));
                current_layer->filter_configs->depth = depth;

            } else if (fields.at(0).compare("stride") == 0) {
                int stride = stoi(fields.at(1));
                current_layer->filter_configs->stride = stride;

            } else if (fields.at(0).compare("padding") == 0) {
                int padding = stoi(fields.at(1));
                current_layer->filter_configs->padding = padding;
            }

        } else if (fields.at(0).compare("[end]") == 0) {
            m_net_config.m_layers.push_back(*current_layer);
            ++num_layers;
            m_net_config.num_layers = num_layers;
        }
    }

    allocator();
}
// current_layer->neurons = (float *)malloc(sizeof(float) * current_layer->width * current_layer->height * current_layer->depth);
/**
 * @brief Allocates space on the CPU for 
 * the layers and filters.
 * 
 */
void setup::allocator() {
    std::cout << "Allocating memory\n";
    for (unsigned layer_num = 0; layer_num < m_net_config.m_layers.size(); ++layer_num) {

        layer &next_layer = (layer_num + 1 >= m_net_config.m_layers.size()) ? m_net_config.m_layers[layer_num + 1] : m_net_config.m_layers[layer_num];
        layer &previous_layer = (layer_num - 1 < 0) ? m_net_config.m_layers[layer_num] : m_net_config.m_layers[layer_num - 1];
        layer &current_layer = m_net_config.m_layers[layer_num];
        int height = 0, width = 0, depth = 0;
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

void setup::load_weights(std::string &weights_file_name) {
    // m_net_config = &net_config;

    unsigned layer_num = 0;
    unsigned filter_num = 0;
    unsigned weight_num = 0;

    // std::string line;
    // layer *current_layer;
    // enum type layer_type;

    // std::ifstream weights_file(weights_file_name, std::ifstream::binary);
    m_weights_file.open(weights_file_name);

    nlohmann::json j_weights = nlohmann::json::parse(m_weights_file);

    unsigned filter_length;

    for (layer_num = 0; layer_num < j_weights["layers"].size(); ++layer_num) {
        switch (m_net_config.m_layers[layer_num].layer_type) {
        case INPUT:
            break;
        case CONVOLUTION:
            for (filter_num = 0; filter_num < j_weights["layers"]["filters"].size(); ++filter_num) {
                filter_length = m_net_config.m_layers[layer_num].filter_configs[filter_num].width *
                                m_net_config.m_layers[layer_num].filter_configs[filter_num].height *
                                m_net_config.m_layers[layer_num].filter_configs[filter_num].depth;

                for (weight_num = 0; weight_num < filter_length; ++weight_num) {
                    m_net_config.m_layers[layer_num].filters[filter_num].filter_weight[weight_num] = j_weights["layers"]["filters"][filter_num].get<float>();
                }
            }
            break;
        case MAXPOOL:
            break;
        case FULLY:
            break;
        case OUTPUT:
            break;
        }
    }
    // j_weights["layers"][0]
    // while(getline(m_weights_file, line)) {
    //     std::vector<std::string> fields;

    // 	boost::split_regex(fields, line, boost::regex("=\\s"));

    // 	if(fields.at(0).compare("layer")) {

    // 		// layer_type =
    // 	}

    // }
    // for(unsigned layer_num = 0; layer_num < m_net_config->num_layers; ++layer_num) {
    //     switch(m_net_config->m_layers[layer_num].layer_type) {
    //     case INPUT:
    //         break;
    //     case CONVOLUTION:
    //         for(unsigned filter_num = 0; filter_num < m_net_config->m_layers[layer_num].num_filters; ++filter_num) {
    //             m_net_config->m_layers[layer_num].filters[filter_num].filter_weight = j["layers"][filter_num]
    //         }
    //     }
    // }
}

void setup::load_input(std::string &input_file, bool is_image) {
    if (!is_image) {
    }
}

net_config setup::get_cfg() {
    return m_net_config;
}