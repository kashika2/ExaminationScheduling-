#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#include<time.h>

#define MAX_COURSE_COUNT 32
#define MAX_COURSE_NAME_LENGTH 32

typedef struct Graph_Vertex{             //typedef is used here to make  Graph_Vertex = Vertex;
    char name[32];
    char color[32];                      //Here a new (abstract) data type is made having two char arrays and one boolean variable;
    bool visited;
}Vertex;



typedef struct Students
{
    char course[32];
    int no;
}students;


char s[5];
bool isCourseExist(char *);
int **createEdges(FILE *);
int **createEdgeBetween(int **, char coursesOneStudentTakes[MAX_COURSE_COUNT][MAX_COURSE_NAME_LENGTH], int );
void readFromFile(FILE *);
void ReadFromFile2(FILE *,students **);
void readFromFile3(FILE *);
void calculateStaff(int); 
void random1(void);
void RoomCalculator(int,students **);
int getAdjUnvisitedVertex(int , int **, Vertex **);
void cleanUpVertex(Vertex *list[]);
void cleanUpEdge(int **);
void push(int);
int pop();
int peek();
bool isStackEmpty();
int determineColor(const int *);
Vertex *coloring(Vertex **, int **);
Vertex *DFS(Vertex **, int **, int);
Vertex *createVertexes(Vertex **);
Vertex *addVertex(char *);
Vertex *sortVertexes(Vertex **);

//Course initialization.
int courses[MAX_COURSE_COUNT][MAX_COURSE_NAME_LENGTH];
int courseCount = 0;

//Necessary variables for DFS.
int stack[MAX_COURSE_COUNT];
int top = -1;

//COLORS IN ORDER
//YELLOW - RED - GREEN - BLACK - PINK - WHITE - BLUE - BROWN - GRAY - ORANGE
char colors[10][10] = {{"Yellow"},{"Red"},{"Green"},{"Black"},{"Pink"},{"White"},{"Blue"},{"Brown"},{"Gray"},{"Orange"}};

int totalRooms = 0;  //total number of rooms 
int count = 0, reqStaff;  //to count total staff from file staff and faculties required for invigilation 

