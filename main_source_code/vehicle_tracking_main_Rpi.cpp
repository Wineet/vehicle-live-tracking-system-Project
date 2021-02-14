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
#include<signal.h>

/*Pi UART Libraries*/
#include<wiringPi.h>
#include<wiringSerial.h>

using namespace std;

/* User Defined Files */
#include"class_decl.h"
#include"vehicle_tracking_func_decl.h"

/* User Defined Macro */
#define RPI_SERIAL_LIB_USE
#define RPI_SERIAL_FILE_USE

//#undef RPI_SERIAL_LIB_USE
#undef RPI_SERIAL_FILE_USE

#define RX_THREAD_ACTIVE
#define TX_THREAD_ACTIVE

//#undef RX_THREAD_ACTIVE
//#undef TX_THREAD_ACTIVE

/* Global Varibales */
static int serialFd_write = 0;
static int serialFd_read  = 0;

#define FILE_NAME "/dev/serial0"

typedef void (*sighandler_t)(int);

static status_t tx_modem_cmd( char *cmd_arg);
static status_t rx_modem_resp(char *resp_buffer,int resp_size);
void signal_handler(int);
void *tx_thread(void *arg);
void *rx_thread(void *arg);

int main()
{
    cout<<"vehicle tracking Program started"<<endl;
    Thread tx_obj,rx_obj;
#ifdef  TX_THREAD_ACTIVE
    if( FAIL == tx_obj.create_thread(tx_thread,&tx_obj))
    {
        cout<<"Thread Creation Failed "<<__LINE__<<endl;
        return 1;
    } 
#endif

#ifdef RX_THREAD_ACTIVE
    if(FAIL==rx_obj.create_thread(rx_thread,&rx_obj))
    {
        tx_obj.cancel_thread();
        cout<<"Thread Creation Failed "<<__LINE__<<endl;
        return 1;   
    }
#endif

    signal(SIGINT,signal_handler);
#ifdef  TX_THREAD_ACTIVE
     pthread_join(tx_obj.get_thread_id(),NULL);
#endif
#ifdef  RX_THREAD_ACTIVE
     pthread_join(rx_obj.get_thread_id(),NULL);
#endif
    return 0;
}

/* Thread Routine */
#define MAX_TX_TRANS_BUFF 80

void *tx_thread(void * arg)
{
    status_t thread_running = SUCCESS;
    Thread *tx_obj = new Thread;
    char tx_trans_buff[MAX_TX_TRANS_BUFF]={0};
    memcpy(tx_obj,arg,sizeof(Thread));
#ifdef RPI_SERIAL_LIB_USE
    status_t ret =SUCCESS;
    serialFd_write=0;
    int bytes_to_write =0;
    serialFd_write=serialOpen(FILE_NAME,9600);
    if(0==serialFd_write)
    {
        cout<<"serial open Failed "<<__LINE__<<":"<<__FILE__<<endl;
        ret = FAIL;
        thread_running = FAIL;
    }
#endif
    cout<<"Tx thread Running"<<endl;    
    while( SUCCESS == thread_running)
    {
         cin.getline(tx_trans_buff,sizeof(tx_trans_buff)-1);
#ifdef RPI_SERIAL_LIB_USE
        serialPrintf(serialFd_write,cmd_arg);
#endif

#ifdef RPI_SERIAL_FILE_USE
        //cin.flush();
        if(FAIL==tx_modem_cmd(tx_trans_buff))
        {
            cout<<"Command Tx Failed line "<<__LINE__<<endl;
        }
#endif
    }
    tx_obj->thread_exit();
    return NULL;
}

#define MAX_RX_RESP_BUFF 1024

void *rx_thread(void * arg)
{
    status_t thread_running = SUCCESS;
    Thread *rx_obj = new Thread;
    char rx_resp_buff[MAX_RX_RESP_BUFF]={0};
    memcpy(rx_obj,arg,sizeof(Thread));
#ifdef RPI_SERIAL_LIB_USE
    serialFd_read=0;
    int bytes_to_read =0;
    char resp_buff[1024]={0};
        serialFd_read=serialOpen(FILE_NAME,9600);
        if(0==serialFd_read)
        {
           cout<<"serial open Failed"<<__LINE__<<__FILE__<<endl;
           ret = FAIL;
           thread_running = FAIL;
        }
#endif
    cout<<"Rx thread Running"<<endl;

    while( SUCCESS == thread_running)
    {
#ifdef RPI_SERIAL_LIB_USE
        status_t ret = SUCCESS;
        status_t read_status=INVALID;
        if( 0 < ( bytes_to_read=serialDataAvail(serialFd_read)) )
        {
            for(int i =0;i<bytes_to_read && i < 1024 ;i++)
            {
                resp_buff[i]=serialGetchar(serialFd_read);
            }
            read_status = BYTES_READ;
            cout<<"read Data bytes "<<bytes_to_read<<resp_buff<<endl;
            if(bytes_to_read > resp_size)
            {
                cout<<"Data Trucated "<<bytes_to_read<<resp_buff<<endl;
                ret = FAIL;
            }
           // memcpy(resp_buffer,resp_buff,bytes_to_read > 1023 ?1023:bytes_to_read);
        }
        else
        {
            read_status = BYTES_NOT_READ;
            cout<<"No Data to read"<<endl;
        }
        
        #endif

#if RPI_SERIAL_FILE_USE
        cout.flush();
        memset(rx_resp_buff,0,sizeof(rx_resp_buff));
        if(FAIL == rx_modem_resp(rx_resp_buff,MAX_RX_RESP_BUFF-1) )
        {
            cout<<"Resp Read Failed line "<<__LINE__<<endl;
        }
        cout<<"Resp= "<<rx_resp_buff<<endl;
        sleep(1);
#endif

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


/* Exception Signal Handler */
void signal_handler(int arg)
{
    cout<<"CTRL+c signal Handler"<<endl;

#ifdef RPI_SERIAL_LIB_USE
    serialClose(serialFd_read);
    serialClose(serialFd_write);
#endif

}