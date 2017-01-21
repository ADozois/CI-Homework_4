

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DEBUG 1


typedef struct Input Input;
typedef struct Fuzzy Fuzzy;
typedef struct Data Data;

struct Input {
  double X_r;
  double Y_r;
  double Theta_r;
  double X_c;
  double Y_c;
  double Theta_c;
};

struct Fuzzy {
  double Ud_small;
  double Ud_large;
  double Ut_small;
  double Ut_large;
};

struct Data {
  Input input[1000];
  int size;
};

void parsingFile(char *path, Data *data);

void parseLine(char *line, Input *data);

double getDistance(Input input);

double getAngle(Input input);

double getU_DSmall(double dist);

double getU_DLarge(double dist);

double getU_TSmall(double theta);

double getU_TLarge(double theta);

void printOutput(Fuzzy *output);

int main(void) {
  char *path = "/home/gemini/TUM/CI/CI-Homework_4/Problem 1/testInput41B.txt";
  Data data;
  char buff[100];
  int i = 0;

  if (DEBUG == 0) {
    while (scanf("%s", buff) == 1) {
      parseLine(buff, &(data.input[i]));
      i++;
    }
    data.size = i;
  } else {
    parsingFile(path, &data);
  }



  return EXIT_SUCCESS;
}

void parsingFile(char *path, Data *data) {
  FILE *file;
  int size = 100, i = 0;
  char buff[size];

  file = fopen(path, "r");

  if (file) {
    while (fgets(buff, size, (FILE *) file) != NULL) {
      parseLine(buff, &(data->input[i]));
      i++;
    }
    data->size = i;
  } else {
    printf("Can't open file");
  }
}

void parseLine(char *line, Input *data) {
  char *token[6], *ptr;
  int i = 0;

  ptr = strtok(line, ",\n");

  while (ptr != NULL) {
    token[i] = ptr;
    ptr = strtok(NULL, ",\n");
    ++i;
  }

  data->X_r = strtod(token[0], NULL);
  data->Y_r = strtod(token[1], NULL);
  data->Theta_r = strtod(token[2], NULL);
  data->X_c = strtod(token[3], NULL);
  data->Y_c = strtod(token[4], NULL);
  data->Theta_c = strtod(token[5], NULL);

}

double getDistance(Input input) {
  return sqrt((pow(input.X_r - input.X_c, 2.0) + pow(input.Y_r - input.Y_c, 2.0)));
}

double getAngle(Input input) {
  return input.Theta_r - input.Theta_c;
}

double getU_DSmall(double dist) {
  return fmin(fmax(0.0, ((dist - 80.0) / -160.0)), 1);
}

double getU_DLarge(double dist) {
  return 1 - getU_DSmall(dist);
}

double getU_TSmall(double theta) {
  return ((-fabs(theta) + 180) / 180);
}

double getU_TLarge(double theta) {
  return 1 - getU_TSmall(theta);
}

void printOutput(Fuzzy *output){
  printf("%1.6f,%1.6f,%1.6f,%1.6f\n", output->Ud_small, output->Ud_large, output->Ut_small, output->Ut_large);
}