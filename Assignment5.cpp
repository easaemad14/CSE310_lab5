/**********************************************************************************
 *File: 	Assignment5.cpp
 *Author: 	Easa El Sirgany (easaemad14@gmail.com)
 *
 *Description:  Implementation of the project requirements set forth in the 
 *		corresponding text file, with modifications in order to improve
 *		flexibility of user input and program process. 
 *********************************************************************************/
#include "assignment5.hpp"

void helpMenu(){
	std::cout << "To begin, please enter the desired size of the hash ";
	std::cout << "table. Then, use the\n following commands in order to ";
	std::cout << "modify the list (case sensitive): \n\n";

	std::cout << "=====================================\n";
	std::cout << " * \"Insertion\" to insert a new class" << std::endl;
	std::cout << " * \"Search\" to search for a class" << std::endl;
	std::cout << " * \"Delete\" to delete a class" << std::endl;
	std::cout << " * \"Display\" to display list and quit" << std::endl;
	std::cout << "=====================================\n" << std::endl;
}

int main() //No parameters will be passed and if there are, we won't address them
{
	unsigned int size;
	char buff[MAX_LINE_LENGTH + 1];

	std::cout << "Please enter a hash table size:\n" << std::endl;
	std::cin.getline(buff, MAX_LINE_LENGTH);
	while(!isdigit(buff[0])){ //Print message until pos number is entered
		std::cout << "To begin, please enter the size of the hash table ";
		std::cout << "that you would like to create.\n" << std::endl;
		get_next_line(buff, MAX_LINE_LENGTH);
	}

	size = atoi(buff);
	HashTable *table = new HashTable(size);

	//Now let's get the first line (command) and start having some fun!
	get_next_line(buff, MAX_LINE_LENGTH);
	while(isalpha(buff[0]) && strcmp(buff, "Display")!=0){
		if(strcmp(buff, "Insertion") == 0){ //Case sensitive!
			/* Today I learned that the comma deliminator isn't as cool
			 * as I thought it was. I had to change my fancy macro, and
			 * now my get_next_line command will not work in a for loop
			 * without some modifications that I don't like the look
			 * of. Therefore, you will have to suffer looking at all of
			 * these unasthetic while loops :( */
			get_next_line(buff, MAX_LINE_LENGTH);
			for(; strcmp(buff, "Search")!=0;){ //Just showing off
				table->insertElement(buff);
				get_next_line(buff, MAX_LINE_LENGTH);
			}
			//Once we are done, display the list
			table->displayTable();
		}
		else if(strcmp(buff, "Search") == 0){
			get_next_line(buff, MAX_LINE_LENGTH);
			while(strcmp(buff, "Delete")!=0){
				table->searchElement(buff);
				get_next_line(buff, MAX_LINE_LENGTH);
			}
			std::cout << std::endl;
		}
		else if(strcmp(buff, "Delete") == 0){
			get_next_line(buff, MAX_LINE_LENGTH);
			while(strcmp(buff, "Display")!=0){
				table->deleteElement(buff);
				get_next_line(buff, MAX_LINE_LENGTH);
			}
			std::cout << std::endl;
		}
		else if(strcmp(buff, "Display")!=0){ //This is an error in input
			helpMenu();
			get_next_line(buff, MAX_LINE_LENGTH);
		}
	}
	//Once we have exited the loop, display the table, and exit
	table->displayTable();

	delete table; //Can't beleive I forgot this line! C++ makes you lazy
	exit(EXIT_SUCCESS);

error: //Error fallthrough (Note: this is never called, so -Wall should warn you)
	std::cout << "An error occured in the main function!\n\n";
	exit(EXIT_FAILURE);
}
