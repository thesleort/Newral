/**
 * @brief 
 * 
 * @file model.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */
#ifndef __MODEL_HPP
#define __MODEL_HPP

#include <string>
#include <vector>

#include "net/net.hpp"


enum LABEL { LAYER,
             NEURON };

class model {
public:
    void save_weights(net &net, std::string filename);
    net load_model(std::string filename);
    void load_weights(net &net, std::fstream &line);

private:
    std::vector<std::string> split(const std::string &text, char sep);
    LABEL m_label;
};

#endif