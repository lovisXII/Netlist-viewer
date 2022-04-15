#include "Net.h"

#include "Cell.h"
#include "Node.h"
#include "Term.h"
#include "Point.h"
#include "Indentation.h"
#include "XmlUtil.h"
#include "Line.h"

namespace Netlist
{
	

	Net::Net(Cell* cellule , const std::string& name, Term::Type dir) 
		: owner_ (cellule),
		  name_(name),
		  id_(cellule->newNetId()),
		  type_(dir),
		  nodes_() //on veut un vecteur vide à l'inialisation
	{
		// on ajoute le noeud créer au niveau de la Cell
		cellule->add(this) ;
	}
	
	Net::~Net()
	{
		//Quand on supprime un net il faut détacher tous les nodes qui lui sont connectés
		//Il faut également mettre l'attribut net_ des Terms à NULL
		for(size_t i = 0 ; i < nodes_.size(); i++)
		{
			//nodes_[i] : Node*
			//nodes_[i]->getTerm() : Term*
			//(nodes_[i]->getTerm())->setNet(NULL) : Term::net_ =NULL

			//On modifie le destructeur pour qu'il sache s'il doit supprimer une NodePoint ou un NodeTerm
			
			NodeTerm* noeud = dynamic_cast<NodeTerm*> (nodes_[i]) ; 
			if(  noeud != NULL )
			{
				(noeud->getTerm())->setNet(NULL) ;	
			}
			else
			{
				delete nodes_[i] ;	
			}
				
			
		}
		owner_->remove(this) ; //on retire le net de sa cell
	}

    Node* Net::getNode(size_t id) const 
    {
        vector<Node*>nodes = getNodes();
        for (auto i = nodes.begin(); i!= nodes.end(); ++i ){
            if(*i)    
                if ((*i)->getId() == id){
                    return *i;
                }
        }
        return NULL;
    }	
	
	size_t Net::getFreeNodeId() const // renverra l'index de la premiere case libre dans le tableau nodes_, si aucune case n'est vide, elle renverra la taille du tableau, ie. l'index ap le dernier element
	{
		for(size_t i = 0 ; i < nodes_.size() ; i++)
			//faire ++i est plus rapide que i++
			//quand on fait i++ le compilateur garde en mémoire la valeur de i avant incrémentation
			//++i ne garde que la nouvelle valeur en mémoire.
		{
			if(nodes_[i] == NULL)
			{
				break ; // dès que l'on rencontre une pointeur NULL on arrete la boucle et on renvoie sa valeur
				return i ;
			}
		}
		return nodes_.size() ;
	} 
	
	//Modificateurs :

	void Net::add ( Node* noeud)
	{
		//Correction :
		//if(not n ) return ;
		//Permet de sortir de la fonction dans le cas ou on ajouterait un pointeur NULL
		//Il faut ajouter un message d'erreur dans ce cas.
			size_t position_insertion = noeud->getId() ; // on récupère l'id du noeud que l'on veut insérer
			//on regarde si l'id est défini, si ce n'est pas le cas, on met notre noeud a la premiere place dispo
			
			if(position_insertion == Node::noid)
			{
				//on aurait pu utiliser la fonction getId pour simplifier ce bordel
				for(size_t i = 0 ; i < nodes_.size() ; i++)
				{
					if(nodes_[i] == NULL)
					{
						nodes_[i] = noeud ;
						noeud->setId(i) ;
						exit(0) ;
					}
				}
				nodes_.push_back(noeud) ;
				noeud->setId(nodes_.size()-1) ;
				
			}
			else
			{
				/*
				On va insérer un noeud à la position position_insertion de notre vecteur nodes_.
				Si la position existe déja, on insère à cette position en vérifiant que rien n'est déjà écrit à cette emplacement
				Sinon si cette position n'existe pas, on va push_back(NULL) jusqu'à créer l'emplacement que l'on veut
				*/
				if(position_insertion < nodes_.size())
				{
					if(nodes_[position_insertion] == NULL)
					{
						nodes_[position_insertion] = noeud ; 
					}
					else
					{
						cerr <<"Erreur le noeud est déjà occupé, merci d'en choisir un autre" << endl ;
					}
				}
				else
				{
					while(nodes_.size() != position_insertion)
					{
						nodes_.push_back(NULL) ;
					}
					nodes_.push_back(noeud) ;
				}
			}
		
		
		
	}
	bool Net::remove( Node* noeud)
	{
		size_t id = noeud->getId() ; // on récupère l'indice du noeud dans nodes_
		nodes_.erase(nodes_.begin()+id) ;
		return 0 ;
	}

