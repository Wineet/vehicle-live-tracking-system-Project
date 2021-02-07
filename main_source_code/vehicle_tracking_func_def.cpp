/*File used to define all locally used Function calls*/
#include<stdio.h>
#include<iostream>
#include<pthread.h>
#include<errno.h>
#include<cstring>
#include<unistd.h>
#include<fstream>


#include "class_decl.h"

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
    ofstream file_write;
    status_t ret = SUCCESS;
//    file_write.open(file_name,ios::out|ios::app|ios::nocreate);
//    ios::nocreate Doesnot exist in standard C++ libraray so following Below Work around
//    To avoide Creating a new Empty File

//>> File is available or not Work around
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
    return ret;
}