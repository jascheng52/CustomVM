#include <stdlib.h>
//the name/size for data. Then name is stored in data null terminated a
//and then following is the rest of the data
typedef struct data{
    size_t dataNameSize;
    size_t dataSize;
    char   data[];
}DATA_STRUCT;

typedef struct label{
    size_t labelSize;
    char   label[];
}LABEL_STRUCT;


DATA_STRUCT *mallocData(int dataNameSize, int dataSize);
size_t dataStructSize(DATA_STRUCT *dataStruct);
