#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include<string.h> 
#define HEBREW_YEAR_LENGTH 6
#define NUMBER 0
#define STRING 1

// structs
struct Worker
{
	unsigned long long id;
	char* name;
	double salary;
	int year_type;
	union Year {
		unsigned long long num_year;
		char he_year[HEBREW_YEAR_LENGTH];
	} year;
}typedef worker;//a
typedef struct WorkerList
{
	worker* data;
	struct WorkerList* next;
} WorkerList;

// functions
worker* CreateWorker(unsigned long long id, char* w_name, double salary, unsigned long long num_year, char he_year[6], int year_type);//b1
void PrintWorker(worker* w);//b2
void PrintList(WorkerList* head);
WorkerList* addWorker(WorkerList* head, worker* w);//c1
int index(WorkerList* head, long unsigned id);//c2
WorkerList* deleteWorstWorker(WorkerList* head);//c3
void update_worker(WorkerList* head, float percent);//c4
WorkerList* reverse(WorkerList* head);//c5
void freeWorkers(WorkerList* head);//c6

int main() {
	WorkerList* head = NULL;

	//Adding employees to the list 
	head = addWorker(head, CreateWorker(224466, "Lior S", 17000, 1948, "0", NUMBER));
	head = addWorker(head, CreateWorker(123456, "Ben G", 12, 2023, "0", NUMBER));
	head = addWorker(head, CreateWorker(11335577, "Orel S", 25000, 2020, "abc", STRING));
	head = addWorker(head, CreateWorker(654321, "Eyal K", 19000, 2012, "rty", NUMBER));
	head = addWorker(head, CreateWorker(98765, "Yuval M", 500, 2009, "nba", STRING));

	// print workers list
	printf("\n<<<<<the list>>>>>\n");
	PrintList(head);

	//Print requested indexes
	printf("<<<<The first requested index>>>>\n%d\n", index(head, 11335577));
	printf("<<<<The second requested index>>>>\n%d\n", index(head, 1133557));
	printf("<<<<The second requested index>>>>\n%d\n", index(head, 98765));

	//Deleting the worker with the lowest salary
	printf("\n<<<<The worst worker>>>>");
	PrintWorker(head->data);
	head = deleteWorstWorker(head);

	//The new list after deletion
	printf("<<<<The new list>>>>");
	PrintList(head);

	printf("<<<<The list after updating salaries>>>>");
	update_worker(head, 0.52);
	PrintList(head);

	printf("<<<<The reverse list>>>>");
	head = reverse(head);
	PrintList(head);

	//Data release
	freeWorkers(head);

	return 0;
}


// b1
worker* CreateWorker(unsigned long long id, char* w_name, double salary, unsigned long long num_year, char he_year[], int year_type)
{
	// allocate memory for worker
	worker* w = (worker*)malloc(sizeof(worker));
	if (!w)
	{
		printf("Allocation Error!");
		exit(1);
	}
	// insert data into worker:
	w->id = id;
	// allocate memory for name
	w->name = (char*)malloc((strlen(w_name) + 1) * sizeof(char));
	if (!(w->name))
	{
		printf("Allocation Error!");
		exit(1);
	}
	strcpy(w->name, w_name);
	w->salary = salary;
	w->year_type = year_type;
	if (year_type == NUMBER)
		w->year.num_year = num_year;
	if (year_type == STRING)
		strcpy(w->year.he_year, he_year);

	if ((year_type != NUMBER) && (year_type != STRING))
		printf("WRONG FORMAT! Please enter a valid year type\n");

	return w;
}

// b2
void PrintWorker(worker* w)
{
	if (w == NULL) // If there are no employees on the list
	{
		printf("There are no workers on the list");
		return;
	}
	printf("\n\nworker details:\n");
	printf("ID: %llu\n", w->id);
	printf("NAME: %s\n", w->name);
	printf("SALARY: %.2lf\n", w->salary);
	if (w->year_type == STRING)
	{
		printf("First year of work: %s\n\n", w->year.he_year);
		return;
	}
	if (w->year_type == NUMBER)
	{
		printf("First year of work: %llu\n\n", w->year.num_year);
		return;
	}
}

