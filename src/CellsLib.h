#pragma once
#include <QTableView>
#include <QPushButton>
#include <iostream>

namespace Netlist
{
  class CellsModel ;
  class CellViewer ;
  class Cell ;

  using namespace std ;

  class CellsLib : public QWidget // va contenir la liste des cells chargées en mémoire
  // On charge en appelant OpenCellDialog
  {
      Q_OBJECT;

    public:
                          CellsLib       ( QWidget* parent=NULL );
    void                  setCellViewer  ( CellViewer* ); // permet de setup le viewer
    int                   getSelectedRow () const; // permet de selectionner la ligne
    inline CellsModel*    getBaseModel   (); // renvoie le modèle selectionné
    public slots:
             void        load           (); // permet de charger la cellule selectionnée
    private:

      CellViewer*  cellViewer_;
      CellsModel*  baseModel_;
      QTableView*  view_;
      QPushButton* load_;
  };

  inline CellsModel* CellsLib::getBaseModel(){return baseModel_ ;} ;


}

