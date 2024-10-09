#pragma once

class IMesh {
public:
    virtual ~IMesh() = default;
    virtual void setupMesh() = 0;
    virtual void render()=0;
};
