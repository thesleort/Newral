/**
 * @brief 
 * 
 * @file net.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#ifndef __NET_HPP
#define __NET_HPP

#include <vector>
#include <CL/cl.hpp>
#include "compute/cl_prepare.hpp"
#include "compute/cl_compute.hpp"
#include "netconfig.hpp"

// #include "neuron/neuron.hpp"



/**
 * @brief 
 * 
 */
class net {
public:
    net(net_config &nc, cl_setup &ocl);
    layer get_layer(unsigned index);
    void feed_forward(float *input);
    void backpropagate(std::vector<class_object> objects); // TODO: Rewrite to use pointer instead at some point

private:
	void compute_maxpool();
	void compute_convolution(layer &this_layer, cl::Program &program);
	void output(layer &this_layer, cl::Program &program);
	void compute_fully_connected();
    void add_layer(layer &config, enum type type);
    void add_filter(filter_config &config);
    void connect_neurons(layer &lc, filter_config &fc);
	int get_3d_index(unsigned x, unsigned y, unsigned z, unsigned width, unsigned depth);
	float random_weight();
	cl_setup m_ocl;
	cl_compute m_ocl_compute;
    net_config m_net_config;
};

unsigned filter_size_calc(unsigned *length, unsigned filter, int padding, unsigned stride);

unsigned filter_size(unsigned &length, filter_config *fmc);

#endif

// Fast User-friendly Classification