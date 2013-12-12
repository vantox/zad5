#include<iostream>
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
	
	
	return 0;
}