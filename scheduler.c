#include "scheduler.h"
#include <stdio.h>   /* required for file operations */
#include <time.h>
#include <stdbool.h>
#include "configLoad.h"

static scheduler_t sData;
static bool initDone ;
   
void algo()
{
   unsigned i;
   for( i = sData.threadID ; i<10 ; i++)
   {
      printf("Current thread is: %d, data: %d\n", sData.threadID, i);
      sleep(1);
   }
}


void restoreState(unsigned idx)
{
    siglongjmp(sData.env[idx], 1);
    return;
}
int saveState(unsigned idx)
{
   // giving thread idx and value to return
  
   return sigsetjmp(sData.env[idx], 1);
}

unsigned lottery (void) {

    unsigned totalactivos = 0;
    unsigned winthread = 0;
    unsigned winner = 0;

    /* En caso ningun thread este activo */
    if (!config.threadbuffer[0] && !config.threadbuffer[1] && !config.threadbuffer[2] && !config.threadbuffer[3] && !config.threadbuffer[4]) {
        winthread = 0;
        winner = 0;
        return winthread;
    }

    /* Determina el total de tiquetes que seran evaluados para obtener el ganador */
    if (config.threadbuffer[0]) totalactivos = config.tickets[0];
    if (config.threadbuffer[1]) totalactivos = (totalactivos + config.tickets[1]);
    if (config.threadbuffer[2]) totalactivos = (totalactivos + config.tickets[2]);
    if (config.threadbuffer[3]) totalactivos = (totalactivos + config.tickets[3]);
    if (config.threadbuffer[4]) totalactivos = (totalactivos + config.tickets[4]);

    /* Realiza la rifa y se obtiene de manera aleatoria el boleto ganador */
    do {
        winner = rand() % totalactivos;
    } while (winner == 0);

    /* Determina segun el boleto cual es el thread ganador */
    /* Casos en q puede ganar thread 1 */
    if (config.threadbuffer[0] && winner <= config.tickets[0]) {
        winthread = 1;
        return winthread;
    }

    /* Casos en q puede ganar thread 2 */

    if (config.threadbuffer[0] && config.threadbuffer[1] && winner <= (config.tickets[1]+config.tickets[0])) {
        winthread = 2;
        return winthread;
    }
    if (!config.threadbuffer[0] && config.threadbuffer[1] && winner <= config.tickets[1]) {
        winthread = 2;
        return winthread;
    }

    /* Casos en q puede ganar thread 3 */

    if (config.threadbuffer[0]) {
        if (config.threadbuffer[1]) {
            if (config.threadbuffer[2] && winner <= (config.tickets[0]+config.tickets[1]+config.tickets[2])) {
                winthread = 3;
                return winthread;
            }
        }
        if (!config.threadbuffer[1]) {
            if (config.threadbuffer[2] && winner <= (config.tickets[0]+config.tickets[2])) {
                winthread = 3;
                return winthread;
            }
        }
    }
    if (!config.threadbuffer[0]) {
        if (config.threadbuffer[1]) {
            if (config.threadbuffer[2] && winner <= (config.tickets[1]+config.tickets[2])) {
                winthread = 3;
                return winthread;
            }
        }
        if (!config.threadbuffer[1]) {
            if (config.threadbuffer[2] && winner <= config.tickets[2]) {
                winthread = 3;
                return winthread;
            }
        }
    }

    /* Casos en q puede ganar thread 4 */

    if (config.threadbuffer[0]) {
        if (config.threadbuffer[1]) {
            if (config.threadbuffer[2]) {
                if (config.threadbuffer[3] && winner <= (config.tickets[0]+config.tickets[1]+config.tickets[2]+config.tickets[3])) {
                    winthread = 4;
                    return winthread;
                }
            }
            if (!config.threadbuffer[2]) {
                if (config.threadbuffer[3] && winner <= (config.tickets[0]+config.tickets[1]+config.tickets[3])) {
                    winthread = 4;
                    return winthread;
                }
            }
        }
        if (!config.threadbuffer[1]) {
            if (config.threadbuffer[2]) {
                if (config.threadbuffer[3] && winner <= (config.tickets[0]+config.tickets[2]+config.tickets[3])) {
                    winthread = 4;
                    return winthread;
                }
            }
            if (!config.threadbuffer[2]) {
                if (config.threadbuffer[3] && winner <= (config.tickets[0]+config.tickets[3])) {
                    winthread = 4;
                    return winthread;
                }
            }
        }
    }
    if (!config.threadbuffer[0]) {
        if (config.threadbuffer[1]) {
            if (config.threadbuffer[2]) {
                if (config.threadbuffer[3] && winner <= (config.tickets[1]+config.tickets[2]+config.tickets[3])) {
                    winthread = 4;
                    return winthread;
                }
            }
            if (!config.threadbuffer[2]) {
                if (config.threadbuffer[3] && winner <= (config.tickets[1]+config.tickets[3])) {
                    winthread = 4;
                    return winthread;
                }
            }
        }
        if (!config.threadbuffer[1]) {
            if (config.threadbuffer[2]) {
                if (config.threadbuffer[3] && winner <= (config.tickets[2]+config.tickets[3])) {
                    winthread = 4;
                    return winthread;
                }
            }
            if (!config.threadbuffer[2]) {
                if (config.threadbuffer[3] && winner <= config.tickets[3]) {
                    winthread = 4;
                    return winthread;
                }
            }
        }
    }

    /* Casos en q puede ganar thread 5 */

    if (config.threadbuffer[0]) {
        if (config.threadbuffer[1]) {
            if (config.threadbuffer[2]) {
                if (config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[0]+config.tickets[1]+config.tickets[2]+config.tickets[3]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
                if (!config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[0]+config.tickets[1]+config.tickets[2]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
            }
            if (!config.threadbuffer[2]) {
                if (config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[0]+config.tickets[1]+config.tickets[3]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
                if (!config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[0]+config.tickets[1]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
            }
        }
        if (!config.threadbuffer[1]) {
            if (config.threadbuffer[2]) {
                if (config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[0]+config.tickets[2]+config.tickets[3]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
                if (!config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[0]+config.tickets[2]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
            }
            if (!config.threadbuffer[2]) {
                if (config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[0]+config.tickets[3]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
                if (!config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[0]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
            }
        }
    }
    if (!config.threadbuffer[0]) {
        if (config.threadbuffer[1]) {
            if (config.threadbuffer[2]) {
                if (config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[1]+config.tickets[2]+config.tickets[3]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
                if (!config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[1]+config.tickets[2]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
            }
            if (!config.threadbuffer[2]) {
                if (config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[1]+config.tickets[3]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
                if (!config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[1]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
            }
        }
        if (!config.threadbuffer[1]) {
            if (config.threadbuffer[2]) {
                if (config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[2]+config.tickets[3]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
                if (!config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[2]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
            }
            if (!config.threadbuffer[2]) {
                if (config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= (config.tickets[3]+config.tickets[4])) {
                        winthread = 5;
                        return winthread;
                    }
                }
                if (!config.threadbuffer[3]) {
                    if (config.threadbuffer[4] && winner <= config.tickets[4]) {
                        winthread = 5;
                        return winthread;
                    }
                }
            }
        }
    }

    return winthread;
}

void preemtiveTime()
{
   setitimer(ITIMER_VIRTUAL, &sData.timer, 0);
   return;
}
// placeholder
bool invalidateThread (unsigned ID) {
    bool Done = false;

    if (ID == 1) config.threadbuffer[0] = false;
    if (ID == 2) config.threadbuffer[1] = false;
    if (ID == 3) config.threadbuffer[2] = false;
    if (ID == 4) config.threadbuffer[3] = false;
    if (ID == 5) config.threadbuffer[4] = false;

    Done = !(config.threadbuffer[0] | config.threadbuffer[1] | config.threadbuffer[2] | config.threadbuffer[3] | config.threadbuffer[4]);

    return Done;
}

void schedulerInit()
{
   unsigned thread;
   initDone = true;
   for( thread = 0; thread < MAX_THREADS ; thread ++ )
   {
      sData.taskInit[thread] = false;
   }
   // init the timer with 0
   sData.timer.it_interval.tv_sec = 0;
   sData.timer.it_interval.tv_usec = 0;
   sData.timer.it_value.tv_sec = 2; // quantum value
   sData.timer.it_value.tv_usec=0;
}
/***************************************************
 *
 * Lottery Scheduler
 *
 ***************************************************/
void scheduler()
{
   int ret;
   bool allDone;
   printf("Getting the alarm for thread: %d\n", sData.threadID );
   // Save the state
   ret = saveState(sData.threadID);
   if( ret != 0 )
   {
      // return to resume execution
      // in the thread
      return;
   }
   if( !initDone )
   {
      schedulerInit();
   }
   sData.threadID = lottery();
   // Set timer
   preemtiveTime();
   // Catch the timer signal
   signal(SIGALRM, scheduler);
   printf("sigalm passed\n");
   if( !sData.taskInit[sData.threadID] )
   {
      sData.taskInit[sData.threadID] = true;
      // when reaching this part the task finished execution
      // call function for first time
      algo();
      allDone = invalidateThread(sData.threadID);
      // enters if all the threads had completed their job
      if(allDone)
      {
         return;
      }
   }
   else
   {
      // Restore the state
      restoreState(sData.threadID);
   }
}


