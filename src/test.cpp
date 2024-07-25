#include<iostream>
#include<glm/glm.hpp>

int main() {
    glm::vec2 vec(1.0f, 0.0f);
    glm::vec2 vec1 = 8.0f * vec;
    std::cout << "vec1.x: " << vec1.x << " vec1.y: " << vec1.y << std::endl;
    std::cout << "vec1.x: " << vec1.r << " vec1.y: " << vec1.y << std::endl;
    
    return 0;
}