/**
 * @brief 
 * 
 * @file net.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#ifndef __NET_HPP
#define __NET_HPP

#include "neuron/neuron.hpp"


class net {
public:
	void init(const std::vector<unsigned> &topology);
    void feed_forward(const std::vector<double> &inputVals);
    void back_prop(const std::vector<double> &targetVals);
    void get_results(std::vector<double> &resultVals) const;
    double get_recent_average_error(void);
    std::vector<Layer> get_net(void);
    void get_net(std::vector<Layer> &layers);
    void net_info(std::vector<Layer> **layers);
private:
	std::vector<Layer> m_layers;
    double m_error;
    double m_recent_average_error;
    static double m_recent_average_smoothing_factor;
};

#endif