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
neuron::neuron(feature_map_config &fmc, unsigned x, unsigned y, unsigned filter) {

    //m_output_weights = (Connection ***)malloc(sizeof(Connection *) * 
	//filter_size(fmc.layer_next->fmc->filters, fmc.layer_next->fmc));

    switch (fmc.layer_type) {
    case INPUT: {
        unsigned num_filters = fmc.layer_next->fmc->filters;

        m_output_weights = (Connection *)malloc(sizeof(Connection) * num_filters);
        for (unsigned i = 0; i < num_filters; ++i) {
            init_connection(m_output_weights[i], NULL);
        }
        break;
    }
    case CONVOLUTION: {
        unsigned num_filters_next = fmc.layer_next->fmc->filters;
        unsigned num_filters_prev = fmc.layer_prev->fmc->filters;

		/**
		 * @brief Add input neurons
		 * 
		 */
        // Allocate space for input neurons.
        m_input_weights = (neuron **)malloc(sizeof(neuron *) * fmc.layer_prev->fmc->size * 2);
		// Set input neurons.
        for (unsigned offset_x = x; offset_x < fmc.layer_prev->fmc->size + x; ++offset_x) {
            for (unsigned offset_y = y; offset_y < fmc.layer_prev->fmc->size + y; ++offset_y) {
                m_input_weights[(1 + offset_x - x) * (offset_y - y)] =
                    &fmc.layer_prev->filters[filter].neurons[offset_x][offset_y];
            }
        }

		/**
		 * @brief Add output weights (and edges) for neuron.
		 * 
		 */
		// Allocate space for output weights.
		m_output_weights = (Connection *)malloc(sizeof(Connection) * fmc.layer_next->fmc->filters);
		// Initialize output weights.
		//for( unsigned)
        break;
    }
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

Connection *neuron::get_output_weight(unsigned x) {
    return &m_output_weights[x];
}

Connection **neuron::get_output_weights() {
}

// std::vector<Connection> *neuron::get_output_weights() {
//     return &m_output_weights;
// }

void neuron::feed_forward(const layer &prev_layer) {
    switch (prev_layer.fmc->layer_type) {
    case CONVOLUTION:
        break;
    case MAXPOOL:
        for (unsigned filter = 0; filter < prev_layer.fmc->filters; ++filter) {
        }
        break;
    case FULLY:
        break;
    case INPUT:
        break;
    }
}

float neuron::random_weight() {
    return rand() / float(RAND_MAX);
}

void neuron::init_connection(Connection &conn, neuron *edge) {
    conn.weight = random_weight();
    conn.delta_weight = 0;
    conn.edge = edge;
}

neuron *neuron::get_edge(unsigned index, neuron *edge) {
    return 0;
}