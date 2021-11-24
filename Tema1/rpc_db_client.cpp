#include "rpc_db.h"
#include <iostream>
#include <fstream>

#define LENGTH 100
#define USER_EXISTS -1
#define NOT_LOGGED_IN -2
#define ERROR -1
#define OK 0

#define ADDED 1
#define UPDATED 2

// Logeaza userul folosind un nume
int login(char *host, char *name) {
	CLIENT *clnt;
	int *result_1;

#ifndef	DEBUG
	clnt = clnt_create (host, RPC_DB, RPC_DB_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif

	result_1 = login_1(&name, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	} else {
		if (*result_1 == USER_EXISTS) {
			std::cout << "This user is already logged in" << std::endl;
		} else {
			std::cout << "Login success. Here is your cookie key: " << *result_1 << std::endl;
		}
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif
	return *result_1;
}

// Citestea tot DB-ul userului cu numele name
char *readall(char *host, char *name) {
	CLIENT *clnt;
	char **result_1;

#ifndef	DEBUG
	clnt = clnt_create (host, RPC_DB, RPC_DB_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif

	result_1 = readall_1(&name, clnt);
	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	} else {
		std::cout << *result_1;
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif
	return *result_1;
}

// Obtine tot DB-ul de la server si il stocheaza
char *store(char *host, char *name) {
	CLIENT *clnt;
	char **result_1;

#ifndef	DEBUG
	clnt = clnt_create (host, RPC_DB, RPC_DB_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif

	result_1 = store_1(&name, clnt);
	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif
	return *result_1;
}

// Incarca continutul fisierului userului in DB-ul din server
int *load(char *host, char *name, char *jsonData) {
	CLIENT *clnt;
	int *result_1;
	LoadData data = {name, jsonData};

#ifndef	DEBUG
	clnt = clnt_create (host, RPC_DB, RPC_DB_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif

	result_1 = load_1(&data, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	if (*result_1 == ERROR) {
		std::cout << "Error while loading data, you altered the server DB" << std::endl;
	} else {
		std::cout << "Data loaded" << std::endl;
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif
	return result_1;
}

// Obtine toate statisticile userului
void getstatall(char *host, char *name) {
	CLIENT *clnt;
	char **result_1;

#ifndef	DEBUG
	clnt = clnt_create (host, RPC_DB, RPC_DB_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif

	result_1 = getstatall_1(&name, clnt);
	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	} else {
		std::cout << *result_1;
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif
}

// Delogheaza userul cu numele name
int logout(char *host, char *name) {
	CLIENT *clnt;
	int *result_1;

#ifndef	DEBUG
	clnt = clnt_create (host, RPC_DB, RPC_DB_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif

	result_1 = logout_1(&name, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	} else {
		if (*result_1 == ERROR) {
			std::cout << "Error while logging out" << std::endl;
		} else {
			std::cout << "Logout success" << std::endl;
		}
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif
	return *result_1;

}

// Adauga in DB-ul userului entry-ul dat
int add(char *host, char *username, struct SensorData entry) {
	
	CLIENT *clnt;
	int *result_1;
	struct UserPackage p = {entry, username};

#ifndef	DEBUG
	clnt = clnt_create (host, RPC_DB, RPC_DB_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif

	result_1 = add_1(&p, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	} else {
		if (*result_1 == ERROR) {
			std::cout << "Error while adding data, id already exists" << std::endl;
		} else {
			std::cout << "Add was successful" << std::endl;
		}
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif
	return *result_1;
}

// Citeste entry-ul cu id-ul id din DB-ul userului cu numele username
void read(char *host, char *username, int dataId) {
	
	CLIENT *clnt;
	char **result_1;
	struct SpecificId p = {dataId, username};

#ifndef	DEBUG
	clnt = clnt_create (host, RPC_DB, RPC_DB_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif

	result_1 = read_1(&p, clnt);
	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	} else if (strcmp(*result_1, "ERROR")) {
			std::cout << "Data read successfully" << std::endl;
			std::cout << *result_1;
	} else {
				std::cout << "Invalid id, the entry doesn't exist\n";
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif
}

// Obtine statisticile entry-ului id din DB-ul userului cu numele username
void getstat(char *host, char *username, int dataId) {
	
	CLIENT *clnt;
	char **result_1;
	struct SpecificId p = {dataId, username};

#ifndef	DEBUG
	clnt = clnt_create (host, RPC_DB, RPC_DB_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif

	result_1 = getstat_1(&p, clnt);
	if (result_1 == (char **) NULL) {
		clnt_perror (clnt, "call failed");
	} else if (strcmp(*result_1, "ERROR")) {
		std::cout << "Stats read successfully" << std::endl;
		std::cout << *result_1;
	} else {
		std::cout << "Invalid id, the entry doesn't exist\n";
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif
}

// Sterge entry-ul cu id-ul id din DB-ul userului cu numele username
void del(char *host, char *username, int dataId) {
	
	CLIENT *clnt;
	int *result_1;
	struct SpecificId p = {dataId, username};

#ifndef	DEBUG
	clnt = clnt_create (host, RPC_DB, RPC_DB_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif

	result_1 = delete_1(&p, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	} else if (*result_1 == OK) {
			std::cout << "Data deleted successfully" << std::endl;
	} else {
			std::cout << "Invalid id, the entry doesn't exist\n";
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif
}

// Updateaza entry-ul cu id-ul dat cu un nou entry
int update(char *host, char *username, struct SensorData entry) {
	CLIENT *clnt;
	int *result_1;
	struct UserPackage p = {entry, username};

#ifndef	DEBUG
	clnt = clnt_create (host, RPC_DB, RPC_DB_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif

	result_1 = update_1(&p, clnt);
	if (result_1 == (int *) NULL) {
		clnt_perror (clnt, "call failed");
	} else {
		if (*result_1 == UPDATED) {
			std::cout << "The entry was updated" << std::endl;
		} else {
			std::cout << "A new entry was added" << std::endl;
		}
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif
	return *result_1;
}

int main(int argc, char *argv[]) {
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	
	host = argv[1];
	std::string command;
	bool keepRunning = true;
	int userKey = NOT_LOGGED_IN;
	int r;
	char username[LENGTH];
	std::string unauthorized_msg = "You need to be logged in to use this command.\n";
	std::string user_file;
	std::ifstream input;
	input.open(argv[2]);

	while (keepRunning) {
		input >> command;

		if (command == "login") {
			if (userKey < 0) {
				input >> username;
				userKey = login(host, username);
				user_file = std::string(username) + ".rpcdb";
			} else {
				std::cout << "You are already logged in as: " << username << std::endl;
			}
		}

		else if (command == "add") {
			if (userKey < 0) {
				std::cout << unauthorized_msg;
				continue;
			}
			int dataId;
			int noValues;
			float *values;
			input >> dataId;
			input >> noValues;
			struct SensorData entry;
			entry.dataId = dataId;
			entry.noValues = noValues;
			entry.value.value_len = noValues;
			entry.value.value_val = new float[noValues + 1];
			for (int i = 0; i < noValues; ++i) {
				input >> entry.value.value_val[i];
			}
			add(host, username, entry);
		}

		else if (command == "read") {
			if (userKey < 0) {
				std::cout << unauthorized_msg;
				continue;
			}
			int dataId;
			input >> dataId;
			read(host, username, dataId);
		}

		else if (command == "getstat") {
			if (userKey < 0) {
				std::cout << unauthorized_msg;
				continue;
			}
			int dataId;
			input >> dataId;
			getstat(host, username, dataId);
		}

		else if (command == "readall") {
			if (userKey < 0) {
				std::cout << unauthorized_msg;
				continue;
			}
			readall(host, username);
		}

		else if (command == "getstatall") {
			if (userKey < 0) {
				std::cout << unauthorized_msg;
				continue;
			}
			getstatall(host, username);
		}

		else if (command == "del") {
			if (userKey < 0) {
				std::cout << unauthorized_msg;
				continue;
			}
			int dataId;
			input >> dataId;
			del(host, username, dataId);
		}		

		else if (command == "logout") {
			r = logout(host, username);
			if (r == SUCCESS || userKey < 0) {
				std::cout << "Logging user out\n";
				keepRunning = false;
			}
		}

		else if (command == "store") {
			if (userKey < 0) {
				std::cout << unauthorized_msg;
				continue;
			}
			std::cout << "Storing the following data:\n";
			std::string result = std::string(store(host, username));
			std::cout << result;
			std::ofstream outfile;
			outfile.open(user_file);
			outfile << result;
			outfile.close();
		}

		else if (command == "load") {
			if (userKey < 0) {
				std::cout << unauthorized_msg;
				continue;
			}
			std::cout << "Loading data from file\n";
			std::string result = "";
			std::string line;
			std::ifstream infile;
			infile.open(user_file);
			while(std::getline(infile, line)) {
				result += line;
				result += "\n";
			}
			std::cout << result;
			infile.close();
			char *result_c = new char[result.length() + 1];
			strcpy(result_c, result.c_str());
			load(host, username, result_c);
		}

		else if (command == "update") {
			if (userKey < 0) {
				std::cout << unauthorized_msg;
				continue;
			}
			int dataId;
			int noValues;
			float *values;
			input >> dataId;
			input >> noValues;
			struct SensorData entry;
			entry.dataId = dataId;
			entry.noValues = noValues;
			entry.value.value_len = noValues;
			entry.value.value_val = new float[noValues + 1];
			for (int i = 0; i < noValues; ++i) {
				input >> entry.value.value_val[i];
			}
			update(host, username, entry);			
		}

	}

	input.close();

	exit (0);
}