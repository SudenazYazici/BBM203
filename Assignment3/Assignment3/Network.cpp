#include "Network.h"

Network::Network() {

}


//NOTE: Not sure about the calculation of number of hops.
void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                               const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */


    vector<std::string> receivedFrames;

    for(const auto& command:commands) {
        int charNum = command.length()+9;
        while(charNum > 0 ) {
            std::cout << "-";
            charNum--;
        }
        std::cout << std::endl;
        std::cout << "Command: " << command << std::endl;
        int charNum2 = command.length()+9;
        while(charNum2 > 0 ) {
            std::cout << "-";
            charNum2--;
        }
        std::cout << std::endl;

        stringstream ss(command);
        string command_first;
        getline(ss, command_first, ' ');

        if(command_first == "MESSAGE") {
            std::string sender_ID, receiver_ID, message;
            getline(ss, sender_ID, ' ');
            getline(ss, receiver_ID, ' ');
            getline(ss, message, '\n'); // not sure about newline(\n)

            for(auto &client:clients) {
                if (sender_ID == client.client_id) {
                    if(message.size() <= message_limit+2) {

                        std::cout << "Message to be sent: \"" << message.substr(1,message.length()-2) << "\"" << std::endl;
                        std::cout << std::endl;
                        ApplicationLayerPacket* applicationLayerPacket = new ApplicationLayerPacket(APPLICATION_LAYER_ID, sender_ID, receiver_ID, message.substr(1, message.length()-2));
                        TransportLayerPacket* transportLayerPacket = new TransportLayerPacket(TRANSPORT_LAYER_ID, sender_port, receiver_port);

                        std::string receiver_ip;
                        for(const auto& receiver:clients) {
                            if(receiver_ID == receiver.client_id) {
                                receiver_ip = receiver.client_ip;
                            }
                        }
                        NetworkLayerPacket* networkLayerPacket = new NetworkLayerPacket(NETWORK_LAYER_ID, client.client_ip, receiver_ip);

                        std::string next_hop_id = client.routing_table.at(receiver_ID);
                        std::string next_hop_mac;
                        for(const auto& next_hop:clients) {
                            if(next_hop_id == next_hop.client_id) {
                                next_hop_mac = next_hop.client_mac;
                            }
                        }
                        PhysicalLayerPacket* physicalLayerPacket = new PhysicalLayerPacket(PHYSICAL_LAYER_ID, client.client_mac, next_hop_mac);

                        stack<Packet*> frames;

                        frames.push(applicationLayerPacket);
                        frames.push(transportLayerPacket);
                        frames.push(networkLayerPacket);
                        frames.push(physicalLayerPacket);

                        client.outgoing_queue.push(frames);

                        std::cout << "Frame #1" << std::endl;
                        std::cout << "Sender MAC address: " << physicalLayerPacket->sender_MAC_address << ", Receiver MAC address: " << physicalLayerPacket->receiver_MAC_address << std::endl;
                        std::cout << "Sender IP address: " << networkLayerPacket->sender_IP_address << ", Receiver IP address: " << networkLayerPacket->receiver_IP_address << std::endl;
                        std::cout << "Sender port number: " << transportLayerPacket->sender_port_number << ", Receiver port number: " << transportLayerPacket->receiver_port_number << std::endl;
                        std::cout << "Sender ID: " << applicationLayerPacket->sender_ID << ", Receiver ID: " << applicationLayerPacket->receiver_ID << std::endl;
                        std::cout << "Message chunk carried: \"" << applicationLayerPacket->message_data << "\"" << std::endl;
                        std::cout << "Number of hops so far: 0" << std::endl;
                        std::cout << "--------" << std::endl;


                    } else {
                        int message_part_num = 1;
                        std::string tempString = message.substr(1, message.length()-2);
                        std::cout << "Message to be sent: \"" << tempString << "\"" << std::endl;
                        std::cout << std::endl;

                        ApplicationLayerPacket* applicationLayerPacket = new ApplicationLayerPacket(APPLICATION_LAYER_ID, sender_ID, receiver_ID, tempString.substr(0, message_limit));
                        //std::cout << tempString.substr(0, message_limit) << std::endl;
                        TransportLayerPacket* transportLayerPacket = new TransportLayerPacket(TRANSPORT_LAYER_ID, sender_port, receiver_port);

                        std::string receiver_ip;
                        for(const auto& receiver:clients) {
                            if(receiver_ID == receiver.client_id) {
                                receiver_ip = receiver.client_ip;
                            }
                        }
                        NetworkLayerPacket* networkLayerPacket = new NetworkLayerPacket(NETWORK_LAYER_ID, client.client_ip, receiver_ip);

                        std::string next_hop_id = client.routing_table.at(receiver_ID);
                        std::string next_hop_mac;
                        for(const auto& next_hop:clients) {
                            if(next_hop_id == next_hop.client_id) {
                                next_hop_mac = next_hop.client_mac;
                            }
                        }
                        PhysicalLayerPacket* physicalLayerPacket = new PhysicalLayerPacket(PHYSICAL_LAYER_ID, client.client_mac, next_hop_mac);

                        stack<Packet*> frames2;

                        frames2.push(applicationLayerPacket);
                        frames2.push(transportLayerPacket);
                        frames2.push(networkLayerPacket);
                        frames2.push(physicalLayerPacket);

                        client.outgoing_queue.push(frames2);

                        int frame_num = 1;
                        std::cout << "Frame #1" << std::endl;
                        std::cout << "Sender MAC address: " << physicalLayerPacket->sender_MAC_address << ", Receiver MAC address: " << physicalLayerPacket->receiver_MAC_address << std::endl;
                        std::cout << "Sender IP address: " << networkLayerPacket->sender_IP_address << ", Receiver IP address: " << networkLayerPacket->receiver_IP_address << std::endl;
                        std::cout << "Sender port number: " << transportLayerPacket->sender_port_number << ", Receiver port number: " << transportLayerPacket->receiver_port_number << std::endl;
                        std::cout << "Sender ID: " << applicationLayerPacket->sender_ID << ", Receiver ID: " << applicationLayerPacket->receiver_ID << std::endl;
                        std::cout << "Message chunk carried: \"" << applicationLayerPacket->message_data << "\"" << std::endl;
                        std::cout << "Number of hops so far: 0" << std::endl;
                        std::cout << "--------" << std::endl;

                        while(message_part_num*message_limit <= tempString.length()) {
                            applicationLayerPacket = new ApplicationLayerPacket(APPLICATION_LAYER_ID, sender_ID, receiver_ID, tempString.substr(message_part_num*message_limit, message_limit));
                            stack<Packet*> frames3;

                            frames3.push(applicationLayerPacket);
                            frames3.push(transportLayerPacket);
                            frames3.push(networkLayerPacket);
                            frames3.push(physicalLayerPacket);

                            client.outgoing_queue.push(frames3);
                            frame_num++;

                            std::cout << "Frame #" << frame_num << std::endl;
                            std::cout << "Sender MAC address: " << physicalLayerPacket->sender_MAC_address << ", Receiver MAC address: " << physicalLayerPacket->receiver_MAC_address << std::endl;
                            std::cout << "Sender IP address: " << networkLayerPacket->sender_IP_address << ", Receiver IP address: " << networkLayerPacket->receiver_IP_address << std::endl;
                            std::cout << "Sender port number: " << transportLayerPacket->sender_port_number << ", Receiver port number: " << transportLayerPacket->receiver_port_number << std::endl;
                            std::cout << "Sender ID: " << applicationLayerPacket->sender_ID << ", Receiver ID: " << applicationLayerPacket->receiver_ID << std::endl;
                            std::cout << "Message chunk carried: \"" << applicationLayerPacket->message_data << "\"" << std::endl;
                            std::cout << "Number of hops so far: 0"<< std::endl;
                            std::cout << "--------" << std::endl;

                            message_part_num++;
                        }
                    }
                }
            }




        } else if(command_first == "SHOW_FRAME_INFO") {
            std::string client_ID, queue_selection, frame_number;
            getline(ss, client_ID, ' ');
            getline(ss, queue_selection, ' ');
            getline(ss, frame_number, '\n');
            int fNum = stoi(frame_number);

            for(auto client:clients) {
                if(client_ID == client.client_id) {

                    if(queue_selection == "in") {
                        if(stoi(frame_number) > client.incoming_queue.size()) {
                            std::cout << "No such frame." << std::endl;
                            continue;
                        }
                        std::cout << "Current Frame #" << frame_number << " on the incoming queue of client " << client_ID << std::endl;
                        queue<stack<Packet*>> tempQ = client.incoming_queue;
                        while(fNum-1>0) {
                            tempQ.pop();
                            fNum--;
                        }
                        stack<Packet*> frames = tempQ.front();
                        PhysicalLayerPacket* physicalLayerPacket = dynamic_cast<PhysicalLayerPacket *>(frames.top());
                        frames.pop();
                        NetworkLayerPacket* networkLayerPacket = dynamic_cast<NetworkLayerPacket *>(frames.top());
                        frames.pop();
                        TransportLayerPacket* transportLayerPacket = dynamic_cast<TransportLayerPacket *>(frames.top());
                        frames.pop();
                        ApplicationLayerPacket* applicationLayerPacket = dynamic_cast<ApplicationLayerPacket *>(frames.top());
                        frames.pop();

                        std::string receiver = "";
                        for(auto &Client:clients) {
                            if(Client.client_mac == physicalLayerPacket->receiver_MAC_address) {
                                receiver = Client.client_id;
                            }
                        }
                        std::string next_hop_id;
                        for(auto c:clients) {
                            if(c.client_id == applicationLayerPacket->sender_ID) {
                                next_hop_id = c.routing_table.at(receiver);
                            }
                        }
                        int num_of_hops = find_num_of_hops(clients, applicationLayerPacket->sender_ID, next_hop_id, 0);

                        std::cout << "Carried Message: \" " << applicationLayerPacket->message_data << "\"" << std::endl;
                        std::cout << "Layer 0 info: Sender ID: " << applicationLayerPacket->sender_ID << ", Receiver ID: " << applicationLayerPacket->receiver_ID << std::endl;
                        std::cout << "Layer 1 info: Sender port number: " << transportLayerPacket->sender_port_number << ", Receiver port number: " << transportLayerPacket->receiver_port_number << std::endl;
                        std::cout << "Layer 2 info: Sender IP address: " << networkLayerPacket->sender_IP_address << ", Receiver IP address: " << networkLayerPacket->receiver_IP_address << std::endl;
                        std::cout << "Layer 3 info: Sender MAC address: " << physicalLayerPacket->sender_MAC_address << ", Receiver MAC address: " << physicalLayerPacket->receiver_MAC_address << std::endl;
                        std::cout << "Number of hops so far: " << num_of_hops << std::endl;

                    } else if(queue_selection == "out") {
                        if(stoi(frame_number) > client.outgoing_queue.size()) {
                            std::cout << "No such frame." << std::endl;
                            continue;
                        }
                        std::cout << "Current Frame #" << frame_number << " on the outgoing queue of client " << client_ID << std::endl;
                        queue<stack<Packet*>> tempQ = client.outgoing_queue;
                        while(fNum-1>0) {
                            tempQ.pop();
                            fNum--;
                        }
                        stack<Packet*> frames = tempQ.front();
                        PhysicalLayerPacket* physicalLayerPacket = dynamic_cast<PhysicalLayerPacket *>(frames.top());
                        frames.pop();
                        NetworkLayerPacket* networkLayerPacket = dynamic_cast<NetworkLayerPacket *>(frames.top());
                        frames.pop();
                        TransportLayerPacket* transportLayerPacket = dynamic_cast<TransportLayerPacket *>(frames.top());
                        frames.pop();
                        ApplicationLayerPacket* applicationLayerPacket = dynamic_cast<ApplicationLayerPacket *>(frames.top());
                        frames.pop();


                        std::string receiver = "";
                        for(auto &Client:clients) {
                            if(Client.client_mac == physicalLayerPacket->receiver_MAC_address) {
                                receiver = Client.client_id;
                            }
                        }
                        std::string next_hop_id;
                        for(auto c:clients) {
                            if(c.client_id == applicationLayerPacket->sender_ID) {
                                next_hop_id = c.routing_table.at(receiver);
                            }
                        }
                        int num_of_hops = find_num_of_hops(clients, applicationLayerPacket->sender_ID, next_hop_id, 0);

                        std::cout << "Carried Message: \"" << applicationLayerPacket->message_data << "\"" << std::endl;
                        std::cout << "Layer 0 info: Sender ID: " << applicationLayerPacket->sender_ID << ", Receiver ID: " << applicationLayerPacket->receiver_ID << std::endl;
                        std::cout << "Layer 1 info: Sender port number: " << transportLayerPacket->sender_port_number << ", Receiver port number: " << transportLayerPacket->receiver_port_number << std::endl;
                        std::cout << "Layer 2 info: Sender IP address: " << networkLayerPacket->sender_IP_address << ", Receiver IP address: " << networkLayerPacket->receiver_IP_address << std::endl;
                        std::cout << "Layer 3 info: Sender MAC address: " << physicalLayerPacket->sender_MAC_address << ", Receiver MAC address: " << physicalLayerPacket->receiver_MAC_address << std::endl;
                        std::cout << "Number of hops so far: " << num_of_hops-1  << std::endl;

                    }
                }
            }




        } else if(command_first == "SHOW_Q_INFO") {
            std::string client_ID, queue_selection;
            getline(ss, client_ID, ' ');
            getline(ss, queue_selection, '\n');

            for(auto &client:clients) {
                if(client_ID == client.client_id) {
                    if(queue_selection == "in") {
                        std::cout << "Client " << client.client_id << " Incoming Queue Status" << std::endl;
                        std::cout << "Current total number of frames: " << client.incoming_queue.size() << std::endl; // size is not working

                    } else if(queue_selection == "out") {
                        std::cout << "Client " << client.client_id << " Outgoing Queue Status" << std::endl;
                        std::cout << "Current total number of frames: " << client.outgoing_queue.size() << std::endl;
                    }
                }
            }

        } else if(command_first == "SEND") {
            int frame_num = 1;
            std::string fullMessage = "";
            for(auto &client:clients) {
                while(!client.outgoing_queue.empty()) {
                    stack<Packet*> frames = client.outgoing_queue.front();
                    PhysicalLayerPacket* physicalLayerPacket = dynamic_cast<PhysicalLayerPacket *>(frames.top());
                    frames.pop();
                    NetworkLayerPacket* networkLayerPacket = dynamic_cast<NetworkLayerPacket *>(frames.top());
                    frames.pop();
                    TransportLayerPacket* transportLayerPacket = dynamic_cast<TransportLayerPacket *>(frames.top());
                    frames.pop();
                    ApplicationLayerPacket* applicationLayerPacket = dynamic_cast<ApplicationLayerPacket *>(frames.top());
                    frames.pop();

                    std::string sender = "";
                    std::string receiver = "";
                    for(auto &Client:clients) {
                        if(Client.client_mac == physicalLayerPacket->sender_MAC_address) {
                            sender = Client.client_id;
                        }
                        if(Client.client_mac == physicalLayerPacket->receiver_MAC_address) {
                            receiver = Client.client_id;
                        }
                    }

                    std::string next_hop_id = client.routing_table.at(receiver);
                    int num_of_hops = find_num_of_hops(clients, applicationLayerPacket->sender_ID, next_hop_id, 0);

                    std::cout << "Client "<< sender << " sending frame #" << frame_num << " to client " << receiver << std::endl;
                    std::cout << "Sender MAC address: " << physicalLayerPacket->sender_MAC_address << ", Receiver MAC address: " << physicalLayerPacket->receiver_MAC_address << std::endl;
                    std::cout << "Sender IP address: " << networkLayerPacket->sender_IP_address << ", Receiver IP address: " << networkLayerPacket->receiver_IP_address << std::endl;
                    std::cout << "Sender port number: " << transportLayerPacket->sender_port_number << ", Receiver port number: " << transportLayerPacket->receiver_port_number << std::endl;
                    std::cout << "Sender ID: " << applicationLayerPacket->sender_ID << ", Receiver ID: " << applicationLayerPacket->receiver_ID << std::endl;
                    std::cout << "Message chunk carried: \"" << applicationLayerPacket->message_data << "\"" << std::endl;
                    std::cout << "Number of hops so far: " << num_of_hops << std::endl;
                    std::cout << "--------" << std::endl;

                    if(applicationLayerPacket->message_data[applicationLayerPacket->message_data.length() - 1] == '.' ||
                       applicationLayerPacket->message_data[applicationLayerPacket->message_data.length() - 1] == '!' ||
                       applicationLayerPacket->message_data[applicationLayerPacket->message_data.length() - 1] == '?') {
                        fullMessage += applicationLayerPacket->message_data;
                        for(auto &tempClient:clients) {
                            if(tempClient.client_mac == physicalLayerPacket->sender_MAC_address) {
                                char time[80];
                                std::time_t currentTime = std::time(nullptr);
                                std::strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

                                bool isInRoute = false;
                                for(auto &tempClient2:clients) {
                                    if(tempClient2.client_mac == physicalLayerPacket->receiver_MAC_address) {
                                        for(auto route:tempClient.routing_table) {
                                            if(route.first == tempClient2.client_id) {
                                                isInRoute = true;
                                            }
                                        }
                                    }
                                }
                                if(!isInRoute) {
                                    Log log = Log(time, fullMessage, frame_num, num_of_hops, applicationLayerPacket->sender_ID, applicationLayerPacket->receiver_ID, true, ActivityType::MESSAGE_DROPPED);
                                    tempClient.log_entries.push_back(log);
                                }

                                if(tempClient.client_id == applicationLayerPacket->sender_ID) { // sent
                                    Log log = Log(time, fullMessage, frame_num, num_of_hops-1, applicationLayerPacket->sender_ID, applicationLayerPacket->receiver_ID, true, ActivityType::MESSAGE_SENT);
                                    tempClient.log_entries.push_back(log);
                                } else { // forwarded
                                    Log log = Log(time, fullMessage, frame_num, num_of_hops-1, applicationLayerPacket->sender_ID, applicationLayerPacket->receiver_ID, true, ActivityType::MESSAGE_FORWARDED);
                                    tempClient.log_entries.push_back(log);
                                }
                            }
                        }
                        fullMessage = "";
                        frame_num = 1;

                    } else {
                        frame_num++;
                        fullMessage += applicationLayerPacket->message_data;
                    }

                    for(auto &client2:clients) {
                        if(physicalLayerPacket->receiver_MAC_address == client2.client_mac) {
                            stack<Packet*> frames2;

                            frames2.push(applicationLayerPacket);
                            frames2.push(transportLayerPacket);
                            frames2.push(networkLayerPacket);
                            frames2.push(physicalLayerPacket);

                            client2.incoming_queue.push(frames2);

                        }
                    }
                    client.outgoing_queue.pop();

                }

            }


        } else if(command_first == "RECEIVE") {
            int frame_num = 1;
            std::string fullMessage = "";

            for(auto &client:clients) {
                while (!client.incoming_queue.empty()) {
                    stack<Packet *> frames = client.incoming_queue.front();
                    PhysicalLayerPacket *physicalLayerPacket = dynamic_cast<PhysicalLayerPacket *>(frames.top());
                    frames.pop();
                    NetworkLayerPacket *networkLayerPacket = dynamic_cast<NetworkLayerPacket *>(frames.top());
                    frames.pop();
                    TransportLayerPacket *transportLayerPacket = dynamic_cast<TransportLayerPacket *>(frames.top());
                    frames.pop();
                    ApplicationLayerPacket *applicationLayerPacket = dynamic_cast<ApplicationLayerPacket *>(frames.top());
                    frames.pop();

                    fullMessage += applicationLayerPacket->message_data;

                    std::string sender = "";
                    std::string receiver = "";
                    for(auto &Client:clients) {
                        if(Client.client_mac == physicalLayerPacket->sender_MAC_address) {
                            sender = Client.client_id;
                        }
                        if(Client.client_mac == physicalLayerPacket->receiver_MAC_address) {
                            receiver = Client.client_id;
                        }
                    }


                    int num_of_hops = find_num_of_hops(clients, applicationLayerPacket->sender_ID, receiver, 0);

                    for(auto &client2:clients) {
                        if(physicalLayerPacket->receiver_MAC_address == client2.client_mac) {
                            if(client2.client_id == applicationLayerPacket->receiver_ID) {
                                if(applicationLayerPacket->message_data[applicationLayerPacket->message_data.length() - 1] == '.' ||
                                   applicationLayerPacket->message_data[applicationLayerPacket->message_data.length() - 1] == '!' ||
                                   applicationLayerPacket->message_data[applicationLayerPacket->message_data.length() - 1] == '?') {
                                    receivedFrames.push_back("Client "+ receiver +" receiving frame #" + std::to_string(frame_num) + " from client "+ sender +", originating from client " + applicationLayerPacket->sender_ID);
                                    receivedFrames.push_back("Sender MAC address: " + physicalLayerPacket->sender_MAC_address + ", Receiver MAC address: " + physicalLayerPacket->receiver_MAC_address);
                                    receivedFrames.push_back("Sender IP address: " + networkLayerPacket->sender_IP_address + ", Receiver IP address: " + networkLayerPacket->receiver_IP_address);
                                    receivedFrames.push_back("Sender port number: " + transportLayerPacket->sender_port_number + ", Receiver port number: " + transportLayerPacket->receiver_port_number);
                                    receivedFrames.push_back("Sender ID: " + applicationLayerPacket->sender_ID + ", Receiver ID: " + applicationLayerPacket->receiver_ID);
                                    receivedFrames.push_back("Message chunk carried: \"" + applicationLayerPacket->message_data + "\"");
                                    receivedFrames.push_back("Number of hops so far: " + std::to_string(num_of_hops));
                                    receivedFrames.push_back("--------");
                                    for(auto str:receivedFrames) {
                                        std::cout << str << std::endl;
                                    }
                                    std::cout << "Client " << applicationLayerPacket->receiver_ID <<" received the message \"" << fullMessage << "\" from client " << applicationLayerPacket->sender_ID << "." << std::endl;
                                    std::cout << "--------" << std::endl;
                                    receivedFrames = {};

                                    char time[80];
                                    std::time_t currentTime = std::time(nullptr);
                                    std::strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
                                    Log log = Log(time, fullMessage, frame_num, num_of_hops, applicationLayerPacket->sender_ID, applicationLayerPacket->receiver_ID, true, ActivityType::MESSAGE_RECEIVED);
                                    client2.log_entries.push_back(log);

                                    fullMessage = "";
                                    frame_num = 1;
                                    continue;
                                } else {
                                    receivedFrames.push_back("Client "+ receiver +" receiving frame #" + std::to_string(frame_num) + " from client "+ sender +", originating from client " + applicationLayerPacket->sender_ID);
                                    receivedFrames.push_back("Sender MAC address: " + physicalLayerPacket->sender_MAC_address + ", Receiver MAC address: " + physicalLayerPacket->receiver_MAC_address);
                                    receivedFrames.push_back("Sender IP address: " + networkLayerPacket->sender_IP_address + ", Receiver IP address: " + networkLayerPacket->receiver_IP_address);
                                    receivedFrames.push_back("Sender port number: " + transportLayerPacket->sender_port_number + ", Receiver port number: " + transportLayerPacket->receiver_port_number);
                                    receivedFrames.push_back("Sender ID: " + applicationLayerPacket->sender_ID + ", Receiver ID: " + applicationLayerPacket->receiver_ID);
                                    receivedFrames.push_back("Message chunk carried: \"" + applicationLayerPacket->message_data + "\"");
                                    receivedFrames.push_back("Number of hops so far: " + std::to_string(num_of_hops));
                                    receivedFrames.push_back("--------");
                                }

                            }else {
                                bool isPresent = false;
                                if(frame_num == 1) {
                                    std::cout << "Client " << receiver << " receiving a message from client " << sender << ", but intended for client "<< applicationLayerPacket->receiver_ID << ". Forwarding... " <<  std::endl;
                                }
                                std::string next_hop_id = client2.routing_table.at(applicationLayerPacket->receiver_ID);
                                std::string next_hop_mac;
                                for(const auto& next_hop:clients) {
                                    if(next_hop_id == next_hop.client_id) {
                                        next_hop_mac = next_hop.client_mac;
                                        isPresent = true;
                                    }
                                }

                                if(!isPresent) {
                                    std::cout << "Error: Unreachable destination. Packets are dropped after " << num_of_hops << " hops!" << std::endl;
                                    char time[80];
                                    std::time_t currentTime = std::time(nullptr);
                                    std::strftime(time, sizeof(time), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

                                    Log log = Log(time, fullMessage, frame_num, num_of_hops, applicationLayerPacket->sender_ID, applicationLayerPacket->receiver_ID, true, ActivityType::MESSAGE_DROPPED);
                                    client2.log_entries.push_back(log);
                                    continue;
                                }

                                PhysicalLayerPacket* physicalLayerPacket2 = new PhysicalLayerPacket(PHYSICAL_LAYER_ID, client2.client_mac, next_hop_mac);

                                stack<Packet*> frames2;

                                frames2.push(applicationLayerPacket);
                                frames2.push(transportLayerPacket);
                                frames2.push(networkLayerPacket);
                                frames2.push(physicalLayerPacket2);

                                std::cout << "Frame #" << frame_num << " MAC address change: New sender MAC " << physicalLayerPacket2->sender_MAC_address << ", new receiver MAC " << physicalLayerPacket2->receiver_MAC_address << std::endl;

                                if(applicationLayerPacket->message_data[applicationLayerPacket->message_data.length() - 1] == '.' ||
                                   applicationLayerPacket->message_data[applicationLayerPacket->message_data.length() - 1] == '!' ||
                                   applicationLayerPacket->message_data[applicationLayerPacket->message_data.length() - 1] == '?') {
                                    std::cout << "--------" << std::endl;
                                }

                                std::string receiver2 = ""; // check again
                                for(auto &Client:clients) {
                                    if(Client.client_mac == physicalLayerPacket2->receiver_MAC_address) {
                                        receiver2 = Client.client_id;
                                    }
                                }
                                client2.outgoing_queue.push(frames2);
                            }

                        }
                    }


                    frame_num++;
                    if(applicationLayerPacket->message_data[applicationLayerPacket->message_data.length() - 1] == '.' ||
                       applicationLayerPacket->message_data[applicationLayerPacket->message_data.length() - 1] == '!' ||
                       applicationLayerPacket->message_data[applicationLayerPacket->message_data.length() - 1] == '?') {
                        frame_num = 1;
                    }
                    client.incoming_queue.pop();
                }
            }

        } else if(command_first == "PRINT_LOG") {
            std::string client_ID;
            getline(ss, client_ID, '\n');

            for(auto &client:clients) {
                if(client_ID == client.client_id) {
                    if(client.log_entries.empty()) {
                        continue;
                    }
                    std::cout << "Client " << client_ID << " Logs:" << std::endl;
                    int logNum = 1;
                    for(auto &log:client.log_entries) {
                        std::cout << "--------------" << std::endl;
                        std::cout << "Log Entry #" << logNum << ":" << std::endl;
                        if(log.activity_type == ActivityType::MESSAGE_RECEIVED) {
                            std::cout << "Activity: Message Received" << std::endl;
                        }else if(log.activity_type == ActivityType::MESSAGE_FORWARDED){
                            std::cout << "Activity: Message Forwarded" << std::endl;
                        }else if(log.activity_type == ActivityType::MESSAGE_SENT){
                            std::cout << "Activity: Message Sent" << std::endl;
                        }else if(log.activity_type == ActivityType::MESSAGE_DROPPED){
                            std::cout << "Activity: Message Dropped" << std::endl;
                        }

                        std::cout << "Timestamp: " << log.timestamp << std::endl;
                        std::cout << "Number of frames: " << log.number_of_frames << std::endl;
                        std::cout << "Number of hops: " << log.number_of_hops << std::endl;
                        std::cout << "Sender ID: " << log.sender_id << std::endl;
                        std::cout << "Receiver ID: " << log.receiver_id << std::endl;
                        std::cout << "Success: " ;
                        if(log.success_status) {
                            std::cout << "Yes" << std::endl;
                            if(log.activity_type == ActivityType::MESSAGE_RECEIVED || log.activity_type == ActivityType::MESSAGE_SENT) {
                                std::cout << "Message: \"" << log.message_content << "\"" << std::endl;
                            }
                        } else {
                            std::cout << "No" << std::endl;
                        }

                        logNum++;
                    }

                }
            }

        } else {
            std::cout << "Invalid command." << std::endl;
        }
    }
}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    // TODO: Read clients from the given input file and return a vector of Client instances.

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(-1);
    }

    std::string line;
    std::getline(file, line);
    std::string client_id, client_ip, client_mac;
    int num_of_clients = std::stoi(line);
    while (getline (file, client_id, ' ') && num_of_clients>0) {
        getline(file, client_ip,    ' ');
        getline(file, client_mac,    '\n');

        Client client(client_id, client_ip, client_mac);
        clients.push_back(client);

        num_of_clients--;
    }
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(-1);
    }

    std::string line;
    int tempNum = 0;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string destination, nearest;

        if (line == "-") {
            tempNum++;
            continue;
        }

        if (iss >> destination >> nearest) {
            clients[tempNum].routing_table[destination] = nearest;
        }
    }
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.

    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(-1);
    }

    std::string line;
    std::getline(file, line);
    std::string client_id, client_ip, client_mac;
    int num_of_commands = std::stoi(line);
    while (std::getline(file, line)  && num_of_commands>0) {
        commands.push_back(line);
        num_of_commands--;
    }

    return commands;
}

int Network::find_num_of_hops(vector<Client> &clients, std::string sender_id, std::string receiver_id, int hops) {
    if (sender_id == receiver_id) {
        return hops;
    } else {
        for (auto client:clients) {
            if(client.client_id == sender_id) {
                std::string next_hop_id = client.routing_table.at(receiver_id);
                return find_num_of_hops(clients, next_hop_id, receiver_id, ++hops);
            }
        }
    }
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
}