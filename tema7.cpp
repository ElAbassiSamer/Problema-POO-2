#include <iostream>
#include <fstream>
#include <ostream>
#include <istream>

using namespace std;

int N = 100;

ifstream f("date.in");
ofstream g("date.out");

// Tipul unui element din structurile de date abstracte
class Pereche
{
protected:
	int prim;
	int doi;

public:
	Pereche() 
	{
	}

	Pereche(int prim, int doi)
	{
		this->prim = prim;
		this->doi = doi;
	}

	Pereche(const Pereche& p)
	{
		this->prim = p.prim;
		this->doi = p.doi;
	}

	friend bool operator==(const Pereche &p1, const Pereche &p2);
	friend ostream& operator<<(ostream& out, const Pereche &p);
	friend istream& operator>>(istream& in, Pereche &p);

	Pereche& operator=(const Pereche& rhs) {
		prim = rhs.prim;
		doi = rhs.doi;

		return *this;
	}
};

// Tipul structura de date abstracte ca tip de baza, abstract. Cuprinde metodele care vor fi disponibile in clasele care mostenesc acest tip.
class Sda
{
public:
	virtual void add(Pereche p) = 0;
	virtual bool remove(Pereche p) = 0;
	virtual bool isEmpty() = 0;
	virtual int count() = 0;
	virtual void display(ostream& out) = 0;
};

class MultimePereche : Sda
{
public:
	MultimePereche()
	{
		this->maxSize = N;
		this->nrElemente = 0;
		this->date = new Pereche[maxSize];
	}

	MultimePereche(int n, Pereche* p) : MultimePereche()
	{
		for (int i = 0; i < n; i++)
		{
			this->add(p[i]);
		}
	}

	// Constructor copiere
	MultimePereche(const MultimePereche& mp)
	{
		this->maxSize = mp.maxSize;
		this->nrElemente = mp.nrElemente;
		this->date = new Pereche[mp.maxSize];

		for (int i = 0; i < mp.nrElemente; i++)
		{
			this->date[i] = Pereche(mp.date[i]);
		}
	}

	~MultimePereche()
	{
		delete[] date;
	}

	// Metoda ajutatoare pentru citire. Va fi apelata din metoda ce supraincarca operatorul >> (inclusiv din clasele mostenitoare).
	void read(istream& in)
	{		
		cout << "Introduceti numarul de perechi: ";
		in >> nrElemente;
		date = new Pereche[nrElemente];
		for (int i = 0; i < nrElemente; i++)
		{
			in >> date[i];
		}
	}

	// Metoda ajutatoare pentru afisare. Va fi apelata din metoda ce supraincarca operatorul << (inclusiv din clasele mostenitoare).
	void display(ostream& out)
	{
		out << "{ ";
		for (int i = 0; i < nrElemente; i++)
		{
			out << this->date[i];
		}
		out << " }" << endl;
	}

	// Adauga un element multimii. Metoda poate fi redefinita in clasele mostenitoare, intrucat este virtuala.
	virtual void add(Pereche p)
	{
		this->prepareStorage();
		if (this->rightIndexOf(p) != -1)
		{
			return;
		}

		this->date[this->nrElemente] = Pereche(p);
		this->nrElemente++;
	}

	// Scoate un element din multime iterand de la stanga la dreapta. Metoda poate fi redefinita in clasele mostenitoare, intrucat este virtuala.
	virtual bool remove(Pereche p)
	{
		if (this->isEmpty())
		{
			// Not found
			return false;
		}

		int index = this->leftIndexOf(p);
		if (index == -1)
		{
			// Not found
			return false;
		}

		for (int i = index; i < this->nrElemente - 1; i++)
		{
			this->date[i] = this->date[i + 1];
		}
		this->nrElemente--;

		return true;
	}

	// Verifica daca multimea e vida
	virtual bool isEmpty()
	{
		return this->nrElemente == 0;
	}

	// Nr elemente ale multimii
	virtual int count()
	{
		return this->nrElemente;
	}

	friend ostream & operator<<(ostream & out, const MultimePereche &mp);
	friend istream & operator>>(istream  & in, MultimePereche &mp);
	
