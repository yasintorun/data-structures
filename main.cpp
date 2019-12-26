#include <iostream>
#include <string>
#include <clocale> //T�rk�e karekter i�in
#include <malloc.h>
#include <conio.h> //_getch() (F1 VE F2 TU�LARINI OKUYAB�LMEM�Z� SAGLAR)
#include <chrono> //nano saniye i�in.
#define MIN(A,B) (A<B?A:B)
using namespace std;

//zaman� tutaca��m�z de�erler.
long long ns_time = 0;
chrono::time_point<chrono::steady_clock> start, finish;


/*
	Program� yazd�m sonra t�rk�e kelimeler kullanabilmek i�in setlocale ile t�rk�e karekterleri tan�mlad�m
	ama kullan�c�dan veri al�rken t�rk�e karekterleri okuyam�yor. �k sa�ma bir hata ile kar��la�t�m. 
	setlocale 'i silinirse kullan�c�dan t�rk�e karekter okunabiliyor. ama cout ile direk "... " bu �ekilde t�rk�e veri yaz�lam�yor.
	
	Bu yuzden a�ag�da t�rk�e alfabesini i�eren ascii tablosunu olu�turmak zorunda kald�m. ama bana g�re g�zel ve mant�kl� olan bu oldu.
*/

//T�rk�e alfabesi. Kar��la�t�rmalar� bu degerlere g�re yapaca��z.
//C1: �, G1: �, I1: �, O1: �, S1: �, U1: �, ERROR: beklenmedik durum.
enum ASCII {
	A,B,C,C1,D,E,F,G,G1,H,I,I1,J,K,L,M,N,O,O1,P,R,S,S1,T,U,U1,V,Y,Z,ERROR
};

//Alfabeyi olu�turduk ama buradaki degerleri char de�erleri ile e�le�meli ki kar��la�t�rma yapabilelim.
//Bunu yapmam�z�n sebebi Ascii koda g�re s�ralamak yada e�itligini kontrol etmek zor. malesef ascii kodlar ingiliz alfabesine g�re'dir.
//Bu y�zden kendi ascii kodlar�m�z� olu�turmak zorunday�z.
//worst case O(n) iyile�tirilebilir.
enum ASCII ASCIIofCHAR(char ch) {
	if (ch == 'A' || ch == 'a') return A;
	else if (ch == 'B' || ch == 'b') return B;
	else if (ch == 'C' || ch == 'c') return C;
	else if (ch == '�' || ch == '�') return C1;
	else if (ch == 'D' || ch == 'd') return D;
	else if (ch == 'E' || ch == 'e') return E;
	else if (ch == 'F' || ch == 'f') return F;
	else if (ch == 'G' || ch == 'g') return G;
	else if (ch == '�' || ch == '�') return G1;
	else if (ch == 'H' || ch == 'h') return H;
	else if (ch == 'I' || ch == '�') return I;
	else if (ch == '�' || ch == 'i') return I1;
	else if (ch == 'J' || ch == 'j') return J;
	else if (ch == 'K' || ch == 'k') return K;
	else if (ch == 'L' || ch == 'l') return L;
	else if (ch == 'M' || ch == 'm') return M;
	else if (ch == 'N' || ch == 'n') return N;
	else if (ch == 'O' || ch == 'o') return O;
	else if (ch == '�' || ch == '�') return O1;
	else if (ch == 'P' || ch == 'p') return P;
	else if (ch == 'R' || ch == 'r') return R;
	else if (ch == 'S' || ch == 's') return S;
	else if (ch == '�' || ch == '�') return S1;
	else if (ch == 'T' || ch == 't') return T;
	else if (ch == 'U' || ch == 'u') return U;
	else if (ch == '�' || ch == '�') return U1;
	else if (ch == 'V' || ch == 'v') return V;
	else if (ch == 'Y' || ch == 'y') return Y;
	else if (ch == 'Z' || ch == 'z') return Z;
	return ERROR;
}

