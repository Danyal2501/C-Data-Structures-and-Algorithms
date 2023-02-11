/* 
 * CSC A48 - Assignment 3 - Ingredient networks
 * 
 * In this assignment, you will practice and strengthen your
 * understanding of graph and recursion. The application 
 * you're working on is a simple network for studying
 * how food ingredients interact. 
 * 
 * This kind of network is the basic model that very
 * powerful systems such as IMB's Watson use as the 
 * basis for creating new combinations of ingredients
 * that are likely to go well together, but haven't been
 * used in previously printed recipes.
 * 
 * Your work here has several components:
 * 
 * - Understanding how the graph is represented by an
 *   adjacency matrix.
 * - Using the adjacency matrix to figure out, given
 *   an input ingredient, what other ingredients it is
 *   related to directly
 * - Using the adjacency matrix to determine, given
 *   a specific ingredient, which others are related
 *   to it directly *and* indirectly by a distance 
 *   of k hops along the graph (k>=1).
 * - Get lits of ingredients that are related, but
 *   not including ingredients from a separate
 *   list of ingredients
 * - *crunchy* Using the adjacency matrix that, given
 *   a recipe (which a list of ingredients) and 
 *   a target ingredient (part of the recipe) to
 *   replace, finds the best replacement from the
 *   remaining ingredients in the graph and updates
 *   the recipe.
 * 
 * This file contains the functions that set up
 * the adjacency matrix and the list of ingredients
 * for you. We provide you with 2 graphs, a 
 * small one with 10 ingredients (and an adjacency
 * matrix of size 10x10), and a full one with 
 * 400 ingredients (adjacency matrix is 400x400).
 * 
 * Use the 10x10 while implementing and testing your
 * solution, and only move to the full-size one once
 * you're confident your code works well.
 * 
 * Select which matrix to use by uncommenting the
 * respective #define statement below.
 * 
 * Sections where you have to implement code are
 * clearly marked
 * ********
 * TO DO:
 * ********
 * 
 * (c) F. Estrada, March 2019
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STR_LEN 1024
#ifndef __testing
//#define MAT_SIZE 10	// Use the small graph
#define MAT_SIZE 400	// Use full-size graph
#endif

// The following are GLOBAL variables you can use
// anywhere in your code. They include the 
// adjacency matrix, as well as an array of strings
// to store the ingredient names.
double AdjMat[MAT_SIZE][MAT_SIZE];
char ingredients[MAT_SIZE][MAX_STR_LEN];

// Bare-bones implementation of a linked-list of
// integers, you can use it to return the ingredient
// lists requested from functions you have to
// implement. DO NOT CHANGE THE IMPLEMENTATION
// OF THE LINKED LISTS.

typedef struct intNode_struct
{
    int  x;
    struct intNode_struct *next;
} intNode;

intNode *insertInt(intNode *h, int x)
{
  // Inserts the int 'x' into the linked list 
  // whose head is at the address in 'h'
  
  intNode *newNode;
  
  newNode=(intNode *)calloc(1,sizeof(intNode));
  newNode->x=x;
  newNode->next=NULL;
  
  if (h!=NULL)
    newNode->next=h;
  
  return newNode;
}

int searchInt(intNode *h, int x)
{
  // Search the linked list for the value 'x', if
  // found, it returns 1, else it returns 0
  while (h!=NULL)
  {
    if (h->x==x) return 1;
    h=h->next;
  }
  return 0;
}

intNode *deleteList(intNode *h)
{
  // Delete list and free memory
  intNode *q;
  if (h==NULL) return NULL;
  
  while (h!=NULL)
  {
     q=h->next;
     free(h);
     h=q;
  }
  
  return NULL;
}

// Read the adjacency matrix from file, and read the ingredient
// names
void load_ingredients(void)
{
  // Loads the adjacency matrix and ingredients list from file,
  // N must be either 10 (small graph), or 400 (full-size).
  
  FILE *f;
  int n=MAT_SIZE;
  
  if (n==10) f=fopen("AdjMat_small.dat","rb");
  else f=fopen("AdjMat_full.dat","rb");
  if (f==NULL)
  {
    printf("Can not open file with adjacency matrix. Please make sure it's in this directory\n");
    return;
  }
  fread(&AdjMat[0][0],n*n*sizeof(double),1,f);
  fclose(f);

  /*
  for (int i=0; i<MAT_SIZE; i++)
  {
    for (int j=0; j<MAT_SIZE; j++)
      printf("%2.2lf, ",AdjMat[i][j]);
    printf("\n");
  }
  */
  
  if (n==10) f=fopen("Ingredient_names_small.txt","r");
  else f=fopen("Ingredient_names_full.txt","r");
  if (f==NULL)
  {
    printf("Can not open file with ingredient names. Please make sure it's in this directory\n");
    return;
  }
  printf("Reading ingredient names. Available ingredient are:\n");
  for (int i=0; i<n; i++)
  {
    fgets(&ingredients[i][0],MAX_STR_LEN,f);
    for (int j=0; j<strlen(ingredients[i]); j++)
      if (ingredients[i][j]==10||ingredients[i][j]==13) ingredients[i][j]=0;
    printf("%s\n",ingredients[i]);
  }
  fclose(f);
}

