/**
 * @brief 
 * 
 * @file neuron.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#ifndef __NEURON_HPP
#define __NEURON_HPP

#include <vector>
#include <cstdlib>

#include "net/net.hpp"

class neuron; 
typedef struct feature_map_config;
typedef struct layer;

enum neuron_type {
	NEURON,
	FILTER
};

typedef struct neuron_filter {
	neuron_weight *weights;
} neuron_filter;

typedef struct neuron_weight {
	float weight;
	float delta_weight;
} neuron_weight;

typedef struct neuron_connection {
    neuron_weight *weights;// Array of weights from filter
	neuron ***edge;			// All input neurons
} neuron_connection;

class neuron {
public:
	neuron(layer_config &lc, unsigned x, unsigned y, unsigned filter /* y */);
	void set_output_val(float output_val);
	float get_output_val();
    void set_output_weight(neuron *edge, unsigned x);
	neuron_connection *get_output_weight(unsigned x);
	neuron_connection **get_output_weights();
	void set_input_weights(layer_config &lc);
	void set_input_weights(filter_config &fc);
	void set_input_weights(filter_config &fc, layer_config &lc);
    void set_output_weights(float weight, float delta_weight);
	void feed_forward(const layer &prev_layer);

private:
	float random_weight(); //static
	void init_connection(neuron_connection &conn, layer_config &lc, unsigned x, unsigned y, unsigned filter);
	neuron *get_edge(unsigned index, neuron *edge);
	static float eta;
	static float alpha;
	float m_gradient;
	float m_ouptut_val;
	unsigned m_num_outputs;
	neuron_connection ***m_input_weights;		// Same as input, but with weights as well.
	neuron_connection ***m_output_axon;			// Only for fully connected?
};

#endif