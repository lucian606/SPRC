/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "suma.h"

int *
get_suma_1_svc(struct nod *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	result = argp->y + argp->x;

	return &result;
}
