#include <stdio.h>
#include <stdlib.h>
int employee[6] = { 1,2,3,4,5,6 };//stack
int top = 5, Time[6][2] = { 0 };
struct nodeCustomer {
	int id;
	int offset;//iþlemin baþladýðý zaman
	int time;
	struct nodeCustomer *next;
};

int main() {
	struct nodeCustomer *head = (struct nodeCustomer*)malloc(sizeof(struct nodeCustomer));
	struct nodeCustomer *rear = head, *front = head, *temp;
	int id, time, offset;
	scanf("%d", &id);
	while (id != -1) {
		scanf("%d%d", &offset, &time);
		rear->id = id;
		rear->offset = offset;
		rear->time = time;
		scanf("%d", &id);
		if (id != -1) {
			rear->next = (struct nodeCustomer*)malloc(sizeof(struct nodeCustomer));
			rear = rear->next;
		}
		rear->next = NULL;
	}
	int x = 1;//dakika degiþimi
	int pos = 0;//müþteri bekleme süresi
	while (front != NULL) {
		if (x != front->offset || top == -1) {
			for (int i = top + 1; i < 6; i++) {
				int k = front->offset - x;//kullanýcý  bir dakikadan daha fazla artýþ yapabilir.
				if (k == 0) k = 1;//top==-1 (?)
				Time[i][0] -= k;
				if (Time[i][0] == 0)
					employee[++top] = i + 1;
				//printf("Top:%d, Time[%d]: %d\n", top, i, Time[i][0]);
			}
		}
		if (top >= 0) {
			Time[employee[top] - 1][0] = front->time;//çalýþanlarýn iþ dakikasi
			Time[employee[top] - 1][1] += front->time;//iþ dakikalarýn toplamý
		//	printf("Top: %d\tEmplyee: %d\tTime: %d\n", top, employee[top], Time[employee[top]-1][0]);
			printf("%d\t%d\t%d\t%d\t%d\n", front->id, employee[top], front->offset + pos, front->offset + front->time + pos, pos);
			x = front->offset;
			temp = front;
			front = front->next;
			free(temp);
		}
		if (top > -1) top--;
		if (top == -1)pos++;
	}printf("\n");
	for (int i = 5; i >= 0; i--)
		printf("%d\t%d\n", i + 1, Time[i][1]);

	//scanf("%d");
	return 0;
}