//B�y�k harf k���k harf duyars�zl�g�n� saglamak...
//ve stringlerin e�itligini kar��la�t�rmak i�in yaz�ld�.
bool operator==(string str1, string str2) {
	int str1Length = str1.length();
	int str2Length = str2.length();
	int size = MIN(str1Length, str2Length); //for da diziyi a�ma durumu olabilir. bu yuzden en k���k olan� almal�y�z.

	/*
		Stringlerin e�itli�i normalde yap�labiliyor ama b�y�k / k���k harf duyarl�l��� vard�r. yani A < a ' d�r.
		��nk� A ascii koda g�re 67 iken a: ascii koda g�re 97 dir.
		Ayn� zamanda "veri." ile "veri" ayn� kelimelerdir. bu yuzden uzunluklar� ayn� olmak zorunda degildir.
		biz bu projede sadece tek noktalama i�areti olanlar i�in e�itlik yazacag�z.
		yani sonunda birden fazla noktalama i�areti gelirse bu yazd�g�m�z kod �al��mayabilir.
		�rne�in  "veri..." ile "veri"  ayn� kelimedir. bizim program false d�nd�recektir.
	*/

	//kar��la�t�rma yapabilmek i�in stringlerin uzunluklar� aras�nda max 1 fark olmal�. oda sondaki noktalama i�areti. 
	if ((str1Length - str2Length) < -1 || (str1Length - str2Length) > 1)
		return false;

	//Olu�turdugumuz ascii kodlara g�re kar��la�t�rma yapacag�z.
	enum ASCII str1_ascii, str2_ascii;
	
	for (int i = 0; i < size; i++) {
		//char'a kar���k gelen degeri buluyoruz. farkettiysen enumdaki degerler 0 dan ba�lay�p 29 a kadar devam ediyor.
		//yani ordaki de�erler artan s�rada int olarak tan�ml�.
		str1_ascii = ASCIIofCHAR(str1[i]);
		str2_ascii = ASCIIofCHAR(str2[i]);

		//Ascii tablosunu olu�turmadan �nce yapm�� oldugum e�itlik. Gerek kalmad�. tablomuz kendi hallediyor.
		//�ki harf de b�y�k veya k���k | B�y�k-k���k | k���k b�y�k kontrol�
		/*if (str1[i] != str2[i] && toupper(str1[i]) != str2[i] && tolower(str1[i]) != str2[i])
			return false;*/

		//degerler farkl�ysa false d�n.
		if (str1_ascii != str2_ascii) return false;
		
		//burada i = size-1 demek for'un son d�ng�s� demektir. ve strignlerin uzunluklar� birbirinden farkl�ysa birbirine e�it olabilirler.
		//�rne�in. "Veri.": 5 harf, "veri"4harf ama birbirine e�it. fakat,
		//"Veri" ile "verim" ayn� kelimeler de�ildir.
		//o yuzden uzun olan�n sondaki deger noktalama i�areti mi kontrol ediyoruz.
		if (str1Length != str2Length && i == size - 1) {
			if (size == str1Length && str2[i + 1] >= 33 && str2[i + 1] <= 64)
				return true;
			else if (size == str2Length && str1[i + 1] >= 33 && str1[i + 1] <= 64)
				return true;
			else 
				return false;
		}
	}
}



/*
	Burada sort ederken laz�m olacak '<' operatorunu overload'luyoruz. Fonskiyon ilede yap�labilir. ama s�rekli fonksiyon �ag�rmak yorucu olmal�...
*/

