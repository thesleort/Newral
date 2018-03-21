/**
 * @brief 
 * 
 * @file load.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-20
 */

#ifndef __data_hpp
#define __data_hpp

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#ifdef __APPLE__
    #include "OpenCL/opencl.h"
#else
    #include "CL/cl.h"
#endif

typedef std::vector<double> t_vals;

class data {
public:
    data(const std::string filename);
    bool is_eof(void);
    void get_topology(std::vector<unsigned> &topology);

    // Returns the number of input values read from the file:
    unsigned get_next_inputs(t_vals &input_vals);
    unsigned get_target_outputs(t_vals &target_output_vals);

private:
    std::ifstream m_training_data_file;
};

#endif