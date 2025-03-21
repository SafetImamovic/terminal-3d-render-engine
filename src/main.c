#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <fcntl.h>
#endif

#include "../include/globals.h"

int main()
{
        printf("\nEngine Started.\n\n");

        while (1)
        {
        }

        return 0;
}
