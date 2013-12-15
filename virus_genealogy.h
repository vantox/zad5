#ifndef VIRUS_GENEALOGY_H
#define VIRUS_GENEALOGY_H

#include<vector>
#include<map>
#include<set>
#include<utility>
#include<memory>
#include<exception>

using namespace std;

class VirusNotFound : public exception
{
	public:
		string s = "Virus not found.";
		VirusNotFound () {}
		virtual const char* what() const throw() 
		{
		return s.c_str();
		}
};

class VirusAlreadyCreated : public exception
{
	public:
		string s = "Virus already created.";
		VirusAlreadyCreated() {}
		virtual const char* what() const throw() 
		{
		return s.c_str();
		}
};

class TriedToRemoveStemVirus : public exception
{
	public:
		string s = "Tried to remove stem virus.";
		TriedToRemoveStemVirus() {}
		virtual const char* what() const throw() 
		{
		return s.c_str();
		}
};

template<class Virus>
class VirusGenealogy{
	private:
		class Virus_node{
			public:
				Virus virus;
				set<shared_ptr<Virus_node> > children, parents;
				Virus_node(Virus virus) : virus(virus) {}
				//w sumie to jest niepotrzebny ale głupio wygląda jak 
				//~VirusGenealogy jest a tego nie ma :P
				~Virus_node()
				{
					children.clear();
					parents.clear();
				}
				
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
					return v; // tu nie ma niepotrzebnego kopiowania wektora?
					//pewnie jest tyle że sygnatura bez & była podana w treści
				}
				
			
		};
		
		// Pomocnicza funkcja do rekurencyjnego usuwania grafu.
		void remove(shared_ptr<Virus_node> nd);
				
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
		
		~VirusGenealogy()
		{
			remove(stem_node);
		}
		
		// Zwraca identyfikator wirusa macierzystego.
		typename Virus::id_type get_stem_id() const
		{
			return stem_node->virus.get_id();
		}
		
		// Zwraca listę identyfikatorów bezpośrednich następników wirusa
		// o podanym identyfikatorze.
		// Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
		vector<typename Virus::id_type> get_children(typename Virus::id_type const &id) const;

		// Zwraca listę identyfikatorów bezpośrednich poprzedników wirusa
		// o podanym identyfikatorze.
		// Zgłasza wyjątek VirusNotFound, jeśli dany wirus nie istnieje.
		vector<typename Virus::id_type> get_parents(typename Virus::id_type const &id) const;
		
		// Sprawdza, czy wirus o podanym identyfikatorze istnieje.
		bool exists(typename Virus::id_type const &id) const;
		
			

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
		
		void printAll()
		{
			for(auto map_it = all_viruses.begin(); map_it != all_viruses.end(); ++map_it){
				
				cout << "wezel " << map_it->first << ":" << endl << "dziecioki: ";
				for(auto child_it = map_it->second->children.begin(); child_it != map_it->second->children.end(); ++child_it){
					cout << (*child_it)->virus.get_id() << " ";
				}
				cout << endl << "ojcowie: ";
				for(auto parent_it = map_it->second->parents.begin(); parent_it != map_it->second->parents.end(); ++parent_it){
					cout << (*parent_it)->virus.get_id() << " ";
				}
				cout << endl;
			}
		
		
		}
		
};


template<class Virus>
vector<typename Virus::id_type> VirusGenealogy<Virus>::get_children(typename Virus::id_type const &id) const
{
	auto it = all_viruses.find(id);
	if(it == all_viruses.end()){
		throw VirusNotFound();
	}
	else{
		return it->second->get_children();
	}
}

template<class Virus>		
vector<typename Virus::id_type> VirusGenealogy<Virus>::get_parents(typename Virus::id_type const &id) const
{
	auto it = all_viruses.find(id);
	if(it == all_viruses.end()){
		throw VirusNotFound();
	}
	else{
		return it->second->get_parents();
	}
}

template<class Virus>
bool VirusGenealogy<Virus>::exists(typename Virus::id_type const &id) const
{
	auto it = all_viruses.find(id);
	if(it == all_viruses.end())
		return false;
	else
		return true;
}

