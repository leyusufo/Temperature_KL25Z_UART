#ifndef THREADS_H_   /* Include guard */
#define THREADS_H_

/*
function declaration
*/
void RTOS_objects_config(void);

void Thread_Convert_Temp(void *arg);
void Thread_Prepare_Msg(void *arg);
void Thread_Transmit_Msg(void *arg);


#endif // THREADS_H_
