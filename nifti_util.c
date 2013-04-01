#include "nifti_util.h"

int set_nifti_orientation(nifti_image* nim, orientation o)
{
  switch(o) {
  case RAS:
    nim->quatern_b = 0.0;
    nim->quatern_c = 0.0;
    nim->quatern_d = 0.0;
    nim->qfac = 1.0;
    break;
  case PIL:
    nim->quatern_b = -0.5;
    nim->quatern_c = 0.5;
    nim->quatern_d = -0.5;
    nim->qfac = -1.0;
    break;
  default:
    //unknown orientation code
    break;
  }
   
  return 0;
}
