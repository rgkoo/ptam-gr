
#ifndef _include_ODEGeomData_hpp_
#define _include_ODEGeomData_hpp_ 


class ODEGeomData {
public:
	enum Type {
		Unknown,
		PlayerHead,
		PlayerFeet,
		Finish,
		LineOfSight
	};
	
	ODEGeomData(void) : type(Unknown) {}

	void setType(int type)			{ this->type = type;		}
	void setOwner(void *owner)		{ this->owner = owner;		}

	int getType(void)				{ return type;				}
	void *getOwner(void)			{ return owner;				}
protected:
	int type;
	void *owner;
};


#endif //_include_ODEGeomData_hpp_
