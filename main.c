#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ftplib.h"
#include "ftp.h"
#include "hdu_log.h"

#define BUFSIZE 1024

extern netbuf* conn;

static char* host = "192.168.1.2";
static char* user = "luoxn28";
static char* pass = "123456";
char pwd[BUFSIZE] = {0};

FILE* debug_fp = NULL;
char debug_time[25];

#if 1
int main(void)
{
    char buf[BUFSIZE];
    char op1[BUFSIZE], op2[BUFSIZE], op3[BUFSIZE];

    // init the log serveer and set the log file name
    InitDebug();
    SetDebugFp("log.txt");
    DEBUG_PRINT("start");

    // connect to FTP server host
    ftp_connect(host, user, pass);
    printf("---- connect to the ftp server ----\n");

    printf("%s:%s$ ", user, pwd);
    get_line(buf, sizeof(buf));
    op1[0] = '\0'; op2[0] = '\0'; op3[0] = '\0';
    sscanf(buf, "%s%s%s", op1, op2, op3);
    fflush(stdin);
    while (strcmp(op1, "exit"))
    {
        if (!strcmp(op1, "clear"))
            system("clear");
        else if (!strcmp(op1, "help") || !strcmp(op1, "h"))
            op_help();
        else if (!strcmp(op1, "pwd"))
            op_pwd();
        else if (!strcmp(op1, "sys"))
            op_sys();
        else if (!strcmp(op1, "ls"))
            op_ls(op2);
        else if (!strcmp(op1, "ll"))
            op_ll(op2);
        else if (!strcmp(op1, "cd"))
            op_cd(op2);
        else if (!strcmp(op1, "mkdir"))
            op_mkdir(op2);
        else if (!strcmp(op1, "rmdir"))
            op_rmdir(op2);
        else if (!strcmp(op1, "get"))
            op_get(op2, op3);
        else if (!strcmp(op1, "put"))
            op_put(op2, op3);
        else if (!strcmp(op1, "rename"))
            op_rename(op2, op3);
        else
            printf("commond not found (help/h to see the commond)\n");

        printf("%s:%s$ ", user, pwd);
        get_line(buf, sizeof(buf));
        op1[0] = '\0'; op2[0] = '\0'; op3[0] = '\0';
        sscanf(buf, "%s%s%s", op1, op2, op3);
        fflush(stdin);
    }

    DEBUG_PRINT("end");;

    ftp_exit();
    return 0;
}
#endif

#if 0
int main(void)
{
    char buf[BUFSIZE];
    char op1[BUFSIZE], op2[BUFSIZE], op3[BUFSIZE];

    ftp_connect(host, user, pass);
    printf("---- connect to the ftp server ----\n");
/*
    if (!FtpNlst(NULL, "/old_code", conn))
    {
        perror("FtpNlst");
        return;
    }*/
    if (!FtpRename("zzzz.cpp", "zzaa.txt", conn))
    {
        //DEBUG_ERR("FtpRename");
        exit(1);
    }
    puts("rename ok");
    sleep(5);

    ftp_exit();
    return 0;
}
#endif
