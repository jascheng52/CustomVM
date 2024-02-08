#define EXTEN_CMP 10
#define MAX_LINE_LENGTH 1024


//the name/size for data. Then name is stored in data null terminated a
//and then following is the rest of the data

typedef struct data{
    size_t dataNameSize;
    size_t dataSize;
    char   data[];
}DATA_STRUCT;



int assemble(char *file);
int checkExtension(char *file);
int createFile(char *file, char *buffer, size_t size);
int parseFile(FILE *openFile, char *buffer);
int parseLine(char *lineBuffer, char *currLine, int *instrNum);
int getNextToken(char *lineBuffer);
char *skipWhite(char *textLine);

