#include "ftplib.h"
#include "ftp.h"
#include <unistd.h>
#include <string.h>

#include "hdu_log.h"

/* exit values */
#define EX_SYNTAX 2 	/* command syntax errors */
#define EX_NETDB 3	/* network database errors */
#define EX_CONNECT 4	/* network connect errors */
#define EX_LOGIN 5	/* remote login errors */
#define EX_REMCMD 6	/* remote command errors */
#define EX_SYSERR 7	/* system call errors */

#define FTP_SEND 1	/* send files */
#define FTP_GET 2	/* retreive files */
#define FTP_DIR 3	/* verbose directory */
#define FTP_RM 4	/* delete files */
#define FTP_LIST 5	/* terse directory */

#define DIRBUF_SIZE 1024 /* for wildcard processing */

extern FILE* debug_fp;         //define in main.c
extern char debug_time[25];

extern char pwd[DIRBUF_SIZE]; //define in main.c

static int logged_in = 0;
netbuf* conn = NULL;

GLOBALREF void op_help(void)
{
    printf("--------------------------------------------------\n");
    printf("help/h                        -- help info\n");
    printf("pwd                           -- current director\n");
    printf("sys                           -- ftp server system\n");
    printf("ls/ll                         -- show file info\n");
    printf("cd dir                        -- cd dirc\n");
    printf("mkdir <dirc>                  -- make dirc\n");
    printf("rmdir <dirc>                  -- remove dirc\n");
    printf("get <outputfile> <path>       -- get file\n");
    printf("put <inputfile> <path>        -- put file\n");
    printf("rename <src_name> <dest_name> -- put file\n");
    printf("---------------------------------------------------\n");
}

GLOBALREF void op_pwd(void)
{
    printf("%s\n", pwd);
}

GLOBALREF void op_sys(void)
{
    char buf[DIRBUF_SIZE];

    if (!FtpSysType(buf, sizeof(buf), conn))
    {
        DEBUG_ERR("FtpSysType");
        return;
    }
    printf("%s\n", buf);
}

GLOBALREF void op_ls(char op2[])
{
    if (!strcmp(op2, ""))
    {
        if (!FtpNlst(NULL, pwd, conn))
        {
            DEBUG_ERR("FtpNlst");
            return;
        }
    }
    else
    {
        if (!FtpNlst(NULL, op2, conn))
        {
            DEBUG_ERR("FtpNlst");
            return;
        }
    }
}

GLOBALREF void op_ll(char op2[])
{
    if (!strcmp(op2, ""))
    {
        if (!FtpDir(NULL, pwd, conn))
        {
            DEBUG_ERR("FtpDir");
            return;
        }
    }
    else
    {
        if (!FtpDir(NULL, op2, conn))
        {
            DEBUG_ERR("FtpDir");
            return;
        }
    }
}

GLOBALREF void op_cd(char op2[])
{
    if (!strcmp(op2, ""))
        return;

    if (!strcmp(op2, ".."))
    {
        if (!strcmp(pwd, "/"))
        {
            printf("the pwd is /\n");
            return;
        }
        else if (!FtpCDUp(conn))
        {
            DEBUG_ERR("FtpCDUp");
            return;
        }
    }
    else if (!FtpChdir(op2, conn))
    {
        DEBUG_ERR("FtpChdir");
        return;
    }

    // update pwd
    if (!FtpPwd(pwd, sizeof(pwd), conn))
    {
        fprintf(stderr, "FtpPwd pwd error");
        FtpQuit(conn);
    }
}

GLOBALREF void op_mkdir(char op2[])
{
    if (!strcmp(op2, ""))
    {
        printf("usage: mkdir <dirc>\n");
        return;
    }
    if (!FtpMkdir(op2, conn))
    {
        DEBUG_ERR("FtpMkdir");
        return;
    }

    char buf[DIRBUF_SIZE] = "create dirc ";
    strcat(buf, op2);
    DEBUG_INFO(buf);
}

GLOBALREF void op_rmdir(char op2[])
{
    if (!strcmp(op2, ""))
    {
        printf("usage: rmdir <dirc>\n");
        return;
    }
    if (!FtpRmdir(op2, conn))
    {
        DEBUG_ERR("FtpRmdir");
        return;
    }

    char buf[DIRBUF_SIZE] = "delete dirc ";
    strcat(buf, op2);
    DEBUG_INFO(buf);
}

GLOBALREF void op_get(char op2[], char op3[])
{
    if (!strcmp(op2, "") || !strcmp(op3, ""))
    {
        printf("usage: get <outputfile> <path>\n");
        return;
    }

    if (!strcmp(op2, "null") || !strcmp(op2, "NULL"))
    {
        if (!FtpGet(NULL, op3, FTPLIB_ASCII, conn))
        {
            DEBUG_ERR("FtpGet");
            return;
        }
    }
    else
    {
        if (!FtpGet(op2, op3, FTPLIB_ASCII, conn))
        {
            DEBUG_ERR("FtpGet");
            return;
        }
        printf("get %s is ok, save to %s\n", op3, op2);
    }

    char buf[DIRBUF_SIZE] = "get file ";
    strcat(buf, op3);
    DEBUG_INFO(buf);
}

GLOBALREF void op_put(char op2[], char op3[])
{
    if (!strcmp(op2, "") || !strcmp(op3, ""))
    {
        printf("usage: put <inputfile> <path>\n");
        return;
    }
    if (!FtpPut(op2, op3, FTPLIB_ASCII, conn))
    {
        DEBUG_ERR("FtpPut");
        return;
    }
    printf("put %s is ok, save to %s\n", op2, op3);

    char buf[DIRBUF_SIZE] = "put file ";
    strcat(buf, op3);
    DEBUG_INFO(buf);
}

GLOBALREF void op_rename(char op2[], char op3[])
{
    if (!strcmp(op2, "") || !strcmp(op3, ""))
    {
        printf("usage: rename <src_name> <dest_name>\n");
        return;
    }
    if (!FtpRename(op2, op3, conn))
    {
        DEBUG_ERR("FtpRename");
        exit(1);
    }
    printf("rename %s to %s ok\n", op2, op3);

    char buf[DIRBUF_SIZE] = "rename file ";
    strcat(buf, op3);
    DEBUG_INFO(buf);
}

GLOBALREF int get_line(char *buf, int len)
{
    int c, i;

    i = 0;
    while (--len > 0 && (c=getchar()) != EOF && c != '\n')
        buf[i++] = c;
    buf[i] = '\0';
    return i;
}

GLOBALREF void ftp_connect(char* host, char* user, char* pass)
{
    if (conn)
        return;
    if (!logged_in)
    {
        if (user == NULL)
        {
            printf("User: ");
            scanf("%s", user);
        }
        if (pass == NULL)
        {
            if((pass = getpass("Passwd: ")) == NULL)
                exit(EX_SYSERR);
        }

        if (!FtpConnect(host, &conn))
        {
            fprintf(stderr, "Unable to connect to node %s\n", host);
            exit(EX_CONNECT);
        }
        if (!FtpLogin(user, pass, conn))
        {
            fprintf(stderr, "Login failure\n%s", FtpLastResponse(conn));
            exit(EX_LOGIN);
        }
        logged_in++;

        /* get the current director, assign to pwd */
        if (!FtpPwd(pwd, sizeof(pwd), conn))
        {
            fprintf(stderr, "FtpPwd pwd error");
            FtpQuit(conn);
        }
    }
}

GLOBALREF void ftp_exit(void)
{
    if(conn)
        FtpQuit(conn);
}
