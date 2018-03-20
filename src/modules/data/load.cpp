/**
 * @brief Load text files into data.
 * 
 * @file load.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-20
 */

#include "load.hpp"

data::data(const std::string filename) {
    m_training_data_file.open(filename.c_str());
}

bool data::is_eof(void) {
    return m_training_data_file.eof();
}

void data::get_topology(std::vector<unsigned> &topology) {
    std::string line;
    std::string label;

    getline(m_training_data_file, line);
    std::stringstream ss(line);
    ss >> label;
    if (this->is_eof() || label.compare("topology:") != 0) {
        abort();
    }

    while (!ss.eof()) {
        unsigned n;
        ss >> n;
        topology.push_back(n);
    }

    return;
}

unsigned data::get_next_inputs(t_vals &input_vals) {
    input_vals.clear();

    std::string line;
    getline(m_training_data_file, line);
    std::stringstream ss(line);

    std::string label;
    ss >> label;
    if (label.compare("in:") == 0) {
        double oneValue;

        while (ss >> oneValue)
            input_vals.push_back(oneValue);
    }

    return input_vals.size();
}

unsigned data::get_target_outputs(t_vals &target_output_vals) {
    target_output_vals.clear();

    std::string line;
    getline(m_training_data_file, line);
    std::stringstream ss(line);

    std::string label;
    ss >> label;
    if (label.compare("out:") == 0) {
        double oneValue;

        while (ss >> oneValue)
            target_output_vals.push_back(oneValue);
    }

    return target_output_vals.size();
}