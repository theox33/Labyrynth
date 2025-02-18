#include "configtouche.h"
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

void setRawMode() {
    struct termios term; // contient les informations de configuration pour un terminal (comportement du terminal, paramètres de lecture, etc.).
    // STDIN : l'entrée standard = "le clavier"
    // Récupère les paramètres actuels du terminal
    // sorte de "get" il lit l'état actuel du terminal et de le stocker dans la structure term
    tcgetattr(STDIN_FILENO, &term);
    
    // Désactive le mode canonique et l'écho (& : désactives uniquement les bits correspondant à ICANON et ECHO, sans toucher aux autres paramètres déjà configurés dans term.c_lflag)
    term.c_lflag &= ~(ICANON | ECHO);
    
    // Applique les changements immédiatement => TCSANOW
    // sorte de "set", prend les paramètres modifiés dans la structure term et les applique au terminal.
    tcsetattr(STDIN_FILENO, TCSANOW, &term);


    // Rend l'entrée standard non bloquante
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void restoreMode() {
    struct termios term;
    
    // Récupère les paramètres actuels du terminal
    tcgetattr(STDIN_FILENO, &term);
    
    // Réactive le mode canonique et l'écho
    term.c_lflag |= (ICANON | ECHO);
    
    // Applique les changements immédiatement
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    
    // Remet stdin en mode bloquant
    fcntl(STDIN_FILENO, F_SETFL, O_RDONLY);
}
