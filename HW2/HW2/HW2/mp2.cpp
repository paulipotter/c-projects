// HW2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


int main()
{
	int c;
	char *input = argv[1];
	bool progress[4]

	int npolys = NULL;
	int nvertices[] = NULL;
	int vertexindex[] = {0}; //has length equal to the sum of all the elements of nvertices[]30
	int vertices[] = {0}; //length == max index number of vertexindex[]

	FILE *input_file;

	input_file = fopen(input, "r");

	if (input_file == 0)
	{
		//fopen returns 0, the NULL pointer, on failure
		perror("Canot open input file\n");
		exit(-1);
	}
	else
	{
		while ((c = fgetc(input_file)) != EOF)
		{
		//	//if it's an alpha, convert it to lower case
		//	if (isalpha(c))
		//	{
		//		c = tolower(c);
		//		putchar(c);
		//	}
		//	else if (isspace(c))
		//	{
		//		;   //do nothing
		//	}
		//	else
		//	{
		//		c = '\n';
		//		putchar(c);
		//	}
		}
	}

	fclose(input_file);

    return 0;
}

