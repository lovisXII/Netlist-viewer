// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
class QPainter;
#include "Box.h"


namespace Netlist {

  class Cell;
  class NodeTerm;


class CellWidget : public QWidget {
    Q_OBJECT;

  public:
                    CellWidget         ( QWidget* parent=NULL );
    virtual        ~CellWidget         ();
            void    setCell            ( Cell* );
    inline  Cell*   getCell            () const;

    inline  int     xToScreenX         ( int x ) const;
    inline  int     yToScreenY         ( int y ) const;
    
    inline  QRect   boxToScreenRect    ( const Box& ) const; // converti une box en QRect
    inline  QPoint  pointToScreenPoint ( const Point& ) const; // converti un point en QPoint
    
    inline  int     screenXToX         ( int x ) const;
    inline  int     screenYToY         ( int y ) const;
    
    inline  Box     screenRectToBox    ( const QRect& ) const; // converti un QRect en Box
    inline  Point   screenPointToPoint ( const QPoint& ) const; // converti un QPoint en point

    virtual QSize   minimumSizeHint    () const;
    virtual void    resizeEvent        ( QResizeEvent* ); // redimensionnement du CellWidget

  protected:

    virtual void    paintEvent         ( QPaintEvent* );
    virtual void    keyPressEvent      ( QKeyEvent* );

  public slots:

            void    goLeft             ();
            void    goRight            ();
            void    goUp               ();
            void    goDown             ();
  private:
    Cell* cell_;
    Box   viewport_; // représente la zone actuellement affiché par le visualisateur dans le référentiel Netlist
};



  inline Cell*    CellWidget::getCell              () const { return cell_; }
  inline int      CellWidget::xToScreenX          (int x) const {return x - viewport_.getX1() ;} ;
  inline int      CellWidget::yToScreenY          (int y) const {return viewport_.getY2() - y ;} ;
  inline int      CellWidget::screenXToX          (int x) const {return x + viewport_.getX1() ;} ;
  inline int      CellWidget::screenYToY          (int y) const {return viewport_.getY2() + y  ;} ;
     
  inline  QRect   CellWidget::boxToScreenRect    ( const Box&    box   )    const  
  {
    return QRect  (xToScreenX(box.getX1()) , yToScreenY(box.getY1()) , xToScreenX(box.getX2())-xToScreenX(box.getX1()) , yToScreenY(box.getY2())-yToScreenY(box.getY1())).normalized() ; 
  };

  inline  QPoint  CellWidget::pointToScreenPoint ( const Point&  point )    const  
  {
    return QPoint (xToScreenX(point.getX()), yToScreenY(point.getY())) ;
  };

  inline  Box     CellWidget::screenRectToBox    ( const QRect&  rect  )    const  
  {
    return Box(screenXToX( rect.x()) , screenYToY(rect.y()) ,screenXToX(rect.x() + rect.width()) , screenYToY(rect.y() + rect.height()) ) ;
  };

  inline  Point   CellWidget::screenPointToPoint ( const QPoint& point )    const  
  {
    return Point( screenXToX(point.x()) , screenYToY(point.y()) ) ;
  };
}  // Netlist namespace.

#endif  // NETLIST_CELL_WIDGET_H
