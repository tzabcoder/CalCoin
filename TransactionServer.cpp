/* -------------------------------------------------------------------------------------
CalCoin

FILENAME:    TransactionServer.cpp
COURSE:      Senior Project
DATE:        4-24-2022

DESCRIPTION:
    Function file to write all functions for the transaction server. 
	Utility files are used to perform data operations in addition
	to sending and receiveing data within the CalCoin ecosystem.
------------------------------------------------------------------------------------- */

#include "TransactionServer.h"
#include "VerifyTransaction.h"

/*
FUNCTION: validate_IP_address
PARAMETERS:
	const string ip_addr:   string containing IP address to check
RETURNS:
	boolean result:   true - if IP address is validated, otherwise false

DESCRIPTION:
	Accepts a string (by reference) as an IP address. Then checks whether or 
	not the IP address is valid and in correct format. Returns the verification 
	status of checking the string IP address.
*/
bool validate_IP_address(const string &ip_addr) {
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, ip_addr.c_str(), &(sa.sin_addr));

	return result != 0;
}

/*
FUNCTION: validate_wallet_length
PARAMETERS:
	char* wallet_addr:   c string of a wallet address
RETURNS:
	boolean result:   true - if wallet length is correct, otherwise false

DESCRIPTION:
	Accepts a C string of a wallet address. Function checks if the receved 
	wallet address is of the correct length. Returns the status of the 
	operation.
*/
bool validate_wallet_length(char* wallet_addr) {
	bool status = false;

	if (strlen(wallet_addr) == WALLET_LENGTH) {
		status = true;	
	}

	return status;
}

/*
FUNCTION: process_client_connection
PARAMETERS:
	int new_fd:   socket descriptior for client connection 
	int numbytes:   number of bytes to read from socket
	char* buf:   buffer for reading from socket 
RETURNS:
	string transaction_hash:   hashed string of transaction data from client

DESCRIPTION:
	Receives transaction data from the client, including the sender wallet address,
	receiver wallet address, and the amount to send to the new wallet address. The 
	function communicaes with the client connection to get the data. Function then 
	hashes all pieces of data through SHA256 to return the transaction hash
*/
string process_client_connection(int& new_fd, int& numbytes, char* buf) {
	//Client Transaction information 
	char sender_addr[BUFFERLEN];
	char receiver_addr[BUFFERLEN];
	char temp_amount[BUFFERLEN];
	int transaction_amount = 0;
	
	string transaction_hash;
	bool valid_wallet;
	
	//Send Request for Sender Address ------------------------------------------------
	if (send(new_fd, REQUEST_SENDERADDR, strlen(REQUEST_SENDERADDR), 0) == -1) {
		cerr << "ERROR: could not send server request..." << endl;
	}
	//Listen to accept the sender address
	if ((numbytes = recv(new_fd, buf, BUFFERLEN-1, 0)) == -1) {
		cerr << "ERROR: could not recieve client response..." << endl;
	}
	buf[numbytes] = '\0';
	strcpy(sender_addr, buf);

	valid_wallet = validate_wallet_length(sender_addr);
	if (valid_wallet == true) {
		//Send Request for Reciever Address ------------------------------------------------
		if (send(new_fd, REQUEST_RECEIVERADDR, strlen(REQUEST_RECEIVERADDR), 0) == -1) {
			cerr << "ERROR: could not send the server request..." << endl;
		}
		//Listen to accept the receiver address 
		if ((numbytes = recv(new_fd, buf, BUFFERLEN-1, 0)) == -1) {
			cerr << "ERROR: could not accept client response..." << endl;
		}
		buf[numbytes] = '\0';
		strcpy(receiver_addr, buf);

		valid_wallet = validate_wallet_length(receiver_addr);
		if (valid_wallet == true) {
			//Send Request for Transaction Amount ------------------------------------------------
			if (send(new_fd, REQUEST_AMOUNT, strlen(REQUEST_AMOUNT), 0) == -1) {
				cerr << "ERROR: could not send the server request..." << endl;
			}
			//Listen to accept the receiver address 
			if ((numbytes = recv(new_fd, buf, BUFFERLEN-1, 0)) == -1) {
				cerr << "ERROR: could not accept client response..." << endl;
			}
			buf[numbytes] = '\0';
			strcpy(temp_amount, buf);
			transaction_amount = atoi(temp_amount);

			if (transaction_amount > 0) {
				//Validate that the transaction was successful (send coins from one wallet addr to another)
				bool transaction_validated = process_transaction(sender_addr, receiver_addr, transaction_amount);

				if (transaction_validated == true) {
					Transaction t = Transaction(sender_addr, receiver_addr, transaction_amount);
					transaction_hash = t.GetHash();

					cout << "Transaction hashed:   " << transaction_hash << endl;
					if (send(new_fd, VALID_TRANSACTION, strlen(VALID_TRANSACTION), 0) == -1) {
						cerr << "ERROR: could not send the server request..." << endl;
					}
				}
				else {
					if (send(new_fd, INVALID_TRANSACTION, strlen(INVALID_TRANSACTION), 0) == -1) {
						cerr << "ERROR: could not send the server request..." << endl;
					}
					transaction_hash = "";
				}
			}
			else {
				if (send(new_fd, INVALID_AMOUNT, strlen(INVALID_AMOUNT), 0) == -1) {
					cerr << "ERROR: could not send the server request..." << endl;
				}
				transaction_hash = "";
			}
		}
		else {
			if (send(new_fd, INVALID_WALLET, strlen(INVALID_WALLET), 0) == -1) {
				cerr << "ERROR: could not send the server request..." << endl;
			}
			transaction_hash = "";
		}
	}
	else {
		if (send(new_fd, INVALID_WALLET, strlen(INVALID_WALLET), 0) == -1) {
			cerr << "ERROR: could not send the server request..." << endl;
		}
		transaction_hash = "";
	}

	return transaction_hash;
}

