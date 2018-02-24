#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include "lpc_client.h"
#include "lpc.h"

/* TYPE DEFINITION */
#define		BOOL	unsigned char
#define		SUCCESS	1
#define		ERROR		0
#define		BUFSIZE 	512   

/* GLOBAL VARIABLE */
char str[7][20] = {"FIFO", "character device", "directory", "block device", "regular", "symbolic", "socket"};
static char* default_filename = "/etc/protocols";

/* FUNCTION DECLARATION */
BOOL	test_case0(const char* filename);
BOOL	test_case1(const char *name1, const char *name2);
BOOL	test_case2(const char* filename);
BOOL	test_case3();
void show_file_mode(mode_t file_mode);

/* This is a program to test homework #2 in system software class
 */

int main(void)
{
	int ret;
	char filename[30] = "test_case1.rlt";

	// If you want to initialize the client-side, you can implement "Init" function.
         // Init() is declared in lpc_client.c. 
         Init();

	ret = 0;
	ret += test_case0(default_filename);
	ret += test_case1(default_filename, filename);
	ret += test_case2(default_filename);
	ret += test_case3();

     printf("=========================================================================\n");	
     printf("=========================================================================\n");
     printf("=========================   Number of Success = %d   ====================\n", ret);
     printf("=========================================================================\n");	
     printf("=========================================================================\n");
	return 0;
}

/* test case 0 is very simple function to check the file size using FmReadFile.
 * If someone doesn't understand what this code means, referenced the chapter1 in your presentation file of ?Œí”„?¸ì›¨?´ì‹¤??2 
 */

BOOL	test_case0(const char *filename)
{
	char buffer[BUFSIZE];
	int filedes;
	ssize_t	nread;
	long		total = 0;

	if((filedes = FmOpenFile(filename, O_RDONLY)) == -1)
		return ERROR;

	while((nread = FmReadFile(filedes, buffer, BUFSIZE)) > 0 )
		total += nread;

     printf("======================================================================\n");
     printf("=========================   Test Case 1 Result   =====================\n");
     printf("========================= total length : %10ld   =====================\n", total);
     printf("======================================================================\n");

	return SUCCESS;
}


/* test case 1 is copyfile function. it deals with system calls for regular file functions such as FmOpenFile, FmReadFile, FmWriteFile, Close 
 * If someone doesn't understand what this code means, referenced the chapter1 in your presentation file of ?Œí”„?¸ì›¨?´ì‹¤??2 
 */

BOOL	test_case1(const char *from, const char *to)
{
	int infile;
	int outfile; 
	ssize_t nread; 
	char buffer[BUFSIZE] = {0}; 
	if ( (infile = FmOpenFile(from, O_RDONLY ) )== -1) 
		return ERROR; 

	if((outfile = FmOpenFile(to, O_WRONLY | O_CREAT | O_TRUNC)) == -1)
	{ 
		close(infile);
		return ERROR; 
	}

	while(( nread = FmReadFile(infile, buffer, BUFSIZE)) > 0)
	{
		if( FmWriteFile(outfile, buffer, nread) < nread)
		{
			close(outfile);
			close(infile);
			return ERROR;
		}
	}

	close(outfile);
	close(infile);
	
	if(nread == -1)
		return ERROR;
     printf("======================================================================\n");
     printf("=========================   Test Case 1 is done   =====================\n");
     printf("======================================================================\n");
	return SUCCESS;
} 


/* test case 2 is a function to gather the target file information using FmStatFile.  
 * If someone doesn't understand what this code means, referenced the chapter2 in your presentation file of ?Œí”„?¸ì›¨?´ì‹¤??2 
 */

BOOL test_case2(const char* file_name)
{
    int return_stat;

    struct stat file_info;
    struct passwd *my_passwd;
    struct group  *my_group;

    mode_t file_mode;

    if ((return_stat = FmStatFile(file_name, &file_info)) == -1)
    {
        return ERROR;
    }
     printf("======================================================================\n");
     printf("=========================   Test Case 2 Result   =====================\n");
     printf("======================================================================\n");
    file_mode = file_info.st_mode;
    printf("filename : %s\n", file_name);
    printf("=======================================\n");
    printf("file type : ");
	printf("%s\n", str[(file_mode & S_IFMT)>>13]);

    printf("file size : %d\n", file_info.st_size);
    printf("last read tiem : %d\n", file_info.st_atime);
    printf("last modified time : %d\n", file_info.st_mtime);
    printf("hardlink files : %d\n", file_info.st_nlink);

    show_file_mode(file_mode);
    printf("\n");

    return SUCCESS;

}

/* test case 3 is a function as 'll' command.  
 * If someone doesn't understand what this code means, referenced the chapter2 in your presentation file of ?Œí”„?¸ì›¨?´ì‹¤??2 
   or using 'll' command in your linux shell.
 */

BOOL test_case3()
{
   struct dirent *direntp;
   struct stat file_info;
   DIR *dirp;
   int return_stat;
   mode_t file_mode;
   
   char file_name[30] = {0};

   if ((dirp = (DIR*)FmOpenDir("/root")) == NULL) {
      return ERROR;
   }   
     printf("======================================================================\n");
     printf("=========================   Test Case 3 Result   =====================\n");
     printf("======================================================================\n");
   while ((direntp = (struct dirent*)FmReadDir(dirp)) != NULL)
   {
	sprintf(file_name, "/root/%s",direntp->d_name); 

	if ((FmStatFile(file_name, &file_info)) == -1)
	{
       	return ERROR;
	}

	file_mode = file_info.st_mode;
	show_file_mode(file_mode);
	printf(" %6s  %12s  %10d  %30s  %25s\n", "root", str[(file_mode & S_IFMT)>>13], file_info.st_size, file_name, ctime(&file_info.st_atime) );
		
   }

   while ((FmCloseDir(dirp) == -1) && (errno == EINTR)) ;
   return SUCCESS;

}

void show_file_mode(mode_t file_mode)
{
    if (S_ISDIR(file_mode))
    {
        printf("d");    
    }
    else
        printf("-");
    if (file_mode & S_IRUSR)
    {
        printf("r");
    }
    else
        printf("-");
    if (file_mode & S_IWUSR)
    {
        printf("w");
    }
    else
        printf("-");
    if (file_mode & S_IXUSR)
    {
        printf("x");
    }
    else
        printf("-");
	if (file_mode & S_IRGRP)
    {
        printf("r");
    }
    else
        printf("-");
    if (file_mode & S_IWGRP)
    {
        printf("w");
    }
    else
        printf("-");
    if (file_mode & S_IXGRP)
    {
        printf("x");
    }
    else
        printf("-");

	if (file_mode & S_IROTH)
    {
        printf("r");
    }
    else
        printf("-");
    if (file_mode & S_IWOTH)
    {
        printf("w");
    }
    else
        printf("-");
    if (file_mode & S_IXOTH)
    {
        printf("x");
    }
    else
        printf("-");
}
