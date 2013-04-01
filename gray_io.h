#define GRAY_CLASS_VERSION_MAJOR 2
#define GRAY_CLASS_VERSION_MINOR 1
#define GRAY_CLASS_UNKNOWN 0
#define GRAY_CLASS_WHITE 16
#define GRAY_CLASS_GRAY 32
#define GRAY_CLASS_CSF 48

typedef unsigned char gray_class_data;

typedef struct {
  int version;
  int minor;
  int voi_xmin;
  int voi_xmax;
  int voi_ymin;
  int voi_ymax;
  int voi_zmin;
  int voi_zmax;
  int xsize;
  int ysize;
  int zsize;
  float csf_mean;
  float gray_mean;
  float white_mean;
  float stdev;
  float confidence;
  float smoothness;
} gray_class_header;

typedef struct {
  gray_class_header header;
  gray_class_data* data;
} gray_class;

gray_class* gray_class_new(int xsize, int ysize, int zsize, int alloc_data);
gray_class* gray_class_read(char* file_name, int read_data);
int gray_class_write(gray_class* gcl, char* file_name);
int gray_class_free(gray_class* gcl);

typedef unsigned char gray_dat_data;

typedef struct {
  int rows;
  int cols;
  int planes;
  float row_dim;
  float col_dim;
  float plane_dim;
} gray_dat_header;

typedef struct {
  gray_dat_header header;
  gray_dat_data* data;
} gray_dat;

gray_dat* gray_dat_new(int rows, int cols, int planes,  int alloc_data);
gray_dat* gray_dat_read(char* file_name, int read_data);
int gray_dat_write(gray_dat* gda, char* file_name);
int gray_dat_free(gray_dat* gda);
