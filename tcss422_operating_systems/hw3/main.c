#include <stdlib.h>
#include "classroom_inspector.h"

int main(int argc, char* argv[]) {
	// Uses a mirror for the Spring time schedule (http://www.washington.edu/students/timeschd/T/SPR2015/)
	inspect_classroom("JOY", "114", "http://css.insttech.washington.edu/~mealden/422/mirror/", 2, 2);

	return EXIT_SUCCESS;
}


