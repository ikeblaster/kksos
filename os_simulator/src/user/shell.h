#pragma once
#include "..\common\api.h"
#include "rtl.h"
#include "parser.h"
#include "sort.h"
#include "wc.h"
#include "vmstdio.h"

extern "C" size_t __stdcall shell(const CONTEXT &regs);


//nasledujici funkce si dejte do vlastnich souboru
extern "C" size_t __stdcall mk(const CONTEXT &regs) { return 0; }
extern "C" size_t __stdcall rd(const CONTEXT &regs) { return 0; }
//extern "C" size_t __stdcall echo(const CONTEXT &regs) { return 0; }
extern "C" size_t __stdcall rgen(const CONTEXT &regs) { return 0; }
extern "C" size_t __stdcall freq(const CONTEXT &regs) { return 0; }
extern "C" size_t __stdcall ps(const CONTEXT &regs) { return 0; }
