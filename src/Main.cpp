#include <QApplication>
#include <QtGui>
#include <iostream>
#include "CellViewer.h"

#include "Term.h"
#include "Net.h"
#include "Instance.h"
#include "Cell.h"
using namespace std;
using namespace Netlist;

int main (int argc , char* argv []) 
{
  cout << "Chargement des modeles..." << endl;
  Cell::load( "vdd" );
  cout << "Chargement des modeles..." << endl;
  Cell::load( "gnd" );
  cout << "Chargement des modeles..." << endl;
  Cell::load( "TransistorN" );
    cout << "Chargement des modeles..." << endl;
  Cell::load( "TransistorP" );
  cout << "Chargement des modeles..." << endl;
 Cell::load( "and2" );
  cout << "Chargement des modeles..." << endl;
  Cell::load( "or2" );
  cout << "Chargement des modeles..." << endl;
 Cell* xor2      = Cell::load( "xor2" );
 Cell* halfadder = Cell::load( "halfadder" );
  
  QApplication* qa = new QApplication(argc ,argv);
  CellViewer* viewer = new CellViewer ();
   viewer ->setCell( halfadder );

  viewer ->show ();
  int rvalue = qa ->exec ();
  
  delete viewer;
  delete qa;
  return rvalue;
}