/**
 * @brief 
 * 
 * @file text.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-22
 */

#ifndef __MODULE_TEXT_HPP
#define __MODULE_TEXT_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

class training_data {
public:
    training_data(const std::string filename);
    bool is_EOF(void) { return m_training_data_file.eof(); }
    void get_topology(std::vector<unsigned> &topology);

    // Returns the number of input values read from the file:
    unsigned get_next_inputs(std::vector<double> &input_vals);
    unsigned get_target_outputs(std::vector<double> &target_output_vals);

private:
    std::ifstream m_training_data_file;
};

#endif