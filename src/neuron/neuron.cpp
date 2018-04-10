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

neuron::neuron(feature_map_config *fmc, unsigned x, unsigned y, unsigned filter) {

    m_output_weights = (Connection ***)malloc(sizeof(Connection *) * filter_size(fmc->layer_next->fmc->filters, fmc->layer_next->fmc));

    if (fmc->layer_type != INPUT) {

        m_input_weights = (neuron ***)malloc(sizeof(neuron *) * fmc->size);

        // Make appropriate filter from previous layer point to this neuron.
        for (unsigned offset_x = 0; offset_x < fmc->size + 2 * fmc->padding; (offset_x = offset_x + fmc->stride)) {

            m_input_weights[x] = (neuron **)malloc(sizeof(neuron) * fmc->size);
            for (unsigned offset_y = 0; offset_y < fmc->size + 2 * fmc->padding; (offset_y = offset_y + fmc->stride)) {
                //Connection conn2{.weight = random_weight(), .delta_weight = 0, .edge = this};
                fmc->layer_prev->filters[filter].neurons[offset_x][offset_y].set_output_weight(this, x, y);
                m_input_weights[x][y] = fmc->layer_prev->filters[filter].neurons[offset_x][offset_y].get_output_weight(x, y)->edge;
            }
        }
    }
}

void neuron::set_output_val(double output_val) {
    m_output_val = output_val;
}

double neuron::get_output_val() {
    return m_output_val;
}

void neuron::set_output_weight(neuron *edge, unsigned x, unsigned y) {
	m_output_weights[x][y] = (Connection*)malloc(sizeof(Connection));

	init_connection(m_output_weights[x][y], edge);
	
}

Connection *neuron::get_output_weight(unsigned x, unsigned y) {
	return m_output_weights[x][y];
}

// std::vector<Connection> *neuron::get_output_weights() {
//     return &m_output_weights;
// }

double neuron::random_weight() {
    return rand() / double(RAND_MAX);
}

void neuron::init_connection(Connection *conn, neuron *edge) {
	conn->weight = random_weight();
	conn->delta_weight = 0;
	conn->edge = edge;
}