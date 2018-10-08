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

net::net(net_config nc, compute &ocl) {
    m_ocl = ocl;
    m_net_config = nc;

    m_net_config.layer_num = (layer *)malloc(sizeof(layer) * m_net_config.m_layer_config.size());
    m_net_config.num_layers = m_net_config.m_layer_config.size();

    for (unsigned layer_num = 0; layer_num < m_net_config.num_layers; ++layer_num) {
        layer *this_layer = &m_net_config.layer_num[layer_num];
        this_layer->m_layer_config = m_net_config.m_layer_config[layer_num];

        //Allocate input data structure

        switch (this_layer->m_layer_config.layer_type) {
        case INPUT:
            add_layer(this_layer->m_layer_config, INPUT);
            break;
        case MAXPOOL:
            add_layer(this_layer->m_layer_config, MAXPOOL);
            break;
        case CONVOLUTION:
            add_layer(this_layer->m_layer_config, CONVOLUTION);
            break;
        case FULLY:
            add_layer(this_layer->m_layer_config, FULLY);
            break;
        case OUTPUT:
            add_layer(this_layer->m_layer_config, OUTPUT);
            break;
        }

        // Connect neurons to neurons in previous layer amd vice versa.
    }
}

void net::feed_forward(std::vector<std::vector<std::vector<float>>> &input) {
    for (unsigned layer_num = 0; layer_num < m_net_config.num_layers; ++layer_num) {
        layer *this_layer = &m_net_config.layer_num[layer_num];
        this_layer->m_layer_config = m_net_config.m_layer_config[layer_num];

        m_ocl.build("src/compute/forward.cl", OPEN_CL_1_2);

        cl::Program program = m_ocl.get_programs()->at(0);
        /**
		 * @brief TODO:
		 * * Load filter(s)
		 * 
		 * * Load layer
		 * 
		 * * Read output layer
		 * 
		 */
        switch (this_layer->m_layer_config.layer_type) {
        case INPUT:
            for (unsigned x = 0; x < input.size(); ++x) {
                for (unsigned y = 0; y < input[x].size(); ++y) {
                    for (unsigned z = 0; z < input[x][y].size(); ++z) {
                        this_layer->neurons[x * this_layer->m_layer_config.height * this_layer->m_layer_config.depth + y * this_layer->m_layer_config.depth + z] = input[x][y][z];
                    }
                }
            }
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
            compute_convolution(*this_layer, program);
            break;
        case FULLY:
            break;
        case OUTPUT:
            break;
        }
    }
}

void net::compute_convolution(layer &this_layer, cl::Program &program) {

    auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
    auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
    auto &device = devices.front();

    int num_filters = this_layer.m_layer_config.num_filters;

    int layer_prev_height = this_layer.layer_prev->m_layer_config.height;
    int layer_prev_width = this_layer.layer_prev->m_layer_config.width;
    int layer_prev_depth = this_layer.layer_prev->m_layer_config.depth;

    int layer_height = this_layer.m_layer_config.height;
    int layer_width = this_layer.m_layer_config.width;
    int layer_depth = this_layer.m_layer_config.depth;

    int filter_height;
    int filter_width;
    int filter_depth;
    int filter_padding;
    int filter_stride;

    cl::CommandQueue queue = cl::CommandQueue(context, device);

    std::vector<cl::Buffer> filter_buffers;
    cl::Buffer input_neurons;
    cl::Buffer output_neurons;

    cl::Buffer input_neurons = cl::Buffer(context, CL_MEM_READ_ONLY, layer_prev_height * layer_prev_width * layer_prev_depth * sizeof(float), this_layer.layer_prev->neurons);
    cl::Buffer output_neurons = cl::Buffer(context, CL_MEM_READ_WRITE, layer_height * layer_width * layer_depth * sizeof(float), this_layer.neurons);

    for (unsigned filter_num = 0; filter_num < this_layer.layer_prev->m_layer_config.num_filters; ++filter_num) {

        filter_height = this_layer.m_layer_config.filter_configs[filter_num].height;
        filter_width = this_layer.m_layer_config.filter_configs[filter_num].width;
        filter_depth = this_layer.m_layer_config.filter_configs[filter_num].depth;

        filter_padding = this_layer.m_layer_config.filter_configs[filter_num].padding;
        filter_stride = this_layer.m_layer_config.filter_configs[filter_num].stride;

        filter_buffers.push_back(cl::Buffer(context, CL_MEM_READ_ONLY, filter_height * filter_width * filter_depth * sizeof(float), this_layer.filters[filter_num].filter_weight));
        cl::Kernel kernel(program, "convolution");

        kernel.setArg(0, input_neurons);
        kernel.setArg(1, layer_height);
        kernel.setArg(2, layer_width);
        kernel.setArg(3, layer_depth);
        kernel.setArg(4, filter_buffers.at(filter_num));
        kernel.setArg(5, filter_height);
        kernel.setArg(6, filter_width);
        kernel.setArg(7, filter_depth);
        kernel.setArg(8, output_neurons);
        kernel.setArg(9, filter_padding);
        kernel.setArg(10, filter_stride);
        kernel.setArg(11, filter_num);

        queue.enqueueNDRangeKernel(kernel, cl::NullRange, )
    }

    // queue.enqueueWriteBuffer()
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
    config.layer_this->neurons = (float *)malloc(config.width * config.height * config.depth * sizeof(float));
    config.layer_this->filters = (filter *)malloc(config.num_filters * sizeof(filter));

    for (unsigned neuron = 0; neuron < sizeof(config.layer_this->neurons); ++neuron) {
        config.layer_this->neurons[neuron] = 0;
    }

    for (unsigned filter = 0; filter < config.layer_this->m_layer_config.num_filters; ++filter) {
        add_filter(config.filter_configs[filter]);
    }
}

void net::add_filter(filter_config &config) {
    config.m_filter->filter_weight = (float *)malloc(config.height * config.width * config.depth * sizeof(float));
    config.m_filter->filter_delta_weight = (float *)malloc(config.height * config.width * config.depth * sizeof(float));

    for (unsigned filter_weight = 0; filter_weight < sizeof(config.m_filter->filter_weight); ++filter_weight) {
        config.m_filter->filter_weight[filter_weight] = random_weight();
        config.m_filter->filter_delta_weight[filter_weight] = 0;
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
void net::connect_neurons(layer_config &lc, filter_config &fc) {
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