#include <fstream>
#include <iostream>
#include <sstream>
#include "BlockFall.h"

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)

    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << input_file << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<bool> tempBool;
        for (int i = 0; i < line.length(); ++i) {
            if (line.substr(0,1) == "[") {
                Block* block = new Block;
                block->left_rotation = block; // initializing the rotations
                block->right_rotation = block;

                if (initial_block == nullptr) {
                    initial_block = block;
                } else {
                    Block* temp = initial_block;
                    while(temp->next_block != nullptr) {
                        temp = temp->next_block;
                    }
                    temp->next_block = block;
                }

                while (i < line.length() && line.substr(i,1) != "]") {
                    if (line.substr(i,1) == "0" || line.substr(i,1) == "1") {
                        tempBool.push_back(line[i] - '0');
                    }
                    ++i;

                }
                block->shape.push_back(tempBool);

            } else {
                Block* temp = initial_block;
                while(temp->next_block != nullptr) {
                    temp = temp->next_block;
                }
                while (i < line.length() && line.substr(i,1) != "]") {
                    if (line.substr(i,1) == "0" || line.substr(i,1) == "1") {
                        tempBool.push_back(line[i] - '0');
                    }
                    ++i;

                }
                temp->shape.push_back(tempBool);
            }
            tempBool.clear();
        }
    }
    file.close();

    Block* temp = initial_block; // deleting last element of linked list and adding it to the power_up
    Block* prev = nullptr;
    while(temp->next_block != nullptr) {
        prev = temp;
        temp = temp->next_block;
    }
    prev->next_block = nullptr;
    power_up = temp->shape;
    delete temp;

    Block* temp2 = initial_block;
    while(temp2->next_block != nullptr) {

        Block* rotated = new Block;
        rotated->shape.resize(temp2->shape[0].size(), vector<bool>(temp2->shape.size(), false)); // initializing rotated vector

        for(int i=0;i<temp2->shape.size();i++){  // right rotation
            for(int j=0;j<temp2->shape[0].size();j++){
                
                rotated->shape[j][temp2->shape.size()-i-1] = temp2->shape[i][j];
            }
        }
        if(rotated != temp2) {
            temp2->right_rotation = rotated;
            temp2->left_rotation = rotated;
            rotated->right_rotation = temp2;
            rotated->left_rotation = temp2;
            rotated->next_block = temp2->next_block;
        }

        Block* rotated2 = new Block;
        rotated2->shape.resize(temp2->right_rotation->shape[0].size(), vector<bool>(temp2->right_rotation->shape.size(), false)); // initializing rotated vector

        for(int i=0;i<temp2->right_rotation->shape.size();i++){  // right rotation
            for(int j=0;j<temp2->right_rotation->shape[0].size();j++){

                rotated2->shape[j][temp2->right_rotation->shape.size()-i-1] = temp2->right_rotation->shape[i][j];
            }
        }
        if(rotated2 != temp2->right_rotation && rotated2 != temp2) {
            temp2->right_rotation->right_rotation = rotated2;
            temp2->left_rotation = rotated2;
            rotated2->right_rotation = temp2;
            if(temp2 != temp2->right_rotation) {
                rotated2->left_rotation = temp2->right_rotation;
            }else {
                rotated2->left_rotation = temp2;
            }
            rotated2->next_block = temp2->next_block;
        }

        Block* rotated3 = new Block;
        rotated3->shape.resize(temp2->right_rotation->right_rotation->shape[0].size(), vector<bool>(temp2->right_rotation->right_rotation->shape.size(), false)); // initializing rotated vector

        for(int i=0;i<temp2->right_rotation->right_rotation->shape.size();i++){  // right rotation
            for(int j=0;j<temp2->right_rotation->right_rotation->shape[0].size();j++){

                rotated3->shape[j][temp2->right_rotation->right_rotation->shape.size()-i-1] = temp2->right_rotation->right_rotation->shape[i][j];
            }
        }
        if(rotated3 != temp2->right_rotation->right_rotation && rotated3 != temp2->right_rotation && rotated3 != temp2) {
            temp2->right_rotation->right_rotation->right_rotation = rotated3;
            temp2->left_rotation = rotated3;
            rotated3->right_rotation = temp2;
            if(temp2 != temp2->right_rotation) {
                rotated3->left_rotation = temp2->right_rotation->right_rotation;
            }else {
                rotated3->left_rotation = temp2;
            }
            rotated3->next_block = temp2->next_block;
        }

        temp2 = temp2->next_block;
    }

}

void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main
    this->rows = 0;
    this->cols = 0;
    std::ifstream file(input_file);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << input_file << std::endl;
        return;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        int block;
        while (iss >> block) {
            row.push_back(block);
        }
        this->grid.push_back(row);
    }
    this->rows = this->grid.size();
    this->cols = this->grid[0].size();

    file.close();
}


BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks

    if (initial_block != nullptr) {
        Block* curr = initial_block;
        while (curr != nullptr) {
            Block* rightRotation = curr->right_rotation;
            while (rightRotation != nullptr && rightRotation != curr) {
                Block* temp = rightRotation;
                rightRotation = rightRotation->right_rotation;
                delete temp;
            }

            Block* temp = curr;
            curr = curr->next_block;
            delete temp;
        }
    }
}
