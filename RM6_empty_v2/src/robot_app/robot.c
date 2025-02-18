// robot.c

#include "robot.h"
#include "mrpiz.h"
#include <errno.h>
#include <stdio.h>

int robot_start(void) {
  int result = 0;

  // @todo
  result = mrpiz_init();
  //if (result != 0) {
  //  fprintf(stderr, "Erreur d'initialisation du robot: %d\n", result);
  // return -1;
  //}
  return result;
}

void robot_set_speed(speed_pct_t left, speed_pct_t right) {
  mrpiz_motor_set(MRPIZ_MOTOR_LEFT, left);
  mrpiz_motor_set(MRPIZ_MOTOR_RIGHT, right);
}


int robot_get_wheel_position(wheel_t wheel_id) {
  return mrpiz_motor_encoder_get(wheel_id);
}

void robot_reset_wheel_pos(void) {
  // @todo
  mrpiz_motor_encoder_reset(MRPIZ_MOTOR_LEFT);
  mrpiz_motor_encoder_reset(MRPIZ_MOTOR_RIGHT);
}

robot_status_t robot_get_status(void) {
  // @todos
  robot_status_t status;

  // Exemple de remplissage des champs
  status.left_sensor = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_LEFT);
  status.right_sensor = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_RIGHT);
  status.center_sensor = mrpiz_proxy_sensor_get(MRPIZ_PROXY_SENSOR_FRONT_CENTER);
  status.left_encoder = mrpiz_motor_encoder_get(MRPIZ_MOTOR_LEFT);
  status.right_encoder = mrpiz_motor_encoder_get(MRPIZ_MOTOR_RIGHT);

  return status;
}

void robot_signal_event(notification_t event) {
  // @todo
  mrpiz_led_rgb_set(event);
}

void robot_close(void) {
  robot_set_speed(0, 0);
  mrpiz_close();
}
