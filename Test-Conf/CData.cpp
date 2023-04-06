#include "CData.h"
#include <iterator>

std::string CData::GetName() const
{
	return m_name;
}

std::variant<bool, int, double, std::string> CData::GetValue() const
{
	return m_value;
}

CData CData::GetData() const
{
	return CData(m_name, m_value);
}

unsigned char CData::GetSeparator()
{
	return m_separator;
}

//static
void CData::Copy(const CData& src, CData& dst)
{
	dst = src;
}

//static
void CData::Swap(CData& src, CData& dst)
{
	CData temp;
	temp.SetData(src);
	src.SetData(dst);
	dst.SetData(temp);
}

void CData::SetData(CData& src)
{
	m_name = src.GetName();
	m_value = src.GetValue();
}

void CData::SetData(std::string name, std::variant<bool, int, double, std::string>& value)
{
	m_name = name;
	m_value = value;
}

void CData::SetValue(CData::TOtype type)
{
	m_whatTheValue = type;
}

void CData::SetData(std::variant<bool, int, double,std::string>& value)
{
	m_value = value;
}

void CData::SetSeparator(unsigned char value)
{
	m_separator = value;
}

CData::TOtype CData::ViewTypeOfValue() const
{//BOOL = 1, INT, DOUBLE, CSTRING, BYTE
	switch (m_value.index())
	{
		using enum CData::TOtype;
	case 0:
	{
		return BOOL;
	}
	break;
	case 1:
	{
		return INT;
	}
	break;
	case 2:
	{
		return DOUBLE;
	}
	break;
	case 3:
	{
		return STRING;
	}
	break;
	case 4:
	{
		return BYTE;
	}
	break;
	default:
	{
		return CORRUPT;
	}
	break;
	}
}

inline std::string base::GetName() const
{
	return m_name;
}

void base::SetName(std::string name)
{
	m_name = name;
}
//                      or wchar_t separator
bool base::FindSeparator(OUT unsigned char& separator) const
{
	return FindSeparator(m_name, separator);
}
std::vector<std::string> base::GetArrayFrom(std::string base) const
{
	std::vector<std::string> res;
	size_t prev = 0;
	size_t next;
	size_t delta = 1;

	while ((next = base.find(m_separator, prev)) != std::string::npos) {
		std::string tmp = base.substr(prev, next - prev);
		res.push_back(base.substr(prev, next - prev));
		prev = next + delta;
	}
	res.push_back(base.substr(prev));
	return res;
}

//static
bool base::isHadSeparator(std::string name, unsigned char separator)
{
	//std::string (separator)
	auto found = name.find(separator);
	return found != std::string::npos;
	
}

bool base::FindSeparator(std::string name, unsigned char& separator)
{
	auto buf = name.c_str();
	std::vector<wchar_t> longbuff;
	for (int i = 1; i < name.size(); i++)
	{
		auto getS = buf[i];
		//ascii table
		if (getS > 32 && getS < 48 || getS > 57 && getS < 65 || getS > 90 && getS < 97 || getS > 122 && getS < 127)
		{
			longbuff.push_back(getS); //если нужен первый попавшийся разделитель, то выйти здесь, иначе найти самый популярный 
		}
	}

	if (longbuff.size() > 1)
	{
		separator = static_cast<unsigned char>(FindFamousChar(longbuff));
		return true;
	}
	else
	{
		separator = static_cast<unsigned char>(longbuff.at(0));
		return true;
	}
	return false;
}

//static 
bool base::ContainChar(wchar_t char4find, std::vector<wchar_t> arrayChar)
{
	
	for (int i = 0; i < arrayChar.size(); i++)
	{
		if (arrayChar.at(i)== char4find)
		{
			return true;
		}
	}
	return false;
}
/*
std::string base::ToSTL(CString value)
{
	CT2CA transform(value);
	std::string result(transform);
	return result;
}

CString base::ToMFC(std::string value)
{
	CString transform(value.c_str());
	return transform;
}
*/
//static
std::string base::GetOptionNameFrom(std::string name, unsigned char separator)
{
	std::string res;
	size_t prev = name.size();
	size_t next = prev - 1;
	while (name.at(next) != separator)
	{
		res = name.substr(next, prev);
		--next;
	}
	return res;
}

//static
wchar_t  base::FindFamousChar(const std::vector<wchar_t>& resource)
{
	std::vector<wchar_t> uniqueChar;

	//нашли уникальные символы:
	for (int i = 0; i < resource.size(); i++)
	{
		if (!ContainChar(resource.at(i), uniqueChar))
		{
			uniqueChar.push_back(resource.at(i));
		}
	}
	//вектор содержит столько же значений, что и uniqueChar:
	std::vector<int> count;
	//инициализация размера вектора:
	for (int i = 0; i < uniqueChar.size(); i++)
	{
		count.push_back(0);
	}
	for (int i = 0; i < resource.size(); i++)
	{
		for (int j = 0; j < uniqueChar.size(); j++)
		{
			if (resource.at(i)== uniqueChar.at(j))
			{
				++count.at(j);
			}
		}
	}
	int maxim(0); wchar_t res(' ');
	for (size_t i = 0; i < count.size(); i++)
	{
		if (count.at(i)>maxim)
		{
			maxim = count.at(i);
			res = uniqueChar.at(i);
		}
	}
	return res;
}