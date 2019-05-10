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
    net(NetConfig &nc, cl_setup &ocl);
    Layer get_layer(unsigned index);
    void feed_forward(float *input);
    // void backpropagate(std::vector<class_object> objects); // TODO: Rewrite to use pointer instead at some point

private:
	void compute_maxpool();
	void compute_convolution(Layer &this_layer, cl::Program &program);
	void output(Layer &this_layer, cl::Program &program);
	void compute_fully_connected();
    void add_layer(Layer &config, enum type type);
    void add_filter(FilterConfig &config, int filter_num);
    void add_filter(FullNetConfig &config); // TODO: Investigate use of template instead.
    void connect_neurons(Layer &lc, FilterConfig &fc);
	int get_3d_index(unsigned x, unsigned y, unsigned z, unsigned width, unsigned depth);
	float random_weight();
	cl_setup m_ocl;
	cl_compute m_ocl_compute;
    NetConfig *m_net_config;
    bool training = false;
};

unsigned filter_size_calc(unsigned *length, unsigned filter, int padding, unsigned stride);

unsigned filter_size(unsigned &length, FilterConfig *fmc);

#endif

// Fast User-friendly Classification