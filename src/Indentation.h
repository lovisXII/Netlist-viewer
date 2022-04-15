// -*- explicit-buffer-name: "Indentation.h<M1-MOBJ/4-5>" -*-

#ifndef  NETLIST_INDENTATION_H
#define  NETLIST_INDENTATION_H

#include <iostream>
#include <string>
#include <vector>
#include  <cstdlib>


namespace Netlist {
/*
Cette classe va permette de gérer les indentations dans un affichage.

_tabulationSize : définit la taille d'une tabulation
_identation : affiche la tabulation 


*/
  class Indentation {
    public:

      //Constructeur :                
      
                    Indentation ();

      //Fonctions, surcharges des opérateurs :

      Indentation&  operator++  (); 
      Indentation   operator++  (int);
      Indentation&  operator--  ();
      Indentation   operator--  (int);
    private:
      size_t        _tabulationSize;
      std::string   _indentation;

      friend std::ostream& operator<< ( std::ostream&, const Indentation& );
  };


  extern Indentation  indent; // variable globale qui envoyée dans le flux de données affiche un nbre variables de tabulation. 


}  // Netlist namespace.

#endif //  NETLIST_INDENTATION_H
