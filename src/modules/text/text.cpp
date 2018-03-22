/**
 * @brief 
 * 
 * @file text.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-22
 */

#include "text.hpp"

void training_data::get_topology(std::vector<unsigned> &topology) {
    std::string line;
    std::string label;

    getline(m_training_data_file, line);
    std::stringstream ss(line);

    ss >> label;
    if (this->is_EOF() || label.compare("topology:") != 0) {
        abort();
    }

    while (!ss.eof()) {
        unsigned n;
        ss >> n;
        topology.push_back(n);
    }

    return;
}

training_data::training_data(const std::string filename) {
    m_training_data_file.open(filename.c_str());
}

unsigned training_data::get_next_inputs(std::vector<double> &input_vals) {
    input_vals.clear();

    std::string line;
    getline(m_training_data_file, line);
    std::stringstream ss(line);

    std::string label;
    ss >> label;
    if (label.compare("in:") == 0) {
        double one_value;
        while (ss >> one_value) {
            input_vals.push_back(one_value);
        }
    }

    return static_cast<unsigned int>(input_vals.size());
}

unsigned training_data::get_target_outputs(std::vector<double> &target_output_vals) {
    target_output_vals.clear();

    std::string line;
    getline(m_training_data_file, line);
    std::stringstream ss(line);

    std::string label;
    ss >> label;
    if (label.compare("out:") == 0) {
        double one_value;
        while (ss >> one_value) {
            target_output_vals.push_back(one_value);
        }
    }

    return target_output_vals.size();
}