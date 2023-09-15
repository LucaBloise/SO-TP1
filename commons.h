#ifndef TP_SO_COMMONS_H
#define TP_SO_COMMONS_H


#define PIPE_CAP 4096

#define MAX_PATH_LENGTH 256
#define MD5_LENGTH 32
#define MAX_PID_LENGTH 6 //TODO: acordarse de poner en el informe que se limita el path a 256 chars y el pid a 6 digitos
#define PID_FMT_LEN 5 //"pid: "
#define PATH_FMT_LEN 6 //"path: "
#define MD5_FMT_LEN 5 //"md5: "
#define FORMAT_LENGTH PID_FMT_LEN+PATH_FMT_LEN+MD5_FMT_LEN
#define SIZE_PER_FILE MAX_PATH_LENGTH+MD5_LENGTH+MAX_PID_LENGTH+FORMAT_LENGTH+1 //"\n"
#define SHM_NAME "myShm"

#endif
