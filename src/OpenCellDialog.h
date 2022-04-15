#pragma once
#include <QDialog>
#include <QLineEdit>

namespace Netlist
{
	class CellViewer ;
	class OpenCellDialog : public QDialog
	{
		Q_OBJECT

		public :
								OpenCellDialog		(QWidget* parent=NULL ) ;
								~OpenCellDialog		() ;
			static bool			run					(QString& name, CellViewer* parent) ;
			const QString 		getCellName 		() const;
			void 				setCellName 		(const QString&) ;
 
		protected :
		QLineEdit *lineEdit_ ;
	};
}