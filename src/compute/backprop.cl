
__kernel void fully_connected_relu(
    __global float *input_layer,
    __constant float *neuron_weights,
    __constant float *targets,
    __local float *local_neurons,
    __local float *local_weights,
    __local float *local_targets,
    int input_size) {
    
    float net_error = 0;
    float delta = 0;
    float recent_avg_error = 0;
    // Copy data into workgroup
    for (unsigned i = 0; i < input_size; i += local_size) {
        for (unsigned j = 0; j < local_size; ++j) {
            if (local_id * i + j < input_size) {
                local_neurons[local_id + j] = input_layer[i + j];
                local_weights[local_id + j] = neuron_weights[i + j];
                local_targets[local_id + j] = targets[i + j];
            }
        }

        for (unsigned j = 0; j < local_size; ++j) {
            if (i + j < input_size) {
                // sum += local_neurons[i + j] * local_weights[i + j];
                delta = local_targets[i + j] - local_neurons[i + j];

                // net_error += delta * delta
                net_error = fma(delta, delta, net_error);

                // sum = fma(local_neurons[i + j], local_weights[i + j], sum);
                // if(local_neurons[i + j ] > local_weights[i + j]) {
                // }
                // TODO: Try FMA and vector types
            }
        }
        barrier(CLK_LOCAL_MEM_FENCE);

        net_error /= input_size - 1;
        net_error = sqrt(net_error);

        for(unsigned j = 0; j < local_size; ++j) {
            
        }
    }
    barrier(CLK_LOCAL_MEM_FENCE);
}