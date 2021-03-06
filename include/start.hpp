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
// #include "json.hpp"
// #include <glib.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>

#include "net/net.hpp"

enum MODE { CLASSIFY,
            TRAIN };

class setup {
  public:
  setup();
  void load_cfg(std::string &cfg_file);
  void load_weights(std::string &weights_file);
  float *load_input(std::string &input_file, bool is_image);
  NetConfig *get_cfg();

  private:
  std::ifstream m_cfg_file;
  std::ifstream m_weights_file;
  std::ifstream m_input_file;
  NetConfig m_net_config;
  void allocator();
};

int options(int argc, const char *argv[]);

void show_vector_vals(std::string label, std::vector<float> &v);

#endif