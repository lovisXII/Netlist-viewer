#include <QDialog>
#include <QLineEdit>

namespace Netlist
{

	//Demande à l'utilisateur quel nom il souhaite donner a la cellule a sauvergarder
	class SaveCellDialog : public QDialog 
	{
		Q_OBJECT;

		public :

							SaveCellDialog 			( QWidget* parent=NULL );
		bool 		run 					( QString& name ); // recupere juste le nom de la cellule a sauvergarder; sauvegarde se fait dans CellViewer::saveCell()
		const QString 		getCellName 			() const;
		void 				setCellName 			( const QString& );
		
		protected :

		QLineEdit* lineEdit_;
	};
}