int main() {
    
    int i, j, k, capacity,input;
    float z;
    int ii;
    int year;
    char str[32];
    FILE *fp1;
    int staff;    //required staff- user input
	FILE *fp3;
    
    //Creating a file pointer.
    FILE *fp;
    fp = fopen("Data 2.txt", "r");   //fp read the full file here. It has every data of the file;
    if(fp==NULL)
    {
        printf("File not found\n");
        return 0;
    }
    readFromFile(fp);  //after this function course array will be filled with names of the unique courses.;
    
    //Graph initializations. We need to do this after reading our file.
    printf("\nNo. of distinct courses : %d \n",courseCount);   //this line will tell us no. of distinct courses;
    students *box[courseCount];       //box is an array of pointers of size courseCount which contains pointers that points to each value in structure students 
    
    for(i=0;i<courseCount;i++)
    {
        box[i]=malloc(sizeof(students));
    }
    
    
    
    Vertex *list[courseCount];    //an array of Vertex data type will be created with having courseCount no. of elements in it.;
    int **adjMatrix;
    
    //Create vertices with classes.
    *list = createVertexes(list); //After the execution of this line of code courseCount no. of boxes will be created. Each box will have name = coursename,color = Null, visited = false;
    //we can also say that malloc is called in these function to dynamically add vertexes according to courseCounts.;
    //for example list[0] will contain data of a vertex data type such that name = daa , color= Null, visited = false;
    //Sorting vertices.
    *list = sortVertexes(list);     //This functions sorts the courses names in the list.;
    
    //Creating edges.
    adjMatrix = createEdges(fp);      //this function is used to add edges between vertices. means those vertices whose exam can't happen on same day.;
    
    ////PRINTING THE ADJACENCY MATRIX.
    printf("\n\t");
    
    for(i = 0; i < courseCount + 1; i++){
        
        for(j = 0; j < courseCount + 1; j++){
            
            if(i == 0 && j != courseCount)
                printf("%s\t", list[j] -> name);
            else if(j == 0)
                printf("%s\t", list[i - 1] -> name);
            if(i != 0 && j != 0)
                printf("   %d\t", adjMatrix[i - 1][j - 1]);
            
        }
        printf("\n");
    }
    printf("\n\n");
    /////PRINTING THE ADJACENCY MATRIX.*/
    
    
    
    /*************************************************************DONE TILL HERE ON 4th OCT 2020*******************************************************************************/
    
    
    
    
    //Coloring Algorithm.
    *list = coloring(list, adjMatrix);
    
    //Let's find how many colors used.
    int numberOfColorsUsed = 0;
    for(i = 0; i < courseCount; i++){
        
        for(j = 0; j < 10; j++)
            if(strcmp(list[i] -> color, colors[j]) == 0 && numberOfColorsUsed < j + 1)
                numberOfColorsUsed = j + 1;
    }
    //printf("no. of colors used are : %d\n",numberOfColorsUsed);
    
    
    /////PRINTING FINAL EXAM PERIODS
    for(i = 0; i < numberOfColorsUsed; i++){
        
        printf("Final Exam Period %d -> ", (i + 1));
        
        for(j = 0; j < courseCount; j++){
            
            if(strcmp(list[j] -> color, colors[i]) == 0){
                printf("%s", list[j] -> name);
                
                for(k = j + 1; k < courseCount; k++){
                    
                    if(strcmp(list[k] -> color, colors[i]) == 0){
                        printf(", ");
                        break;
                    }
                }
            }
        }
        
        printf("\n");
    }
    printf("\n");
    /////PRINTING FINAL EXAM PERIODS
    
    cleanUpVertex(list);
    cleanUpEdge(adjMatrix);
    fclose(fp);
    fp1 = fopen ("Data 3.txt", "r");
    ReadFromFile2(fp1,box); //check no. of students for each courses
        
        printf("Enter your Choice : \nPress 1 to calculate rooms required with 50 percent capacity for each exam and to sign invigilation duties.\nPress 2 to view distinct courses and the strength of each course.\nPress 3 to exit.\n");
		scanf("%d",&input);

        switch(input)
        {
            case 1:
            {
                printf("\n");
                printf("Enter the capacity of each room available for examination\n");
                scanf("%d",&capacity);
                RoomCalculator(capacity,box);  //Function to calculate no. of rooms reqd for each exam
                //------------FOR INVIGILATION DUTIES-----------------
                
                 fp3 = fopen("STAFF.txt", "r");   //fp read the full file here. It has every data of the file;
                 if(fp3==NULL)
                 {
                     printf("File not found\n");
                     return 0;
                 }
                 
                 readFromFile3(fp3);
                 printf("\n");
                 
                 //After getting data from file and counting how many total faculties are there
                 //take input from user that what percentage of total faculties they need for invigilation
                 printf("What percentage of the total staff you need for invigilation?\n");
                 scanf("%d",&staff);
                 calculateStaff(staff);
                 
                if(reqStaff < totalRooms)
                { 
                    printf("Staff is less than total rooms. You need more staff!\n");
	            }
             	else
	            { 
	               random1(); 
                }
            
                 
                //------------FOR INVIGILATION DUTIES-----------------
               break;
            }
            case 2:
                {
                for(i=0;i<courseCount;i++) //box array contains course name and the strength of students enrolled in that course
                {
                    printf("Course = %s   No. of Students = %d  \n",box[i]->course,box[i]->no);
                }
                break;
                }
            case 3:
                {
                    exit(1);
                } 
            default:
                {
                printf("INVALID CHOICE\n");
                exit(1);
                }
                
    }   //End of switch case loop
}



