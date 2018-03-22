/**
 * @brief 
 * 
 * @file model.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#include <iostream>
#include <fstream>

#include "model/model.hpp"

void model::save_weights(net &net, std::string filename) {

    std::vector<Layer> layers;
    std::string line;
    std::vector<std::string> strings;
    net.get_net(layers);
    // If no model-filename has been specified
    if(filename.size() == 0) {
        std::cin >> filename;
    }

    std::fstream file;
    file.open(filename);

    // Start by specifying meta data about model
//    file << "[Model]" << std::endl << "topology";
//    for (int layer = 0; layer < layers.size(); ++layer) {
//        file << " " << layers[layer].size();
//    }

//    file << std::endl << std::endl;

    // Save weights for each neuron
    while( getline(file, line) ) {
        strings = split(line, ' ');
        std::cout << "---------" << line << std::endl;
        if( strings[0].compare("[Weights]") == 0) {
            std::cout << "TRUE" << std::endl;
            for (int layer = 0; layer < layers.size() - 1; ++layer) {

                file << "Layer " << layer << std::endl;
                for (int neuron = 0; neuron < layers[layer].size(); ++neuron) {

                    file << "neuron " << neuron << std::endl;
                    for (int weight = 0; weight <= layers[layer][neuron].get_output_weights().size(); ++weight) {
                        file << std::fixed
                             << layers[layer][neuron].get_output_weights()[weight].weight
                             << " "
                             << std::fixed << layers[layer][neuron].get_output_weights()[weight].delta_weight
                             << std::endl;
//                std::cout << std::fixed
//                          << layers[layer][neuron].getOutputWeights()[weight].weight
//                          << " "
//                          << std::fixed << layers[layer][neuron].getOutputWeights()[weight].deltaWeight
//                          << std::endl;
                    }
                }
            }
            break;
        }
    }
    file.close();
}

net model::load_model(std::string filename) {
    // If no model-filename has been specified

    net net;

    std::string line;
    std::vector<std::string> strings;

    std::vector<unsigned> topology;

    std::fstream file;
    file.open(filename);
    while( getline(file, line) ) {

        strings = split(line, ' ');

        // If model
        if ( strings[0] == "[Model]" ) {
            getline(file, line);
            strings = split(line, ' ');
            for (unsigned string = 1; string < strings.size(); ++string) {
                topology.push_back(std::stoi(strings[string]));
            }
            net.init(topology);
        }

        if ( strings[0] == "[Weights]") {

            std::cout << "-------------DEBUG" << std::endl;
            load_weights(net, file);

            std::cout << "-------------DEBUG2" << std::endl;
        }
    }
    file.close();
    return net;
}

void model::load_weights(net &net, std::fstream &stream) {
    // If no model-filename has been specified

    std::vector<std::string> strings;
    std::string line;
    int layerNum = 0;
    int neuronNum = 0;

    std::vector<Layer> *currentLayer;

    net.net_info(&currentLayer);
    std::cout << "------------------ " << currentLayer << std::endl;

    std::cout << "-------------DEBUG" << std::endl;
    std::cout << "-------------DEBUG" << std::endl;
    std::cout << "-------------DEBUG" << std::endl;

    int i = 0;
    while( getline(stream, line) ) {
        strings = split(line, ' ');
        // If layer
        std::cout << strings[1] << " " << strings[1] << std::endl;
        i = strings.size();
        if (strings[0].compare("Layer") == 0) {
            layerNum = std::stoi(strings[1]);
            //std::cout << "DBG layer " << layerNum << std::endl;
            neuronNum = 0;
            m_label = LAYER;
        }

        if (strings[0].compare("neuron") == 0) {
            neuronNum = std::stoi(strings[1]);

            std::cout << "DBG neuron " << neuronNum << " " << std::endl;
            m_label = NEURON;
        }

        if (m_label == NEURON && strings[0].compare("neuron") != 0 && strings[0].compare("")) {
            //std::cout << "DBG 3 " << neuronNum << std::endl;
//            connection.weight = std::stod(strings[0]);
            //std::cout << connection.weight << std::endl;
            //std::cout << line << " size " << strings.size() << std::endl;
//            connection.deltaWeight = std::stod(strings[1]);
            //std::cout << std::stod(strings[1]) << std::endl;
            //std::cout << connection.weight << " " << connection.deltaWeight << std::endl;
            //net.getNet()[layerNum][neuronNum].setOutputWeight(connection);
//            currentLayer[layerNum][neuronNum].setOutputWeight(std::stod(strings[0], std::stod(strings[1])));

            std::cout << std::stod(strings[0]) << " " << std::stod(strings[1]) <<  std::endl;
            currentLayer[0][layerNum][neuronNum].set_output_weights(std::stod(strings[0]), std::stod(strings[1]));
//            std::cout << "---------- " << currentLayer[layerNum][neuronNum].getOutputWeights()[0].weight << std::endl;



            //std::cout << std::endl;
        }
        ++i;
    }
}

std::vector<std::string> model::split(const std::string &text, char sep) {
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
    return tokens;
}