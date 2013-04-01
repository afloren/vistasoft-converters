#include <nifti1_io.h>

typedef enum {
  RAS,
  PIL
} orientation;

int set_nifti_orientation(nifti_image* nim, orientation o);
