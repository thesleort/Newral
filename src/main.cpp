/**
 * @brief 
 * 
 * @file main.c
 * @author Troels Blicher Petersen
 * @date 2018-03-20
 */

#include <iostream>
#include <boost/program_options.hpp>
#include "operator/functions.hpp"
#include "start.hpp"
// #pragma OPENCL EXTENSION cl_khr_fp64 : enable

namespace 
{ 
  const size_t ERROR_IN_COMMAND_LINE = 1; 
  const size_t SUCCESS = 0; 
  const size_t ERROR_UNHANDLED_EXCEPTION = 2; 
 
} // namespace 

int main(int argc, const char *argv[]) {

  std::string cfg_file;
  std::string weights_file;
  std::string input_file;


  try {
    namespace po = boost::program_options;

    po::options_description desc("Options");

    desc.add_options()
      ("help,h","Show parameters")
      ("config,c", po::value<std::string>(&cfg_file)->required(), "Configuration file")
      ("input,i", po::value<std::string>(&input_file)->required(), "Input file")
      ("weights,w", po::value<std::string>(&weights_file)->required(), "Weights file")
      ("targets,t", "Target classification file");

      po::variables_map v_map;

      try {
        po::store(po::parse_command_line(argc, argv, desc), v_map);

        if (v_map.count("help")) { 
          std::cout << "Basic Command Line Parameter App" << std::endl 
                    << desc << std::endl; 
          return SUCCESS; 
        } 
 
        po::notify(v_map);
        
      } catch(po::error& e) {
        std::cerr << "ERROR: " << e.what() << std::endl << std::endl; 
        std::cerr << desc << std::endl; 
        return ERROR_IN_COMMAND_LINE; 
      }

      std::cout << "Starting Newral...\n";

      std::cout << "Preparing setup...\n";
      setup network_setup;

      std::cout << "Preparing OpenCL...\n";
      cl_setup compute;
      compute.setup(CL_DEVICE_TYPE_GPU);

      std::cout << "Loading config...\n";
      network_setup.load_cfg(cfg_file);
      std::cout << "Done.\n";

      std::cout << "Setting up network...\n";
      net network(*network_setup.get_cfg(), compute);
      std::cout << "Done.\n";

      std::cout << "Loading filters...\n";
      network_setup.load_weights(weights_file);
      std::cout << "Done.\n";

      std::cout << "Classifying...\n";
      network.feed_forward(network_setup.load_input(input_file, false));
      std::cout << "Done.\n";
  } catch(std::exception& e) {
    std::cerr << "Unhandled Exception reached the top of main: " 
              << e.what() << ", application will now exit" << std::endl; 
    return ERROR_UNHANDLED_EXCEPTION; 
  }


  // std::vector<std::vector<std::vector<float>>> *input;


  // m_setup = new setup::setup();




  // std::cout << "Loading input...\n";
  // network_setup.load_input(input_file, false);
  // std::cout << "Done.\n";

  // network


  // return 0;
  exit(EXIT_SUCCESS);
}