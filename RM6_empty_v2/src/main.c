/**
 * main program with pilot and robot modules using socket for copilot communication
 */

 #include <signal.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <time.h>
 #include <unistd.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 
 #include "robot_app/pilot.h"
 #include "robot_app/robot.h"
 // On ne fait plus appel directement au copilot, la communication se fait via socket
 #include "robot_app/configtouche.h"
 
 #include "utils.h"
 
 #define PORT_DU_SERVEUR 12387
 
 typedef enum {
   STOPPED = 0,
   ALIVE        
 } process_state_t;
 
 #define DELAY 1000
 #define ENCODERS_SCAN_NB 1000
 #define STEPS_NUMBER 6
 
 static void app_loop(void);
 
 static process_state_t running = ALIVE;
 
 static void sigint_handler(int dummy) { running = STOPPED; }
 
 int main(void)
 {
   printf("**** Version démo RM2 **** \n");
   printf("**** par JDL **** \n");
   printf("Ctrl+C pour quitter\n");
   fflush(stdout);
 
   signal(SIGINT, sigint_handler);
   app_loop();
   return EXIT_SUCCESS;
 }
 
 static bool quit = false;
 
 static void app_loop()
 {
   robot_status_t my_status;
   int sockfd;
   struct sockaddr_in server_addr;
   char key;
 
   // Création du socket client
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0)
   {
       perror("socket");
       exit(EXIT_FAILURE);
   }
   
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(PORT_DU_SERVEUR);
   server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   
   // Connexion au serveur copilot
   if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
   {
       perror("connect");
       exit(EXIT_FAILURE);
   }
   printf("Connecté au serveur copilot sur le port %d.\n", PORT_DU_SERVEUR);
 
   // Affichage du menu
   printf("Contrôle du robot :\n");
   printf("z = Avancer | q = Gauche | s = Reculer | d = Droite | x = Quitter\n");
 
   setRawMode();
   while (!quit) {
      usleep(1000);
       key = getchar();
       if (key != EOF) {
           // Envoi de la touche saisie au serveur
           if (send(sockfd, &key, 1, 0) < 0)
           {
               perror("send");
               break;
           }
           if (key == 'x')
           {
               quit = true;
           }
       }
   }
   restoreMode();
   printf("Fin du contrôle.\n");
 
   running = STOPPED;
 
   close(sockfd);
 }
 