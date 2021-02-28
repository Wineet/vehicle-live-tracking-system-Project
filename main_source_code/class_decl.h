#ifndef CLASS_DECL_H
#define CLASS_DECL_H


using namespace std;
typedef enum{INVALID=-1,SUCCESS=0, FAIL=1,BYTES_READ=1,BYTES_NOT_READ=3}status_t;
typedef enum{INVALID=-1,OK=0,ERROR}at_return_t;
typedef void *(*thread_routine)(void *);

/*AT COMMAND strcture To handle all possible Output and it's value
 * Structure is Global 
 * Structure should only get changed by function " "
 * */
struct{
        at_return_t cmd_output;
        char raw_output[1024];

}at_out_struct;

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