bool operator<(string str1, string str2) {
	int str1Length = str1.length();
	int str2Length = str2.length();
	//fordaki i degeri en az string uzunluguna g�re d�nmesi gerekir.
	//bu durumda stringlerin uzunluklar� kar��la�t�r�l�r.
	int size = MIN(str1Length, str2Length);
	enum ASCII Str1_Ascii, Str2_Ascii;
	for (int i = 0; i < size; i++) {

		//e�it e�it operator� ile ayn� fakat burada sadece b�y�k m� yoksa k���k m� kontrolu yap�yoruz.
		Str1_Ascii = ASCIIofCHAR(str1[i]);
		Str2_Ascii = ASCIIofCHAR(str2[i]);
		if (Str1_Ascii < Str2_Ascii)
			return true;
		else if (Str1_Ascii > Str2_Ascii)
			return false;
		
		//BUNLAR T�RK�E KAREKTER TABLOSU YAPMADAN �NCEK� YAZMI� OLDUGUM KODLAR
		/*if (str1[i] >= 97 && str2[i] < 97) {
			//k���k harfi b�y�k harfe �evirmek i�in 32 ��kar�l�r.
			if ((str1[i] - 32) < str2[i])
				return true;
			else if((str1[i] - 32) > str2[i])
				return false;
		}
		//�lk deger b�y�k ikinci deger k���k harf ise
		else if (str1[i] < 97 && str2[i] >= 97) {
			//b�y�k harfi k���k harf yapmak i�in 32 eklenir.
			if ((str1[i] + 32) < str2[i])
				return true;
			else if((str1[i] + 32 > str2[i]))
				return false;
		}
		//Her ikisi k���k veya b�y�k harf ise
		else {
			if (str1[i] < str2[i]) {
				return true;
			}
			else if (str1[i] > str2[i])
				return false;
		}
	}
	//Eger for i�inde hi�bir deger d�nd�r�lmediyse bu stringler birbirine e�ittir
	//veya stringlerden biri digerinin uzunlugundan uzundur.
	//�rnegin: kalem-kalemlik, kalem alfabeye g�re kalemlik den �nce gelmeli.
	//yani ilk string ikinci stringten k�sa ise true d�ner.*/
	}if (str1Length < str2Length) return true;
	else return false;
	
}


/*
	Malesef yukarda da bahsettigim gibi. T�rk�e karekterler input olarak al�rken s�k�nt� yarat�yor.
	��z�m olarak a�a��daki fonksiyonu yazd�m. burada ald�g�m�z stringi at�yoruz.
	E�er stringin i�erisinde t�rk�e karekter var ise bunu setlocale'e uygun bi�imde d�n��t�rme yap�yoruz.
*/
string TrCharacter(string str) {
	for (int i = 0; i < str.length(); i++) {
		//k���k harfler
		if (str[i] == -97) str[i] = 254; //�
		else if (str[i] == -108) str[i] = 246; //�
		else if (str[i] == -127) str[i] = 252; //�
		else if (str[i] == -115) str[i] = 253; //�
		else if (str[i] == -121) str[i] = 231; //�
		else if (str[i] == -89) str[i] = 240; //�
		//B�y�k harfler
		else if (str[i] == -98) str[i] = 222; //�
		else if (str[i] == -103) str[i] = 214; //�
		else if (str[i] == -102) str[i] = 220; //�
		else if (str[i] == -104) str[i] = 221; //�
		else if (str[i] == -128) str[i] = 199; //�
		else if (str[i] == -90) str[i] = 208; //�
	}
	return str;
}


//Listemizi tutan ana class '�m�z.
class Node {
public:
	string str;
	Node *next;
};

//Listemizin Rootunu global tan�ml�yoruz. Asl�nda gerek yok.
//��nk� fonksiyonlar� en genel yap�ya g�re tan�mlad�m. sadece root i�in degil. farkl� farkl� liste olu�turulabilsin diye.
Node* root = NULL;

//Listeye veri ekler.
void push(Node **head, string s) {
	Node *temp = *head;
	Node *nnode = new Node();
	nnode->str = s;
	nnode->next = NULL;
	//Head NULL ise head root olur ve fonskiyonu sonland�r�r.
	if (*head == NULL) {
		*head = nnode;
		return;
	}
	//Root NULL degildir. o halde Son NULL degere kadar d�n. ve notu oraya ekle.
	while (temp->next != NULL) temp = temp->next;
	temp->next = nnode;
}


