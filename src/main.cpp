#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>

#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

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


using namespace Mongoose;
namespace fs = boost::filesystem;

class JController : public JsonController{
	    void list(Request &request, JsonResponse &response)
	    {

	    	fs::path targetDir(".");
	    	fs::directory_iterator it(targetDir), eod;

	    	int i = 0;
	    	BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod))
	    	{
	    	    if(fs::is_regular_file(p))
	    	    {
	    	        // do something with p
	    	    	response[i++]["name"] = p.filename().generic_string();
	    	    }
	    	}


/*   for (int i=0; i<5; i++) {
	            	response[i]["id"] = i;
	                response[i]["name"] = "Note ";// + std::to_string(i);*/

	    }

        void setup()
        {
        	addRouteResponse("GET", "/list", JController, list, JsonResponse);
        }
};

class MyController : public Mongoose::WebController
{
    public:
        void hello(Mongoose::Request &request, Mongoose::StreamResponse &response)
        {
            response << "Hello " << htmlEntities(request.get("name", "... what's your name ?")) << endl;
        }

        void setup()
        {
            addRoute("GET", "/hello", MyController, hello);
        }
};

void MongooseTest(){
	    MyController myController;
	    JController myjController;

	    Server server(8080);
	    server.registerController(&myController);
	    server.registerController(&myjController);

	    server.start();

	    while (1) {
	    	usleep(10);
	    }
}

void hashTest(){
	  std::string s("dsfghjkhgfghjg");
	  CryptoPP::SHA256 hash;
	  byte digest[CryptoPP::SHA256::DIGESTSIZE];

	  hash.Update( (const byte*) s.c_str(), s.length());
	  hash.Final(digest);

	  CryptoPP::HexEncoder encoder;
	  std::string output;

	  encoder.Attach( new CryptoPP::StringSink( output ) );
	  encoder.Put( digest, sizeof(digest) );
	  encoder.MessageEnd();

	  std::cout << output << std::endl;
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

int main(){
  std::cout << "Hi!" << std::endl;

  SQLite::Database db("database.db3", SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE);
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
  man.addRelationObjectTag(a[4],t2);/**/

  return(0);
}
