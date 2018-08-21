/**
 * @brief 
 * 
 * @file neuron.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#include "config.h"
#include "neuron/neuron.hpp"
#include "operator/functions.hpp"

/**
 * @brief Construct a new neuron::neuron object
 * 
 * @param lc 
 * @param x 
 * @param y 
 * @param filter 
 */
neuron::neuron(layer_config &lc, unsigned x, unsigned y, unsigned filter) {

    //m_output_weights = (Connection ***)malloc(sizeof(Connection *) *
    //filter_size(fmc.layer_next->fmc->filters, fmc.layer_next->fmc));
    layer *prev_layer = lc.layer_prev;

    unsigned num_filters = prev_layer->m_layer_config.depth; // .num_filters maybe?
    unsigned stride = prev_layer->m_layer_config.filter_configs[filter].stride;
    unsigned padding = prev_layer->m_layer_config.filter_configs[filter].padding;

    switch (lc.layer_type) {
    case INPUT: {
        // There are no input weights
        break;
    }

    case CONVOLUTION: {

        // TODO: Don't allocate space for padding neurons
        m_input_weights = (neuron_connection ***)malloc(sizeof(neuron_connection *) * prev_layer->m_layer_config.filter_configs[filter].width);

        for (unsigned input_x = 0; input_x < prev_layer->m_layer_config.filter_configs[filter].width; input_x) {
            m_input_weights[input_x] = (neuron_connection **)malloc(sizeof(neuron_connection *) * prev_layer->m_layer_config.filter_configs[filter].height);

            for (unsigned input_y = 0; input_y < prev_layer->m_layer_config.filter_configs[filter].height; ++input_y) {
                m_input_weights[input_x][input_y] = (neuron_connection *)malloc(sizeof(neuron_connection *) * prev_layer->m_layer_config.filter_configs[filter].depth);

                for (unsigned input_z = 0; input_z < prev_layer->m_layer_config.filter_configs[filter].depth; ++input_z) {
                    // Set shared weight for connection
                    m_input_weights[input_x][input_y][input_z].weights = lc.filter_configs[filter].m_filter->filter_weight[x][y][filter].weights;

                    // Set specific connected neuron in previous layer
                    // calculation might be wrong - please test
                    if (input_x + x - padding >= 0 && input_x + x + padding < prev_layer->m_layer_config.width) {

                        if (input_y + y - padding >= 0 && input_y + y + padding < prev_layer->m_layer_config.height) {
                            m_input_weights[input_x][input_y][input_z].edge = &prev_layer->neurons[x * stride + input_x][y * stride + input_y][input_z];
                        }
                    }
                    // else if (x - padding < 0 || x - padding > prev_layer->layer_config.width) {

                    //     if (y - padding < 0 || y - padding > prev_layer->layer_config.height) {
                    //         m_input_weights[input_x][input_y][input_z].edge = &prev_layer->neurons[x * stride + input_x][y * stride + input_y][input_z];
                    //     }
                    // }
                }
            }
        }
        break;
    }
    case FULLY:
        /* Could maybe be similar to CONVOLUTION, but with filter size equal to layer size,
         * so that all neurons from previous layer are connected to each neuron in the current
         * layer
         */

        // TODO: Don't allocate space for padding neurons
        m_input_weights = (neuron_connection ***)malloc(sizeof(neuron_connection *) * prev_layer->m_layer_config.filter_configs[filter].width);

        for (unsigned input_x = 0; input_x < prev_layer->m_layer_config.filter_configs[filter].width; input_x) {
            m_input_weights[input_x] = (neuron_connection **)malloc(sizeof(neuron_connection *) * prev_layer->m_layer_config.filter_configs[filter].height);

            for (unsigned input_y = 0; input_y < prev_layer->m_layer_config.filter_configs[filter].height; ++input_y) {
                m_input_weights[input_x][input_y] = (neuron_connection *)malloc(sizeof(neuron_connection *) * prev_layer->m_layer_config.filter_configs[filter].depth);

                for (unsigned input_z = 0; input_z < prev_layer->m_layer_config.filter_configs[filter].depth; ++input_z) {

                    // Set individual weight for connection
                    neuron_weight *nw = (neuron_weight *)malloc(sizeof(neuron_weight));
                    nw->weight = random_weight();
                    nw->delta_weight = 0;
                    m_input_weights[input_x][input_y][input_z].weights = nw;
                }
            }
        }
        break;
    case MAXPOOL: {
        /*
        *  Operates independently on each depth slice, hence it does not take a 3D input.
        * 
        */
        // Similar in structure to CONVOLUTION, however, padding is often not used.

        // TODO: Don't allocate space for padding neurons
        m_input_weights = (neuron_connection ***)malloc(sizeof(neuron_connection *) * prev_layer->m_layer_config.filter_configs[filter].width);

        for (unsigned input_x = 0; input_x < prev_layer->m_layer_config.filter_configs[filter].width; input_x) {
            m_input_weights[input_x] = (neuron_connection **)malloc(sizeof(neuron_connection *) * prev_layer->m_layer_config.filter_configs[filter].height);

            for (unsigned input_y = 0; input_y < prev_layer->m_layer_config.filter_configs[filter].height; ++input_y) {

                unsigned input_z = filter;

                // Set shared weight for connection
                // TODO: Check if possible to have both 2D and 3D array in struct of same variable.
                m_input_weights[input_x][input_y][input_z].weights = lc.filter_configs[input_z].m_filter->filter_weight[x][y][input_z].weights;
            }
        }
    } break;
    case OUTPUT:
        break;
    }
}

