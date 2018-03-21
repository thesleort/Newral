/**
 * @brief 
 * 
 * @file model.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */
#ifndef __model_hpp
#define __model_hpp

#include <string>
#include <vector>

#include "../neural-network/net.h"

#ifndef LUNEAR_MODEL_H
#define LUNEAR_MODEL_H

#endif //LUNEAR_MODEL_H

enum Label { layer, neuron };

class Model {
public:
    void saveWeights(Net &net, std::string filename);
    Net loadModel(std::string filename);
    void loadWeights(Net &net, std::fstream &line);

private:
    std::vector<std::string> split(const std::string &text, char sep);
    Label m_label;
};

#endif