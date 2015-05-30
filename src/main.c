#include <stdlib.h>
#include <stdio.h>

#include "api.h"
#include "spider.h"


int main (int argc, char **argv){

	initApi();
	initSpider();
	//printAllPositions();
	startSpider();
	return EXIT_SUCCESS;
}