	MultimePereche& operator=(const MultimePereche rhs) {
		this->maxSize = rhs.maxSize;
		this->nrElemente = rhs.nrElemente;
		this->date = rhs.date;

		return *this;
	}

protected:
	// Capacitatea maxima a multimii (i.e. numarul total de sloturi de memorie rezervate in care am putea pune date)
	int maxSize;
	// Capacitatea efectiva a multimii (i.e. numarul de soloturi efectiv ocupate cu date)
	int nrElemente;
	// Datele multimii
	Pereche* date;

	// In cazul in care capacitatea efectiva atinge capacitatea maxima realocam memoria, marind capacitatea maxima. 
	void reallocate(int newSize)
	{
		// Retinem datele existente
		Pereche* copy = new Pereche[this->nrElemente];
		for (int i = 0; i < this->nrElemente; i++)
		{
			copy[i] = Pereche(this->date[i]);
		}

		// Eliberam memoria
		delete[]this->date;

		// Alocam un spatiu nou de capacitate marita
		this->date = new Pereche[newSize];

		// Copiem datele in noul spatiu alocat
		for (int i = 0; i < this->nrElemente; i++)
		{
			this->date[i] = Pereche(copy[i]);
		}

		// Eliberam memoria
		delete[]copy;

		this->maxSize = newSize;
	}

	// In cazul in care capacitatea maxima == capacitatea efectiva + 1, crestem capacitatea containerului.
	void prepareStorage() {
		if (this->nrElemente + 1 > maxSize)
		{
			reallocate(this->nrElemente >> 1);
		}
	}

	int leftIndexOf(Pereche p)
	{
		for (int i = 0; i < nrElemente; i++)
		{
			if (date[i] == p)
			{
				return i;
			}
		}

		return -1;
	}

	int rightIndexOf(Pereche p)
	{
		for (int i = nrElemente - 1; i >= 0; i--)
		{
			if (date[i] == p)
			{
				return i;
			}
		}

		return -1;
	}
};

class StivaPereche : public MultimePereche
{
public:
	StivaPereche() : MultimePereche()
	{
	}

	StivaPereche(int n, Pereche* p) : MultimePereche(n, p)
	{

	}

	StivaPereche(const StivaPereche& s) : MultimePereche(s.nrElemente, s.date)
	{

	}

	/*
	 * NU E NEVOIE DE DESTRUCTOR INTRUCAT:
	 * - clasa StivaPereche nu defineste variabile locale altele decat cele mostenite si deci nu e nevoie se dealocam nimic
	 * - implicit se va apela destructorul clasei parinte care va duce la eliberarea datelor alocate in heap
	 */

	virtual bool remove(Pereche p) {
		int index = this->rightIndexOf(p);
		if (index == -1) {
			return false;
		}

		for (int i = index; i < nrElemente - 1; i++)
		{
			date[i] = date[i + 1];
		}

		nrElemente--;
		return true;
	}

	void push(Pereche p)
	{
		this->add(p);
	}

	Pereche* pop()
	{
		if (this->isEmpty())
		{
			return NULL;
		}

		Pereche safeCopy = Pereche(this->date[this->nrElemente - 1]);
		this->remove(safeCopy);

		return new Pereche(safeCopy);
	}

	friend ostream & operator<<(ostream& out, const StivaPereche &sp);
	friend istream & operator>>(istream& in, StivaPereche &sp);

	StivaPereche& operator=(const StivaPereche rhs) {
		this->maxSize = rhs.maxSize;
		this->nrElemente = rhs.nrElemente;
		this->date = rhs.date;

		return *this;
	}
};

class CoadaPereche : public MultimePereche
{
public:
	CoadaPereche() : MultimePereche()
	{
	}


	CoadaPereche(int n, Pereche* p) : MultimePereche(n, p)
	{
	}

	CoadaPereche(const CoadaPereche& c) : MultimePereche(c.nrElemente, c.date)
	{
	}

	/*
	* NU E NEVOIE DE DESTRUCTOR INTRUCAT:
	* - clasa CoadsaPereche nu defineste variabile locale altele decat cele mostenite si deci nu e nevoie se dealocam nimic
	* - implicit se va apela destructorul clasei parinte care va duce la eliberarea datelor alocate in heap
	*/

	void enqueue(Pereche p)
	{
		this->add(p);
	}

	Pereche* dequeue()
	{
		if (this->isEmpty())
		{
			return NULL;
		}

		Pereche safeCopy = Pereche(date[0]);
		this->remove(safeCopy);

		return new Pereche(safeCopy);
	}