//Listeden veri siler.
void pop(string str) {	
	auto start = chrono::high_resolution_clock::now();
	bool found = false;//silinmek istenen kelime listede var m�?
	
	//Liste bo� ise
	if (root == NULL) {
		cout << "Hata metin bo�\n";
		auto finish = chrono::high_resolution_clock::now();
		ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
		return;
	}
	//Silinmek istenen kelime root'ta m�?. o halde rootu bir sonraki listenin eleman�yla de�i�tir.
	if (root->str== str) {
		Node *temp = root;
		root = root->next;
		found = true;
		delete temp;
	}

	//(Linear search) kelime aran�r. bulundugu anda silinir. ve listenin sonuna kadar devam eder.
	Node *temp1 = root;
	Node *temp2 = NULL;
	while (temp1 != NULL) {
		if (temp1->str == str) {
			temp2->next = temp1->next;
			temp2 = temp1;
			temp1 = temp1->next;
			delete(temp2);
			found = true;
		}
		if (temp1 != NULL) {
			temp2 = temp1;
			temp1 = temp1->next;
		}
	}
	//Kelime mevcut degilse
	if (!found)
		cout << "Silmek istedi�iniz Kelime Metinde Bulunmamaktad�r." << endl;
	
	auto finish = chrono::high_resolution_clock::now();
	ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
}

//Ekrana listeyi yazd�racag�m�z fonksiyon.
void display(Node *head) {
	while (head != NULL) {
		cout << head->str << " ";
		head = head->next;
	}
	cout << "\n";
}

/*
	Buraya kdar sadece proje i�in gerekli olan yap�lar� kurduk.
	K�sa tekrar, Neler yapt�k:
	T�rk�e karekter i�in ascii tablomuzu olu�turduk. Stringleri kar��la�t�rmak(e�it e�it ve k���kt�r operatorleri) i�in operator overload yapt�k.
	Kullan�c�dan t�rk�e karekter i�eren kelime alabilmek i�in fonksiyon tan�mlad�k.
	LinkedList olu�turduk. pop ve push fonksiyonlar�n� tan�mlad�k.
	art�k proje i�in ana fonksiyonlar� yazma zaman� geldi.
	
	A: Metin ekleme: Kullan�c�dan entera basana kadar veri al->bu veriyi stringlere b�l->stringleri tek tek listeye push et.

	B: Kelime Say�s�:
	Toplam: listenin uzunlugu toplam kelime say�s�n� verir.
	Farkl�: bir d�ng� ile ayn� veri olup olmad�g�n� kontrol et. bir counter tan�mla ayn� ise counter art�r daha sonra toplam veriden ��kar.

	C:Kelimeleri Listeleme:
	Alfabeye g�re: 2. bir farkl� olacak �ekilde liste tan�mla bu listeyi Ana listemizden stringleri kar��la�t�rarak push et.
	push edilen veri bizim alfabetik olarak s�ralanm�� verimiz olacakt�r.
	Adete g�re: 2. listeden ayn� say�da ka� kelime ge�tigine g�re s�rala

	D:Search ile kelime aran�r. bulundugunda konumu ekrana bast�r�l�r. 

	E: Kelime Silme: kullan�c�dan string al�n�r ve pop ile kelime mevcut ise silinir.

	F: kullan�c�dan 2 string al�n�r. Degi�tirlmek istenen string listede mevcut ise yeni string degeri ile degi�tirilir.
	*/


// Listedeki degi�tirilmek istenilen Stringleri degi�tirmek i�in tan�mland�.
void swapWord(Node *head, string changeStr, string newStr) {
	auto start = chrono::high_resolution_clock::now();
	Node *temp = head;
	bool found = false;
	//Listenin sonuna kadar d�ner. degi�tirilmek istenilen stringi yeni string ile degi�tirir.
	while (temp != NULL) {
		if (temp->str == changeStr) {
			temp->str = newStr;
			found = true;
		}
		temp = temp->next;
	}
	//string mevcut mu?
	if (!found)
		cout << "De�i�tirmek istedi�iniz Kelime Metinde Mevcut de�ildir.\n";
	auto finish = chrono::high_resolution_clock::now();
	ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
}


