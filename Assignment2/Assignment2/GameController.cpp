#include <fstream>
#include <iostream>
#include <sstream>
#include <chrono>
#include <algorithm>
#include "GameController.h"

bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.

    std::ifstream file(commands_file);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << commands_file << std::endl;
        return false;
    }

    Block* temp = game.initial_block;
    game.active_rotation = game.initial_block;
    int row = 0; // to store current row and column
    int col = 0;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);

        if(game.active_rotation == nullptr) { // if there is no more blocks
            std::cout << "YOU WIN!" << std::endl;
            std::cout << "No more blocks." << std::endl;
            std::cout << "Final grid and score: " << std::endl;
            std::cout << std::endl;
            std::cout << "Score: " << game.current_score << std::endl;
            std::cout << "High Score: ";
            if(game.leaderboard.head_leaderboard_entry && game.leaderboard.head_leaderboard_entry->score) {
                std::cout << game.leaderboard.head_leaderboard_entry->score << std::endl;
            } else {
                std::cout << "0" << std::endl;
            }

            for(int k=0;k<game.rows;k++){
                for(int m=0;m<game.cols;m++){
                    if(game.grid[k][m]) {
                        std::cout << occupiedCellChar << " ";
                    } else {
                        std::cout << unoccupiedCellChar << " ";
                    }
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
            game.leaderboard.print_leaderboard();
            return true;

        }

        for(int i=0;i<game.active_rotation->shape.size();i++){
            for(int j=0;j<game.active_rotation->shape[0].size();j++){
                if(game.active_rotation->shape[i][j] && game.grid[i][j]) { // if the block can not fit
                    std::cout << "GAME OVER!" << std::endl;
                    std::cout << "Next block that couldn't fit:" << std::endl;
                    for(int k=0;k<game.active_rotation->shape.size();k++){
                        for(int m=0;m<game.active_rotation->shape[0].size();m++){
                            if(game.active_rotation->shape[k][m]) {
                                std::cout << occupiedCellChar << " ";
                            } else {
                                std::cout << unoccupiedCellChar << " ";
                            }
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;
                    std::cout << "Final grid and score: " << std::endl;
                    std::cout << std::endl;
                    std::cout << "Score: " << game.current_score << std::endl;
                    std::cout << "High Score: ";
                    if(game.leaderboard.head_leaderboard_entry) {
                        std::cout << game.leaderboard.head_leaderboard_entry->score << std::endl;
                    } else {
                        std::cout << "0" << std::endl;
                    }

                    for(int k=0;k<game.rows;k++){
                        for(int m=0;m<game.cols;m++){
                            if(game.grid[k][m]) {
                                std::cout << occupiedCellChar << " ";
                            } else {
                                std::cout << unoccupiedCellChar << " ";
                            }
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;
                    game.leaderboard.print_leaderboard();
                    game.leaderboard.write_to_file(game.leaderboard_file_name);
                    return false;
                }
            }
        }

        if(line == "PRINT_GRID") {
            std::cout << "Score: " << game.current_score << std::endl;
            std::cout << "High Score: ";
            if(game.leaderboard.head_leaderboard_entry && game.leaderboard.head_leaderboard_entry->score) {
                std::cout << game.leaderboard.head_leaderboard_entry->score << std::endl;
            } else {
                std::cout << "0" << std::endl;
            }

            for(int i=0;i<game.rows;i++){
                for(int j=0;j<game.cols;j++){
                    if (row-1 < i && i < game.active_rotation->shape.size()+row &&  col-1 < j && j < game.active_rotation->shape[0].size()+col) {
                        if(game.active_rotation->shape[i-row][j-col]) {
                            std::cout << occupiedCellChar << " ";
                        } else {
                            if(game.grid[i][j]) {
                                std::cout << occupiedCellChar << " ";
                            } else {
                                std::cout << unoccupiedCellChar << " ";
                            }
                        }

                    } else {
                        if(game.grid[i][j]) {
                            std::cout << occupiedCellChar << " ";
                        } else {
                            std::cout << unoccupiedCellChar << " ";
                        }
                    }
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
            std::cout << std::endl;

        }else if(line == "ROTATE_RIGHT") {
            game.active_rotation = game.active_rotation->right_rotation;
            if(col+game.active_rotation->shape[0].size()+1 > game.cols) {
                continue;
            }else {
                bool isAllowed = true;
                for(int i=0;i<game.active_rotation->shape.size();i++){  // checking if there is block at right
                    for(int j=0;j<game.active_rotation->shape[0].size();j++){
                        if(game.active_rotation->shape[i][j] && game.grid[i][j+col+1]) {
                            game.active_rotation = game.active_rotation->left_rotation;
                            isAllowed =  false;
                        }
                    }
                }
                if(!isAllowed){
                    continue;
                }
            }


        }else if(line == "ROTATE_LEFT") {
            game.active_rotation = game.active_rotation->left_rotation;

            if(col+game.active_rotation->shape[0].size()+1 > game.cols) {
                continue;
            }else {
                bool isAllowed = true;
                for(int i=0;i<game.active_rotation->shape.size();i++){  // checking if there is block at right
                    for(int j=0;j<game.active_rotation->shape[0].size();j++){
                        if(game.active_rotation->shape[i][j] && game.grid[i][j+col+1]) {
                            game.active_rotation = game.active_rotation->right_rotation;
                            isAllowed =  false;
                        }
                    }
                }
                if(!isAllowed){
                    continue;
                }
            }

        }else if(line == "MOVE_RIGHT") {
            if(col+game.active_rotation->shape[0].size()+1 > game.cols) {
                continue;
            }else {
                bool isAllowed = true;
                for(int i=0;i<game.active_rotation->shape.size();i++){  // checking if there is block at right
                    for(int j=0;j<game.active_rotation->shape[0].size();j++){
                        if(game.active_rotation->shape[i][j] && game.grid[i][j+col+1]) {
                            isAllowed =  false;
                        }
                    }
                }
                if(isAllowed){
                    col++;
                } else {
                    continue;
                }
            }

        }else if(line == "MOVE_LEFT") {
            if(col == 0) {
                continue;
            }else {
                col--;
            }


        }else if(line == "DROP") {

            if(game.gravity_mode_on) {
                // firstly dropping the block as if the gravity mode is off
                bool isAllowed = true;

                while (isAllowed) {
                    for(int i=0;i<game.active_rotation->shape.size();i++){
                        for(int j=0;j<game.active_rotation->shape[0].size();j++){
                            if(row >= game.rows || 1+ row + game.active_rotation->shape.size()> game.rows || (game.active_rotation->shape[i][j] && game.grid[i+row+1][col+j])  ) {
                                isAllowed = false;
                                break;
                            }
                        }
                    }
                    if(isAllowed) {
                        row++;
                    }

                }

                for(int i=0;i<game.rows;i++){
                    for(int j=0;j<game.cols;j++){
                        if (row-1 < i && i < game.active_rotation->shape.size()+row &&  col-1 < j && j < game.active_rotation->shape[0].size()+col) {
                            if(game.grid[i][j] && !game.active_rotation->shape[i-row][j-col]) {
                                game.grid[i][j] = 1;
                            }else{
                                game.grid[i][j] = game.active_rotation->shape[i-row][j-col];
                            }
                        }
                    }
                }

                // dropping every available block
                for(int i=game.rows-2;i>=0;i--){
                    for(int j=0;j<game.cols;j++){
                        if(game.grid[i][j]) {
                            if(i == game.rows-2) {
                                if (!game.grid[i+1][j]) {
                                    game.grid[i][j] = 0;
                                    game.grid[i+1][j] = 1;
                                }
                            } else {
                                int tempRow = i+1;
                                if(!game.grid[game.rows-1][j]) {
                                    while(tempRow <= game.rows-1) {
                                        tempRow++;
                                    }
                                } else {
                                    while(tempRow < game.rows-1 && !game.grid[tempRow][j]) {

                                        tempRow++;
                                    }

                                }
                                tempRow--;
                                game.grid[i][j] = 0;
                                game.grid[tempRow][j] = 1;

                            }
                        }
                    }
                }

                for(int i=0;i<game.rows-game.power_up.size()+1;i++){ // checking power up
                    for(int j=0;j<game.cols-game.power_up[0].size()+1;j++){
                        bool isEqual = true;
                        for(int k=0;k<game.power_up.size();k++){
                            for(int m=0;m<game.power_up[0].size();m++){
                                if(game.power_up[k][m] != game.grid[i+k][j+m]) {
                                    isEqual = false;
                                    break;
                                }
                            }
                        }
                        if(isEqual) { // if there is power up, first add power up points then add points for every part of block
                            std::cout << "Before clearing:" << std::endl;
                            for(int k=0;k<game.rows;k++){
                                for(int m=0;m<game.cols;m++){
                                    if(game.grid[k][m]) {
                                        std::cout << occupiedCellChar << " ";
                                    } else {
                                        std::cout << unoccupiedCellChar << " ";
                                    }
                                }
                                std::cout << std::endl;
                            }
                            std::cout << std::endl;
                            std::cout << std::endl;
                            for(int k=i;k<i+game.power_up.size();k++){
                                for(int m=j;m<j+game.power_up[0].size();m++){
                                    game.grid[k][m] = game.grid[k-1][m];
                                }
                            }
                            int num = 0;
                            for(int k=0;k<game.power_up.size();k++){ // checking cleared cells to correctly calculate points
                                for(int m=0;m<game.power_up[0].size();m++){
                                    if(game.power_up[k][m]) {
                                        num++;
                                    }
                                }
                            }
                            game.current_score += 1000;

                        }
                    }
                }

                for(int i=0;i<game.rows;i++){ // checking if a row is full of 1's
                    bool isFull = true;
                    for(int j=0;j<game.cols;j++){
                        if(!game.grid[i][j]) {
                            isFull = false;
                        }
                    }

                    if(isFull) {
                        std::cout << "Before clearing:" << std::endl;
                        for(int k=0;k<game.rows;k++){
                            for(int m=0;m<game.cols;m++){
                                if(game.grid[k][m]) {
                                    std::cout << occupiedCellChar << " ";
                                } else {
                                    std::cout << unoccupiedCellChar << " ";
                                }
                            }
                            std::cout << std::endl;
                        }
                        std::cout << std::endl;
                        std::cout << std::endl;
                        for(int k=0;k<i;k++){
                            for(int m=0;m<game.cols;m++){
                                game.grid[i-k][m] = game.grid[i-k-1][m];
                            }
                        }
                    }
                }

                LeaderboardEntry* entry = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
                LeaderboardEntry* tempLeaderBoardEntry = game.leaderboard.head_leaderboard_entry;
                LeaderboardEntry* prevLeaderBoardEntry = game.leaderboard.head_leaderboard_entry;
                LeaderboardEntry* last = game.leaderboard.head_leaderboard_entry;
                while(last != nullptr && last->next_leaderboard_entry != nullptr) {
                    last = last->next_leaderboard_entry;
                }

                bool isPresent = false;
                while(tempLeaderBoardEntry != nullptr) {
                    if(tempLeaderBoardEntry->player_name == game.player_name) {
                        if(tempLeaderBoardEntry == game.leaderboard.head_leaderboard_entry) {
                            game.leaderboard.head_leaderboard_entry->score = game.current_score;
                            game.leaderboard.head_leaderboard_entry->last_played = time(nullptr);
                            isPresent = true;
                            break;
                        } else if(tempLeaderBoardEntry == last) {
                            prevLeaderBoardEntry->next_leaderboard_entry = nullptr;
                            last = prevLeaderBoardEntry;
                            delete tempLeaderBoardEntry;
                            break;
                        } else {
                            prevLeaderBoardEntry->next_leaderboard_entry = tempLeaderBoardEntry->next_leaderboard_entry;
                            delete tempLeaderBoardEntry;
                            break;
                        }
                    }
                    prevLeaderBoardEntry = tempLeaderBoardEntry;
                    tempLeaderBoardEntry = tempLeaderBoardEntry->next_leaderboard_entry;
                }
                if(!isPresent) {
                    game.leaderboard.insert_new_entry(entry);
                }

            }else {
                bool isAllowed = true;

                while (isAllowed) {
                    for(int i=0;i<game.active_rotation->shape.size();i++){
                        for(int j=0;j<game.active_rotation->shape[0].size();j++){
                            if(row >= game.rows || 1+ row + game.active_rotation->shape.size()> game.rows || (game.active_rotation->shape[i][j] && game.grid[i+row+1][col+j])  ) {
                                isAllowed = false;
                                break;
                            }
                        }
                    }
                    if(isAllowed) {
                        row++;
                    }

                }

                for(int i=0;i<game.rows;i++){
                    for(int j=0;j<game.cols;j++){
                        if (row-1 < i && i < game.active_rotation->shape.size()+row &&  col-1 < j && j < game.active_rotation->shape[0].size()+col) {
                            if(game.grid[i][j] && !game.active_rotation->shape[i-row][j-col]) {
                                game.grid[i][j] = 1;
                            }else{
                                game.grid[i][j] = game.active_rotation->shape[i-row][j-col];
                            }
                        }
                    }
                }

                int tempNum = 0;
                for(int k=0;k<game.active_rotation->shape.size();k++){ // checking cleared cells to correctly calculate points
                    for(int m=0;m<game.active_rotation->shape[0].size();m++){
                        if(game.active_rotation->shape[k][m]) {
                            tempNum++;
                        }
                    }
                }

                game.current_score += tempNum*row;

                for(int i=0;i<game.rows-game.power_up.size()+1;i++){ // checking power up
                    for(int j=0;j<game.cols-game.power_up[0].size()+1;j++){
                        bool isEqual = true;
                        for(int k=0;k<game.power_up.size();k++){
                            for(int m=0;m<game.power_up[0].size();m++){
                                if(game.power_up[k][m] != game.grid[i+k][j+m]) {
                                    isEqual = false;
                                    break;
                                }
                            }
                        }
                        if(isEqual) {
                            std::cout << "Before clearing:" << std::endl;
                            for(int k=0;k<game.rows;k++){
                                for(int m=0;m<game.cols;m++){
                                    if(game.grid[k][m]) {
                                        std::cout << occupiedCellChar << " ";
                                    } else {
                                        std::cout << unoccupiedCellChar << " ";
                                    }
                                }
                                std::cout << std::endl;
                            }
                            std::cout << std::endl;
                            std::cout << std::endl;
                            int num = 0;
                            for(int x=0;x<game.rows;x++){ // checking every full block to calculate points
                                for(int y=0;y<game.cols;y++){
                                    if(game.grid[x][y]) {
                                        num++;
                                    }
                                }
                            }
                            game.current_score += num;
                            for(int x=0;x<game.rows;x++){
                                for(int y=0;y<game.cols;y++){
                                    game.grid[x][y] = 0;
                                }
                            }
                            game.current_score += 1000;
                        }
                    }
                }


                for(int i=0;i<game.rows;i++){ // checking if a row is full of 1's
                    bool isFull = true;
                    for(int j=0;j<game.cols;j++){
                        if(!game.grid[i][j]) {
                            isFull = false;
                        }
                    }

                    if(isFull) {
                        std::cout << "Before clearing:" << std::endl;
                        for(int k=0;k<game.rows;k++){
                            for(int m=0;m<game.cols;m++){
                                if(game.grid[k][m]) {
                                    std::cout << occupiedCellChar << " ";
                                } else {
                                    std::cout << unoccupiedCellChar << " ";
                                }
                            }
                            std::cout << std::endl;
                        }
                        std::cout << std::endl;
                        std::cout << std::endl;
                        for(int k=0;k<i;k++){
                            for(int m=0;m<game.cols;m++){
                                game.grid[i-k][m] = game.grid[i-k-1][m];
                            }
                        }
                        game.current_score += game.cols;
                    }
                }

                LeaderboardEntry* entry = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
                LeaderboardEntry* tempLeaderBoardEntry = game.leaderboard.head_leaderboard_entry;
                LeaderboardEntry* prevLeaderBoardEntry = game.leaderboard.head_leaderboard_entry;
                LeaderboardEntry* last = game.leaderboard.head_leaderboard_entry;
                while(last != nullptr && last->next_leaderboard_entry != nullptr) {
                    last = last->next_leaderboard_entry;
                }

                bool isPresent = false;
                while(tempLeaderBoardEntry != nullptr) {
                    if(tempLeaderBoardEntry->player_name == game.player_name) {
                        if(tempLeaderBoardEntry == game.leaderboard.head_leaderboard_entry) {
                            game.leaderboard.head_leaderboard_entry->score = game.current_score;
                            game.leaderboard.head_leaderboard_entry->last_played = time(nullptr);
                            isPresent = true;
                            break;
                        } else if(tempLeaderBoardEntry == last) {
                            prevLeaderBoardEntry->next_leaderboard_entry = nullptr;
                            last = prevLeaderBoardEntry;
                            delete tempLeaderBoardEntry;
                            break;
                        } else {
                            prevLeaderBoardEntry->next_leaderboard_entry = tempLeaderBoardEntry->next_leaderboard_entry;
                            delete tempLeaderBoardEntry;
                            break;
                        }
                    }
                    prevLeaderBoardEntry = tempLeaderBoardEntry;
                    tempLeaderBoardEntry = tempLeaderBoardEntry->next_leaderboard_entry;
                }
                if(!isPresent) {
                    game.leaderboard.insert_new_entry(entry);
                }
            }

            temp = temp->next_block;
            game.active_rotation = game.active_rotation->next_block;
            row = 0;
            col=0;
            game.leaderboard.write_to_file(game.leaderboard_file_name);


        }else if(line == "GRAVITY_SWITCH") {
            if(game.gravity_mode_on) {
                game.gravity_mode_on = false;
            }else {
                game.gravity_mode_on = true;
                // dropping every available block
                for(int i=game.rows-2;i>=0;i--){
                    for(int j=0;j<game.cols;j++){
                        if(game.grid[i][j]) {
                            if(i == game.rows-2) {
                                if (!game.grid[i+1][j]) {
                                    game.grid[i][j] = 0;
                                    game.grid[i+1][j] = 1;
                                }
                            } else {
                                int tempRow = i+1;
                                if(!game.grid[game.rows-1][j]) {
                                    while(tempRow <= game.rows-1) {
                                        tempRow++;
                                    }
                                } else {
                                    while(tempRow < game.rows-1 && !game.grid[tempRow][j]) {

                                        tempRow++;
                                    }

                                }
                                tempRow--;
                                game.grid[i][j] = 0;
                                game.grid[tempRow][j] = 1;
                            }
                        }
                    }
                }

                for(int i=0;i<game.rows;i++){ // checking if a row is full of 1's
                    bool isFull = true;
                    for(int j=0;j<game.cols;j++){
                        if(!game.grid[i][j]) {
                            isFull = false;
                        }
                    }

                    if(isFull) {
                        
                        for(int k=0;k<i;k++){
                            for(int m=0;m<game.cols;m++){
                                game.grid[i-k][m] = game.grid[i-k-1][m];
                            }
                        }
                        game.current_score += game.cols;
                    }
                }
            }

        }else {
            if(!line.empty()) {
                std::cerr << "Unknown command: " << line << std::endl;
            }
        }

    }

    std::cout << "GAME FINISHED!" << std::endl;
    std::cout << "No more commands." << std::endl;
    std::cout << "Final grid and score: " << std::endl;
    std::cout << std::endl;
    std::cout << "Score: " << game.current_score << std::endl;
    std::cout << "High Score: ";
    if(game.leaderboard.head_leaderboard_entry && game.leaderboard.head_leaderboard_entry->score) {
        std::cout << game.leaderboard.head_leaderboard_entry->score << std::endl;
    } else {
        std::cout << "0" << std::endl;
    }

    for(int k=0;k<game.rows;k++){
        for(int m=0;m<game.cols;m++){
            if(game.grid[k][m]) {
                std::cout << occupiedCellChar << " ";
            } else {
                std::cout << unoccupiedCellChar << " ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    game.leaderboard.print_leaderboard();
    game.leaderboard.write_to_file(game.leaderboard_file_name);

    return true;
}