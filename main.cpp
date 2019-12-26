#include <iostream>
#include <string>
#include <clocale> //Türkçe karekter için
#include <malloc.h>
#include <conio.h> //_getch() (F1 VE F2 TUÞLARINI OKUYABÝLMEMÝZÝ SAGLAR)
#include <chrono> //nano saniye için.
#define MIN(A,B) (A<B?A:B)
using namespace std;

//zamaný tutacaðýmýz deðerler.
long long ns_time = 0;
chrono::time_point<chrono::steady_clock> start, finish;


/*
	Programý yazdým sonra türkçe kelimeler kullanabilmek için setlocale ile türkçe karekterleri tanýmladým
	ama kullanýcýdan veri alýrken türkçe karekterleri okuyamýyor. çk saçma bir hata ile karþýlaþtým. 
	setlocale 'i silinirse kullanýcýdan türkçe karekter okunabiliyor. ama cout ile direk "... " bu þekilde türkçe veri yazýlamýyor.
	
	Bu yuzden aþagýda türkçe alfabesini içeren ascii tablosunu oluþturmak zorunda kaldým. ama bana göre güzel ve mantýklý olan bu oldu.
*/

//Türkçe alfabesi. Karþýlaþtýrmalarý bu degerlere göre yapacaðýz.
//C1: Ç, G1: Ð, I1: Ý, O1: Ö, S1: Þ, U1: Ü, ERROR: beklenmedik durum.
enum ASCII {
	A,B,C,C1,D,E,F,G,G1,H,I,I1,J,K,L,M,N,O,O1,P,R,S,S1,T,U,U1,V,Y,Z,ERROR
};

//Alfabeyi oluþturduk ama buradaki degerleri char deðerleri ile eþleþmeli ki karþýlaþtýrma yapabilelim.
//Bunu yapmamýzýn sebebi Ascii koda göre sýralamak yada eþitligini kontrol etmek zor. malesef ascii kodlar ingiliz alfabesine göre'dir.
//Bu yüzden kendi ascii kodlarýmýzý oluþturmak zorundayýz.
//worst case O(n) iyileþtirilebilir.
enum ASCII ASCIIofCHAR(char ch) {
	if (ch == 'A' || ch == 'a') return A;
	else if (ch == 'B' || ch == 'b') return B;
	else if (ch == 'C' || ch == 'c') return C;
	else if (ch == 'Ç' || ch == 'ç') return C1;
	else if (ch == 'D' || ch == 'd') return D;
	else if (ch == 'E' || ch == 'e') return E;
	else if (ch == 'F' || ch == 'f') return F;
	else if (ch == 'G' || ch == 'g') return G;
	else if (ch == 'Ð' || ch == 'ð') return G1;
	else if (ch == 'H' || ch == 'h') return H;
	else if (ch == 'I' || ch == 'ý') return I;
	else if (ch == 'Ý' || ch == 'i') return I1;
	else if (ch == 'J' || ch == 'j') return J;
	else if (ch == 'K' || ch == 'k') return K;
	else if (ch == 'L' || ch == 'l') return L;
	else if (ch == 'M' || ch == 'm') return M;
	else if (ch == 'N' || ch == 'n') return N;
	else if (ch == 'O' || ch == 'o') return O;
	else if (ch == 'Ö' || ch == 'ö') return O1;
	else if (ch == 'P' || ch == 'p') return P;
	else if (ch == 'R' || ch == 'r') return R;
	else if (ch == 'S' || ch == 's') return S;
	else if (ch == 'Þ' || ch == 'þ') return S1;
	else if (ch == 'T' || ch == 't') return T;
	else if (ch == 'U' || ch == 'u') return U;
	else if (ch == 'Ü' || ch == 'ü') return U1;
	else if (ch == 'V' || ch == 'v') return V;
	else if (ch == 'Y' || ch == 'y') return Y;
	else if (ch == 'Z' || ch == 'z') return Z;
	return ERROR;
}

