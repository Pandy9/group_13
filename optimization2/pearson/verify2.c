/*
Author: Enqrique de la Cal <ende24@student.bth.se>
Modified: allow single-file mode (compare file to itself if only one path given)
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
    const char *file1 = NULL;
    const char *file2 = NULL;
    unsigned long line = 1;

    /* Usage: verify <file1> [file2] [stop]
       If file2 is omitted, compare file1 to itself (single-file mode). */
    if (argc < 2) {
#ifndef QUIET
        fprintf(stderr, "ERROR:\tWrong usage.\n");
        fprintf(stderr, "Usage:\t%s <file1> [file2] [stop]\n", argv[0]);
        fprintf(stderr,
                "\tCompares 64-bit floating point numbers line-by-line.\n"
                "\tIf only <file1> is given, the program compares it to itself (single-file mode).\n"
                "\tIf numbers differ by >= 1e-15 but < 1e-11 -> return 1 (warning).\n"
                "\tIf numbers differ by >= 1e-11 -> return 2 (error).\n"
                "\t[stop]=1 or 2 to stop at first occurrence.\n");
#endif
        return -1;
    }

    file1 = argv[1];
    if (argc >= 3 && argv[2] && argv[2][0] != '\0') {
        file2 = argv[2];
    } else {
        /* Single-file mode: compare file to itself */
        file2 = file1;
#ifndef QUIET
#ifdef DEBUG
        fprintf(stderr, "INFO:\tSingle-file mode: comparing '%s' to itself.\n", file1);
#endif
#endif
    }

    if (argc >= 4) {
        stop = atoi(argv[3]);
    }

    f1 = fopen(file1, "rb");
    if (!f1) {
#ifndef QUIET
        fprintf(stderr, "ERROR:\tCannot open file '%s'.\n", file1);
#endif
        ret = -1;
        goto end;
    }
    f2 = fopen(file2, "rb");
    if (!f2) {
#ifndef QUIET
        fprintf(stderr, "ERROR:\tCannot open file '%s'.\n", file2);
#endif
        ret = -1;
        goto close_end;
    }

    while (!feof(f1) && !feof(f2)) {
        double d1, d2;

        if (fscanf(f1, "%lg\n", &d1) != 1) {
#ifndef QUIET
            fprintf(stderr, "ERROR:\tCannot read number from file '%s' at line %lu.\n", file1, line);
#endif
            ret = 2;
            goto close_end;
        }
        if (fscanf(f2, "%lg\n", &d2) != 1) {
#ifndef QUIET
            fprintf(stderr, "ERROR:\tCannot read number from file '%s' at line %lu.\n", file2, line);
#endif
            ret = 2;
            goto close_end;
        }

        double error = fabs(d1 - d2);
        if (error >= ERROR_15) {
            if (error < ERROR_11) {
#ifndef QUIET
                fprintf(stderr, "WARNING:\tNumbers at line %lu differ by more than 15 decimal places.\n", line);
#endif
                ret = 1;
            } else {
#ifndef QUIET
                fprintf(stderr, "ERROR:\tNumbers at line %lu differ by more than 11 decimal places.\n", line);
#endif
                ret = 2;
            }
            if (stop > 0 && stop == ret) {
                goto close_end;
            }
        }
        line++;
    }

    if ((feof(f1) && !feof(f2)) || (!feof(f1) && feof(f2))) {
#ifndef QUIET
        fprintf(stderr, "ERROR:\tDifferent number of lines in files '%s' and '%s'.\n", file1, file2);
#endif
        ret = 2;
    }

close_end:
    if (f1) fclose(f1);
    if (f2) fclose(f2);

end:
#ifndef QUIET
#ifdef DEBUG
    printf("%lu lines read\n", line);
    printf("Return value: %d\n", ret);
#endif
#endif
    return ret;
}
