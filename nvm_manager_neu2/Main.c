#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <conio.h>
#include <Windows.h>

#include "nvm_add.h"
#include "typedef.h"
#include "Init.h"
#include "nvm_del.h"
#include "nvm_syncrw.h"
#include "nvm_asyncrw.h"
#include "nvm_reorganize.h"
#include "nvm_demoapplikation.h"
#include "testfunktion.h"



int main() {

	testfunktion();

	int ret = NVM_demoapplikation();
	return ret;

}

