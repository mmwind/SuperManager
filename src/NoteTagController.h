/*
 * NoteTagController.h
 *
 *  Created on: 5 июня 2016 г.
 *      Author: lladmin
 */

#ifndef NOTETAGCONTROLLER_H_
#define NOTETAGCONTROLLER_H_

#include <iostream>
#include <string>
#include <mongoose/Server.h>
#include <mongoose/JsonController.h>
#include <Entity/ObjectManager.h>

namespace Mongoose
{
class NoteTagController : public JsonController{
	private:
		ObjectManager *man;
	public:
		void setManager(ObjectManager *objman){
			man = objman;
		}

		ObjectManager * getManager(){
			return(man);
		}

		void getNote(Request &request, JsonResponse &response)
		{
			    	std::string sid = request.get("id","");
			    	int64_t id;
			    	stringstream strm(request.get("id",""));
			    	strm >> id;

			    	AbstractObject o;
			    	if(man->getObjectByID(id,o)){
						response["id"]= sid;
						response["name"]=o.getName();
						response["content"]=o.getContent();
						response["url"]=o.getUrl();
						response["hash"]=o.getHash();
						response["type"]=(int)o.getType();
						response["created"]= (Json::Int64)o.getCreated();
						response["changed"]= (Json::Int64)o.getChanged();
			    	}
				std::cout << request.get("id","0") << std::endl;
		}

		void getNoteList(Request &request, JsonResponse &response)
		{
			    	std::vector<AbstractObject> os;

			    	if(man->getObjectsList(os)){
			    		for(int i=0; i< os.size(); i++){
							response[i]["id"]= (Json::Int64)os[i].getId();
							response[i]["name"]=os[i].getName();
							response[i]["type"]=(int)os[i].getType();
							response[i]["created"]= (Json::Int64)os[i].getCreated();
							response[i]["changed"]= (Json::Int64)os[i].getChanged();
							response[i]["hash"]=os[i].getHash();

			    		}
			    	}
				std::cout << "List of notes "<< os.size() << std::endl;
		}

		void storeNote(Request &request, JsonResponse &response)
		{

			AbstractObject a;
			int tmp;
			stringstream strm(request.get("id","0"));
			if(strm.str().empty()){
				tmp = 0;
			} else
				strm >> tmp;

			std::cout << "POST id = "<< tmp << " '" << strm.str()<< "'"<< std::endl;

			a.setId(tmp);
			a.setName(request.get("name","New note"));
			a.setContent(request.get("content","New note"));
			a.setUrl(request.get("url",""));

			stringstream strm1(request.get("type",""));
			if(strm1.str().empty()){
				tmp = 0;
			} else
				strm1 >> tmp;
			a.setType((ObjectType)tmp);

			man->storeObject(&a);
			response["id"] = (Json::Int64)a.getId();
		}

		void  getTagTree(Request &request, JsonResponse &response)
		{
			man->getTagsTree(response);
		}

		void  getTagsList(Request &request, JsonResponse &response)
		{
			man->getTagList(response);
			std::cout << " getTagList " << std::endl;
		}

		void  getTest(Request &request, JsonResponse &response)
		{
			response["test"] = request.getUrl();
			std::cout << response.toStyledString() << std::endl;
		}

		void setup()
		{
		        	addRouteResponse("GET", "/getNoteById", NoteTagController, getNote, JsonResponse);
		        	addRouteResponse("GET", "/getNoteList", NoteTagController, getNoteList, JsonResponse);

		        	addRouteResponse("GET", "/getTagList", NoteTagController, getTagsList, JsonResponse);

		        	addRouteResponse("GET", "/getTree", NoteTagController, getTagTree, JsonResponse);

		        	addRouteResponse("POST", "/storeNote", NoteTagController, storeNote, JsonResponse);

		        	addRouteResponse("GET", "/getTest", NoteTagController, getTest, JsonResponse);





		}
};
}

#endif /* NOTETAGCONTROLLER_H_ */
