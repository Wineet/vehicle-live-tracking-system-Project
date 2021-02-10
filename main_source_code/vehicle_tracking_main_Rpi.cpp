/* Purpose of Program:
 *                    1. Program Helps to form a Data Channel and connect to Internet by Modem
 *                    2. Program Fetche GPS co-ordinates from Modem To execute Geo-fence 
 *                    3. Vehicle Tracking Application
 * 
 * Software Details: 
 *                  Rx Thread will be continuesly running to fetch response form Modem (AT Command Response)
 *                  Tx Thread Used To fire At commands to the device
 *                  Function to Identify weather location are inside perimeter or outside (Geofence)
 *                  other Modem specific calls (For SMS, CALL, Internet)
 */

#include<stdio.h>
#include<iostream>
#include<pthread.h>
#include<errno.h>
#include<cstring>
#include<unistd.h>

using namespace std;

/* User Defined Files */
#include"class_decl.h"
#include"vehicle_tracking_func_decl.h"

/* User Defined Macro */


//#define FILE_NAME "/home/vinit/vinit/sample.txt"
#define FILE_NAME "/home/vinit/demo.txt"

static status_t tx_modem_cmd( char *cmd_arg);
static status_t rx_modem_resp(char *resp_buffer,int resp_size);

void *tx_thread(void *arg);
void *rx_thread(void *arg);

int main()
{
    cout<<"vehicle tracking Program started"<<endl;
    Thread tx_obj,rx_obj;
    if( FAIL == tx_obj.create_thread(tx_thread,&tx_obj))
    {
        cout<<"Thread Creation Failed "<<__LINE__<<endl;
        return 1;
    }
    if(FAIL==rx_obj.create_thread(rx_thread,&rx_obj))
    {
        tx_obj.cancel_thread();
        cout<<"Thread Creation Failed "<<__LINE__<<endl;
        return 1;   
    }

     pthread_join(tx_obj.get_thread_id(),NULL);
     pthread_join(rx_obj.get_thread_id(),NULL);

    return 0;
}

/* Thread Routine */
#define MAX_TX_TRANS_BUFF 80

void *tx_thread(void * arg)
{
    status_t thread_running = TRUE;
    Thread *tx_obj = new Thread;
    char tx_trans_buff[MAX_TX_TRANS_BUFF]={0};
    memcpy(tx_obj,arg,sizeof(Thread));
    cout<<"Tx thread Running"<<endl;    
    while( TRUE == thread_running)
    {
        //cin.flush();
        cin.getline(tx_trans_buff,sizeof(tx_trans_buff)-1);
        if(FAIL==tx_modem_cmd(tx_trans_buff))
        {
            cout<<"Command Tx Failed line "<<__LINE__<<endl;
        }
    }

    tx_obj->thread_exit();
    return NULL;
}

#define MAX_RX_RESP_BUFF 1024

void *rx_thread(void * arg)
{
    status_t thread_running = TRUE;
    Thread *rx_obj = new Thread;
    char rx_resp_buff[MAX_RX_RESP_BUFF]={0};
    memcpy(rx_obj,arg,sizeof(Thread));
    cout<<"Rx thread Running"<<endl;
    while( TRUE == thread_running)
    {
        cout.flush();
        memset(rx_resp_buff,0,sizeof(rx_resp_buff));
        if(FAIL == rx_modem_resp(rx_resp_buff,MAX_RX_RESP_BUFF-1) )
        {
            cout<<"Resp Read Failed line "<<__LINE__<<endl;
        }
        cout<<"Resp= "<<rx_resp_buff<<endl;
        sleep(1);

    }

    rx_obj->thread_exit();
    return NULL;
}

/* Thread Routine End*/

/* Thread Function call */

static status_t tx_modem_cmd( char *cmd_arg)
{
    status_t ret =SUCCESS;
     if (FAIL == write_data_to_file(FILE_NAME,cmd_arg) )
     {
         ret =FAIL;
         return ret;
     }
    return ret;
}

static status_t rx_modem_resp(char *resp_buffer,int resp_size)
{
    status_t ret =SUCCESS;
    if(FAIL == read_data_from_file(FILE_NAME,resp_buffer,resp_size) )
    {
        ret = FAIL;
        return ret;
    }
    return ret;
}