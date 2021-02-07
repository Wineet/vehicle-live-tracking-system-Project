#include<stdio.h>
#include<iostream>
#include<pthread.h>
#include<errno.h>
#include<cstring>
#include<unistd.h>

using namespace std;

/* User Defined Files */
#include"class_decl.h"


/*
* Independant thread
* Only thread can cancel it self
*
*/

void *routine(void *arg);

int main()
{
    Thread t1_obj;
    cout<<"Hello World"<<endl;
    t1_obj.create_thread(routine,&t1_obj);
     cout<<"Hello World2"<<endl;
    pthread_join(t1_obj.get_thread_id(),NULL);
    return 0;
}

void *routine(void *arg)
{
    Thread *t1_obj = new Thread;
    memcpy(t1_obj,arg,sizeof(Thread));
    int count=0;
    while(1)
    {
        cout<<"Alive"<<endl;
        sleep(1);
        count++;
        if(count ==10)
        {
            t1_obj->cancel_thread();
        }
    }
}