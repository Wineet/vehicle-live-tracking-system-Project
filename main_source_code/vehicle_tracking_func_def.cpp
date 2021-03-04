/*File used to define all locally used Function calls*/
#include<stdio.h>
#include<iostream>
#include<pthread.h>
#include<errno.h>
#include<cstring>
#include<unistd.h>
#include<fstream>
#include<string.h>

#include "class_decl.h"
#define SPECIAL_FILE
//#define NORMAL_FILE
#define FILE_CHAR_SIZE 1
#define FILE_READ_SIZE 512

#ifdef RPI_SERIAL_FILE_USE
 extern FILE *file_ptr_write  ;
 extern FILE *file_ptr_read   ;
#endif
/*
 *  read_data_from_file
 *  Functionality:
 *               Function call should be used to read chunck of data in One shot
 *               Function should not be used for stream reading
 *  
 *  Read Data From File and store in address pointed by data_buff pointer
 *  Function open file pointed by argument file_name
 *  read status returned by function
 *  Argument: file_name     --> File Name
 *            data_buff     --> Address of array
 *            bytes_to_read -->
 *  Function Return Number bytes read From File
 */

status_t read_data_from_file(const char *file_name, char *read_data_buff, int bytes_to_read) 
{
    status_t ret = SUCCESS;
 #ifdef SPECIAL_FILE 
 #ifdef RPI_SERIAL_FILE_USE
    int  read_bytes = 0,read_ret=0;
    file_ptr_read=fopen(file_name,"r+");
    cout<<"FILE name "<<file_name<<endl;
    if(NULL==file_ptr_read)
    {
         cout<<"FILE open Failed "<<__LINE__<<strerror(errno)<<endl;
         ret = FAIL;
         exit(1);
    }
    //fseek(file_ptr,0,SEEK_SET);
    
        char data_buff[1024]={0};
        fread(data_buff,FILE_READ_SIZE,1,file_ptr_read);

        //read_bytes += read_ret;
       // cout<<"read_ret "<<read_ret<<endl;
        cout<<" bytes read"<<data_buff<<endl;
 #endif
 #endif

 #ifdef NORMAL_FILE
 #ifdef RPI_SERIAL_FILE_USE
    ifstream file_read;
    file_read.open(file_name,ios::in);
    if(file_read.fail())
    {
        ret = FAIL;
        return ret;
    }
    file_read.seekg(0);                         // setting Cursor to Start
    file_read.read(read_data_buff,bytes_to_read);
    if(!file_read.eof())
    {
        cout<<"Data Not Read Completely"<<endl;
        ret = FAIL;
    }
    file_read.close();
#endif
#endif
    return ret;
}


/*
 *  write_data_to_file
 *  Functionality:
 *               Function call should be used to write chunck of data in One shot
 *               Function should not be used for stream Writing
 *  
 *  write Data to File, Writing data will be fetched from address pointed by write_data_buff pointer
 *  Function open file pointed by argument file_name
 *  read status returned by function
 *  Argument: file_name           --> File Name
 *            write_data_buff     --> Address of data array
 *            
 *  Function Return Number bytes read From File
 *  Note: Function will not create a new File if file is not Present (Designed as per requirement) 
 */
status_t write_data_to_file(const char *file_name, char *write_data_buff)
{
    status_t ret = SUCCESS;

 #ifdef SPECIAL_FILE 
 #ifdef RPI_SERIAL_FILE_USE
    int  write_bytes = 0,write_ret=0;
    file_ptr_write=fopen(file_name,"a+");
   //  cout<<"FILE name "<<file_name<<endl;
    if(NULL==file_ptr_write)
    {
         cout<<"FILE open Failed "<<__LINE__<<strerror(errno)<<endl;
         ret = FAIL;
         exit(1);
    }
    fseek(file_ptr_write,0,SEEK_SET);
   // rewind(file_ptr);
    write_bytes=strlen(write_data_buff);
    
    for(int i=0;i<=write_bytes;i++)
    {
         write_ret = fwrite(write_data_buff+i,FILE_CHAR_SIZE,1,file_ptr_write);
     //    write_bytes += read_ret;
    }
    
#endif
#endif

#ifdef NORMAL_FILE
#ifdef RPI_SERIAL_FILE_USE
//    file_write.open(file_name,ios::out|ios::app|ios::nocreate);
//    ios::nocreate Doesnot exist in standard C++ libraray so following Below Work around
//    To avoide Creating a new Empty File

//>> File is available or not Work around
    ofstream file_write;
    ifstream file_check;
    file_check.open(file_name);
    if(file_check.fail())
    {
        ret = FAIL;
        cout<<" File Doesn't Exist "<<__FILE__<<" "<<__LINE__<<endl;
        return ret;
    }
    file_check.close();
//>> Workaoriund End

    file_write.open(file_name,ios::out);
    if(file_write.fail())
    {
        ret = FAIL;
        cout<<" File open Failed "<<__FILE__<<" "<<__LINE__<<endl;
        return ret;
    }
    file_write.seekp(0);
    file_write.write(write_data_buff,strlen(write_data_buff));
    if(file_write.fail())
    {
        ret = FAIL;
    }
    file_write.close();
#endif
#endif
    return ret;
}