/*
FUNCTION: process_node_connection
PARAMETERS:
	char* buf:   buffer for getting data from socket
	int new_fd:   socket descriptor for the connecting node
	int numbytes:   number of bytes reading from socket 
	Vector_Structure<string> ip_addr_list:   IP address list (to add new node IP address)
	Vector_Structure<string> wallet_addr_list:   wallet address list (to add new node wallet addresses)
	Vector_Structure<string> stake_list:   stake list (to add new node stake amount)
RETURNS:
	nothing

DESCRIPTION:
	Function to communicate with the newly connected node. The function sends requests to the node
	to get it's wallet address, IP address, and stake amount. Once the data is received, all data 
	is added to the corresponding Vector_Structure<string> lists.
*/
void process_node_connection(char* buf, int& new_fd, int& numbytes, Vector_Structure<string> &ip_addr_list, Vector_Structure<string> &wallet_list, Vector_Structure<string> &stake_list) {
	bool ip_validated;
	string wallet_address;
	string ip_address;

	//Node is attempting to establish a connection with the CalCoin network
	if (strcmp(buf, "NODE") == 0) {
		//Subscription -> prompt for the Node's IP address ------------------------------------------------
		if (send(new_fd, SUBSCRIBE, strlen(SUBSCRIBE), 0) == -1) {
			cerr << "ERROR: could not send server request..." << endl;
		}
		//Listen to accept the node's IP address
		if ((numbytes = recv(new_fd, buf, BUFFERLEN-1, 0)) == -1) {
			cerr << "ERROR: could not recieve node response..." << endl;
		}
		buf[numbytes] = '\0';

		//Check if the buffer contains an IP address 
		ip_validated = validate_IP_address(buf);

		//IP was extracted and confirmed
		if (ip_validated == true) {
			ip_address = buf;

			if (send(new_fd, REQ_WALLETADDR, strlen(REQ_WALLETADDR), 0) == -1) {
				cerr << "ERROR: could not send the server request..." << endl;
			}
			//Listen to accept the sender address
			if ((numbytes = recv(new_fd, buf, BUFFERLEN-1, 0)) == -1) {
				cerr << "ERROR: could not recieve node response..." << endl;
			}
			buf[numbytes] = '\0';

			//1.) Add IP address to ip_addr_list
			//2.) Add node's wallet address to wallet_list
			wallet_address = buf;

			ip_addr_list.push(ip_address);
			wallet_list.push(wallet_address);
		}
		//Invalid IP
		else {
			if (send(new_fd, SUB_DENY, strlen(SUB_DENY), 0) == -1) {
				cerr << "ERROR: could not send the server request..." << endl;
			}
		}
	}
}

