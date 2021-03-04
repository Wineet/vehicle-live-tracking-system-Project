/* All Class Function are define Here
 *
 * */
#include<stdio.h>
#include<iostream>
#include<pthread.h>
#include<errno.h>
#include<cstring> 
#include<unistd.h>  

#include"class_decl.h"


/****** Thread class Body Declaration Start Here ********/

/* Thread Class Constructor
 * Function Definition: Function initilize variable
 * Passing Argument : None
 * Job: Thread Class Parameter Initilisation
 *      Default Scheduling Policy Scheduling Policy 
 *      Thread Cancelation is Disabled
 * 
 */
      Thread ::  Thread()
        {
            memset(thread_name,0,sizeof(thread_name));
            thread_id=0;
            pthread_attr_init(&thread_attr);
            if( 0 != pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL))
            {
                cout<<"pthread_set_cancel sate Failed"<<strerror(errno)<<endl;
            }
        }
/* create_thread
 * Function Definition: Function creates Thread
 * Passing Argument : Function Rountine, Passing argument (void *)
 * Job: 
 *      sched_rr rounding robin Scheduling Policy Scheduling Policy 
 *      Thread Cancelation is Disabled
 * 
 */
        status_t  Thread :: create_thread(thread_routine routine_handle,void *arg)
        {
            if(0 != pthread_create(&thread_id,&thread_attr,routine_handle,arg) )
            {
                cout<<"Pthread Create fail "<<strerror(errno)<<endl;
                return FAIL;
            }
            return SUCCESS;
        }

 /* set_thread_sched_param
 * Function Definition: Function chnages scheduling Policy
 * Passing Argument :   sched_param object, scheduling Policy
 * Job: 
 *      Set scheduling Policy For thread
 * 
 */       
        status_t  Thread :: set_thread_sched_param(sched_param thread_sched_param,int sched_policy = SCHED_RR)
        {
            if (0 != pthread_attr_setschedpolicy(&thread_attr,sched_policy))
            {
                cout<<"set sched fail "<<strerror(errno)<<endl;
                return FAIL;
            }
            
            if (0!= pthread_attr_setschedparam(&thread_attr,&thread_sched_param))
            {
                cout<<"set sched param fail "<<strerror(errno)<<endl;
                return FAIL;
            }
            return SUCCESS;
        }

 /* get_thread_id
 * Function Definition: Function return Thread Id
 * Passing Argument :   None
 * Job: 
 *      return thread_id
 * 
 */
        pthread_t  Thread :: get_thread_id()
        {
            return  thread_id;
        }
    
 /* cancel_thread
 * Function Definition: Function Cancel Thread
 * Passing Argument :   None
 * Job: 
 *       Thread activate thread Cancel policy, calling Thread will Cancel It Self 
 * 
 */
        status_t  Thread :: cancel_thread()
        {
            if( 0 != pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL))
            {
                cout<<"pthread_set_cancel sate Failed Line "<<__LINE__<<"File "<<__FILE__<<strerror(errno)<<endl;
                return FAIL;
            }
            if(0!= pthread_cancel(thread_id))
            {
                cout<<"pthread_cancel Failed Line "<<__LINE__<<"File "<<__FILE__<<strerror(errno)<<endl;                
                return FAIL;
            }
            return SUCCESS;
        }



/* Thread Exit Function call
 * Implemented For only Exit without any return Status
 * Can be modified For later Use
 * */
        void Thread :: thread_exit()
        {
            pthread_exit(NULL);

        }
