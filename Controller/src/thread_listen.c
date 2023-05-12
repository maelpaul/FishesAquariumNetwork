#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "thread.h"

#include "server_utils.h"

// int thread_listen((void) arg (struct ))
// {

//     while (1) {
//         // Met le socket en écoute sur le port spécifié
//         if (listen(server_fd, 5) < 0) {
//             perror("Erreur lors de la mise en écoute du socket");
//             exit(EXIT_FAILURE);
//         }
//         => appel fonction ajout client

//     }

// }

// code main 

// struct thread_listening {
//     boolean should_stop;
//     (void) f;
// }

