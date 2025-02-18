#include <stdbool.h>
#ifndef COPILOT_H
#define COPILOT_H

/**
 * @brief pilote l'action et la création du chemin
 *
 */
void copilot_start_path();


/**
 * @enum path_status_t
 *
 * @brief 
 */
typedef enum
{
    PATH_DONE,
    MOVING,
} path_status_t;

/**
 * @brief 
 *
 */
path_status_t copilot_stop_at_step_completion();

void copilot_check_path(char touche);

#endif /* COPILOT_H_ */