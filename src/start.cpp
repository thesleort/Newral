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
    m_cfg_file.open(cfg_file);

    unsigned layer_num;
    enum type layer_type = INPUT;
    layer *current_layer = (layer *)malloc(sizeof(layer_config));

    while (getline(m_cfg_file, line)) {
        std::vector<std::string> fields;

        //boost
        boost::split_regex(fields, line, boost::regex("="));

        if (fields.at(0).compare("[net]") == 0 || layer_type == INPUT) {
            // Input
            layer_type = INPUT;
            current_layer->layer_type = INPUT;
            if (fields.at(0).compare("width")) {
                int width = stoi(fields.at(1));
                m_net_config.input_width = width;
                current_layer->width = width;
            }

            else if (fields.at(0).compare("height")) {
                int height = stoi(fields.at(1));
                m_net_config.input_height = height;
                current_layer->height = height;
            }

            else if (fields.at(0).compare("depth")) {
                int depth = stoi(fields.at(1));
                m_net_config.input_depth = depth;
                current_layer->depth = depth;
            }

            else if (fields.at(0).compare(" ")) {
                m_net_config.m_layers.push_back(*current_layer);
                current_layer = (layer *)malloc(sizeof(layer));
            }



        } else if (fields.at(0).compare("[convolution]") || layer_type == CONVOLUTION) {

            layer_type = CONVOLUTION;
            current_layer->layer_type = CONVOLUTION;

            if (fields.at(0).compare("filters")) {
				int filters = stoi(fields.at(1));
				// current_layer
            } else if (fields.at(0).compare("width")) {
                int width = stoi(fields.at(1));
                current_layer->width = width;
            }

            else if (fields.at(0).compare("height")) {
                int height = stoi(fields.at(1));
                current_layer->height = height;
            }

            else if (fields.at(0).compare("depth")) {
                int depth = stoi(fields.at(1));
                current_layer->depth = depth;
            }

            else if (fields.at(0).compare(" ")) {
                m_net_config.m_layers.push_back(*current_layer);
                current_layer = (layer *)malloc(sizeof(layer));
            }
        }
    }
}
