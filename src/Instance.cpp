#include "Instance.h"

#include "Term.h"
#include "Cell.h"
#include "Indentation.h"
#include "XmlUtil.h"
#include "Shape.h"
#include "Point.h"

namespace Netlist
{


	Instance::Instance( Cell* owner, Cell* model, const std::string& name) 
		: 	owner_(owner) , 
			masterCell_(model) ,
			name_(name) ,
			position_() 
	{
		owner->add(this) ; // on ajoute l'instance créée à la cellule 
		
		std::vector<Term*> terms_copie = model->getTerms() ; // on copie les Terminaux de la cellule à l'instance
		//on avait pas besoin de faire ca 
		//En effet le terminal est automatiquement ajouté à terms_ via l'appel du constructeur de term
		for(auto it = terms_copie.begin() ; it != terms_copie.end() ; ++it)
		{
			new Term(this,(*it)) ;
			
		}
		/*Correction :
		for(Term* t:masterCell->getTerms())
		{
			new Term(this,t) ;
		}
		owner_->this ;
		*/
		
	}	
	Instance::~Instance()
	{
		owner_->remove(this) ;
		//masterCell_->remove(this) ; ici pas nécessaire car la masterCell ne connait pas ses instances
 		for(size_t i = 0 ; i < terms_.size() ; i++)
		{
			delete terms_[i] ;
		}

	}

	//Accesseurs :


	Term* Instance::getTerm ( const  std:: string& s) const
	{
		for(auto it = terms_.begin() ; it != terms_.end() ; ++it)
		{
			if(s == (*it)->getName())
			{
				return *it ;
			} 
		}
		cerr<<"Le terminal auquel vous essayer d'accéder n'existe pas" <<endl ;
		exit(1) ;
	}

	// Modificateurs :

	bool Instance::connect ( const  std:: string& name , Net* net)
	{
		Term* term = getTerm( name); // on regarde si le terminal existe dans l'instance
		if(term == NULL ) return false ; // s'il n'existe pas on renvoit faux
		term->setNet(net) ; // s'il existe on le connect sur le net
		return true ;
	}


	void Instance::add ( Term* terminal)
	{
		for(auto it = terms_.begin() ; it != terms_.end() ; ++it)
		{
			if(terminal->getName() == (*it)->getName())
			{
				cerr<<"Impossible d'ajouter ce terminal, le nom est déjà utilisé" << endl ;
				exit(1) ;
			}	
		}
		terms_.push_back(terminal) ; // si le terminal n'existe pas on l'ajoute a la liste des terminaux de l'instance
		
	}
	void Instance::remove ( Term* terminal)
	{
		//on va regarder si le terminal existe, si c'est le cas on le retire sinon on renvoit une erreur
		for(size_t i = 0 ; i < terms_.size() ; i ++)
		{
			if(terms_[i]->getName() == terminal->getName())
			{
				terms_[i] = NULL ;
				exit(0) ;
			}
		}
		cerr<<"Le terminal que vous tentez de retirer n'existe pas" << endl ;
		exit(1) ;
	}
	void Instance::setPosition ( const  Point& pos) // on veut modifier les coordonnées de l'instance
	 {
	 	//On récupère les coordonnées du point dont on veut translater l'instance

	 	int x1_instance, y1_instance ;
	 	
	 	x1_instance = pos.getX() ;
	 	y1_instance = pos.getY() ;



	 	std::vector<Term*> terms = this->getTerms() ; // on récupère le vecteur de Term de l'instance.
	 	//Le but va être de redefinir la position de chacun des Term de l'instance
	 	
	 	//Pour ce faire, on va parcourir les vecteurs term et shape dans le but de chercher les TermShape, des que l'on en rencontre un, on modifie les coordonnées du Term associé
	 	
	 	for(size_t i = 0 ; i < terms.size() ; ++i) //on parcours shape a la recherche des term shape
	 	{
			
			Shape *  shapes = this->getMasterCell()->getSymbol()->getTermShape(terms[i]) ; // on récupère le vecteur contenant tous les shape de notre instance
	 		if(static_cast<TermShape*> (shapes) != NULL) // on a un TermShape
	 		{
	 			TermShape* termshape = static_cast<TermShape*> (shapes) ;
	 			int x1, y1 ;

	 			//on récupère les coordonnées du TermShape
	 			
	 			x1 = termshape -> getX() ;
	 			y1 = termshape -> getY() ;
	 			
	 			//Maintenant qu'on a les coordonnées on va pouvoir changer celle du terminal

	 			terms[i] -> setPosition(x1 + x1_instance , y1 + y1_instance ) ; // on modifie la position des terminaux
	 			//la position d'un terminal est la position de sa TermShape translaté de la position de l'instance
	 		} 
	 	}
	 	position_ = pos ;
	 }
	void Instance::setPosition ( int x, int y )
	{


	 	std::vector<Term*> terms = this->getTerms() ; // on récupère le vecteur de Term de l'instance.
	 	//Le but va être de redefinir la position de chacun des Term de l'instance
	 	
	 	//Pour ce faire, on va parcourir les vecteurs term et shape dans le but de chercher les TermShape, des que l'on en rencontre un, on modifie les coordonnées du Term associé
	 	
	 	for(size_t i = 0 ; i < terms.size() ; ++i) //on parcours shape a la recherche des term shape
	 	{

	 		Shape * shape = this->getMasterCell()->getSymbol()->getTermShape(terms[i]) ; // on récupère le vecteur contenant tous les shape de notre instance
	 		if(static_cast<TermShape*> (shape) != NULL) // on a un TermShape
	 		{
	 			TermShape* termshape = static_cast<TermShape*> (shape) ;
	 			int x1, y1 ;

	 			//on récupère les coordonnées du TermShape
	 			
	 			x1 = termshape -> getX() ;
	 			y1 = termshape -> getY() ;
	 			
	 			//Maintenant qu'on a les coordonnées on va pouvoir changer celle du terminal

	 			terms[i] -> setPosition(x1 + x , y1 + y ) ; // on modifie la position des terminaux
	 			//la position d'un terminal est la position de sa TermShape translaté de la position de l'instance
	 		} 
	 	}
	 	position_ = Point(x,y) ;
	}



	void Instance::toXml(ostream& os)
	{
		extern Indentation indent ;
		os<<indent<<"<instance name=\""<<this->getName()<<"\" mastercell=\""<<this->getMasterCell()->getName()<<"\" "<<"x=\""<<this->getPosition().getX()<<"\" y=\""<<this->getPosition().getY()<<"\"/>\n" ;
	}
	Instance* Instance::fromXml	(Cell* cellule, xmlTextReaderPtr reader)
	{
		std::string name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
		std::string mastercell =xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"mastercell" ) );
		std::string x = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x" ) );
		std::string y = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y" ) );
		if (not (name.empty() or mastercell.empty()))
		{
			Instance* instance = new Instance(cellule,Cell::find(mastercell),name) ;
			instance->setPosition(stoi(x),stoi(y)) ;
			return instance ;
		}	
		else
		{
			cerr << "Impossible de créer correctement l'instance" ;
			return NULL ;
		}
	}
}

