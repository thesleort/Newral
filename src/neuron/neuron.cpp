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
 * @param fmc 
 * @param x 
 * @param y 
 * @param filter 
 */
neuron::neuron(layer_config &lc, unsigned x, unsigned y, unsigned filter) {

    //m_output_weights = (Connection ***)malloc(sizeof(Connection *) *
    //filter_size(fmc.layer_next->fmc->filters, fmc.layer_next->fmc));

    switch (lc.layer_type) {
    case INPUT: {
        // layer *next_layer = lc.layer_next;
        // unsigned num_filters = next_layer->layer_config.num_filters; // Determines how many neurons each neuron should connect to in next layer.

        // // Allocate space for all output edges and initialize these with correct weights
        // m_output_axon = (neuron_connection ***)malloc(sizeof(neuron_connection *) * next_layer->layer_config.width);
        // for (unsigned x = 0; x < next_layer->layer_config.width; ++x) {
        //     m_output_axon[x] = (neuron_connection **)malloc(sizeof(neuron_connection *) * next_layer->layer_config.height);
        //     for (unsigned y = 0; y < next_layer->layer_config.height; ++y) {
        //         m_output_axon[x][y] = (neuron_connection *)malloc(sizeof(neuron_connection *) * next_layer->layer_config.num_filters);
        //         for (unsigned filter = 0; filter < num_filters; ++filter) {
        //             init_connection(m_output_axon[x][y][filter], next_layer->layer_config, x, y, filter);
        //         }
        //     }
        // }
        break;
    }

    case CONVOLUTION: {
        layer *prev_layer = lc.layer_prev;
        unsigned num_filters = prev_layer->layer_config.depth; // .num_filters maybe?

        m_input_weights = (neuron_connection ***)malloc(sizeof(neuron_connection *) * prev_layer->layer_config.width);
        for (unsigned input_x = 0; input_x < prev_layer->layer_config.width; ++input_x) {
            m_input_weights[input_x] = (neuron_connection **)malloc(sizeof(neuron_connection *) * prev_layer->layer_config.height);
            for (unsigned input_y = 0; input_y < prev_layer->layer_config.height; ++input_y) {
                m_input_weights[input_x][input_y] = (neuron_connection *)malloc(sizeof(neuron_connection *) * prev_layer->layer_config.height);
                for (unsigned input_z = 0; input_z < prev_layer->layer_config.depth; ++input_z) {
                    init_connection(m_input_weights[input_x][input_y][input_z], prev_layer->layer_config, x, y, filter);
                }
            }
        }

        break;
    }
    // case CONVOLUTION: {
    //     unsigned num_filters_next = lc.layer_next->layer_config.num_filters;
    //     unsigned num_filters_prev = lc.layer_prev->layer_config.depth;

    // 	/**
    // 	 * @brief Add input neurons
    // 	 *
    // 	 */
    //     // Allocate space for input neurons.
    //     m_input_weights = (neuron **)malloc(sizeof(neuron *) * fmc.layer_prev->fmc->size * 2);
    // 	// Set input neurons.
    //     for (unsigned offset_x = x; offset_x < fmc.layer_prev->fmc->size + x; ++offset_x) {
    //         for (unsigned offset_y = y; offset_y < fmc.layer_prev->fmc->size + y; ++offset_y) {
    //             m_input_weights[(1 + offset_x - x) * (offset_y - y)] =
    //                 &fmc.layer_prev->filters[filter].neurons[offset_x][offset_y];
    //         }
    //     }

    // 	/**
    // 	 * @brief Add output weights (and edges) for neuron.
    // 	 *
    // 	 */
    // 	// Allocate space for output weights.
    // 	m_output_weights = (neuron_connection *)malloc(sizeof(neuron_connection) * fmc.layer_next->fmc->filters);
    // 	// Initialize output weights.
    // 	//for( unsigned)
    //     break;
    // }
    case FULLY:
        break;
    case MAXPOOL:
        break;
    case OUTPUT:
    }
    // if (fmc.layer_type != INPUT && fmc.layer_type != CONVOLUTION) {
    //     m_input_weights = (neuron ***)malloc(sizeof(neuron *));
    //     m_input_weights[0] = (neuron **)malloc(sizeof(neuron *));
    //     m_input_weights[0][0] = (neuron *)malloc(sizeof(neuron *));

    // 	unsigned calc_x = fmc.layer_prev->fmc->stride;
    // 	unsigned calc_y;

    //     m_input_weights[0][0] = fmc.layer_prev
    //                                 ->filters[filter]
    //                                 .neurons[offset_x][offset_y]
    //                                 .get_output_weight(x, y)
    //                                 ->edge;
    // } else if (fmc.layer_type == FULLY) {
    //     m_input_weights = (neuron ***)malloc(sizeof(neuron *) * fmc.size);

    //     // Make appropriate filter from previous layer point to this neuron.
    //     for (unsigned offset_x = 0; offset_x < fmc.size + 2 * fmc.padding;
    //          (offset_x = offset_x + fmc.stride)) {

    //         m_input_weights[x] = (neuron **)malloc(sizeof(neuron) * fmc.size);
    //         for (unsigned offset_y = 0; offset_y < fmc.size + 2 * fmc.padding;
    //              (offset_y = offset_y + fmc.stride)) {
    //             //Connection conn2{.weight = random_weight(), .delta_weight = 0, .edge = this};
    //             fmc.layer_prev
    //                 ->filters[filter]
    //                 .neurons[offset_x][offset_y]
    //                 .set_output_weight(this, x, y);

    //             m_input_weights[x][y] = fmc.layer_prev
    //                                         ->filters[filter]
    //                                         .neurons[offset_x][offset_y]
    //                                         .get_output_weight(x, y)
    //                                         ->edge;
    //         }
    //     }
    // }
}

void neuron::set_output_val(float output_val) {
    m_ouptut_val = output_val;
}

float neuron::get_output_val() {
    return m_ouptut_val;
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

void neuron::init_connection(neuron_connection &conn, layer_config &lc, unsigned x, unsigned y, unsigned filter) {

    switch (lc.layer_type) {
    case CONVOLUTION:
        conn.weights->weight = random_weight();
        conn.weights->delta_weight = 0;

        // conn.weights->weight = lc.filter_configs[filter].filter->filter_weight[x][y][filter].weights->weight;
        // conn.weights->delta_weight = lc.filter_configs[filter].filter->filter_weight[x][y][filter].weights->delta_weight;
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