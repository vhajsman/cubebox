#ifndef _SHELL_H
#define _SHELL_H

#include "cubebox.h"

typedef struct {
	int type;
	bool optional;
	string name;
	string name_longer;
	string description;
	string hint;

	uint32_t _default;
} shell_pardef;

typedef struct {
	int type;
	bool optional;
	string name;
	string name_longer;
	string description;
	string hint;
} shell_switchdef;

typedef struct {
	string name;
	string description;

	int minParams;

	shell_pardef params[];
	shell_switchdef switches[];

	string help_url;
} shell_command;

void shell_enter();

#endif
