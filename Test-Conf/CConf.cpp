#include "pch.h"
#include "CConf.h"
#include <Windows.h>
#include <filesystem>

void CConf::Conf::AddSubSection(CConf::CSection& masterSection, CConf::CSection slaveSection)
{
		std::vector<int> result;
		std::string  slave = slaveSection.GetName();
		std::string master = masterSection.GetName();
		if (slave.find(master) == 0)
		{
			//slave.erase(master.size());
			if (slave.at(0)==m_separator)
			{
				slave.erase(0);
			}
		}
		else if (master.find(slave) == 0)
		{
			master.erase(slave.size());
			//master.Delete(0, slave.GetLength());
		}
		std::string fullname = master;
		master += static_cast<wchar_t>(m_separator);
		master += slave;
		if (isHadSeparator(fullname, m_separator))
		{
			//TODO: 
			//AddDependentSec(fullname);
			this->m_settings.at(masterSection.GetName()).AddSection(slaveSection);
		}
		else
		{
			slaveSection.SetName(fullname);
			m_settings.emplace(fullname, slaveSection);
		}
}

void CConf::Conf::AddSubSection()
{
	auto it = m_settings.begin();
	for (int i = 0; it != m_settings.end(); it++, i++) {
		//TODO: 
		AddDependentSec(it->first);
	}
}

bool CConf::Conf::Write(std::string dstSectionSepOption, std::variant<bool, int, double, std::string> srcValue)
{
	std::vector<int> result;
	try
	{
		if (isHadSeparator(dstSectionSepOption, m_separator))
		{
			auto OptionName = GetOptionNameFrom(dstSectionSepOption);
			auto SectionName = GetSectionNameFrom(dstSectionSepOption);
			if(!Contain(SectionName))
			{ 
				AddSection(SectionName);
			}

			if (HasEntry(dstSectionSepOption))
			{
				this->SetOption(OptionName, srcValue);
				return true;
			}
			else
			{

				this->AddOption(SectionName, OptionName, srcValue);
				return true;
			}
		}
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error Write 48str"), a.what());
		m_error.AddData(res);
		return false;
	}
	return false;
}
bool CConf::Conf::Write(std::string dstSectionSepOption, std::string srcValue)
{
	try
	{
		std::variant<bool, int, double, std::string> buf = srcValue;
		auto result= CConf::Conf::Write(dstSectionSepOption, buf);
		CData::TOtype type = CData::TOtype::STRING;
		auto section = GetSectionNameFrom(dstSectionSepOption);
		auto option = GetOptionNameFrom(dstSectionSepOption);
		m_settings[section].SetTypeOfValue(option, type);
		return result;
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error Write 83str"), a.what());
		m_error.AddData(res);
		return false;
	}


	return false;
}

bool CConf::Conf::Write(std::string dstSectionSepOption, CData src)
{
	try
	{
		auto section = GetSectionNameFrom(dstSectionSepOption);
		auto option = GetOptionNameFrom(dstSectionSepOption);
		auto result = this->Write(dstSectionSepOption, src.GetValue());
		return result;
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error Write 83str"), a.what());
		m_error.AddData(res);
		return false;
	}


	return false;
}

