// -*- explicit-buffer-name: "Node.h<M1-MOBJ/4-5>" -*-

#ifndef NETLIST_NODE_H
#define NETLIST_NODE_H

#include <iostream>
#include <string>
#include <vector>
#include  <cstdlib>
#include  <libxml/xmlreader.h>

#include "Indentation.h"
#include "Point.h"

namespace Netlist {

  class Term;
  class Net;
  class Line;


  class Node { 
    public:
      static  const size_t              noid;
    public:
                                        Node          ( size_t id );
      virtual                          ~Node          ();
      inline  Point                     getPosition   () const;
      inline  const std::vector<Line*>& getLines      () const;
      inline  size_t                    getDegree     () const;
      inline  size_t                    getId         () const;
      virtual Net*                      getNet        () const = 0;
      inline  void                      setId         ( size_t );
      inline  void                      setPosition   ( const Point& );
      inline  void                      setPosition   ( int x, int y );
              void                      attach        ( Line* );
              void                      detach        ( Line* );
      virtual void                      toXml         ( std::ostream& ) const= 0;
      static  bool                      fromXml       ( Net*, xmlTextReaderPtr );
    private:
                                        Node          ( const Node& );
              Node&                     operator=     ( const Node& );
    protected:
      size_t              id_;
      Point               position_;
      std::vector<Line*>  lines_;
  };


  inline const std::vector<Line*>& Node::getLines    () const { return lines_; }
  inline size_t                    Node::getDegree   () const { return lines_.size(); }
  inline size_t                    Node::getId       () const { return id_; }
  inline Point                     Node::getPosition () const { return position_; }
  inline void                      Node::setId       ( size_t id ) { id_=id; }
  inline void                      Node::setPosition ( const Point& pos ) { position_ = pos; }
  inline void                      Node::setPosition ( int x, int y )     { position_ = Point(x,y); }



  class NodePoint : public Node // classe fille qui hérite de Node 
  //Permet de créer des points purement géométrique permettant d'ancrer les lines
  {
    public:
                         NodePoint   ( Net*, size_t id=Node::noid, Point p=Point() );
      virtual           ~NodePoint   ();
      virtual Net*       getNet      () const;
      virtual void       toXml       ( std::ostream& ) const;
    private:
                         NodePoint   ( const NodePoint& );
              NodePoint& operator=   ( const NodePoint& );
    private:
      Net* net_;
  };


  class NodeTerm : public Node // classe fille héritant de Node 
  {
    public:
                         NodeTerm    ( Term*, size_t id=Node::noid );
      virtual           ~NodeTerm    ();
      inline   Term*     getTerm     () const;
      virtual  Net*      getNet      () const;
      virtual  void      toXml       ( std::ostream& ) const;
    private:
                         NodeTerm    ( const NodeTerm& );
               NodeTerm& operator=   ( const NodeTerm& );
    private:
      Term* term_;
  };


  inline Term* NodeTerm::getTerm () const { return term_; }


}  // Netlist namespace.

#endif  // NETLIST_NODE_H
