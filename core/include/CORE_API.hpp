// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2026 STupidLord

#pragma once

#if defined(_WIN32)
    #ifdef CORE_EXPORTS
        #define CORE_API __declspec(dllexport)
    #else
        #define CORE_API __declspec(dllimport)
    #endif
#else
    #if __GNUC__ >= 4
        #define CORE_API __attribute__ ((visibility ("default")))
    #else
        #define CORE_API
    #endif
#endif