void neuron::set_output_val(float output_val) {
    m_output_val = output_val;
}

float neuron::get_output_val() {
    return m_output_val;
}

void neuron::set_output_weight(neuron *edge, unsigned pos) {
    // m_output_weights[pos] = (Connection*)malloc(sizeof(Connection));

    // init_connection(m_output_weights[pos], edge);
}
// neuron_connection *neuron::get_output_weight(unsigned x) {
//     return &m_output_weights[x];
// }

neuron_connection **neuron::get_output_weights() {
}

// std::vector<Connection> *neuron::get_output_weights() {
//     return &m_output_weights;
// }

void neuron::feed_forward(const layer &prev_layer) {
    // switch (prev_layer.fmc->layer_type) {
    // case CONVOLUTION:
    //     break;
    // case MAXPOOL:
    //     for (unsigned filter = 0; filter < prev_layer.fmc->filters; ++filter) {
    //     }
    //     break;
    // case FULLY:
    //     break;
    // case INPUT:
    //     break;
    // }
}

float neuron::random_weight() {
    return rand() / float(RAND_MAX);
}

/**
 * @brief Sets up a connection between the current neuron and a neuron in the previous layer 
 * based on the filter parameters
 * 
 * @param conn "pointer" to neuron in previous layer
 * @param lc layer config to describe the connectivity with the neuron in the previous layer.
 * @param x neuron position
 * @param y neuron position
 * @param filter neuron position (z)
 */
void neuron::init_connection(neuron_connection &conn, layer_config &lc, unsigned &x, unsigned &y, unsigned &filter) {

    switch (lc.layer_type) {
    case CONVOLUTION:
        // conn.weights->weight = random_weight();
        // conn.weights->delta_weight = 0;

        // lc.filter_configs[filter].filter->filter_weight[x][y][filter].weights->weight = random_weight();

        // Set shared weight for connection
        conn.weights = lc.filter_configs[filter].m_filter->filter_weight[x][y][filter].weights;

        // Set specific connected neuron in previous layer
        // if (lc.filter_configs[filter].padding > 0) {

        //     conn.edge =
        // }
        break;
    case FULLY:
        conn.weights->weight = random_weight();
        conn.weights->delta_weight = 0;
        break;
    }
}
neuron *neuron::get_edge(unsigned index, neuron *edge) {
    return 0;
}