void readFromFile(FILE *fp)
{
    
    char input[256], word[256];
    char *token;
    
    while(!feof(fp))
    {
        
        if(fgets(input, 512, fp) != NULL)  //For reading a string value with spaces, we can use either gets() or fgets()
        {                                  //  printf("string is: %s\n", input);   //string is: Dhruv Gupta:MATHS,DAA,PDS , and more because eof is used in 155.input contain every string
            //printf("string is: %s\n", input);
            strcpy(word, strtok(input, "\n"));   //here word array contains the whole complete line of a string eg: Dhruv Gupta:MATHS,DAA,PDS
            strtok(word, ":");                   //here word array contains only name of the student
            token = strtok(NULL, ",");          //these two lines will break the string into courses names
            //token variable will contain the courses for each individual here
            
            while(token != NULL)
            {
                
                // printf("Token = %s\n",token);       //when while loopon 155 operates for the first time, it will give: string is: Dhruv Gupta:MATHS,DAA,PDS
                // Token = MATHS
                //Token = DAA
                //  Token = PDS
                if(isCourseExist(token) == false)
                {
                    strcpy((char *)courses[courseCount], token);  //this line of code copies token value (course name) into courses array if it is not already there
                    courseCount++;
                }
                
                token = strtok(NULL, ",");    //it changes token from MATHS to DAA here and so on
            }
        }
    }
}

bool isCourseExist(char *course){     //this function checks whether course name is already in the courses array if not it returns false. so that course is added to the array
    
    int i;
    
    for(i = 0; i < courseCount; i++)
    {
        
        //Means there is a course with the same name.
        if(strcmp((const char *)courses[i], course) == 0)    //strcmp returns 0, if both strings are identical (equal)
        {
            return true;
        }
    }
    return false;   //Means there is no course with the same name.
}

Vertex *createVertexes(Vertex **list){
    
    int i;
    
    for(i = 0; i < courseCount; i++){
        
        list[i] = addVertex((char *) courses[i]);
        
    }
    
    return *list;
}

Vertex *addVertex(char *name){
    
    Vertex *newVertex = malloc(sizeof(Vertex));
    strcpy(newVertex->name, name);
    strcpy(newVertex->color, "NULL");
    newVertex -> visited = false;
    
    return newVertex;
}

Vertex *sortVertexes(Vertex **list){
    
    char tempChar[32];
    
    int i, j;
    
    for(i = 0; i < courseCount - 1; i++){
        
        for(j = 0; j < courseCount - i - 1; j++){
            
            if(strcmp(list[j] -> name, list[j + 1] -> name) > 0){
                
                strcpy(tempChar, list[j] -> name);
                strcpy(list[j] -> name, list[j + 1] -> name);
                strcpy(list[j + 1] -> name, tempChar);
                strcpy((char *) courses[j], (const char *) courses[j + 1]);
                strcpy((char *) courses[j + 1], tempChar);
                
            }
        }
    }
    
    return *list;
}

int **createEdges(FILE *fp)
{
    
    rewind(fp);    //moves the file pointer at beginning of the file
    
    //Creating an adj matrix.
    int **adjMatrix, i, j;
    adjMatrix = malloc(sizeof(int*) * courseCount);      //from line 249 to line 265 adjacency matrix is created with courseCount*courseCount no.'s and initialised to 0
    
    for(i = 0; i < courseCount; i++)
        adjMatrix[i] = malloc(sizeof(int*) * courseCount);
    
    for(i = 0; i < courseCount; i++)
    {
        for(j = 0; j < courseCount; j++)
            adjMatrix[i][j] = 0;              //0 is initialised to every edge as of now
    }
    
    char input[256], word[256], *token;
    char coursesOneStudentTakes[MAX_COURSE_COUNT][MAX_COURSE_NAME_LENGTH];
    int numberOfCoursesOneStudentTakes = 0;
    
    while(!feof(fp)){
        
        if(fgets(input, 512, fp) != NULL){
            
            numberOfCoursesOneStudentTakes = 0;
            
            strcpy(word, strtok(input, "\n"));  //word takes one whole line here
            strtok(word, " ");             //it takes first name of every person from the file
            strtok(NULL, ":");
            token = strtok(NULL, ",");  //Every string is breaking down from here. Token takes course names.
            
            while(token != NULL){
                //printf("Token = %s\n",token);
                
                strcpy(coursesOneStudentTakes[numberOfCoursesOneStudentTakes], token);  //coursesOneStudentTakes[numberOfCoursesOneStudentTakes] this array will contain max no. of courses taken by any student. for example Dhruv took maths,DAA,PDS so this array will contain these 4 courses
                numberOfCoursesOneStudentTakes++;
                
                
                token = strtok(NULL, ",");
                // printf("------------------------------------------------\n");
                // printf("courseOneStudentTakes = %s  %s  %s  %s\n",coursesOneStudentTakes[0],coursesOneStudentTakes[1],coursesOneStudentTakes[2],coursesOneStudentTakes[3]);
                
            }
            // printf("courseOneStudentTakes = %s  %s  %s  %s\n",coursesOneStudentTakes[0],coursesOneStudentTakes[1],coursesOneStudentTakes[2],coursesOneStudentTakes[3]);
            
            adjMatrix = createEdgeBetween(adjMatrix, coursesOneStudentTakes, numberOfCoursesOneStudentTakes);
        }
    }
    
    return adjMatrix;
}

