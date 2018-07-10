
#ifndef __Struct_h__
#define __Struct_h__

#include <vector>

#include "Definition.h"
#include "Field.h"

/** Struct ����һ���û��Զ������������. 
	Struct ����һ�� c++ struct ���壬�ɳ�Ա�����б���ɡ�
	APRC_Struct ֧������.
*/
class Struct : 
	public Definition,
	public FieldContainer
{
public:
	Struct():
	Definition(),
	super_(NULL),
	skipComp_(false)
	{}
	Struct(const std::string& f, const std::string& n):
	Definition(f, n),
	super_(NULL),
	skipComp_(false)
	{}

	virtual Struct*	getStruct()	{ return this; }

	/** ����һ����Ա���Զ����Ƿ����. */
	bool findField( const std::string& name );

	/** ���struct field����. */
	size_t getFieldNum();

	Struct*				super_;		///< �̳���
	std::string					cppcode_;	///< c++ code.
	bool						skipComp_;	///< ����ѹ����ʽ.
};


#endif//__Struct_h__