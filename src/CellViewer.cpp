#include "CellViewer.h"
#include "Cell.h"
#include "CellsLib.h"
#include "CellsModel.h"
#include "CellWidget.h"
#include "InstancesWidget.h"
#include "SaveCellDialog.h"
#include "OpenCellDialog.h"

#include <QMenu> 	// Permet de créer un menu de selection
#include <QMenuBar> //Horizontal menu bar
#include <QAction> 	// interface utilisateur qui peut etre inséré dans un widget
#include <QDialog>
#include <QMessageBox>

namespace Netlist
{

	CellViewer :: CellViewer ( QWidget* parent )
	: QMainWindow (parent) // on créer la fenetre mere 
	, cellWidget_ (NULL)
	, cellsLib_(NULL)
	, instancesWidget_(NULL)
	, saveCellDialog_(NULL)
	{
	cellWidget_                           = new CellWidget (); // on créer un cellWidget
	saveCellDialog_                       = new SaveCellDialog( cellWidget_ );
	instancesWidget_					  = new InstancesWidget(parent) ;
	cellsLib_ 							  = new CellsLib(parent) ;
	setCentralWidget( cellWidget_ );

	instancesWidget_ -> setCellViewer(this) ;
	cellsLib_ -> setCellViewer(this) ;
	QMenu* fileMenu                       = menuBar()-> addMenu( "&File" );//on créer le menu file
	QAction* action                       = new QAction( "&Save As", this ); // on créer l'action Save As
	action                                -> setStatusTip( "Save to disk (rename) the Cell" );
	action                                -> setShortcut ( QKeySequence("CTRL+S") ); // ajoute un raccourci pour faire la sauvegarde
	action                                -> setVisible ( true ); // permet de la rendre visible par l'user

	fileMenu                              -> addAction( action ); // on ajoute l'action au menu
	connect( action , SIGNAL(triggered ()), this , SLOT(saveCell ()) ); // on effectue la connection / triggered() -> quand l'user clique sur le bouton, on appelle saveCell()
	

	action = new QAction("&Open Cell", this) ;
	action -> setStatusTip("Opening a Cell") ;
	action -> setShortcut(QKeySequence("CTRL+O")) ;
	action -> setVisible(true) ;
	fileMenu -> addAction(action) ;
	connect(action, SIGNAL(triggered()), this ,SLOT(openCell())) ;

	action                                = new QAction( "&Cell", this );
	action                                -> setShortcut ( QKeySequence("CTRL+A") );
	action                                -> setVisible ( true );
	fileMenu                              -> addAction( action );
	connect( action , SIGNAL(triggered ()), this , SLOT(showCellsLib()) );


	action                                = new QAction( "&Instance", this );
	action                                -> setShortcut ( QKeySequence("CTRL+E") );
	action                                -> setVisible ( true );
	fileMenu                              -> addAction( action );
	connect( action , SIGNAL(triggered ()), this , SLOT(showInstancesWidget()) );


	action                                = new QAction( "&Quit", this );
	action                                -> setStatusTip( "Exit the Netlist Viewer" );
	action                                -> setShortcut ( QKeySequence("CTRL+Q") );
	action                                -> setVisible ( true );
	fileMenu                              -> addAction( action );
	connect( action , SIGNAL(triggered ()), this , SLOT(close ()) );
	}

	CellViewer::~CellViewer()
	{
		delete cellWidget_ ;

	}

	void CellViewer::setCell(Cell* cellule)
	{
		cellWidget_->setCell(cellule) ; // on va setup le dessin que l'on veut dans la fenetre. 
		instancesWidget_ -> setCell(cellule) ; // permet de setup toutes les instances d'une entitée chargée
		cellsLib_ -> getBaseModel() -> setCell(cellule) ; //pas de setCell dans cellLib.h

	}

	Cell* CellViewer::getCell() const
	{
		return cellWidget_ -> getCell() ; // on récupère la cellule du dessin
	}

	void CellViewer :: saveCell ()
	{
		Cell* cell = getCell (); // On récupère la cellule voulue
		
		if ( cell == NULL ) return; 
		
		QString cellName = cell ->getName (). c_str ();
		
		if (saveCellDialog_ ->run(cellName )) // on appelle la fonction run qui va 
		{
			cerr << "In save cell" << endl ;
			cell ->setName( cellName.toStdString () );
			cell ->save ( cellName.toStdString () );
		}
	}
	void CellViewer::openCell() // doit recuperer le nom de la cellule a ouvrir
	{
		QString cellName ;
		Cell* cell;
		if(OpenCellDialog::run(cellName,this ))
		{	

			cell = Cell::find(cellName.toStdString()) ;

			if(not(cell)) // si find renvoie NULL c'est que la cellule n'est pas chargée dans ce cas on va la load5
			{

				cell = Cell::load(cellName.toStdString()) ;
				if(cell == nullptr) // on test si le load a bien réussi, permet d'éviter une segfault
				{
					return ;
				}
				cell->	setName(cellName.toStdString()) ;
				this -> setCell(cell) ;
				cerr << "Cell is well loaded " << endl ;			
			}
			else //Si find ne renvoie pas NULL c'est que la cellule est déjà chargée, dans ce cas on va juste l'afficher
			{
				cerr << "Cell is already load, printing " << endl ;
				cell->	setName(cellName.toStdString()) ;
				this -> setCell(cell) ;

			}


		}

	}
	CellWidget* CellViewer::getCellWidget() 
	{
		return cellWidget_ ;
	}
	void CellViewer::showInstancesWidget ()
	{
		instancesWidget_->show();
	}
	void CellViewer::showCellsLib()
	{
		cellsLib_ -> show() ;
	}



}



