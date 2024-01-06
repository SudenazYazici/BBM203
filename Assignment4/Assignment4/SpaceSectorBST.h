#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    void inOrder(Sector* treePtr);
    void preOrder(Sector* treePtr);
    void postOrder(Sector* treePtr);
    Sector* insert(Sector* parent, Sector* treePtr, int x, int y, int z);
    Sector* findSector(const std::string& sector_code);
    Sector* retrieveSector(Sector* treePtr, const std::string& sector_code);
    void deleteNode(Sector* ptr);

};

#endif // SPACESECTORBST_H
