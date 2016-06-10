#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include <mongoose/WebController.h>
#include <mongoose/JsonController.h>
#include <string>

#include <Entity/ObjectManager.h>
#include <ConfigManager.h>
#include <NoteTagController.h>

using namespace Mongoose;
namespace fs = boost::filesystem;


void MongooseTest(){
		ConfigManager cm;
		cm.read();

		SQLite::Database db("database.db3", SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE);
		ObjectManager man;
		man.setDb(&db);
		man.initDatabase();

		NoteTagController ntController;
		ntController.setManager(&man);

	    Server server(8080);
	    //server.setOption("enable_directory_listing", "false");
	    server.setOption("document_root", cm.getWebroot());
	    server.registerController(&ntController);

	    server.start();

	    Json::Value v;
	    man.getTagsTree(v);

	    while (1) {
	    	usleep(10);
	    }
}



void dbTest(){
	try
	  {
	      // Open a database file in readonly mode
	      SQLite::Database    db("db.db3", SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE);  // SQLITE_OPEN_READONLY
	      std::cout << "SQLite database file '" << db.getFilename().c_str() << "' opened successfully\n";

	      // Test if the 'test' table exists
	      const bool bExists = db.tableExists("test");
	      std::cout << "SQLite table 'test' exists=" << bExists << "\n";
	      if(!bExists){
	    	  db.exec("CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)");
	          // Get a single value result with an easy to use shortcut
	          db.exec("INSERT INTO test(value) VALUES ('webpages')");
	      }
	      SQLite::Statement query(db, "SELECT * FROM test");
	      while (query.executeStep())
	      {
	          // Demonstrates how to get some typed column value (and the equivalent explicit call)
	          const int         id     = query.getColumn(0); // = query.getColumn(0).getInt();
	          std::string       value     = query.getColumn(1); // = query.getColumn(0).getInt();
	          std::cout << "[" << id << "] : "<< value.c_str() << std::endl;
	      }
	  }
	  catch (std::exception& e)
	  {
	      std::cout << "SQLite exception: " << e.what() << std::endl;
	  }
}


void dbTest2(){
	SQLite::Database db("database.db3", SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE);
			ObjectManager man;
			man.setDb(&db);
			AbstractObject a;
			man.getObjectByID(3,a);
			std::cout << "name=" << a.getName() << std::endl;
}
int main(){
  std::cout << "Hi!" << std::endl;
  MongooseTest();
  //dbTest2();



  /*SQLite::Database db("database.db3", SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE);
  ObjectManager man;
  man.setDb(&db);
  man.initDatabase();


  AbstractObject a[5];

  for(int i=0; i< 5; i++){
	  a[i].setName("Note "+ std::to_string(i));
	  a[i].setContent(" preety <i>good</i> <b>html</b> test");
	  man.storeObject(&a[i]);
  }
  TagObject t1;
  t1.setName("Good");
  man.storeTag(&t1);
  TagObject t2;
  t2.setName("TRtf");
  man.storeTag(&t2);

  man.addRelationObjectTag(a[0],t1);
  man.addRelationObjectTag(a[1],t1);
  man.addRelationObjectTag(a[2],t1);
  man.addRelationObjectTag(a[2],t2);
  man.addRelationObjectTag(a[3],t2);
  man.addRelationObjectTag(a[4],t2);*/

  return(0);
}