/*
FUNCTION: notify_network
PARAMETERS:
	vector<int> node_sockets:   vector or existing node socket descriptors
	int chosen_node:   chosen node to mine the next block
	string chosen_ip:   chosen node's IP address
	string chosen_wallet:   chosen node's wallet address
RETURNS:
	boolean result:   true - if network notified, otherwise false

DESCRIPTION:
	Notifys all nodes on the network what node is mining the next block. The 
	function either sends a CHOSEN message header or a NOT_CHOSEN method header 
	to the corresponding node. (message prep to send transaction data)
*/
bool notify_network(vector<int> &node_sockets, int chosen_node, string chosen_ip, string chosen_wallet) {
	bool notification_status = true;
	bool mining_status = true;

	int ip_len = chosen_ip.length();
	int wal_len = chosen_wallet.length();
	char mining_node_ip[ip_len + 1];
	char mining_node_wallet[wal_len + 1];

	strcpy(mining_node_ip, chosen_ip.c_str());
	strcpy(mining_node_wallet, chosen_wallet.c_str());

	for (int i = 0; i < node_sockets.size(); i++) {
		if (i == chosen_node) {
			//Send chosen node notification
			if (send(node_sockets[i], CHOSEN, sizeof(CHOSEN), 0) == -1) {
				cerr << "ERROR: could not notify miner..." << endl;
				notification_status = false;
			}
		}
		else {
			//Send not chosen notification
			if (send(node_sockets[i], NOT_CHOSEN, sizeof(NOT_CHOSEN), 0) == -1) {
				cerr << "ERROR: could not notify non-miners..." << endl;
				notification_status = false;
			}
			usleep(500);
			//Send Mining Node's Wallet Address
			cout << "Sending Mining Node's Wallet Address..." << endl;
			if (send(node_sockets[i], mining_node_wallet, sizeof(mining_node_wallet), 0) == -1) {
				cerr << "ERROR: sending mining node's wallet address..." << endl;
				notification_status = false;
			}
		}
	}

	return notification_status;
}

/*
FUNCTION: send_block_data
PARAMETERS:
	vector<int> node_sockets:   list of socket file descriptors (all connected nodes)
	int mining_node:   the current mining node
	char* mined_hash_block:   the hash of the newly mined block
RETURNS:
	boolean status:   the status of the notification throughout the network

DESCRIPTION:
	The function sends the block data (the hashed block from the mining node) to 
	all of the nodes on the network. Function sends the NEW_BLOCK message header 
	followed by the hash of the block
*/
bool send_block_data(vector<int> &node_sockets, int mining_node, char* mined_block_hash) {
	bool status = true;

	cout << "Sending: " << mined_block_hash << endl;

	//Propagate block information throughout the server
	usleep(500);
	for (int i = 0; i < node_sockets.size(); i++) {
		if (i != mining_node) {
			//Send block information
			if (send(node_sockets[i], NEW_BLOCK, sizeof(NEW_BLOCK), 0) == -1) {
				status = false;
			}
			usleep(500);
			if (send(node_sockets[i], mined_block_hash, strlen(mined_block_hash), 0) == -1) {
				status = false;
			}
		}
	}

	return status;
}

