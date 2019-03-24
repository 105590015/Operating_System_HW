#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

double avg;
int min;
int max;
int size;

void* calculateAverage(void* data)
{
  int* input = (int*) data;
  int i, sum = 0;
  for (i = 0; i < size; i++)
  {
    sum += input[i];
  }
  avg = (double)sum / size;
}

void* calculateMinimum(void* data)
{
  int* input = (int*) data;
  int i;
  max = input[0];
  for (i = 0; i < size; i++)
  {
    if(input[i] > max)
      max = input[i];
  }
}

void* calculateMaximum(void* data)
{
  int* input = (int*) data;
  int i;
  min = input[0];
  for (i = 0; i < size; i++)
  {
    if(input[i] < min)
      min = input[i];
  }
}

int main(int argc, char *argv[])
{
  int i;
  int data[argc - 1];
  int t1, t2, t3;
  pthread_t thread1, thread2, thread3;
  if(argc <= 1)
  {
    printf("Incorrect input. Please enter one or more integers.\n");
    exit(0);
  }

  for (i = 0; i < (argc - 1); i++)
  {
    data[i] = atoi(argv[i + 1]);
    size++;
  }

  t1 = pthread_create(&thread1, NULL, (void*) calculateAverage, (void*) data);
  if(t1)
  {
    fprintf(stderr, "Error creating thread1(calculateAverage), return code : %d\n", t1);
    exit(EXIT_FAILURE);
  }

  t2 = pthread_create(&thread2, NULL, (void*) calculateMinimum, (void*) data);
  if(t2)
  {
    fprintf(stderr, "Error creating thread2(calculateMinimum), return code: %d\n",t2);
    exit(EXIT_FAILURE);
  }

  t3 = pthread_create(&thread3, NULL, (void*) calculateMaximum, (void*) data);
  if(t3)
  {
    fprintf(stderr, "Error creating thread3(calculateMaximum), return code: %d\n", t3);
    exit(EXIT_FAILURE);
  }

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
  printf("The average : %f\n", avg);
  printf("The minimum : %d\n", min);
  printf("The maximum : %d\n", max);
  exit(EXIT_SUCCESS);
}
