#include "Box.h"

#include <iostream>
#include <string>
#include <vector>
#include  <cstdlib>
#include  <libxml/xmlreader.h>

#include "Term.h"

namespace Netlist
{
	using namespace std ;
	class Symbol ;
	class Shape 
	{
		public :

									Shape 				( Symbol * );
			virtual					~Shape 				() ; // on met le destructeur en virtuel pour pas avoir à l'appeler a chaque fois
			inline Symbol * 		getSymbol 			() const ;
			virtual Box 			getBoundingBox 		() const = 0 ; // méthode virtuelle pure
			virtual void			toXml				(ostream&);
			static Shape*			fromXml				(Symbol*, xmlTextReaderPtr);

			
		private :
			
			Symbol * owner_ ;


	};
	inline Symbol * Shape :: getSymbol () const { return owner_ ; }



	class BoxShape : public Shape 
	{
		public :
			
								BoxShape 		( Symbol *, const Box & );
								BoxShape 		( Symbol *, int x1 , int y1 , int x2 , int y2 );
								~BoxShape 		();
			Box 				getBoundingBox 	() const ;	
			void				toXml			(ostream&) ;
			static BoxShape*	fromXml			(Symbol*, xmlTextReaderPtr) ;
		private :
			Box box_ ;
	};

		
	class LineShape : public Shape 
	{
		public :
		
							LineShape 			( Symbol*, int x1 , int y1 , int x2 , int y2 );
							~LineShape 			() ;
			    inline int 	getX1 () const ;
			    inline int 	getY1 () const ;
			    inline int 	getX2 () const ;
			    inline int 	getY2 () const ;
		Box 				getBoundingBox 		() const ;
		void				toXml				(ostream&) ;
		static LineShape*	fromXml				(Symbol*, xmlTextReaderPtr) ;
		
		private :
			int x1_ , y1_ , x2_ , y2_ ;
	};

	inline int LineShape::getX1 () const { return x1_; }
    inline int LineShape::getY1 () const { return y1_; }
    inline int LineShape::getX2 () const { return x2_; }
    inline int LineShape::getY2 () const { return y2_; }

	class EllipseShape : public Shape 
	{
		
		public :

									EllipseShape 		( Symbol*, const Box &box );
									~EllipseShape 		();
			Box 					getBoundingBox 	() const ;
			void					toXml			(ostream&) ;
			static EllipseShape*	fromXml			(Symbol*, xmlTextReaderPtr) ;
		
		private :
			Box box_ ;
	};

	class ArcShape : public Shape
	{
		public :
								ArcShape			(Symbol* owner, const Box&, int, int ) ;
								~ArcShape			() ;
			Box 				getBoundingBox		() const ;
			int 				getStart 		 	() const ;
			int 				getSpan 		 	() const ; 
			void				toXml				(ostream&) ;
			static ArcShape*	fromXml				(Symbol*, xmlTextReaderPtr) ;
		private :
			Box box_ ;
			int start_ ;
			int span_ ;
	};

	class TermShape : public Shape 
	{
		public :
			enum NameAlign {Topleft = 1, TopRight, BottomLeft, BottomRight } ;
		public :

								TermShape 		( Symbol *, string name , int x , int y );
								~TermShape 		();
			Box 				getBoundingBox 	() const ;
			inline Term * 		getTerm 		() const ;
			inline int 			getX 			() const ;
			inline int 			getY  			() const ;
			inline NameAlign	getAlign		() const ;
			void				toXml			(ostream&) ;
			static TermShape*	fromXml			(Symbol*, xmlTextReaderPtr) ;
		
		private :
			Term * 		term_ ;
			int 		x1_ , y1_ ;
			NameAlign align_ ;
	};
	inline Term* 				TermShape::getTerm 	() const {return term_ ; } ;
	inline int 					TermShape::getX 	() const {return x1_ ;} ;
	inline int 					TermShape::getY  	() const {return y1_ ;} ;
	inline TermShape::NameAlign	TermShape::getAlign () const {return align_ ;} ;	


}

