#include <iostream> 
using namespace std;

class Node {
public:
	int data;
	Node *next;
};


/************************************************
*	Global Degisken ile listeye eleman ekleme.
*	Main de global_test fonksiyonunu çagirarak test edebilirsin.
************************************************/

Node *global_root = NULL;

void global_insert(int data) {
	Node *newnode = new Node();
	newnode->data = data;
	newnode->next = NULL;
	
	if (global_root == NULL) {
		global_root = newnode;
		return;
	}
	Node *temp = global_root;
	
	while (temp->next != NULL)
		temp = temp->next;
	
	temp->next = newnode;
}

void global_print() {
	Node *temp = global_root;
	while (temp != NULL) {
		cout << temp->data << " ";
		temp = temp->next;
	}
	cout << endl;
}

void global_test() {

	global_insert(100);

	global_insert(120);

	global_insert(140);

	global_insert(130);
	global_print();
}
/***********GLOBAL END************/



/*****************************************************************
*	Fonksiyona Root'un Adresini göndererek eleman ekleme
*	main de adres_test fonksiyonunu çagirarak test edebilirsin.
*****************************************************************/

void adres_insert(Node **head, int data) {
	Node *newnode = new Node();
	newnode->data = data;
	newnode->next = NULL;
	
	if ((*head) == NULL) {
		(*head) = newnode;
		return;
	}
	Node *temp = (*head);
	
	while (temp->next != NULL)
		temp = temp->next;
	
	temp->next = newnode;
}

void adres_print(Node *head) {
	while (head != NULL) {
		cout << head->data<<" ";
		head = head->next;
	}
	cout << endl;
}

void adres_test() {
	Node *root = NULL;
	adres_insert(&root, 10);
	adres_insert(&root, 70);
	adres_insert(&root, 50);
	adres_insert(&root, 20);
	adres_insert(&root, 40);
	adres_insert(&root, 15);
	adres_print(root);
}
/*********ADDRESS END**********/



/*************************************************************************
*	Fonksiyonu Olusturulan class(Node) tipinde yazarak eleman ekleme.
*	mainde type_test fonksiyonunu çagirarak test edebilirsin.
*************************************************************************/

Node *type_insert(Node *head, int data) {
	Node *newnode = new Node();
	newnode->data = data;
	newnode->next = NULL;

	if (head == NULL)
		return newnode;
	Node *temp = head;
	
	while (temp->next != NULL)
		temp = temp->next;
	
	temp->next = newnode;
	return head;
}

void type_print(Node *head) {
	while (head != NULL) {
		cout << head->data << " ";
		head = head->next;
	}
	cout << endl;
}

void type_test() {
	Node *root = NULL;
	root = type_insert(root, 42);
	root = type_insert(root, 54);
	root = type_insert(root, 21);
	root = type_insert(root, 17);
	root = type_insert(root, 92);
	type_print(root);
}
/***********TYPE END************/


int main() {
	global_test();
	adres_test();
	type_test();
	getchar();
	return 0;
}
