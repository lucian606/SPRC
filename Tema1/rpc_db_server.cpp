/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpc_db.h"
#include <string>
#include <map>
#include <iostream>
#include <queue>

#define OK 0
#define USER_EXISTS -1
#define ERROR -1

#define ADDED 1
#define UPDATED 2

int currentId = 0;

std::map<std::string, int> loggedUsers;
std::map<std::string, std::map<int, struct SensorData>> dataBase;

float getMin(float *v, int len) {
	float min = v[0];
	for (int i = 1; i < len; i++)
		min = v[i] < min ? v[i] : min;
	return min;
}

float getMax(float *v, int len) {
	float max = v[0];
	for (int i = 1; i < len; i++)
		max = v[i] > max ? v[i] : max;
	return max;
}

float getMean(float *v, int len) {
	float sum = 0;
	for (int i = 0; i < len; i++)
		sum += v[i];
	return sum / len;
}

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

int *
add_1_svc(struct UserPackage *argp, struct svc_req *rqstp)
{
	static int result = ADDED;

	/*
	 * insert server code here
	 */
	struct SensorData data = argp->data;
	struct SensorData newData;
	std::cout << "Received data from user: " << argp->name << std::endl;
	std::cout << "Here are the received values: ";
	newData.dataId = data.dataId;
	newData.noValues = data.noValues;
	newData.value.value_len = data.noValues;
	newData.value.value_val = new float[data.noValues];

	for (int i = 0; i < data.noValues; i++) {
		std::cout << data.value.value_val[i] << " ";
		newData.value.value_val[i] = data.value.value_val[i];
	}

	if (dataBase[argp->name].find(data.dataId) != dataBase[argp->name].end())
		result = UPDATED;

	dataBase[argp->name][data.dataId] = newData;

	std::cout << std::endl;
	return &result;
}

int *
delete_1_svc(struct SpecificId *argp, struct svc_req *rqstp)
{
	static int result = OK;

	/*
	 * insert server code here
	 */

	std::cout << "User wants to delete data: " << argp->name << std::endl;

	if (dataBase.find(argp->name) == dataBase.end() || 
			dataBase[argp->name].find(argp->dataId) == dataBase[argp->name].end()) {		
		std::cout << "User wants to delete unexisting data: " << argp->name << std::endl;
		result = ERROR;
		return &result;
	}

	dataBase[argp->name].erase(argp->dataId);
	std::cout << " User deleted the data: " << argp->dataId << std::endl;
	result = OK;
	return &result;
}

int *
update_1_svc(struct UserPackage *argp, struct svc_req *rqstp)
{
	static int result = ADDED;

	struct SensorData data = argp->data;
	struct SensorData newData;
	std::cout << "Received data from user: " << argp->name << std::endl;
	std::cout << "Here are the received values: ";
	newData.dataId = data.dataId;
	newData.noValues = data.noValues;
	newData.value.value_len = data.noValues;
	newData.value.value_val = new float[data.noValues];

	for (int i = 0; i < data.noValues; i++) {
		std::cout << data.value.value_val[i] << " ";
		newData.value.value_val[i] = data.value.value_val[i];
	}

	if (dataBase[argp->name].find(data.dataId) != dataBase[argp->name].end())
		result = UPDATED;

	dataBase[argp->name][data.dataId] = newData;

	std::cout << std::endl;

	return &result;
}

char **
read_1_svc(struct SpecificId *argp, struct svc_req *rqstp)
{
	static char *result;

	/*
	 * insert server code here
	 */

	if (dataBase.find(argp->name) == dataBase.end() || 
			dataBase[argp->name].find(argp->dataId) == dataBase[argp->name].end()) {
		result = (char *) calloc(7, sizeof(char));			
		std::cout << "User wants to read unexisting data: " << argp->name << std::endl;
		strcpy(result, "ERROR");
		return &result;
	}
	
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

char **
getstat_1_svc(struct SpecificId *argp, struct svc_req *rqstp)
{
	static char * result;

	/*
	 * insert server code here
	 */


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
	
	float min = getMin(dataBase[argp->name][argp->dataId].value.value_val, size);
	float max = getMax(dataBase[argp->name][argp->dataId].value.value_val, size);
	float mean = getMean(dataBase[argp->name][argp->dataId].value.value_val, size);
	float median = getMedian(dataBase[argp->name][argp->dataId].value.value_val, size);

	std::string stringRes = "Here are the stats:\n";
	stringRes += "  Min value: " + std::to_string(min) + "\n";
	stringRes += "  Max value: " + std::to_string(max) + "\n";
	stringRes += "  Mean value: " + std::to_string(mean) + "\n";	
	stringRes += "  Median value: " + std::to_string(median) + "\n";
	result = (char *) calloc(stringRes.size() + 1, sizeof(char));	
	strcpy(result, stringRes.c_str());
	return &result;
}

char **
getstatall_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	std::cout << "User wants to get all of his stats: " << *argp << std::endl;
	std::string resultStr = "Here are all of your stats:\n[";

		
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

	/*
	 * insert server code here
	 */
	result = (char *) calloc(resultStr.size() + 1, sizeof(char));	
	strcpy(result, resultStr.c_str());
	return &result;
}

int *
load_1_svc(char **argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}

char **
store_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	std::cout << "User wants to store his data: " << *argp << std::endl;
	std::string resultStr = "[";

	for(std::map<int, struct SensorData>::iterator iter = dataBase[*argp].begin(); iter != dataBase[*argp].end(); ++iter) {
		int dataId = iter->first;
		resultStr += "\n{\n";
		resultStr += "  dataId: " + std::to_string(dataId) + '\n';
		resultStr += "  dataSize: " + std::to_string(iter->second.noValues) + '\n';
		resultStr += "  data: ";
		for (int i = 0; i < iter->second.noValues; i++)
			resultStr = resultStr + std::to_string(iter->second.value.value_val[i]) + " ";
		resultStr += "\n}";
	}

	resultStr += "\n]\n";

	/*
	 * insert server code here
	 */
	result = (char *) calloc(resultStr.size() + 1, sizeof(char));	
	strcpy(result, resultStr.c_str());
	return &result;
}

int *
login_1_svc(char **argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

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

int *
logout_1_svc(char **argp, struct svc_req *rqstp)
{
	static int  result = SUCCESS;

	/*
	 * insert server code here
	 */
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

char **
readall_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	std::cout << "User wants to read all of his data: " << *argp << std::endl;
	std::string resultStr = "Here is your data:\n[";

		
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

	/*
	 * insert server code here
	 */
	result = (char *) calloc(resultStr.size() + 1, sizeof(char));	
	strcpy(result, resultStr.c_str());
	return &result;
}