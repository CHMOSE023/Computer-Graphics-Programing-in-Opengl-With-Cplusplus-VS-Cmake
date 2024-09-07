#include<iostream>
#include<glm/glm.hpp>

int main() {
    glm::mat2x2 matria(1.0f);
    glm::mat2x2 matrib(5.0f);
    auto matrix = matria * matrib;

    std::cout << matrix[0][0] << " " << matrix[0][1] << std::endl;
    std::cout << matrix[1][0] << " " << matrix[1][1] << std::endl;

    return 0;
}