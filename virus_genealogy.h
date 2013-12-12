#ifndef VIRUS_GENEALOGY_H
#define VIRUS_GENEALOGY_H

#include<vector>
#include<memory>
#include<map>
#include<utility>
#include<set>
using namespace std;

template<class Virus>
class VirusGenealogy{
	private:
		class Virus_node{
			public:
				Virus virus;
				set<shared_ptr<Virus_node> > children, parents;
				Virus_node(Virus virus) : virus(virus) {}
				
				vector<typename Virus::id_type> get_children()
				{
					vector<typename Virus::id_type> v;
					for (shared_ptr<Virus_node> s : children)
						v.push_back(s->virus.get_id());
					return v;
				}
				
				vector<typename Virus::id_type> get_parents()
				{
					vector<typename Virus::id_type> v;
					for (shared_ptr<Virus_node> s : parents)
						v.push_back(s->virus.get_id());
					return v;
				}
				
			
		};
				
		shared_ptr<Virus_node> stem_node;
		map<typename Virus::id_type, shared_ptr<Virus_node> > all_viruses;
	public:		
		// Tworzy nową genealogię.
		// Tworzy także węzęł wirusa macierzystego o identyfikatorze stem_id.
		VirusGenealogy(typename Virus::id_type const &stem_id) :
		stem_node(new Virus_node(Virus(stem_id)))
		{
			all_viruses.insert(make_pair(stem_id, stem_node));
		}

		// Zwraca identyfikator wirusa macierzystego.
		typename Virus::id_type get_stem_id() const
		{
			return stem_node->virus.get_id();
		}
		
		// Zwraca listę identyfikatorów bezpośrednich następników wirusa
		// o podanym identyfikatorze.
		// Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
		vector<typename Virus::id_type> get_children(typename Virus::id_type const &id) const
		{
			auto it = all_viruses.find(id);
			if(it == all_viruses.end()){
				//rzuc wyjatek
				exit(0);
			}
			else{
				return it->second->get_children();
			}
		}

		// Zwraca listę identyfikatorów bezpośrednich poprzedników wirusa
		// o podanym identyfikatorze.
		// Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
		vector<typename Virus::id_type> get_parents(typename Virus::id_type const &id) const
		{
			auto it = all_viruses.find(id);
			if(it == all_viruses.end()){
				//rzuc wyjatek
				exit(0);
			}
			else{
				return it->second->get_parents();
			}
		}
		
		// Sprawdza, czy wirus o podanym identyfikatorze istnieje.
		bool exists(typename Virus::id_type const &id) const
		{
			auto it = all_viruses.find(id);
			if(it == all_viruses.end())
				return false;
			else
				return true;
		}
			

		// Zwraca referencję do obiektu reprezentującego wirus o podanym
		// identyfikatorze.
		// Zgłasza wyjątek VirusNotFound, jeśli żądany wirus nie istnieje.
		Virus& operator[](typename Virus::id_type const &id) const;

		// Tworzy węzęł reprezentujący nowy wirus o identyfikatorze id
		// powstały z wirusów o podanym identyfikatorze parent_id lub
		// podanych identyfikatorach parent_ids.
		// Zgłasza wyjątek VirusAlreadyCreated, jeśli wirus o identyfikatorze
		// id już istnieje.
		// Zgłasza wyjątek VirusNotFound, jeśli któryś z wyspecyfikowanych
		// poprzedników nie istnieje.
		void create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id);
		void create(typename Virus::id_type const &id, vector<typename Virus::id_type> const &parent_ids);

		// Dodaje nową krawędź w grafie genealogii.
		// Zgłasza wyjątek VirusNotFound, jeśli któryś z podanych wirusów nie istnieje.
		void connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id);

		// Usuwa wirus o podanym identyfikatorze.
		// Zgłasza wyjątek VirusNotFound, jeśli żądany wirus nie istnieje.
		// Zgłasza wyjątek TriedToRemoveStemVirus przy próbie usunięcia
		// wirusa macierzystego.
		void remove(typename Virus::id_type const &id);
		
};

/*

template<class Virus>
vector<typename Virus::id_type> VirusGenealogy<Virus>::get_parents(typename Virus::id_type const &id) const;
vector<typename Virus::id_type> VirusGenealogy<Virus>::get_parents()
		
*/
#endif