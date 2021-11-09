#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80

int main(void)
{
    char *args[MAX_LINE/2 +1];
    int should_run = 1;
    int execvp_status;
    int wait_status;
    char history[10][MAX_LINE];
    pid_t processIDs[10];
    int historyTracker = 0;

    pid_t pid;
    int n = 0;

    char inputLine[200];

    int arrSize = sizeof args / sizeof args[0];


    while (should_run){
	        printf("CSCI3120>");
	        fflush(stdout);

	        fgets(inputLine, 200, stdin);

		inputLine[strlen(inputLine) - 1] = '\0';


		int commandNum = (inputLine[1] - '0') - 1;
		int secondCommand = (inputLine[2] - '0');


		if (inputLine[0] == '!') {
			if (inputLine[1] == '!') {
				inputLine[strlen(inputLine) -1] = '\n';
				strcpy(inputLine, history[0]);
			}
			else if (commandNum >= 0 && commandNum < 9 && commandNum <= historyTracker && strlen(inputLine) < 4) {
				if (secondCommand == 0){
					inputLine[strlen(inputLine) - 1] = '\n';
					strcpy(inputLine, history[9]);
					printf("Visited");
				}

				else{
					inputLine[strlen(inputLine) - 1] = '\n';
					strcpy(inputLine, history[commandNum]);
				}
			}
			else if (commandNum > historyTracker && inputLine[2] != '0') {
				printf("Such a command is  NOT in history!\n");
				exit(1);
			}
		}

	        char * splitLine = strtok(inputLine, " ");
	        n = 0;
	        while (splitLine != NULL){
			args[n] = splitLine;
		        splitLine = strtok(NULL, " \n");
		        n++;
	        }
	        args[n] = NULL;


	        //args[arrSize - 1] = NULL;

	        if (strcmp(args[0], "exit") == 0){
	        	should_run = 0;
	        	break;
	        }

		if (strcmp(args[0], "history") == 0){
			if(historyTracker == 0){
		        	printf("No command in history!\n");
		    	}
		        else{
				printf("ID\tPID\tCommand\n");
				for (int i = 0; i < historyTracker; i++){
			        	printf("%d\t%d\t%s\n", i+1, processIDs[i], history[i]);
				}
		    	}
		}

		else if(strcmp(args[0], "history") != 0){
			for (int i = 9; i > 0; i--){
		        	strcpy(history[i], history[i-1]);
		        	processIDs[i] = processIDs[i-1];
			}
			strcpy(history[0], inputLine);

			if (historyTracker != 10){
		        	historyTracker++;
			}

		        pid = fork();

		        if (pid < 0){
		            fprintf(stderr, "Fork Failed");
		            return 1;
		        }
		        else if (pid == 0){
		            execvp_status = execvp(args[0], args);
			        //break;
		            if (execvp_status == -1){
		                printf("Invalid Command!\n");
		                exit(1);
		            }
		        }
		        else{
			    //wait();
		            wait(&wait_status);
		            //printf("Child complete!\n");
			    processIDs[0] = pid;
		        }
		}

    }
    return 0;
}
