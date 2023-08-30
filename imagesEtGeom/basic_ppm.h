typedef enum {
    black, red, green, yellow, blue, magenta, cyan, white, gray
} color;

int init(int nbl, int nbc, color c);

void set_pixel_color(int i, int j, color c);

int nbl(void);

int nbc(void);

void set_pixel_rgb(int i, int j, int r, int g, int b);

int write(char* filename);
