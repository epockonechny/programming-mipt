#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "rt_geometry.h"

typedef unsigned int uint;

class RtCamera3d
{
private:

public:
    vector3d spot;
    vector3d TL;
    vector3d TR;
    vector3d BL;
    uint xResolution;
    uint yResolution;

    RtCamera3d() {

    }

    RtCamera3d(vector3d spot, vector3d p1, vector3d p2, vector3d p3,
             uint xResolution=320, uint yResolution=240) {
        this->spot = spot;
        this->TL = p1;
        this->TR = p2;
        this->BL = p3;
        this->xResolution = xResolution;
        this->yResolution = yResolution;
    }

    uint xRes() {
        return xResolution;
    }

    uint yRes() {
        return yResolution;
    }

    vector3d getVector3dByPixelCoord(uint pixelX, uint pixelY, uint resX, uint resY) {
        return TL + (TR - TL) * DDOUBLE(pixelX) / (1.0 * resX) + (BL - TL) * DDOUBLE(pixelY) / (1.0 * resY);
    }

    void print() {
        std::cout << "camera[\n Spot(" << spot.x() << "; " << spot.y() << "; " << spot.z() << ")\n"
                  << "  TL(" << TL.x() << "; " << TL.y() << "; " << TL.z() << ")\n"
                  << "  TR(" << TR.x() << "; " << TR.y() << "; " << TR.z() << ")\n"
                  << "  BL(" << BL.x() << "; " << BL.y() << "; " << BL.z() << ")\n"
                  << "  resolution: " << xResolution << "x" << yResolution << "\n]" << std::endl;
    }
};

#endif // CAMERA_H
