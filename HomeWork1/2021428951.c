#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define PATH_LEN 4096
#define GB 1073741824
#define MB 1048576
#define KB 1024

void convertUnit(long, char *);      // 단위 변환
void printInfo(long, char *);        // 출력
long long sizeCal(char *, int, int); // 파일,디렉터리 크기 계산

int main(int argc, char *argv[])
{
    int isA = 0;
    int isH = 0;

    char *filePath = NULL;
    char *dirPath[argc];
    int dirCount = 0;

    int opt;
    while ((opt = getopt(argc, argv, "ah")) != -1)
    {
        switch (opt)
        {
        case 'a':
            isA = 1;
            break;
        case 'h':
            isH = 1;
            break;
        default:
            fprintf(stderr, "not varified opt");
            exit(1);
        }
    }

    for (int i = optind; i < argc; i++)
    {
        dirPath[dirCount] = argv[i];
        dirCount++;
    }

    if (dirCount)
    {
        for (int i = 0; i < dirCount; i++)
        {
            long total_size = sizeCal(dirPath[i], isA, isH);
            if (isH)
            {
                convertUnit(total_size, dirPath[i]);
            }
            else
            {
                printInfo(total_size, dirPath[i]);
            }
        }
    }
    else
    {
        if (optind < argc)
        {
            filePath = argv[optind];
        }
        else
        {
            filePath = ".";
        }

        long total_size = sizeCal(filePath, isA, isH);

        if (isH)
        {
            convertUnit(total_size, filePath);
        }
        else
        {
            printInfo(total_size, filePath);
        }
    }

    return 0;
}
void convertUnit(long size, char *dir)
{
    char unit[50];
    double fSize;
    if (size >= GB) // 1G보다 큰 경우
    {
        fSize = (double)size / GB; // 1G로 나눔
        if (fSize < 10)
        {
            if (!(size % 1024))
            {
                fSize += 0.05f; // 소수점 2자리에서 반올림
            }
            snprintf(unit, sizeof(unit), "%.1lf", fSize);
        }
        else
        {
            if (!(size % 1024))
            {
                fSize += 0.5f; // 소수점 1자리에서 반올림
            }
            snprintf(unit, sizeof(unit), "%.0lf", fSize);
        }
        strcat(unit, "G"); // G단위 붙이기
    }
    else if (size >= MB) // 1M보다 큰 경우
    {
        fSize = (double)size / MB; // 1M로 나눔
        if (fSize < 10)
        {
            if (!(size % 1024))
            {
                fSize += 0.05f;
            }
            snprintf(unit, sizeof(unit), "%.1lf", fSize);
        }
        else
        {
            if (!(size % 1024))
            {
                fSize += 0.5f;
            }
            snprintf(unit, sizeof(unit), "%.0lf", fSize);
        }
        strcat(unit, "M");
    }
    else if (size >= KB) // 1K보다 큰 경우
    {
        fSize = (double)size / KB;
        if (fSize < 10)
            snprintf(unit, sizeof(unit), "%.1lf", (double)size / 1024);
        else
            snprintf(unit, sizeof(unit), "%.0lf", (double)size / 1024);
        strcat(unit, "K");
    }
    else
    {
        printf("%-7ld\t%s\n", size, dir);
        return;
    }
    printf("%-7s\t%s\n", unit, dir);
}
void printInfo(long size, char *dir)
{
    long fSize;
    if (size >= 1024)
    {
        fSize = size / 1024;
        printf("%-8ld%s", fSize, dir);
    }
    else
    {
        printf("%-8ld%s", size, dir);
    }
    printf("\n");
}
long long sizeCal(char *path, int isA, int isH)
{
    struct stat statbuf, statbuf2;
    struct dirent *direntp;
    long total = 4096;
    DIR *dir_ptr = opendir(path);

    if (stat(path, &statbuf) == -1)
    {
        perror("stat");
        return 0;
    }

    if (!S_ISDIR(statbuf.st_mode))
    {
        return statbuf.st_blocks * 512;
    }

    if (dir_ptr == NULL)
    {
        perror("opendir");
        return 0;
    }

    while ((direntp = readdir(dir_ptr)) != NULL)
    {
        if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
            continue;

        char entry_path[PATH_LEN];

        snprintf(entry_path, sizeof(entry_path), "%s/%s", path, direntp->d_name);
        long entry_size = sizeCal(entry_path, isA, isH);

        stat(entry_path, &statbuf2);

        if (S_ISDIR(statbuf2.st_mode) || isA)
        {
            if (isH)
                convertUnit(entry_size, entry_path);
            else
                printInfo(entry_size, entry_path);
        }

        total += entry_size;
    }

    closedir(dir_ptr);
    return total;
}