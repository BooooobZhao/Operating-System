#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s [info|link|symlink|rm] <args>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "info") == 0) {
        // Perform info action
        if (argc != 3) {
            fprintf(stderr, "Usage: %s info <file>\n", argv[0]);
            return 1;
        }

        struct stat file_stat;
        if (stat(argv[2], &file_stat) == -1) {
            perror("stat");
            return 1;
        }

        printf("inode: %ld\n", file_stat.st_ino);
        printf("size: %ld bytes\n", file_stat.st_size);
        printf("permissions: ");
        printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
        printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
        printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
        printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
        printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
        printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
        printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
        printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
        printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
        printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
        printf("\n");
    }
    else if (strcmp(argv[1], "link") == 0) {
        // Perform link action
        if (argc != 4) {
            fprintf(stderr, "Usage: %s link <src> <dest>\n", argv[0]);
            return 1;
        }

        if (link(argv[2], argv[3]) == -1) {
            perror("link");
            return 1;
        }
    }
    else if (strcmp(argv[1], "symlink") == 0) {
        // Perform symlink action
        if (argc != 4) {
            fprintf(stderr, "Usage: %s symlink <src> <dest>\n", argv[0]);
            return 1;
        }

        if (symlink(argv[2], argv[3]) == -1) {
            perror("symlink");
            return 1;
        }
    }
    else if (strcmp(argv[1], "rm") == 0) {
        // Perform rm action
        if (argc != 3) {
            fprintf(stderr, "Usage: %s rm <file>\n", argv[0]);
            return 1;
        }

        struct stat file_stat;
        if (stat(argv[2], &file_stat) == -1) {
            perror("stat");
            return 1;
        }

        if (S_ISDIR(file_stat.st_mode)) {
            fprintf(stderr, "%s: cannot remove directory '%s'\n", argv[0], argv[2]);
            return 1;
        }

        if (unlink(argv[2]) == -1) {
            perror("unlink");
            return 1;
        }
    }
    else {
        fprintf(stderr, "Unknown action: %s\n", argv[1]);
        return 1;
    }

    return 0;
}
