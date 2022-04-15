#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>

#include "Term.h"
#include "Cell.h"
#include "Instance.h"
#include "Net.h"
#include "Indentation.h"
#include "XmlUtil.h"

namespace Netlist {


	//Constructeurs :

	Term::Term(Cell* cellule, const std::string& name, Direction direction)// Constructeur pour une Cell
		:	owner_(cellule) ,
			name_(name) ,
			direction_ (direction) ,
			type_ (External) ,
			net_(NULL), // pas connecté au début
			node_ (this)//Quand on initialise un pointeur d'un objet dans une classe il faut l'allouer dynamiquement comme ca
		 
	{
		cellule->add(this) ; // on ajoute le terminal à la cellule
	}

	Term::Term( Instance* instance, const Term* modelTerm) // Constructeur pour une instance
		//modelTerm est le terminal d'une cellule à laquelle notre instance est ratachée
		//Dans ce cas on veut dupliquer le modele de modelTer qui est le terminal d'une cellule
		:	owner_(instance) ,
			name_(modelTerm->getName()) ,
			direction_ (modelTerm->getDirection()) ,
			type_ (Internal) ,
			net_(NULL), 
			node_ (this)//on envoie notre objet au node_
			
	{ 
		instance->add(this) ; // on ajoute le terminal à l'instance
	}

	Term::~Term()
	//le destructeur doit retirer le terminal de son propriétaire
	{
		//on déconnecte le terminal de son owner

		if(this->getType() == Internal)
		{
			static_cast<Instance*> (owner_)->remove(this) ;
		}
		else
		{
			static_cast<Cell*> (owner_)->remove(this) ;
		}

		//on déconnecte le terminal de son net

		(this->getNet())->remove(this->getNode()) ;

		//on supprime son noeud
		
		delete &node_ ;
	}


 	Cell* Term::getOwnerCell  ()  const 	
 	{

 		if(type_ == Internal)
 		{
			return static_cast<Instance*> (owner_)->getCell() ;
 		} 		
 		else 
 		{
 			return static_cast<Cell*> (owner_) ;
 		}
 	} 

 	
	Point     Term::getPosition   ()  const
	{
		return node_.getPosition() ;
	}

	void Term::setNet ( Net* net)
	{
		net_ = net ;
		//net->add(this->getNode()) ;

		//correction :
		//net_ = net ;
		if(net)
		{
			net_->add(&node_) ;
		}

	}
	
	void Term::setNet ( const std::string& name )
	{
		//correction : 
		//setNet(getOwnerCell()->getNet(name))
		if(type_ == Internal)
		{
			std::vector<Net*> nets_Cell = this->getInstance()->getCell()->getNets() ; //on recupère les nets de la cell 
			int trouve = 0 ;
			//On parcourt tous les nets de la cellule propriéraire 
			for(size_t i = 0 ; i < nets_Cell.size() ; i++)
			{
				if(nets_Cell[i]->getName() == name)//si le noeud existe on l'affecte directement au terminal
				{
					net_ = nets_Cell[i] ;
					trouve = 1 ;
				}
			}
			if (trouve==0)//si on ne l'a pas trouvé, on le créer.
			{
				net_ = new Net(this->getInstance()->getCell(),name,Internal) ;	
			}
			
					
		}
		else
		{
			
			std::vector<Net*> nets_Cell = this->getCell()->getNets() ; //on recupère les nets de la cell 
			int trouve = 0 ;
			//On parcourt tous les nets de la cellule propriéraire 
			for(size_t i = 0 ; i < nets_Cell.size() ; i++)
			{
				if(nets_Cell[i]->getName() == name)//si le noeud existe on l'affecte directement au terminal
				{
					net_ = nets_Cell[i] ;
					trouve = 1 ;
				}
			}
			if (trouve==0)//si on ne l'a pas trouvé, on le créer.
			{
				net_ = new Net(this->getInstance()->getCell(),name,Internal) ;	
			}
		}
	}

	inline void Term::setDirection ( Direction direction)
	{
		direction_ = direction ;
	}
	void Term::setPosition  ( const Point& pos)
	{
		node_ .setPosition(pos) ;
	}
	void Term::setPosition  ( int x, int y )
	{
		node_ .setPosition(x,y) ;
	}

	std::string  Term::toString    ( Type type)
	{
		if(type == Internal)
		{
			return "Internal" ;
		}
		else if(type == External)
		{
			return "External" ;
		}
		else{return "Le type choisit n'est pas défini" ;}	
	}
	std::string  Term::toString    ( Direction direction )
	{
		
			if (direction == In) 
			{
				return "In" ;
			}
			else if (direction == Out)
			{
				return "Out" ;
			}
			else if (direction == Inout)
			{
				return "Inout" ;
			}
			else if (direction == Tristate)
			{
				return "Tristate" ;
			}
			else if (direction == Transcv)
			{
				return "Transcv" ;
			} 				
			else if (direction == Unknow)
			{
				return "Unknow" ;
			} 				
			else
			{
				return "Erreur le type de direction choisi n'est pas défini" ;
			}
		
	}
	Term::Direction    Term::toDirection ( std::string s)
	{
	
			if (s == "In") 
			{
				return In ;
			}
			else if (s == "Out")
			{
				return Out ;
			}
			else if (s == "Inout")
			{
				return Inout ;
			}
			else if (s == "Tristate")
			{
				return Tristate ;
			}
			else if (s == "Transcv")
			{
				return Transcv ;
			} 				
			else if (s == "Unknow")
			{
				return Unknow ;
			} 				
			else
			{
				cerr<<"Erreur le type de direction choisi n'est pas défini" ;
				exit(1);
			}
	}
	Term::Type Term::toType(std::string s) 
	{
		if( s == "Internal")
		{
			return Internal ;
		}
		else if( s == "External")
		{
			return External ;
		}
		else
		{
			cerr << "Le type saisi est incorrect" ;
			exit(0) ;
		}
	}

	void Term::toXml(ostream& os)
	{
		extern Indentation indent ;
		os<<indent<<"<term name=\""<<this->getName()<<"\" direction=\""<<toString(this->getDirection())<<"\" "<<"x=\""<<this->getPosition().getX()<<"\" y=\""<<this->getPosition().getY()<<"\"/>\n" ;
	}


	Term* Term::fromXml(Cell* Cell, xmlTextReaderPtr reader)
	{
			std::string 			term_name 			= xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
			std::string 			direction_string 	= xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"direction" ) );
			std::string 			x_string 			= xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x" ) );
			std::string 			y_string 			= xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y" ) );
			if(not (term_name.empty() or direction_string.empty()) )
			{
					Direction direction = toDirection(direction_string) ;
					Term* term = new Term(Cell, term_name, direction) ;
					term->setPosition(stoi(x_string),stoi(y_string)) ;
					return term ;
			}
			else 
			{
				cerr << "Impossible de charger les terminaux" << endl; 
				return NULL;
			}

	}

}
