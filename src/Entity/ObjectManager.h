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
#include <json/json.h>

#include <iostream>
#include <string>
#include <memory>

#include <crypt.h>

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
						db->exec(SetupQuery);
				      // Test if the 'test' table exists
				      const bool bExists = db->tableExists("objects");
				      if(bExists){
				    	  std::cout << "SQLite base already exists!\n";
				    	  return;
				      }

				      if(!bExists){
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

			a->setHash(getHash( a->getContent() ) );
			a->setChanged( std::time(nullptr) );

			bool insertNew = (a->getId()==0);
			if(insertNew){

				s << "INSERT INTO objects(NAME,CONTENT,URL,HASH,TYPE,CREATED,CHANGED) VALUES ("
	    			  << q(a->getName())<<c
					  << q(a->getContent())<<c
					  << q(a->getUrl())<<c
					  << q(a->getHash())<<c
					  << std::to_string(a->getType())<<c
					  << std::to_string(a->getCreated())<<c
					  << std::to_string(a->getChanged())
					  << ");";
				a->setCreated(a->getChanged());
			} else {
				s << "UPDATE objects SET "<<
		    			  "NAME="<< q(a->getName())<<c<<
						  "CONTENT="<< q(a->getContent())<<c<<
						  "URL="<< q(a->getUrl())<<c<<
						  "HASH="<< q(a->getHash())<<c<<
						  "TYPE="<< std::to_string(a->getType())<<c<<
						  "CREATED="<< std::to_string(a->getCreated())<<c<<
						  "CHANGED="<< std::to_string(a->getChanged())<<
						  " WHERE ID="<< a->getId() <<";";
			}
			std::cout << s.str() << std::endl;

			try {

				    	  db->exec(s.str());
				    	  if(insertNew){
				    		  a->setId( db->getLastInsertRowid());
				    	  }
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
									<<" WHERE ID=" << a->getId()<< ";";
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
			s << "INSERT INTO tagtotag(ID_CHILD,ID_PARENT) VALUES (" << fromid << ","<< toid << ");";

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
			s << "INSERT INTO objecttotag(ID_OBJECT,ID_TAG) VALUES (" << objid << ","<< tagid << ");";

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

		bool getObjectByID(int64_t id, AbstractObject &a){
			try {
				SQLite::Statement   query(*db, "SELECT * FROM objects WHERE ID =  ?");
				query.bind(1, (long long int)id);
				query.executeStep();

				a.setId(id);
				a.setName(query.getColumn(1));
				a.setContent(query.getColumn(2));
				a.setUrl(query.getColumn(3));
				a.setHash(query.getColumn(4));
				int i = query.getColumn(5);
				a.setType((ObjectType)i);
				int64_t crTime = query.getColumn(6);
				a.setCreated(crTime);
				int64_t chTime = query.getColumn(7);
				a.setChanged(chTime);

			}
				catch (std::exception& e)
			{
				std::cout << "SQLite exception: " << e.what() << std::endl;
				return(false);
			}
				return(true);
		}

		bool getObjectsList(std::vector<AbstractObject> &a){
			try {
				SQLite::Statement   query(*db, "SELECT ID,NAME,TYPE,CREATED,CHANGED,HASH FROM objects ORDER BY CHANGED DESC");
				a.clear();
				while(query.executeStep()){
					AbstractObject obj;
					obj.setId((int64_t)query.getColumn(0));
					obj.setName(query.getColumn(1));
					int ty = query.getColumn(2);
					obj.setType((ObjectType)ty);
					int64_t crTime = query.getColumn(3);
					obj.setCreated(crTime);
					int64_t chTime = query.getColumn(4);
					obj.setChanged(chTime);
					obj.setHash(query.getColumn(5));
					a.push_back(obj);
				}

			}
				catch (std::exception& e)
			{
				std::cout << "SQLite exception: " << e.what() << std::endl;
				return(false);
			}
				return(true);
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

		bool getTagsTree(Json::Value &val){
			try {
							SQLite::Statement   query(*db, "select t1.ID, t1.name, t.ID, t.name FROM tagtotag tt INNER JOIN tags t ON tt.ID_CHILD=t.ID INNER JOIN tags t1 ON tt.ID_PARENT=t1.ID;");

							int j(0);
							int k = 0;
							bool first(true);
							std::map< int,std::pair<int,int> > m;
							while(query.executeStep()){
								int id = query.getColumn(0);
								std::map< int,std::pair<int,int> >::iterator it = m.find(id);
								if(it!=m.end()){
									m[id].second++;
									val[j]["nodes"][m[id].second]["id"]= (Json::Int64) query.getColumn(2);
									val[j]["nodes"][m[id].second]["text"]= query.getColumn(3).getText();
								} else {
									m[id]=std::make_pair(j++,0);
									val[j]["id"] = id;
									val[j]["text"] =  query.getColumn(1).getText();
									val[j]["nodes"][0]["id"]= (Json::Int64) query.getColumn(2);
									val[j]["nodes"][0]["text"]= query.getColumn(3).getText();
								}

							}

							std::cout << val.toStyledString() <<"" << std::endl;

						}
							catch (std::exception& e)
						{
							std::cout << "SQLite exception: " << e.what() << std::endl;
							return(false);
						}
							return(true);
		}


		bool getTagList(Json::Value &val){
			try {
							SQLite::Statement   query(*db, "SELECT ID,NAME FROM tags;");

							int j(0);
							while(query.executeStep()){

									val[j]["id"]= (Json::Int64) query.getColumn(0);
									val[j]["name"]= query.getColumn(1).getText();
									j++;
							}
							std::cout << val.toStyledString() <<"" << std::endl;

						}
							catch (std::exception& e)
						{
							std::cout << "SQLite exception: " << e.what() << std::endl;
							return(false);
						}
							return(true);
		}
/*
 select ID_PARENT, group_concat(ID_CHILD) from tagtotag group by ID_PARENT;

INSERT INTO tags(NAME,CONTENT) VALUES ('Родитель','такой тэг');

INSERT INTO tagtotag(ID_CHILD,ID_PARENT) VALUES ();



select ID_PARENT, group_concat(ID_CHILD) from tagtotag LEFT OUTER JOIN tags ON tagtotag.ID_PARENT = tags.ID;

 * */

};

#endif /* ENTITY_OBJECTMANAGER_H_ */
