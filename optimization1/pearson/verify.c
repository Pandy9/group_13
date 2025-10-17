/*
Author: Enqrique de la Cal <ende24@student.bth.se>
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ERROR_15 (0.000000000000001)
#define ERROR_11 (0.00000000001)

int main(int argc, char *argv[])
{

	int ret = 0;
	int stop = 0;
	FILE *f1 = NULL;
	FILE *f2 = NULL;
	long unsigned int line = 1;

	if (argc < 3)
	{
#ifndef QUIET
		fprintf(stderr, "ERROR:\tWrong usage.\n");
		fprintf(stderr, "Usage:\t%s <file1> <file2> [stop]\n", argv[0]);
		fprintf(stderr,
				"\tThis program compares 64 bit floating point numbers stored in a separate\n"
				"\tline each from <file1> and <file2>. If all the pairs of numbers are the\n"
				"\tsame up to the 15th decimal place, 0 will be returned. If not, this program\n"
				"\twill check if all numbers are the same up to the 11th decimal place, and\n"
				"\tif so 1 will be returned. If [stop] is 1, this program will stop as soon\n"
				"\tas this happens. If any numbers are not the same up to the 11th decimal\n"
				"\tplace, 2 will be returned. If [stop] is 2, this program will stop as soon\n"
				"\tas this happens. If there is an error while executing this program, -1 will\n"
				"\tbe returned, unless the error lies within the files' structures, in which\n"
				"\tcase 2 will be returned, as it means there was a significant difference in\n"
				"\tthe outputs. Define DEBUG in the source code or compile with -DDEBUG (or use\n"
				"\tmake debug) to print debug information. Define QUIET in the source code or\n"
				"\tcompile with -DQUIET (or use make quiet) to prevent the program from printing\n"
				"\tanything.\n");
#endif
		ret = -1;
		goto end;
	}

	if (argc > 3)
	{
		stop = atoi(argv[3]);
	}

	f1 = fopen(argv[1], "rb");
	if (!f1)
	{
#ifndef QUIET
		fprintf(stderr, "ERROR:\tCannot open file '%s'.\n", argv[1]);
#endif
		ret = -1;
		goto end;
	}
	f2 = fopen(argv[2], "rb");
	if (!f2)
	{
#ifndef QUIET
		fprintf(stderr, "ERROR:\tCannot open file '%s'.\n", argv[2]);
#endif
		ret = -1;
		goto end;
	}

	while (!feof(f1) && !feof(f2))
	{
		double d1, d2;

		if (fscanf(f1, "%lg\n", &d1) != 1)
		{
#ifndef QUIET
			fprintf(stderr, "ERROR:\tCannot read number from file '%s' at line %lu.\n", argv[1], line);
#endif
			ret = 2;
			goto close_end;
		}
		if (fscanf(f2, "%lg\n", &d2) != 1)
		{
#ifndef QUIET
			fprintf(stderr, "ERROR:\tCannot read number from file '%s' at line %lu.\n", argv[2], line);
#endif
			ret = 2;
			goto close_end;
		}

		double error = fabs(d1 - d2);
		if (error >= ERROR_15)
		{
			if (error < ERROR_11)
			{
				fprintf(stderr, "WARNING:\tNumbers at line %lu differ by more than 15 decimal places.\n", line);
				ret = 1;
			}
			else
			{
				fprintf(stderr, "ERROR:\tNumbers at line %lu differ by more than 11 decimal places.\n", line);
				ret = 2;
			}
			if (stop > 0 && stop == ret)
			{
				goto close_end;
			}
		}
		line++;
	}

	if ((feof(f1) && !feof(f2)) || (!feof(f1) && feof(f2)))
	{
#ifndef QUIET
		fprintf(stderr, "ERROR:\tDifferent number of lines in files '%s' and '%s'.\n", argv[1], argv[2]);
#endif
		ret = 2;
	}

close_end:
	fclose(f1);
	fclose(f2);

end:
#ifndef QUIET
#ifdef DEBUG
	printf("%lu lines read\n", line);
	printf("Return value: %d\n", ret);
#endif
#endif

	return ret;
}
