#ifndef __LPC_H__

#include <sys/types.h>
#include <unistd.h> 


// definition of lpc parameters
#define LPC_DATA_MAX		(512)
#define LPC_ARG_MAX			(10)


// declaration of lpc types
typedef enum __lpcService
{
	LPC_OPEN_FILE = 0,
	LPC_READ_FILE,
	LPC_WRITE_FILE,
	LPC_CLOSE_FILE,
	LPC_STAT,
	LPC_OPEN_DIR,
	LPC_CLOSE_DIR,
	LPC_READ_DIR
} LpcService;

typedef struct __lpcArgMsg
{
	pid_t	pid;           // message type
	int		argSize;
	char	argData[LPC_DATA_MAX];    
} LpcArgPack;

typedef struct __lpcHdrPack
{
	pid_t 		pid;               // message type
	LpcService 	service;
	int 		numArg;
} LpcHdrPack;

typedef struct __lpcResult
{
	int		errorno;
	int 	resultSize;
	char 	resultData[LPC_DATA_MAX];
} LpcResult;


#endif /* __LPC_H__ */
