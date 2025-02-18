/**
 * main program with pilot and robot modules
 */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "robot_app/pilot.h"
#include "robot_app/robot.h"
#include "robot_app/copilot.h"
#include "utils.h"

#include "mrpiz.h"

/**
 * @mainpage Robot application mission 2.
 * This projects aims to move a robot along a predefined trajectory.
 */

/**
 * @enum process_state_t
 * @brief indicate the process state management for the application.
 */
typedef enum {
  STOPPED = 0, /**< Process is stopped */
  ALIVE        /**< Process is running */
} process_state_t;

/** @brief Waiting time between two encoder's scans (in microseconds).
 * Adapt according to your cpu and simulator.
 */
#define DELAY 1000

/** @brief Max scanning's attempt to check if a move is finished.
 * Adapt according your move
 */
#define ENCODERS_SCAN_NB 1000

#define STEPS_NUMBER 6 /**< number of steps (or moves) in the path */

// declaration of private functions
static void app_loop(void);

/**
 * @brief Global variable used for program clean exit
 */
static process_state_t running = ALIVE;

/**
 * @brief Function for CTRL+C signal management
 */
static void sigint_handler(int dummy) { running = STOPPED; }

int main(void)
{
  /* start the robot simulator and check its good starting */
  if (robot_start())
  {
    printf("Pb lancement mrPizz\n");
    fflush(stdout);
    return EXIT_FAILURE;
  }
  printf("**** Version démo RM2 **** \n");
  printf("**** par JDL **** \n");
  printf("Ctrl+C pour quitter\n");
  fflush(stdout);

  /* Ctrl+C to stop the program. */
  signal(SIGINT, sigint_handler);
  /* main loop */
  app_loop();
  /* close the robot simulator */
  robot_close();
  return EXIT_SUCCESS;
}

/**
 * @brief Main loop for the application.
 * Send commands to the pilot and display robot's status with a specific period.
 */

static void app_loop()
{
  robot_status_t my_status;

  while (running)
  {
    // Lance le mode manuel
    copilot_start_path();
    running = STOPPED;

    my_status = robot_get_status();
    fprintf(stdout, "codeurs: g = %d, d = %d\n", my_status.left_encoder,
            my_status.right_encoder);
    fprintf(stdout, "proxy: g = %d, c = %d, d = %d\n", my_status.left_sensor,
            my_status.center_sensor, my_status.right_sensor);
    fprintf(stdout, "batterie: %d %%\n", my_status.battery);
  }
}