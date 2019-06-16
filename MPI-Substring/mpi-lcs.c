#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define MASTER 0 //Task id of master task

double timer();
char* longest_common_substring(char* line1, char* line2);
void print_longest(short int **c, char *x, int index1, int index2);
int read_file();

char** lines;


int main(int argc, char *argv[])
{
    /*
    * size: number of processes that is going to run - num_tasks
    * rank: unique integer identifier (logical way of numbering processes) - task_id
    */
    double tstart, ttotal;
    tstart = timer();
    int task_id, num_tasks, indx, block, start, end;
    char** res = (char**) malloc(10000 * sizeof(char *));
    char** results = (char**) malloc(10000 * sizeof(char *));
    char* longest = NULL;
    lines = (char**) malloc(10000 * sizeof(char *)); // global data
    char** local_lines = malloc(sizeof(char**) * block);


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&num_tasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&task_id);


    //First, read the data from the files and store it in an array
    if (task_id == MASTER)
    {
        indx = read_file();
        block = indx-1 / num_tasks;
        start = task_id * block;
        end = start + block;
        //printf("Processor %d out of %d, executing on %s.", size, num_tasks, processor_name);
        printf("Processor %d has data", task_id);
        // Broadcast file data to all processes
        //MPI_Bcast(&lines,0,MPI_CHAR,0,MPI_COMM_WORLD);
        MPI_Scatter(&lines,     //array to be scattered
                    block,      //chunk of array per core
                    MPI_CHAR,   //datatype
                    &local_lines, //variable that can hold
                    block,  //chunk to be held
                    MPI_CHAR,//datatype
                    0,          //root process that is scattering the arra
                    MPI_COMM_WORLD);
        printf("hi");

        // Wait for slave processes
        MPI_Barrier(MPI_COMM_WORLD);

        MPI_Gather(&local_lines,    //array to gather
                    block,      //chunk of array per core
                    MPI_CHAR,   //datatype
                    &lines,
                    block,      //count of elements received per proces
                    MPI_CHAR,   //datatype
                    0,          //root process that is scattering the arra
                    MPI_COMM_WORLD);



    } else {
        //read data from master process
        MPI_Scatter(&lines,     //array to be scattered
                    block,      //chunk of array per core
                    MPI_CHAR,   //datatype
                    &local_lines, //variable that can hold
                    block,  //chunk to be held
                    MPI_CHAR,//datatype
                    0,          //root process that is scattering the arra
                    MPI_COMM_WORLD);
        printf("hi");

        // Wait for slave processes
        MPI_Barrier(MPI_COMM_WORLD);

        //send data to master process
        MPI_Gather(&local_lines,    //array to gather
                    block,      //chunk of array per core
                    MPI_CHAR,   //datatype
                    &lines,
                    block,      //count of elements received per proces
                    MPI_CHAR,   //datatype
                    0,          //root process that is scattering the arra
                    MPI_COMM_WORLD);


        for(int j = start; j<end; j++)
        {

        }
    }

    ttotal = timer() - tstart;
    printf( "The run on %d cores took %lf seconds\n", num_tasks, ttotal);
    MPI_Finalize();
    return 0;
}

//this method is found https://gist.github.com/adrian-source/4111719 .... THIS CODE IS NOT MINE
char* longest_common_substring(char* s1, char* s2)
{

    int s1_l = strlen(s1), s2_l = strlen(s2);
    int i, s1_i, s2_i, longest = 0;

    char* set = (char*)malloc(sizeof(char) * ((s1_l < s2_l)?s1_l:s2_l));
    memset(set, ' ', strlen(set));

    // initialize 2d array
    int** l = (int**)malloc((s1_l + 1) * sizeof(int*));
    for (i = 0; i < s1_l + 1; i++)
    l[i] = (int*)malloc((s2_l + 1) * sizeof(int));

    // initialize all array fields to 0
    for (s1_i = 0; s1_i < s1_l; s1_i++)
    for (s2_i = 0; s2_i < s2_l; s2_i++)
    l[s1_i][s2_i] = 0;

    // loop through all letters
    for (s1_i = 0; s1_i < s1_l; s1_i++)
    for (s2_i = 0; s2_i < s2_l; s2_i++)
    {
        // if characters match
        if (s1[s1_i] == s2[s2_i])
        {
            // calculate the incremented length for current sequence
            int v = l[s1_i][s2_i] + 1;

            // save the incremented length in south-east cell
            l[s1_i+1][s2_i+1] = v;

            // if new length is longest thus far
            if (v > longest)
            {
                // set that to be our new longest length
                longest = v;

                // and clear the set
                memset(set, ' ', strlen(set));
            }

            // if new length equals to the longest length
            if (v == longest)
            {
                // copy the char set to the return set
                for (i = 0; i < longest; i++)
                set[i] = s1[s1_i-longest+i+1];
            }
        }
    }

    // free
    for (i = 0; i < s1_l + 1; i++)
    free(l[i]);
    free(l);

    // return
    return set;
}



void print_results(char** results)
{
    int i;
    for (i= 0; i < sizeof(results); i++)
    {
        printf("%d-%d: %s\n", i, i+1, results[i]);
    }
}

int read_file()
{
    int indx = 0;
    char* line;
    FILE *input_file;
    input_file = fopen("/homes/dan/625/wiki_dump.txt", "r");
    if (input_file != NULL)
    {    while(fgets(line, sizeof(line), input_file) != NULL)
        {
            strcpy(lines[indx],line);
            indx++;
        }
    } else { //file object is null
        puts("Unable to open file\n");
        exit(-1);
    }
    fclose(input_file);
    return indx;
}

// taken from find_keys.c in the 625 directory
double timer() {
    static time_t t_start = 0;  // Save and subtract off each time

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    if( t_start == 0 ) t_start = ts.tv_sec;

    return (double) (ts.tv_sec - t_start) + ts.tv_nsec * 1.0e-9;
}
