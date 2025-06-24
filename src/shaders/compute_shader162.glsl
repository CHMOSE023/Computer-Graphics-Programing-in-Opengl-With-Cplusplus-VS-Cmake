#version 450

layout(local_size_x = 1, local_size_y = 1) in;
layout(binding = 0, rgba8) uniform image2D output_texture;

uniform ivec2 image_size;

void main() {
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);

    if (pixel.x >= image_size.x || pixel.y >= image_size.y)
        return;

    imageStore(output_texture, pixel, vec4(1.0, 0.5, 1.0, 1.0)); // Blanco
}
