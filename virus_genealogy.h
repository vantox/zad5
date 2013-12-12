#ifndef VIRUS_GENEALOGY_H
#define VIRUS_GENEALOGY_H

#include<vector>
#include<memory>
#include<map>
#include<utility>
using namespace std;

template<class Virus>
class VirusGenealogy{
	private:
		class Virus_node{
			public:
				Virus virus;
				vector<shared_ptr<Virus_node> > children, parents;
				Virus_node(Virus virus);
				
			
		};
				
		shared_ptr<Virus_node> stem_node;
		map<typename Virus::id_type, shared_ptr<Virus_node> > all_viruses;
	public:		
		// Tworzy nową genealogię.
		// Tworzy także węzęł wirusa macierzystego o identyfikatorze stem_id.
		VirusGenealogy(typename Virus::id_type const &stem_id);

		// Zwraca identyfikator wirusa macierzystego.
		 typename Virus::id_type get_stem_id() const;

		
};

template<class Virus>
VirusGenealogy<Virus>::Virus_node::Virus_node(Virus virus) : virus(virus) {}

template<class Virus>
VirusGenealogy<Virus>::VirusGenealogy(typename Virus::id_type const &stem_id) :
stem_node(new Virus_node(Virus(stem_id)))
{
	all_viruses.insert(make_pair(stem_id, stem_node));
}

template<class Virus>
typename Virus::id_type VirusGenealogy<Virus>::get_stem_id() const
{
	return stem_node->virus.get_id();
}


#endif