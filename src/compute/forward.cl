
// TODO: Change floats to vectors for hardware optimization
__kernel void convolution(
		__global float *input_layer, 
		int layer_width,
		int layer_height,
		int layer_depth,
		__constant float *filter, 
		int filter_width,
		int filter_height,
		int filter_depth,
		__global float *output_layer,
		int filter_padding,
		int filter_stride,
		int filter_num,
		float bias) {
	int column = get_local_id(0);
	int row = get_local_id(1);
	int depth = get_local_id(2);

	float sum = 0;

	int filter_id = 0;

	int half_width = (int)(filter_width / 2);
	int half_height = (int)(filter_height / 2);

	int output_column;
	int output_row;
	int output_depth;

	int output_height;
	int output_width;

	int3 coord; 

				
	bool padX;
	bool padY;

	for(int z = 0; z < layer_depth; ++z) {
		coord.z = z;

		for(int y = -half_height; y <= half_height; ++y) {
			coord.y = -filter_padding + (row * filter_stride) + y + half_height;

			for(int x = -half_width; x <= half_width; ++x) {
				coord.x = -filter_padding + (column * filter_stride) + x + half_width;

				padX = false;
				padY = false;

				padX = (coord.x >= 0 && coord.x < layer_width ) ? false : true;
				padY = (coord.y >= 0 && coord.y < layer_height) ? false : true;

				if(!padX && !padY) {
					sum += 
					input_layer[
						coord.x + 
						coord.y * 
						layer_width + 
						coord.z * 
						layer_width * 
						layer_height] * 
						filter[filter_id];
				}
				filter_id++;
			}
		}
	}
	output_column = column;
	output_row = row;
	output_depth = filter_num;

	output_width = (layer_width - filter_width + 2 * filter_padding) / filter_stride + 1;
	output_height = (layer_height - filter_height + 2 * filter_padding) / filter_stride + 1;

	output_layer[output_column + output_row * output_width + filter_num * output_width * output_height] = sum + bias;
}

__kernel void maxpool(
	__global float *input_layer, 
	int layer_width,
	int layer_height,
	int layer_depth,
	__global float *output_layer,
	int pool_width,
	int pool_height,
	int pool_slice,
	int pool_stride,
	int pool_padding
) {

}


__kernel void simple(
	__global float *output_layer
) {
	output_layer[0] = 1.0f;
}