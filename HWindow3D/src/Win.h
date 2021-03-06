#pragma once

// target Windows 7 or later
//#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>
/*
 * The following #defines disable a bunch of unused windows stuff. If you 
 * get weird errors when trying to do some windows stuff, try removing some
 * (or all) of these defines (it will increase build time though).
 */
#ifndef FULL_WINTARD
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#endif

// Does not cause errors when trying to use std::min/std::max
#define NOMINMAX

//#define STRICT

// The magical Windows API header
#include <Windows.h>

// Make the Class/Struct Object Non-Copyable/Assignable
#define NONCOPYABLE(type_identifier)                                \
    type_identifier(const type_identifier&) = delete;               \
    type_identifier& operator=(const type_identifier&) = delete;
