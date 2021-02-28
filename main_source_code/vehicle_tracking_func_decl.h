#ifndef VEHICLE_TRACKING_FUNC_DECL_H
#define VEHICLE_TRACKING_FUNC_DECL_H


/* Header File Holds All locally used function call definition */

/* File read and Write Operation Specific to Modem */

status_t write_data_to_file(const char *file_name, char *write_data_buff);
status_t read_data_from_file(const char *file_name, char *read_data_buff, int bytes_to_read);
void send_at_cmd(char *arg);
char *parse_at_output(char *resp_buff, int bytes_to_parse);




#endif