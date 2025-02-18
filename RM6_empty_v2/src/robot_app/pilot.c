#include "pilot.h"
#include "robot.h"
#include <stdio.h>
#include <stdbool.h>

static move_status_t robot_moving;
static int target_pos;
static wheel_t reference_wheel;

#define DISTANCE_FORWARD 1 // adapt according robot simulator
#define DISTANCE_TURN 1

void pilot_start_move(move_t a_move)
{
  int distance = 0;
  int speed_left = 0 ;  
  int speed_right = 0; 
  rotation_direction_t direction;

  if (!robot_moving){


    switch (a_move.type) {
      case FORWARD:
        // Utiliser la distance spécifiée dans les paramètres
        distance = a_move.params.distance * DISTANCE_FORWARD;
        speed_left = a_move.speed;
        speed_right = a_move.speed;
        robot_moving = MOVE_FORWARDING; 
        reference_wheel= LEFT_WHEEL; 
        fprintf(stdout, "FORWARD, %d, %d \n", distance, speed_left);
        break;
      // Utiliser la distance spécifiée dans les paramètres
      case BACKWARD:
        distance = a_move.params.distance * DISTANCE_FORWARD;
        speed_left = -a_move.speed;
        speed_right = -a_move.speed;
        robot_moving = MOVE_FORWARDING; 
        reference_wheel= LEFT_WHEEL; 
        fprintf(stdout, "BACKWARD, %d, %d \n", distance, speed_left);
      case ROTATION:
        direction = a_move.params.direction;
        switch (direction){
          case LEFT :
            speed_left = -a_move.speed;  
            speed_right = a_move.speed;
            robot_moving = MOVE_TURNING; 
            reference_wheel = RIGHT_WHEEL; 
            distance = DISTANCE_TURN; 
            fprintf(stdout, "ROTATION, LEFT, %d \n", speed_right);
            break;
          case RIGHT :
            speed_left = a_move.speed;  
            speed_right = -a_move.speed;
            robot_moving = MOVE_TURNING; 
            reference_wheel = LEFT_WHEEL; 
            distance = DISTANCE_TURN; 
            fprintf(stdout, "ROTATION, RIGHT, %d \n", speed_left);
            break;
          case U_TURN : 
            speed_left = -a_move.speed;  
            speed_right = a_move.speed;
            robot_moving = MOVE_TURNING; 
            reference_wheel = RIGHT_WHEEL; 
            distance = DISTANCE_TURN * 10; 
            fprintf(stdout, "ROTATION, U_TURN, %d \n", speed_right);
            break;
        }
        break; 
    }  

    if (robot_moving){
      target_pos = robot_get_wheel_position(reference_wheel) + distance; 
      robot_set_speed(speed_left, speed_right); 
    }
  }
    
    
}

move_status_t pilot_stop_at_target(bool backwards)
{
  if (robot_get_wheel_position(reference_wheel) >= target_pos && robot_moving && !backwards){
    robot_set_speed(0,0); 
    robot_moving = MOVE_DONE; 
  } else if (robot_get_wheel_position(reference_wheel) <= target_pos && robot_moving && backwards) {
    robot_set_speed(0,0); 
    robot_moving = MOVE_DONE; 
  }
  return robot_moving; 
  
}

move_status_t pilot_get_status(){
  return robot_moving;
}
