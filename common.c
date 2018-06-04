#include "common.h"

int myfprintf(FILE* fp,const char* fmt,...)
{
    char temp[256];
    va_list vaa;
    va_start(vaa,fmt);
    memset(temp,0,256);
    vsprintf(temp,fmt,vaa);
    fwrite(temp,strlen(temp),1,fp);
    va_end(vaa);

    return 0;
}

unsigned int mygetticks()
{
    unsigned int ticks=svcGetSystemTick()/19200.0;
    return ticks;
}

char* myfgets(char* buf,int maxcount,FILE* fp)
{
    int offset=fseek(fp,0,SEEK_CUR);
    if(fseek(fp,0,SEEK_END)==offset)
    {
        return NULL;
    }
    fseek(fp,offset,SEEK_SET);

    for(int i=0;i<maxcount;i++)
    {
        char c;
        fread(&c,1,1,fp);
        buf[i]=c;

        if(c=='\n')   //\n
        {
            buf[i]='\0';
            break;
        }     
    }
    return buf;
}