	friend ostream & operator<<(ostream & out, const CoadaPereche &mp);
	friend istream & operator>>(istream  & in, CoadaPereche &mp);

	CoadaPereche& operator=(const CoadaPereche rhs) {
		this->maxSize = rhs.maxSize;
		this->nrElemente = rhs.nrElemente;
		this->date = rhs.date;

		return *this;
	}
};

bool operator ==(const Pereche &p1, const Pereche &p2)
{
	return p1.prim == p2.prim && p1.doi == p2.doi;
}


ostream& operator <<(ostream &out, const Pereche &p)
{
	out << "(" << p.prim << "," << p.doi << ") ";

	return out;
}

istream& operator >>(istream &in, Pereche &p)
{
	cout << "Introduceti pereche: ";
	in >> p.prim;
	in >> p.doi;
	return in;
}

ostream& operator << (ostream &out, const MultimePereche &mp)
{
	((MultimePereche) mp).display(out);
	return out;
}

istream& operator >> (istream &in, MultimePereche &mp)
{
	mp.read(in);
	return in;
}


ostream& operator << (ostream &out, const CoadaPereche &cp)
{
	((MultimePereche)cp).display(out);
	return out;
}

istream& operator >> (istream &in, CoadaPereche &cp)
{
	cp.read(in);
	return in;
}

ostream& operator << (ostream &out, const StivaPereche &sp)
{
	((MultimePereche)sp).display(out);
	return out;
}

istream& operator >> (istream &in, StivaPereche &sp)
{
	sp.read(in);
	return in;
}

void pushPrinCozi(CoadaPereche& cm1, CoadaPereche& cm2, Pereche p)
{
	if (cm1.isEmpty())
	{
		cm2.enqueue(p);
	}
	else {
		cm1.enqueue(p);
	}
}

Pereche* popPrinCozi(CoadaPereche& cm1, CoadaPereche& cm2)
{
	if (cm1.isEmpty() && cm2.isEmpty())
	{
		return NULL;
	}

	CoadaPereche* src = cm1.isEmpty() ? &cm2 : &cm1;
	CoadaPereche* dest = cm1.isEmpty() ? &cm1 : &cm2;
	int count = src->count();

	while (count > 1)
	{
		Pereche* p = src->dequeue();
		dest->enqueue(*p);
		count--;
	}

	return src->dequeue();
}

void afisareStivaPrinCozi(CoadaPereche cm1, CoadaPereche cm2)
{
	if (cm1.isEmpty())
	{
		cm1.display(cout);
	}
	else {
		cm2.display(cout);
	}
}

void testGeneralCitireAfisare()
{
	// Citeste o multime de perechi si o afiseaza	
	MultimePereche m;
	cin >> m;
	cout << "Multimea citita:" << endl << m;

	Pereche p1 = Pereche(1001, 1002);
	Pereche p2 = Pereche(1002, 1003);
	Pereche p3 = Pereche(1003, 1004);

	// Initializeaza o stiva de perechi pe baza multimii citite anterior
	StivaPereche s;
	cin >> s;
	cout << "Stiva citita:" << s << endl;

	// Adauga niste elemente
	s.push(p1);
	s.push(p2);
	s.push(p3);

	cout << "Stiva dupa adaugarea unor elemente:" << endl << s;

	// Scoate un element
	Pereche* p = s.pop();

	cout << "Stiva dupa o operatie pop():" << endl << s;

	CoadaPereche c;
	cin >> c;
	cout << "Coada citita:" << endl << c;

	// Adauga niste elemente
	c.enqueue(p1);
	c.enqueue(p2);
	c.enqueue(p3);

	cout << "Coada dupa adaugarea unor elemente:" << endl << c;

	p = c.dequeue();
	cout << "Coada dupa o operatie dequeue()" << endl << c;
}

