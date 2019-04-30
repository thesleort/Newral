/**
 * @brief 
 * 
 * @file net.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#include <iostream>
#include <cmath>
#include <thread>

#include "config.h"
#include "net/net.hpp"
// #include "neuron/neuron.hpp"

net::net(NetConfig &nc, cl_setup &ocl) {
    m_ocl = ocl;
    m_net_config = &nc;

    // m_net_config.layer_num = (layer *)malloc(sizeof(layer) * m_net_config.m_layers.size());
    m_net_config->num_layers = m_net_config->layers.size();

    for (unsigned layer_num = 0; layer_num < m_net_config->num_layers; ++layer_num) {
        // layer &this_layer = m_net_config.layer_num[layer_num];
        Layer &this_layer = m_net_config->layers.at(layer_num);
        // this_layer = &m_net_config.m_layers[layer_num];

        //Allocate input data structure

        switch (this_layer.layer_type) {
        case INPUT:
            std::cout << "Adding layer: INPUT\n";
            add_layer(this_layer, INPUT);
            break;
        case MAXPOOL:
            std::cout << "Adding layer: MAXPOOL\n";
            add_layer(this_layer, MAXPOOL);
            break;
        case CONVOLUTION:
            std::cout << "Adding layer: CONVOLUTION\n";
            add_layer(this_layer, CONVOLUTION);
            break;
        case FULLY:
            std::cout << "Adding layer: FULLY CONNECTED\n";
            add_layer(this_layer, FULLY);
            break;
        case OUTPUT:
            std::cout << "Adding layer: OUTPUT\n";
            add_layer(this_layer, OUTPUT);
            break;
        }

        // Connect neurons to neurons in previous layer amd vice versa.
    }
}

void net::feed_forward(float *input) {
    std::cout << "Building OpenCL kernels...\n";
    m_ocl.build("src/compute/forward.cl", OPEN_CL_2_0);
    std::cout << "Done.\n";
    cl::Program program = m_ocl.get_programs()->at(0);

    for(int i = 0; i < (m_net_config->input_depth * m_net_config->input_height *m_net_config->input_width); ++i) {
        std::cout << input[i] << "-";
    }

    std::cout << "\n";

    for (unsigned layer_num = 0; layer_num < m_net_config->num_layers; ++layer_num) {
        // layer &this_layer = m_net_config.layer_num[layer_num];
        Layer &this_layer = m_net_config->layers.at(layer_num);
        // this_layer = &m_net_config.m_layers[layer_num];

        /**
		 * @brief TODO:
		 * * Load filter(s)
		 * 
		 * * Load layer
		 * 
		 * * Read output layer
		 * 
		 */

        int layersize = this_layer.height * this_layer.width * this_layer.depth;

        switch (this_layer.layer_type) {
        case INPUT:
            std::cout << "Computation started...\n";
            m_ocl_compute.load(program);
            this_layer.neurons = input;
            // for(unsigned i = 0; i < m_net_config.input_width * m_net_config.input_height * m_net_config.input_depth; ++i) {
            // this_layer.neurons[i] =
            // }
            // for (unsigned x = 0; x < m_net_config.input_width; ++x) {
            //     for (unsigned y = 0; y < m_net_config.input_height; ++y) {
            //         for (unsigned z = 0; z < m_net_config.input_depth; ++z) {
            //             this_layer.neurons[x * this_layer.height * this_layer.depth + y * this_layer.depth + z] = input[x * this_layer.height * this_layer.depth + y * this_layer.depth + z];
            //         }
            //     }
            // }
            break;
        case MAXPOOL:
            // m_ocl.build("src/compute/forward.cl", "-cl-std=CL1.2");

            // cl::Program program = m_ocl.get_programs()->at(0);

            // auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
            // auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
            // auto &device = devices.front();

            // int filter_height = this_layer

            break;
        case CONVOLUTION:
            std::cout << "Computing convolution, layer:" << layer_num << "\n";
            m_ocl_compute.compute_convolution(this_layer);
            break;
        case FULLY:
            break;
        case OUTPUT:
            std::cout << "Result\n";
            m_net_config->layers.at(layer_num).depth = m_net_config->layers.at(layer_num - 1).num_filters;
            m_ocl_compute.output(m_net_config->layers.at(layer_num));
            break;
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

unsigned filter_size(unsigned &length, FilterConfig *fmc) {
    return filter_size_calc(length, fmc->filters, fmc->padding, fmc->stride);
}

/**
 * @brief To add the volumes containing the actual neurons.
 * 
 * @param config 
 */
void net::add_layer(Layer &layer, enum type type) {
    int layersize = layer.width * layer.height * layer.depth;
    layer.neurons = new float[layersize];
    // layer.neurons = (float *)malloc(sizeof(float) * layersize);

    layer.filters = new Filter[layer.num_filters];

    if (type != OUTPUT && type != INPUT) {
        layer.filters_config->filter = layer.filters;

        for (unsigned filter = 0; filter < layer.num_filters; ++filter) {
            add_filter(*layer.filters_config, filter);
        }
    }
}

void net::add_filter(FilterConfig &config, int filter_num) {
    // config.filter->filter_weights = (float *)malloc(config.height * config.width * config.depth * sizeof(float));
    int filter_size = config.height * config.width * config.depth;
    config.filter[filter_num].filter_weights = new float[filter_size];

    config.filter[filter_num].filter_delta_weights = new float[filter_size];
    // config.filter->filter_delta_weights = (float *)malloc(config.height * config.width * config.depth * sizeof(float));

    for (unsigned filter_weight = 0; filter_weight < filter_size; ++filter_weight) {
        // config.filter[filter_num].filter_weights[filter_weight] = random_weight();
        // config.filter[filter_num].filter_weights[filter_weight] = 1;
        config.filter[filter_num].filter_delta_weights[filter_weight] = 0;
    }
}

/**
 * @brief Connects the neurons in the current layer with the respective 
 * neurons in the filter before it.
 * 
 * @param lc 
 * @param fc 
 */
// TODO: Remove, since unnecessary ?
void net::connect_neurons(Layer &lc, FilterConfig &fc) {
    for (unsigned x = 0; x < lc.width; ++x) {
        for (unsigned y = 0; y < lc.height; ++y) {
            for (unsigned filter_z = 0; filter_z < lc.depth; ++filter_z) {
            }
        }
    }
}

float net::random_weight() {
    return rand() / float(RAND_MAX);
}

int net::get_3d_index(unsigned x, unsigned y, unsigned z, unsigned width, unsigned depth) {
    return (x + width * (y + depth * z));
}