// [ADD] zakładam że nie zostanie zmieniony identyfikator wirusa
template<class Virus>
Virus& VirusGenealogy<Virus>::operator[](typename Virus::id_type const &id) const
{
	auto it = all_viruses.find(id);
	if(it == all_viruses.end()){
		throw VirusNotFound();
	}
	else{
		return it->second->virus;
	}
}

// [ADD]
template<class Virus>
void VirusGenealogy<Virus>::create(typename Virus::id_type const &id, typename Virus::id_type const &parent_id)
{
	if(all_viruses.find(id) != all_viruses.end()){
		throw VirusAlreadyCreated();
	}
	auto it = all_viruses.find(parent_id);
	if(it == all_viruses.end()){
		throw VirusNotFound();
	}
	
	shared_ptr<Virus_node> to_add(new Virus_node(Virus(id)));
	to_add->parents.insert(it->second);
	it->second->children.insert(to_add);
	all_viruses.insert(make_pair(id, to_add));
}

// [ADD]
template<class Virus>
void VirusGenealogy<Virus>::create(typename Virus::id_type const &id, vector<typename Virus::id_type> const &parent_ids)
{
	if(all_viruses.find(id) != all_viruses.end()){
		throw VirusAlreadyCreated();
	}
	if(parent_ids.size() == 0)
		throw VirusNotFound();
	
	auto it = all_viruses.begin(); 
	// auto == typename std::map<typename Virus::id_type, shared_ptr<Virus_node> >::iterator
	// w tej postaci chyba bardziej czytelne
	for(unsigned int i = 0; i < parent_ids.size(); ++i){
		static auto it = all_viruses.find(parent_ids.at(i));
		if(it == all_viruses.end()){
		throw VirusNotFound();
		}
	}
	
	shared_ptr<Virus_node> to_add(new Virus_node(Virus(id)));
	for(unsigned int i = 0; i < parent_ids.size(); ++i){
		it = all_viruses.find(parent_ids.at(i));
		to_add->parents.insert(it->second);
		it->second->children.insert(to_add);
		}
	all_viruses.insert(make_pair(id, to_add));
}

// [ADD]
template<class Virus>
void VirusGenealogy<Virus>::connect(typename Virus::id_type const &child_id, typename Virus::id_type const &parent_id)
{
	auto child_it = all_viruses.find(child_id);
	auto parent_it = all_viruses.find(parent_id);
	
	if(child_it == all_viruses.end() || parent_it == all_viruses.end()){
		throw VirusNotFound();
	}
	// co jesli ten syn juz jest ojcem nowego ojca? po cyklu niezdefiniowane zachowanie
	// co jesli krawedz istnieje? nei dodajemy kolejnej
	// co jesli ojciec == syn? jak pkt.1 cykl
	// co jesli syn == stem_node->virus.get_id()? w sumie nie mam pojęcia, chyba nie powinniśmy
	//stem node-a zmieniać
	
	child_it->second->parents.insert(parent_it->second);
	parent_it->second->children.insert(child_it->second);
}


// [ADD]
template<class Virus>
void VirusGenealogy<Virus>::remove(typename Virus::id_type const &id)
{
	if(id == stem_node->virus.get_id()){
		throw TriedToRemoveStemVirus();
	}
	
	if(all_viruses.find(id) == all_viruses.end()){
		throw VirusNotFound();
	}
	
	auto me = all_viruses.find(id)->second;
	remove(me);
}


// [ADD] tez te wskazniki pierwszy raz na oczy widze wiec moze byc zle :P
template<class Virus>
void VirusGenealogy<Virus>::remove(shared_ptr<Virus_node> me)
{
	typename set<shared_ptr<Virus_node> >::iterator it = me->parents.begin();
	
	while(it != me->parents.end()){
		if((*it)->children.find(me) != (*it)->children.end()){
			(*it)->children.erase(me);
			}
		++it;
	}
	// nie jestem juz niczyim synem
	
	it = me->children.begin();
	while(it != me->children.end()){
		if((*it)->parents.find(me) != (*it)->parents.end()){
			(*it)->parents.erase(me);
			if((*it)->parents.size() == 0)
				remove(*it); // usuwamy jak nie ma już żadnego ojca
		}
		++it;
	}
	
	// nikt nie uwaza mnie za ojca
	
	me->children.clear();
	me->parents.clear();
	
	all_viruses.erase(me->virus.get_id());
	
}

#endif