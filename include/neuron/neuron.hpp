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


class neuron; 


typedef struct connection {
    double weight;
    double delta_weight;
} Connection;

class neuron {
public:
	neuron(feature_map_config *fmc);
	void set_output_val(double output_val);
	double get_output_val();

    void set_output_weight(Connection &connection, unsigned index);
    void set_output_weights(double weight, double delta_weight);
    std::vector<Connection> *get_output_weights();

private:
	static double random_weight() {
        return rand() / double(RAND_MAX);
    };
	static double eta;
	static double alpha;
	double m_gradient;
	double m_output_val;
	std::vector<Connection> m_output_weights;
};

#endif