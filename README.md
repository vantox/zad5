=== Zadanie Genealogia Wirusów ===


Biolog, zajmujący się konstruowaniem wirusów, potrzebuje klasy do
manipulowania informacjami o ich genealogii. Nowy wirus powstaje przez
mutację jakiegoś znanego wirusa. Może też powstać przez zmutowanie
połączenia więcej niż jednego wirusa. Genealogia wirusa jest grafem
skierowanym acyklicznym o jednym źródle, które reprezentuje wirusa
macierzystego. Wierzchołek grafu odpowiada konkretnej mutacji wirusa.
Krawędź łączy wirusa z bezpośrednio otrzymaną z niego mutacją.

Należy zaimplementować szablon klasy, która reprezentuje taki graf.

template <class Virus> VirusGenealogy;

Klasa Virus reprezentuje informacje o wirusie. Jej implementacja
zostanie dostarczona w stosownym czasie.

Klasa VirusGenealogy powinna udostępniać następujący interfejs.

// Tworzy nową genealogię.
// Tworzy także węzęł wirusa macierzystego o identyfikatorze stem_id.
VirusGenealogy(Virus::id_type const &stem_id);

// Zwraca identyfikator wirusa macierzystego.
Virus::id_type get_stem_id() const;

// Zwraca listę identyfikatorów bezpośrednich następników wirusa
// o podanym identyfikatorze.
// Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
std::vector<Virus::id_type> get_children(Virus::id_type const &id) const;

// Zwraca listę identyfikatorów bezpośrednich poprzedników wirusa
// o podanym identyfikatorze.
// Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
std::vector<Virus::id_type> get_parents(Virus::id_type const &id) const;

// Sprawdza, czy wirus o podanym identyfikatorze istnieje.
bool exists(Virus::id_type const &id) const;

// Zwraca referencję do obiektu reprezentującego wirus o podanym
// identyfikatorze.
// Zgłasza wyjątek VirusNotFound, jeśli żądany wirus nie istnieje.
Virus& operator[](Virus::id_type const &id) const;

// Tworzy węzęł reprezentujący nowy wirus o identyfikatorze id
// powstały z wirusów o podanym identyfikatorze parent_id lub
// podanych identyfikatorach parent_ids.
// Zgłasza wyjątek VirusAlreadyCreated, jeśli wirus o identyfikatorze
// id już istnieje.
// Zgłasza wyjątek VirusNotFound, jeśli któryś z wyspecyfikowanych
// poprzedników nie istnieje.
void create(Virus::id_type const &id, Virus::id_type const &parent_id);
void create(Virus::id_type const &id, std::vector<Virus::id_type> const &parent_ids);

// Dodaje nową krawędź w grafie genealogii.
// Zgłasza wyjątek VirusNotFound, jeśli któryś z podanych wirusów nie istnieje.
void connect(Virus::id_type const &child_id, virus::id_type const &parent_id);

// Usuwa wirus o podanym identyfikatorze.
// Zgłasza wyjątek VirusNotFound, jeśli żądany wirus nie istnieje.
// Zgłasza wyjątek TriedToRemoveStemVirus przy próbie usunięcia
// wirusa macierzystego.
void remove(Virus::id_type const &id);

Zakładamy, że:
* klasa Virus ma konstruktor przyjmujący argument typu Virus::id_type;
* klasa Virus ma metodę Virus::id_type get_id() const;
* typ Virus::id_type ma konstruktor bezargumentowy, konstruktor
  kopiujący, operator przypisania;
* wartości typu Virus::id_type tworzą porządek liniowy i można je
  porównywać za pomocą operatorów ==, !=, <=, >=, <, >.

Ponadto:
* wszystkie metody klasy VirusGenealogy powinny gwarantować silną odporność
  na wyjątki, a tam, gdzie to jest możliwe i pożądane, powinny być no-throw;
* próba użycia konstruktora kopiującego lub operatora przypisania dla
  obiektów klasy VirusGenealogy powinna zakończyć się błędem kompilacji;
* zachowanie obiektu typu VirusGenealogy po utworzeniu cyklu pozostaje
  niezdefiniowane -- nie trzeba wykrywać takiej sytuacji;
