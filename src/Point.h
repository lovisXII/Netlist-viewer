// -*- explicit-buffer-name: "Point.h<M1-MOBJ/4-5>" -*-

#ifndef NETLIST_POINT_H
#define NETLIST_POINT_H

#include <iostream>
#include <string>
#include <vector>
#include  <cstdlib>

namespace Netlist {


  class Point {
    public:
      //Constructeurs :
      inline         Point     ();
      inline         Point     ( int x, int y);

      //Accesseurs : 
      inline  int    getX      () const;
      inline  int    getY      () const;

      //Fonctions :

      inline  Point& setX      ( int );
      inline  Point& setY      ( int );
      inline  Point& translate ( int dx, int dy );
      inline  Point& translate ( const Point& );
      
    private:
      int  x_;
      int  y_;
  };

  inline         Point::Point     () : x_(0), y_(0) { } // Constructeur par défaut
  inline         Point::Point     ( int x, int y) : x_(x), y_(y) { } // Constructeur par affectation

  inline  int    Point::getX      () const { return x_; }
  inline  int    Point::getY      () const { return y_; }

  inline  Point& Point::setX      ( int x )  { x_ = x; return *this; } //Permet d'associer la valeur de x
  inline  Point& Point::setY      ( int y )  { y_ = y; return *this; } //Permet d'associer la valeur de x

  inline  Point& Point::translate ( int dx, int dy ) { x_+=dx; y_+=dy; return *this; } // retourne un point translaté de dx et dy
  inline  Point& Point::translate ( const Point& p ) { return translate( p.getX(), p.getY() ); } // retourne un point translaté d'un point, ex : A(1,2) et B(3,4) B.translate(A) = (3-1,4-2) = (2,3)

  inline  std::ostream& operator<< ( std::ostream& stream, const Point& p ) // surcharge de l'opérateur d'affichage
  { stream << "<Point " << p.getX() << " " << p.getY() << ">"; return stream; }


}  // Netlist namespace.

#endif  // NETLIST_POINT_H
