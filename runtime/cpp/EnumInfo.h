#ifndef __EnumInfo_h__
#define __EnumInfo_h__

#include "Common.h"

/** ������enum��Ϣ. 
 * ��Ҫ�����������ڽ���id�����Ƶ�ת��.
 */
class EnumInfo
{
public:
	EnumInfo(const std::string& name, void (*initFunc)(EnumInfo* einfo)):
	name_(name)
	{
		initFunc(this);
	}

	/** ��һ���ַ���ת��Ϊ��Ӧ��enum item id.
		@param item enum item name.
		@return -1 ��ʾת��ʧ��.
	*/
	int getItemId(const std::string& item)
	{
		std::vector<std::string>::iterator r;
		r = std::find(items_.begin(), items_.end(), item);
		if(r == items_.end())
			return -1;
		return (int)(r - items_.begin());
	}

	/** ��һ��enum item idת��Ϊ�ַ�������.
		@param item enum item id.
		@return NULL ��ʾת��ʧ��.
	*/
	const char* getItemName(int item)
	{
		if(item < 0 || item >= (int)items_.size())
			return NULL;
		return items_[item].c_str();
	}

	std::string						name_;
	std::vector<std::string>		items_;
};

/** ������ȡһ��enum��������Ϣ�ĺ�. */
#define ENUM(E)	enum##E


#endif//__EnumInfo_h__
