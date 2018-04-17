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
    double weight;
    double delta_weight;
	unsigned num_edges;
	neuron *edge;
} Connection;

class neuron {
public:
	neuron(feature_map_config &fmc, unsigned x, unsigned y, unsigned filter);
	void set_output_val(double output_val);
	double get_output_val();
    void set_output_weight(neuron *edge, unsigned x, unsigned y);
	Connection *get_output_weight(unsigned x, unsigned y);
	Connection **get_output_weights();
    void set_output_weights(double weight, double delta_weight);
	void feed_forward(const layer &prev_layer);

private:
	double random_weight(); //static
	void init_connection(Connection &conn, neuron *edge);
	static double eta;
	static double alpha;
	double m_gradient;
	double m_output_val;
	unsigned m_num_outputs;
	Connection ***m_output_weights;		// Same as input, but with weights as well
	neuron ***m_input_weights;
};

#endif