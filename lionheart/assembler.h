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

/**
 *  Start of program:
 *  LDS #$00FF ; Initialize stack pointer
 *  ...
 *  ...
 *  End of program:
 *
 *  --- THE HARDWARE VECTORS ---
        ORG $FFF8       ; 4. Jump to the exact end of memory

        FDB $0000       ; IRQ Vector (Not used right now)
        FDB $0000       ; SWI Vector (Not used right now)
        FDB $0000       ; NMI Vector (Not used right now)
        FDB START       ; RESET Vector: Points the CPU to $F000 on boot!
 *
 */