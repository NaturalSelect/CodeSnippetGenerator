#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static void ConsumeLine(size_t tableSpaces,char *line)
{
    size_t size = strlen(line);
    size_t spaceCount = 0;
    putchar('\"');
    for(size_t i = 0;i != size;++i)
    {
        if(line[i] == '\"')
        {
            fputs("\\\"",stdout);
        }
        else if(line[i] == '\t')
        {
            fputs("\\t",stdout);
        }
        else if(line[i] != '\r' && line[i] != '\n')
        {
            if(line[i] == ' ')
            {
                spaceCount += 1;
            }
            else
            {
                while(spaceCount >= tableSpaces)
                {
                    fputs("\\t",stdout);
                    spaceCount -= tableSpaces;
                }
                for(size_t i = 0;i != spaceCount;++i)
                {
                    putchar(' ');
                }
                spaceCount = 0;
                putchar(line[i]);
            }
        }
    }
    while(spaceCount >= tableSpaces)
    {
        putchar('\t');
        spaceCount -= tableSpaces;
    }
    for(size_t i = 0;i != spaceCount;++i)
    {
        putchar(' ');
    }
    putchar('\"');
}

int main(int argc,char const *argv[])
{
    if(argc < 3)
    {
        fputs("usgae:\n"
              "<name> <path> - generate code snippet\n"
              "<name> <path> <begin line> <end line> - generate code snippet [begin,end]",stderr);
        return -1;
    }
    const char *name = argv[1];
    const char *path = argv[2];
    size_t beginLine = 1;
    size_t endLine = 0;
    size_t tableSpaces = 4;
    if(argc > 4)
    {
        sscanf(argv[3],"%zu",&beginLine);
        sscanf(argv[4],"%zu",&endLine);
    }
    FILE *fp = NULL;
    char *buf = NULL;
    int exitCode = 0;
    size_t bufSize = 4096;
    do
    {
        buf = calloc(bufSize,sizeof(*buf));
        if(!buf)
        {
            fputs("cannot allocate buffer",stderr);
            exitCode = 1;
            break;
        }
        fp = fopen(path,"r");
        if(!fp)
        {
            fprintf(stderr,"cannnot open file %s\n",path);
            exitCode = 1;
            break;
        }
        //skip lines
        for(size_t i = 1; i != beginLine && !feof(fp); ++i)
        {
            char *line = fgets(buf,(int)bufSize,fp);
            if(!line && ferror(fp))
            {
                fprintf(stderr,"line %zu too long",beginLine);
                exitCode = -1;
                break;
            }
        }
        if(exitCode)
        {
            break;
        }
        if(feof(fp))
        {
            fputs("end of file",stderr);
            exitCode = -1;
            break;
        }
        printf("\"%s\": {\n",name);
        printf("\t\"prefix\":\"%s\",\n",name);
        fputs("\t\"body\": [",stdout);
        if(endLine)
        {
            putchar('\n');
            for(size_t i = beginLine,end = endLine + 1; i != end && !feof(fp); ++i)
            {
                fputs("\t\t",stdout);
                char *line = fgets(buf,(int)bufSize,fp);
                if(!line && ferror(fp))
                {
                    fprintf(stderr,"line %zu too long",i);
                    exitCode = -1;
                    break;
                }
                if(!feof(fp))
                {
                    ConsumeLine(tableSpaces,buf);
                }
                if(i != endLine)
                {
                    puts(",");
                }
            }
            if(exitCode)
            {
                break;
            }
        }
        else
        {
            char *line = fgets(buf,(int)bufSize,fp);
            if(!line && ferror(fp))
            {
                fprintf(stderr,"line %zu too long",beginLine + 1);
                exitCode = -1;
                break;
            }
            size_t i = 1;
            while(!feof(fp))
            {
                fputs("\n\t\t",stdout);
                ConsumeLine(tableSpaces,buf);
                i += 1;
                line = fgets(buf,(int)bufSize,fp);
                if(!line && ferror(fp))
                {
                    fprintf(stderr,"line %zu too long",beginLine + i);
                    exitCode = -1;
                    break;
                }
            }
            if(exitCode)
            {
                break;
            }
        }
        puts("\n\t],");
        puts("\tdescription: \"\"");
        puts("}");
    } while(0);
    if(fp)
    {
        fclose(fp);
    }
    if(buf)
    {
        free(buf);
    }
    return exitCode;
}