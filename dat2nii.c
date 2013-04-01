#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <argp.h>
#include <error.h>

#include <nifti1_io.h>
#include <nifti_util.h>
#include <gray_io.h>

const char *argp_program_version = 
  "dat2nii 1.0";

const char *argp_program_bug_address = 
  "afloren@utexas.edu";

static char doc[] =
  "dat2nii -- A simple program for converting mrGray vAnatomy.dat files to the nifti-1 format.";

static char args_doc[] = 
  "[input_file] [output_file]";

static struct argp_option options[] = {
  {"input", 'i', "input_file", 0, "The vAnatomy.dat file to be converted."},
  {"output", 'o', "output_file", 0, "The nifti file to output."},
  {0}
};

typedef struct {
  char *input_file;
  char *output_file;
} arguments_t;

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  arguments_t *arguments = state->input;

  switch (key) {
  case 'i':
    if(arguments->input_file == NULL) {
      arguments->input_file = arg;
    } else {
      /*already specified input*/
      argp_usage(state);
    }
    break;
  case 'o':
    if(arguments->output_file == NULL) {
      arguments->output_file = arg;
    } else {
      /*already specified output*/
      argp_usage(state);
    }
    break;
  case ARGP_KEY_ARG:
    switch (state->arg_num) {
    case 0:
      if(arguments->input_file == NULL) {
  	arguments->input_file = arg;
      } else if(arguments->output_file == NULL) {
  	arguments->output_file = arg;
      } else {
  	/*already specified input and output*/
  	argp_usage(state);
      }
      break;
    case 1:
      if(arguments->output_file == NULL) {
  	arguments->output_file = arg;
      } else {
  	/*already specified output*/
  	argp_usage(state);
      }
      break;
    default:
      /*too many arguments*/
      argp_usage(state);
      break;
    }
    break;
  case ARGP_KEY_END:
    if(arguments->input_file == NULL || arguments->output_file == NULL) {
      /*must specify both input and output files*/
      argp_usage(state);
    }
    break;
  default:
    return ARGP_ERR_UNKNOWN;
    break;
  }
  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char *argv[])
{
  arguments_t args;
  gray_dat *gda;
  nifti_image *nim;

  args.input_file = NULL;
  args.output_file = NULL;

  argp_parse(&argp, argc, argv, 0, 0, &args);

  gda = gray_dat_read(args.input_file,true);

  if(gda == NULL) {
    error(EXIT_FAILURE,errno,"Couldn't open file \"%s\"",args.input_file);
  }

  int dims[8];
  dims[0] = 4;
  dims[1] = gda->header.rows;
  dims[2] = gda->header.cols;
  dims[3] = gda->header.planes;
  dims[4] = 1;

  nim = nifti_make_new_nim(dims,DT_UINT8,true);

  memcpy(nim->data,gda->data,dims[1]*dims[2]*dims[3]);

  nim->xyz_units = NIFTI_UNITS_MM;
  nim->dx = gda->header.row_dim;
  nim->dy = gda->header.col_dim;
  nim->dz = gda->header.plane_dim;

  nim->qform_code = NIFTI_XFORM_SCANNER_ANAT;
  set_nifti_orientation(nim,PIL);

  nifti_set_filenames(nim,args.output_file,false,true);

  nifti_image_write(nim);

  nifti_image_free(nim);

  gray_dat_free(gda);

  return 0;
}
