/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "rpc_db.h"

bool_t
xdr_SensorData (XDR *xdrs, SensorData *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->dataId))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->noValues))
		 return FALSE;
	 if (!xdr_pointer (xdrs, (char **)&objp->value, sizeof (float), (xdrproc_t) xdr_float))
		 return FALSE;
	return TRUE;
}
