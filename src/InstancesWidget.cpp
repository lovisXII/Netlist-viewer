#include "InstancesWidget.h"
#include "InstancesModel.h"
#include <QHeaderView>
#include "CellViewer.h"
#include "Cell.h"
namespace Netlist
{
	InstancesWidget :: InstancesWidget ( QWidget* parent )
	: QWidget (parent)
	, cellViewer_ (NULL)
	, baseModel_ (new InstancesModel(this))
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
		view_ ->setModel ( baseModel_ ); // On associe le modele, affiche la liste des instances + masterCell

		QHeaderView* horizontalHeader = view_ ->horizontalHeader ();
		horizontalHeader ->setDefaultAlignment ( Qt:: AlignHCenter );
		horizontalHeader ->setMinimumSectionSize( 300 );
		horizontalHeader ->setStretchLastSection( true );
		QHeaderView* verticalHeader = view_ ->verticalHeader ();
		verticalHeader ->setVisible( false );
		load_ ->setText( "Load" );
		connect( load_ , SIGNAL(clicked ()), this , SLOT(load ()) );
	}


	void InstancesWidget ::setCellViewer(CellViewer* viewer)
	{
		cellViewer_ = viewer ; 
	}
	int InstancesWidget :: getSelectedRow () const // renvoie l'index de la ligne selectionnée
	// si aucune ligne selectionnée renvoie -1
	{
		QModelIndexList selecteds = view_ ->selectionModel ()->selection (). indexes ();
		if (selecteds.empty ()) return -1;
		return selecteds.first ().row ();
	}

	void InstancesWidget ::load ()//on charge la master cell dans la ligne selectionnée
	{
		int selectedRow = getSelectedRow (); // renvoie la ligne selectionnée
		if (selectedRow < 0) return; // si la ligne selectionnée est négative il y a un pb
		cellViewer_ ->CellViewer::setCell(baseModel_ ->getModel(selectedRow)); // on charge la MasterCell dans le main 
		cerr <<"Chargement de " << cellViewer_ -> getCell() -> getName() <<" réussi" << endl ;
	}
}