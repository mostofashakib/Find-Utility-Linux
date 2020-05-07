#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

static int dopath();
static int namepath();
static int nodepath();
static int modepath();

int main(int argc, char **argv){
	char actualpath [PATH_MAX+1];
	char * resolved = realpath(argv[1],actualpath);

	if (argc == 2){         // checks to see if the argument passed is for the first case.
		dopath(resolved);
	} 

	++argv; // moves argv to the index 1 argument

	while(*++argv != NULL && **argv == '-'){ 
		char *temp = *argv;

		if (strncmp(temp, "-name", 4) == 0 ){
			char *pattern = *++argv;
			namepath(resolved, pattern);
		}
		else if (strncmp(temp, "-mmin", 4) == 0 ){
			char temp_mode = **(++argv);

			switch(temp_mode){
				case '+':
				printf("This is the plus condition\n");
				break;
				case '-':
				printf("This is the minus condition\n");
				break;
				default:
				printf("This is the default condition\n");
				printf("%c\n", temp_mode);
				break;
			}
		}
		else if (strncmp(temp, "-inum", 4) == 0 ){
			nodepath(resolved, atoi(*++argv));
		}
	}

	exit(0);
}

char * full_path;

static int dopath(char * sub_dir) {
	struct stat     statbuf;
	struct dirent   *sub_dirp;
	DIR  *sub_dp = opendir(sub_dir);   //open the parent

	if ( lstat(sub_dir, &statbuf) < 0 ){
		printf("%s\n", sub_dir);
	}


  if(sub_dp!=NULL) {
       while((sub_dirp = readdir(sub_dp) )!= NULL) {    //read the files in the parent
        char * temp = sub_dirp->d_name;                 // print names of files in the parent
        char temp1[2048]= ".\0";
        char temp2[2048]= "..\0";

	 //recurcively loop into the sub-directory

        if( (strncmp(temp,temp1, 3)!= 0) && (strncmp(temp,temp2, 3) != 0) ) {
        	char temp3[2048] = "/";
        	char *temp_sub = temp3;
        	temp_sub = strcat(temp_sub,temp);
        	printf("./%s\n", sub_dirp->d_name);
        	char * temp_full_path = calloc(2048, sizeof(unsigned char) );
        	temp_full_path = strcpy(temp_full_path,sub_dir);
        	strcat(temp_full_path,temp_sub);
        	DIR * subsubdp = opendir(temp_full_path);
        	if(subsubdp != NULL){
        		closedir(subsubdp);
        		dopath(temp_full_path);   // recursively calls the dopath function
        	}
        }
        else{
        	continue;
        }
    }
       closedir(sub_dp);    // closes dir
   }
    else
    {
        printf("cannot open directory\n");
        exit(2);
    }
}



char * full_path;

static int namepath(char * sub_dir, char * pattern){
	struct stat     statbuf;
	struct dirent   *sub_dirp;
	DIR  *sub_dp = opendir(sub_dir);         //open the parent

	if ( lstat(sub_dir, &statbuf) < 0 ){
		printf("%s\n", sub_dir);
	}


  if(sub_dp!=NULL) {
       while((sub_dirp = readdir(sub_dp) )!= NULL) {              // read the files in the parent
        char * temp = sub_dirp->d_name;                           // print names of files in the parent
        char temp1[2048]= ".\0";
        char temp2[2048]= "..\0";

	 //recurcively loop into the sub-directory

        if( (strncmp(temp,temp1, 3)!= 0) && (strncmp(temp,temp2, 3) != 0) ) {
        	char temp3[2048] = "/";
        	char *temp_sub = temp3;
        	temp_sub = strcat(temp_sub,temp);
        	if (strncmp(sub_dirp->d_name, pattern, 2048) == 0){   // checks to see if the given pattern exists in the directory
        		printf("./%s\n", sub_dirp->d_name);
        	}
        	char * temp_full_path = calloc(2048, sizeof(unsigned char) );
        	temp_full_path = strcpy(temp_full_path,sub_dir);
        	strcat(temp_full_path,temp_sub);
        	DIR * subsubdp = opendir(temp_full_path);
        	if(subsubdp != NULL){
        		closedir(subsubdp);
        		if( (strncmp(temp,temp1, 3)!= 0) ) {

        		}
        		namepath(temp_full_path, pattern);  // recursively calls the namepath function
        	}
        }
        else{
        	continue;
        }
    }
       closedir(sub_dp);
   }
    else
    {
        printf("cannot open directory\n");
        exit(2);
    }
}


char * full_path;

static int nodepath(char * sub_dir, int number){
	// struct stat     statbuf;
	struct dirent   *sub_dirp;
	DIR  *sub_dp = opendir(sub_dir);         //open the parent

  if(sub_dp!=NULL) {
       while((sub_dirp = readdir(sub_dp) )!= NULL) {              // read the files in the parent
        char * temp = sub_dirp->d_name;                           // print names of files in the parent
        char temp1[2048]= ".\0";
        char temp2[2048]= "..\0";

	 //recurcively loop into the sub-directory

        if( (strncmp(temp,temp1, 3)!= 0) && (strncmp(temp,temp2, 3) != 0) ) {
        	char temp3[2048] = "/";
        	char *temp_sub = temp3;
        	temp_sub = strcat(temp_sub,temp);

            if(sub_dirp->d_ino == number){    //compare inode numbers
                printf("%s\n", sub_dirp->d_name);    //also print the file/directory in the parent that links to the child
            }

        	char * temp_full_path = calloc(2048, sizeof(unsigned char) );
        	temp_full_path = strcpy(temp_full_path,sub_dir);
        	strcat(temp_full_path,temp_sub);
        	DIR * subsubdp = opendir(temp_full_path);
        	if(subsubdp != NULL){
        		closedir(subsubdp);
        		if( (strncmp(temp,temp1, 3)!= 0) ) {

        		}
        		nodepath(temp_full_path, number);  // recursively calls the nodepath function
        	}
        }
        else{
        	continue;
        }
    }
       closedir(sub_dp);
   }
    else
    {
        printf("cannot open directory\n");
        exit(2);
    }
}