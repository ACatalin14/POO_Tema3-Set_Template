#include <iostream>
#include <stdarg.h>

using namespace std;

#ifndef SET_H
#define SET_H

template <class T> class Set
{
	int card;      	// cardinalul multimii
	T *mult;		// multimea propriu-zisa
	int maxCard;    // cardinalul maxim pe care l-a atins multimea vreodata in durata ei de viata
					// SAU numarul de spatii de memorie alocate pt multime

	int PozitieInserare(T elem, int first, int last);
	// returneaza pozitia unde ar trebui inserat elem in multime [in timp O(log card)]
	// pentru a pastra elementele in ordine crescatoare in multime;
	// este garantat ca elem nu apartine multimii

	int CautareBinara(T elem, int first, int last);
	// returneaza -1 SAU pozitii indexate de la 0; dupa cum elem apartine sau nu multimii [in timp O(log card)]

public:
	Set();
	Set(int n);					// aloca n spatii de memorie goale
	Set(int n, T x0, ...);	// ex: Set o(4, 100, 75, 56, 100) va aloca 3 spatii de memorie ce contin: 56 75 100
	Set(Set &A);
	~Set();

	int Cauta(T elem);		// returneaza pozitia lui elem (indexata de la 0) sau -1 daca x nu este in multime
	void Insereaza(T elem);
	void Sterge(T elem);
	T Suma();
	Set<T> Par();			// returneaza un set care contine numai elementele pare
	Set<T> Impar();			// returneaza un set care contine numai elementele impare
	int Cardinal();

	Set<T>& operator= (const Set<T> &A);
	T& operator[] (int i);

	template<class T1, class T2>
	friend bool operator< (const Set<T1> &A, const Set<T2> &B);
	template<class T1, class T2>
	friend bool operator> (const Set<T1> &A, const Set<T2> &B);
	template<class T>
	friend ostream& operator<< (ostream &stream, const Set<T> &A);
	template<class T>
	friend istream& operator>> (istream &stream, Set<T> &A);
	template<class T>
	friend Set<T>& operator+ (const Set<T> &A, const Set<T> &B);
	template<class T>
	friend Set<T>& operator* (T k, const Set<T> &A);
	template<class T>
	friend Set<T>& operator* (const Set<T> &A, T k);
};

template<class T>
int Set<T>::PozitieInserare(T elem, int first, int last) {
	if (first > last) {
		return first;
	}

	if (first == last) {
		if (elem < mult[first]) {
			return first;
		}
		else {
			return first + 1;
		}
	}

	int med = (first + last) / 2;

	if (elem < mult[med]) {
		return PozitieInserare(elem, first, med - 1);
	}

	return PozitieInserare(elem, med + 1, last);		// aici elem > mult[med]
}

template<class T>
int Set<T>::CautareBinara(T elem, int first, int last) {
	if (first > last) {
		return -1;
	}

	if (first == last) {
		if (elem == mult[first]) {
			return first;
		}
		else {
			return -1;
		}
	}

	int med = (first + last) / 2;

	if (elem == mult[med]) {
		return med;
	}

	if (elem < mult[med]) {
		return CautareBinara(elem, first, med - 1);
	}

	return CautareBinara(elem, med + 1, last);			// aici elem > mult[med]
}

template<class T>
Set<T>::Set() {
	card = 0;
	maxCard = 0;
	mult = 0;
}

template<class T>
Set<T>::Set(int n) {
	card = 0;
	maxCard = n;
	mult = new T[n];
}

template<class T>
Set<T>::Set(int n, T x0, ...) {	// n-am citit nicaieri ca ar fi interzisa folosirea notiunilor din C...
	va_list lp;	// lista de parametrii
	int i;
	T x;

	card = 0;
	maxCard = 0;
	mult = 0;

	va_start(lp, x0);
	Insereaza(x0);
	for (i = 1; i < n; i++) {	// luam fiecare parametru din lista si il inseram in set
		x = va_arg(lp, T);
		Insereaza(x);
	}
	va_end(lp);
}

template<class T>
Set<T>::Set(Set<T> &A) {
	card = A.card;
	maxCard = A.maxCard;
	mult = new T[maxCard];
	for (int i = 0; i < card; i++) {
		mult[i] = A.mult[i];
	}
}

template<class T>
Set<T>::~Set() {
	delete[] mult;
}

template<class T>
int Set<T>::Cauta(T elem) {
	return CautareBinara(elem, 0, card - 1);
}