//girilen metini bo�luklara g�re stringlere ay�r�r. ve listeye push eder.
void split(string str) {
	auto start = chrono::high_resolution_clock::now();
	char temp;
	int offset = 0;//stringi nereden b�lecegimizi tutcag�m�z de�er.
	for (int i = 0; i <= str.length(); i++) {
		temp = str[i];
		
		//Kullan�c� en ba�ta(i==0) bo�luk tusuna bast�g�nda || arka arkaya 2 kere bo�luk tusuna bast�g�nda listeye push etme ama offset degeri artt�r.
		if (temp == ' ' && (i==0 || (i != 0 && str[i - 1] == ' ')))offset++;
		
		//Bo�lugu g�rd�g�nde stringi ay�r. offset ba�lang�c yeri. i-offset stringin sonu.
		else if (temp == ' ' || str[i] == '\0'){
			string s(str, offset, i - offset);
			offset = i+1;
			//stringi listeye push et.
			push(&root, s);
		}
	}
	auto finish = chrono::high_resolution_clock::now();
	ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
}

//Listenin uzunlugunu D�nd�r�r.
int sizeofList(Node *head) {
	int counter = 0;
	while (head != NULL) {
		counter++;
		head = head->next;
	}
	return counter;
}

//Alfabetik olarak s�ralanm�� listeyi adetine g�re tutacag�m�z yap�m�z.
struct SortListEleman {
	string str;
	int total;
};

typedef struct SortListEleman sortListEleman; //kolayl�k olsun diye :)

/*
	Yap�m�z� dizi olarak tan�mlad�m. asl�nda liste en mant�kl�s� ��nk� burada en fazla 250 kelimelik yer vard�r.
*/

#define MAX_WORD 250
sortListEleman sle_root[MAX_WORD];//Sort List Eleman Root ' u
int ListSize = 0; // dizimizin uzunlugu

/*
	Alfabetik olarak gelen listeyi adetlerine g�re sortListEleman 'a atar.
	�u �ekilde �al���yor. Listeden string deger al�n�r ve farkl� bir string g�rene kadar counter ile artt�rma yapar�z.
	farkl� deger g�rd�g�nde string degerini sortListElemana atar ve yeni string �le Listenin sonuna kadar d�ner.
*/
void sort(Node *head) {
	Node *temp1 = head, *temp2;
	int counter = 0;
	string str = head->str;
	while (temp1 != NULL) {
		if (str == temp1->str) {
			counter++;
		}
		else {
			//cout << str << ": " << counter<<endl;
			sle_root[ListSize].str = str;
			sle_root[ListSize].total = counter;
			str = temp1->str;
			counter = 1;
			ListSize++;
		}
		//Listnin sondaki eleman� alabilmek i�in .
		if (temp1->next == NULL) {
			//cout << str << ": " << counter << endl;
			sle_root[ListSize].str = str;
			sle_root[ListSize].total = counter;
			str = temp1->str;
			ListSize++;
		}
		temp1 = temp1->next;
	}
}

//ayn� kelime say�s�n� d�nd�r�r
int DifferentWordSize(Node *head) {
	int counter = 0;
	Node *temp1 = head, *temp2 = head;
	while (temp1 != NULL) {
		temp2 = temp1->next;
		while (temp2 != NULL) {
			if (temp1->str == temp2->str) {
				counter++;
				break;
			}
			temp2 = temp2->next;
		}
		temp1 = temp1->next;
	}
	return (sizeofList(head)- counter);//toplam kelimeden ayn� kelime say�s�n� ��kart�rarak ka� farkl� kelime oldugu bulunur.
}

//Alfabetik olarak s�ralayacag��m�z 2. liste
Node *CopyNode = NULL;

//2. liste Ana listeyi aynen almal�. daha sonra alfabeye g�re s�ralama ger�ekle�tircez.
//Listeyi kopyalar,
void copy(Node **start, Node *head) {
	while (head != NULL) {
		push(start, head->str);
		head = head->next;
	}
}


