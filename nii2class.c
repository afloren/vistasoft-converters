#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <argp.h>
#include <error.h>

#include <nifti1_io.h>
#include <nifti_util.h>
#include <gray_io.h>


const char *argp_program_version = 
  "nii2class 1.0";

const char *argp_program_bug_address = 
  "afloren@utexas.edu";

static char doc[] =
  "nii2class -- A simple program for converting nifti-1 format files to mrGray .Class files. "
  "This program assumes the orientation of the nifti file is PIL "
  "(the only orientation supported by .Class files).";

static char args_doc[] = 
  "[input_file] [output_file]";

static struct argp_option options[] = {
  {"input", 'i', "input_file", 0, "The nifti file to be converted."},
  {"output", 'o', "output_file", 0, "The .Class file to output."},
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

int main(int argc, char* argv[])
{
  arguments_t args;
  nifti_image* nim;
  gray_class* gcl;

  args.input_file = NULL;
  args.output_file = NULL;

  argp_parse(&argp, argc, argv, 0, 0, &args);

  nim = nifti_image_read(args.input_file,true);
  if(nim == NULL) {
    error(EXIT_FAILURE,errno,"Couldn't open file \"%s\"",args.input_file);
  }

  //should perform some sanity checks on shape/type of nifti image

  gcl = gray_class_new(nim->nx,nim->ny,nim->nz,true);
  if(gcl == NULL) {
    error(EXIT_FAILURE,errno,"Couldn't allocate new class struct");
  }

  memcpy(gcl->data,nim->data,nim->nx*nim->ny*nim->nz);

  //should recalculate stats for header

  if(gray_class_write(gcl,args.output_file) == -1) {
    error(EXIT_FAILURE,errno,"Couldn't write to file \"%s\"",args.output_file);
  }

  gray_class_free(gcl);

  nifti_image_free(nim);

  return 0;
}
