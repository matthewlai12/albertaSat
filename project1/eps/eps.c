/*

1 Goals:

  Make use of several multithreading pthread calls. Introduce multitasking, synchronistion, and resource protection and sharing. Also some vague notion of how
  some components of an embedded power system might actually be programmed.

  The actual satellite is using an operating system called FreeRTOS. Doing a quick (and suggested) google search, you will find that FreeRTOS is a popular
  option for projects such as ours. It is a powerful, lightweight Real Time Operating System made for microcontrollers such as our Cortex-R5F microcontroller.

  FreeRTOS provides a very similar API for threading, tasks, and synchronization as POSIX. Hence for this excericise we will use the POSIX pthreads API to
  get some points across.

  It is suggested to begin by reading about multithreading, thread safety and learn the proper usage of these powerful operating system features, both in POSIX, and in
  FreeRTOS (see the resource section for suggested starting points).

2 Requirements:

  You must create a multi-threaded program using the POSIX pthread API. You will be simulating some functionality of the power system on a spacecraft. Your program must update the battery's voltage, current and temperature
  readings constantly. We must also check these values to see that the values are within an expected range, and finally check to see if the power system as a whole is doing okay.

  You should have at least three thread's in your program, all sharing a single resource that is the battery (in the form of the battery vector array).

    First thread (every 30us):
      This thread will be updating the operational values of the EPS (electronic power system) board (EPS_CURRENT_VAL, EPS_VOLTAGE_VAL, EPS_TEMPERATURE_VAL) using
      the given functions that will return values for the board(see next section).

    second thread (every 3 seconds):
      The job of this thread is to check the operational values of the EPS board againt the given expected values, and assign values to the state of each
      value (EPS_CURRENT_STATE, EPS_VOLTAGE_STATE, EPS_TEMPERATURE_STATE). This will be either NOMINAL, WARNING, or DANGER.
      A value has the state NOMINAL if the reading is the SAFE value +/- warning_threshold, and has the state WARNING if the reading is the SAFE value +/- danger_threshold,
      and the state is DANGER if the value is outside SAFE +/- danger_threshold.
    
      This thread should also print the values of each reading.

    Third thread (also every 3 seconds):
      This thread will be responsible for checking for the state of each reading on the EPS board, reporting bad states, and responding to them.
      This thread will handle non-normal states thus:
        a> If all values report nominal state, or strictly fewer than 2 values were in critical, or warning state, do nothing
        b> If two or more states are in warning, and none in critical, report the bad statesto the terminal
        c> If two or more states are non-nominal, and at least one of them is in a critical state, report the bad states and increment
          EPS_ALERT value in the EPS battery vector to keep track of the number of such states.
        d> If EPS_ALERT reaches 5, as in condition (c) was met 5 times, report the bad states, reset the EPS_ALERT counter, and cancel all
          threads execution for 10 seconds before beginning them again. (note: you can assume that the counter is initialized to 0 since it is part of the array that is declared globally)
  
  Your implementation MUST make use of thread synchronization features (such as a mutex, or semaphore) to ensure that the shared space in memory (battery vector) is not accessed by two threads at once!

3 Given code:

  You are given some useful inclusions, a list of values and tolerances for each reading (all values share the same tolerances), and an enum with each values that you will need to keep track of the values in your battery vector.
  (use the enum to index into the vector).
  You are also given three functions that are used to generate dummy values for the 'spacecraft EPS board'.
  Feel free to add or subtract from the given code as you see fit.

  These funcitons are found in the sc/ folder:
    uint16_t sc_get_voltage(void);
    uint16_t sc_get_current(void);
    uint16_t sc_get_temp(void);

Resources:

  Thread safety:
  https://en.wikipedia.org/wiki/Thread_safety

  Full documentation of POSIX pthreads:
  http://man7.org/linux/man-pages/man7/pthreads.7.html

  Learn about threads, some code examples:
  https://computing.llnl.gov/tutorials/pthreads/

  FreeRTOS quick start guide: ask for the password on slack or email arrooney@ualberta.ca if you are interested (might come in handy for this, and in the future)
  https://bytebucket.org/AlbertaSat/albertasat-athena/wiki/docs/Using%20the%20FreeRTOS%20Real%20Time%20Kernel%20-%20A%20Practical%20Guide.pdf?token=ebd884b36a62f2bd406f1a99294962e7aef84ca7&rev=682e10c0c84209983541cfab5c221e14a29a5939


Submission:
  Please submit your solution with a working makefile in the root directory of the project (i.e. not in eps/ or sc/)

*/

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>   
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <inttypes.h>
#include "../sc/sc.h"
#include "eps.h"

/* VALUES AND TOLERANCES */
#define VOLTAGE_SAFE          15
#define CURRENT_SAFE          45
#define TEMPERATURE_SAFE      355
#define WARNING_THRESHOLD     2
#define DANGER_THRESHOLD      4

/* ERROR CODES */
#define NOMINAL               1
#define WARNING               2
#define DANGER                3


/* BATTERY VECTOR */
uint16_t EPS_BATT[EPS_SIZE];

// YOUR CODE HERE!

int main() {
  // initialize your threads and start the program
  pthread_t thread1;
  pthread_t thread2;
  pthread_t thread3;

  int id1 = 1;
  int id2 = 2;
  int id3 = 3;

  pthread_create(&thread1, NULL, getValues, &id1);
  
  return 0;
}


