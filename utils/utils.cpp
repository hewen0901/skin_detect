#include "utils.h"

#include <iostream>
#include <string>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "dirent.h"
#include <sys/time.h>
#include <unistd.h>

using namespace std;

bool compare(int a, int b)
{
    return a>b;
}

double getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000.0;
}

int get_dir(const char *path,std::vector<std::string> &filename_vec)
{
    DIR *dir = opendir(path);
    if(dir == NULL)
    {
        //printf("opendir fail:%s\n",path);
        return -1;
    }
    struct dirent *p = NULL;
    struct stat buf;
    int res = -1;
    char sub_name[200] = {0};
    while(1)
    {
        p = readdir(dir);
        if(p == NULL)
        {
            break;
            if(errno != 0)
            {
                //perror("readdir fail");
                return -1;
            }
        }
        sprintf(sub_name,"%s/%s",path,p->d_name);
        res = stat(sub_name,&buf);
        if(res == -1)
        {
            //perror("stat error");
            return -1;
        }
        if(S_ISDIR(buf.st_mode))
        {
            //printf("name:%s\n",p->d_name);
            if(strcmp(p->d_name,".") ==0 || strcmp(p->d_name,"..") == 0)
                continue;

            get_dir(sub_name,filename_vec);
        }
        if(S_ISREG(buf.st_mode))
        {
            //printf("%s\n",p->d_name);
            //filename_vec.push_back(p->d_name);
            //printf("%s\n",sub_name);
            filename_vec.push_back(sub_name);
        }
    }
    return 0;
}
