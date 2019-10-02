/**
 * @brief 
 * 
 * @file start.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-22
 */

#include "start.hpp"
#include <libconfig.h++>
#include <math.h>

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

    m_cfg_file.open(cfg_file);

    unsigned num_layers = 0;
    enum type layer_type = INPUT;
    // enum type previous_layer_type = INPUT;

    Layer *current_layer;
    while (getline(m_cfg_file, line)) {
        std::vector<std::string> fields;

        //boost
        boost::split_regex(fields, line, boost::regex("="));

        if (fields.at(0).compare("[net]") == 0) {
            layer_type = INPUT;
            // current_layer = (Layer *)malloc(sizeof(Layer));
            current_layer = new Layer;
            current_layer->net_config = &m_net_config;
        } else if (fields.at(0).compare("[convolution]") == 0) {
            layer_type = CONVOLUTION;
            m_net_config.layers.push_back(*current_layer);
            // current_layer = (Layer *)malloc(sizeof(Layer));
            // current_layer->filters_config = (FilterConfig *)malloc(sizeof(FilterConfig));
            current_layer = new Layer;
            current_layer->filters_config = new FilterConfig;
            current_layer->net_config = &m_net_config;
            ++num_layers;
        } else if (fields.at(0).compare("[maxpool]") == 0) {
            layer_type = MAXPOOL;
            m_net_config.layers.push_back(*current_layer);

            current_layer = new Layer;
            current_layer->maxpool_config = new MaxpoolConfig;
            current_layer->net_config = &m_net_config;
            ++num_layers;
        } else if (fields.at(0).compare("[full]") == 0) {
            layer_type = FULLY;
            m_net_config.layers.push_back(*current_layer);

            current_layer = new Layer;
            current_layer->fully_config = new FullNetConfig;
            current_layer->net_config = &m_net_config;
            ++num_layers;
        } else if (fields.at(0).compare("[end]") == 0) {
            layer_type = OUTPUT;
            m_net_config.layers.push_back(*current_layer);

            current_layer = new Layer;
            current_layer->net_config = &m_net_config;
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
                current_layer->num_filters = depth;
            }

        } else if (layer_type == CONVOLUTION) {

            layer_type = CONVOLUTION;
            current_layer->layer_type = CONVOLUTION;

            if (fields.at(0).compare("filters") == 0) {
                int filters = stoi(fields.at(1));
                std::cout << "conv filters: " << filters << "\n";
                current_layer->filters_config->filters = filters;
                current_layer->num_filters = filters;
                std::cout << "layer conv filters: " << current_layer->filters_config->filters << "\n";

            } else if (fields.at(0).compare("width") == 0) {
                int width = stoi(fields.at(1));
                current_layer->filters_config->width = width;

            } else if (fields.at(0).compare("height") == 0) {
                int height = stoi(fields.at(1));
                current_layer->filters_config->height = height;

            } else if (fields.at(0).compare("depth") == 0) {
                int depth = stoi(fields.at(1));
                current_layer->filters_config->depth = depth;

            } else if (fields.at(0).compare("stride") == 0) {
                int stride = stoi(fields.at(1));
                current_layer->filters_config->stride = stride;

            } else if (fields.at(0).compare("padding") == 0) {
                int padding = stoi(fields.at(1));
                current_layer->filters_config->padding = padding;
            } else if (fields.at(0).compare("id") == 0) {
                current_layer->id = fields.at(1);
                std::cout << "ID: " << current_layer->id << "\n";
            }
        } else if (layer_type == MAXPOOL) {
            layer_type = MAXPOOL;
            current_layer->layer_type = MAXPOOL;

            if (fields.at(0).compare("width") == 0) {
                int width = stoi(fields.at(1));
                current_layer->maxpool_config->width = width;

            } else if (fields.at(0).compare("height") == 0) {
                int height = stoi(fields.at(1));
                current_layer->maxpool_config->height = height;

            } else if (fields.at(0).compare("stride") == 0) {
                int stride = stoi(fields.at(1));
                current_layer->maxpool_config->stride = stride;
            }
        } else if (layer_type == FULLY) {
            layer_type = FULLY;
            current_layer->layer_type = FULLY;

            if (fields.at(0).compare("size") == 0) {
                int size = stoi(fields.at(1));
                current_layer->width = size;
            } else if (fields.at(0).compare("id") == 0) {
                current_layer->id = fields.at(1);

            } else if (fields.at(0).compare("activation") == 0) {
                if (fields.at(1).compare("relu") == 0) {
                    current_layer->fully_config->activation_function = RELU;
                }
            }

        } else if (layer_type == OUTPUT) {
            ++num_layers;
            layer_type = OUTPUT;
            current_layer->layer_type = OUTPUT;
            m_net_config.layers.push_back(*current_layer);
            m_net_config.num_layers = num_layers;
        }
    }
    allocator();
}
/**
 * @brief Allocates space on the CPU for 
 * the layers and filters.
 * 
 */
