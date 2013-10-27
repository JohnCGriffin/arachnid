
/*

This file is a compensation for archaic compiler versions.  The first
use of it was with GCC 4.6 which did not supply std::underlying_type.

 */

#ifndef _PORTABILITY_HPP_
#define _PORTABILITY_HPP_

#if __GNUC__

#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_VERSION__ * 100 + __GNUC_PATCHLEVEL__)

#if GCC_VERSION >= 40302
#define HAS_UNDERLYING_TYPE 
#endif

#endif

#endif

