
#define EXTEN_CMP 10
#define MAX_LINE_LENGTH 1024


int assemble(char *file);
int checkExtension(char *file);
int createFile(char *file, char *buffer, size_t size);
int parseFile(FILE *openFile, char *buffer);
int parseLine(char *lineBuffer, char *currLine, int *instrNum);
int getNextToken(char *lineBuffer);
char *skipWhite(char *textLine);
void init();

void debug(void *buffer, size_t num);