/***************************************************************
 * The part of the assignment you need to implement starts below
 ***************************************************************/

int used[MAT_SIZE] = {0};

void print_ingredients(intNode *h)
{
    /*
     * This function prints out all the ingredients corresponding
     * to the ingredient indexes stored in the linked list
     * whose head is 'h'.
     */
    
    /*****
     * TO Do:
     * Complete this function
     *****/
	 intNode *temp = h;
	 temp = h;

	 while (temp!=NULL){
		 printf("%s\n", ingredients[temp->x]);
		 temp = temp->next;
	 }

}

int ingredient_index(char source_ingredient[MAX_STR_LEN])
{
    /*
     * This function looks into the array with ingredient
     * names for one that matches the requested 
     * 'source_ingredient'. If a match is found, it
     * returns the index at which the matching ingredient
     * name was found.
     * 
     * If the 'source_ingredient' is *not* in the ingredients
     * array, the function returns -1
     */
    
	for (int i=0; i<MAT_SIZE; i++){
		if (strcmp(source_ingredient, ingredients[i]) == 0){
			return i;
		}
	}
	
	
    /******
     * TO Do:
     * Implement this function
     *****/
     return -1;
}

void related_ingredients(char source_ingredient[MAX_STR_LEN])
{
	
	
    /*
     * This function prints out all the ingredients related
     * to the source ingredient. 
     * 
     * One per line, with no additional characters or symbols.
     *
     * Format of the adjacency matrix:
     * 
     * AdjMat[i][j] = 0 if two ingredients i and j are not directly linked
     *                (no edge in the graph between them, this means
     *                 they never appeared together in one recipe)
     * AdjMat[i][j] > 0 if two ingredients i and j are neighbours (they
     * 			appeared together in at least a recipe). The
     * 			actual value indicates how many times these two
     * 			ingredients appeared together in a recipe
     * 
     * Make sure you understand what this is telling you about the
     * ingredients in your graph. You'll need it later
     */
    
    /**********
     * TO DO:
     * Implement this function
     * ********/
	 
	 int index = ingredient_index(source_ingredient);
	 
	 for (int i=0; i<MAT_SIZE; i++){
		 if (AdjMat[i][index] > 0){
			 printf("%s\n", ingredients[i]);
		 }
	 }

}

int reachDistance[MAT_SIZE] = {0};

intNode *related_k_dist(intNode *h, char source_ingredient[MAX_STR_LEN], int k, int dist)
{
    /*
     * This function determines the ingredients related to the
     * specified source ingredient by a distance of at most k.
     * k >= 1
     * 
     * For instance, if k=2, the list should contain all the
     * ingredients who are neighbours of source_ingredient[]
     * as well as the ingredients that are neighbours of
     * the neighbours (distance=2).
     * 
     * If k=3, then we want source_ingredients' neighbours, its
     * neighbours' neighbours, and its neighbours' neighbours'
     * neighbours. And so on.
     * 
     * Obviously, you'll have to employ recursion carefully.
     * 
     * Ingredients are returned as *indexes*, so, for instance,
     * if we find a related ingredient 'chicken' is stored at
     * index 7 in ingredients[][], then we store 7 in the
     * linked list of related ingredients.
     * 
     * The returned list MUST CONTAIN NO DUPLICATES.
     * 
     * And be smart about it, or you'll end up in an infinite
     * recursion! So think carefully about the base case(s)
     * and about what the recursive case must do.
     *
     * Example call:
     * 
     * Our test code may call your function in this way:
     * 
     * intNode *head=NULL;
     * head=related_k_dist(head,"rice",2,0);
     * 
     * After that call, 'head' must point to a linked list with
     * all the ingredients related to 'ride' up to a distance of
     * 2 away.
     * 
     * It's up to *you* to figure out what the 'dist' parameter
     * is for!
     * 
     */
	 
	 if (k > MAT_SIZE){
		 k = MAT_SIZE;
	 }
	 
	 if (dist == 0){
		for (int i=0; i<MAT_SIZE; i++){
			reachDistance[i] = MAT_SIZE+1;
		}
	}
	 
	 int index = ingredient_index(source_ingredient);
	 

	 if (dist > k+1){
		 return h;
	 }
	 
	 if(searchInt(h, index) == 0 && dist !=0){
		 h=insertInt(h, index);
	 }

	 for (int i=0; i<MAT_SIZE; i++){
		 if (AdjMat[index][i] > 0 && reachDistance[i] > dist){
			 reachDistance[i] = dist;
			 h = related_k_dist(h, ingredients[i], k, dist+1);
		 }
	 }
    
    /*******
     * TO DO:
     * Complete this function
     *******/
	 
	if (dist == 0){
		for (int i=0; i<MAT_SIZE; i++){
			reachDistance[i] = MAT_SIZE+1;
		}
	}
    return h;
}

