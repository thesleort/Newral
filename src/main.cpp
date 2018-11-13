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

int main(int argc, const char * argv[]) {
	std::cout << "Starting Newral...\n";
	std::string cfg_file(argv[1]);
	std::string weights_file(argv[2]);

	setup network_setup;
	
	// m_setup = new setup::setup();
	network_setup.load_cfg(cfg_file);
	network_setup.load_weights(weights_file);
	// network

}