void PrintList(WorkerList* head) //print all workers
{
	WorkerList* tmp = head;
	while (tmp != NULL)
	{
		PrintWorker(tmp->data);
		tmp = tmp->next;
	}
}

//c1
WorkerList* addWorker(WorkerList* head, worker* w)
{
	//Add an employee to the list for the correct location.
	//from the low salary to the high salary.
	if (head == NULL)	// if list is empty
	{
		// create a new head for the list
		head = (WorkerList*)malloc(sizeof(WorkerList));
		if (head)
		{
			head->data = w;
			head->next = NULL;
			return head;
		}
		else
		{
			printf("Allocation Error!"); exit(1);
		}
	}
	else // if list is not empty
	{
		WorkerList* tmp = head;
		WorkerList* last = NULL;
		// allocate memory for list item
		WorkerList* newlist = (WorkerList*)malloc(sizeof(WorkerList));
		if (!(newlist))
		{
			printf("Allocation Error!"); exit(1);
		}
		// add worker data
		newlist->data = w;

		//If the salary of the new employee is lower than the salary of the head, the new
		//employee will be the new head, the previous head will be after him
		if (w->salary <= tmp->data->salary)
		{
			newlist->next = head;
			return newlist;
		}
		//Otherwise, put the employee in the exact place on the list
		else
		{
			last = tmp;
			tmp = tmp->next;
			int foundPlaceToInsert = 0;
			while (tmp != NULL && foundPlaceToInsert == 0)
			{
				if (w->salary <= tmp->data->salary)
				{
					foundPlaceToInsert = 1;
				}
				else
				{
					last = tmp;
					tmp = tmp->next;
				}
			}
			newlist->next = last->next;
			last->next = newlist;
			return head;
		}
	}
}

//c2.1 - Loop function
int index(WorkerList* head, long long unsigned id)
{
	WorkerList* ptr = head;
	int counter = 1;
	//Check the list from the head up to the desired index
	while (ptr->data->id != id)
	{
		//If the list is over
		if (ptr->next == NULL)
			return -1;
		counter++;
		ptr = ptr->next;//Progress to the next employee
	}
	//return the required index
	return counter;
}
//If you want to use a recursive function, use a function in the next line marked with "//"
// c2.2 - Recursive function
//int index(WorkerList* head, long long unsigned id) 
//{
//	static int counter = 1;
//	WorkerList* ptr = head;
//	if (ptr == NULL) //If not found
//		return -1;
//	if (ptr->data->id == id)
//	{
//		//return index
//		return counter;
//	}
//	//check next item
//	counter++;
//	index(ptr->next, id);
//}

// c3
WorkerList* deleteWorstWorker(WorkerList* head)
{
	//Since the employee with the lowest salary is the head - the head should be
	//deleted and the next employee on the list should be the head
	WorkerList* ptr = head->next;
	free(head);//Data release
	head = ptr;
	return head;
}


//c4 - Employee salary update
void update_worker(WorkerList* head, float percent)
{
	WorkerList* ptr = head;
	while (ptr != NULL)//Running all over the list
	{
		ptr->data->salary *= (1 + percent);
		ptr = ptr->next;
	}
}

////c5 - Reversing the list
WorkerList* reverse(WorkerList* head)
{
	WorkerList* tmp;
	WorkerList* previous;
	WorkerList* next;
	tmp = head;
	previous = NULL;
	while (tmp != NULL) //Running on the list
	{
		next = tmp->next;//save next worker
		tmp->next = previous;//connect current worker to previous worker
		previous = tmp;//save previous worker
		tmp = next;//save current worker
	}
	return previous;
}

//c6 - Running on the list and releasing
void freeWorkers(WorkerList* head)
{
	WorkerList* tmp;
	WorkerList* release;
	tmp = head;
	while (tmp != NULL)
	{
		release = tmp;
		tmp = tmp->next;
		free(release->data->name);
		free(release->data);
		free(release);
	}
	printf("The memory was successfully released!!!\n");
}