int **createEdgeBetween(int **adjMatrix, char coursesOneStudentTakes[MAX_COURSE_COUNT][MAX_COURSE_NAME_LENGTH], int numberOfCoursesOneStudentTakes){
    
    int i, j, correctIndexes[courseCount];
    
    for(i = 0; i < courseCount; i++){
        
        correctIndexes[i] = 0;
        
    }
    
    for(i = 0; i < courseCount; i++){
        
        for(j = 0; j < numberOfCoursesOneStudentTakes; j++){
            
            if(strcmp((char *)courses[i], coursesOneStudentTakes[j]) == 0){
                
                correctIndexes[i] = 1;
                
            }
        }
    }
    
    for(i = 0; i < courseCount; i++){
        
        if(correctIndexes[i] == 1){
            
            for(j = 0; j < courseCount; j++) {
                
                if(correctIndexes[j] == 1 && i != j) {
                    
                    adjMatrix[i][j] = 1;
                    adjMatrix[j][i] = 1;    //here edge is added between vertices.
                    
                }
                
            }
        }
    }
    
    return adjMatrix;
}

//Stack Functions.
void push(int item){
    
    stack[++top] = item;
    
}

int pop(){
    
    return stack[top--];
}

int peek(){
    
    return stack[top];
}

bool isStackEmpty(){
    
    return top == -1;
}

int getAdjUnvisitedVertex(int vertexIndex, int **adjMatrix, Vertex **list){
    
    int i;
    
    for(i = 0; i < courseCount; i++){
        
        if(adjMatrix[vertexIndex][i] == 1 && list[i] -> visited == false)
            return i;
        
    }
    
    return -1;
}

Vertex *coloring(Vertex **list, int **adjMatrix){
    
    int i;
    
    for(i = 0; i < courseCount; i++){
        
        if(list[i] -> visited == false)
            *list = DFS(list, adjMatrix, i);
        
    }
    
    return *list;
}

Vertex *DFS(Vertex **list, int **adjMatrix, int firstIndex){
    
    //An array to hold the colors around the vertex.
    int colorsAround[10];
    
    //I will traverse the graph using Depth First Search(DFS) algorithm.
    int i, j;
    //First, we mark the first vertex as visited.
    list[firstIndex] -> visited = true;
    
    //Set the first vertexes color to yellow.
    strcpy(list[firstIndex] -> color, colors[0]);
    
    //Pushing the first vertexes index into stack.
    push(firstIndex);
    
    while(!isStackEmpty()){
        
        //Initializing colorsAround array.
        for(i = 0; i < 10; i++)
            colorsAround[i] = 0;
        
        //Get the unvisited vertex of vertex which is at the top of the stack.
        int unvisitedVertex = getAdjUnvisitedVertex(peek(), adjMatrix, list);
        
        //If no adjacent vertex found...
        if(unvisitedVertex == -1)
            pop();
        else{
            
            list[unvisitedVertex] -> visited = true;
            push(unvisitedVertex);
            //Now, we need to determine this vertexes color.
            for(i = 0; i < courseCount; i++){
                
                if(adjMatrix[unvisitedVertex][i] == 1){
                    
                    for(j = 0; j < 10; j++){
                        
                        if(strcmp(list[i] -> color, colors[j]) == 0)
                            colorsAround[j]++;
                        
                    }
                }
            }
            
            if(determineColor(colorsAround) == -1)
                strcpy(list[unvisitedVertex] -> color, "NULL");
            else
                strcpy(list[unvisitedVertex] -> color, colors[determineColor(colorsAround)]);
            
        }
    }
    
    return *list;
}

