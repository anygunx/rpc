#ifndef __Compiler_h__
#define __Compiler_h__
#include <cstring>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <string>

namespace String
{
	static void Trim(std::string& str, bool left, bool right)
	{
		static const std::string delims = " \t\r\n";
		if (right)
			str.erase(str.find_last_not_of(delims) + 1); // trim right
		if (left)
			str.erase(0, str.find_first_not_of(delims)); // trim left
	}

	static void ToLowerCase(std::string& str)
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			tolower);
	}

	static void ToUpperCase(std::string& str)
	{
		std::transform(
			str.begin(),
			str.end(),
			str.begin(),
			toupper);
	}

	static std::vector<std::string > Split(std::string const &str, std::string const&delims, unsigned maxSplits = 0)
	{
		std::vector<std::string> ret;
		ret.reserve(maxSplits ? maxSplits + 1 : 1000);

		unsigned int numSplits = 0;
		size_t start, pos;
		start = 0;
		do
		{
			pos = str.find_first_of(delims, start);
			if (pos == start)
			{
				start = pos + 1;
			}
			else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
			{
				ret.push_back(str.substr(start));
				break;
			}
			else
			{
				ret.push_back(str.substr(start, pos - start));
				start = pos + 1;
			}
			start = str.find_first_not_of(delims, start);
			++numSplits;

		} while (pos != std::string::npos);
		return ret;
	}

	static void Split(const char* line, char delims, std::vector<std::string>& out, bool skip = true)
	{
		size_t i = 0, p = 0, s = strlen(line);
		char word[1024] = { '\0' };
		if (0 == s)
			return;
		do
		{
			if (line[i] == delims)
			{
				word[p] = '\0';
				out.push_back(word);

				if (!skip)
				{
					word[0] = delims;
					word[1] = '\0';
					out.push_back(word);
				}

				p = 0;
			}
			else
			{
				word[p++] = line[i];
			}
			++i;
		} while (i < s);

		word[p] = '\0';
		out.push_back(word);
	}

	static std::string	Join(std::vector< std::string > arr, std::string const&delims)
	{
		std::string ret = "";
		if (arr.empty())
			return ret;

		for (size_t i = 0, len = arr.size(); i < len; ++i)
		{
			ret += arr[i];
			if (1 + i != len)
			{
				ret += delims;
			}
		}

		return ret;
	}

	static int IndexOf(std::string const& str, char ch)
	{
		size_t i = str.find_first_of(ch);
		if (i == std::string::npos)
			return -1;
		return i;
	}
}

namespace File
{
	static std::string GetParentPath(const std::string fn)
	{
		if (fn.empty())
			return "./";

		size_t idx = fn.find_last_of('/');
		if (idx != std::string::npos)
		{
			return fn.substr(0, idx);
		}
		else
		{
			return "./";
		}
	}

	static std::string GetFileName(const std::string& fn)
	{
		if (fn.empty())
			return "";

		std::string _fn = fn;
		if (_fn.find_last_of('/') != std::string::npos)
		{
			std::vector<std::string> pathpart = String::Split(_fn, "/");
			_fn = pathpart.back();
		}
		return _fn;
	}

	static std::string GetFileExt(const std::string& fn)
	{
		if (fn.empty())
			return "";

		std::string _fn = fn;

		if (_fn.find_last_of('.') != std::string::npos)
		{
			std::vector<std::string> parts = String::Split(_fn, ".");
			return parts.back();
		}
		else
			return "";
	}

	static std::string GetFileBaseName(const std::string& fn)
	{
		if (fn.empty())
			return "";

		std::string _fn = fn;
		if (_fn.find_last_of('/') != std::string::npos)
		{
			std::vector<std::string> pathpart = String::Split(_fn, "/");
			_fn = pathpart.back();
		}
		if (_fn.find_last_of('.') != std::string::npos)
		{
			std::vector<std::string> part = String::Split(_fn, ".");
			part.pop_back();
			return part.back();
		}
		else
		{
			return _fn;
		}
	}
}

#include "Definition.h"
#include "Field.h"
#include "Enum.h"
#include "Struct.h"
#include "Service.h"

/** arpc���Ա�������.
	Compiler ͨ��flex��bison��Դ�ļ����з�������������arpc�﷨�淶�ĳ����﷨
	����Ȼ�����ɶ�Ӧ�Ĵ����ļ�.
*/
class Generator;
class Compiler
{
public:
	/** ���ȫ������. */
	static Compiler& inst();

	Compiler();
	~Compiler();

	/** ����Դ�ļ�. */
	int compile();

	/** �������Ʋ���һ������. 
	*/
	Definition* findDefinition( const std::string& name );

	/** ����������. */
	void outputError(const char* e, ...);
	/** �����ǰ�ļ��б������. */
	void outputErrorFL(const char* e, ...);

	/** @name ������Ϣ. */
	//@{
	/** Դ�ļ�����·������. */
	std::string						inputFileName_;
	/** ���ɴ����ļ���Ŀ¼. */
	std::string						outputDir_;
	/** ����Ѱ��import�ļ���Ŀ¼. */
	std::vector<std::string>		importPaths_;
	/** �������������� .*/
	std::string						generator_;
	/** ��ǰ����ĸ��ļ�����. */
	std::string						filename_;
	/** ��ǰ������ļ���stem. */
	std::string						fileStem_;
	//@}

	/** ���ļ�ֱ�Ӱ������ļ�����. */
	std::vector<std::string>		imports_;
	/** �����Ѿ������������ļ����ƣ���ֹ�ظ�����. */
	std::set<std::string>			importedFiles_;
	/** ��ǰ���������ļ�����. */
	std::string						curFilename_;
	/** ��ǰ�������ļ����к�. */
	int								curLineno_;
	/** ��ʼ����cppcode. */
	std::string						cppcode_;
	/** ���Դ�ļ�����������arpc����. */
	std::vector<Definition*>	definitions_;
	// ��ǰ����״̬.
	Enum						curEnum_;
	Struct						curStruct_;
	Service					curService_;
	Method						curMethod_;
	Field						curField_;
};

#endif//__Compiler_h__