//Büyük harf küçük harf duyarsýzlýgýný saglamak...
//ve stringlerin eþitligini karþýlaþtýrmak için yazýldý.
bool operator==(string str1, string str2) {
	int str1Length = str1.length();
	int str2Length = str2.length();
	int size = MIN(str1Length, str2Length); //for da diziyi aþma durumu olabilir. bu yuzden en küçük olaný almalýyýz.

	/*
		Stringlerin eþitliði normalde yapýlabiliyor ama büyük / küçük harf duyarlýlýðý vardýr. yani A < a ' dýr.
		çünkü A ascii koda göre 67 iken a: ascii koda göre 97 dir.
		Ayný zamanda "veri." ile "veri" ayný kelimelerdir. bu yuzden uzunluklarý ayný olmak zorunda degildir.
		biz bu projede sadece tek noktalama iþareti olanlar için eþitlik yazacagýz.
		yani sonunda birden fazla noktalama iþareti gelirse bu yazdýgýmýz kod çalýþmayabilir.
		örneðin  "veri..." ile "veri"  ayný kelimedir. bizim program false döndürecektir.
	*/

	//karþýlaþtýrma yapabilmek için stringlerin uzunluklarý arasýnda max 1 fark olmalý. oda sondaki noktalama iþareti. 
	if ((str1Length - str2Length) < -1 || (str1Length - str2Length) > 1)
		return false;

	//Oluþturdugumuz ascii kodlara göre karþýlaþtýrma yapacagýz.
	enum ASCII str1_ascii, str2_ascii;
	
	for (int i = 0; i < size; i++) {
		//char'a karþýýk gelen degeri buluyoruz. farkettiysen enumdaki degerler 0 dan baþlayýp 29 a kadar devam ediyor.
		//yani ordaki deðerler artan sýrada int olarak tanýmlý.
		str1_ascii = ASCIIofCHAR(str1[i]);
		str2_ascii = ASCIIofCHAR(str2[i]);

		//Ascii tablosunu oluþturmadan önce yapmýþ oldugum eþitlik. Gerek kalmadý. tablomuz kendi hallediyor.
		//Ýki harf de büyük veya küçük | Büyük-küçük | küçük büyük kontrolü
		/*if (str1[i] != str2[i] && toupper(str1[i]) != str2[i] && tolower(str1[i]) != str2[i])
			return false;*/

		//degerler farklýysa false dön.
		if (str1_ascii != str2_ascii) return false;
		
		//burada i = size-1 demek for'un son döngüsü demektir. ve strignlerin uzunluklarý birbirinden farklýysa birbirine eþit olabilirler.
		//örneðin. "Veri.": 5 harf, "veri"4harf ama birbirine eþit. fakat,
		//"Veri" ile "verim" ayný kelimeler deðildir.
		//o yuzden uzun olanýn sondaki deger noktalama iþareti mi kontrol ediyoruz.
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
	Burada sort ederken lazým olacak '<' operatorunu overload'luyoruz. Fonskiyon ilede yapýlabilir. ama sürekli fonksiyon çagýrmak yorucu olmalý...
*/

bool operator<(string str1, string str2) {
	int str1Length = str1.length();
	int str2Length = str2.length();
	//fordaki i degeri en az string uzunluguna göre dönmesi gerekir.
	//bu durumda stringlerin uzunluklarý karþýlaþtýrýlýr.
	int size = MIN(str1Length, str2Length);
	enum ASCII Str1_Ascii, Str2_Ascii;
	for (int i = 0; i < size; i++) {

		//eþit eþit operatorü ile ayný fakat burada sadece büyük mü yoksa küçük mü kontrolu yapýyoruz.
		Str1_Ascii = ASCIIofCHAR(str1[i]);
		Str2_Ascii = ASCIIofCHAR(str2[i]);
		if (Str1_Ascii < Str2_Ascii)
			return true;
		else if (Str1_Ascii > Str2_Ascii)
			return false;
		
		//BUNLAR TÜRKÇE KAREKTER TABLOSU YAPMADAN ÖNCEKÝ YAZMIÞ OLDUGUM KODLAR
		/*if (str1[i] >= 97 && str2[i] < 97) {
			//küçük harfi büyük harfe çevirmek için 32 çýkarýlýr.
			if ((str1[i] - 32) < str2[i])
				return true;
			else if((str1[i] - 32) > str2[i])
				return false;
		}
		//Ýlk deger büyük ikinci deger küçük harf ise
		else if (str1[i] < 97 && str2[i] >= 97) {
			//büyük harfi küçük harf yapmak için 32 eklenir.
			if ((str1[i] + 32) < str2[i])
				return true;
			else if((str1[i] + 32 > str2[i]))
				return false;
		}
		//Her ikisi küçük veya büyük harf ise
		else {
			if (str1[i] < str2[i]) {
				return true;
			}
			else if (str1[i] > str2[i])
				return false;
		}
	}
	//Eger for içinde hiçbir deger döndürülmediyse bu stringler birbirine eþittir
	//veya stringlerden biri digerinin uzunlugundan uzundur.
	//örnegin: kalem-kalemlik, kalem alfabeye göre kalemlik den önce gelmeli.
	//yani ilk string ikinci stringten kýsa ise true döner.*/
	}if (str1Length < str2Length) return true;
	else return false;
	
}


/*
	Malesef yukarda da bahsettigim gibi. Türkçe karekterler input olarak alýrken sýkýntý yaratýyor.
	Çözüm olarak aþaðýdaki fonksiyonu yazdým. burada aldýgýmýz stringi atýyoruz.
	Eðer stringin içerisinde türkçe karekter var ise bunu setlocale'e uygun biçimde dönüþtürme yapýyoruz.
*/
string TrCharacter(string str) {
	for (int i = 0; i < str.length(); i++) {
		//küçük harfler
		if (str[i] == -97) str[i] = 254; //þ
		else if (str[i] == -108) str[i] = 246; //ö
		else if (str[i] == -127) str[i] = 252; //ü
		else if (str[i] == -115) str[i] = 253; //ý
		else if (str[i] == -121) str[i] = 231; //ç
		else if (str[i] == -89) str[i] = 240; //ð
		//Büyük harfler
		else if (str[i] == -98) str[i] = 222; //Þ
		else if (str[i] == -103) str[i] = 214; //Ö
		else if (str[i] == -102) str[i] = 220; //Ü
		else if (str[i] == -104) str[i] = 221; //Ý
		else if (str[i] == -128) str[i] = 199; //Ç
		else if (str[i] == -90) str[i] = 208; //Ð
	}
	return str;
}


//Listemizi tutan ana class 'ýmýz.
class Node {
public:
	string str;
	Node *next;
};

//Listemizin Rootunu global tanýmlýyoruz. Aslýnda gerek yok.
//çünkü fonksiyonlarý en genel yapýya göre tanýmladým. sadece root için degil. farklý farklý liste oluþturulabilsin diye.
Node* root = NULL;

//Listeye veri ekler.
void push(Node **head, string s) {
	Node *temp = *head;
	Node *nnode = new Node();
	nnode->str = s;
	nnode->next = NULL;
	//Head NULL ise head root olur ve fonskiyonu sonlandýrýr.
	if (*head == NULL) {
		*head = nnode;
		return;
	}
	//Root NULL degildir. o halde Son NULL degere kadar dön. ve notu oraya ekle.
	while (temp->next != NULL) temp = temp->next;
	temp->next = nnode;
}


//Listeden veri siler.
void pop(string str) {	
	auto start = chrono::high_resolution_clock::now();
	bool found = false;//silinmek istenen kelime listede var mý?
	
	//Liste boþ ise
	if (root == NULL) {
		cout << "Hata metin boþ\n";
		auto finish = chrono::high_resolution_clock::now();
		ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
		return;
	}
	//Silinmek istenen kelime root'ta mý?. o halde rootu bir sonraki listenin elemanýyla deðiþtir.
	if (root->str== str) {
		Node *temp = root;
		root = root->next;
		found = true;
		delete temp;
	}

	//(Linear search) kelime aranýr. bulundugu anda silinir. ve listenin sonuna kadar devam eder.
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
		cout << "Silmek istediðiniz Kelime Metinde Bulunmamaktadýr." << endl;
	
	auto finish = chrono::high_resolution_clock::now();
	ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
}

//Ekrana listeyi yazdýracagýmýz fonksiyon.
void display(Node *head) {
	while (head != NULL) {
		cout << head->str << " ";
		head = head->next;
	}
	cout << "\n";
}

/*
	Buraya kdar sadece proje için gerekli olan yapýlarý kurduk.
	Kýsa tekrar, Neler yaptýk:
	Türkçe karekter için ascii tablomuzu oluþturduk. Stringleri karþýlaþtýrmak(eþit eþit ve küçüktür operatorleri) için operator overload yaptýk.
	Kullanýcýdan türkçe karekter içeren kelime alabilmek için fonksiyon tanýmladýk.
	LinkedList oluþturduk. pop ve push fonksiyonlarýný tanýmladýk.
	artýk proje için ana fonksiyonlarý yazma zamaný geldi.
	
	A: Metin ekleme: Kullanýcýdan entera basana kadar veri al->bu veriyi stringlere böl->stringleri tek tek listeye push et.

	B: Kelime Sayýsý:
	Toplam: listenin uzunlugu toplam kelime sayýsýný verir.
	Farklý: bir döngü ile ayný veri olup olmadýgýný kontrol et. bir counter tanýmla ayný ise counter artýr daha sonra toplam veriden çýkar.

	C:Kelimeleri Listeleme:
	Alfabeye göre: 2. bir farklý olacak þekilde liste tanýmla bu listeyi Ana listemizden stringleri karþýlaþtýrarak push et.
	push edilen veri bizim alfabetik olarak sýralanmýþ verimiz olacaktýr.
	Adete göre: 2. listeden ayný sayýda kaç kelime geçtigine göre sýrala

	D:Search ile kelime aranýr. bulundugunda konumu ekrana bastýrýlýr. 

	E: Kelime Silme: kullanýcýdan string alýnýr ve pop ile kelime mevcut ise silinir.

	F: kullanýcýdan 2 string alýnýr. Degiþtirlmek istenen string listede mevcut ise yeni string degeri ile degiþtirilir.
	*/


// Listedeki degiþtirilmek istenilen Stringleri degiþtirmek için tanýmlandý.
void swapWord(Node *head, string changeStr, string newStr) {
	auto start = chrono::high_resolution_clock::now();
	Node *temp = head;
	bool found = false;
	//Listenin sonuna kadar döner. degiþtirilmek istenilen stringi yeni string ile degiþtirir.
	while (temp != NULL) {
		if (temp->str == changeStr) {
			temp->str = newStr;
			found = true;
		}
		temp = temp->next;
	}
	//string mevcut mu?
	if (!found)
		cout << "Deðiþtirmek istediðiniz Kelime Metinde Mevcut deðildir.\n";
	auto finish = chrono::high_resolution_clock::now();
	ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
}


//girilen metini boþluklara göre stringlere ayýrýr. ve listeye push eder.
void split(string str) {
	auto start = chrono::high_resolution_clock::now();
	char temp;
	int offset = 0;//stringi nereden bölecegimizi tutcagýmýz deðer.
	for (int i = 0; i <= str.length(); i++) {
		temp = str[i];
		
		//Kullanýcý en baþta(i==0) boþluk tusuna bastýgýnda || arka arkaya 2 kere boþluk tusuna bastýgýnda listeye push etme ama offset degeri arttýr.
		if (temp == ' ' && (i==0 || (i != 0 && str[i - 1] == ' ')))offset++;
		
		//Boþlugu gördügünde stringi ayýr. offset baþlangýc yeri. i-offset stringin sonu.
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

//Listenin uzunlugunu Döndürür.
int sizeofList(Node *head) {
	int counter = 0;
	while (head != NULL) {
		counter++;
		head = head->next;
	}
	return counter;
}

//Alfabetik olarak sýralanmýþ listeyi adetine göre tutacagýmýz yapýmýz.
struct SortListEleman {
	string str;
	int total;
};

typedef struct SortListEleman sortListEleman; //kolaylýk olsun diye :)

/*
	Yapýmýzý dizi olarak tanýmladým. aslýnda liste en mantýklýsý çünkü burada en fazla 250 kelimelik yer vardýr.
*/

#define MAX_WORD 250
sortListEleman sle_root[MAX_WORD];//Sort List Eleman Root ' u
int ListSize = 0; // dizimizin uzunlugu

/*
	Alfabetik olarak gelen listeyi adetlerine göre sortListEleman 'a atar.
	þu þekilde çalýþýyor. Listeden string deger alýnýr ve farklý bir string görene kadar counter ile arttýrma yaparýz.
	farklý deger gördügünde string degerini sortListElemana atar ve yeni string þle Listenin sonuna kadar döner.
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
		//Listnin sondaki elemaný alabilmek için .
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

//ayný kelime sayýsýný döndürür
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
	return (sizeofList(head)- counter);//toplam kelimeden ayný kelime sayýsýný çýkartýrarak kaç farklý kelime oldugu bulunur.
}

//Alfabetik olarak sýralayacagýýmýz 2. liste
Node *CopyNode = NULL;

//2. liste Ana listeyi aynen almalý. daha sonra alfabeye göre sýralama gerçekleþtircez.
//Listeyi kopyalar,
void copy(Node **start, Node *head) {
	while (head != NULL) {
		push(start, head->str);
		head = head->next;
	}
}


//alfabetik sýralama
void AlphabetSort(Node *head) {
	// sürekli listeyi sýralamak istediginde diziyi ve 2.listeyi sýfýrlýyoruz. tekrar sýralýyoruz.
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

//Alfabetik olarak sýralama. Zaten sortListEleman bizim için bunu tutuyor.
//Bizim tek yapmamýz gereken stringleri ve degerlerini almak.
void printAlphabetSort() {
	for (int i = 0; i < ListSize; i++)
		cout << sle_root[i].str << "->" << sle_root[i].total<<endl;
}

//Adete göre sýralama.
//SortListEleman daki total degerlerine göre max'dan en düþügüne dogru bir biçimde yazdýrýyoruz.
void printTotalSort() {
	int max = 0; // En fazla tekrar eden
	for (int i = 0; i < ListSize; i++) {
		if (max < sle_root[i].total) max = sle_root[i].total;
	}
	//max 0 olana kadar dön.
	while (1) {
		//her max degeri için baþtan Listeyi tarar. max'ý azaltýgýmýz için ayný degerleri yazdýrmasý olasý deðil.
		for (int i = 0; i < ListSize; i++) {
			if (sle_root[i].total == max)
				cout << sle_root[i].str << "->" << sle_root[i].total << endl;
		}
		max--;
		if (max == 0) break;
	}
}

//listedeki pozisyon olarak aldýðý diziyi ekrana konumunu yazdýrýr. Daha iyi yazýlabilir. þuanlýk kötü.
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

//Kelime arama gerçekleþir.
void search(Node *head, string str) {
	int position[100] = { 0 };
	auto start = chrono::high_resolution_clock::now();
	Node *temp = head;
	int i = 0, pos = 1;
	while (temp != NULL){

		//aranan kelime listedeki kelime ile ayný ise pozisyonu diziye at.
		if (temp->str == str) {
			position[i++] = pos;
		}
		pos++;
		temp = temp->next;
	}
	//i'nin tek arttýgý yer aranan stringi mevcut oldugunda o halde i = 0 ise aranan kelime mevcut degildir.
	if (i == 0)
		cout << "Aranan kelime Bulunamadý." << endl;
	//aranan kelime mevcut ise ekrana yazdýrmak için searchWordPrint fonksiyonu çagrýlýr.
	else
		SearchWordPrint(head, position);
	auto finish = chrono::high_resolution_clock::now();
	ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
}

//Kullanýcýdan text alýnýr.
string getText() {
	string str = "";
	//getline kullanýcý enter tusuna basana kadar string degeri alýr.
	getline(cin, str);
	str = TrCharacter(str);//Alýnan veriyi turkçe karekterleri kontrol eder.
	split(str);
	return str;
}

int main() {
	setlocale(LC_ALL, "Turkish");//Türkçe karekter için.
	string myText = "";//alacagýmýz text.
	int choice = 0;//seçim.
	while (choice != -1) {
		cout << "1. Metin Girme: \n";
		cout << "2. Metindeki Kelime Sayýsý: \n";
		cout << "3. Kelimeleri Listeleme: \n";
		cout << "4. Kelime Arama: \n";
		cout << "5. Kelime Silme: \n";
		cout << "6. Kelime Deðiþtirme: \n";
		cout << "-1. Çýkýþ: \n";
		cout << "\nLütfen iþleminizi seçiniz: ";
		
		//kullanýcýdan veri alýnýr eger kullanýcý sayý yerine Baþka bir deger girerse hata döndür.
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\n\nHatalý giriþ yaptýnýz.\n";
		}
		//beklenmedik giriþ yapýlmýþsa.
		else if (choice > 6|| choice < -1 || choice == 0)
			cout << "\n\nHatalý Giriþ\n";
		
		//dogru seçim.
		else {
			ns_time = 0; //ZAMAN.
			string str;
			int select = -1;//3. SECENEK ÝÇÝN.
			
			switch (choice){
			case 1:
				cout << "Metni Giriniz:\n";
				getchar();
				myText = getText();
				printf("\n");
				display(root);
				cout << "Ýþlem " << ns_time << "ns sürede tamamlanmýþtýr.\n\n";
				break;
			case 2:
				start = chrono::high_resolution_clock::now();
				cout << "Metindeki toplam kelime sayýsý: " << sizeofList(root) << endl;
				cout << "Metindeki tekil kelime(farklý) sayýsý: " << DifferentWordSize(root) << endl;
				finish = chrono::high_resolution_clock::now();
				ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
				cout << "Ýþlem " << ns_time << "ns sürede tamamlanmýþtýr.\n\n";
				break;
			case 3:
				cout << "Kelimeleri alfabetik listeleme için \"F1\" tuþuna,kaç adet geçtiðine göre listelemek için ise \"F2\" tuþuna basýn...\n";
				
				if (root == NULL) {
					cout << "Hata! Metin boþ.\n";
					break;
				}
				_getch();//_GETCH() Bir giriþ alýr(0) ve bir sonraki giriþte girilen degeri döndürür(F1: 59, F2: 60). Farklý bir çalýþma mekanizmasý var.
				select = _getch();//sonraki giriþ. yani asýl giriþ.
				if (select == 59) {
					start = chrono::high_resolution_clock::now();
					AlphabetSort(root);
					printAlphabetSort();
					finish = chrono::high_resolution_clock::now();
					ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
					cout << "Ýþlem " << ns_time << "ns sürede tamamlanmýþtýr.\n\n";
				}
				else if (select == 60) {
					start = chrono::high_resolution_clock::now();
					AlphabetSort(root);
					printTotalSort();
					finish = chrono::high_resolution_clock::now();
					ns_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count();
					cout << "Ýþlem " << ns_time << "ns sürede tamamlanmýþtýr.\n\n";
				}
				else
					cout << "Hatalý iþlem girdiniz.\n";
				break;
			case 4:
				cout << "Aranan kelimeyi giriniz: ";
				cin >> str;
				str = TrCharacter(str);
				search(root, str);
				cout << "Ýþlem " << ns_time << "ns sürede tamamlanmýþtýr.\n\n";
				break;
			case 5:
				cout << "Silmek istediðiniz kelimeyi giriniz: ";
				cin >> str;
				str = TrCharacter(str);
				pop(str);
				cout << "Silme iþlemi sonrasýnda metnin son durumu:\n\n";
				display(root);
				cout << "Ýþlem " << ns_time << "ns sürede tamamlanmýþtýr.\n\n";
				break;
			case 6:
				cout << "Deðiþtirmek istediðiniz kelimeyi giriniz: ";
				cin >> str;
				str = TrCharacter(str);
				string str1;
				cout << "Yeni Kelimeyi giriniz: ";
				cin >> str1;
				str = TrCharacter(str);
				swapWord(root, str, str1);
				display(root);
				cout << "Ýþlem " << ns_time << "ns sürede tamamlanmýþtýr.\n\n";
				break;
			}
		}
	}
	
	getchar();
	return 0;
}