#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    
    if(argc >= 2)
    {
        if(strcmp(argv[1], "read") == 0)
        {
            int fid = open(argv[2], O_RDONLY);
            if(fid < 0)
            {
                printf("cant open file %s\n", argv[2]);
                return 1;
            }
            
            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            int n = read(fid, buffer, sizeof(buffer)-1);
            if(n > 0)
            {
                printf("%s\n", buffer);
                return 0;
            }
            else
            {
                printf("failed reading file\n");
                return 1;
            }
            
            close(fid);
        }
        else if(strcmp(argv[1], "write") == 0)
        {
            if(argc >= 3)
            {
                int fid = open(argv[2], O_WRONLY);
                if(fid < 0)
                {
                    printf("cant open file %s\n", argv[2]);
                    return 1;
                }
               
                int n = strlen(argv[3]);
                n = write(fid, argv[3], n);
                if(n > 0)
                {
                    return 0;
                }
                else
                {
                    printf("failed writing file\n");
                    return 1;
                }
                
                close(fid);
            }
            else
            {
                printf("missing write argument\n");
            }
        }
        
    }
    
    printf("test (read | write) <file> [<argument>]\n");
    
    return 0;
}

