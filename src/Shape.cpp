#include "Shape.h"
#include "Cell.h"
#include "Term.h"
#include "XmlUtil.h"
#include "Indentation.h"

namespace Netlist
{
		//-----------------------------------	Class Shape 	-------------------------------------------------------------

		Shape::Shape ( Symbol * owner ) : // constructeur
		owner_ ( owner )
		{ 
			owner -> add ( this ); // ajoute le Shape dans le vecteur shape_ de symbol
		}

		Shape::~Shape ()
		{ 
			owner_ -> remove ( this ); 
		}

		void Shape::toXml(ostream&)
		{

		}

		Shape* Shape::fromXml ( Symbol* owner, xmlTextReaderPtr reader )
		{
		// Factory-like method.
		  const xmlChar* boxTag
		    = xmlTextReaderConstString( reader, (const xmlChar*)"box" );
		  const xmlChar* ellipseTag
		    = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );
		  const xmlChar* arcTag
		    = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );
		  const xmlChar* lineTag
		    = xmlTextReaderConstString( reader, (const xmlChar*)"line" );
		  const xmlChar* termTag
		    = xmlTextReaderConstString( reader, (const xmlChar*)"term" );
		  const xmlChar* nodeName
		      = xmlTextReaderConstLocalName( reader );

		  Shape* shape = NULL;
		  if (boxTag == nodeName)
		    shape = BoxShape::fromXml( owner, reader );
		  if (ellipseTag == nodeName)
		    shape = EllipseShape::fromXml( owner, reader );
		  if (arcTag == nodeName)
		    shape = ArcShape::fromXml( owner, reader );
		  if (lineTag == nodeName)
		    shape = LineShape::fromXml( owner, reader );
		  if (termTag == nodeName)
		    shape = TermShape::fromXml( owner, reader );

		  if (shape == NULL)
		    cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:"
		         << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;

		  return shape;
		}
	//-----------------------------------	Class BoxShape	-------------------------------------------------------------

		BoxShape::BoxShape ( Symbol * owner , const Box & box ) // constructeur à partir d'une box
		: 
		Shape(owner),
		box_ ( box )
		{

		}

		BoxShape::BoxShape ( Symbol * owner , int x1 , int y1 , int x2 , int y2 ) // constructeur à partir de points
		: 
		Shape(owner),
		box_ ( x1 ,y1 ,x2 , y2 )
		{ 
			
		}


		BoxShape::~BoxShape ()
		{

		}

		Box BoxShape::getBoundingBox () const
		{ 
			return box_ ; 
		}

		void BoxShape::toXml(ostream& os)
		{
			os<<indent <<"<box x1=\"" << box_.getX1() <<"\" y1=\""<< box_.getY1() <<"\"" <<" x2=\"" << box_.getX2() <<"\" y2=\""<< box_.getY2() <<"\"/>" << endl ;
			
		}
		BoxShape* BoxShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
		{
			std::string x1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
			std::string x2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
			std::string y1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
			std::string y2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
			if( not(x1.empty()) or not(x2.empty()) or not(y1.empty()) or not(y2.empty()) )
			{
				BoxShape* boxshape = new BoxShape(owner, stoi(x1), stoi(y1),stoi(x2), stoi(y2) ) ;
				return boxshape ; 
			}
			else
			{
				cerr << "Impossible de charger les BoxShape" <<endl ;
				exit(0) ;
			}

		}
		

	//-----------------------------------	Class LineShape -------------------------------------------------------------
	
		LineShape::LineShape ( Symbol * owner , int x1 , int y1 , int x2 , int y2 )
		: 
		Shape( owner ),
		x1_ ( x1 ),
		y1_ ( y1 ),
		x2_ ( x2 ),
		y2_ ( y2 )
		{ 
			
		}

		LineShape::~LineShape()
		{

		}

		Box LineShape::getBoundingBox () const
		{ 
			return Box ( x1_ , y1_ , x2_ , y2_ ); 
		}

		void LineShape::toXml(ostream& os)
		{
			os<<indent <<"<line x1=\"" << this->getBoundingBox().getX1() <<"\" y1=\""<< this->getBoundingBox().getY1() <<"\"" <<" x2=\"" << this->getBoundingBox().getX2() <<"\" y2=\""<< this->getBoundingBox().getY2() <<"\"/>" << endl ;
			
		}

		LineShape* LineShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
		{
			std::string x1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
			std::string x2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
			std::string y1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
			std::string y2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
			if( not(x1.empty()) or not(x2.empty()) or not(y1.empty()) or not(y2.empty()) )
			{
				LineShape* lineshape = new LineShape(owner, stoi(x1), stoi(y1), stoi(x2), stoi(y2) ) ;
				return lineshape ; 
			}
			else
			{
				cerr << "Impossible de charger les LineShape" <<endl ;
				exit(0) ;
			}

		}
	//-----------------------------------	Class Ellipse  -------------------------------------------------------------
		
		EllipseShape::EllipseShape(Symbol* owner,const Box &box ) :
		Shape(owner) ,
		box_(box)
		{

		}

		EllipseShape::~EllipseShape()
		{

		}

		Box EllipseShape::getBoundingBox () const
		{
			return box_ ;
		}

		void EllipseShape::toXml(ostream& os)
		{
			os<<indent <<"<ellipse x1=\"" << this->getBoundingBox().getX1() <<"\" y1=\""<< this->getBoundingBox().getY1() <<"\"" <<" x2=\"" << this->getBoundingBox().getX2() <<"\" y2=\""<< this->getBoundingBox().getY2() <<"\"/>" << endl ;
			
		}

		EllipseShape* EllipseShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
		{
			std::string x1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
			std::string x2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
			std::string y1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
			std::string y2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
			if( not(x1.empty()) or not(x2.empty()) or not(y1.empty()) or not(y2.empty()) )
			{
				Box box(stoi(x1), stoi(y1), stoi(x2), stoi(y2)) ;
				EllipseShape* ellipseshape = new EllipseShape(owner, box ) ;
				return ellipseshape ; 
			}
			else
			{
				cerr << "Impossible de charger les EllipseShape" <<endl ;
				exit(0) ;
			}

		}

	//-----------------------------------	Class ArcShape -------------------------------------------------------------
		
		
		ArcShape::ArcShape(Symbol* owner, const Box& box,int start, int span) :
		Shape(owner),
		box_(box),
		start_(start),
		span_(span)
		{

		}
		ArcShape::~ArcShape() 
		{

		}
		Box ArcShape::getBoundingBox() const
		{
			return box_ ;
		}

		void ArcShape::toXml(ostream& os)
		{
			os<<indent <<"<arc x1=\"" << box_.getX1() <<"\" y1=\""<< box_.getY1() <<"\"" <<" x2=\"" << box_.getX2() <<"\" y2=\""<< box_.getY2() <<"\" start=\""<< this->getStart() <<"\" span=\""<< this->getSpan() <<"\"/>" << endl ;
			
		}

		int ArcShape::getStart() const
		{
			return start_ ;
		}

		int ArcShape::getSpan() const
		{
			return span_ ;
		}

		ArcShape* ArcShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
		{
			std::string x1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
			std::string x2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x2" ) );
			std::string y1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
			std::string y2 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y2" ) );
			std::string start = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"start" ) );
			std::string span = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"span" ) );

			if( not(x1.empty()) or not(x2.empty()) or not(y1.empty()) or not(y2.empty()) or not(start.empty()) or not(span.empty()) )
			{
				
				Box box(stoi(x1), stoi(y1), stoi(x2), stoi(y2)) ;
				ArcShape* arcshape = new ArcShape(owner, box, stoi(start), stoi(span) ) ;
				
				return arcshape ; 
			}
			else
			{
				cerr << "Impossible de charger les ArcShape" <<endl ;
				exit(0) ;
			}

		}


	//-----------------------------------	Class TermShape -------------------------------------------------------------

		TermShape::TermShape ( Symbol * owner , string name , int x1 , int y1 )
		: 
		Shape ( owner ) ,
		term_ ( NULL ),
		x1_ ( x1 ),
		y1_ ( y1 ) 
		{
				getSymbol() -> add(this) ;
				term_ = getSymbol() -> getCell() -> getTerm ( name );
		}

		TermShape ::~TermShape () 
		{

		}

		Box TermShape :: getBoundingBox () const
		{ 
			return Box ( x1_ , y1_ , x1_ , y1_ ); 
		}

		void TermShape::toXml(ostream& os)
		{
			os<<indent <<"<term name=\"" << this->getTerm()->getName() << "\" x1=\""<<x1_<<"\" y1=\""<<y1_<<"\" align=\""<< align_<<"\"/>" <<endl ;
			
		}

		TermShape* TermShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
		{
			std::string name = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"name" ) );
			std::string x1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"x1" ) );
			std::string y1 = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"y1" ) );
			std::string align = xmlCharToString( xmlTextReaderGetAttribute( reader, (const xmlChar*)"align" ) );

			if( not(name.empty()) or not(x1.empty()) or not(y1.empty()) or not(align.empty()) )
			{
				
				TermShape* termshape = new TermShape(owner, name, stoi(x1), stoi(y1) ) ;
				
				return termshape ; 
			}
			else
			{
				cerr << "Impossible de charger les TermShape" <<endl ;
				exit(0) ;
			}

		}



}

