#include "CSection.h"
#include <iostream>
#define ERROR404 CData(std::string("Error"), std::string ("this option did'nt exist"))
#define NAMEOFSSDEFAULT std::string("__system_nameOfSection-default-name2023")

void CConf::CSection::Write(std::string NameOfData, std::variant<bool, int, double,  std::string> value)
{
	CData buf;
	buf.SetData(NameOfData, value);
	m_Data[NameOfData] = buf;
}

void CConf::CSection::Write(CData value)
{
	m_Data[value.GetName()] = value;
}

bool CConf::CSection::Contain(const std::string nameOfData) const
{
	if (isHadSeparator(nameOfData, m_separator)) 
	{
		//auto temp = base::GetOptionNameFrom(nameOfData, m_separator);
		if (m_Data.contains(nameOfData)) {
			return true;
		};
	
	
	}
	if (m_Data.contains(nameOfData)) {
		return true;
	};
	return false;
}

bool CConf::CSection::ContainExt(const std::string nameOfCData) const
{
	for (auto& item : m_sub)
	{
		if (item.first== nameOfCData||item.second.contains(nameOfCData))
		{
			return true;
		}
	}
	return false;
}

//static
bool CConf::CSection::Contain(CSection& src, const std::string nameOfData)
{
	try
	{	
		if (src.Contain(nameOfData)) {
			return true;
		};
	}
	catch (const std::exception& a)
	{
	}
	
	return false;
}

bool CConf::CSection::isHadSubSect()
{
	return m_sub.size() == 0 ? false : true;
}

size_t CConf::CSection::GetOptionsCount() const
{
	return m_Data.size();
}



void CConf::CSection::Empty()
{
	m_name = "";
	m_Data.clear();
}

bool CConf::CSection::HasEntry(const std::string name) const
{
	if (m_Data.contains(name)||m_sub.contains(name))
	{
		return true;
	}
	return false;
}

bool CConf::CSection::Read(std::string from, OUT bool& where)
{

	try
	{
		if (!isHadSeparator(from, m_separator))
		{
			if (this->Contain(from))
			{
				std::string res = std::visit(CData::make_string_functor(), this->GetData(from).GetValue());
				if (res.c_str() == std::string("true"))
				{
					where = true;
				}
				else
				{
					where = false;
				}

				return true;
			}
			std::string buf;
			buf+=(m_separator);
			buf = this->GetName().c_str() + buf;
			from = buf + from;
		}
		if (this->Contain(from))
		{
			std::string res = std::visit(CData::make_string_functor(), this->GetData(from).GetValue());
			if (res.c_str()== std::string("true"))
			{
				where = true;
			}
			else 
			{
				where = false;
			}
			
			return true;
		}		
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error"), a.what());
		m_error.SetData(res);
		return false;
	}
	return false;
}

bool CConf::CSection::Read(std::string from, OUT int& where)
{
	try
	{
		std::vector<int> PosSep;
		if (!isHadSeparator(from, m_separator))
		{
			if (this->Contain(from))
			{
				std::string res = std::visit(CData::make_string_functor(), this->GetData(from).GetValue());
				where = atoi(res.c_str());
				return true;
			}
			std::string buf;
			buf += (m_separator);
			buf = this->GetName().c_str() + buf;
			from = buf + from;
		}

		if (this->Contain(from))
		{
			std::string res = std::visit(CData::make_string_functor(), this->GetData(from).GetValue());
			where = atoi(res.c_str());
			return true;
		}
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error"), a.what());
		m_error.SetData(res);
		return false;
	}
	return false;
}

bool CConf::CSection::Read(std::string from, OUT double& where)
{

	try
	{
		std::vector<int> PosSep;
		if (!isHadSeparator(from, m_separator))
		{
			if (this->Contain(from))
			{
				std::string res = std::visit(CData::make_string_functor(), this->GetData(from).GetValue());
				where = atof(res.c_str());
				return true;
			}
			std::string buf;
			buf += (m_separator);
			buf = this->GetName().c_str() + buf;
			from = buf + from;
		}
		if (this->Contain(from))
		{
			std::string res = std::visit(CData::make_string_functor(), this->GetData(from).GetValue());
			where = atof(res.c_str());
			return true;
		}
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error"), a.what());
		m_error.SetData(res);
		return false;
	}
	return false;
}

//может быть, стоит переписать CConf::CSection::Read(), ибо структура без const std::string from довольно запутанно выглядит
bool CConf::CSection::Read(std::string from, OUT std::string& where)
{
	try
	{
		if (!isHadSeparator(from, m_separator))
		{
			if (this->Contain(from))
			{
				std::string res = std::visit(CData::make_string_functor(), this->GetData(from).GetValue());
				where = res.c_str();
				return true;
			}
			std::string buf;
			buf += (m_separator);
			buf = this->GetName().c_str() + buf;
			from = buf + from;
		}
		if (this->Contain(from))
		{
			std::string res = std::visit(CData::make_string_functor(), this->GetData(from).GetValue());
			where = res.c_str();
			return true;
		}
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error"), a.what());
		m_error.SetData(res);
		return false;
	}
	return false;
}

size_t CConf::CSection::GetSectCount() const
{
	return m_sub.size();
}

size_t CConf::CSection::size() const
{
	return m_Data.size();
}

void CConf::CSection::SetTypeOfValue(std::string name, CData::TOtype type)
{
	m_Data[name].SetValue(type);
}



CConf::CSection CConf::CSection::GetSubSection(std::string name)
{
	CConf::CSection res;
	if (HasEntry(name))
	{
		auto what = m_sub.at(name);
		res.SetName(name);
		for (auto& item : what)
		{
			res.Write(item.second);
		}
		return res;
	}

	return CConf::CSection(ERROR404.GetName(), ERROR404);
}

CData CConf::CSection::GetData(std::string name) const
{
	if (m_Data.contains(name)) { return m_Data.at(name); }
	return ERROR404;
}


void CConf::CSection::AddData(CData& value)
{
	m_Data[value.GetName()] = value;
}

void CConf::CSection::AddSection(CSection& section) 
{
	m_sub[section.GetName()];
	for (auto& item : section.m_Data)
	{
		m_sub[section.GetName()].insert(std::pair<std::string, CData>(item.first, item.second));
	}
}

void CConf::CSection::AddSection(std::string name) 
{
	auto constructSection = CSection(name);
	AddSection(constructSection);
}

bool CConf::operator==(const CSection& rhs, const CSection& lhs)
{
	if (rhs.GetName()== lhs.GetName()&& rhs.GetSectCount() == lhs.GetSectCount()) 
	{
		for (auto it1 = rhs.m_Data.begin(), it2 = lhs.m_Data.begin(); it1 != rhs.m_Data.end(); ++it1, ++it2){
			if (it1->second != it2->second)
				return false;
		}
		if (rhs.GetSectCount() != 0) {
			for (auto it1 = rhs.m_sub.begin(), it2 = lhs.m_sub.begin(); it1 != rhs.m_sub.end(); ++it1, ++it2) {
				if (it1->second != it2->second)
					return false;
			}


		}
		return true;
	}
	// TODO: вставьте здесь оператор return
	return false;
}
