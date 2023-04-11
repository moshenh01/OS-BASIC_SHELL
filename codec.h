
void encode(char *messege);
void decode(char *messege);

typedef void(*encode_func)(char*);
typedef void(*decode_func)(char*);