#ifndef FTP_H_INCLUDED
#define FTP_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define GLOBALREF extern

GLOBALREF void op_help(void);
GLOBALREF void op_pwd(void);
GLOBALREF void op_sys(void);
GLOBALREF void op_ls(char op2[]);
GLOBALREF void op_ll(char op2[]);
GLOBALREF void op_cd(char op2[]);
GLOBALREF void op_mkdir(char op2[]);
GLOBALREF void op_rmdir(char op2[]);

GLOBALREF void op_get(char op2[], char op3[]);
GLOBALREF void op_put(char op2[], char op3[]);
GLOBALREF void op_rename(char op2[], char op3[]);

GLOBALREF int get_line(char *buf, int len);
GLOBALREF void ftp_connect(char* host, char* user, char* pass);
GLOBALREF void ftp_exit(void);

#endif // FTP_H_INCLUDED
