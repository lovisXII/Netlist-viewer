#pragma once 

#include <QMainWindow>


//Gestion de la fenetre principale

namespace Netlist
{
  class Cell ;
  class CellsLib ;
  class CellWidget ;
  class InstancesWidget ;
  class SaveCellDialog ;


  class CellViewer : public QMainWindow 
  {
      Q_OBJECT;
    public:
                                CellViewer          ( QWidget* parent=NULL );
      virtual                   ~CellViewer         ();
              Cell*             getCell             () const;
              CellWidget*       getCellWidget       ();
      inline  CellsLib*         getCellsLib         (); 
       
    public slots:
              void      setCell             ( Cell* );
              void      saveCell            (); // sauvergarde la cellule avec le nom definit dans SaveCellDialog::run
              void      openCell            ();

              void      showCellsLib        ();  // TME9+.
              void      showInstancesWidget ();  // TME9+.
    signals :

            void    cellLoaded() ; // implémentation gérée par moc, permet d'avertir CellsLib qu'une cellule a été chargée 
    private:
      CellWidget*      cellWidget_; 
      CellsLib*        cellsLib_;         
      InstancesWidget* instancesWidget_;  
      SaveCellDialog*  saveCellDialog_;
  };

  inline CellsLib* CellViewer::getCellsLib(){return cellsLib_ ;} ;
}

