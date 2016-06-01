/*
 * Note.h
 *
 *  Created on: 11 апр. 2016 г.
 *      Author: lladmin
 */

#ifndef ENTITY_NOTE_H_
#define ENTITY_NOTE_H_

#include "AbstractObject.h"

class Note: public AbstractObject {
	public:
		Note(){
			setType(TYPE_NOTE);
		}
};

#endif /* ENTITY_NOTE_H_ */
