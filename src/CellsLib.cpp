
#include "CellsLib.h"
#include "CellViewer.h"
#include "CellsModel.h"
#include "Cell.h"
#include "Instance.h"


#include <QHeaderView>

namespace Netlist
{
	CellsLib::CellsLib(QWidget* parent)
	: QWidget (parent)
	, cellViewer_ (NULL)
	, baseModel_ (new CellsModel(this))
	, view_ (new QTableView(this)) // on créer une Qtable, dans laquelle on va pouvoir stocker les Instance
	, load_ (new QPushButton(this)) // bouton de chargement de la cellule
	{
		setAttribute( Qt:: WA_QuitOnClose , false ); // fonction de QWidget, supprime l'attribut 
		
		//WA_QuitOnClose makes qt quit the application when the last widget with the attribute set has accepted closeEvent()
		
		setAttribute( Qt:: WA_DeleteOnClose , false );
		
		//WA_DeleteOnClose Qt delete this widget when there is a closeEvent()
		
		setContextMenuPolicy( Qt:: ActionsContextMenu );
		setSizePolicy   ( QSizePolicy::Expanding, QSizePolicy::Expanding );
		
		view_ ->setShowGrid ( false ); // say if the grid is drawn
		view_ ->setAlternatingRowColors( true );
		view_ ->setSelectionBehavior ( QAbstractItemView :: SelectRows ); //SelectRows : select a line 
		view_ ->setSelectionMode ( QAbstractItemView :: SingleSelection ); // can only select a ligne at once
		view_ ->setSortingEnabled ( true ); // permet de sortir 
		view_ ->setModel ( baseModel_ ); // On associe le modele.

		QHeaderView* horizontalHeader = view_ ->horizontalHeader ();
		horizontalHeader ->setDefaultAlignment ( Qt:: AlignHCenter );
		horizontalHeader ->setMinimumSectionSize( 300 );
		horizontalHeader ->setStretchLastSection( true );
		QHeaderView* verticalHeader = view_ ->verticalHeader ();
		verticalHeader ->setVisible( false );
		load_ ->setText( "Load" );
		connect( load_ , SIGNAL(clicked ()), this , SLOT(load ()) );
	}


	void CellsLib::setCellViewer(CellViewer* viewer)
	{
		cellViewer_ = viewer ;
	}
	int CellsLib::getSelectedRow () const
	// si aucune ligne selectionnée renvoie -1
	{
		QModelIndexList selecteds = view_ ->selectionModel ()->selection (). indexes ();
		if (selecteds.empty ()) return -1;
		return selecteds.first ().row ();
	}
	void CellsLib::load()
	{
		int selectedRow = getSelectedRow (); // renvoie la ligne selectionnée
		if (selectedRow < 0) return; // si la ligne selectionnée est négative il y a un pb
		cellViewer_ ->CellViewer::setCell(baseModel_ ->CellsModel :: getModel(selectedRow)); // on charge la MasterCell dans le main 
		cerr <<"Chargement de " << cellViewer_ -> getCell() -> getName() <<" réussi" << endl ;
	}
}