void testAddRemove()
{
	Pereche p1 = Pereche(1, 2);
	Pereche p2 = Pereche(3, 4);
	Pereche p3 = Pereche(5, 6);

	MultimePereche mp;
	CoadaPereche cp;
	StivaPereche sp;

	mp.add(p1); cp.add(p1); sp.add(p1);
	mp.add(p2); cp.add(p2); sp.add(p2);
	mp.add(p3); cp.add(p3); sp.add(p3);

	// Elementele adaugate trebuie sa poata fi scoase
	if (!mp.remove(p1) || !mp.remove(p2) || !mp.remove(p3))
	{
		cout << "Test 1 multime esuat!";
		exit(0);
	}

	if (!mp.isEmpty())
	{
		cout << "Test 2 empty esuat!";
		exit(0);
	}

	// Elementele adaugate trebuie sa poata fi scoase o singura data
	if (mp.remove(p1) || mp.remove(p2) || mp.remove(p3))
	{
		cout << "Test 3 multime vida esuat!";
		exit(0);
	}

	// Elementele adaugate trebuie sa poata fi scoase
	if (!cp.remove(p1) || !cp.remove(p2) || !cp.remove(p3))
	{
		cout << "Test 4 coada esuat!";
		exit(0);
	}

	if (!cp.isEmpty())
	{
		cout << "Test 5 coada vida esuat!";
		exit(0);
	}

	// Elementele adaugate trebuie sa poata fi scoase o singura data
	if (cp.remove(p1) || cp.remove(p2) || cp.remove(p3))
	{
		cout << "Test 6 coada esuat!";
		exit(0);
	}

	// Elementele adaugate trebuie sa poata fi scoase
	if (!sp.remove(p1) || !sp.remove(p2) || !sp.remove(p3))
	{
		cout << "Test 7 coada esuat!";
		exit(0);
	}

	// Elementele adaugate trebuie sa poata fi scoase o singura data
	if (sp.remove(p1) || sp.remove(p2) || sp.remove(p3))
	{
		cout << "Test 8 coada esuat!";
		exit(0);
	}

	if (!sp.isEmpty())
	{
		cout << "Test 9 stiva vida esuat!";
		exit(0);
	}
}

void testStivaPrinCozi()
{
	CoadaPereche c1, c2;

	cout << "Cozile initiale: " << c1 << " si " << c2 << endl;

	Pereche p1 = Pereche(1, 2);
	Pereche p2 = Pereche(3, 4);
	Pereche p3 = Pereche(5, 6);
	Pereche p4 = Pereche(7, 8);

	pushPrinCozi(c1, c2, p1);
	pushPrinCozi(c1, c2, p2);
	pushPrinCozi(c1, c2, p3);

	cout << "Cozile dupa operatiile push(): " << c1 << " si " << c2 << endl;

	Pereche* rezultat = popPrinCozi(c1, c2);
	
	cout << "Elementul scos e " << *rezultat << endl;

	rezultat = popPrinCozi(c1, c2);

	cout << "Elementul scos e " << *rezultat << endl << endl;

	cout << "Cozile dupa 2 operatii pop(): " << c1 << " si " << c2 << endl;

	pushPrinCozi(c1, c2, p4);

	cout << "Cozile dupa 1 operatie push(): " << c1 << " si " << c2 << endl;
}

void testPushPopEnqueueDequeue()
{
	Pereche p1 = Pereche(1, 2);
	Pereche p2 = Pereche(3, 4);

	CoadaPereche cp;
	StivaPereche sp;

	cp.enqueue(p1); cp.enqueue(p2);
	sp.push(p1); sp.push(p2);

	Pereche* p = cp.dequeue();
	if (!(*p == p1))
	{
		cout << "Test 1 dequeue esuat" << endl;
		exit(0);
	}

	p = cp.dequeue();
	if (!(*p == p2))
	{
		cout << "Test 2 dequeue esuat" << endl;
		exit(0);
	}

	p = cp.dequeue();
	if (p != NULL || !cp.isEmpty()) {
		cout << "Test 3 dequeue esuat" << endl;
		exit(0);
	}

	p = sp.pop();
	if (!(*p == p2))
	{
		cout << "Test 4 pop esuat" << endl;
		exit(0);
	}

	p = sp.pop();
	if (!(*p == p1))
	{
		cout << "Test 5 pop esuat" << endl;
		exit(0);
	}

	p = sp.pop();
	if (p != NULL || !cp.isEmpty()) {
		cout << "Test 6 dequeue esuat" << endl;
		exit(0);
	}
}

int main(void) {
	//testGeneralCitireAfisare();
	//testAddRemove();
	//testPushPopEnqueueDequeue();

	testStivaPrinCozi();
}
