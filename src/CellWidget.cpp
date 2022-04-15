// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include  <QResizeEvent>
#include  <QPainter>
#include  <QPen>
#include  <QBrush>
#include  <QFont>
#include  <QApplication>
#include  <QRect>
#include  <QLine>

#include  "CellWidget.h"
#include  "Term.h"
#include  "Instance.h"
#include  "Symbol.h"
#include  "Shape.h"
#include  "Cell.h"
#include  "Line.h"
#include  "Node.h"
#include  "Net.h"
#include  "Box.h"

namespace Netlist {

  using namespace std;


  ostream& operator<< ( ostream& o, const QRect& rect )
  {
    o << "<QRect x:" << rect.x()
      <<       " y:" << rect.y()
      <<       " w:" << rect.width()
      <<       " h:" << rect.height() << ">";
    return o;
  }


  ostream& operator<< ( ostream& o, const QPoint& p )
  { o << "<QRect x:" << p.x() << " y:" << p.y() << ">"; return o; }



  CellWidget :: CellWidget ( QWidget * parent )
  : QWidget ( parent )
  , cell_ ( NULL )
  , viewport_ ( Box (0 ,0 ,500 ,500))
  {
  setAttribute ( Qt :: WA_OpaquePaintEvent );
  setAttribute ( Qt :: WA_NoSystemBackground );
  setSizePolicy ( QSizePolicy :: Expanding // X direction .
  , QSizePolicy :: Expanding ); // Y direction .
  setFocusPolicy ( Qt :: StrongFocus );
  setMouseTracking ( true );
  }


  CellWidget::~CellWidget ()
  { 
  }


  void  CellWidget::setCell ( Cell* cell )
  {

    cell_ = cell;
    repaint(); // fonction de Qt qui permet de repeindre une cellule
  }


  QSize  CellWidget::minimumSizeHint () const
  { return QSize(500,500); }

  void CellWidget :: resizeEvent ( QResizeEvent * event ) 
  {
    const QSize & size = event -> size ();
    // Assume the resize is always done by drawing the bottom right corner .
    viewport_ . setX2 ( viewport_ . getX1 () + size . width () );
    viewport_ . setY1 ( viewport_ . getY2 () - size . height () );
    cerr << " CellWidget :: resizeEvent () ␣ viewport_ : " << viewport_ << endl ;
  }


  void CellWidget :: keyPressEvent ( QKeyEvent * event ) 
  {
    event -> ignore ();
    if ( event -> modifiers () & ( Qt :: ControlModifier | Qt :: ShiftModifier ))
    return ;
    switch ( event -> key ()) {
    case Qt :: Key_Up : goUp (); break ;
    case Qt :: Key_Down : goDown (); break ;
    case Qt :: Key_Left : goLeft (); break ;
    case Qt :: Key_Right : goRight (); break ;
    default : return ;
    }
    event -> accept ();
  }

  void CellWidget :: goRight () 
  {
  viewport_ . translate ( Point (20 ,0) );
  repaint ();
  }

  void CellWidget :: goUp () {
  viewport_ . translate ( Point (0 ,20) );
  repaint ();
  }

  void CellWidget :: goDown () // ATTENTION VALEUR AU PIF PSQ NIQUE
  {
    viewport_ . translate ( Point (0,-20) );
    repaint ();
  }

  void CellWidget :: goLeft () //  ATTENTION VALEUR AU PIF PSQ NIQUE
  {
    viewport_ . translate ( Point (-20,0) );
    repaint ();
  }