/* Generoc Function for Data Parsing */



cmd_resp_header_e get_header_enum(char *arg_header)
{
    if(0 == strcmp(arg_header,"+CGNSINF"))
        return CGNSINF;
    else if(0 == strcmp(arg_header,"+COPS"))
        return COPS;
    else if(0 == strcmp(arg_header,"+CSQ"))
        return CSQ;
    else
        return INVALID_HEADER;
}
at_resp_data resp_data={0};


void fill_at_response(char *arg_header,char *arg_body)
{
    char body_parts[20][20]={0};
    switch(get_header_enum(arg_header))
    {
        case CGNSINF:
        {
         char *token=NULL;
         int i=0;
        // cout<<"CGNSINF Enum"<<endl;
         token=strtok(arg_body,",");
         if(token == NULL)
         {
            cout<<"ERROR No token Found"<<endl;
            return;
         }
         strncpy(body_parts[i],token,20);
         while(NULL !=(token=strtok(NULL,",")) )
         {
            strncpy(body_parts[++i],token,20);    
         }
         if( 1 == atof(body_parts[0]) && 1 == atof(body_parts[0]) )
         {
            resp_data.gps_data.lat = atof(body_parts[3]);
            resp_data.gps_data.lon = atof(body_parts[4]);
            resp_data.gps_data.alt = atof(body_parts[5]);
            strncpy(resp_data.gps_data.date,body_parts[2],25);
            cout<<"GPS Latched lat "<<resp_data.gps_data.lat<<" lon"<< resp_data.gps_data.lon <<endl;
         }
         else
         {
             cout<<"GPS Not Latched"<<endl;
         }
        
         break;
        }    
        case COPS:
        {
          cout<<"COPS Enum "<<endl;
          
          break;
        }    
        case CSQ:
        {
            cout<<"CSQ Enum"<<endl;
            break;
        }   
        default:
        cout<<"Invalid Enum"<<endl;
        break;
    }
}
char *parse_at_output(char *resp_buff, int bytes_to_parse)
{
    char dum_buff[1024]={0};
    char *dum_ptr=NULL;
    char cmd_header[20]={0};
    char *_header_ptr =NULL;
    char cmd_body[512]={0};
 // at_resp_data resp_data={0};
 //   cout<<"resp Buff "<<resp_buff<<endl;
    memcpy(dum_buff,resp_buff,bytes_to_parse);
 //   cout<<"dum Buff "<<dum_buff<<endl;
    dum_ptr=(char *)strstr(dum_buff,"\r\n");
    if(dum_ptr == NULL)
    {
    	cout<<"Parsing Failed"<<endl;
   	return NULL; 
    }
    dum_ptr+=2; // to move further "\r\n"
 //   cout<<"dum_ptr = "<<dum_ptr<<endl;
    _header_ptr = strchr(dum_ptr,':');
    if(NULL == _header_ptr)
    {
        if (NULL != strstr(dum_ptr,"OK"))
        {
            strcpy(resp_buff,"OK");
            cout<<"Resp " << resp_buff<<endl;
            return resp_buff;
        }
        cout<<"Can't Parse No Header"<<dum_ptr<<endl;
        return NULL; 
    }
    memcpy(cmd_header,dum_ptr,_header_ptr-dum_ptr);
    //cout<<"_header_ptr = "<<cmd_header<<endl;
    //cout<<"cmd_header = "<<cmd_header<<endl;
    strcpy(cmd_body,_header_ptr+1);
    //cout<<"cmd_body = "<<cmd_body<<endl;
    
    fill_at_response(cmd_header,cmd_body);
return resp_buff;
}
