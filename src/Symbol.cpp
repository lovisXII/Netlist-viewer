#include "Symbol.h"
#include "Box.h"
#include "Point.h"
#include "Term.h"
#include "Shape.h"
#include "Indentation.h"
#include "Cell.h"

#include  <libxml/xmlreader.h>

namespace Netlist
{
	using namespace std ;

	Symbol::Symbol(Cell* cell) :
	owner_(cell) ,
	shapes_(NULL)
	{

	}
	Symbol::~Symbol()
	{
		
	}
	Box Symbol::getBoundingBox  () const
	{	

	}
	Point Symbol::getTermPosition ( Term* term) const
	{
		Point point = term -> getPosition() ; 
		return point ;
	}
	
	TermShape* Symbol::getTermShape ( Term* term) const // on va parcourir le vecteur de shape à la recherche du term
	{
		for(auto it = shapes_.begin() ; it != shapes_.end() ; ++it) // it est de type Shape
		{
			TermShape* termshape = dynamic_cast<TermShape*> (*it) ;
			if(termshape != nullptr)
			{
				if(termshape->getTerm() -> getName() == term -> getName())
				{
					return termshape ;
				}	
			}

		}
		return nullptr ;
	}

	void Symbol::add ( Shape* shape)
	{
		shapes_.push_back(shape) ;
	}

	void Symbol::remove ( Shape* shape)// on va retirer shape de shapes_
	{
		int compteur = 0 ;
		for(auto it = shapes_.begin() ; it != shapes_.end() ; ++it) // on parcours tous les vecteur de Shape
		{
			if((*it) == shape) // des que l'on rencontre shape que l'on veut effacer, on le retire du vecteur
			{
				shapes_[compteur] = NULL ;
			}
			compteur++ ;	
		}
		
	}
	void Symbol::toXml ( std::ostream& os ) const
	{
		os << indent << "<symbol>"<<endl ;
		indent++;
		
		for(size_t i = 0 ; i < shapes_.size() ; ++i)
		{
			shapes_[i]->toXml(os) ;
		}
		indent--;
		os << indent << "</symbol>" << endl ;
	}
	Symbol* Symbol::fromXml ( Cell* cell, xmlTextReaderPtr reader )
	{
        const xmlChar* symbolTag = xmlTextReaderConstString( reader, (const xmlChar*)"symbol" );
        Symbol* symbol = cell->getSymbol();

        while(true)
        {
            xmlTextReaderRead(reader);

            switch ( xmlTextReaderNodeType(reader) ) 
            {
                case XML_READER_TYPE_COMMENT:
                
                case XML_READER_TYPE_WHITESPACE:
                
                case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                
                continue;

            }
            
            const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );
            
            if ((nodeName==symbolTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT))
            {
                break ;
            }
            
            else
            {
                Shape::fromXml(symbol,reader) ;
            }


        }
        return symbol;
    }

	Symbol& Symbol::operator= ( const Symbol& symbol)
	{
		//Cell* cell = symbol->getCell() ;

	}

}

