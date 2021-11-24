#include "rpc_db.h"
#include "nlohmann/json.hpp"
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <queue>

#define OK 0
#define USER_EXISTS -1
#define ERROR -1

#define ADDED 1
#define UPDATED 2

// Cheia de autentificare a ultimului utilizator
int currentId = 0;

// Hashmap pentru a retine ce utilizatori sunt logati
std::map<std::string, int> loggedUsers;

// Hasmap pentru a retine DB pentru fiecare user
std::map<std::string, std::map<int, struct SensorData>> dataBase;

// Extrage valoarea minima din array
float getMin(float *v, int len) {
	float min = v[0];
	for (int i = 1; i < len; i++)
		min = v[i] < min ? v[i] : min;
	return min;
}


// Extrage valoarea maxima din array
float getMax(float *v, int len) {
	float max = v[0];
	for (int i = 1; i < len; i++)
		max = v[i] > max ? v[i] : max;
	return max;
}

// Obtine media aritmetica a unui array
float getMean(float *v, int len) {
	float sum = 0;
	for (int i = 0; i < len; i++)
		sum += v[i];
	return sum / len;
}

// Obtine mediana a unui array
float getMedian(float *v, int len) {
	std::priority_queue<float, std::vector<float>, std::greater<float>> pq;
	float *aux = new float[len + 1];
	for (int i = 0; i < len; i++)
		pq.push(v[i]);
	for (int i = 0; i < len; i++) {
		aux[i] = pq.top();
		pq.pop();
	}
	if (len % 2 != 0) 
		return aux[len / 2];
	return (aux[(len - 1) / 2] + aux [len / 2]) / 2;
}


// Functia apelata de user pentru a adauga date
int *
add_1_svc(struct UserPackage *argp, struct svc_req *rqstp)
{
	static int result = ADDED;
	result = ADDED;

	// Extrag datele
	struct SensorData data = argp->data;
	struct SensorData newData;
	std::cout << "User wants to add data entry: " << argp->name << std::endl;
	std::cout << "  Here are the received values for add: ";
	newData.dataId = data.dataId;
	newData.noValues = data.noValues;
	newData.value.value_len = data.noValues;
	newData.value.value_val = new float[data.noValues];

	for (int i = 0; i < data.noValues; i++) {
		std::cout << data.value.value_val[i] << " ";
		newData.value.value_val[i] = data.value.value_val[i];
	}

	std::cout << std::endl;

	// Verific daca ID-ul este deja in DB
	if (dataBase[argp->name].find(data.dataId) != dataBase[argp->name].end()) {
		result = ERROR;
		std::cout << "  The user tried to add already existing id\n";

		return &result;
	}

	std::cout << "  User add success\n";
	
	// Adaug datele in DB daca acestea nu exista
	dataBase[argp->name][data.dataId] = newData;

	return &result;
}

// Functia apelata de user pentru a sterge date
int *
delete_1_svc(struct SpecificId *argp, struct svc_req *rqstp)
{
	static int result = OK;
	result = OK;

	std::cout << "User wants to delete data: " << argp->name << std::endl;

	// Verific daca exista datele cu ID-ul specificat
	if (dataBase.find(argp->name) == dataBase.end() || 
			dataBase[argp->name].find(argp->dataId) == dataBase[argp->name].end()) {		
		std::cout << "  User wants to delete unexisting data: " << argp->name << std::endl;
		result = ERROR;
		return &result;
	}

	// Daca exista, atunci sterg datele din DB
	dataBase[argp->name].erase(argp->dataId);
	std::cout << "  User deleted the data: " << argp->dataId << std::endl;
	result = OK;
	return &result;
}


// Functia apelata de user pentru a actualiza date
int *
update_1_svc(struct UserPackage *argp, struct svc_req *rqstp)
{
	static int result = UPDATED;
	result = UPDATED;	

	// Extrag datele
	struct SensorData data = argp->data;
	struct SensorData newData;
	std::cout << "User wants to update data: " << argp->name << std::endl;
	std::cout << "  Here are the updated values: ";
	newData.dataId = data.dataId;
	newData.noValues = data.noValues;
	newData.value.value_len = data.noValues;
	newData.value.value_val = new float[data.noValues];

	for (int i = 0; i < data.noValues; i++) {
		std::cout << data.value.value_val[i] << " ";
		newData.value.value_val[i] = data.value.value_val[i];
	}

	std::cout << std::endl;

	// Verific daca exista datele cu ID-ul specificat
	if (dataBase.find(argp->name) == dataBase.end() || 
			dataBase[argp->name].find(argp->data.dataId) == dataBase[argp->name].end()) {
		result = ADDED;
		std::cout << "  Update call adds new data entry: " << argp->data.dataId << std::endl;
	} else {
		std::cout << "  Update call updates existing data entry" << std::endl;
	}

	dataBase[argp->name][argp->data.dataId] = newData;

	return &result;
}

