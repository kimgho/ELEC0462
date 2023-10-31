/*
spwd.c : a simplified version of pwd

        start in current directory adn recursively
        climbs up to root of filesystem, prints top part
        then prints current part
        uses readdir() to get info about each thing
        bug: prints an empty string if run from "/"
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
ino_t get_inode(char *);
void printpathto(ino_t);
struct passwd *pw;
void inum_to_name(ino_t, char *, int);
int main()
{
    pw = getpwuid(getuid());
    printpathto(get_inode(".")); /* print path to here */
    putchar('\n');               /* then add newline */
    return 0;
}
/*
prints path leading down to an object with this inode
kind of recursive
*/
void printpathto(ino_t this_inode)
{
    ino_t my_inode;
    char its_name[BUFSIZ];
    if (get_inode(pw->pw_dir) == this_inode)
    {
        putchar('~');
        return;
    }

    if (get_inode("..") != this_inode)
    {
        chdir("..");                                /* up one dir */
        inum_to_name(this_inode, its_name, BUFSIZ); /* get this name*/
        my_inode = get_inode(".");                  /* print head*/
        printpathto(my_inode);                      /*recursively*/
        printf("/%s", its_name);                    /*name of this */
    }
}
/*
looks through current directory for a file with this inode
number and copies its name into namebuf
*/
void inum_to_name(ino_t inode_to_find, char *namebuf, int buflen)
{
    DIR *dir_ptr;           /* the directory*/
    struct dirent *direntp; /* each entry */
    dir_ptr = opendir(".");
    if (dir_ptr == NULL)
    {
        perror(".");
        exit(1);
    }
    /*
    search directory for a file with specified inum
    */
    while ((direntp = readdir(dir_ptr)) != NULL)
    {
        if (direntp->d_ino == inode_to_find)
        {
            strncpy(namebuf, direntp->d_name, buflen);
            namebuf[buflen - 1] = '\0'; /*just in case*/
            closedir(dir_ptr);
            return;
        }
    }
    fprintf(stderr, "error looking for inum %ld\n", inode_to_find);
    exit(1);
}
/*
returns inode number of the file
*/
ino_t get_inode(char *fname)
{
    struct stat info;
    if (stat(fname, &info) == -1)
    {
        fprintf(stderr, "Cannot stat");
        perror(fname);
        exit(1);
    }
    return info.st_ino;
}