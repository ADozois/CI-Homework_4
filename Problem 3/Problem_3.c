

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

void fuzzyfication(Input *input, Fuzzy *fuzzy);

void computeOutput(Fuzzy fuzzy, double *output, Input *data);

int main(void) {
  char *path = "/home/gemini/TUM/CI/CI-Homework_4/Problem 2/testInput42A.txt";
  Data data;
  char buff[100];
  int i = 0;
  Fuzzy fuzzy;
  double out[2] = {0};

  if (DEBUG == 0) {
    while (scanf("%s", buff) == 1) {
      parseLine(buff, &(data.input[i]));
      i++;
    }
    data.size = i;
  } else {
    parsingFile(path, &data);
  }

  for (int j = 0; j < data.size; ++j) {
    fuzzyfication(&data.input[j],&fuzzy);
    computeOutput(fuzzy,out,&data.input[j]);
    printf("%1.6f,%1.6f\n",out[0],out[1]);
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
  char *token[3], *ptr;
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
  data->X_c = strtod(token[1], NULL);
  data->Y_c = strtod(token[2], NULL);
  data->Theta_c = strtod(token[3], NULL);

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

void fuzzyfication(Input *input, Fuzzy *fuzzy){
  fuzzy->Ut_small = getU_TSmall(getAngle(*input));
  fuzzy->Ut_large = getU_TLarge(getAngle(*input));
  fuzzy->Ud_small = getU_DSmall(getDistance(*input));
  fuzzy->Ud_large = getU_DLarge(getDistance(*input));
}

void computeOutput(Fuzzy fuzzy, double *output, Input *data) {
  double kd[4] = {0.25,0.25,0.50,0.50};
  double kt[4] = {0.12,0.25,0.12,0.25};
  double U[4][2] = {{0.0}};
  double W[4] = {0};
  double num[2] = {0}, deno = 0;

  for (int i = 0; i < 4; ++i) {
    U[i][0] = getDistance(*data)*kd[i]+getAngle(*data)*kt[i];
    U[i][1] = getDistance(*data)*kd[i]-getAngle(*data)*kt[i];
  }

  W[0] = fmin(fuzzy.Ud_small, fuzzy.Ut_small);
  W[1] = fmin(fuzzy.Ud_small, fuzzy.Ut_large);
  W[2] = fmin(fuzzy.Ud_large, fuzzy.Ut_small);
  W[3] = fmin(fuzzy.Ud_large, fuzzy.Ut_large);

  for (int j = 0; j < 4; ++j) {
    num[0] += W[j]*U[j][0];
    num[1] += W[j]*U[j][1];
    deno += W[j];
  }

  output[0] = num[0]/deno;
  output[1] = num[1]/deno;
}