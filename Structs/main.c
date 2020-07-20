//
// Created by Haim Adrian
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ID_LENGTH 9

typedef struct student_init_data {
    char *id;
    int *grades, gradesCount;
} studentInitData;

typedef struct student_processed_data {
    float gradesAvg;
    char *id;
} studentProcessedData;

typedef struct statis {
    float totalAvg;
    studentProcessedData *groupBelowTotalAvg;
    studentProcessedData *groupAboveOrEqTotalAvg;
    int groupBelowTotalAvgLength, groupAboveOrEqTotalAvgLength;
} statistics;

int *inputData(int gradesCount);
int *inputDataRand(int gradesCount);
float studentAverage(const int *grades, int length);
float totalAverage(const float *studAvgs, int length);
void classification(studentInitData *students, int length, statistics *stats);
void printTab(statistics *stats);
void freeStudInitData(studentInitData *student);
void freeStats(statistics *stats);

void inputRun();
void randomRun();

int main() {
    char choose;

    srand(time(NULL));

    printf("Run random test? (Y/N)\n");
    scanf_s("%c%*c", &choose, sizeof(char));

    if ((choose == 'y') || (choose == 'Y')) {
        randomRun();
    } else {
        inputRun();
    }

    printf("\nPress any key to continue...\n");
    getchar();

    return 0;
}

void inputRun() {
    studentInitData *students;
    int i, studentsCount;
    statistics stats;

    printf("Enter amount of students to scan:\n");
    scanf_s("%d%*c", &studentsCount);

    if (studentsCount > 0) {
        students = malloc(studentsCount * sizeof(studentInitData));
        if (students) {
            for (i = 0; i < studentsCount; i++) {
                printf("Student #%03d\n", i + 1);
                printf("Enter ID:\n");
                students[i].id = calloc(ID_LENGTH + 1, sizeof(char));
                fgets(students[i].id, (ID_LENGTH + 1) * sizeof(char), stdin);
                students[i].id[ID_LENGTH] = '\0';

                printf("Enter amount of grades this student got:\n");
                scanf_s("%*c%d%*c", &(students[i].gradesCount));
                students[i].grades = inputData(students[i].gradesCount);
            }

            classification(students, studentsCount, &stats);

            printTab(&stats);

            freeStats(&stats);

            for (i = 0; i < studentsCount; i++) {
                freeStudInitData(&(students[i]));
            }
        } else {
            printf("Failed to allocate memory for students input.\n");
        }
    }
}

void randomRun() {
    studentInitData students[] = { { "000000000\0" }, { "111111111\0" }, { "222222222\0" }, { "333333333\0" }, { "444444444\0" },
                                   { "555555555\0" }, { "666666666\0" }, { "777777777\0" }, { "888888888\0" }, { "999999999\0" },
                                   { "123456780\0" }, { "123456781\0" }, { "123456782\0" }, { "123456783\0" }, { "123456784\0" },
                                   { "123456785\0" }, { "123456786\0" }, { "123456787\0" }, { "123456788\0" }, { "123456789\0" },
                                   { "100000000\0" }, { "100000001\0" }, { "100000002\0" }, { "100000003\0" }, { "100000004\0" },
                                   { "100000005\0" }, { "100000006\0" }, { "100000007\0" }, { "100000008\0" }, { "100000009\0" },
                                   { "100000010\0" }, { "100000011\0" }, { "100000012\0" }, { "100000013\0" }, { "100000014\0" },
                                   { "100000015\0" }, { "100000016\0" }, { "100000017\0" }, { "100000018\0" }, { "100000019\0" },
                                   { "100000020\0" }, { "100000021\0" }, { "100000022\0" }, { "100000023\0" }, { "100000024\0" },
                                   { "100000025\0" }, { "100000026\0" }, { "100000027\0" }, { "100000028\0" }, { "100000029\0" } };
    int i, gradesCount, *grades, studentsCount = sizeof(students) / sizeof(studentInitData);
    statistics stats;

    for (i = 0; i < studentsCount; i++) {
        gradesCount = 10 + (rand() % 3);
        grades = inputDataRand(gradesCount);

        students[i].gradesCount = gradesCount;
        students[i].grades = grades;
    }

    classification(students, studentsCount, &stats);

    printTab(&stats);

    freeStats(&stats);

    for (i = 0; i < studentsCount; i++) {
        freeStudInitData(&(students[i]));
    }
}

int *inputData(int gradesCount) {
    int *grades = malloc(gradesCount * sizeof(int)), i;

    printf("Enter grades, one by one:\n");
    for (i = 0; i < gradesCount; i++) {
        scanf_s("%d%*c", grades + i);
    }

    return grades;
}

