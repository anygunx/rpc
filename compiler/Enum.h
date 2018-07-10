#ifndef __Enum_h__
#define __Enum_h__

#include <vector>
#include "Definition.h"
#include "Field.h"

/** ö�ٶ�����.
	Enum ��ʾһ����c++ enum���Ƶ�ö�ٶ��塣
*/
class Enum : public Definition
{
public:
	Enum():Definition(){}
	Enum(const std::string& f, const std::string& n)
		:Definition(f, n)
	{
		super_.setType(FT_INT32);
	}

	/** ����һ��ö�����Ƿ����.
	*/
	bool findItem( const std::string& item );
	Field &getSuperType(){return super_;}
	virtual Enum* getEnum() { return this; }

	Field						super_;
	std::vector< std::string >	items_;	///< ö�����б�.
};


#endif//__Enum_h__