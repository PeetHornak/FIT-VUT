#include <string.h>
#include <student/parseArguments.h>
#include <tests/groundTruth.h>

/**
 * @brief This function parses command line arguments
 *
 * @param args arguments 
 * @param argc number of command line arguments
 * @param argv[] command line arguments
 */
void arguments_parse(Arguments*const args,int argc,char*argv[]){
  args->runConformanceTests = (argc>1 && strcmp(argv[1],"-c") == 0);
  args->runPerformanceTests = (argc>1 && strcmp(argv[1],"-p") == 0);
  args->takeScreenShot      = (argc>1 && strcmp(argv[1],"-s") == 0);
  if(argc>2)
    groundTruthFile = argv[2];
}
