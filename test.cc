#include<iostream>
#include<vector>
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

int main()
{
	VirusGenealogy<Virus> gens("AH1N1");
	cout << gens.get_stem_id() << "\n";
	vector<typename Virus::id_type> c = gens.get_children("AH1N1");
	cout << c.size() << "\n";
	vector<typename Virus::id_type> p = gens.get_parents("AH1N1");
	cout << p.size() << "\n";
	cout << gens.exists("AH1N1") << "\n" << gens.exists("AH1N2") << "\n" ;
	return 0;
}