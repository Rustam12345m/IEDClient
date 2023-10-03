#include "logical_device.hpp"

extern "C"
{
	#include "iec61850_server.h"
	#include "hal_thread.h"

	#include <signal.h>
	#include <stdlib.h>
	#include <stdio.h>
}

static int running = 0;

void sigint_handler(int signalId)
{
	running = 0;
}

int main(int argc, char* argv[])
{
	int tcpPort = 102;
	if (argc > 1) {
		tcpPort = atoi(argv[1]);
	}
	signal(SIGINT, sigint_handler);

	/*********************
	 * Setup data model
	 ********************/

	IedModel *model = IedModel_create("IEDTest");

	// Logical Device N1
	TLogicalDevice ld1(model, "LDeviceNumber1");
	ld1.push(new TLLN0(ld1.lib()));
	ld1.push(new TLPHD(ld1.lib(), "LPHD1"));
	ld1.push(new TGGIO(ld1.lib(), "GGIO1", 5, 15));

	// Logical Device N2
	TLogicalDevice ld2(model, "LDeviceNumber2");
	ld2.push(new TLLN0(ld2.lib()));
	ld2.push(new TLPHD(ld2.lib(), "LPHD1"));
	ld2.push(new TGGIO(ld2.lib(), "GGIO1", 3, 1));
	ld2.push(new TGGIO(ld2.lib(), "GGIO2", 99, 99));

	// Logical Device N3
	TLogicalDevice ld3(model, "LDeviceNumber3");
	ld2.push(new TLLN0(ld3.lib()));
	ld2.push(new TLPHD(ld3.lib(), "LPHD1"));
	ld2.push(new TGGIO(ld3.lib(), "GGIO1", 3, 1));
	ld2.push(new TGGIO(ld3.lib(), "GGIO2", 1, 7));
	ld2.push(new TGGIO(ld3.lib(), "GGIO3", 1, 1));


	/*********************
	 * run server
	 ********************/

	IedServer iedServer = IedServer_create(model);

	/* MMS server will be instructed to start listening to client connections. */
	IedServer_start(iedServer, tcpPort);

	if (!IedServer_isRunning(iedServer)) {
		printf("Starting server failed! Exit.\n");
		IedServer_destroy(iedServer);
		exit(-1);
	}

	running = 1;
	while (running) {
		IedServer_lockDataModel(iedServer);
		IedServer_unlockDataModel(iedServer);

		Thread_sleep(300);
	}

	/* stop MMS server - close TCP server socket and all client sockets */
	IedServer_stop(iedServer);

	/* Cleanup - free all resources */
	IedServer_destroy(iedServer);

	/* destroy dynamic data model */
	IedModel_destroy(model);
	return 0;
}