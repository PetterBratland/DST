#ifndef LINKEDLIST_H
  #define LINKEDLIST_H


struct LinkedList;
struct LinkedList *createNode();
void appendNode(struct LinkedList **head, int id, double sensorData);
void removeLastNode(struct LinkedList **head);
void printList(struct LinkedList *head);
void fillList(void);
void printElement2Screen(struct LinkedList *head);


#endif