template<class T>
void Set<T>::Insereaza(T elem)
{
	// fiecare element trebuie sa apara o singura data
	// mai intai verificam daca elem apare deja in multime
	// folosind o cautare binara (stim ca multimea e sortata crescator)

	if (Cauta(elem) == -1) {
		int pos = PozitieInserare(elem, 0, card - 1);	// pozitia pe care se insereaza elem
		if (card == maxCard) {	// daca am umplut memoria, mai alocam memorie
			maxCard++;
			T *mult1;			// noua multime pe care o construim
			mult1 = new T[card + 1];

			for (int i = 0; i < pos; i++) {	// constructia lui mult1 pana la pozitia pos
				mult1[i] = mult[i];
			}
			mult1[pos] = elem;	// inseram elem
			for (int i = pos + 1; i < card + 1; i++) {	// constructia lui mult1 de la pozitia pos pana la final
				mult1[i] = mult[i - 1];
			}

			card++;
			delete[] mult;	// prevenim "memory leaks"
			mult = mult1;
		}
		else {	// in schimb, daca avem memorie pt inca un element, il inseram pur si simplu
			for (int i = card + 1; i > pos; i--) { // mutam toate elementele de la pos spre dreapta cu o pozitie
				mult[i] = mult[i - 1];
			}

			mult[pos] = elem;	// si inseram elem
			card++;
		}
	}
}

template<class T>
void Set<T>::Sterge(T elem) {
	// cautam elem; daca nu exista in multime, nu avem ce sterge
	int pos = Cauta(elem);
	if (pos == -1) {
		cout << "\nWarning: Elementul " << elem << " de sters nu se gaseste in multime!\n";
	}
	else {
		for (int i = pos; i < card - 1; i++) {
			mult[i] = mult[i + 1];
		}
		card--;
	}
}

template<class T>
T Set<T>::Suma() {
	T s = 0;
	for (int i = 0; i < card; i++) {
		s = s + mult[i];
	}
	return s;
}

template<class T>
Set<T> Set<T>::Par() {
	Set<T> P;
	for (int i = 0; i < card; i++)
		if (mult[i] % 2 == 0) {
			P.Insereaza(mult[i]);
		}
	return P;
}

template<class T>
Set<T> Set<T>::Impar() {
	Set<T> I;
	for (int i = 0; i < card; i++)
		if (mult[i] % 2 != 0) {
			I.Insereaza(mult[i]);
		}
	return I;
}

template<class T>
int Set<T>::Cardinal() {
	return card;
}

template<class T>
Set<T>& Set<T>::operator= (const Set<T> &A) {
	card = A.card;
	maxCard = A.maxCard;

	delete[] mult;					// eliminam ce continea setul inainte
	mult = new T[maxCard];		// pentru a-l popula cu elemente din setul A
	for (int i = 0; i < card; i++) {
		mult[i] = A.mult[i];
	}

	return *this;
}

template<class T>
T& Set<T>::operator[] (int i) {
	if (i >= 0 && i < card) {
		return mult[i];
	}

	cout << "\nEroare: Nu exista niciun element pe pozitia " << i << "!\n";
	exit(1);
}

template<class T1, class T2>
bool operator< (const Set<T1> &A, const Set<T2> &B) {
	return A.card < B.card;
}

template<class T1, class T2>
bool operator> (const Set<T1> &A, const Set<T2> &B) {
	return A.card > B.card;
}

template<class T>
ostream& operator<< (ostream &stream, const Set<T> &A) {
	for (int i = 0; i < A.card; i++)
		stream << A.mult[i] << " ";
	return stream;
}

template<class T>
istream& operator>> (istream &stream, Set<T> &A) {
	int n;
	T x;
	cout << "Nr elemente: ";
	stream >> n;	// citim nr de elemente
	for (int i = 0; i < n; i++) {
		cout << "Elementul " << i << ":\n";
		stream >> x;	// citim elementul i
		A.Insereaza(x);
	}
	return stream;
}

template<class T>
Set<T>& operator+ (const Set<T> &A, const Set<T> &B) {
	// pentru a realiza reuniunea celor doua multimi (sortate crescator)
	// putem realiza o interclasare inteligenta (care nu pune duplicate) in timp O(A.card + B.card)
	// iar in final, multimea obtinuta ramane sortata crescator

	Set<T> *C;
	C = new Set<T>;
	int i = 0, j = 0;

	while (i < A.card && j < B.card) {
		if (A.mult[i] < B.mult[j]) {
			C->Insereaza(A.mult[i]);
			i++;
		}
		else if (A.mult[i] > B.mult[j]) {
			C->Insereaza(B.mult[j]);
			j++;
		}
		else {					// aici A[i] == B[j]
			C->Insereaza(A.mult[i]);	// aleg in mod arbitrar sa introduc din A
			i++;						// si sa il maresc pe i
		}
	}

	while (i < A.card) {
		C->Insereaza(A.mult[i]);
		i++;
	}

	while (j < B.card) {
		C->Insereaza(B.mult[j]);
		j++;
	}

	return *C;
}

template<class T>
Set<T>& operator* (T k, const Set<T> &A) {
	// inmultim fiecare element din multimea A cu scalarul k si il inseram corespunzator in noul set rezultat
	Set<T> *C;
	C = new Set<T>;

	for (int i = 0; i < A.card; i++) {
		C->Insereaza(k * A.mult[i]);
	}

	return *C;
}

template<class T>
Set<T>& operator* (const Set<T> &A, T k) {
	// inmultim fiecare element din multimea A cu scalarul k si il inseram corespunzator in noul set rezultat
	Set<T> *C;
	C = new Set<T>;

	for (int i = 0; i < A.card; i++) {
		C->Insereaza(A.mult[i] * k);
	}

	return *C;
}

#endif //SET_H
