#include<iostream>
#include<glm/glm.hpp>

int main() {
    glm::mat2x2 matrix(5.0f);

    std::cout << matrix[0][0] << " " << matrix[0][1] << std::endl;
    std::cout << matrix[1][0] << " " << matrix[1][1] << std::endl;

    return 0;
}