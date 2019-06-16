/*Authors: Paula Mendez and Ryan Bates
* Date: 4/4/16
* this project is a continuation of the last assignment. After reading in the stopwords and 
* inserting them in a tree, we now read a txt file, process it with the help of the tree to help 
* determine which language it belongs to.
*/

//TODO
//Read a text file
//stopwords++ when you find a stopword in the  
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>

//language struct holds all languages imported from the stopword files
typedef struct language {
	char *language;
	int count;  
}Lang;

//holds all the nodes in the tree
typedef struct node {
	char *word;
	Lang* languages[200];
	struct node* leftChild;
	struct node* rightChild;
} Node;

void Add(Node **root, Lang *s, char *word);
void search(Node *n, char *word);
void testLanguages(Node *root);
void parseTextInput();
Lang checkMax(int cnt);
int ExtractStopwords(DIR *dir, char *ar);


// ***GLOBAL VARIABLES***
Node *root = NULL;
Lang *languageList[200]; //lang array that holds all the languages that were read

//extract all the stopwords in all the files given by the user 
int ExtractStopwords(DIR *dir, char *ar)
{
	// Return the number of languages
	/*variables*/
	FILE *file;
	struct dirent *pDirent;
	char *p;
	char fname[FILENAME_MAX];
	char line[50]; //this line is word that will be added into the tree of stopwords
	int index = 0; //holds the current index of the lang struct array 
	Lang *s; //struct to be initialized that will hold the current language being read atm

	
	while ((pDirent = readdir(dir)) != NULL)
	{
		//read each file into the tree
		p = strrchr(pDirent->d_name, '.');

		//**ONLY** reads files that have the '.txt.' extention
		if (p && (strcmp(p, ".txt") == 0)) 
		{		
			s = malloc(sizeof(Lang));		
			sprintf(fname, "%s/%s", ar, pDirent->d_name);
			
			s->language = strdup(strtok(pDirent->d_name,"."));

			//ADD LANG STRUCT TO LANG STRUCT ARRAY
			languageList[index] = s;

			//Increment index
			index++;

			
			//Open each file
			if((file = fopen(fname, "r")) != NULL)
			{
				while(fgets(line, 50, file))
				{
					
					//Removes the \n from each line
					line[strcspn(line, "\n")] = 0;
					//Insert words into tree
					Add(&root, s, line);
					
				}//end while

			}//end if

		}//end if

	}//end while	

	closedir(dir);	
	return index;
}

//Inserts node into tree
void Add(Node **root, Lang *s, char *word)
{
	if(root != NULL)
	{
		if((*root) == NULL)
		{
			//create node
			*root = malloc(sizeof(Node));
			(*root)->word = strdup(word);
			(*root)->languages[0] = s;
			(*root)->languages[1] = NULL;
			(*root)->leftChild = NULL;
			(*root)->rightChild = NULL;

		}
		else
		{
			if(strcasecmp((*root)->word, word) > 0)
			{
				Add(&((*root)->rightChild), s, word);
			}
			else if(strcasecmp((*root)->word,word) < 0)
			{
				Add(&((*root)->leftChild), s, word);
				
			}
			else //word is already in the tree
			{
				//add current language in the array of the existing node
				int j = 0;
				while ((*root)->languages[j] != NULL)
				{
					printf("Already in %s\n", (*root)->languages[j]->language);
					j++;
				}
				(*root)->languages[j] = s;
				(*root)->languages[j+1] = NULL;
			}
		}
	}
}

//checks each word whether it's a stopword + it's in the tree
void parseTextInput()
{
	char line[100];
	char del[] = {'!', '"', '#', '%', '&', '(',')', ';', '<' ,'=', '<', '>', '?', '[' , '/',']','*', '+', ',', '-','.','\\', ':','^','_','{','}','|', '~',' ', '\0'};
	char *word;
		
	while(fgets(line, sizeof(line), stdin))
	{
		//Removes the \n from each line
		line[strcspn(line, "\n")] = 0;
		
		word = strtok(line,del);
		while (word) 
		{
			//look for it into stopword tree
			search(root, word);
			word = strtok(NULL, del);
		 }
	}
		
}

//Tests the functionality of the tree
void testLanguages(Node *root)
{
	char word[100];
	puts("Insert a stop-word you'd like to search: ");
	fgets(word, 100, stdin);
	word[strcspn(word, "\n")] = 0;
	search(root ,word);
}	

//Searches the tree for user inputted stop word
void search(Node *n, char *word)
{
	int i = 0;
	if(n == NULL)
	{
		// puts("Not found");
	}
	else
	{
		int val = strcmp(word, n->word);	
		
		if(val == 0)//FOUND IT!!!
		{
			
			while(n->languages[i] != NULL)
			{
				(n->languages[i]->count)++;
				i++;
			}

		}
		else if (val > 0)
		{
			//recursively call left child
			search(n->leftChild, word);
		}
		else //less than zero
		{
			//recursively call right child
			search(n->rightChild, word);
		}
	}
}

/* checks which one of all the languages given is the one that has the highest 
* stop count. The one with the highest corresponds with the language in the text.
*/
Lang checkMax(int langcount)
{
	int i;
	Lang currMax;
	currMax.count = 0;
	//printf("Languages: %d\n", langcount);
	for(i = 0; i < langcount; i++)
	{
		//printf("%d Pointer = %p\n", i, languageList[i]);fflush(stdout);
		if (languageList[i]->count > currMax.count)
		{
			(void)memcpy(&currMax, languageList[i], sizeof(Lang));
		}
	}
	return currMax;
}


int main(int argc, char *argv[])
{
   int langcount = 0;
   
	if ( argc !=2 ) //arg should be name of the exe + directory + txt file you want to open
	{
		printf("%d",argc);
		printf("Inside folder: %s\n", argv[0] );
	}
	else 
	{	
		DIR *dir;
		dir = opendir(argv[1]);
		
		if(dir == NULL)
		{	
			printf("Can't open directory '%s' \nThe program will terminate.", argv[1]);
			return 1;
		}
		else
		{
			char *ar = argv[1];
			langcount = ExtractStopwords(dir,ar);
		}

		//read txt file 
		parseTextInput();

		Lang max;
		max = checkMax(langcount);

		printf("Language: %s with %d stopwords found in the text\n", max.language, max.count);
	}

	//testLanguages(root);
	return 0;
}