  void CellWidget :: paintEvent ( QPaintEvent * event ) 
  {
    QPainter painter ( this );
    painter . setBackground ( QBrush ( Qt :: white ) );
    painter . eraseRect ( QRect ( QPoint (0 ,0) , size () ) );
    painter . setPen ( QPen ( Qt :: darkGreen , 1 ) );
    QRect rect = boxToScreenRect ( Box(0,0,0,0) );
    painter . drawRect ( rect );
    painter . setPen ( QPen ( Qt :: red , 0 ) );
    painter . setBrush ( QBrush ( Qt :: red ) );
    
    if(cell_ == NULL)
    {
      return ;
    }
    else
    {
      std::vector<Instance*> instances  = cell_ -> getInstances() ;

      if(instances.size() == 0) // dans le cas ou il n'y a pas d'instance
      {
                auto shapes     = cell_ -> getSymbol() -> getShapes() ;
                for(auto it = shapes.begin() ; it != shapes.end() ; ++it) // on va parcourir toutes les shapes contenues dans la cellule ouverte
                {
                  
                  if(dynamic_cast<BoxShape*>(*it) != NULL) // on effectue des cast a chaque fois pour vérifier dans quel cas on est et tracer la bonne shape
                  //a chaque cast on vérifie bien que le changement de coordonnées est correct vis a vis de la fenetre
                  {
                    BoxShape* shape = dynamic_cast<BoxShape*>(*it) ;
                    QRect rect = boxToScreenRect((shape -> getBoundingBox()).translate(200,200)) ;
                    painter . setPen ( QPen ( Qt :: green , 0 ) );
                    painter . setBrush ( QBrush ( Qt :: green ) );
                    painter.drawRect(rect) ;
                    continue ; 
                  }

                  if(dynamic_cast<LineShape*>(*it) != NULL)
                  {
                    LineShape* shape = dynamic_cast<LineShape*>(*it) ;

                    Point p0 = Point(shape -> getX1() ,shape -> getY1() ) ;
                    Point p1 = Point(shape -> getX2(),shape -> getY2()) ;
                    
                    p0 = p0.translate(200,200) ;
                    p1 = p1.translate(200,200) ;
                    
                    QLine line = QLine(xToScreenX(p0.getX()),yToScreenY(p0.getY()),xToScreenX(p1.getX()),yToScreenY(p1.getY()));
                    painter.setPen ( QPen ( Qt :: darkGreen , 2 ) );
                    painter.drawLine(line) ;       
                    continue ; 
                  }

                  if(dynamic_cast<EllipseShape*>(*it) != NULL)
                  {
                    EllipseShape* shape = dynamic_cast<EllipseShape*>(*it) ;
                    QRect rect = boxToScreenRect((shape -> getBoundingBox()).translate(200,200)) ;
                    painter . setPen ( QPen ( Qt :: darkGreen, 0 ) );
                    painter.drawEllipse(rect) ;
                    continue ; 
                  }

                  if(dynamic_cast<ArcShape*>(*it) != NULL)
                  {
                    ArcShape* shape = dynamic_cast<ArcShape*>(*it) ;
                    QRect rect = boxToScreenRect((shape -> getBoundingBox()).translate(200,200)) ;
                    painter.setPen ( QPen ( Qt :: darkGreen , 3 ) );        
                    painter.drawArc(rect, (shape ->getStart())*16 , (shape -> getSpan())*16) ;
                    continue ; 
                  }

                  if(dynamic_cast<TermShape*>(*it) != NULL)
                  {
                    TermShape* shape = dynamic_cast<TermShape*>(*it) ;
                    QRect rect = boxToScreenRect((shape->getBoundingBox().translate(200,200).inflate(4)  ));
                    painter . setPen ( QPen ( Qt :: red , 0 ) );
                    painter.drawRect(rect) ;
                    continue ; 
                  }
              }

      }
      else // cas où il y a des instances
      {

          /*----------------------------------------------------- AFFICHAGE DES SHAPES -------------------------------------------------------------------------------*/        
          for(size_t i = 0 ; i < instances.size() ; ++ i) // on va parcourir tous les instances de la cell
          {
            Point instPos   = instances[i] -> getPosition ();
            auto shapes     = instances[i] -> getMasterCell() -> getSymbol() -> getShapes() ;
            for(auto it = shapes.begin() ; it != shapes.end() ; ++it) // on va parcourir toutes les shapes contenues dans la cellule ouverte
            {
              
              if(dynamic_cast<BoxShape*>(*it) != NULL) // on effectue des cast a chaque fois pour vérifier dans quel cas on est et tracer la bonne shape
              {
                BoxShape* shape = dynamic_cast<BoxShape*>(*it) ;
                QRect rect = boxToScreenRect((shape -> getBoundingBox()).translate(instPos)) ;
                painter . setPen ( QPen ( Qt :: green , 0 ) );
                painter.drawRect(rect) ;
                continue ; 
              }

              if(dynamic_cast<LineShape*>(*it) != NULL)
              {
                LineShape* shape = dynamic_cast<LineShape*>(*it) ;

                Point p0 = Point(shape -> getX1() ,shape -> getY1() ) ;
                Point p1 = Point(shape -> getX2(),shape -> getY2()) ;
                
                p0 = p0.translate(instPos) ;
                p1 = p1.translate(instPos) ;
                
                QLine line = QLine(xToScreenX(p0.getX()),yToScreenY(p0.getY()),xToScreenX(p1.getX()),yToScreenY(p1.getY()));
                painter.setPen ( QPen ( Qt :: darkGreen , 2 ) );
                painter.drawLine(line) ;       
                continue ; 
              }

              if(dynamic_cast<EllipseShape*>(*it) != NULL)
              {
                EllipseShape* shape = dynamic_cast<EllipseShape*>(*it) ;
                QRect rect = boxToScreenRect((shape -> getBoundingBox()).translate(instPos)) ;
                painter . setPen ( QPen ( Qt :: darkGreen, 1 ) );
                painter.drawEllipse(rect) ;
                continue ; 
              }

              if(dynamic_cast<ArcShape*>(*it) != NULL)
              {
                ArcShape* shape = dynamic_cast<ArcShape*>(*it) ;
                QRect rect = boxToScreenRect((shape -> getBoundingBox()).translate(instPos)) ;
                painter.setPen ( QPen ( Qt :: darkGreen , 3 ) );        
                painter.drawArc(rect, (shape ->getStart())*16 , (shape -> getSpan())*16) ;
                continue ; 
              }

              if(dynamic_cast<TermShape*>(*it) != NULL)
              {
                TermShape* shape = dynamic_cast<TermShape*>(*it) ;
                QRect rect = boxToScreenRect((shape->getBoundingBox().translate(instPos).inflate(4)  ));
                painter . setPen ( QPen ( Qt :: red , 0 ) );
                painter.drawRect(rect) ;
                continue ; 
              }
            }

          }


          /*----------------------------------------------------- AFFICHAGE DES TERMS -------------------------------------------------------------------------------*/ 
          auto terms = cell_ -> getTerms() ; 
          
          for(auto it = terms.begin() ; it != terms.end() ; ++it)
          {
            //QRect rect = boxToScreenRect(Box((*it) -> getPosition()).inflate(3)) ;
            QRect rect = boxToScreenRect(Box((*it)->getPosition().getX(),(*it)->getPosition().getY(), (*it)->getPosition().getX(), (*it)->getPosition().getY()).inflate(4)) ;
            painter.setPen ( QPen ( Qt :: magenta , 2 ) );
            painter.drawRect(rect) ;                
          }


          /*----------------------------------------------------- AFFICHAGE DES LINES -------------------------------------------------------------------------------*/ 
          

          std::vector<Net*> nets = cell_ -> getNets() ; // ON PARCOURT TOUS LES NETS DE LA CELL POUR RECUPERER LES LINES A TRACER

          for(auto it = nets.begin() ; it != nets.end() ; ++it) // On AFFICHE LES line
          {

            auto it_lines = (*it)   -> getLines() ; // On récupère toutes les lines d'un net
            auto it_nodes = (*it)   -> getNodes() ;

            for( auto it_line = it_lines.begin() ; it_line != it_lines.end() ; ++it_line ) // on va parcourir le lines pour récupérer tous les line
            {
                 // On récupère les position de chacun des line
                 Point position_source = (*it_line) -> getSourcePosition() ; // de type line* -> Point 
                 Point position_target = (*it_line) -> getTargetPosition() ; 
                                  
                 QLine line1 = QLine(xToScreenX(position_source.getX()),yToScreenY(position_source.getY()),xToScreenX(position_target.getX()),yToScreenY(position_target.getY()));
                 painter.setPen(QPen(Qt :: cyan , 2)) ;
                 painter.drawLine(line1) ;             
            }
            for(auto it_node = it_nodes.begin() ; it_node != it_nodes.end() ; ++it_node) // on va récupéré des Node* qu'on va devoir caster en Node Point
            {
              NodePoint* node_point = dynamic_cast<NodePoint*>(*it_node) ; // on caste tous les Node* en NodePoint*
              if(node_point)
              {
                int compteur = 0 ;
                auto lines = (*it) -> getLines() ; // on récupère les lignes du Net
                for(auto it_line = lines.begin() ; it_line != lines.end() ; ++ it_line)
                {
                    if(node_point == dynamic_cast<NodePoint*>((*it_line) -> getSource()) or node_point == dynamic_cast<NodePoint*>((*it_line) -> getTarget()))
                    {
                      compteur++ ;
                    }
                    if(compteur > 2)
                    {
                      QPoint point = QPoint(xToScreenX(node_point -> getPosition().getX()) , yToScreenY(node_point -> getPosition().getY())) ;
                      painter.setPen(QPen(Qt :: cyan , 2)) ;
                      painter.drawEllipse(point, 3 ,3 ) ;
                      break ;
                    }
                }
              }
              else
              {

              }
            } 
           
          }


      }




      
  
    }

    painter . drawRect ( rect );
      
  }

}  // Netlist namespace.
