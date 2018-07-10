#ifndef __Service_h__
#define __Service_h__

#include "Definition.h"
#include "Field.h"

/** Method ��ʾһ�� service method ���壬����һ��c function declaration. */
class Method : public FieldContainer
{
public:
	Method()
	{}

	const std::string& getName()	{ return name_; }
	const char* getNameC()			{ return name_.c_str(); }
	void setName(const std::string& n) { name_ = n; }
private:
	std::string				name_;				///< ��������.
};


class Service : public Definition
{
public:
	Service():
	Definition(),
	super_(NULL)
	{}
	Service(const std::string&f, const std::string& n):
	Definition(f, n),
	super_(NULL)
	{}

	virtual Service* getService()	{ return this; }

	/** ����һ�������Ƿ����. */
	bool findMethod( const std::string& name );

	/** ���Լ������и�������˳������һ������. */
	void getParents(std::vector<Service*>& parents);

	/** ��÷�������. */
	size_t getMethodNum();

	/** �������method */
	void getAllMethods(std::vector<Method*>& methods);

	Service*				super_;
	std::vector<Method>	methods_;
};


#endif//__Service_h__