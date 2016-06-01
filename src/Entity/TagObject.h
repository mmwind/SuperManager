/*
 * TagObject.h
 *
 *  Created on: 2 июня 2016 г.
 *      Author: lladmin
 */

#ifndef ENTITY_TAGOBJECT_H_
#define ENTITY_TAGOBJECT_H_

class TagObject {
	private:
		int64_t id;
		std::string name;
		std::string content;
	public:
		TagObject():
			id(0),
			name(""),
			content("")
		{

		}
		virtual ~TagObject(){

		}

		const std::string& getContent() const {
			return content;
		}

		void setContent(const std::string& content) {
			this->content = content;
		}

		int64_t getId() const {
			return id;
		}

		void setId(int64_t id) {
			this->id = id;
		}

		const std::string& getName() const {
			return name;
		}

		void setName(const std::string& name) {
			this->name = name;
		}
};

#endif /* ENTITY_TAGOBJECT_H_ */
