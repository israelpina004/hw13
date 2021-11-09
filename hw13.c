#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

struct pop_entry {
	int year;
	int population;
	char boro[15];
};

int read_csv() {
	struct stat sb;

	//Open csv file.
	int fd = open("nyc_pop.txt", O_RDONLY);
	if (fd < 0) {
		printf("File does not exist.");
		return -1;
	}

	//Read csv file info into a string.
	stat("nyc_pop.txt", &sb);
	char* csv = malloc(sb.st_size);
	read(fd, csv, sb.st_size);

	//Used to determine the number of elements needed in the struct array
	//that will be used to store all the data in the file.
	int i;
	int commas = 0;
	for (i = 51; i < sb.st_size; i++) {
		if(csv[i] == ',') {
			commas++;
		}
	}
	//Creates the struct array.
	struct pop_entry* all_data = calloc(commas, sizeof(struct pop_entry));

	//Populates the struct array.
	int yr;
	int count = 0;
	int index = 0;
	i = 51;
	int j;
	while(i < sb.st_size) {

		char temp[8];

		for (j = 0; j < 8; j++) {
			//If newline, do not add to the temp string.
			if (csv[i] == '\n') {
				i++;
				count++;
				break;
			}
			//If a comma, do not add to the temp string.
			if (csv[i] ==  ',') {
				i++;
				count++;
				break;
			}
			temp[j] = csv[i];
			i++;
		}
		//Convert temp into a number.
		int num = atoi(temp);

		struct pop_entry data;

		//Uses comma placement to determine what to store in the struct.
		switch(count) {
			case 1:
				yr = num;
				break;
			case 2:
				data.year = yr;
				data.population = num;
				strncpy(data.boro, "Manhattan", 15);
				all_data[index] = data;
				index++;
				break;
			case 3:
				data.year = yr;
				data.population = num;
				strncpy(data.boro, "Brooklyn", 15);
				all_data[index] = data;
				index++;
				break;
			case 4:
				data.year = yr;
				data.population = num;
				strncpy(data.boro, "Queens", 15);
				all_data[index] = data;
				index++;
				break;
			case 5:
				data.year = yr;
				data.population = num;
				strncpy(data.boro, "Bronx", 15);
				all_data[index] = data;
				index++;
				break;
			case 6:
				data.year = yr;
				data.population = num;
				strncpy(data.boro, "Staten Island", 15);
				all_data[index] = data;
				index++;
				count = 0;
				break;
			}

		strncpy(temp, "\0", 8);
	}

	//Writes the struct array into a new file.
	int new_file = open("nyc.data", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	int bytes = (index+1) * sizeof(struct pop_entry);
	write(new_file, all_data, bytes);
	printf("Wrote %d bytes to nyc.data\n", bytes);

	//Free malloced/calloced variables.
	free(all_data);
	free(csv);

	//Returns 0 if all goes well.
	return 0;
}


void read_data(){
	//Set up the struct array.
  struct stat sb;
  stat("nyc.data", &sb);
	struct pop_entry * arr = malloc(sb.st_size);

	//Read nyc.data into struct array.
	int data = open("nyc.data", O_RDONLY);
  read(data, arr, sb.st_size);

	//Print out the struct array.
  int i;
	for(i = 0; i < sb.st_size/sizeof(struct pop_entry)-1; i++) {
		if (strcmp(arr[i].boro, "Staten Island") != 0) {
    	printf("%d: year: %d\tboro: %s\t\tpop: %d\n", i+1, arr[i].year, arr[i].boro, arr[i].population);
		}
		else {
			printf("%d: year: %d\tboro: %s\tpop: %d\n", i+1, arr[i].year, arr[i].boro, arr[i].population);
		}
  }

	//Close/free when needed.
	close(data);
  free(arr);
}

void add_data() {
	//Set up the data file for appending.
	int data = open("nyc.data", O_WRONLY | O_APPEND);

	//Reads from stdin new data.
	char line[40];
	printf("Enter year boro pop, with a space in between each input: ");
	fgets(line, 40, stdin);

	//Stores inputs from stdin into variables.
	char borough[15];
	int yr;
	int pop;
	sscanf(line, "%d %s %d", &yr, borough, &pop);

	//Stores these variables into a struct.
	struct pop_entry new_data;
	strncpy(new_data.boro, borough, 15);
	new_data.year = yr;
	new_data.population = pop;

	//Writes the struct to the data file.
	write(data, &new_data, sizeof(struct pop_entry));

	close(data);
}

void update_data() {
	//Set up the struct array.
  struct stat sb;
  stat("nyc.data", &sb);
	struct pop_entry * arr = malloc(sb.st_size);

	//Read nyc.data into struct array.
	int data = open("nyc.data", O_RDWR | O_TRUNC);
  read(data, arr, sb.st_size);

	//Reads from stdin the entry that will be modified.
	char line1[5];
	printf("What entry would you like to modify? ");
	fgets(line1, 5, stdin);
	int entry;
	sscanf(line1, "%d", &entry);

	//Reads from stdin new data.
	char line2[40];
	printf("Enter year boro pop, with a space in between each input: ");
	fgets(line2, 40, stdin);

	//Stores inputs from stdin into variables.
	char borough[15];
	int yr;
	int pop;
	sscanf(line2, "%d %s %d", &yr, borough, &pop);

	//Variables are stored into a struct.
	struct pop_entry new_data;
	strncpy(new_data.boro, borough, 15);
	new_data.year = yr;
	new_data.population = pop;

	//The array of structs is modified and written into the data file.
	arr[entry-1] = new_data;
	write(data, arr, sb.st_size);

	close(data);
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
		if(strcmp(argv[1], "read_csv") == 0) {
			read_csv();
		}
		else if (strcmp(argv[1], "read_data") == 0) {
			read_data();
		}
		else if (strcmp(argv[1], "add_data") == 0) {
			add_data();
		}
		else if (strcmp(argv[1], "update_data") == 0) {
			update_data();
		}
	}

	return 0;
}
