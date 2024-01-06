#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(-1);
    }

    std::string line;
    std::getline(file, line);
    std::string x, y, z;
    while (getline (file, x, ',')) {
        getline(file, y,    ',');
        getline(file, z,    '\n');

        insertSectorByCoordinates(std::stoi(x), std::stoi(y), std::stoi(z));
    }
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.

    if(root == nullptr) {
        root = new Sector(x,y,z);
        root->color = false;
    }else {
        insert(nullptr, root, x, y, z);
    }
}

Sector* SpaceSectorLLRBT::insert(Sector* parent, Sector* treePtr, int x, int y, int z) { // not working correctly
    if(treePtr == nullptr) {
        Sector* newNode = new Sector(x, y, z);
        newNode->parent = parent;
        // after insertion do necessary rotations and color flips
        /*if (newNode->parent != nullptr) { // not working properly
            if((newNode->parent->right!= nullptr && newNode->parent->left!= nullptr && newNode->parent->right->color && !newNode->parent->left->color) || (newNode->parent->right!= nullptr && newNode->parent->left== nullptr && newNode->parent->right->color)) { // only right link is red
                newNode->parent = rotateLeft(newNode->parent);
            }
            if(newNode->parent->parent != nullptr && newNode->parent->parent->left!= nullptr && newNode->parent->parent->left->left!= nullptr && newNode->parent->parent->left->color && newNode->parent->parent->left->left->color) { // if both left child and left-left grandchild links are red
                newNode->parent->parent = rotateRight(newNode->parent->parent);
            }
            if(newNode->parent->right!= nullptr && newNode->parent->left!= nullptr && newNode->parent->left->color && newNode->parent->right->color) { // if both child links are red
                flipColors(newNode->parent);
            }
        }*/
        return newNode;
    }

    if(x > treePtr->x) {
        treePtr->right = insert(treePtr, treePtr->right, x, y, z);
    } else if(x < treePtr->x) {
        treePtr->left = insert(treePtr, treePtr->left, x, y, z);
    } else {
        if(y > treePtr->y) {
            treePtr->right = insert(treePtr, treePtr->right, x, y, z);
        } else if(y < treePtr->y) {
            treePtr->left = insert(treePtr, treePtr->left, x, y, z);
        } else {
            if(z > treePtr->z) {
                treePtr->right = insert(treePtr, treePtr->right, x, y, z);
            } else if(z < treePtr->z) {
                treePtr->left = insert(treePtr, treePtr->left, x, y, z);
            }
            return treePtr;
        }
    }

    if((treePtr->right!= nullptr && treePtr->left!= nullptr && treePtr->right->color && !treePtr->left->color ) || (treePtr->right!= nullptr && treePtr->left == nullptr && treePtr->right->color)) { // only right link is red
        treePtr = rotateLeft(treePtr);
    }
    if(treePtr->left!= nullptr && treePtr->left->left!= nullptr && treePtr->left->color && treePtr->left->left->color) { // if both left child and left-left grandchild links are red
        treePtr = rotateRight(treePtr);
    }
    if(treePtr->right!= nullptr && treePtr->left!= nullptr && treePtr->left->color && treePtr->right->color) { // if both child links are red
        flipColors(treePtr);
    }
    if(root->color) {
        root->color = false;
    }

    return treePtr;
}

Sector* SpaceSectorLLRBT::rotateLeft(Sector* treePtr) {
    Sector* temp = treePtr->right;
    treePtr->right = temp->left;
    temp->left = treePtr;
    temp->color = treePtr->color;
    treePtr->color = true;
    //temp->parent = treePtr->parent;
    //treePtr->parent = temp;
    return temp;
}

Sector* SpaceSectorLLRBT::rotateRight(Sector* treePtr) {
    Sector* temp = treePtr->left;
    treePtr->left = temp->right;
    temp->right = treePtr;
    temp->color = treePtr->color;
    treePtr->color = true;
    //temp->parent = treePtr->parent;
    //treePtr->parent = temp;
    return temp;
}

void SpaceSectorLLRBT::flipColors(Sector* treePtr) {
    treePtr->color = true;
    treePtr->left->color = false;
    treePtr->right->color = false;
}

void SpaceSectorLLRBT::inOrder(Sector* treePtr) {
    if(treePtr) {
        inOrder(treePtr->left);
        if(treePtr->color) {
            std::cout << "RED sector: " << treePtr->sector_code << std::endl;
        } else {
            std::cout << "BLACK sector: " << treePtr->sector_code << std::endl;
        }
        inOrder(treePtr->right);
    }
}

void SpaceSectorLLRBT::preOrder(Sector* treePtr) {
    if(treePtr) {
        if(treePtr->color) {
            std::cout << "RED sector: " << treePtr->sector_code << std::endl;
        } else {
            std::cout << "BLACK sector: " << treePtr->sector_code << std::endl;
        }
        preOrder(treePtr->left);
        preOrder(treePtr->right);
    }
}

void SpaceSectorLLRBT::postOrder(Sector* treePtr) {
    if(treePtr) {
        postOrder(treePtr->left);
        postOrder(treePtr->right);
        if(treePtr->color) {
            std::cout << "RED sector: " << treePtr->sector_code << std::endl;
        } else {
            std::cout << "BLACK sector: " << treePtr->sector_code << std::endl;
        }
    }
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.

    std::cout << "Space sectors inorder traversal:" << std::endl;
    inOrder(root);
    std::cout << std::endl;
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.

    std::cout << "Space sectors preorder traversal:" << std::endl;
    preOrder(root);
    std::cout << std::endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.

    std::cout << "Space sectors postorder traversal:" << std::endl;
    postOrder(root);
    std::cout << std::endl;
}

Sector* SpaceSectorLLRBT::findSector(const std::string& sector_code) {
    return retrieveSector(root, sector_code);
}

Sector* SpaceSectorLLRBT::retrieveSector(Sector* treePtr, const std::string& sector_code) {
    if(treePtr == nullptr) {
        return nullptr;
    } else if(treePtr->sector_code == sector_code) {
        return treePtr;
    } else {

        Sector* rightSubTree = retrieveSector(treePtr->right, sector_code);
        if(rightSubTree != nullptr){
            return rightSubTree;
        } else {
            return retrieveSector(treePtr->left, sector_code);
        }
    }

}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!

    Sector* ptr = findSector(sector_code);
    while(ptr != nullptr) {
        path.push_back(ptr);
        ptr = ptr->parent;
    }

    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.

    if(path.empty()) {
        std::cout << "A path to Dr. Elara could not be found." << std::endl;
    }else {
        std::cout << "The stellar path to Dr. Elara: ";
        for (auto it =  path.rbegin(); it != path.rend()-1; ++it){
            std::cout << (*it)->sector_code << "->";

        }
        std::cout << path.front()->sector_code << std::endl;
    }
}