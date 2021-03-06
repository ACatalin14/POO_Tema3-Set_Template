#include "stdafx.h"
#include <iostream>
#include "Set.h"

using namespace std;

// Conventii de denumire:
//		toate variabilele se vor denumi in stil camelCase
//		toate functiile se vor denumi in stil PascalCase
//		exceptie: variabilele care denumesc obiecte de tip Set se vor nota cu o singura litera majuscula ca in matematica

// Conventie:
//		Indexarea elementelor in seturi se face de la 0!

struct complex {	// o structura pe care am facut-o doar cu scop demonstrativ ca programul ruleaza pe orice tip de date
	double re, im;

	complex() {};
	complex(double r, double i = 0) : re(r), im(i) {};
	
	bool operator== (const complex &z) {	// == necesar pt inserarea unui complex in set
		return re == z.re && im == z.im;
	}

	bool operator< (const complex &z) {		// < necesar pt inserarea unui complex in set
		if (re == z.re) {
			return im < z.im;
		}
		return re < z.re;
	}

	bool operator> (const complex &z) {		// > necesar pt inserarea unui complex in set
		if (re == z.re) {
			return im > z.im;
		}
		return re > z.re;
	}

	complex operator* (const complex z) {	// * necesar pt inmultirea setului cu un scalar
		complex rez;
		rez.re = re * z.re - im * z.im;
		rez.im = re * z.im + im * z.re;
		return rez;
	}

	complex operator+ (const complex z) {	// + necesar pt suma elementelor dintr-un set
		return complex(re + z.re, im + z.im);
	}

	friend istream& operator>> (istream& in, complex &z) {		// >> optional (l-am scris pt completarea lui <<)
		cout << "Re = ";
		in >> z.re;
		cout << "Im = ";
		in >> z.im;
		return in;
	}

	friend ostream& operator<< (ostream& out, const complex &z) {	// << necesar pt afisarea setului de nr complexe
		out << z.re << (z.im >= 0 ? "+" : "-") << abs(z.im) << "i";
		return out;
	}
};

int main() {
	Set<int> A;
	Set<double> B(100);
	Set<complex> C(10);
	Set<char> D(5, 'o', 'a', 'u', 'e', 'i');
	Set<char> E(D);
	cout << "Citire Set<int> A:\n";
	cin >> A;

	B.Insereaza(4.49);
	B.Insereaza(-6);
	B.Insereaza(6.28318);
	B.Insereaza(14.44);
	B.Insereaza(0);
	B.Insereaza(-6);

	C.Insereaza(complex(4, 3));
	C.Insereaza(complex(3));
	C.Insereaza(complex(4, -1.5));
	C.Insereaza(complex(0, 5));
	C.Insereaza(complex(2, 9));
	C.Insereaza(complex(0, 5));

	cout << "\nDupa citire/initializari/inserari:";
	cout << "\nA: " << A;
	cout << "\nB: " << B;
	cout << "\nC: " << C;
	cout << "\nD: " << D;
	cout << "\nE: " << E;

	B.Sterge(-6);
	B.Sterge(6.28318);
	C.Sterge(complex(4, -1.5));
	D.Sterge('a');
	D.Sterge('u');
	E.Sterge('e');
	E.Sterge('i');

	cout << "\n\nDupa stergeri:";
	cout << "\nA: " << A;
	cout << "\nB: " << B;
	cout << "\nC: " << C;
	cout << "\nD: " << D;
	cout << "\nE: " << E;

	cout << "\n\nCautari:";
	cout << "\n57 apare in A pe poz: " << A.Cauta(57);
	cout << "\n4.49 apare in B pe poz: " << B.Cauta(4.49);
	cout << "\n\'e\' apare in D pe poz: " << D.Cauta('e');

	cout << "\n\nAccesari:";
	cout << "\nA[0] = " << A[0];
	cout << "\nC[1] = " << C[1];

	cout << "\n\nD: " << D;
	cout << "\nE: " << E;
	E = D + E;
	cout << "\nReuniunea E = D + E:";
	cout << "\nE: " << E;
	

	cout << "\n\nComparari:";
	cout << (A > B ? "\nA este mai mare ca B" : "\nA nu este mai mare ca B");
	cout << (A < E ? "\nA este mai mic ca E" : "\nA nu este mai mic ca E");

	cout << "\n\nInmultire cu scalar: ";
	cout << "\nA * -2: " << A * -2;
	cout << "\n1.5 * B: " << 1.5 * B;
	cout << "\n0+1i * C: " << complex(0, 1) * C;
	cout << "\nD * \'a\': " << D * 'a';

	cout << "\n\nC: " << C;
	cout << "\nE: " << E;
	cout << "\nSuma elementelor din C: " << C.Suma();
	cout << "\nSuma elementelor din E: " << E.Suma();

	cout << "\n\nA: " << A;
	cout << "\nElementele pare din A: " << A.Par();
	cout << "\nElementele impare din A: " << A.Impar();

	cout << "\n\nNumarul de elemente din A: " << A.Cardinal();
	cout << "\n";
	return 0;
}

