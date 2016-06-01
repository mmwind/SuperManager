/*
 * ObjectManager.h
 *
 *  Created on: 11 апр. 2016 г.
 *      Author: lladmin
 */

#ifndef ENTITY_OBJECTMANAGER_H_
#define ENTITY_OBJECTMANAGER_H_

#include "AbstractObject.h"
#include "TagObject.h"

#include <SQLiteCpp/SQLiteCpp.h>


#include <iostream>
#include <string>
#include <memory>

const std::string CreateObjectTableQuery =
"CREATE TABLE objects ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
	"NAME TEXT,"
    "CONTENT TEXT,"
    "URL TEXT,"
    "HASH TEXT,"
    "TYPE INTEGER NOT NULL,"
    "CREATED INTEGER,"
    "CHANGED INTEGER"
");";

const std::string CreateTagTableQuery =
"CREATE TABLE tags ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
	"NAME TEXT UNIQUE,"
    "CONTENT TEXT"
");";

const std::string CreateObjectTagRelationQuery =
"CREATE TABLE objecttotag ("
    "ID_OBJECT INTEGER,"
    "ID_TAG INTEGER,"
    "FOREIGN KEY(ID_OBJECT) REFERENCES objects(ID)"
    "FOREIGN KEY(ID_TAG) REFERENCES tags(ID)"
");";

const std::string CreateTagTagRelationQuery =
"CREATE TABLE tagtotag ("
    "ID_CHILD INTEGER,"
    "ID_PARENT INTEGER,"
    "FOREIGN KEY(ID_CHILD) REFERENCES tags(ID)"
    "FOREIGN KEY(ID_PARENT) REFERENCES tags(ID)"
");";

const std::string c =",";

const std::string SetupQuery = "PRAGMA foreign_keys = ON;";

/*
// get tags
 SELECT * FROM objects o LEFT JOIN objecttoobject oo ON oo.ID_PARENT = o.ID WHERE oo.ID_CHILD = 1



 */

class ObjectManager {
	private:
		SQLite::Database    *db;

	public:
		ObjectManager() { };

		virtual ~ObjectManager() {
		}

		const SQLite::Database* getDb() const {
			return db;
		}

		void setDb(SQLite::Database* db) {
			this->db = db;
		}

		void initDatabase(){
			try
				  {

				      // Test if the 'test' table exists
				      const bool bExists = db->tableExists("objects");
				      if(bExists){
				    	  std::cout << "SQLite base already exists!\n";
				    	  return;
				      }
				      if(!bExists){
				    	  db->exec(SetupQuery);
				    	  db->exec(CreateObjectTableQuery);
				    	  db->exec(CreateTagTableQuery);
				    	  db->exec(CreateObjectTagRelationQuery);
				    	  db->exec(CreateTagTagRelationQuery);
				      }
				  }
				  catch (std::exception& e)
				  {
				      std::cout << "SQLite exception: " << e.what() << std::endl;
				  }
		}
		std::string q(std::string s){
			return("\'"+s+"\'");
		}

		AbstractObjectPtr newObject(ObjectType Type){
			AbstractObjectPtr p(new AbstractObject);
			return(std::move(p));
		}

		void storeObject(AbstractObject *a){

			std::stringstream s;

			if(a->getId()==0){

			s << "INSERT INTO objects(NAME,CONTENT,URL,HASH,TYPE,CREATED,CHANGED) VALUES ("
	    			  << q(a->getName())<<c
					  << q(a->getContent())<<c
					  << q(a->getUrl())<<c
					  << q(a->getHash())<<c
					  << std::to_string(a->getType())<<c
					  << std::to_string(a->getCreated())<<c
					  << std::to_string(a->getChanged())
					  << ");";
			} else {
				s << "UPDATE objects SET"<<
		    			  "NAME="<< q(a->getName())<<c<<
						  "CONTENT="<< q(a->getContent())<<c<<
						  "URL="<< q(a->getUrl())<<c<<
						  "HASH="<< q(a->getHash())<<c<<
						  "TYPE="<< std::to_string(a->getType())<<c<<
						  "CREATED="<< std::to_string(a->getCreated())<<c<<
						  "CHANGED="<< std::to_string(a->getChanged())<<
						  "WHERE ID="<< a->getId() <<";";
			}
			std::cout << s.str() << std::endl;

			try {
				    	  db->exec(s.str());
				    	  a->setId( db->getLastInsertRowid());
				}
				 catch (std::exception& e)
				{
				      std::cout << "SQLite exception: " << e.what() << std::endl;
				}
		}

		void storeTag(TagObject *a){
			std::stringstream s;

			if(a->getId()==0){

				s << "INSERT INTO tags(NAME,CONTENT) VALUES ("
	    			  << q(a->getName())<<c
					  << q(a->getContent())<< ");";
			} else {
				s << "UPDATE tags SET NAME="<< q(a->getName())<<
									",CONTENT="<< q(a->getContent())
									<<"WHERE ID=" << a->getId()<< ";";
			}
			std::cout << s.str() << std::endl;

			try {
				    	  db->exec(s.str());
				    	  if(a->getId()==0)
				    		  a->setId( db->getLastInsertRowid());
				}
				 catch (std::exception& e)
				{
				      std::cout << "SQLite exception: " << e.what() << std::endl;
				}
		}

		void addRelationTagTagID(int64_t fromid, int64_t toid){

			std::stringstream s;
			s << "INSERT INTO tagtotag(ID_CHILD,ID_PARENT) VALUES (" << fromid << ","<< toid << ")";

			try {
				    	  db->exec(s.str());
				}
				 catch (std::exception& e)
				{
				      std::cout << "SQLite exception: " << e.what() << std::endl;
				}
		}

		void addRelationObjectTagID(int64_t objid, int64_t tagid){

			std::stringstream s;
			s << "INSERT INTO objecttotag(ID_OBJECT,ID_TAG) VALUES (" << objid << ","<< tagid << ")";

			try {
				    	  db->exec(s.str());
				}
				 catch (std::exception& e)
				{
				      std::cout << "SQLite exception: " << e.what() << std::endl;
				}
		}

		void addRelationTagTag(TagObject tchild,TagObject tparent){
			addRelationTagTagID(tchild.getId(),tparent.getId());
		}

		void addRelationObjectTag(AbstractObject object,TagObject tag){
			addRelationObjectTagID(object.getId(),tag.getId());
		}


		void getObjectsByTagID(int64_t tagid){
			std::stringstream s;
						s << "SELECT * FROM objects o LEFT JOIN objecttotag ot ON ot.ID_OBJECT = o.ID WHERE ot.ID_TAG = "<<tagid <<";";

						try {
							    	  db->exec(s.str());
							}
							 catch (std::exception& e)
							{
							      std::cout << "SQLite exception: " << e.what() << std::endl;
							}
		}
		void getTagsByObjectID(int64_t objid){
			std::stringstream s;
						s << "SELECT * FROM tags t LEFT JOIN objecttotag ot ON ot.ID_TAG = t.ID WHERE ot.ID_OBJECT = "<<objid <<";";

						try {
							    	  db->exec(s.str());
							}
							 catch (std::exception& e)
							{
							      std::cout << "SQLite exception: " << e.what() << std::endl;
							}
		}
};

#endif /* ENTITY_OBJECTMANAGER_H_ */
