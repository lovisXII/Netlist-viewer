#pragma once
#include <iostream>
#include <string>
#include <vector>
#include  <cstdlib>
#include  <libxml/xmlreader.h>

#include "Term.h"

namespace Netlist {

class Cell ;
class Node ;
class Line ;


class Net
{

	/*
	Comme pour le Term, le Net aura un type externe ou interne.
	Un Net est unique au niveau d'une Cell.
	On va réutiliser les types internes et externe de Term.
	*/
	public:

		//Constructeurs : 

		Net(Cell*, const std::string&, Term::Type) ;
		~Net();

		//Accesseurs :

		Cell* 							getCell()				const ;
		const  std:: string& 			getName()	  			const;
		unsigned  int         			getId()  				const;
		Term::Type           			getType()  				const;
		Node*                 			getNode( size_t  id ) 	const;
		const  std::vector <Node*>&		getNodes()  			const;
		size_t                			getFreeNodeId() 		const; // renverra l'index de la premiere case libre dans le tableau nodes_, si aucune case n'est vide, elle renverra la taille du tableau, ie. l'index ap le dernier element
		const std::vector<Line*>& 		getLines () 			const; // renvoie les lignes d'un Net
		//Modificateurs :

		void                  			add( Node* );
		bool                  			remove( Node* );
		void							toXml(ostream&);
		static Net* 					fromXml(Cell*, xmlTextReaderPtr) ;
		static bool  					fromXml(Net*, xmlTextReaderPtr);
		void  							add ( Line* line ) ; // ajoute des lignes à lines_
		bool  							remove ( Line* line ) ; // retire des lignes à lines_
		
	private :
	
							Net ( const  Net& );//constructeur par copie que l'on met en privée pour être sur qu'il n'est pas utilisé.
	Cell*                  	owner_;
	std:: string           	name_;
	unsigned  int          	id_; // 
	Term::Type            	type_;
	std::vector <Node*>   	nodes_; // recense tous les nodes qui lui sont connectés.
	std::vector<Line*> 		lines_ ; // Permet de connaitre et gérer tous les Line qui vont lui être connecté
};

inline void  							Net::add ( Line* line ) 		  { if (line) lines_.push_back( line ); }
inline const std::vector<Line*>& 		Net::getLines () 			const { return lines_; } ;
inline Cell* 							Net::getCell() 				const {return owner_ ;}; 
inline const  std:: string& 			Net::getName() 				const {return name_ ; } ;
inline unsigned  int 					Net::getId() 				const {return id_ ; } ;
inline Term::Type 						Net::getType() 				const {return type_ ; } ;
inline 	const  std::vector <Node*>& 	Net::getNodes() 			const {return nodes_ ;} ; //on retourne le tableau de noeud du net

}//namspace Netlist