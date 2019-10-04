/****************************************************************
 * Name        : Romeel Chaudhari                               *
 * Class       : CSC 415                                        *
 * Date        :                                                *
 * Description :  Writting a simple bash shell program          *
 *                that will execute simple commands. The main   *
 *                goal of the assignment is working with        *
 *                fork, pipes and exec system calls.            *
 ****************************************************************/

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_MAX 256
#define PROMPT "MyShell>> "
#define PROMPTSIZE sizeof(PROMPT)

int main(int argc, char** argv)
{
    while (1)
        {
        
        int input = 0;// boolean variable to check the input
        int output1 = 0;// boolean variable to check the output file
        int output2= 0;// boolean variable to check the appending
        char buffer_max[BUFFER_MAX];//buffer
        char *token;  //next token
        int ch_Bgrd = 0; //check for &
        printf(PROMPT);//use input
        if(fgets(buffer_max, 256, stdin) == NULL)
        {
            exit(0);
        }
        token = strtok(buffer_max, " \n\t"); // tokenazies the first token
        char *args1[65];// array to hold commands
        char *args2[65];// array to hold commands
        int argc1 = 0;
        int argc2 = 0;
        char pwd[256]; //present working directory.
        int testPipe = 0;// to check the pipe.
        char *inputf1;
        char *outf1;
        char *outf2;
        
        while (token != NULL)  //tokenizes the input
        {
        if(strcmp(token,"&") == 0)
        {
                ch_Bgrd = 1;
                args1[argc1] = NULL;
                break;
        }
        if(strcmp(token,"|") == 0)
        {
                testPipe = 1;
                args1[argc1] = NULL;
                token = strtok(NULL, " \n\t");
        }
        if(strcmp(token,"<") == 0)
        {
                input = 1;
                args1[argc1] = NULL;
                token = strtok(NULL, " \n\t");
                inputf1 = token;
                break;
        }
        if(strcmp(token,">") == 0)
        {
                output1 = 1;
                args1[argc1] = NULL;
                token = strtok(NULL, " \n\t");
                outf1 = token;
                break;
        }
        if(strcmp(token,">>") == 0)
        {
                output2 = 1;
                args1[argc1] = NULL;
                token = strtok(NULL, " \n\t");
                outf2 = token;
                break;
        }
        if(!testPipe)
        {
                args1[argc1] = token;
                argc1++;
        }
        else
        {
                args2[argc2] = token;
                argc2++;
        }
                token = strtok(NULL, " \n\t");
        }
                args1[argc1] = NULL;
                args2[argc2] = NULL;
        if(strcmp(args1[0], "exit") == 0)
        {
            exit(0);
        }
        else if (strcmp(args1[0], "pwd") == 0)
        {
        if (getcwd(pwd, sizeof(pwd)) == NULL)
        {
            perror("Error getting the path");
        }
        else
        {
            printf("%s ", pwd);
        }
        }
        else if (strcmp(args1[0], "cd") == 0)
        {
            chdir(args1[1]);
        }
        else
        {
        if(testPipe)
        {
            pid_t p1, p2;
            int p[2];
            int returnValue = 0;
            pipe(p);
            p1 = fork();
        if(p1 < 0)
        {
            perror("Error in fork child");
            exit(0);
        }
        else if (p1 == 0)
        {
            dup2(p[1], STDOUT_FILENO);// redirection
            close(p[0]);
            returnValue = execvp(args1[0], args1);
        }
        if(returnValue < 0)
        {
            perror("Error");
            exit(0);
        }
            p2 = fork();
        if(p2 < 0)
        {
            perror("Error in fork child ");
            exit(0);
        }
        else if (p2 == 0)
        {
            dup2(p[0], STDIN_FILENO);
            close(p[1]);
            returnValue = execvp(args2[0], args2);
        if(returnValue < 0)
        {
            perror("Error");
            exit(0);
        }
        }
            close(p[0]);
            close(p[1]);
            int a1, a2;
        if(waitpid(p1, &a1, 0) < 0)
        {
            perror("Error1");
            exit(0);
        }
        if(waitpid(p2, &a2, 0) < 0) //status of child process
        {
            perror("Error2");
            exit(0);
        }
        }
        else
        {
            pid_t pid;
            int inputf =0;
            int outputf1 =0;
            int outputf2 =0;
            int holdInput = 0;
            int holdOutput1 =0;
            int holdOutput2 =0;
            int rValue = 0;
        if(output1)
        {
            holdOutput1 = dup(STDOUT_FILENO);
            outputf1 = open(outf1, O_WRONLY | O_TRUNC | O_CREAT);
            dup2(outputf1, STDOUT_FILENO);
            close(outputf1);
        }
        if(output2)
        {
            holdOutput2 = dup(STDOUT_FILENO);
            outputf2 = open(outf2, O_WRONLY | O_APPEND | O_CREAT);
            dup2(outputf2, STDOUT_FILENO);
            close(outputf2);
        }
        if(input)
        {
            holdInput = dup(STDIN_FILENO);
            inputf = open(inputf1, O_RDONLY);
            dup2(inputf, STDIN_FILENO);
            close(inputf);
        }
            pid = fork();
        if(pid < 0)
        {
            perror("Error in forking child");
            exit(0);
        }
        else if (pid == 0)
        {
            rValue = execvp(args1[0], args1);
        if(rValue < 0)
        {
            perror("Error in Command Line");
        }
            exit(0);
        }
        else
        {
        if(!ch_Bgrd)
        {
            int status;
        if((waitpid(pid, &status, 0)) < 0)
        {
            perror("Error");
            exit(0);
        }
        if(output1)
        {
            dup2(holdOutput1, STDOUT_FILENO);
            close(holdOutput1);
        }
        if(output2)
        {
            dup2(holdOutput2, STDOUT_FILENO);
            close(holdOutput2);
        }
        if(input)
        {
            dup2(holdInput, STDIN_FILENO);
            close(holdInput);
        }
        }
        }
        }
        }
        printf("\n \n");    // new line parse.
        
        }
    
        return 0;
}