//alfabetik s�ralama
void AlphabetSort(Node *head) {
	// s�rekli listeyi s�ralamak istediginde diziyi ve 2.listeyi s�f�rl�yoruz. tekrar s�ral�yoruz.
	ListSize = 0; 
	CopyNode = NULL;
	copy(&CopyNode, head);
	Node *temp1 = CopyNode, *temp2 = CopyNode;
	while (temp1 != NULL) {
		temp2 = temp1->next;
		while (temp2 != NULL) {
			if (temp2->str < temp1->str) {
				string tempStr = temp2->str;
				temp2->str = temp1->str;
				temp1->str = tempStr;
			}
			temp2 = temp2->next;
		}
		temp1 = temp1->next;
	}
	sort(CopyNode);
}

//Alfabetik olarak s�ralama. Zaten sortListEleman bizim i�in bunu tutuyor.
//Bizim tek yapmam�z gereken stringleri ve degerlerini almak.
void printAlphabetSort() {
	for (int i = 0; i < ListSize; i++)
		cout << sle_root[i].str << "->" << sle_root[i].total<<endl;
}

//Adete g�re s�ralama.
//SortListEleman daki total degerlerine g�re max'dan en d���g�ne dogru bir bi�imde yazd�r�yoruz.
void printTotalSort() {
	int max = 0; // En fazla tekrar eden
	for (int i = 0; i < ListSize; i++) {
		if (max < sle_root[i].total) max = sle_root[i].total;
	}
	//max 0 olana kadar d�n.
	while (1) {
		//her max degeri i�in ba�tan Listeyi tarar. max'� azalt�g�m�z i�in ayn� degerleri yazd�rmas� olas� de�il.
		for (int i = 0; i < ListSize; i++) {
			if (sle_root[i].total == max)
				cout << sle_root[i].str << "->" << sle_root[i].total << endl;
		}
		max--;
		if (max == 0) break;
	}
}

//listedeki pozisyon olarak ald��� diziyi ekrana konumunu yazd�r�r. Daha iyi yaz�labilir. �uanl�k k�t�.
void SearchWordPrint(Node *head, int position[]) {
	printf("Bulunan konumlar: ");
	for (int i = 0; i < 100; i++) {
		if (position[i + 1] == 0) {
			printf("%d", position[i]);
			break;
		}
		printf("%d-", position[i]);
	}
	printf("\n");
}

//Kelime arama ger�ekle�ir.
void search(Node *head, string str) {
	int position[100] = { 0 };
	auto start = chrono::high_resolution_clock::now();
	Node *temp = head;
	int i = 0, pos = 1;
	while (temp != NULL){

		//aranan kelime listedeki kelime ile ayn� ise pozisyonu diziye at.
		if (temp->str == str) {
			position[i++] = pos;
		}
		pos++;
		temp = temp->next;
	}
	//i'nin tek artt�g� yer aranan stringi mevcut oldugunda o halde i = 0 ise aranan kelime mevcut degildir.
	if (i == 0)
		cout << "Aranan kelime Bulunamad�." << endl;
	//aranan kelime mevcut ise ekrana yazd�rmak i�in searchWordPrint fonksiyonu �agr�l�r.
	else
		SearchWordPrint(head, position);
	auto finish = chrono::high_resolution_clock::now();
	ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
}

//Kullan�c�dan text al�n�r.
string getText() {
	string str = "";
	//getline kullan�c� enter tusuna basana kadar string degeri al�r.
	getline(cin, str);
	str = TrCharacter(str);//Al�nan veriyi turk�e karekterleri kontrol eder.
	split(str);
	return str;
}

