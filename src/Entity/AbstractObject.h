/*
 * AbstractObject.h
 *
 *  Created on: 11 апр. 2016 г.
 *      Author: lladmin
 */

#ifndef ENTITY_ABSTRACTOBJECT_H_
#define ENTITY_ABSTRACTOBJECT_H_

#include <ctime>
#include <iostream>
#include <memory>

typedef enum {
	TYPE_NOTE,
	TYPE_FILE
} ObjectType;

class AbstractObject {
	private:
		int64_t id;
		std::string name;
		std::string content;
		std::time_t created;
		std::time_t changed;
		std::string url;
		std::string hash;
		uint32_t views;
		ObjectType type;
	public:
		AbstractObject():
			id(0),
			created(std::time(nullptr)),
			changed(std::time(nullptr)),
			views(0),
			type(TYPE_NOTE)
		{

		}

		virtual ~AbstractObject(){ }

		const std::string& getName() const {
			return name;
		}

		void setName(const std::string& caption) {
			this->name = caption;
		}

		std::time_t getChanged() const {
			return changed;
		}

		void setChanged(std::time_t changed) {
			this->changed = changed;
		}

		const std::string& getContent() const {
			return content;
		}

		void setContent(const std::string& content) {
			this->content = content;
		}

		std::time_t getCreated() const {
			return created;
		}

		void setCreated(std::time_t created) {
			this->created = created;
		}

		ObjectType getType() const {
			return type;
		}

		void setType(ObjectType type) {
			this->type = type;
		}

		uint32_t getViews() const {
			return views;
		}

		void setViews(uint32_t views) {
			this->views = views;
		}

		int64_t getId() const {
			return id;
		}

		void setId(int64_t id) {
			this->id = id;
		}

		const std::string& getUrl() const {
			return url;
		}

		void setUrl(const std::string& url) {
			this->url = url;
		}

		const std::string& getHash() const {
			return hash;
		}

		void setHash(const std::string& hash) {
			this->hash = hash;
		}
};

typedef std::unique_ptr<AbstractObject> AbstractObjectPtr;

#endif /* ENTITY_ABSTRACTOBJECT_H_ */
