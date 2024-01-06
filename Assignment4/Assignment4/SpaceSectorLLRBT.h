#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    void inOrder(Sector* treePtr);
    void preOrder(Sector* treePtr);
    void postOrder(Sector* treePtr);
    Sector* findSector(const std::string& sector_code);
    Sector* retrieveSector(Sector* treePtr, const std::string& sector_code);
    Sector* insert(Sector* parent, Sector* treePtr, int x, int y, int z);
    Sector* rotateLeft(Sector* treePtr);
    Sector* rotateRight(Sector* treePtr);
    void flipColors(Sector* treePtr);

};

#endif // SPACESECTORLLRBT_H
