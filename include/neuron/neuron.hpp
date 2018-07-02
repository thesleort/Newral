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


typedef struct connection {
    float weight;
    float delta_weight;
	neuron *edge;			// Where does this connection come from/go to
} Connection;

class neuron {
public:
	neuron(feature_map_config &fmc, unsigned x, unsigned y, unsigned filter);
	void set_output_val(float output_val);
	float get_output_val();
    void set_output_weight(neuron *edge, unsigned x);
	Connection *get_output_weight(unsigned x);
	Connection **get_output_weights();
    void set_output_weights(float weight, float delta_weight);
	void feed_forward(const layer &prev_layer);

private:
	float random_weight(); //static
	void init_connection(Connection &conn, neuron *edge);
	neuron *get_edge(unsigned index, neuron *edge);
	static float eta;
	static float alpha;
	float m_gradient;
	float m_ouptut_val;
	unsigned m_num_outputs;
	Connection *m_output_weights;		// Same as input, but with weights as well.
	neuron **m_input_weights;			// Only for fully connected?
};

#endif