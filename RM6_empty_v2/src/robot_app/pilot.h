#include <stdbool.h>
#ifndef PILOT_H
#define PILOT_H

/**
 * @enum move_type_t
 * @brief Définit les types de mouvements possibles.
 */
typedef enum {
    FORWARD,
    BACKWARD,
    ROTATION,
} move_type_t;

/**
 * @enum rotation_direction_t
 * @brief Définit les directions possibles pour une rotation.
 */
typedef enum{
    RIGHT, 
    LEFT, 
    U_TURN,
} rotation_direction_t; 

/**
 * @struct move_t
 * @brief Définit un mouvement.
 */
typedef struct {
    move_type_t type;   // Type de mouvement (FORWARD ou ROTATION)
    union {
        int distance;                   // Distance pour FORWARD
        rotation_direction_t direction; // Direction pour ROTATION
    } params;                           // Paramètres spécifiques au type de mouvement
    int speed;                       // Durée ou distance du mouvement
} move_t;


/**
 * @brief pilote l'action et la mise en arrêt
 *
 */
void pilot_start_move(move_t a_move);

/**
 * @enum move_status_t
 *
 * @brief permet de définir un mouvement
 */
typedef enum
{
    MOVE_DONE, 
	MOVE_FORWARDING, 
    MOVE_TURNING, 
    MOVE_OBSTACLE_FORWARD
} move_status_t;

/**
 * @brief pilote l'action et la mise en arrêt
 *
 */
move_status_t pilot_stop_at_target(bool backwards);

/**
 * @brief pilote l'action et la mise en arrêt
 *
 */
move_status_t pilot_get_status();

#endif /* PILOT_H_ */