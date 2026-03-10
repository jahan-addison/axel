/*****************************************************************************
 * Copyright (c) Jahan Addison
 *
 * This software is dual-licensed under the Apache License, Version 2.0 or
 * the GNU General Public License, Version 3.0 or later.
 *
 * You may use this work, in part or in whole, under the terms of either
 * license.
 *
 * See the LICENSE.Apache-v2 and LICENSE.GPL-v3 files in the project root
 * for the full text of these licenses.
 ****************************************************************************/

#pragma once

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

#define WIDE_AND_STRINGIFY(x) L##x
#define WIDE_STRINGIFY(x) WIDE_AND_STRINGIFY(x)

#define AXEL_PUBLIC public
#ifdef AXEL_TEST
#define AXEL_PRIVATE_UNLESS_TESTED public
#define AXEL_PROTECTED_UNLESS_TESTED public

#else
#define AXEL_PRIVATE_UNLESS_TESTED private
#define AXEL_PROTECTED_UNLESS_TESTED protected
#endif