/*
FUNCTION: send_transaction_data
PARAMETERS:
	Vector_Structure<string> transaction_list:   list of all transactions
	int network_socket:   the chosen node socket descriptor
RETURNS:
	char* buffer:   Hash of the mined block

DESCRIPTION:
	Function sends all transactions to the mining node. The function then allows 
	for 5 seconds for the node to mine the node (timeout to mine the node). The function 
	then receives the block hash and returns it.
*/
char* send_transaction_data(Vector_Structure<string> &transaction_list, int network_socket) {
	bool mining_status = true;
	int transaction_len;
	int numbytes;
	static char buffer[BUFFERLEN];
	
	for (int i = 0; i < transaction_list.size(); i++) {
		usleep(500);
		transaction_len = transaction_list.get(i).length();
		char temp_transaction[transaction_len+1];
		strcpy(temp_transaction, transaction_list.get(i).c_str());

		if (send(network_socket, temp_transaction, sizeof(temp_transaction), 0) == -1) {
			mining_status = false;
		}
	}

	usleep(500);
	if (send(network_socket, END_STREAM, sizeof(END_STREAM), 0) == -1) {
		mining_status = false;
	}

	//Get the block hash
	sleep(5);
	if ((numbytes = recv(network_socket, buffer, BUFFERLEN-1, 0)) == -1) {
        perror("ERROR: server receiving error...");
    }
    buffer[numbytes] = '\0';

	if (strlen(buffer) == 0) {
		cout << "Timeout: mining process took too long..." << endl;
	}
	else {
		//Propogate block information to all nodes
		cout << endl << "Block Hash: " << buffer << endl;
	}

	return buffer;
}

/*
FUNCTION: mine_transaction_pool
PARAMETERS:
	Vector_Structure<string> t_pool:   transaction pool structure
	Vector_Structure<string> wallet_list:   list with all node wallet address 
	Vector_Structure<string> ip_addr_list:   list with all node's IP adress 
	Concensus c_model:   consensus model object for choosing the mining node
	vector<int> node_sockets:   all current connected nodes
RETURNS:
	nothing

DESCRIPTION:
	Function accepts all of the listed parameters and selects a mining node from all connected 
	nodes. After selecting the mining node, the transaction server notifies the entire network
	(mining and non-mining nodes) who is mining. After the mining node is selected, all information 
	is sent to the mining and non-mining nodes.
*/
void mine_transaction_pool (Vector_Structure<string> &t_pool, Vector_Structure<string> &wallet_list, Vector_Structure<string> &ip_addr_list, Consensus c_model, vector<int> &node_sockets) {	
	int num_miners;
	int chosen_node;
	string chosen_ip;
	string chosen_wallet;
	bool status = false;
	char* b_hash;

	while (status == false) {
		num_miners = node_sockets.size();
		chosen_node = c_model.choose_mining_node(num_miners);

		chosen_ip = ip_addr_list.get(chosen_node);
		chosen_wallet = wallet_list.get(chosen_node);

		status = notify_network(node_sockets, chosen_node, chosen_ip, chosen_wallet);
		
		//Network was notified
		if (status == true) {
			b_hash = send_transaction_data(t_pool, node_sockets[chosen_node]);
			cout << b_hash << endl;
			status = send_block_data(node_sockets, chosen_node, b_hash);

			if (status == true) {
				cout << "Mined properly..." << endl;
			}
			else {
				cout << "Improper mine, redo..." << endl;
			}
		}
	}
}