bool CConf::Conf::Write(std::string dstSectionSepOption, bool srcValue)
{
	try
	{
		std::variant<bool, int, double, std::string> buf = srcValue;
		auto result = CConf::Conf::Write(dstSectionSepOption, buf);
		CData::TOtype type = CData::TOtype::BOOL;
		auto section = GetSectionNameFrom(dstSectionSepOption);
		auto option = GetOptionNameFrom(dstSectionSepOption);
		m_settings[section].SetTypeOfValue(option, type);
		return result;
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error Write 106str"), a.what());
		m_error.AddData(res);
		return false;
	}

	return false;
}
bool CConf::Conf::Write(std::string dstSectionSepOption, int srcValue)
{
	try
	{
		std::variant<bool, int, double, std::string> buf = srcValue;
		auto result = CConf::Conf::Write(dstSectionSepOption, buf);
		if (result) {
			CData::TOtype type = CData::TOtype::INT;
			auto section = GetSectionNameFrom(dstSectionSepOption);
			auto option = GetOptionNameFrom(dstSectionSepOption);
			m_settings[section].SetTypeOfValue(option, type);
		}

		return result;
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error Write 127str"), a.what());
		m_error.AddData(res);
		return false;
	}


	return false;
}
bool CConf::Conf::Write(std::string dstSectionSepOption, double srcValue)
{
	try
	{
		std::variant<bool, int, double, std::string> buf = srcValue;
		auto result = CConf::Conf::Write(dstSectionSepOption, buf);
		CData::TOtype type = CData::TOtype::DOUBLE;
		auto section = GetSectionNameFrom(dstSectionSepOption);
		auto option = GetOptionNameFrom(dstSectionSepOption);
		m_settings[section].SetTypeOfValue(option, type);
		return result;
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error Write 149str"), a.what());
		m_error.AddData(res);
		return false;
	}


	return false;
}

bool CConf::Conf::CheckExistSectionForThisOption(std::string NameOfDataWithSep) const
{
	std::vector<int> result;
	if (isHadSeparator(NameOfDataWithSep, m_separator))
	{
		std::string tempName = this->GetSectionNameFrom(NameOfDataWithSep);
		return (this->Contain(tempName));
		
	}


	return false;
}

void CConf::Conf::Rem(const CConf::CSection& section)
{
	Rem( section.GetName());
}

void CConf::Conf::Rem(const std::string nameOfSection)
{

	m_settings.erase(nameOfSection);
}

std::ostream& operator<<(std::ostream& out, const CData& source)
{
	using namespace CConf;
	auto value = source.GetValue();
	std::string res = std::visit(CData::make_string_functor(), value);
	return out << "Data[" << source.GetName() << "],[" << res<<"]";
}

bool operator==(const CData& lhs, const CData& rhs)
{
	if (lhs.GetName()== rhs.GetName() &&
		lhs.GetValue()== rhs.GetValue())
	{
		return true;
	}

	return false;
}



std::string CDataStorage::binary::GetData() const
{
	return m_data;
}

bool CDataStorage::binary::WriteData(std::string array)
{
	m_data = array;
	if (m_data == array)
	{
		return true;
	}
	else return false;
}

size_t CDataStorage::binary::size()
{
	return m_data.size();
}




CConf::CSection* CConf::Conf::GetSection(std::string name)
{
	try
	{
		return &m_settings[name];
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error Write 241str"), a.what());
		m_error.AddData(res);

	}
	return nullptr;
}

bool CConf::Conf::Contain(std::string nameOfSection)  const
{
	return m_settings.contains(nameOfSection);
}


//static
CConf::Conf CConf::Conf::CreateConf(typeOfContain type)
{
	CConf::Conf result;
	switch (type)
	{
	using enum CConf::Conf::typeOfContain;
	case client:
	{
		std::string secName = "[connection-to-source]";
		CConf::CSection section(secName.c_str());//AddSection type - connectinfo
		auto data1 = CData(std::string("PathINIFile"), std::string("C:\\ \\"));
		auto data2 = CData(std::string("NameINIFile"), std::string("config.txt"));
		auto data3 = CData(std::string("Type"), std::string("TXT"));
		section.AddData(data1);
		section.AddData(data2);
		section.AddData(data3);
		//m_name=_T("client");
		//TODO: добавить всё это в конфиг
	}
		break;
	case win:
	{
		//m_name = _T("win");
	}
		break;
	case win_client:
	{
		//m_name = _T("win_client");
	}
		break;
	case user:
	{
		//m_name = _T("user");
	}
		break;
	default:
		break;
	}
	return result;
}


size_t CConf::Conf::GetSectionsCount(const std::string name)
{
	return m_settings.at(name).GetSectCount();
}

