#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "copilot.h"
#include "pilot.h"
#include "robot.h"
#include "configtouche.h"

#define SPEED_DEFAULT 50
#define DISTANCE_DEFAULT 1
#define ENCODERS_SCAN_NB 1000
#define DELAY 1000

typedef enum {
    S_IDLE = 0,
    S_MOVING,
    S_QUIT,
    NB_STATE
} state_t;

typedef enum {
    E_STOP = 0,
    E_FORWARD,
    E_BACKWARD,
    E_LEFT,
    E_RIGHT,
    E_QUIT,
    NB_EVENT
} event_t;

typedef enum {
    A_NOP = 0,
    A_MOVE_FORWARD,
    A_MOVE_BACKWARD,
    A_TURN_LEFT,
    A_TURN_RIGHT,
    A_STOP

} action_t;

typedef struct {
    state_t destination_state;
    action_t transition_action;
} transition_t;

static transition_t transition_table[NB_STATE][NB_EVENT] = {
    [S_IDLE][E_FORWARD]  = {S_MOVING, A_MOVE_FORWARD},
    [S_IDLE][E_BACKWARD] = {S_MOVING, A_MOVE_BACKWARD},
    [S_IDLE][E_LEFT]     = {S_MOVING, A_TURN_LEFT},
    [S_IDLE][E_RIGHT]    = {S_MOVING, A_TURN_RIGHT},

    [S_MOVING][E_FORWARD] = {S_MOVING, A_MOVE_FORWARD},
    [S_MOVING][E_BACKWARD] = {S_MOVING, A_MOVE_BACKWARD},
    [S_MOVING][E_LEFT]    = {S_MOVING, A_TURN_LEFT},
    [S_MOVING][E_RIGHT]   = {S_MOVING, A_TURN_RIGHT},

    [S_IDLE][E_STOP]     = {S_IDLE, A_NOP},
    [S_IDLE][E_QUIT]     = {S_QUIT, A_STOP},
    
    [S_MOVING][E_STOP]   = {S_IDLE, A_STOP},
    [S_MOVING][E_QUIT]   = {S_QUIT, A_STOP},
    
    [S_QUIT][E_QUIT]     = {S_QUIT, A_NOP}
};

static state_t state = S_IDLE;
static bool backwards = false;
// static bool quit = false;

static void execute_action(action_t action) {
    switch (action) {
        case A_MOVE_FORWARD:
            pilot_start_move((move_t){FORWARD, {DISTANCE_DEFAULT}, SPEED_DEFAULT});
            fprintf(stderr, "Etat : %d\n", action);
            break;
        case A_MOVE_BACKWARD:
            backwards = true;
            pilot_start_move((move_t){BACKWARD, {-DISTANCE_DEFAULT}, SPEED_DEFAULT});
            fprintf(stderr, "Etat : %d\n", action);
            break;
        case A_TURN_LEFT:
            pilot_start_move((move_t){ROTATION, {LEFT}, SPEED_DEFAULT});
            fprintf(stderr, "Etat : %d\n", action);
            break;
        case A_TURN_RIGHT:
            pilot_start_move((move_t){ROTATION, {RIGHT}, SPEED_DEFAULT});
            fprintf(stderr, "Etat : %d\n", action);
            break;
        case A_STOP:
            // quit = true;
            fprintf(stderr, "Etat : %d\n", action);
            break;
        case A_NOP:
        default:
            break;
    }
}

static void run(event_t event) {
    transition_t transition = transition_table[state][event];
    execute_action(transition.transition_action);
    state = transition.destination_state;
}

void copilot_check_path(char touche) {
    switch (touche) {
        case 'z': run(E_FORWARD); break;
        case 'q': run(E_LEFT); break;
        case 'd': run(E_RIGHT); break;
        case 's': run(E_BACKWARD); break;
        case 'x': run(E_QUIT); return;
        default:
            printf("Touche non reconnue. Utilisez Z, Q, S, D, X.\n");
            return;
    }
    copilot_stop_at_step_completion();
}


path_status_t copilot_stop_at_step_completion() {
    for (int i = 0; i < ENCODERS_SCAN_NB; i++) {
        usleep(DELAY);
        if (pilot_stop_at_target(backwards) == MOVE_DONE) {
            return PATH_DONE;
        }
    }
    return MOVING;
}
