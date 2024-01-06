#include "Sector.h"
#include <cmath>

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code

        distance_from_earth = sqrt((x*x)+(y*y)+(z*z));
        int int_distance = static_cast<int> (distance_from_earth);
        sector_code.append(std::to_string(int_distance));

        if(x==0) {
            sector_code.append("S");
        } else if(x > 0) {
            sector_code.append("R");
        } else {
            sector_code.append("L");
        }

        if(y==0) {
            sector_code.append("S");
        } else if(y > 0) {
            sector_code.append("U");
        } else {
            sector_code.append("D");
        }

        if(z==0) {
            sector_code.append("S");
        } else if(z > 0) {
            sector_code.append("F");
        } else {
            sector_code.append("B");
        }

}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    // TODO: Overload the assignment operator
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->sector_code = other.sector_code;
    this->distance_from_earth = other.distance_from_earth;
    this->right = other.right;
    this->left = other.left;
    this->parent = other.parent;
    this->color = other.color;
    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}