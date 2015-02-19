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
	std::cout << "table. Then, use the following commands in order to ";
	std::cout << "modify the list: \n\n";

	std::cout << "=======================================\n";
	std::cout << " * \"Insertion\" to insert a new class" << std::endl;
	std::cout << " * \"Search\" to search for a class" << std::endl;
	std::cout << " * \"Delete\" to delete a class" << std::endl;
	std::cout << " * \"Display\" to display list and quit" << std::endl;
	std::cout << "=======================================\n" << std::endl;
}


int main(int argc, char *argv[])
{
	unsigned int size;
	char buff[MAX_LINE_LENGTH];
	std::string Insertion = "Insertion", Search = "Search",
			Delete = "Delete", Display = "Display";

	std::cin.get(buff, MAX_LINE_LENGTH);
	if(!isdigit(buff[0])){
		std::cout << "To begin, please enter the size of the hash table ";
		std::cout << "that you would like to create.\n" << std::endl;
	}
	else size = atoi(buff);
	flush_buffer(buff, MAX_LINE_LENGTH);
	HashTable *table = new HashTable(size);

	for(std::cin.getline(buff, MAX_LINE_LENGTH); //Initialization
	Display.compare(buff) != 0; //Condition
	get_next_line(buff, MAX_LINE_LENGTH)){
		if(Insertion.compare(buff) == 0){
			//Do some shit
		}
		else if(Search.compare(buff) == 0){
			//Do some different shit
		}
		else if(Delete.compare(buff) == 0){
			//More shit to do
		}
		else if(Display.compare(buff) == 0){
			//Display some shit
		}
		else{ //This is an error in input
			helpMenu();
		}
	}

	return 0;

error: //Error fallthrough (Note: this is never called, so -Wall should warn you)
	std::cout << "An error occured in the main function!\n\n";
	exit(EXIT_FAILURE);
}
