#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <mode>\n", argv[0]);
        return -1;
    }

    char *filename = argv[1];

    // Convert the input string to octal mode using sscanf
    mode_t mode;
    if (sscanf(argv[2], "%o", &mode) != 1) {
	printf("Error: Invalid mode format: %s\n", argv[2]);
        return -1;
    }

    // Set the umask to get the default file creation behavior
    mode_t mask = umask(0);

    // Open the file with O_CREAT | O_EXCL to ensure it is created exclusively
    int fd = open(filename, O_CREAT | O_EXCL, mode);
    if (fd == -1) {
	perror("Error creating file \n");        
	umask(mask);
        return -1;
    }

    close(fd);

    // Restore the original umask
    umask(mask);

    int mychmod = chmod(argv[1], mode);
    if (mychmod == -1) {
        perror("Error changing file permissions");
        return -1;
    }
    

    printf("File '%s' created successfully with mode %o.\n", filename, mode);

    return 0;
}