int main() {
	setlocale(LC_ALL, "Turkish");//T�rk�e karekter i�in.
	string myText = "";//alacag�m�z text.
	int choice = 0;//se�im.
	while (choice != -1) {
		cout << "1. Metin Girme: \n";
		cout << "2. Metindeki Kelime Say�s�: \n";
		cout << "3. Kelimeleri Listeleme: \n";
		cout << "4. Kelime Arama: \n";
		cout << "5. Kelime Silme: \n";
		cout << "6. Kelime De�i�tirme: \n";
		cout << "-1. ��k��: \n";
		cout << "\nL�tfen i�leminizi se�iniz: ";
		
		//kullan�c�dan veri al�n�r eger kullan�c� say� yerine Ba�ka bir deger girerse hata d�nd�r.
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\n\nHatal� giri� yapt�n�z.\n";
		}
		//beklenmedik giri� yap�lm��sa.
		else if (choice > 6|| choice < -1 || choice == 0)
			cout << "\n\nHatal� Giri�\n";
		
		//dogru se�im.
		else {
			ns_time = 0; //ZAMAN.
			string str;
			int select = -1;//3. SECENEK ���N.
			
			switch (choice){
			case 1:
				cout << "Metni Giriniz:\n";
				getchar();
				myText = getText();
				printf("\n");
				display(root);
				cout << "��lem " << ns_time << "ns s�rede tamamlanm��t�r.\n\n";
				break;
			case 2:
				start = chrono::high_resolution_clock::now();
				cout << "Metindeki toplam kelime say�s�: " << sizeofList(root) << endl;
				cout << "Metindeki tekil kelime(farkl�) say�s�: " << DifferentWordSize(root) << endl;
				finish = chrono::high_resolution_clock::now();
				ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
				cout << "��lem " << ns_time << "ns s�rede tamamlanm��t�r.\n\n";
				break;
			case 3:
				cout << "Kelimeleri alfabetik listeleme i�in \"F1\" tu�una,ka� adet ge�ti�ine g�re listelemek i�in ise \"F2\" tu�una bas�n...\n";
				
				if (root == NULL) {
					cout << "Hata! Metin bo�.\n";
					break;
				}
				_getch();//_GETCH() Bir giri� al�r(0) ve bir sonraki giri�te girilen degeri d�nd�r�r(F1: 59, F2: 60). Farkl� bir �al��ma mekanizmas� var.
				select = _getch();//sonraki giri�. yani as�l giri�.
				if (select == 59) {
					start = chrono::high_resolution_clock::now();
					AlphabetSort(root);
					printAlphabetSort();
					finish = chrono::high_resolution_clock::now();
					ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
					cout << "��lem " << ns_time << "ns s�rede tamamlanm��t�r.\n\n";
				}
				else if (select == 60) {
					start = chrono::high_resolution_clock::now();
					AlphabetSort(root);
					printTotalSort();
					finish = chrono::high_resolution_clock::now();
					ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
					cout << "��lem " << ns_time << "ns s�rede tamamlanm��t�r.\n\n";
				}
				else
					cout << "Hatal� i�lem girdiniz.\n";
				break;
			case 4:
				cout << "Aranan kelimeyi giriniz: ";
				cin >> str;
				str = TrCharacter(str);
				search(root, str);
				cout << "��lem " << ns_time << "ns s�rede tamamlanm��t�r.\n\n";
				break;
			case 5:
				cout << "Silmek istedi�iniz kelimeyi giriniz: ";
				cin >> str;
				str = TrCharacter(str);
				pop(str);
				cout << "Silme i�lemi sonras�nda metnin son durumu:\n\n";
				display(root);
				cout << "��lem " << ns_time << "ns s�rede tamamlanm��t�r.\n\n";
				break;
			case 6:
				cout << "De�i�tirmek istedi�iniz kelimeyi giriniz: ";
				cin >> str;
				str = TrCharacter(str);
				string str1;
				cout << "Yeni Kelimeyi giriniz: ";
				cin >> str1;
				str = TrCharacter(str);
				swapWord(root, str, str1);
				display(root);
				cout << "��lem " << ns_time << "ns s�rede tamamlanm��t�r.\n\n";
				break;
			}
		}
	}
	
	getchar();
	return 0;
}