/**
 * @brief 
 * 
 * @file load.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-20
 */

#ifndef __MODULE_DATA_HPP
#define __MODULE_DATA_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#ifdef __APPLE__
    #include "OpenCL/opencl.h"
#else
    #include <CL/cl.h>
#endif

typedef std::vector<float> t_vals;

class data {
public:
    data(const std::string filename);
    bool is_eof(void);
    // Returns the number of input values read from the file:
    unsigned get_next_inputs(t_vals &input_vals);
    unsigned get_target_outputs(t_vals &target_output_vals);

private:
    std::ifstream m_training_data_file;
};

#endif