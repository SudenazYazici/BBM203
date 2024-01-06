#include <iostream>
#include "Network.h"

using namespace std;

int main(int argc, char *argv[]) {

    // Instantiate HUBBMNET
    Network* HUBBMNET = new Network();

    // Read from input files
    vector<Client> clients = HUBBMNET->read_clients("C:/Users/Lenovo/CLionProjects/Assignment3/sampleIO/1_sample_from_instructions/clients.dat");
    HUBBMNET->read_routing_tables(clients, "C:/Users/Lenovo/CLionProjects/Assignment3/sampleIO/1_sample_from_instructions/routing.dat");
    vector<string> commands = HUBBMNET->read_commands("C:/Users/Lenovo/CLionProjects/Assignment3/sampleIO/1_sample_from_instructions/commands.dat");

    // Get additional parameters from the cmd arguments
    int message_limit = stoi("20");
    string sender_port = "0706";
    string receiver_port = "0607";

    // Run the commands
    HUBBMNET->process_commands(clients, commands, message_limit, sender_port, receiver_port);

    // Delete HUBBMNET
    delete HUBBMNET;

    return 0;
}


