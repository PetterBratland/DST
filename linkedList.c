#include "allLib.c"

double max = -DBL_MAX;
double minv = DBL_MAX;
float total = 0;
int count = 0;
int dayBefore = 0;
// Definition av en nod i en länkad lista
struct LinkedList
{
  float maxValue;
  float minValue;
  float avgValue;
  int id;                         // ID för noden
  char timestamp[40];             // Används för att spara tidsstämpel
  char timestampMax[40];
  char timestampMin[40];
  double sensorData;              // Data i noden
  struct LinkedList *next;        // Pekare till nästa nod
};

//The sectret sauce
float findAverage(struct LinkedList *head) {
  total += head->sensorData;
  count++;
  return total / count;
  
}


struct LinkedList *WeeklySaveData(int id, double minSave, double maxSave, double avgSave){
  
  struct LinkedList *node = (struct LinkedList*)malloc(sizeof(struct LinkedList));
  
  node->id = id;
  node->maxValue = maxSave;
  node->minValue = minSave;
  node->avgValue = avgSave;
  
  return node;
  
}

// Skapa en ny nod
struct LinkedList *createNode(int id, double sensorData)
{
  struct LinkedList *newNode = (struct LinkedList *)malloc(sizeof(struct LinkedList));
  if (newNode == NULL)
  {
    printf("Memory allocation failed\n");
    return NULL;
  }
  
  // Fyll noden med data
  newNode->id = id;
  newNode->sensorData = sensorData;
  sprintf(newNode->timestamp, "Date:%d/%d/%d, Time: %d/%d/%d", day, month, year, hour, min, sec);
  newNode->next = NULL;
  
  return newNode;
}

// Lägg till en ny nod i slutet av den befintliga listan
void appendNode(struct LinkedList **head, int id, double sensorData)
{
  // Skapa en ny nod
  struct LinkedList *newNode = createNode(id, sensorData);
  if (newNode == NULL)
    return;
  
  // Om listan är tom, gör den nya noden till listans huvud
  if (*head == NULL)
  {
    *head = newNode;
    return;
  }
  
  // Annars hitta den sista noden och lägg till den nya noden
  struct LinkedList *lastNode = *head;
  while (lastNode->next != NULL)
  {
    lastNode = lastNode->next;
  }
  lastNode->next = newNode;
}

// Ta bort den sista noden från listan
void removeLastNode(struct LinkedList **head)
{
  if (*head == NULL)
  {
    printf("List is empty\n");
    return;
  }
  
  struct LinkedList *prevNode = NULL;
  struct LinkedList *currentNode = *head;
  
  while (currentNode->next != NULL)
  {
    prevNode = currentNode;
    currentNode = currentNode->next;
  }
  
  prevNode->next = NULL;
  free(currentNode);
}

// Skriv ut innehållet i listan
void printList(struct LinkedList *head)
{
  struct LinkedList *current = head;
  while (current != NULL)
  {
    printf("ID: %d, Sensor Data: %.2f, %s\n", current->id, current->sensorData, current->timestamp);
    current = current->next;
  }
}


unsigned short Rvar = 0;

void fillList(void){
  struct LinkedList *head = NULL;
  if(tempflag==1){
    tempflag = 0;
    appendNode(&head, Rvar, tempValue());
    Rvar++;
    printElement2Screen(head);
    removeLastNode(&head); 
  }
}

/* Keeps the max value "todo save it and reshresh everyday"*/
double findMax(struct LinkedList *head)
{
  struct LinkedList *current = head;
  if (current->sensorData > max)
  {
    max = current->sensorData;
    sprintf(current->timestampMax, " Time: %02d/%02d/%02d", hour, min, sec);
  }
  return max;
}


/* Keeps the min value "todo save it and reshresh everyday"*/
double findMin(struct LinkedList *head)
{
  
  struct LinkedList *current = head;
  if (current->sensorData < minv)
  {
    minv = current->sensorData;
    sprintf(current->timestampMin, " Time: %02d/%02d/%02d", hour, min, sec);
  }
  return minv;
}

unsigned char daySave = 0;

void printElement2Screen(struct LinkedList *head){
  struct LinkedList *saveNode;
  struct LinkedList *element = head;
  double findMin2 = findMin(element);
  double findMax2 = findMax(element);
  double findAverage2 = findAverage(element);
  
  if(dayBefore != day){
    saveNode = WeeklySaveData(daySave,findMin2,findMax2,findAverage2);
    displayPos((0+daySave*11),6);
    char savemin[11];
    sprintf(savemin,"Day%d:%0.2f ",daySave+1,saveNode->minValue);
    for(int i = 0; i < 11 ; i++){
      Write_Data_2_Display(savemin[i]-0x20);
      Write_Command_2_Display(0xc0); //Move to next slot
    }
    displayPos((0+daySave*11),7);
    char savemax[11];
    sprintf(savemax,"Day%d:%0.2f ",daySave+1,saveNode->maxValue);
    for(int i = 0; i < 11 ; i++){
      Write_Data_2_Display(savemax[i]-0x20);
      Write_Command_2_Display(0xc0); //Move to next slot
    }
    
    dayBefore = day;
    daySave++;
    total = 0;
    count = 0;
    max = -DBL_MAX;
    minv = DBL_MAX;
  }
  
  
  
  
  displayPos(0,8);
  
  Write_Data_2_Display(0x34); //T
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x45); //e
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x4D); //m
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x50); //p
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x1A); //:
  Write_Command_2_Display(0xc0); //Move to next slot
  char tempstr[10];
  sprintf(tempstr,"%0.2f",element->sensorData);
  for(int i = 0; i < 5 ; i++){
    Write_Data_2_Display(tempstr[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
  
  Write_Data_2_Display(0x21); //  A
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x56);  // v
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x47); // g
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x1A); // :
  Write_Command_2_Display(0xc0); //Move to next slot
  
  char avgstr[10];
  sprintf(avgstr,"%0.2f",findAverage2);
  for(int i = 0; i < 5 ; i++){
    Write_Data_2_Display(avgstr[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
  
  displayPos(0,9);
  
  Write_Data_2_Display(0x2D); //  M
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x41);  // a
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x58); // x
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x1A); // :
  Write_Command_2_Display(0xc0); //Move to next slot
  char maxstr[10];
  sprintf(maxstr,"%0.2f",findMax2);
  for(int i = 0; i < 5 ; i++){
    Write_Data_2_Display(maxstr[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
  
  for(int i = 0; i < 15 ; i++){
    Write_Data_2_Display(element->timestampMax[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
  
  displayPos(0,10);
  
  Write_Data_2_Display(0x2D); //  M
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x49);  // i
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x4E); // n
  Write_Command_2_Display(0xc0); //Move to next slot
  Write_Data_2_Display(0x1A); // :
  Write_Command_2_Display(0xc0); //Move to next slot
  
  char minstr[10];
  sprintf(minstr,"%0.2f",findMin2);
  for(int i = 0; i < 5 ; i++){
    Write_Data_2_Display(minstr[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
  
  for(int i = 0; i < 15 ; i++){
    Write_Data_2_Display(element->timestampMin[i]-0x20);
    Write_Command_2_Display(0xc0); //Move to next slot
  }
  
}