intNode *related_with_restrictions(char source_ingredient[MAX_STR_LEN], char avoid[MAX_STR_LEN], int k_source, int k_avoid)
{

	intNode *sourceList = NULL;
	intNode *avoidList = NULL;
	intNode *restricted = NULL;
	
	sourceList = related_k_dist(sourceList, source_ingredient,k_source,0);
	avoidList = related_k_dist(avoidList, avoid, k_avoid,0);
	avoidList = insertInt(avoidList, ingredient_index(avoid));
	intNode *temp = sourceList;

	while(temp !=NULL){
		if (searchInt(avoidList, temp->x) == 1){
			temp = temp->next;
			continue;
		}
		restricted = insertInt(restricted, temp->x);
		temp = temp->next;	
	}
	
	deleteList(sourceList);
	deleteList(avoidList);
	
	return restricted;
	
    /*
     * This function returns a linked list that contains the indexes of
     * all ingredients related to source_ingredient[] with a distance
     * of at most k_source.
     * 
     * BUT, the list *must not contain* any ingredients related to avoid[]
     * (including avoid[] itself) by a distance of up to k_avoid.
     * 
     * Example:
     * 
     * intNode *head=NULL;
     * head=related_with_avoidance("rice", "nuts", 2, 0);
     * 
     * Should return a list of ingredients related to 'rice' by a distance 
     * of up to 2, NOT INCLUDING 'nuts'.
     * 
     * intNode *head=NULL;
     * head=related_with_avoidance("rice", "chicken", 2, 1);
     * 
     * Should return a list of ingredients related to 'rice' by a distance
     * of up to 2, NOT INCLUDING 'nuts' and any ingredients related
     * to 'nuts' with a distance of up to 1.
     * 
     */
    
    /****
     * TO DO:
     * Implement this function
     *****/
}

int checkArray(char recipe[10][MAX_STR_LEN], char ingredient[MAX_STR_LEN]){
	for (int i=0; i<10 || strcmp(recipe[i],"") != 0; i++){
		if (strcmp(recipe[i], ingredient) == 0){
			return 1;
		}
	}
	return 0;
}

void substitute_ingredient(char recipe[10][MAX_STR_LEN], char to_change[MAX_STR_LEN])
{
  /*
   * *CRUNCHY!*
   * 
   * This function takes a recipe (which is an array of ingredients with up to 10
   * ingredients), and replaces the one in 'to_change' by *the most compatible
   * ingredient from the graph*
   * 
   * By most compatible, we mean the ingredient that appears most often with 
   * the remaining ingredients in the recipe.
   * 
   * The definition above may seem fuzzy, but it's not if you consider that the
   * weights in the adjacency matrix are meaningful.
   * 
   * As you see, nothing is returned - the result of this function is that the
   * ingredient 'to_change' is replaced in the recipe by the most compatible
   * ingredient given the graph that is *not already in the recipe*.
   * 
   * Assume that the input recipe is such that *all ingredients are in the
   * array of ingredients* (i.e. you don't have to worry about ingredients
   * that don't exists).
   * 
   * However, the recipe *may have less than 10 ingredients*, in which case
   * unused ingredients will be set to empty strings ("")
   */
  
   /*******
    * TO DO:
    * Complete this function!
    ******/
	
	int counter[MAT_SIZE] = {0};
	int change = 0;
	int recipeNum[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	int numIngredients = 0;
	int max=-1;
	
	if (checkArray(recipe, to_change) == 0){
		return;
	}
	
	for (int i=0; i<10 && strcmp(recipe[i],"") != 0; i++){
		numIngredients++;
	}

	if(MAT_SIZE <= numIngredients){
		return;
	}
	
	for (int i=0; strcmp(recipe[i],"") != 0; i++){
		recipeNum[i] = ingredient_index(recipe[i]);
	}
	
	for (int i=0; recipeNum[i] != -1; i++){
		if (strcmp(ingredients[recipeNum[i]], to_change) != 0){
			for (int j=0; j<MAT_SIZE; j++){
				counter[j] += AdjMat[recipeNum[i]][j];
			}
		}
	}
	
	for (int i=0; i<MAT_SIZE; i++){
		if (((counter[i] > 0 && max == -1) ||counter[i] > counter[max] ) && checkArray(recipe, ingredients[i]) == 0){
			max = i;
		}
	}
	
	if (max == -1){
		return;
	}
	
	for (int i=0; i<10; i++){
		if (strcmp(recipe[i], to_change) == 0){
			change = i;
			break;
		}
	}
	
	strcpy(recipe[change], ingredients[max]);

}
