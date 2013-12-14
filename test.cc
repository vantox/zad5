#include<iostream>
#include<vector>
#include<cassert>
#include "virus_genealogy.h"

using namespace std;

class Virus {
	public:
		typedef typename std::string id_type;
		Virus(id_type const &id) : id(id) { }
		id_type get_id() const { return id; }
	private:
		id_type id; 
};

void test1()
{
	VirusGenealogy<Virus> gens("AH1N1");
	//cout << gens.get_stem_id() << "\n";
	gens.create("AH1N2", "AH1N1");
	
	vector<typename Virus::id_type> tmp;
	tmp.push_back("AH1N2");
	tmp.push_back("AH1N1");
	//gens.create("AH1N3", tmp);
	
	gens.printAll();

	gens.remove("AH1N2");
	
	cout << endl << endl << endl;
	
	gens.printAll();
	
	vector<typename Virus::id_type> c = gens.get_children("AH1N1");
	//cout << c.size() << "\n";
	//cout << c[0] << endl;
	
	vector<typename Virus::id_type> p = gens.get_parents("AH1N1");
	//cout << p.size() << "\n";
		
	//cout << gens.exists("AH1N1") << "\n" << gens.exists("AH1N2") << "\n" ;
}

void pchar_test()
{
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
	
void memory_test()
{
	VirusGenealogy<Virus> gens("A");
	gens.create("B", "A");
	gens.create("C", "B");
	gens.create("D", "C");
	//gens.remove("B");
	
}
int main()
{
	//test1();
	pchar_test();
	//memory_test();
	return 0;
}