int *inputDataRand(int gradesCount) {
    int *grades = malloc(gradesCount * sizeof(int));

    while (gradesCount-- > 0) {
        grades[gradesCount] = 60 + (rand() % 41);
    }

    return grades;
}

float studentAverage(const int *grades, int length) {
    int i;
    double avg = 0;

    for (i = 0; i < length; i++) {
        avg += (double)grades[i];
    }

    return (float)(avg / (double)length);
}

float totalAverage(const float *studAvgs, int length) {
    int i;
    double avg = 0;

    for (i = 0; i < length; i++) {
        avg += studAvgs[i];
    }

    return (float)(avg / (double)length);
}

void classification(studentInitData *students, int length, statistics *stats) {
    int i, studentsBelowTotalAvg = 0, studentsAboveOrEqTotalAvg = 0;
    studentProcessedData *groupBelowTotalAvg, *groupAboveOrEqTotalAvg;
    float *allAvgs = malloc(length * sizeof(float));

    for (i = 0; i < length; i++) {
        // Add 0.009 cause we display 2 digits after the dot.
        allAvgs[i] = studentAverage(students[i].grades, students[i].gradesCount) + 0.009F;
    }
    stats->totalAvg = totalAverage(allAvgs, length);

    for (i = 0; i < length; i++) {
        studentsBelowTotalAvg += (allAvgs[i] < stats->totalAvg);
    }
    studentsAboveOrEqTotalAvg = length - studentsBelowTotalAvg;

    groupBelowTotalAvg = malloc(studentsBelowTotalAvg * sizeof(studentProcessedData));
    groupAboveOrEqTotalAvg = malloc(studentsAboveOrEqTotalAvg * sizeof(studentProcessedData));

    studentsBelowTotalAvg = studentsAboveOrEqTotalAvg = 0;
    for (i = 0; i < length; i++) {
        if (allAvgs[i] >= stats->totalAvg) {
            groupAboveOrEqTotalAvg[studentsAboveOrEqTotalAvg].id = students[i].id;
            groupAboveOrEqTotalAvg[studentsAboveOrEqTotalAvg].gradesAvg = studentAverage(students[i].grades, students[i].gradesCount);
            studentsAboveOrEqTotalAvg++;
        } else {
            groupBelowTotalAvg[studentsBelowTotalAvg].id = students[i].id;
            groupBelowTotalAvg[studentsBelowTotalAvg].gradesAvg = studentAverage(students[i].grades, students[i].gradesCount);
            studentsBelowTotalAvg++;
        }
    }

    stats->groupBelowTotalAvg = groupBelowTotalAvg;
    stats->groupBelowTotalAvgLength = studentsBelowTotalAvg;
    stats->groupAboveOrEqTotalAvg = groupAboveOrEqTotalAvg;
    stats->groupAboveOrEqTotalAvgLength = studentsAboveOrEqTotalAvg;

    free(allAvgs);
}

void printTab(statistics *stats) {
    int i;
    studentProcessedData *currStud;

    printf("Statistics\n");
    printf("----------\n");
    printf("* Total Average: %.2lf\n", stats->totalAvg);
    printf("* Students with average greater than or equal to total average:\n");
    for (i = 0; i < stats->groupAboveOrEqTotalAvgLength; i++) {
        currStud = &(stats->groupAboveOrEqTotalAvg[i]);
        printf("  %3d. ID: %s, Average: %.2f\n", i + 1, currStud->id, currStud->gradesAvg);
    }
    printf("* Students with average lower than total average:\n");
    for (i = 0; i < stats->groupBelowTotalAvgLength; i++) {
        currStud = &(stats->groupBelowTotalAvg[i]);
        printf("  %3d. ID: %s, Average: %.2f\n", i + stats->groupAboveOrEqTotalAvgLength + 1, currStud->id, currStud->gradesAvg);
    }
}

void freeStudInitData(studentInitData *student) {
    if (student->grades) {
        free(student->grades);
    }

    student->gradesCount = 0;
}

void freeStats(statistics *stats) {
    if (stats) {
        if (stats->groupBelowTotalAvg) {
            /*for (i = 0; i < stats->groupBelowTotalAvgLength; i++) {
                free((stats->groupBelowTotalAvg)[i])
            }*/
            free(stats->groupBelowTotalAvg);
        }

        if (stats->groupAboveOrEqTotalAvg) {
            free(stats->groupAboveOrEqTotalAvg);
        }
    }
}
