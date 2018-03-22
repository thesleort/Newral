/**
 * @brief 
 * 
 * @file neuron.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#ifndef __NEURON_HPP
#define __NEURON_HPP

#include <vector>
#include <cstdlib>


class neuron; 

typedef std::vector<neuron> Layer;

typedef struct connection {
    double weight;
    double delta_weight;
} Connection;

class neuron {
public:
    neuron(unsigned num_outputs, unsigned my_index);
    void set_output_val(double val);
    double get_output_val(void) const;
    void feed_forward(const Layer &prev_layer);
    void calc_output_gradients(double target_val);
    void calc_hidden_gradients(const Layer &next_layer);
    void update_input_weights(Layer &prev_layer);
    std::vector<Connection> get_output_weights();
    void set_output_weight(Layer &prev_layer);
    void set_output_weight(Connection &connection, unsigned index);
    void set_output_weights(double weight, double delta_weight);

private:
    static double random_weight(void) {
        return rand() / double(RAND_MAX);
    };
    static double transfer_function(double x);
    static double transfer_function_derivative(double x);
    static double eta;  // [0.0..1.0] Overall net training rate
    static double alpha;// [0.0..n] multiplier of last weight change (momentum)
    double sum_DOW(const Layer &next_layer) const;
    double m_output_val;
    double m_gradient;
    unsigned m_my_index;
    unsigned m_output_index = 0;
    std::vector<Connection> m_output_weights;
};

#endif