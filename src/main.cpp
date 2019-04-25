/**
 * @brief 
 * 
 * @file main.c
 * @author Troels Blicher Petersen
 * @date 2018-03-20
 */

#include <iostream>
#include "operator/functions.hpp"
#include "start.hpp"

// #pragma OPENCL EXTENSION cl_khr_fp64 : enable

int main(int argc, const char * argv[]) {
	std::cout << "Starting Newral...\n";
	std::string cfg_file(argv[1]);
	std::string weights_file(argv[2]);
	std::string input_file(argv[3]);

	// std::vector<std::vector<std::vector<float>>> *input;

	std::cout << "Preparing setup...\n";
	setup network_setup;
	
	std::cout << "Preparing OpenCL...\n";
	cl_setup compute;
	compute.setup(CL_DEVICE_TYPE_GPU);
	
	// m_setup = new setup::setup();
	std::cout << "Loading config...\n";
	network_setup.load_cfg(cfg_file);
	std::cout << "Done.\n";

	std::cout << "Loading filters...\n";
	network_setup.load_weights(weights_file);
	std::cout << "Done.\n";

	// std::cout << "Loading input...\n";
	// network_setup.load_input(input_file, false);
	// std::cout << "Done.\n";

	std::cout << "Setting up network...\n";
	net network(*network_setup.get_cfg(), compute);
	std::cout << "Done.\n";
	// network	

	std::cout << "Classifying...\n";
	network.feed_forward(network_setup.load_input(input_file, false));
	std::cout << "Done.\n";
	// return 0;
}