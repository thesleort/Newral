/**
 * @brief 
 * 
 * @file start.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-22
 * 
 * 
 * TODO: Add msgpack.org at some point
 */

#ifndef __START_HPP
#define __START_HPP

#include <vector>
#include <iostream> 
#include <fstream>
// #include <jsoncpp/json/json.h>
#include "json.hpp"
// #include <glib.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>

#include "net/net.hpp"

enum MODE { CLASSIFY,
            TRAIN };


class load_cfg {
	public:
		load_cfg(std::string &cfg_file);
		net_config get_cfg();
	private:
		std::ifstream m_cfg_file;
		net_config m_net_config;
		void allocator();
};

class load_weights {
	public:
		load_weights(std::string &weights_file, net_config &net_config);

	private:
		std::ifstream m_weights_file;
		net_config *m_net_config;
		
};

int options(int argc, const char * argv[]);

void show_vector_vals(std::string label, std::vector<float> &v);

#endif