// including all the necessary libraries

#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>


// initializing the static methods

static int dopath();
static int namepath();
static int namepath_delete();
static int nodepath();
static int nodepath_delete();
static int modepath_bool();
static int modepath_bool_delete();
static int modepath_number();
static int modepath_number_delete();

int main(int argc, char **argv){
	char actualpath [PATH_MAX+1];   // initializing an array of strings
	char * resolved = realpath(argv[1],actualpath); // a variable that stores real path
	char ** argv_p = argv;   // copy the reference the argv array

	if (argc == 2){         // checks to see if the argument passed is for the first case.
		dopath(resolved);
	} 

	++argv; // moves argv to the index 1 argument


	// this loop is used to parse the argument from the command prompt

	while(*++argv != NULL && **argv == '-'){ 
		char *temp = *argv;

		if (strncmp(temp, "-name", 4) == 0 ){
			char *pattern = *++argv;
			if (argc >= 5){
				if (strncmp(argv_p[4], "-delete", 8) == 0){
					printf("This is for the delete testing purpose\n");   // this is used for testing purposes
					namepath_delete(resolved, pattern);
				}
				else if (strncmp(argv_p[4], "-exec", 6) == 0){
					execvp(argv_p[5], &argv_p[5]);   // this is used to execute the -exec functionality
				}
			}
			else{
				namepath(resolved, pattern);
			}
		}
		else if (strncmp(temp, "-mmin", 4) == 0 ){
			char temp_mode = **++argv;     // this character stores the +/- flag
			char * timenum = (*argv)+1;    // this string stores everything from the 1st index 
			
			// initializing a string then concatenating the two strings. This is done for the default case n

			char finalArray[2048];
			finalArray[0] = temp_mode;
			strncat(finalArray, timenum, 2048);

			if (argc >= 5){
				if (strncmp(argv_p[4], "-delete", 8) == 0){
					printf("This is for the delete testing purpose\n");
					switch(temp_mode){
						case '+':  // this is for the +n case
						printf("This is the plus condition for delete\n");
						modepath_bool_delete(resolved, atoi(timenum), 1);   // atoi function converts the string to an integer
						break;
						case '-':   // this is for the -n case
						printf("This is the minus condition for delete\n");
						modepath_bool_delete(resolved, atoi(timenum), -1);
						break;
						default:   // this is for the n case
						printf("This is the default condition for delete\n");
						modepath_number_delete(resolved, atoi(finalArray));
						break;
					}
				}
				else if (strncmp(argv_p[4], "-exec", 6) == 0){
					printf("This is for the exec testing purpose\n");
					printf("%s\n", argv_p[5]);
				}
			}
			else{
				switch(temp_mode){
					case '+':  // this is for the +n case
					printf("This is the plus condition\n");
					modepath_bool(resolved, atoi(timenum), 1);   // atoi function converts the string to an integer
					break;
					case '-':   // this is for the -n case
					printf("This is the minus condition\n");
					// printf("Timenum: %s\n",timenum);
					modepath_bool(resolved, atoi(timenum), -1);
					break;
					default:   // this is for the n case
					printf("This is the default condition\n");
					modepath_number(resolved, atoi(finalArray));
					break;
				}
			}
		}
		else if (strncmp(temp, "-inum", 4) == 0 ){
			if (argc >= 5){
				if (strncmp(argv_p[4], "-delete", 8) == 0){
					printf("This is for the delete testing purpose\n");
					//nodepath_delete(resolved, atoi(*++argv));
				}
				else if (strncmp(argv_p[4], "-exec", 6) == 0){
					printf("This is for the exec testing purpose\n");
					printf("This is resolved: %s\n", resolved);
					printf("This is pattern: %d\n", atoi(*++argv));
					printf("This is command: %s\n", argv_p[5]);
					// nodepath_exec(resolved, atoi(*++argv), &argv_p[5]);
				}
			}
			else{
				nodepath(resolved, atoi(*++argv));
			}
		}
	}

	exit(0);
}

char * full_path;

