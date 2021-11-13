/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "rpc_db.h"
#include <string>
#include <map>
#include <iostream>

#define OK 0
#define USER_EXISTS -1
#define ERROR -1

int currentId = 0;

std::map<std::string, int> loggedUsers;
std::map<std::string, std::map<std::string, struct SensorData>> dataBase;

int *
add_1_svc(struct UserPackage *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}

int *
delete_1_svc(struct SpecificId *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}

int *
update_1_svc(struct UserPackage *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}

int *
read_1_svc(struct SpecificId *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}

char **
getstat_1_svc(struct SpecificId *argp, struct svc_req *rqstp)
{
	static char * result;

	/*
	 * insert server code here
	 */

	return &result;
}

char **
getstatall_1_svc(char **argp, struct svc_req *rqstp)
{
	static char * result;

	/*
	 * insert server code here
	 */

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

	/*
	 * insert server code here
	 */

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
