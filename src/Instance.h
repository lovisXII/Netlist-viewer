#pragma once
#include <iostream>
#include <string>
#include <vector>
#include  <cstdlib>
#include <iostream>
#include  <libxml/xmlreader.h>

#include "Point.h"


namespace Netlist {

	class Cell;
	class Term ;
	class Net;
	using namespace std;
class Instance
{
   	/*
	
	Classe qui va permettre de définir une instance, une instance est interne à une Cell.

	*/
	public:

		//Constructeur
		/*
		Const & Dest devront gérer l'ajout et le retrait de l'instance au niveau de la cell.
		Le const devra dupliquer la liste des Term de la Cell model qu'il instancie.
		A l'in le destr détruit ses Term
		*/

											Instance( Cell* owner, Cell* model, const std::string&) ;
											~Instance();

		//Accesseurs :


		const std::string& 					getName				()							const ;
		Cell* 								getMasterCell		() 							const ;
		Cell* 								getCell 			() 							const ; 
		const  std::vector <Term*>&			getTerms        	()  						const;
		Term*      							getTerm         	(const  std:: string&) 		const;
		Point      							getPosition    		()  						const;

		// Modificateurs :

		bool        						connect         	( const  std:: string& name , Net* ); // associe le Net au terminal de nom name s'il existe
		void        						add             	( Term* );
		void        						remove          	( Term* );
		void        						setPosition    		( const  Point& );
		void        						setPosition    		( int x, int y );
		void								toXml		   		(ostream&);
		static Instance* 					fromXml	(Cell*, xmlTextReaderPtr);




	private :	
		Cell* 					owner_ ; // Cell dans laquelle l'instance se trouve
		Cell* 					masterCell_ ; // De quoi c'est une instance
		std::string 			name_ ;
		std::vector<Term*> 		terms_ ; //liste de tous les terminal de l'instance
		Point 					position_ ;
		
};
inline 	const 		std::string& 						Instance::getName			() 			const {return name_ ; } ;
inline 				Cell* 								Instance::getMasterCell		()	 		const {return masterCell_ ; } ; 
inline 				Cell* 								Instance::getCell 			() 			const { return owner_ ; } ; 
inline 	const 	 	std::vector <Term*>&				Instance::getTerms 			() 			const {return terms_ ; } ; 
inline 				Point 								Instance::getPosition 		()			const {return position_ ; } ;


} //namespace Netlist

