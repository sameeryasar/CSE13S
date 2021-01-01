#include <getopt.h>
#include <math.h>
#include <stdio.h>
//
// Declaring/defining all math functions and getopt() arguments
// to run them, including Sin, Cos, Tan, Exp, power.
//
#define OPTIONS "sctea"

double power(double, unsigned short int);
double Sin(double x);
double Cos(double x);
double Tan(double x);
long double Exp(double x);

int main(int argc, char **argv) {
  int option_val = 0;
  //
  // Reads for arguments on command line after running math.c, checking
  // for s, c, t, e, or a to run their respective test.
  //
  while ((option_val = getopt(argc, argv, OPTIONS)) != -1) {
    //
    // Prints test case on Sin function in range [-2pi, 2pi), comparing it to
    // math.h's sin and lists difference between the two.
    //
    if (option_val == 's') {
      printf("x\t Sin\t\t Library \t Difference\n");
      printf("-\t ---\t\t ------- \t ----------\n");
      for (double x = (-2 * M_PI); x < (2 * M_PI); x += (M_PI / 16)) {
        printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Sin(x), sin(x),
            Sin(x) - sin(x));
      }
      break;
      //
      // Prints test case on Cos function in range [-2pi, 2pi), comparing it to
      // the math.h's cos and lists difference between the two.
      //
    } else if (option_val == 'c') {
      printf("x\t Cos\t\t Library \t Difference\n");
      printf("-\t ---\t\t ------- \t ----------\n");
      for (double x = (-2 * M_PI); x < (2 * M_PI); x += (M_PI / 16)) {
        printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Cos(x), cos(x),
            Cos(x) - cos(x));
      }
      break;
      //
      // Prints test case on Tan function in range [-pi/3, pi/3), comparing it to
      // the math.h's tan and lists difference between the two.
      //
    } else if (option_val == 't') {
      printf("x\t Tan\t\t Library \t Difference\n");
      printf("-\t ---\t\t ------- \t ----------\n");
      for (double x = (-M_PI / 3); x < (M_PI / 3); x += (M_PI / 16)) {
        printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Tan(x), tan(x),
            Tan(x) - tan(x));
      }
      break;
      //
      // Prints test case on Exp function in range [0, 9], comparing it to
      // the math.h's exp and lists difference between the two.
      //
    } else if (option_val == 'e') {
      printf("x\t Exp\t\t Library \t Difference\n");
      printf("-\t ---\t\t ------- \t ----------\n");
      for (double x = 0; x < 9; x += .1) {
        printf("% 6.4lf\t% 10.8Lf\t% 10.8lf\t% 12.10Lf\n", x, Exp(x), exp(x),
            Exp(x) - exp(x));
      }
      break;
      //
      // Prints all test cases shown above.
      //
    } else if (option_val == 'a') {
      printf("x\t Sin\t\t Library \t Difference\n");
      printf("-\t ---\t\t ------- \t ----------\n");
      for (double x = (-2 * M_PI); x < (2 * M_PI); x += (M_PI / 16)) {
        printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Sin(x), sin(x),
            Sin(x) - sin(x));
      }
      printf("x\t Cos\t\t Library \t Difference\n");
      printf("-\t ---\t\t ------- \t ----------\n");
      for (double x = (-2 * M_PI); x < (2 * M_PI); x += (M_PI / 16)) {
        printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Cos(x), cos(x),
            Cos(x) - cos(x));
      }
      printf("x\t Tan\t\t Library \t Difference\n");
      printf("-\t ---\t\t ------- \t ----------\n");
      for (double x = (-M_PI / 3); x < (M_PI / 3); x += (M_PI / 16)) {
        printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", x, Tan(x), tan(x),
            Tan(x) - tan(x));
      }
      printf("x\t Exp\t\t Library \t Difference\n");
      printf("-\t ---\t\t ------- \t ----------\n");
      for (double x = 0; x < 9; x += .1) {
        printf("% 6.4lf\t% 10.8Lf\t% 10.8lf\t% 12.10Lf\n", x, Exp(x), exp(x),
            Exp(x) - exp(x));
      }
      break;
    }
    //
    // If no arguments found, prints no arguments supplied.
    //
    if (argc == 1) {
      printf("No arguments supplied\n");
    }
  }
  return 0;
}
//
// Power function to raise an input base to an
// input power using a while loop.
//
double power(double base, unsigned short exponent) {
  double product = 1;
  while (exponent > 0) {
    product *= base;
    --exponent;
  }
  return product;
}
//
// Sin approximation: consists of normalizing x to be in
// range [-pi, pi] and then using horner equation with 14 terms
// for accuracy (given from lab manual).
//
double Sin(double x) {

  if (x < -M_PI) {
    x += (2 * M_PI);
  } else if (x > M_PI) {
    x -= (2 * M_PI);
  }

  double x_2 = power(x, 2);

  double approx
      = (x
            * ((((x_2 * (52785432 - (479249 * x_2))) - 1640635920) * x_2)
                  + 11511339840))
        / ((((((18361 * x_2) + 3177720) * x_2) + 277920720) * x_2)
              + 11511339840);
  return approx;
}
//
// Cos approximation: consists of normalizing x to be in
// range [-pi, pi] and then using horner equation with 14 terms
// for accuracy (given from lab manual).
//
double Cos(double x) {

  if (x < -M_PI) {
    x += (2 * M_PI);
  } else if (x > M_PI) {
    x -= (2 * M_PI);
  }

  double x_2 = power(x, 2);

  double approx
      = ((((x_2 * (1075032 - (14615 * x_2))) - 18471600) * x_2) + 39251520)
        / ((((((127 * x_2) + 16632) * x_2) + 1154160) * x_2) + 39251520);
  return approx;
}
//
// Tan approximation: consists of normalizing x to be in
// range [-pi/3, pi/3] and a derived horner equation from
// a pade approximant with 14 terms (given from lab manual).
//
double Tan(double x) {

  if (x < (-M_PI / 3)) {
    x += M_PI;
  } else if (x > (M_PI / 3)) {
    x -= M_PI;
  }

  double x_2 = power(x, 2);

  double approx
      = (x
            * (((((((x_2 - 990) * x_2) + 135135) * x_2) - 4729725) * x_2)
                  + 34459425))
        / (45
              * (((((((x_2 - 308) * x_2) + 21021) * x_2) - 360360) * x_2)
                    + 765765));
  return approx;
}
//
// Exp approximation: Summation of e^x by for loop, exiting loop
// when next term added is smaller than epsilon 10e-9 and is
// negligble. Code derived from Prof. Dunne's e^x approximation
// seen in Lecture 9.
//
long double Exp(double x) {
  long double term = 1.0;
  long double sum = term;
  for (long double k = 1.0; term > 10e-9; k += 1.0) {
    term = x / k * term;
    sum += term;
  }
  return sum;
}