void setup::allocator() {
    std::cout << "Allocating memory\n";
    std::cout << "Num channels: " << m_net_config.layers[0].num_filters << "\n";
    std::cout << "Num layers: " << m_net_config.layers.size() << "\n";
    for (unsigned layer_num = 0; layer_num < m_net_config.layers.size(); ++layer_num) {

        // Layer &next_layer = (layer_num + 1 >= m_net_config.layers.size()) ? m_net_config.layers.at(layer_num) : m_net_config.layers.at(layer_num + 1);
        Layer &previous_layer = (layer_num > 0) ? m_net_config.layers.at(layer_num - 1) : m_net_config.layers.at(layer_num);
        // Layer &previous_layer = m_net_config.layers.at(layer_num);
        Layer &current_layer = m_net_config.layers.at(layer_num);

        float height = 0, width = 0, depth = 0, size = 0;

        switch (current_layer.layer_type) {
        case INPUT:
            std::cout << "Allocating: Input\n";
            std::cout << "Input Width: " << current_layer.width << "\n";
            std::cout << "Input Height: " << current_layer.height << "\n";
            std::cout << "Input Depth: " << current_layer.depth << "\n";

            current_layer.layer_prev = NULL;
            // current_layer.neurons = (float *)malloc(sizeof(float) * current_layer.width * current_layer.height * current_layer.depth);
            break;
        case CONVOLUTION:
            // Calculate size of layer
            std::cout << "Allocating: Convolution\n";
            width = (previous_layer.width - current_layer.filters_config->width + 2 * current_layer.filters_config->padding) / current_layer.filters_config->stride + 1;
            height = (previous_layer.height - current_layer.filters_config->height + 2 * current_layer.filters_config->padding) / current_layer.filters_config->stride + 1;
            depth = current_layer.num_filters;
            // current_layer.neurons = (float *)malloc(sizeof(float) * width * height * depth);

            // Set values in struct
            current_layer.width = width;
            current_layer.height = height;
            current_layer.depth = depth;

            // Set pointers between layers
            previous_layer.layer_next = &current_layer;
            current_layer.layer_prev = &previous_layer;
            // current_layer.layer_this = &current_layer;

            // Allocate filters

            // current_layer.filters = (Filter *)malloc(sizeof(Filter) * current_layer.filters_config->filters);
            current_layer.filters_config->layer = &current_layer;
            // std::cout << "Allocating: Filters (" << current_layer.filters_config->filters << ")\n";
            // for (unsigned filter_num = 0; filter_num < current_layer.filters_config->filters; ++filter_num) {
            //     std::cout << "Allocating: Filter (" << filter_num << ")\n";
            //     current_layer.filters[filter_num].filter_weights = (float *)malloc(sizeof(float) * current_layer.filters_config->width * current_layer.filters_config->height * current_layer.filters_config->depth);
            //     current_layer.filters[filter_num].filter_delta_weights = (float *)malloc(sizeof(float) * current_layer.filters_config->width * current_layer.filters_config->height * current_layer.filters_config->depth);
            //     current_layer.filters[filter_num].filter_config = current_layer.filters_config;
            // }
            break;
        case MAXPOOL:
            std::cout << "Allocation: Maxpool\n";
            // Calculate size of layer
            width = (previous_layer.width - current_layer.maxpool_config->width) / current_layer.maxpool_config->stride + 1;
            height = (previous_layer.height - current_layer.maxpool_config->height) / current_layer.maxpool_config->stride + 1;
            depth = previous_layer.depth;

            // Set values in struct
            current_layer.width = width;
            current_layer.height = height;
            current_layer.depth = depth;
            current_layer.num_filters = depth;

            // Set pointers between layers
            previous_layer.layer_next = &current_layer;
            current_layer.layer_prev = &previous_layer;

            // current_layer.filters_config->layer = &current_layer;

            break;
        case FULLY:
            std::cout << "Allocation: Fully Connected\n";

            size = current_layer.fully_config->size;

            // current_layer.width = size;
            current_layer.height = 1;
            current_layer.depth = 1;
            current_layer.num_filters = current_layer.width;
            current_layer.fully_config->size = previous_layer.width * previous_layer.height * previous_layer.depth;

            previous_layer.layer_next = &current_layer;
            current_layer.layer_prev = &previous_layer;

            break;
        case DROPOUT:
            break;
        case OUTPUT:
            std::cout << "Allocation: Output\n";
            previous_layer.layer_next = &current_layer;
            current_layer.layer_prev = &previous_layer;
            current_layer.layer_next = NULL;

            current_layer.width = current_layer.layer_prev->width;
            current_layer.height = current_layer.layer_prev->height;

            current_layer.depth = current_layer.layer_prev->depth;
            break;
        }
    }
    return;
}

