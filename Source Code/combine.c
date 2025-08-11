#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

// Student structure
struct alumno {
    char nombre[50];
    int nota;
    int convocatoria;
};

// Function to compare students by grade (ascending order)
int compareStudents(const void *a, const void *b) {
    int notaA = ((struct alumno *)a)->nota;
    int notaB = ((struct alumno *)b)->nota;

    if (notaA < notaB) return -1;  
    if (notaA > notaB) return 1;   
    return 0; 
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <course file 1> <course file 2> <output file>\n", argv[0]);
        return -1;
    }

    char *inputFile1 = argv[1];
    char *inputFile2 = argv[2];
    char *outputFile = argv[3];
    struct alumno students[100];
    int totalStudents = 0;

    // Open first input file
    int file1 = open(inputFile1, O_RDONLY);
    if (file1 == -1) {
        perror("Error opening first input file");
        return -1;
    }

    // Read students from first file
    struct alumno temp;
    while (read(file1, &temp, sizeof(struct alumno))) {
        if (totalStudents >= 100) {
            printf("Error: Maximum student limit exceeded.\n");
            close(file1);
            return -1;
        }
        students[totalStudents++] = temp;
    }
    close(file1);

    // Open second input file
    int file2 = open(inputFile2, O_RDONLY);
    if (file2 == -1) {
        perror("Error opening second input file");
        return -1;
    }

    // Read students from second file
    while (read(file2, &temp, sizeof(struct alumno))) {
        if (totalStudents >= 100) {
            printf("Error: Maximum student limit exceeded.\n");
            close(file2);
            return -1;
        }
        students[totalStudents++] = temp;
    }
    close(file2);

    // Sort students by score (nota) in ascending order
    qsort(students, totalStudents, sizeof(struct alumno), compareStudents);

    // Open output binary file
    int out = open(outputFile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (out == -1) {
        perror("Error opening output file");
        return -1;
    }

    // Write sorted students to output file
    for (int i = 0; i < totalStudents; i++) {
        ssize_t bytesWritten = write(out, &students[i], sizeof(struct alumno));
        if (bytesWritten == -1) {
            perror("Error writing to output file");
            close(out);
            return -1;
        }
        if (bytesWritten != sizeof(struct alumno)) {
            printf("Error: Failed to write the full student record.\n");
            close(out);
            return -1;
        }
    }
    close(out);

    // Create statistics file "estadisticas.csv"
    int csvFile = open("estadisticas.csv", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (csvFile == -1) {
        perror("Error creating statistics file");
        return -1;
    }

    // Count students per grade category
    int Mcount = 0, Scount = 0, Ncount = 0, Acount = 0, Fcount = 0; int excluded=0;
    for (int i = 0; i < totalStudents; i++) {
        if (students[i].nota == 10) Mcount++;
        else if (students[i].nota == 9) Scount++;
        else if (students[i].nota == 8 || students[i].nota == 7) Ncount++;
        else if (students[i].nota == 6 || students[i].nota == 5) Acount++;
        else if(students[i].nota <=4 && students[i].nota >=0) Fcount++;
        else {
            printf("Student, %s, excluded from statistics due to invalid note score \n", students[i].nombre);
            excluded++;
        }
    }
    
    totalStudents = totalStudents - excluded;
    
    // Calculate percentages
    double Mpercent = (double)Mcount / totalStudents * 100;
    double Spercent = (double)Scount / totalStudents * 100;
    double Npercent = (double)Ncount / totalStudents * 100;
    double Apercent = (double)Acount / totalStudents * 100;
    double Fpercent = (double)Fcount / totalStudents * 100;

    char buffer[100];

    // Write data to CSV file
    ssize_t bytesWritten;
    bytesWritten = snprintf(buffer, sizeof(buffer), "M;%d;%.2f%%\n", Mcount, Mpercent);
    if (bytesWritten > 0) {
        ssize_t result = write(csvFile, buffer, bytesWritten);
        if (result == -1) {
            perror("Error writing to CSV file");
            close(csvFile);
            return -1;
        }
    }

    bytesWritten = snprintf(buffer, sizeof(buffer), "S;%d;%.2f%%\n", Scount, Spercent);
    if (bytesWritten > 0) {
        ssize_t result = write(csvFile, buffer, bytesWritten);
        if (result == -1) {
            perror("Error writing to CSV file");
            close(csvFile);
            return -1;
        }
    }

    bytesWritten = snprintf(buffer, sizeof(buffer), "N;%d;%.2f%%\n", Ncount, Npercent);
    if (bytesWritten > 0) {
        ssize_t result = write(csvFile, buffer, bytesWritten);
        if (result == -1) {
            perror("Error writing to CSV file");
            close(csvFile);
            return -1;
        }
    }

    bytesWritten = snprintf(buffer, sizeof(buffer), "A;%d;%.2f%%\n", Acount, Apercent);
    if (bytesWritten > 0) {
        ssize_t result = write(csvFile, buffer, bytesWritten);
        if (result == -1) {
            perror("Error writing to CSV file");
            close(csvFile);
            return -1;
        }
    }

    bytesWritten = snprintf(buffer, sizeof(buffer), "F;%d;%.2f%%\n", Fcount, Fpercent);
    if (bytesWritten > 0) {
        ssize_t result = write(csvFile, buffer, bytesWritten);
        if (result == -1) {
            perror("Error writing to CSV file");
            close(csvFile);
            return -1;
        }
    }

    close(csvFile);  // Close the CSV file

    printf("Successfully merged and sorted students into '%s'.\n", outputFile);
    printf("Statistics saved in 'estadisticas.csv'.\n");

    return 0;
}
