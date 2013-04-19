#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <error.h>

#include "gray_io.h"

gray_class* gray_class_new(int xsize, int ysize, int zsize, int alloc_data)
{
  gray_class* gcl;

  gcl = malloc(sizeof(gray_class));
  if(gcl == NULL) {
    return NULL;
  }
  memset(gcl,0,sizeof(gray_class));

  gcl->header.version = GRAY_CLASS_VERSION_MAJOR;
  gcl->header.minor = GRAY_CLASS_VERSION_MINOR;
  gcl->header.voi_xmin = 0;
  gcl->header.voi_xmax = (xsize-1);
  gcl->header.voi_ymin = 0;
  gcl->header.voi_ymax = (ysize-1);
  gcl->header.voi_zmin = 0;
  gcl->header.voi_zmax = (zsize-1);
  gcl->header.xsize = xsize;
  gcl->header.ysize = ysize;
  gcl->header.zsize = zsize;

  if(alloc_data) {
    size_t data_size;
    data_size = xsize * ysize * zsize;
    gcl->data = malloc(data_size);
    if(gcl->data == NULL) {
      free(gcl);
      return NULL;
    }
    memset(gcl->data,0,data_size);
  } else {
    gcl->data = NULL;
  }

  return gcl;
}

gray_class* gray_class_read(char* file_name, int read_data)
{
  gray_class* gcl;
  FILE* fp;
  int params = 0;
  
  gcl = malloc(sizeof(gray_class));
  if(gcl == NULL) {
    return NULL;
  }
  memset(gcl,0,sizeof(gray_class));

  fp = fopen(file_name,"rb");
  if(fp == NULL) {
    free(gcl);
    return NULL;
  }

  params += fscanf(fp,"version=%d\n",&gcl->header.version);
  params += fscanf(fp,"minor=%d\n",&gcl->header.minor);
  params += fscanf(fp,"voi_xmin=%d\n",&gcl->header.voi_xmin);
  params += fscanf(fp,"voi_xmax=%d\n",&gcl->header.voi_xmax);
  params += fscanf(fp,"voi_ymin=%d\n",&gcl->header.voi_ymin);
  params += fscanf(fp,"voi_ymax=%d\n",&gcl->header.voi_ymax);
  params += fscanf(fp,"voi_zmin=%d\n",&gcl->header.voi_zmin);
  params += fscanf(fp,"voi_zmax=%d\n",&gcl->header.voi_zmax);
  params += fscanf(fp,"xsize=%d\n",&gcl->header.xsize);
  params += fscanf(fp,"ysize=%d\n",&gcl->header.ysize);
  params += fscanf(fp,"zsize=%d\n",&gcl->header.zsize);
  params += fscanf(fp,"csf_mean=%g\n",&gcl->header.csf_mean);
  params += fscanf(fp,"gray_mean=%g\n",&gcl->header.gray_mean);
  params += fscanf(fp,"white_mean=%g\n",&gcl->header.white_mean);
  params += fscanf(fp,"stdev=%g\n",&gcl->header.stdev);
  params += fscanf(fp,"confidence=%g\n",&gcl->header.confidence);
  params += fscanf(fp,"smoothness=%g\n",&gcl->header.smoothness);
  if(params != 17) {
    error(0,errno,"Failed to read parameters from header");
    free(gcl);
    return NULL;
  }

  if(read_data) {
    size_t data_size;
    size_t read_size;
    data_size = sizeof(gray_class_data) * gcl->header.xsize * gcl->header.ysize * gcl->header.zsize;
    gcl->data = malloc(data_size);
    if(gcl->data = NULL) {
      free(gcl);
      return NULL;
    }
    read_size = fread(gcl->data,data_size,1,fp);
    if(read_size != 1) {
      error(0,errno,"Unexpected end of file");
      free(gcl);
      return NULL;
    }
  } else {
    gcl->data = NULL;
  }

  fclose(fp);
  return gcl;  
}