// Functia apelata de user pentru a citi un ID
char **
read_1_svc(struct SpecificId *argp, struct svc_req *rqstp)
{
	static char *result;

	// Verific daca exista datele cu ID-ul specificat
	if (dataBase.find(argp->name) == dataBase.end() || 
			dataBase[argp->name].find(argp->dataId) == dataBase[argp->name].end()) {
		result = (char *) calloc(7, sizeof(char));			
		std::cout << "User wants to read unexisting data: " << argp->name << std::endl;
		strcpy(result, "ERROR");
		return &result;
	}
	
	// Extrag rezultatul
	int noValues = dataBase[argp->name][argp->dataId].noValues;
	float *values = dataBase[argp->name][argp->dataId].value.value_val;
	std::string resultStr;
	std::cout << "User wants to read data: " << argp->name << std::endl;

	resultStr += "  Data id: " + std::to_string(argp->dataId) + '\n';
	resultStr += "  Data size: " + std::to_string(dataBase[argp->name][argp->dataId].noValues) + '\n';
	resultStr += "  Data: ";

	for (int i = 0; i < noValues; i++)
		resultStr = resultStr + std::to_string(values[i]) + " ";

	resultStr += '\n';
	std::cout << resultStr;	
	resultStr = "Here is the received data:\n" + resultStr;
	result = (char *) calloc(resultStr.size() + 1, sizeof(char));	
	strcpy(result, resultStr.c_str());
	return &result;
}


// Functia apelata de user pentru a obtine statisticile unui entry
char **
getstat_1_svc(struct SpecificId *argp, struct svc_req *rqstp)
{
	static char * result;

	// Verific daca exista datele cu ID-ul specificat
	if (dataBase.find(argp->name) == dataBase.end() || 
			dataBase[argp->name].find(argp->dataId) == dataBase[argp->name].end()) {
		result = (char *) calloc(7, sizeof(char));			
		std::cout << "User wants to get unexisting stats: " << argp->name << std::endl;
		strcpy(result, "ERROR");
		return &result;
	}	 

	int size = dataBase[argp->name][argp->dataId].noValues;

	std::cout << "User wants to get stats: " << argp->name << std::endl;
	std::cout << "  Of the entry with id: " << argp->dataId << std::endl;
	
	// Calculez statisticile
	float min = getMin(dataBase[argp->name][argp->dataId].value.value_val, size);
	float max = getMax(dataBase[argp->name][argp->dataId].value.value_val, size);
	float mean = getMean(dataBase[argp->name][argp->dataId].value.value_val, size);
	float median = getMedian(dataBase[argp->name][argp->dataId].value.value_val, size);

	std::string stringRes = "Here are the stats:\n";
	stringRes += "  Data Id: " + std::to_string(argp->dataId) + '\n';
	stringRes += "  Min value: " + std::to_string(min) + "\n";
	stringRes += "  Max value: " + std::to_string(max) + "\n";
	stringRes += "  Mean value: " + std::to_string(mean) + "\n";	
	stringRes += "  Median value: " + std::to_string(median) + "\n";
	result = (char *) calloc(stringRes.size() + 1, sizeof(char));	
	strcpy(result, stringRes.c_str());
	return &result;
}


// Functia apelata de user pentru a obtine toate statisticile din DB
char **
getstatall_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	std::cout << "User wants to get all of his stats: " << *argp << std::endl;
	std::string resultStr = "Here are all of your stats:\n[";

	// Iterez prin DB si iau statisticile pentru fiecare entry
	for(std::map<int, struct SensorData>::iterator iter = dataBase[*argp].begin(); iter != dataBase[*argp].end(); ++iter) {
		int dataId = iter->first;
		struct SensorData data = iter->second;
		resultStr += "\n{\n";
		resultStr += "  Data id: " + std::to_string(dataId) + '\n';
		resultStr += "  Min value: " + std::to_string(getMin(data.value.value_val, data.noValues)) + '\n';
		resultStr += "  Max value: " + std::to_string(getMax(data.value.value_val, data.noValues)) + '\n';
		resultStr += "  Mean value: " + std::to_string(getMean(data.value.value_val, data.noValues)) + '\n';
		resultStr += "  Median value: " + std::to_string(getMedian(data.value.value_val, data.noValues));		
		resultStr += "\n}";
	}

	resultStr += "\n]\n";

	// Trimist rezultatul
	result = (char *) calloc(resultStr.size() + 1, sizeof(char));	
	strcpy(result, resultStr.c_str());
	return &result;
}

