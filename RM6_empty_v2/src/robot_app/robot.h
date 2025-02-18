#ifndef ROBOT_H
#define ROBOT_H

/**
 * @file robot.h
 * @brief Declaration of the Robot class.
 */

// @todo : Define the different types needed


/**
 * @brief Initializes and starts the robot.
 *
 * @return The initialization status.
 */
int robot_start(void);

/**
 * @enum speed_pct_t
 *
 * @brief valeur de la vitesse du robot
 */
typedef enum
{
	LEFT_SPEED = 0,	
	RIGHT_SPEED	= 1
} speed_pct_t;

/**
 * @brief Sets the speed of the robot's wheels.
 *
 */
void robot_set_speed(speed_pct_t left, speed_pct_t right);

/**
 * @enum robot wheel
 *
 * @brief 
 */
typedef enum
{
	LEFT_WHEEL 			= 0,	
	RIGHT_WHEEL 		= 1,
    BOTH_WHEEL          = 2		
} wheel_t;

/**
 * @brief Gets the position of a specific wheel.
 * 
 * @param wheel_id
 * @return The position of the specified wheel.
 */
int robot_get_wheel_position(wheel_t wheel_id);

/**
 * @brief Resets the position of the robot's wheels.
 */
void robot_reset_wheel_pos(void);

/**
 * @enum notification
 *
 * @brief 
 */
typedef struct
{
	int left_encoder;
    int right_encoder;
    int left_sensor ;
    int right_sensor;
    int center_sensor;
    int battery;
} robot_status_t;

/**
 * @brief Gets the current status of the robot.
 *
 * @return The status of the robot.
 */
robot_status_t robot_get_status(void);

/**
 * @enum notification
 *
 * @brief 
 */
typedef enum
{
	ROBOT_IDLE      = 0,	/**< extinction de la led : Robot is doing nothing*/
	ROBOT_PROBLEM   = 1,	/**< couleur rouge : Robot has encountered a general problem*/
	ROBOT_OK 	    = 2,	/**< couleur verte : Robot is operating normally*/
	ROBOT_OBSTACLE	= 3		/**< couleur bleue : Robot has encountered an obstacle*/
} notification_t;

/**
 * @brief Signals an event to external users.
 *
 * @param event The event notification.
 */
void robot_signal_event(notification_t event);

/**
 * @brief Stops the robot and close the link to the robot.
 */
void robot_close(void);

#endif // ROBOT_H