/*
FUNCTION: log_wallet_address
PARAMETERS:
	int socket_fd:   the connecting client socket descriptor
RETURNS:
	nothing

DESCRIPTION:
	This function requests the wallet address from the client to log to the 
	database. The function gets the wallet address from the client and validates 
	the length of the received wallet. If the length of the wallet address is 
	valid, it is logged to the file for future use.
*/
void log_wallet_address(int socket_fd) {
	int numbytes;
	char buffer[BUFFERLEN];
	bool wallet_status;

	if (send(socket_fd, LOG_WALLET, sizeof(LOG_WALLET), 0) == -1) {
		cerr << "ERROR: request wallet to log." << endl;
	}
	if ((numbytes = recv(socket_fd, buffer, BUFFERLEN-1, 0)) == -1) {
        perror("ERROR: server receiving error...");
    }
    buffer[numbytes] = '\0';

	wallet_status = validate_wallet_length(buffer);
	if (wallet_status == true) {
		int init_amount = 0;
		ofstream wallet_database;
		
		wallet_database.open(ADDRESSBASE, ios_base::app);
		if (wallet_database.is_open()) {
			wallet_database << buffer << " - " << init_amount << endl;
		}
		wallet_database.close();
	}
}

/*
FUNCTION: retreive_wallet_address
PARAMETERS:
	int socket_fd:   Socket descriptor of the the client 
RETURNS:
	nothing

DESCRIPTION:
	The function opens the database file on the server and checks 
	to see if the wallet address exists. It checks the existence of the 
	wallet address and sends the corresponding message back to the client.
*/
void retreive_wallet_balance(int socket_fd) {
	int numbytes;
	char buffer[BUFFERLEN];
	string r_addr;
	fstream wallet_database;

	if (send(socket_fd, REQ_WALLETADDR, sizeof(REQ_WALLETADDR), 0) == -1) {
		cerr << "ERROR: cannot request address" << endl;
	}
	if ((numbytes = recv(socket_fd, buffer, BUFFERLEN-1, 0)) == -1) {
        perror("ERROR: server receiving error...");
    }
    buffer[numbytes] = '\0';
	r_addr = buffer;

	wallet_database.open(ADDRESSBASE);
	if (wallet_database.is_open()) {
		int len;
		string temp;
		string address;
		string amount;
		bool status;

		while (getline(wallet_database, temp)) {
			len = temp.length();
			address = temp.substr(0, WALLET_LENGTH);
			if (address.compare(r_addr) == 0) {
				amount = temp.substr(WALLET_LENGTH+3, len);

				int l = amount.length();
				char temp_amount[l + 1];
				strcpy(temp_amount, amount.c_str());
				send(socket_fd, temp_amount, sizeof(temp_amount), 0);
				status = true;
				break;
			}
			else {
				status = false;
			}
		}

		if (status == false) {
			char* inval_msg = "invalid_message";
			send(socket_fd, inval_msg, sizeof(inval_msg), 0);
		}
	}
	wallet_database.close();
}

/*
FUNCTION: process_transaction
PARAMETERS:
	char* sender_addr:   wallet address of the sender
	char* receiver_addr:   wallet address of the receiver
	int transaction_amount:   amount to send between the two wallet address
RETURNS:
	boolean status:   returns the status of the transaction

DESCRIPTION:
	Function transfers the coin between the sender address and the receiver wallet
	address.
*/
bool process_transaction(char* sender_addr, char* receiver_addr, int transaction_amount) {
	ifstream database;
	bool status;

	database.open(ADDRESSBASE);
	if (database.is_open()) {
		//Verify sender has enough coins
		status = verify_sender_amount(sender_addr, transaction_amount, WALLET_LENGTH);
		if (status == true) {
			status = verify_wallet_address(receiver_addr, WALLET_LENGTH);
			if (status == true) {
				//Transfer value from sender to receiver
				transfer_value(transaction_amount, sender_addr, receiver_addr);
			}
			else {
				//No receiver wallet address
				status = false;
			}
		}
		else {
			//Sender does not have enough coins
			status = false;
		}
	}

	database.close();

	return status;
}