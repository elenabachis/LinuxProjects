#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define FIFO_FILE "myfifo"

int countLines(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    int lineCount = 0;
    char buffer[BUFFER_SIZE];

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        lineCount++;
    }//es zahlt die non nullen strings

    fclose(file);

    return lineCount;
}

int main() {
    if (access(FIFO_FILE, F_OK) == -1) {//pruft ob das File schon existiert
        if (mkfifo(FIFO_FILE, 0666) == -1) {//wenn nein, erstelle ich ein fifo, 0666 erlaubt read und write
            perror("Error creating FIFO file");
            exit(1);
        }
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Error creating child process");
        exit(1);
    }

    if (pid == 0) {
        // Child process (server)
        int fifo_fd = open(FIFO_FILE, O_RDONLY); //es offnet das Fifo mit read only mode
        if (fifo_fd == -1) {
            perror("Error opening FIFO file");
            exit(1);
        }

        char filename[BUFFER_SIZE];
        read(fifo_fd, filename, BUFFER_SIZE);
        filename[strcspn(filename, "\n")] = '\0';//es loscht das newline und ersetzt es mit null
        int lineCount = countLines(filename);

        printf("Number of lines in file '%s': %d\n", filename, lineCount);

        close(fifo_fd);
        unlink(FIFO_FILE);
    } else {
        // Parent process (client)
        int fifo_fd = open(FIFO_FILE, O_WRONLY);
        if (fifo_fd == -1) {
            perror("Error opening FIFO file");
            exit(1);
        }

        char filename[BUFFER_SIZE];
        printf("Enter filename: ");
        fgets(filename, BUFFER_SIZE, stdin);

        write(fifo_fd, filename, strlen(filename) + 1);

        close(fifo_fd);
    }

    return 0;
}

