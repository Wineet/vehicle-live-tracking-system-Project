#ifndef CLASS_DECL_H
#define CLASS_DECL_H


using namespace std;
typedef enum{SUCCESS=0, FAIL=1,TRUE=0,FALSE =1}status_t;
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
        void thread_exit();

};



#endif