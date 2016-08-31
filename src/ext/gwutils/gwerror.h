#ifndef GW_ERROR_H
#define GW_ERROR_H

/**
 * If you want to include gwerror.h, include gw.h.
 */
#if 1
enum GW_ERROR_CODE
{
    E_GW_OK                     = 0,

	E_GW_FAIL                   = -1,
	E_GW_INVALID_ARGS           = -2,
	E_GW_OUT_OF_RANGE           = -3,
	E_GW_NOT_FOUND              = -4,
	E_GW_ALREADY_EXIST          = -5,
    E_GW_TIMEOUT                = -6,
};
#endif

#endif
