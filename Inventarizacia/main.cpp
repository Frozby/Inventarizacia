#include "data.h"
#include "Consolehandler.h"

int main() {
	Data datahandler;
	if (!datahandler.state) {
		return 1;
	}
	Consolehandler consolehandler(&datahandler);
	return 0;
}