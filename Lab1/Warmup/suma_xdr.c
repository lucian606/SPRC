/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "suma.h"

bool_t
xdr_nod (XDR *xdrs, nod *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->x))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->y))
		 return FALSE;
	return TRUE;
}
