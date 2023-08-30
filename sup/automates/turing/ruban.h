typedef enum {
    SPACE=10, ERROR=255, REJECT=254, ACCEPT=253
} codes_t;

void init(int tape[], int length);

void left(void);
void right(void);

int read(void);
void write(int data);

void show_tape(int imin, int imax);