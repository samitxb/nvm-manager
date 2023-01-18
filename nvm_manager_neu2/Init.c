#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "typedef.h"


// Initialisiert den NVM-Manager
int NVM_Init(NVMManager* manager) {
	memset(manager, 0, sizeof(NVMManager));

	// Initialisiere die Warteschlange
	manager->queueEintrag = 0;
	manager->queueCount = 0;
	manager->queueLesen = 0;
	manager->queueSchreiben = 0;


	return 0;
}
