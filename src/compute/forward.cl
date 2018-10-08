

__kernel void convolution(
		__global float *input_layer, 
		__constant int layer_height,
		__constant int layer_width,
		__constant int layer_depth,
		__constant float *filter, 
		__constant int filter_width,
		__constant int filter_height,
		__constant int filter_depth,
		__global float *output_layer,
		__constant int filter_padding,
		__constant int filter_stride,
		__constant int filter_num) {
	int column = get_global_id(0);
	int row = get_global_id(1);
	int depth = get_global_id(2);

	int sum = 0;

	int filter_id = 0;

	int half_width = (int)(filter_width / 2);
	int half_height = (int)(filter_height / 2);

	int output_column;
	int output_row;
	int output_depth;

	int output_height;
	int output_width;

	int2 coords; 

	bool pad; 

	for(int x = -half_width; x <= half_width; ++x) {
		coords.x = x + column;
		for(int y = -half_height; y <= half_height; ++y) {
			coords.y = y + row;
			for(int z = 0; z < filter_depth; ++z) {
				pad = false;
				coords.x = (coords.x < 0) ? 0 : coords.x;
				coords.y = (coords.y < 0) ? 0 : coords.y;
				coords.x = (coords.x >= layer_width) ? layer_width-1 : coords.x;
				coords.y = (coords.y >= layer_height) ? layer_height-1 : coords.y;

				pad = (coords.x < 0 || coords.x >= layer_width) ? true : false;
				pad = (coords.y < 0 || coords.y >= layer_height) ? true : false;

				if(!pad) {
					sum += input_layer[coords.x + coords.y * layer_width + z * layer_width * layer_height] *
						filter[filter_id++];
				}
			}
		}
	}

	output_column = column - filter_stride;
	output_row = column - filter_stride;
	output_depth = filter_num;

	output_width = (layer_width - filter_width + 2 * filter_padding) / filter_stride + 1;
	output_height = (layer_height - filter_height + 2 * filter_padding) / filter_stride + 1;

	output_layer[output_column + output_row * output_width + filter_num * output_width * output_height] = sum;
}