// Functie apelata de user pentru a incarca baza de date din fisier
int *
load_1_svc(LoadData *argp, struct svc_req *rqstp)
{
	static int result = OK;
	result = OK;

	if (!dataBase[argp->name].empty()) {
		std::cout << "User wants to load data after altering DB\n" << argp->name << std::endl;
		result = ERROR;
		return &result;
	}

	std::cout << "User wants to load DB from file: " << argp->name << std::endl;
	std::stringstream ss(argp->dataJson);
	int dataId;
	dataBase[argp->name].clear();
	
	// Citirea datelor din fisier (string-ul cu continutul fisierului)
	// Inserez datele in DB
	while (ss >> dataId) {
		struct SensorData data;
		data.dataId = dataId;
		std::cout << "  Data id: " << dataId << std::endl;
		int noValues;
		ss >> noValues;
		data.noValues = noValues;
		data.value.value_val = (float *) calloc(noValues, sizeof(float));
		data.value.value_len = noValues;
		std::cout << "  Data size: " << noValues << std::endl;
		std::cout << "  Data: ";
		for (int i = 0; i < noValues; i++) {
			float value;
			ss >> value;
			data.value.value_val[i] = value;
			std::cout << value << " ";
		}
		std::cout << std::endl;
		dataBase[argp->name][dataId] = data;
	}

	return &result;
}

// Functie apelata de user pentru a incarca baza de date in fisier
char **
store_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	std::cout << "User wants to store his data: " << *argp << std::endl;
	std::string resultStr = "";

	// Iau fiecare entry din map si il adaug intr-un string
	for(std::map<int, struct SensorData>::iterator iter = dataBase[*argp].begin(); iter != dataBase[*argp].end(); ++iter) {
		int dataId = iter->first;
		resultStr += std::to_string(dataId) + ' ';
		resultStr += std::to_string(iter->second.noValues) + ' ';
		for (int i = 0; i < iter->second.noValues; i++) {
			resultStr = resultStr + std::to_string(iter->second.value.value_val[i]);
			if (i < iter->second.noValues - 1)
				resultStr += " ";
		}
		resultStr += "\n";
	}

	// Trimit stringul la client si acesta il scrie in fisier
	result = (char *) calloc(resultStr.size() + 1, sizeof(char));	
	strcpy(result, resultStr.c_str());
	return &result;
}


// Functie apelata de user pentru a se loga
int *
login_1_svc(char **argp, struct svc_req *rqstp)
{
	static int result;
	result = OK;
	
	// Verific daca userul este logat
	if (loggedUsers.find(*argp) == loggedUsers.end()) {
		std::cout << "User logged in: " << *argp << std::endl;
		loggedUsers[*argp] = currentId;
				result = currentId;
		currentId++;
	} else {
		std::cout << "User was already logged in: " << *argp << std::endl;
		result = USER_EXISTS;
	}


	return &result;
}

// Functie apelata de user pentru a se deloga
int *
logout_1_svc(char **argp, struct svc_req *rqstp)
{
	static int result = OK;

	// Verific daca userul e logat
	if (loggedUsers.find(*argp) == loggedUsers.end()) {
		std::cout << "User is not logged in: " << *argp << std::endl;
		result = ERROR;
	} else {
		loggedUsers.erase(*argp);
		dataBase.erase(*argp);
		std::cout << "User was logged out: " << *argp << std::endl;
		result = OK;
	}

	return &result;
}

// Functie apelata de user pentru a citit tot DB-ul lui
char **
readall_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	std::cout << "User wants to read all of his data: " << *argp << std::endl;
	std::string resultStr = "Here is your data:\n[";

	// Parcurg fiecare entry din DB si il adaug in string
	for(std::map<int, struct SensorData>::iterator iter = dataBase[*argp].begin(); iter != dataBase[*argp].end(); ++iter) {
		int dataId = iter->first;
		resultStr += "\n{\n";
		resultStr += "  Data id: " + std::to_string(dataId) + '\n';
		resultStr += "  Data size: " + std::to_string(iter->second.noValues) + '\n';
		resultStr += "  Data: ";
		for (int i = 0; i < iter->second.noValues; i++)
			resultStr = resultStr + std::to_string(iter->second.value.value_val[i]) + " ";
		resultStr += "\n}";
	}

	resultStr += "\n]\n";

	// Trimit stringul la user
	result = (char *) calloc(resultStr.size() + 1, sizeof(char));	
	strcpy(result, resultStr.c_str());
	return &result;
}