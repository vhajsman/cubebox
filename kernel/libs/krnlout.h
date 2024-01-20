#ifndef _KRNLOUT_H
#define _KRNLOUT_H

#include "cubebox.h"
#include "krnlstatus.h"

void printColorized(string formattedMessage);

void print(string str);
void println(string str);

void printStatusMsg_noSource(KRNL_STATUS_T status, string msg, int code);
void printStatusMsg(KRNL_STATUS_T status, string source, string msg, int code);

#endif