	void Net::toXml(ostream& os)
	{
		extern Indentation indent ;
		
		os<<indent<<"<net name=\""<<this->getName()<<"\" type=\""<<Term::toString(this->getType())<<"\">\n";//on affiche tous les nets de la Cell
		
		std::vector<Node*> nodes = this->getNodes() ;
		
		indent++ ;

		for(auto it = nodes.begin() ; it != nodes.end() ; ++it) // on affiche les nodes
		{
			if((*it) !=NULL)
			{
				(*it)->toXml(os) ;
			}
		}
		
		std::vector<Line*> lines = this ->getLines() ;
		for(auto it = lines.begin() ; it != lines.end() ; ++it)
		{
			if((*it) != NULL)
			{
				(*it)->toXml(os) ;
			}
		}
		indent -- ;
		os<<indent<<"</net>" <<endl;
	}

	Net* Net::fromXml(Cell* cellule, xmlTextReaderPtr reader)
	{

		std::string name 		= xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name")) ;
		std::string type_string = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"type")) ;
		
		if(not(name.empty() or type_string.empty())) // si le nom de la cell et son type ne sont pas vides
		{
			Term::Type type = Term::toType(type_string) ;
			Net* net = new Net(cellule, name, type) ;
			
			const xmlChar* nodeTag = xmlTextReaderConstString( reader, (const xmlChar*)"node" );
			const xmlChar* lineTag = xmlTextReaderConstString( reader, (const xmlChar*)"line" );

			while(true)
			{

				  xmlTextReaderRead(reader);

			      switch ( xmlTextReaderNodeType(reader) ) 
			      {
			        //xmlTextReaderNodeType récupère le type du noeud actuel
			        //ca retourne le type du noeud ou -1 en cas d'erreur

			        case XML_READER_TYPE_COMMENT:
			          //si on est de type commentaire on ne fait rien
			        case XML_READER_TYPE_WHITESPACE:
			          //si on est de type espace on ne fait rien
			        case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
			          //si on est de type "espace significatif" on ne fait rien
			          continue;
			      }

			      const xmlChar* nodeName = xmlTextReaderConstLocalName( reader ); // on va setup le nodeName, au debut il va valoir Node

			    if ((nodeName == nodeTag ))          
                {
                	
                    if(Node::fromXml(net,reader))
                    {
                    	
                    	continue;	
                    } 
                    else
                    {
                    	cerr << "Impossible de charger le node depuis la Net, une erreur a été rencontrée" <<endl ;
                    	exit(0) ;
                    }                    
                }
                else if((nodeName == lineTag))
                {
                	
                	if(Line::fromXml(net,reader))
                	{
      
                		continue ;
                	}
                	else
                	{
                		cerr << "Impossible de charger les lines depuis le net" << endl ;
                		exit(0) ;
                	}
                }
                else
                {
                    break;       
                }
                

			} 

			return net ;
		}
		else
		{
			if(name.empty())
			{
				cerr << "name is empty" <<endl ;
			}
			if (type_string.empty())
			{
				cerr << "Type is empty" << endl ;
			}
			cerr<< "Impossible de récupérer les nets correctement"<<endl ;
			exit(0) ;
		}
		
	}
	bool  Net::remove ( Line* line )
	{
	  if (line) {
	    for ( vector<Line*>::iterator il = lines_.begin()
	        ; il != lines_.end() ; ++il ) {
	      if (*il == line) {
	        lines_.erase( il );
	        return true;
	      }
	    }
	  }
	  return false;
	}
	
}