int gray_class_write(gray_class* gcl, char* file_name)
{
  FILE* fp;
  size_t data_size;

  if(gcl->data == NULL) {
    return 1;//if class instance has no data then we can't write it...
  }

  fp = fopen(file_name,"wb");
  if(fp == NULL) {
    return 1;
  }
  fprintf(fp,"version=%d\n",gcl->header.version);
  fprintf(fp,"minor=%d\n",gcl->header.minor);
  fprintf(fp,"voi_xmin=%d\n",gcl->header.voi_xmin);
  fprintf(fp,"voi_xmax=%d\n",gcl->header.voi_xmax);
  fprintf(fp,"voi_ymin=%d\n",gcl->header.voi_ymin);
  fprintf(fp,"voi_ymax=%d\n",gcl->header.voi_ymax);
  fprintf(fp,"voi_zmin=%d\n",gcl->header.voi_zmin);
  fprintf(fp,"voi_zmax=%d\n",gcl->header.voi_zmax);
  fprintf(fp,"xsize=%d\n",gcl->header.xsize);
  fprintf(fp,"ysize=%d\n",gcl->header.ysize);
  fprintf(fp,"zsize=%d\n",gcl->header.zsize);
  fprintf(fp,"csf_mean=%g\n",gcl->header.csf_mean);
  fprintf(fp,"gray_mean=%g\n",gcl->header.gray_mean);
  fprintf(fp,"white_mean=%g\n",gcl->header.white_mean);
  fprintf(fp,"stdev=%g\n",gcl->header.stdev);
  fprintf(fp,"confidence=%g\n",gcl->header.confidence);
  fprintf(fp,"smoothness=%g\n",gcl->header.smoothness);
  data_size = sizeof(gray_class_data) * gcl->header.xsize * gcl->header.ysize * gcl->header.zsize;
  fwrite(gcl->data,data_size,1,fp);
  fclose(fp);

  return 0;
}

int gray_class_free(gray_class* gcl)
{
  if(gcl->data != NULL) {
    free(gcl->data);
    gcl->data = NULL;
  }
  free(gcl);

  return 0;
}

gray_dat* gray_dat_new(int rows, int cols, int planes, int alloc_data)
{
  gray_dat* gda;

  gda = malloc(sizeof(gray_dat));
  if(gda == NULL) {
    return NULL;
  }
  memset(gda,0,sizeof(gray_dat));

  gda->header.rows = rows;
  gda->header.cols = cols;
  gda->header.planes = planes;
  gda->header.row_dim = 1.0;
  gda->header.col_dim = 1.0;
  gda->header.plane_dim = 1.0;

  if(alloc_data) {
    size_t data_size = rows * cols * planes;
    gda->data = malloc(data_size);
    if(gda->data = NULL) {
      free(gda);
      return NULL;
    }
    memset(gda->data,0,data_size);
  } else {
    gda->data = NULL;
  }

  return gda;
}

gray_dat* gray_dat_read(char* file_name, int read_data)
{
  gray_dat* gda;
  FILE* fp;
  float rows,cols,planes;
  int params = 0;

  gda = malloc(sizeof(gray_dat));
  if(gda == NULL) {
    return NULL;
  }
  fp = fopen(file_name,"rb");
  if(fp == NULL) {
    return NULL;
  }

  params += fscanf(fp,"rows \t%g (%g mm/pixel)\n",&rows,&gda->header.row_dim);
  gda->header.rows = rows;
  params += fscanf(fp,"cols \t%g (%g mm/pixel)\n",&cols,&gda->header.col_dim);
  gda->header.cols = cols;
  params += fscanf(fp,"planes \t%g (%g mm/pixel)\n",&planes,&gda->header.plane_dim);
  gda->header.planes = planes;
  params += fscanf(fp,"*\n");

  if(params != 6) {
    error(0,errno,"Failed to read parameters from header");
    free(gda);
    return NULL;
  }

  if(read_data) {
    size_t data_size;
    size_t read_size;
    data_size = sizeof(gray_dat_data) * gda->header.rows * gda->header.cols * gda->header.planes;
    gda->data = malloc(data_size);
    if(gda->data == NULL) {
      free(gda);
      return NULL;
    }
    read_size = fread(gda->data,data_size,1,fp);
    if(read_size != 1) {
      error(0,errno,"Unexpected end of file");
      free(gda);
      return NULL;
    }
  } else {
    gda->data = NULL;
  }
  fclose(fp);

  return gda;
}

int gray_dat_write(gray_dat* gda, char* file_name)
{
  FILE* fp;
  size_t data_size;

  if(gda->data == NULL) {
    return 1;//if dat instance has no data then we can't write it...
  }

  fp = fopen(file_name,"wb");
  if(fp == NULL) {
    return 1;
  }
  fprintf(fp,"rows \t%#f (%#f mm/pixel)\n",(float)gda->header.rows,gda->header.row_dim);
  fprintf(fp,"cols \t%#f (%#f mm/pixel)\n",(float)gda->header.cols,gda->header.col_dim);
  fprintf(fp,"planes \t%#f (%#f mm/pixel)\n",(float)gda->header.planes,gda->header.plane_dim);
  fprintf(fp,"*\n");
  data_size = sizeof(gray_dat_data) * gda->header.rows * gda->header.cols * gda->header.planes;
  fwrite(gda->data,data_size,1,fp);
  fclose(fp);

  return 0;
}

int gray_dat_free(gray_dat* gda)
{
  if(gda->data != NULL) {
    free(gda->data);
  }
  free(gda);

  return 0;
}
