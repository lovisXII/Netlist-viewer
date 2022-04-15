#pragma once
#include <QTableView>
#include <QPushButton>
#include <iostream>
#include "InstancesModel.h"

namespace Netlist
{

  using namespace std;
  class Cell ;
  class CellViewer ;


  class InstancesWidget : public QWidget 
  {
      Q_OBJECT;
    public:
                    InstancesWidget ( QWidget* parent=NULL );
                    InstancesWidget ( QWidget* parent, CellViewer* viewer );
              void  setCellViewer   ( CellViewer* );
              int   getSelectedRow  () const;
      inline  void  setCell         ( Cell* );

    public slots:

              void  load            ();
    
    private:
              CellViewer*     cellViewer_;
              InstancesModel* baseModel_;
              QTableView*     view_; 
              QPushButton*    load_;
  };
  inline void InstancesWidget::setCell(Cell* cellule) {return baseModel_ -> setCell(cellule) ; };  
}
