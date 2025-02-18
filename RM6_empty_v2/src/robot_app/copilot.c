#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "copilot.h"
#include "pilot.h"
#include "robot.h"
#include "configtouche.h"

#define NUMBER_STEP 6
#define SPEED_DEFAULT 50
#define DISTANCE_DEFAULT 1
#define STEP_INIT 0

static path_status_t path_status;
static bool quit = false;
static bool backwards = false;

/** @brief Max scanning's attempt to check if a move is finished. */
#define ENCODERS_SCAN_NB 1000
/** @brief Waiting time between two encoder's scans (in microseconds). */
#define DELAY 1000

void copilot_check_path(char touche) {
    switch (touche) {
        case 'z': // Avancer
            pilot_start_move((move_t){FORWARD, {DISTANCE_DEFAULT}, SPEED_DEFAULT});
            break;
        case 'q': // Tourner à gauche
            pilot_start_move((move_t){ROTATION, {LEFT}, SPEED_DEFAULT});
            break;
        case 'd': // Tourner à droite
            pilot_start_move((move_t){ROTATION, {RIGHT}, SPEED_DEFAULT});
            break;
        case 's': // Reculer
            // pilot_start_move((move_t){BACKWARD, {DISTANCE_DEFAULT}, SPEED_DEFAULT});
            backwards = true;
            pilot_start_move((move_t){BACKWARD, {-DISTANCE_DEFAULT}, (SPEED_DEFAULT)});
            break;
        case 'x': // Quitter
            quit = true;
            return; // Sortie immédiate
        default:
            printf("Touche non reconnue. Utilisez Z, Q, S, D, X.\n");
            return;
    }
    copilot_stop_at_step_completion(); // Vérifie si le mouvement est terminé
    backwards = false;
}

void copilot_start_path() {
    printf("Contrôle du robot :\n");
    printf("z = Avancer | q = Gauche | s = Reculer | d = Droite | x = Quitter\n");

    setRawMode();  // Active le mode brut

    char key;
    while (!quit) {
        key = getchar();  // Lire une touche
        
        if (key != EOF && path_status == PATH_DONE) {  // Si une touche a été pressée
            copilot_check_path(key);  // Exécute l'action correspondante
        }
    }

    restoreMode(); // Rétablit le mode par défaut
    printf("Fin du contrôle.\n");
}

path_status_t copilot_stop_at_step_completion() {
    for (int i = 0; i < ENCODERS_SCAN_NB; i++) {
        usleep(DELAY);
        if (pilot_stop_at_target(backwards) == MOVE_DONE) {
            path_status = PATH_DONE;
            break;
        } else {
            path_status = MOVING;
        }
    }
    return path_status;
}
