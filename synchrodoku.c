// vim: noet:ts=4:sw=4
#include <syslog.h>
#include "log.h"
#include "manager.h"

int main(int argc, char *argv[]) {
	manager_start((manager_options_t){
		.thread_count = 4
	});
	manager_stop();

	log("exiting");
}
