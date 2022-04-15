#pragma once
#include <iostream>
#include <string>
#include <vector>
#include  <cstdlib>
#include  <libxml/xmlreader.h>

#include "Point.h"
#include "Node.h"

namespace Netlist {

	class Cell;
	class Instance;
	class Net;

	using namespace std ;


	class Term
	{

		/*
		Cette classe définit un terminal.
		Ce dernier peut appartenir à une Cell ou une instance.
		Il a donc deux constructeurs, un pour chaque type.

		Si le Term appartient à :
		 - Une Cell, son type sera : EXTERNAL 
		 - Une Instance, son type sera : INTERNAL
		*/

		
		public:

		/*
		enum définit un énumérateur, c'est un sous type entier.
		Ici on créer les types :
		- Type 
		- Direction
		Ces sous types entier ne peuvent valoir que 1,2 dans le cas de Type et 1,2,3,4,5,6 dans le cas de Direction.
		*/

		enum Type {Internal = 1 , External = 2} ;
		enum Direction {In = 1, Out = 2, Inout = 3, Tristate = 4 , Transcv = 5, Unknow = 6} ;

		public:

			//Constructeurs :

								  Term(Cell*, const std::string& name, Direction) ; // Constructeur pour une Cell
								  Term( Instance*, const Term* modelTerm) ; // Constructeur pour une instance
								  ~Term();

			bool 				  isInternal () 	const ;
			bool 				  isExternal () 	const ;

			//Accesseurs :

			const std::string&    getName() 		const;
			NodeTerm*              getNode()				 ; //renvoie un pointeur sur l'attribut node_
			Net*                  getNet()  		const;

			Cell*                 getCell() 		const; // getCell doit renvoyer la Cell propriétaire dans le cas d'un Term Externe, sinon renvoie NULL
			Cell*                 getOwnerCell  ()  const; // getOwnerCell doit renvoyer la Cell propriétaire de l'instance propriétaire	
			Instance*             getInstance   ()  const;
			/*
				getInstance doit renvoyer l'Instance propriétaire dans le cas d'un Term Interne, sinon renvoie NULL
			*/
			Direction             getDirection  ()  const;
			Point                 getPosition   ()  const;
			Type                  getType() 		const;
			//Modificateurs :

			void  				setNet       ( Net* );
	       	void  				setNet       ( const std::string& );
			inline void  		setDirection ( Direction );
	       	void  				setPosition  ( const Point& );
	       	void  				setPosition  ( int x, int y );
	       	void				toXml		 (ostream&) ;
			static Term* 		fromXml		 (Cell*, xmlTextReaderPtr reader) ;

			//Fonctions de conversion :
			//Ces fonctions vont nous permettre de convertir :
			/*
				- un type vers un string
				- une direction vers un string
				- un string vers une direction 
			*/
			static std::string  toString    ( Type );
			static std::string  toString    ( Direction );
			static Direction    toDirection ( std::string s);
			static Type 		toType(std::string) ;


		private :

			void* owner_ ; // type void car ca peut être une instance* ou une cell*, void* peut prendre n'importe quel type de pointeur
			std::string name_ ;	
			Direction direction_ ;
			Type type_ ;
			Net* net_ ; //pointeur vers le net auquel il est connecté
			NodeTerm node_ ; // pointeur vers le noeud auquel il est connecté


	}; //namespace Netlist
	

	inline Cell* 					Term:: getCell () 		const 	{return (type_  ==  External) ? static_cast <Cell*>(owner_): NULL; } ;
	inline Instance* 				Term:: getInstance  ()  const 	{return (type_  ==  Internal) ? static_cast <Instance*>(owner_): NULL;} ;

	inline bool 					Term::isInternal () 	const 	{return (type_ == Internal) ? true : false ; } ;
	inline bool 					Term::isExternal () 	const 	{return (type_ == External) ? true : false ; };

	inline const std::string&    	Term::getName() 		const 	{return name_  ; } ;
	inline NodeTerm*     			Term::getNode() 				{return &node_ ;} ;
	inline Net* 					Term::getNet() 			const 	{return net_ ;};

	inline Term::Direction 			Term::getDirection  ()  const 	{return direction_ ; } ;
	inline Term::Type 				Term::getType() 		const	{return type_ ;}

	

}
