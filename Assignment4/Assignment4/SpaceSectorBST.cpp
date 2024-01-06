#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
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

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.

    if(root == nullptr) {
        root = new Sector(x,y,z);
    }else {
        insert(nullptr, root, x, y, z);
    }
}

Sector* SpaceSectorBST::insert(Sector* parent, Sector* treePtr, int x, int y, int z) {
    if(treePtr == nullptr) {
        Sector* newNode = new Sector(x, y, z);
        newNode->parent = parent;
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
    return treePtr;
}

void SpaceSectorBST::deleteSector(const std::string& sector_code) { // not working!
    // TODO: Delete the sector given by its sector_code from the BST.

    Sector* ptr = findSector(sector_code);
    if(ptr == nullptr) {
        return;
    } else {
        deleteNode(ptr);
    }

}

void SpaceSectorBST::deleteNode(Sector* ptr) {
    if (root == nullptr || ptr == nullptr) {
        return;
    }

    if(ptr->left == nullptr && ptr->right == nullptr) { // no child
        ptr->parent = nullptr;
        ptr = nullptr;
        delete ptr;
    } else if(ptr->left == nullptr && ptr->right != nullptr) { // only right child exists
        Sector* delPtr = ptr;
        ptr = ptr->right;
        delPtr->right = nullptr;
        ptr->right->parent = ptr->parent;
        delete delPtr;
    } else if(ptr->right == nullptr && ptr->left != nullptr) { // only left child exists
        Sector* delPtr = ptr;
        ptr = ptr->left;
        delPtr->left = nullptr;
        ptr->left->parent = ptr->parent;
        delete delPtr;
    } else { // two child
        Sector* temp = ptr->right;
        while(temp->left != nullptr) { // process leftmost
            temp = temp->left;
        }
        ptr->x = temp->x;
        ptr->y = temp->y;
        ptr->z = temp->z;
        ptr->sector_code = temp->sector_code;
        ptr->distance_from_earth = temp->distance_from_earth;

        deleteNode(temp);

        /*Sector delPtr = temp;
        temp = temp->right;
        delPtr->right = nullptr;
        delete delPtr;*/
    }
}

Sector* SpaceSectorBST::findSector(const std::string& sector_code) {
    return retrieveSector(root, sector_code);
}

Sector* SpaceSectorBST::retrieveSector(Sector* treePtr, const std::string& sector_code) {
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

void SpaceSectorBST::inOrder(Sector* treePtr) {
    if(treePtr) {
        inOrder(treePtr->left);
        std::cout << treePtr->sector_code << std::endl;
        inOrder(treePtr->right);
    }
}

void SpaceSectorBST::preOrder(Sector* treePtr) {
    if(treePtr) {
        std::cout << treePtr->sector_code << std::endl;
        preOrder(treePtr->left);
        preOrder(treePtr->right);
    }
}

void SpaceSectorBST::postOrder(Sector* treePtr) {
    if(treePtr) {
        postOrder(treePtr->left);
        postOrder(treePtr->right);
        std::cout << treePtr->sector_code << std::endl;
    }
}

void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.

    std::cout << "Space sectors inorder traversal:" << std::endl;
    inOrder(root);
    std::cout << std::endl;
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.

    std::cout << "Space sectors preorder traversal:" << std::endl;
    preOrder(root);
    std::cout << std::endl;
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.

    std::cout << "Space sectors postorder traversal:" << std::endl;
    postOrder(root);
    std::cout << std::endl;
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
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

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
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