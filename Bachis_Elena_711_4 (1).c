#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int pipe_fd1[2]; // Pipe for client to server communication
    int pipe_fd2[2]; // Pipe for server to client communication
    pid_t pid;
    char filename[256];

    // Create the pipes
    if (pipe(pipe_fd1) == -1 || pipe(pipe_fd2) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // Child process (Server)

        // Close unused ends of the pipes
        close(pipe_fd1[1]);
        close(pipe_fd2[0]);

        // Read the filename from the pipe
        ssize_t bytes_read = read(pipe_fd1[0], filename, sizeof(filename));
        if (bytes_read == -1) {
            perror("read");
            exit(1);
        }

        // Null-terminate the filename
        filename[bytes_read] = '\0';

        // Open the file
        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            perror("fopen");
            exit(1);
        }

        int word_count = 0;

        // Count the number of words in the file
        char word[256];
	while(fscanf(file,"%s",word)==1){
                word_count+=1;
}

        // Close the file
        fclose(file);

        // Send the word count to the client through the pipe
        ssize_t bytes_written = write(pipe_fd2[1], &word_count, sizeof(word_count));
        if (bytes_written == -1) {
            perror("write");
            exit(1);
        }

        // Close the write end of the pipe
        close(pipe_fd2[1]);

        // Close the read end of the pipe
        close(pipe_fd1[0]);

        exit(0);
    } else {
        // Parent process (Client)

        // Close unused ends of the pipes
        close(pipe_fd1[0]);
        close(pipe_fd2[1]);

        // Read the filename from the user
        printf("Enter the filename: ");
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0'; // Remove the newline character

        // Send the filename to the server through the pipe
        ssize_t bytes_written = write(pipe_fd1[1], filename, strlen(filename));
        if (bytes_written == -1) {
            perror("write");
            exit(1);
        }

        // Read the word count from the server through the pipe
        int word_count;
        ssize_t bytes_read = read(pipe_fd2[0], &word_count, sizeof(word_count));
        if (bytes_read == -1) {
            perror("read");
            exit(1);
        }

        // Display the word count
        printf("The number of words in the file is: %d\n", word_count);

        // Close the read

  	close(pipe_fd2[0]);
}
}
