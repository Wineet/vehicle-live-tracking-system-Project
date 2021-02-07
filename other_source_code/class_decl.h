#ifndef CLASS_DECL_H
#define CLASS_DECL_H


using namespace std;
typedef enum{SUCCESS, FAIL}status_t;
typedef void *(*thread_routine)(void *);

class Thread{
    private:
        pthread_t thread_id;
        pthread_attr_t thread_attr;
        char thread_name[50];
    public:
        Thread();
        status_t create_thread(thread_routine routine_handle,void *arg);
        status_t set_thread_sched_param(sched_param thread_sched_param,int );
        pthread_t get_thread_id();
        status_t cancel_thread();


};



#endif