static int dopath(char * sub_dir) {
	struct dirent   *sub_dirp;         // pointer to the dirent struct
	DIR  *sub_dp = opendir(sub_dir);   // open the parent directory

  if(sub_dp!=NULL) {
       while((sub_dirp = readdir(sub_dp) )!= NULL) {    //read the files in the parent
        char * temp = sub_dirp->d_name;                 // print names of files in the parent
        char temp1[2048]= ".\0";
        char temp2[2048]= "..\0";

	 // walks over the directory

        if( (strncmp(temp,temp1, 3)!= 0) && (strncmp(temp,temp2, 3) != 0) ) {
        	char temp3[2048] = "/";
        	char *temp_sub = temp3;
        	temp_sub = strcat(temp_sub,temp);
        	printf("./%s\n", sub_dirp->d_name);   // prints the files that are present in the directory
        	char * temp_full_path = calloc(2048, sizeof(unsigned char) ); // allocating memory space
        	temp_full_path = strcpy(temp_full_path,sub_dir); // takes a copy
        	strcat(temp_full_path,temp_sub);  //  joins two strings together
        	DIR * subsubdp = opendir(temp_full_path);
        	if(subsubdp != NULL){
        		closedir(subsubdp);
        		dopath(temp_full_path);   // recursively calls the dopath function for sub directories
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

static int namepath(char * sub_dir, char * pattern){
	struct dirent   *sub_dirp;
	DIR  *sub_dp = opendir(sub_dir);         //open the parent

  if(sub_dp!=NULL) {
       while((sub_dirp = readdir(sub_dp) )!= NULL) {              // read the files in the parent
        char * temp = sub_dirp->d_name;                           // print names of files in the parent
        char temp1[2048]= ".\0";
        char temp2[2048]= "..\0";

	 // walk over a directory

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
        		namepath(temp_full_path, pattern);  // // recursively calls the dopath function for sub directories
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


static int nodepath(char * sub_dir, int number){
	struct dirent   *sub_dirp;
	DIR  *sub_dp = opendir(sub_dir);         // open the parent

  if(sub_dp!=NULL) {
       while((sub_dirp = readdir(sub_dp) )!= NULL) {              // read the files in the parent
        char * temp = sub_dirp->d_name;                           // print names of files in the parent
        char temp1[2048]= ".\0";
        char temp2[2048]= "..\0";

	 // walk over a directory

        if( (strncmp(temp,temp1, 3)!= 0) && (strncmp(temp,temp2, 3) != 0) ) {
        	char temp3[2048] = "/";
        	char *temp_sub = temp3;
        	temp_sub = strcat(temp_sub,temp);

            if(sub_dirp->d_ino == number){    // compare inode numbers
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
        		nodepath(temp_full_path, number);  // recursively calls the dopath function for sub directories
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


static int modepath_number(char * sub_dir, int number){
	struct dirent   *sub_dirp;
	struct stat buf;
	DIR  *sub_dp = opendir(sub_dir);        // open the parent
	time_t curtime;                         // current time
	time(&curtime);
	unsigned long elapsed_time;             // stores the elapsed time
	char actualpath[PATH_MAX+1];
	char * resolved = realpath(sub_dir, actualpath);
	strncat(resolved, "/", 2048);
	char array_path[2048];           // initializing the reference string
	strcpy(array_path, resolved);   // copying a reference of the actual path

  if(sub_dp!=NULL) {
       while((sub_dirp = readdir(sub_dp) )!= NULL) {              // read the files in the parent
        char * temp = sub_dirp->d_name;                           // print names of files in the parent
        char temp1[2048]= ".\0";
        char temp2[2048]= "..\0";

	 // walks over the directory

        if( (strncmp(temp,temp1, 3)!= 0) && (strncmp(temp,temp2, 3) != 0) ) {
        	char temp3[2048] = "/";
        	char *temp_sub = temp3;
        	temp_sub = strcat(temp_sub,temp);
        	strncat(resolved, sub_dirp->d_name, 2048);    // delete file/directory in the parent that links to the child
        	stat(resolved,&buf);

        	elapsed_time = (curtime-buf.st_mtim.tv_sec)/60;  // Time elapsed between current Unix Time and latest time file was modified in minutes

            if( elapsed_time == number){  // this is for the n condition
            	printf("./%s\n", sub_dirp->d_name);
            }

            strcpy(resolved, array_path);

        	char * temp_full_path = calloc(2048, sizeof(unsigned char) );
        	temp_full_path = strcpy(temp_full_path,sub_dir);
        	strcat(temp_full_path,temp_sub);
        	DIR * subsubdp = opendir(temp_full_path);
        	if(subsubdp != NULL){
        		closedir(subsubdp);
        		if( (strncmp(temp,temp1, 3)!= 0) ) {

        		}
        		modepath_number(temp_full_path, number);  // recursively calls the dopath function for sub directories
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

static int modepath_bool(char * sub_dir, int number, int flag){
	struct dirent   *sub_dirp;
	struct stat buf;
	DIR  *sub_dp = opendir(sub_dir);         //open the parent
	time_t curtime;                         // current time
	time(&curtime);
	unsigned long elapsed_time;             // stores the elapsed time
	char actualpath[PATH_MAX+1];
	char * resolved = realpath(sub_dir, actualpath);
	strncat(resolved, "/", 2048);
	char array_path[2048];           // initializing the reference string
	strcpy(array_path, resolved);   // copying a reference of the actual path


  if(sub_dp!=NULL) {
       while((sub_dirp = readdir(sub_dp) )!= NULL) {              // read the files in the parent
        char * temp = sub_dirp->d_name;                           // print names of files in the parent
        char temp1[2048]= ".\0";
        char temp2[2048]= "..\0";

	 // walk over the directory

        if( (strncmp(temp,temp1, 3)!= 0) && (strncmp(temp,temp2, 3) != 0) ) {
        	char temp3[2048] = "/";
        	char *temp_sub = temp3;
        	temp_sub = strcat(temp_sub,temp);
        	strncat(resolved, sub_dirp->d_name, 2048);    // delete file/directory in the parent that links to the child
        	stat(resolved,&buf);

        	elapsed_time = (curtime-buf.st_mtim.tv_sec)/60;  // Time elapsed between current Unix Time and latest time file was modified in minutes

        	// printf("./%s\n", resolved);
        	// printf("Time elapsed between current Unix Time and latest time file was modified for MINUS: %lu\n", elapsed_time);

            if( flag == -1 && elapsed_time < number){    // This is for the -n condition
            	printf("./%s\n", sub_dirp->d_name);
            }

            else if(flag == 1 && elapsed_time > number){  // This is for the +n condition
            	printf("./%s\n", sub_dirp->d_name);
            }

            strcpy(resolved, array_path);

        	char * temp_full_path = calloc(2048, sizeof(unsigned char) );
        	temp_full_path = strcpy(temp_full_path,sub_dir);
        	strcat(temp_full_path,temp_sub);
        	DIR * subsubdp = opendir(temp_full_path);
        	if(subsubdp != NULL){
        		closedir(subsubdp);
        		if( (strncmp(temp,temp1, 3)!= 0) ) {

        		}
        		modepath_bool(temp_full_path, number, flag);  // recursively calls the dopath function for sub directories
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

static int namepath_delete(char * sub_dir, char * pattern){
	struct dirent   *sub_dirp;
	DIR  *sub_dp = opendir(sub_dir);         //open the parent
	char actualpath[PATH_MAX+1];
	char * resolved = realpath(sub_dir, actualpath);
	strncat(resolved, "/", 2048);
	char array_path[2048];           // initializing the reference string
	strcpy(array_path, resolved);   // copying a reference of the actual path


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
            	remove(strncat(resolved, sub_dirp->d_name, 2048));    // delete file/directory in the parent that links to the child
            	strcpy(resolved, array_path);                       // goes back to the original path
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


static int nodepath_delete(char * sub_dir, int number){
	struct dirent   *sub_dirp;
	DIR  *sub_dp = opendir(sub_dir);         //open the parent
	char actualpath[PATH_MAX+1];
	char * resolved = realpath(sub_dir, actualpath);
	strncat(resolved, "/", 2048);
	char array_path[2048];           // initializing the reference string
	strcpy(array_path, resolved);   // copying a reference of the actual path

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
            	remove(strncat(resolved, sub_dirp->d_name, 2048));    // delete file/directory in the parent that links to the child
            	strcpy(resolved, array_path);
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


static int modepath_number_delete(char * sub_dir, int number){
	struct dirent   *sub_dirp;
	struct stat buf;
	DIR  *sub_dp = opendir(sub_dir);        // open the parent
	time_t curtime;                         // current time
	time(&curtime);
	unsigned long elapsed_time;             // stores the elapsed time
	char actualpath[PATH_MAX+1];
	char * resolved = realpath(sub_dir, actualpath);
	strncat(resolved, "/", 2048);
	char array_path[2048];           // initializing the reference string
	strcpy(array_path, resolved);   // copying a reference of the actual path

  if(sub_dp!=NULL) {
       while((sub_dirp = readdir(sub_dp) )!= NULL) {              // read the files in the parent
        char * temp = sub_dirp->d_name;                           // print names of files in the parent
        char temp1[2048]= ".\0";
        char temp2[2048]= "..\0";

        // loop over the directory

        if( (strncmp(temp,temp1, 3)!= 0) && (strncmp(temp,temp2, 3) != 0) ) {
        	char temp3[2048] = "/";
        	char *temp_sub = temp3;
        	temp_sub = strcat(temp_sub,temp);
        	strncat(resolved, sub_dirp->d_name, 2048);    // delete file/directory in the parent that links to the child
        	stat(resolved,&buf);

        	elapsed_time = (curtime-buf.st_mtim.tv_sec)/60;  // Time elapsed between current Unix Time and latest time file was modified in minutes

            if( elapsed_time == number){              // this is for the n condition
            	remove(resolved);                    //  delete file/directory in the parent that links to the child
            	strcpy(resolved, array_path);
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


static int modepath_bool_delete(char * sub_dir, int number, int flag){
	struct dirent   *sub_dirp;
	struct stat buf;
	DIR  *sub_dp = opendir(sub_dir);         //open the parent
	time_t curtime;                         // current time
	time(&curtime);
	unsigned long elapsed_time;             // stores the elapsed time
	char actualpath[PATH_MAX+1];
	char * resolved = realpath(sub_dir, actualpath);   // gets the absolute path of the file
	strncat(resolved, "/", 2048);
	char array_path[2048];           // initializing the reference string
	strcpy(array_path, resolved);   // copying a reference of the actual path

  if(sub_dp!=NULL) {
       while((sub_dirp = readdir(sub_dp) )!= NULL) {              // read the files in the parent
        char * temp = sub_dirp->d_name;                           // print names of files in the parent
        char temp1[2048]= ".\0";
        char temp2[2048]= "..\0";

        if( (strncmp(temp,temp1, 3)!= 0) && (strncmp(temp,temp2, 3) != 0) ) {
        	char temp3[2048] = "/";
        	char *temp_sub = temp3;
        	temp_sub = strcat(temp_sub,temp);
        	strncat(resolved, sub_dirp->d_name, 2048);    // delete file/directory in the parent that links to the child
        	stat(resolved,&buf);

        	elapsed_time = (curtime-buf.st_mtim.tv_sec)/60;  // Time elapsed between current Unix Time and latest time file was modified in minutes

            if( flag == -1 && elapsed_time < number){    // This is for the -n condition
            	//printf("Time elapsed between current Unix Time and latest time file was modified for MINUS: %lu\n", elapsed_time);
            	remove(resolved);    // delete file/directory in the parent that links to the child
            	strcpy(resolved, array_path);
            }

            else if(flag == 1 && elapsed_time > number){  // This is for the +n condition
            	remove(resolved);    // delete file/directory in the parent that links to the child
            	strcpy(resolved, array_path);
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