int determineColor(const int *colorsAround){
    
    if(colorsAround[0] != 0)
        if(colorsAround[1] != 0)
            if(colorsAround[2] != 0)
                if(colorsAround[3] != 0)
                    if(colorsAround[4] != 0)
                        if(colorsAround[5] != 0)
                            if(colorsAround[6] != 0)
                                if(colorsAround[7] != 0)
                                    if(colorsAround[8] != 0)
                                        if(colorsAround[9] != 0)
                                            return -1;
                                        else
                                            return 9;
                                        else
                                            return 8;
                                        else
                                            return 7;
                                        else
                                            return 6;
                                        else
                                            return 5;
                                        else
                                            return 4;
                                        else
                                            return 3;
                                        else
                                            return 2;
                                        else
                                        {
                                            return 1;
                                        }
    return  0;
}

void cleanUpVertex(Vertex *list[]){
    
    int i;
    
    for(i = 0; i < courseCount; i++){
        
        free(list[i]);
        
    }
}

void cleanUpEdge(int **adjMatrix){
    
    int i;
    
    for(i = 0; i < courseCount; i++){
        
        free(adjMatrix[i]);
        
    }
    
    free(adjMatrix);
    
}

//-----------------------------------ROOM CALCULATION-----------------------

void ReadFromFile2(FILE *fp,students **box) //function to read no. of students enrolled in a particualr course
{
    char str[32];
    int strength,i=0;
    while(!feof(fp))
    {
        fscanf(fp, "%s %d", str,&strength);
        //printf("Read String1 |%s|\n", str );
        //printf("Read String2 |%d|\n", strength );
        strcpy(box[i]->course,str);
        box[i]->no=strength;
        i++;
        
    }
    
}
void RoomCalculator(int capacity,students **box)
{    int i; 
     float z=capacity/2;
     int ab;  
    for(i=0;i<courseCount;i++)
    {
	   ab = ceil((box[i]->no/z)); 
	   printf("Minimum no. of rooms required with 50 percent capacity of each room for %s exam is %d\n",box[i]->course,ab);
	   totalRooms = totalRooms+ab; 
    }
    printf("Therefore, total rooms required are %d",totalRooms);
    printf("\n\n");
}

//------------------------------------INVIGILATION-----------------------------------

void readFromFile3(FILE *fp3)
{
    int i; 
    char input[260], word[260]; 
    //char *token;
    
    while(!feof(fp3))
    {   
        if(fgets(input, 520, fp3) != NULL)  //For reading a string value with spaces, we can use either gets() or fgets()
        {                                   //input is: A1:A , and more because eof is used in 155.input contain every string
            //printf("input is: %s", input);
            
			strcpy(word, strtok(input, ":"));   //word array contains the id like A1, A2 etc of the faculties 
           // printf("ID is: %s\n",word); 
            //printf("\n");
            count++;   //to get the total staff 
        }
    }
      printf("Total staff: %d",count);   //Total staff: 40 
} 
 
void calculateStaff(int staff)
{   
     
	reqStaff = ceil(count * ((float)staff/100));    //use float to avoid implicit type casting 
	printf("Required staff = %d members\n \n",reqStaff);        //we get the number of faculties we need 
}

void random1()
{
	int ranArr[reqStaff], i;      //ranArr will store random numbers generated by rand()
	
	srand(time(NULL));        //to generate different numbers everytime 
	for(i=0; i<reqStaff; i++)      //generating random numbers and storing in ranArr 
	{  ranArr[i] = rand() % count + 1; }
	
	printf("Invigilation duties are assigned to faculties: \n"); 
	for(i=0; i<reqStaff; i++)          //printing the array containing random numbers 
    {  printf("A%d\n",ranArr[i]); }
}