bool CConf::Conf::IsSection(const std::string name)
{
	return Contain(name);
}

bool CConf::Conf::IsOption(const std::string name)
{
	return (HasEntry(name)? !IsSection(name):false);
}



bool CConf::Conf::AddSection(const CConf::CSection newSection)
{
	try
	{
		m_settings[newSection.GetName()] = newSection;
		return true;
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error Write 322str"), a.what());
		m_error.AddData(res);
		return false;
	}
	return false;
}

bool CConf::Conf::AddSection(const std::string name)
{

	try
	{
		CConf::CSection result(name.c_str());
		m_settings[name] = result;
		return true;
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error AddSection 338str"), a.what());
		m_error.AddData(res);
		return false;
	}
	return false;
}
/*
bool CConf::Conf::AddSectFromSection(CConf::CSection& section)
{
	try
	{
		for (size_t i = 0; i < section.m_sub.size(); i++)
		{
			this->AddSection(section.GetSubSection(i));
		}
	}
	catch (const std::exception&)
	{
	}
	return false;
}
*/
bool CConf::Conf::HasEntry(const std::string name) const
{
	if (this->Contain(name))
	{
		return true;
	}

	for (auto it = m_settings.begin(); it != m_settings.end(); it++) {
		auto temp = m_settings.at(it->first);
		if (temp.Contain(name))
		{
			return true;
		}
		//std::cout << "{" << it->first << ", " << it->second << "}" << std::endl;
	}
	return false;
}

void CConf::Conf::SetOption(const std::string name, std::variant<bool, int, double, std::string> value)
{
	if (m_settings.contains(name))
	{
		m_settings.at(name).Write(name, value);
	}
	//TODO: проверить целесообразность наличия этого метода
}

size_t CConf::Conf::Size() const
{
	return m_settings.size();
}

size_t CConf::Conf::GetOptionsCount(const std::string name)
{
	return m_settings.at(name).GetOptionsCount();
}


void CConf::Conf::AddOption(std::string section, std::string option, std::variant<bool, int, double, std::string> value)
{
	//m_settings.insert(std::pair<std::string, CSection>(dependSection, value));
	m_settings[section].Write(option, value);
}

void CConf::Conf::AddDependentSec(std::string name)
{
	try
	{
		std::vector<int> locationSeparatorInString;
		auto words = base::GetArrayFrom(name);
		auto nameDepSect = GetDependedArray(words);
		for (int i = 0; i < nameDepSect.size(); i++)
		{
			auto sectionName = nameDepSect.at(i).c_str();
			if (!HasEntry(sectionName))
			{
				AddSection(sectionName);
			}
		}
	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error AddDependentSec 415str"), a.what());
		m_error.AddData(res);
	}

}

std::vector<std::string> CConf::Conf::GetDependedArray(std::vector<std::string> base) const
{
	std::vector<std::string> res;
	std::string temporary;
	for (size_t i = 0; i < base.size(); i++)
	{
		if (temporary.size()!=0)
		{
			temporary += m_separator;
		}
		temporary += base[i];
		res.push_back(temporary);
	}
	return res;
}

std::vector<std::string> CConf::Conf::GetDependedArray(std::string name) const
{
	auto res = base::GetArrayFrom(name);
	return GetDependedArray(res);
}


std::string CConf::Conf::GetOptionNameFrom(std::string name) const
{
	std::string res;
	size_t prev = name.size();
	size_t next= prev-1;
	while (name.at(next)!=m_separator)
	{
		res= name.substr(next, prev);
		--next;
	}
	return res;
}


std::string CConf::Conf::GetSectionNameFrom(std::string name) const
{
	auto arr = GetDependedArray(name);
	return arr.at(arr.size()-2);
}

//static 

std::unique_ptr<CConf::Conf> CConf::CreateConf()
{
	auto conf = std::make_unique<CConf::Conf>();
	conf->AddSection(CConf::CSection("Window"));
	conf->AddSection(CConf::CSection("Source"));
	return conf;
}