void setup::load_weights(std::string &weights_file_name) {

    unsigned layer_num = 0;
    unsigned filter_num = 0;
    unsigned weight_num = 0;

    m_weights_file.open(weights_file_name);

    libconfig::Config cfg;

    // cfg.readFile(weights_file_name);
    cfg.readFile(weights_file_name.c_str());

    const libconfig::Setting &root = cfg.getRoot();
    const libconfig::Setting &layers = root["layers"];

    // unsigned filter_length;
    bool found = true;

    for (layer_num = 0; layer_num < m_net_config.num_layers; layer_num++) {
        switch (m_net_config.layers[layer_num].layer_type) {
        case INPUT:
            break;
        case CONVOLUTION: {
            for (int i = 0; i < layers.getLength(); i++) {
                std::string id;
                layers[i].lookupValue("id", id);

                if (id.compare(m_net_config.layers[layer_num].id) == 0) {
                    libconfig::Setting &filters = layers[i]["filters"];
                    const unsigned filter_size = m_net_config.layers[layer_num].filters_config->width *
                                                 m_net_config.layers[layer_num].filters_config->height *
                                                 m_net_config.layers[layer_num].filters_config->depth;
                    for (filter_num = 0; filter_num < m_net_config.layers[layer_num].num_filters; ++filter_num) {
                        for (weight_num = 0; weight_num < filter_size; weight_num++) {
                            m_net_config.layers[layer_num].filters[filter_num].filter_weights[weight_num] = (float)filters[filter_num][weight_num];
                        }
                        m_net_config.layers[layer_num].filters[filter_num].bias = (float)layers[i]["bias"][filter_num];
                        std::cout << "\n";
                    }
                    break;
                }
            }
        } break;
        case MAXPOOL:
            // Maxpool does not contain any weights
            break;
        case FULLY: {
            for (int i = 0; i < layers.getLength(); i++) {

                std::string id;
                layers[i].lookupValue("id", id);

                if (id.compare(m_net_config.layers[layer_num].id) == 0) {
                    libconfig::Setting &weights = layers[i]["weights"];
                    const unsigned filter_size = m_net_config.layers[layer_num].fully_config->size;

                    for (filter_num = 0; filter_num < m_net_config.layers[layer_num].num_filters; ++filter_num) {

                        for (weight_num = 0; weight_num < filter_size; weight_num++) {
                            m_net_config.layers[layer_num].weights[filter_num].net_weights[weight_num] = (float)weights[filter_num][weight_num];
                        }
                        m_net_config.layers[layer_num].weights[filter_num].bias = (float)layers[i]["bias"][0];
                    }
                }
                break;
            }
        } break;
        case DROPOUT:
            break;
        case OUTPUT:
            break;
        }

        if (!found) {
            std::cout << "Weights file appears to be incorrect. Exiting program\n";
            exit(EXIT_FAILURE);
        }
    }

    // for (layer_num = 0; layer_num < j_weights["layers"].size(); ++layer_num) {
    //     switch (m_net_config.layers[layer_num].layer_type) {
    //     case INPUT:
    //         break;
    //     case CONVOLUTION:
    //         for (filter_num = 0; filter_num < m_net_config.layers[layer_num].num_filters; ++filter_num) {
    //             filter_length = m_net_config.layers[layer_num].filters_config->width *
    //                             m_net_config.layers[layer_num].filters_config->height *
    //                             m_net_config.layers[layer_num].filters_config->depth;

    //         std::cout << "HELP\n";
    //             for (weight_num = 0; weight_num < filter_length; ++weight_num) {
    //                 // m_net_config.layers[layer_num].filters[filter_num].filter_weights[weight_num] = j_weights["layers"]["filters"][filter_num].get<float>();
    //                 // std::cout << j_weights["layers"]["filters"].size() << ",";
    //             }
    //         }
    //         break;
    //     case MAXPOOL:
    //         break;
    //     case FULLY:
    //         break;
    //     case OUTPUT:
    //         break;
    //     }
    // }
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

float *setup::load_input(std::string &input_file, bool is_image) {
    int input_size = m_net_config.input_width * m_net_config.input_height * m_net_config.input_depth;
    int slice = m_net_config.input_width * m_net_config.input_height;
    // float *input = (float *)malloc(sizeof(float) * input_size);
    float *input = new float[input_size];

    if (!is_image) {
        std::string line;
        m_input_file.open(input_file);
        unsigned line_num = 0;
        while (getline(m_input_file, line)) {
            std::vector<std::string> fields;

            boost::split_regex(fields, line, boost::regex(" "));
            // TODO: Turn into SIMD instruction later
            for (unsigned i = 0; i < m_net_config.input_width * m_net_config.input_height * m_net_config.input_depth; ++i) {

                // TODO: Normalize data
                // float normalize = line[i]
                input[slice * line_num + i] = std::stof(fields[i]);
            }
            ++line_num;
        }
        return input;
    }
    return input;
}

NetConfig *setup::get_cfg() {
    return &m_net_config;
}