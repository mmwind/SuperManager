/*
 * ConfigReader.h
 *
 *  Created on: 29 марта 2016 г.
 *      Author: llproger
 */

#ifndef UTILS_CONFIGMANAGER_H_
#define UTILS_CONFIGMANAGER_H_

#include "json/json.h"
#include <iostream>
#include <fstream>

class ConfigManager {
private:
	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	std::string error;

	// Options
	std::string webroot;
public:
	ConfigManager(){	}
	virtual ~ConfigManager(){ }
	int read(const std::string name = "conf.json"){
		try{
			std::ifstream test(name, std::ifstream::binary);
			bool parsingSuccessful = reader.parse( test, root, false );
			if ( !parsingSuccessful )
			{
				// report to the user the failure and their locations in the document.
				error = reader.getFormatedErrorMessages();
				std::cout << "[ConfigReader]: Error " <<  error << std::endl;
				return(-1);
			}
		} catch(std::exception &e){
			std::cout  << "[ConfigReader]: Error " << e.what() << std::endl;
			return(-1);
		}
	    webroot = root.get("webroot", "UTF-8" ).asString();
	    std::cout  << "[ConfigReader]: webroot = " << webroot << std::endl;

	    return(0);
	}

	const std::string& getError() const {
		return error;
	}

	const std::string& getWebroot() const {
		return webroot;
	}

	void setWebroot(const std::string& webroot) {
		this->webroot = webroot;
	}

	int write(const std::string name = "conf.json"){
		// todo replaced by dummy
		return(0);
	}
};


#endif /* UTILS_CONFIGMANAGER_H_ */