* wyjątki VirusAlreadyCreated, VirusNotFound oraz TriedToRemoveStemVirus
  powinny być zdefiniowane poza klasą VirusGenealogy i powinny dziedziczyć
  z std::exception;
* wyszukiwanie wirusów powinno być szybsze niż liniowe.

Zarządzanie pamięcią powinno być zrealizowane za pomocą inteligentnych
wskaźników tak, żeby wierzchołek był dealokowany w momencie usunięcia
ostatniego jego poprzednika. Można to zrealizować, używając odpowiednich
szablonów z biblioteki, bądź przez zaimplementowanie własnego takiego
szablonu.

template <class T> class SharedPtr {
  public:
    // Tworzy pusty wskaźnik, na nic niewskazujący.
    // Metoda get() powinna zwrócić wskaźnik 0 (NULL, nullptr).
    SharedPtr();

    // Przestaje być właścicielem wskazywanego obiektu.
    ~SharedPtr();

    // Przejmuje na własność obiekt wskazywany przez pointer.
    SharedPtr(T* pointer);

    // Staje się współwłaścicielem obiektu wskazywanego przez other.
    SharedPtr(SharedPtr<T> const &other);

    // Zwraca przechowywany wskaźnik, ale pozostaje właścicielem
    // obiektu.
    T* get();

    // Staje się (współ)właścicielem obiektu wskazywanego przez other.
    // Przestaje być właścicielem obiektu dotychczas wskazywanego.
    SharedPtr<T>& operator=(SharedPtr<T> const &other);
    SharedPtr<T>& operator=(T* other);

    // Zwraca true wtw, gdy porównywane wskaźniki wskazują na ten sam
    // obiekt.
    bool operator==(const SharedPtr<T>& other) const;

  private:
    // Tutaj już możesz umieścić, co Ci pasuje.
};

=== Przykład użycia ===

#include "virus_genealogy.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

class Virus {
  public:
    typedef typename std::string id_type;
    Virus(id_type const &_id) : id(_id) {
    }
    id_type get_id() const {
      return id;
    }
  private:
    id_type id;
};

int main() {
  VirusGenealogy<Virus> gen("A1H1");
  Virus::id_type const id1 = gen.get_stem_id();
  assert(gen.exists(id1));
  assert(gen.get_parents(id1).size() == 0);
  gen.create("A", id1);
  gen.create("B", id1);
  assert(gen.get_children(id1).size() == 2);
  gen.create("C", "A");
  gen.connect("C", "B");
  assert(gen.get_parents("C").size() == 2);
  assert(gen.get_children("A").size() == 1);
  std::vector<Virus::id_type> parents;
  parents.push_back("A");
  parents.push_back("B");
  gen.create("D", parents);
  assert(gen.get_parents("D").size() == parents.size());
  assert(gen.get_children("A").size() == 2);
  assert("D" == gen["D"].get_id());
  gen.remove("A");
  assert(!gen.exists("A"));
  assert(gen.exists("B"));
  assert(gen.exists("C"));
  assert(gen.exists("D"));
  gen.remove("B");
  assert(!gen.exists("A"));
  assert(!gen.exists("B"));
  assert(!gen.exists("C"));
  assert(!gen.exists("D"));
  try {
    gen["E"];
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  try {
    gen.create("E", "A1H1");
    gen.create("E", "A1H1");
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  try {
    gen.remove("A1H1");
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

Powyższy program powinien wypisać na standardowe wyjście coś w stylu:

VirusNotFound
VirusAlreadyCreated
TriedToRemoveStemVirus

=== Ustalenia techniczne ===

Jako rozwiązanie należy dostarczyć plik virus_genealogy.h, który
należy umieścić w repozytorium w katalogu

grupaN/zadanie5/ab123456+cd123456

lub

grupaN/zadanie5/ab123456+cd123456+ef123456

gdzie N jest numerem grupy, a ab123456, cd123456, ef123456 są
identyfikatorami członków zespołu umieszczającego to rozwiązanie.
Katalog z rozwiązaniem nie powinien zawierać innych plików, ale może
zawierać podkatalog private, gdzie można umieszczać różne pliki, np.
swoje testy. Pliki umieszczone w tym podkatalogu nie będą oceniane.
