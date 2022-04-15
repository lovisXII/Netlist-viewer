#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "OpenCellDialog.h"
#include "CellViewer.h"
#include "CellWidget.h"

namespace Netlist
{
    //a mdofier pour bien prendere en compte le fait que ca open et pas que ca ferme
     OpenCellDialog ::  OpenCellDialog ( QWidget* parent )
    : QDialog (parent)
    , lineEdit_(NULL)
    {
        setWindowTitle( tr("Open Cell") );
        QLabel* label               = new QLabel ();
        label                       -> setText( tr("Enter Cell name (without extention)") );
        lineEdit_                   = new QLineEdit ();
        lineEdit_                   -> setMinimumWidth( 400 );
        QPushButton* okButton       = new QPushButton ();
        okButton                    -> setText ( "OK" );
        okButton                    -> setDefault( true );
        QPushButton* cancelButton   = new QPushButton ();
        cancelButton                -> setText( "Cancel" );
        QHBoxLayout* hLayout        = new QHBoxLayout ();
        hLayout                     -> addStretch ();
        hLayout                     -> addWidget ( okButton );
        hLayout                     -> addStretch ();
        hLayout                     -> addWidget ( cancelButton );
        hLayout                     -> addStretch ();
        QFrame* separator           = new QFrame ();
        separator                   -> setFrameShape ( QFrame :: HLine );
        separator                   -> setFrameShadow( QFrame :: Sunken );
        QVBoxLayout* vLayout        = new QVBoxLayout ();
        vLayout                     -> setSizeConstraint( QLayout :: SetFixedSize );
        vLayout                     -> addWidget( label );
        vLayout                     -> addWidget( lineEdit_ );
        vLayout                     -> addLayout( hLayout );
        setLayout( vLayout );
        connect( okButton , SIGNAL(clicked ()), this , SLOT(accept ()) );
        connect( cancelButton , SIGNAL(clicked ()), this , SLOT(reject ()) );
        //...
    }
    
    OpenCellDialog::~OpenCellDialog()
    {

    }

    const QString  OpenCellDialog :: getCellName () const
    { 
        return lineEdit_ ->text (); 
    }

    void  OpenCellDialog :: setCellName ( const QString& name )
    { 
        return lineEdit_ ->setText( name );
    }

    bool  OpenCellDialog ::run ( QString& name, CellViewer* parent)
    {
        OpenCellDialog* test = new OpenCellDialog(parent->getCellWidget()) ;
        test -> setCellName( name );
        int dialogResult = test -> exec ();
        name = test -> getCellName ();
        delete test ;
        return (dialogResult == Accepted );
    }

}

