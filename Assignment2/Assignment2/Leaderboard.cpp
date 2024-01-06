#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include "Leaderboard.h"

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).

    if(head_leaderboard_entry == nullptr) {
        head_leaderboard_entry = new_entry;
    }else {
        LeaderboardEntry* temp = head_leaderboard_entry;
        LeaderboardEntry* prev = head_leaderboard_entry;
        while (temp != nullptr) {
            if(temp == head_leaderboard_entry && new_entry->score > temp->score) { // add to beginning
                LeaderboardEntry* oldHead = temp;
                LeaderboardEntry* newHead = new_entry;
                newHead->next_leaderboard_entry = oldHead;
                head_leaderboard_entry = newHead;
                break;
            }
            if(new_entry->score > temp->score) { //add in the middle
                prev->next_leaderboard_entry = new_entry;
                new_entry->next_leaderboard_entry = temp;
                break;
            }

            if(temp->next_leaderboard_entry == nullptr && temp->score >= new_entry->score) { // add to end
                temp->next_leaderboard_entry = new_entry;
                break;
            }

            if( temp != head_leaderboard_entry) {
                prev = prev->next_leaderboard_entry;
            }
            temp = temp->next_leaderboard_entry;

        }

        int count = 1;
        LeaderboardEntry* temp2 = head_leaderboard_entry; // to count leader board elements
        while(temp2->next_leaderboard_entry != nullptr) {
            if(count == MAX_LEADERBOARD_SIZE) {
                LeaderboardEntry* willBeDeleted = temp2->next_leaderboard_entry;
                temp2->next_leaderboard_entry = nullptr;
                delete willBeDeleted;
                break;
            }
            temp2 = temp2->next_leaderboard_entry;
            count++;
        }

    }
}

void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions

    std::ofstream output_file(filename);
    LeaderboardEntry* temp = head_leaderboard_entry;

    if(output_file.is_open()) {
        while (temp != nullptr) {
            output_file << temp->score << " " << temp->last_played << " " << temp->player_name << std::endl;
            temp = temp->next_leaderboard_entry;
        }

        output_file.close();
    }

}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.

    ifstream input_file(filename);
    string name, score, time;
    while (getline (input_file, score, ' ')) {
        getline(input_file, time,    ' ');
        getline(input_file, name,    '\n');

        if (head_leaderboard_entry == nullptr) {
            head_leaderboard_entry =new LeaderboardEntry(std::stoul(score), static_cast<time_t>(std::stoll(time)), name); // time is giving error
            head_leaderboard_entry->next_leaderboard_entry = nullptr;
            continue;
        }
        LeaderboardEntry* temp = head_leaderboard_entry;
        while (temp->next_leaderboard_entry != nullptr) {
            temp = temp->next_leaderboard_entry;
        }
        LeaderboardEntry* tail = new LeaderboardEntry(std::stoul(score), static_cast<time_t>(std::stoll(time)), name);
        tail->next_leaderboard_entry = nullptr;
        temp->next_leaderboard_entry = tail;

    }
    input_file.close();
}


void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions

    std::cout << "Leaderboard" << std::endl;
    std::cout << "-----------" << std::endl;

    LeaderboardEntry* temp = head_leaderboard_entry;
    int count = 0;

    while (temp != nullptr) {
        char time[80];
        std::strftime(time, sizeof(time), " %H:%M:%S/%d.%m.%Y", std::localtime(&temp->last_played));
        std::cout << count+1 << ". " << temp->player_name << " " << temp->score << " " << time << std::endl;
        temp = temp->next_leaderboard_entry;
        count++;
    }

}

Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries

    LeaderboardEntry* curr = head_leaderboard_entry;
    while(curr != nullptr) {

        LeaderboardEntry* temp = curr->next_leaderboard_entry;
        delete curr;
        curr = temp;
    }
}
