#include <iostream>
#include <glm/glm.hpp>
using namespace std;
using namespace glm;
int main(){
    
    vec4 myVector(1.0f, -1.0f, 1.0f, 1.0f);
    myVector.r = 2.0f;
    myVector.g = 3.0f;
    myVector.b = 4.0f;
    
    std::cout << "Initial vector: " << myVector.x << " " << myVector.y << " " << myVector.z << std::endl;
    std::cout << "Initial vector: " << myVector.r << " " << myVector.g << " " << myVector.b << std::endl;
    std::cout << "Initial vector: " << myVector.s << " " << myVector.t << " " << myVector.p << std::endl;

    return 0;
}
