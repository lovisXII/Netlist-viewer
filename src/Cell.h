// -*- explicit-buffer-name: "Cell.h<M1-MOBJ/4-5>" -*-

#ifndef NETLIST_CELL_H
#define NETLIST_CELL_H

#include <string>
#include <vector>
#include  <cstdlib>
#include<fstream>
#include  <libxml/xmlreader.h>
#include "Symbol.h"       // TME7

namespace Netlist {

  class Instance ;
  class Term ;
  class Net;
  
  
  using namespace std;

  class Cell 
  {
  
    public:
  
      static       std::vector<Cell*>&     getAllCells       ();
      static       Cell*                   find              ( const std::string& );
  
    public:                                                  
    
    //Constructeurs :
                                           Cell              ( const std::string& );
                                          ~Cell              ();

    //Accesseurs :

      inline const std::string&            getName           () const;
      inline Symbol*                       getSymbol         () const;  // TME7
      inline const std::vector<Instance*>& getInstances      () const;
      inline const std::vector<Term*>&     getTerms          () const;
      inline const std::vector<Net*>&      getNets           () const;
                   Instance*               getInstance       ( const std::string& ) const;
                   Term*                   getTerm           ( const std::string& ) const;
                   Net*                    getNet            ( const std::string& ) const;

    //Modificateurs :
                   
                   void                    setName           ( const std::string& );
                   void                    add               ( Instance* );
                   void                    add               ( Term* );
                   void                    add               ( Net* );
                   void                    remove            ( Instance* );
                   void                    remove            ( Term* );
                   void                    remove            ( Net* );
                   bool                    connect           ( const std::string& name, Net* net );
                   unsigned int            newNetId          ();
    
                   void                    toXml             (ostream&) const ;
     static        Cell*                   fromXml           (xmlTextReaderPtr reader);              
                   void                    save              (const std::string& name ) const ;
     static        Cell*                   load              ( const string& cellName ) ;
    private:
  
      static  std::vector<Cell*>      cells_; // liste de toutes les cells allouées
              Symbol                  symbol_;
              std::string             name_;
              std::vector<Term*>      terms_; // conteneur de tous les Terms de la cell
              std::vector<Instance*>  instances_; // conteneur de tous les Instance de la cell
              std::vector<Net*>       nets_; // conteneur de tous les nets de la cell
              unsigned int            maxNetIds_;
  };

  inline Symbol*                       Cell::getSymbol    () const { return const_cast<Symbol*>(&symbol_); }  // TME7
  inline const std::string&            Cell::getName      () const { return name_; }
  inline const std::vector<Instance*>& Cell::getInstances () const { return instances_; };
  inline const std::vector<Term*>&     Cell::getTerms     () const { return terms_; };
  inline const std::vector<Net*>&      Cell::getNets      () const { return nets_; };


}  // Netlist namespace.

#endif  // NETLIST_CELL_H

