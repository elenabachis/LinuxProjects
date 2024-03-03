#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("NOT ENOUGH\n");
        exit(1);
    }

    int zahl = atoi(argv[1]);
    if (zahl <= 1) {
        printf("INVALID\n");
        exit(1);
    }

    int fd1[2], fd2[2];
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        printf("CANNOT CREATE THE PIPE\n");
        exit(1);
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        printf("CANNOT CREATE PROCESS");
        exit(1);
    } else if (pid1 == 0) { // Der Prozess des erstes Kind berechnet die Faktoren und Gewichten
        close(fd1[0]);
        close(fd2[0]);
        int d = 2, power = 0;
        while (zahl > 1) {
            power = 0;
            while (zahl % d == 0) {
                power++;
                zahl /= d;
            }
            if (power > 0) {
                write(fd1[1], &d, sizeof(d));
                write(fd1[1], &power, sizeof(power));

            }
            d++;
             if (zahl > 1 && d * d > zahl) {
                    d = zahl;
                }

        }
        close(fd1[1]); //das "Writing" ist vorbei
        close(fd2[1]);
        exit(0);
    } else {
        pid_t pid2 = fork();
        if (pid2 == -1) {
            printf("CANNOT CREATE PROCESS");
            exit(1);
        } else if (pid2 == 0) { // der Prozess des zweites Kind liest die Daten aus dem andere n kind und ubermittelt diese
            close(fd1[1]);
            close(fd2[0]);
            int d, power;
            while (read(fd1[0], &d, sizeof(d)) > 0) {
                read(fd1[0], &power, sizeof(power));
                write(fd2[1], &d, sizeof(d));
                write(fd2[1], &power, sizeof(power));
            }
            close(fd1[0]);
            close(fd2[1]);
            exit(0);
        } else { // Der Eltern Prozess zeigt die Ergebnisse auf dem Bildschirm
            close(fd1[0]);
            close(fd1[1]);
            close(fd2[1]);
            int d, power;
            while (read(fd2[0], &d, sizeof(d)) > 0) {
                read(fd2[0], &power, sizeof(power));
                printf("%d ist ein Teiler von %d und der Ordnung der Multiplizitat ist %d\n", d, zahl, power);
            }
            close(fd2[0]);
            waitpid(pid1, NULL, 0);  //Der Prozess wartet auf die Kinder
            waitpid(pid2, NULL, 0);
            exit(0);
        }
    }
}

