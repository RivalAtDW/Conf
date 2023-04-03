#include "CSource.h"
#include "CSection.h"
#include <fstream> 
#include <string> 
#include <sstream>

using CConf::Conf;

CConf::TemplateSource::~TemplateSource()
{
}

bool CConf::TemplateSource::SaveConf(const CConf::CSection& SourceInfo)
{
    return false;
}

CConf::Conf CConf::TemplateSource::LoadConf(const CConf::CSection& SourceInfo)
{
	return CConf::Conf();
}



CConf::CSource::CSource()
{
}

bool CConf::CSource::Init()
{
	try
	{
		switch (m_type)
		{
		case CConf::CSource::type::TXT:
		{
			std::variant<bool, int, double, std::string > localvalue;
			SPosition i;
			std::string localname = "PathTXTFile";
			std::string res = std::visit(CData::make_string_functor(), localvalue);
			if (isDirectoryExists(res.c_str()))
			{
				m_error.Empty();
				return true;
			}
			else
			{
				//m_error.Write("", "Error: this directory didn't exist");
			}
		}
		break;
		case CConf::CSource::type::JSON:
			break;
		case CConf::CSource::type::XML:
			break;
		case CConf::CSource::type::WINREG:
			break;
		case CConf::CSource::type::INI:
			break;
		case CConf::CSource::type::DB:

			break;
		default:
			break;
		}
		return false;

	}
	catch (const std::exception& a)
	{
		auto res = CData(std::string("Error"), a.what());
		m_error.AddData(res);
		return false;
	}

	return true;
}
CConf::Conf CConf::CSource::Load(CConf::CSection* ConnectInfo)
{
	CString wantedSectionName(_T("[connection-to-source]"));
	CConf::Conf result;
	auto enumType = std::visit(CData::make_type_functor(), ConnectInfo->GetData("Type").GetValue());
	auto concreteType = static_cast<CConf::CSource::type>(enumType);
	switch (concreteType)
	{
		using enum CConf::CSource::type;
	case TXT:
	{  
		CConf::CSection& ref = *ConnectInfo;
		result = m_funcTXT.LoadConf(ref);
	}
	break;
	case JSON:
	{
		CConf::CSection& ref = *ConnectInfo;
		result = m_funcJSON.LoadConf(ref);
	}
	break;
	case XML:
	{
		CConf::CSection& ref = *ConnectInfo;
		result = m_funcXML.LoadConf(ref);
	}
	break;
	case WINREG:
	{
		CConf::CSection& ref = *ConnectInfo;
		result = m_funcWINREG.LoadConf(ref);
	}
	break;
	case INI:
	{
		CConf::CSection& ref = *ConnectInfo;
		result = m_funcINI.LoadConf(ref);
	}
	break;
	case DB:
	{
		CConf::CSection& ref = *ConnectInfo;
		result = m_funcDB.LoadConf(ref);

	}
	break;
	default:
		break;
	}


	//switch data
	//check connection\available res
	//load to conf from src


	return result;
}

CConf::CSource::implem::TXT::TXT(const CString path, const CString name)
{
}

CConf::CSource::implem::TXT::TXT(const CConf::CSection& SettingsOfConnectToSource)
{
}



bool CConf::CSource::implem::TXT::SaveConf(const CConf::CSection& SourceInfo)
{
	return false;
}


CConf::Conf CConf::CSource::implem::TXT::LoadConf(const CConf::CSection& SourceInfo)
{

	CConf::Conf result;
	size_t indexOfPath(0);
	size_t indexOfFileName(0);
	std::string path("PathTXTFile");//_T("C:\\ \\")
	std::string fileName("NameTXTFile");//config.txt
	//SourceInfo.Contain(path, indexOfPath);
	
	if (SourceInfo.Contain(path) && SourceInfo.Contain(fileName))
	{
		auto a = SourceInfo.GetData(path).GetValue();
		std::string whatPath = std::visit(CData::make_string_functor(), a);
		std::string whatFileName = std::visit(CData::make_string_functor(), SourceInfo.GetData(fileName).GetValue());
		whatPath += whatFileName;
		if (isFileExists(whatPath))
		{
			std::ifstream file(whatPath);
			try
			{
				std::string lastnameOfSec("");
				std::string s; 
				while (getline(file, s)) { 
					
					if (TXTisSectionName(s)&&
						!TXTisOption(s)) 
					{
						lastnameOfSec = s;
						//TODO:
						result.AddSection(lastnameOfSec);
					}
					else if (!TXTisSectionName(s) && TXTisOption(s))
					{
						auto positionEqual = s.find(m_optionSep,0);
						s.at(positionEqual) = ' ';
						std::istringstream ist(s);
						std::vector<std::string> words;
						std::string tmp;
						while (ist >> tmp)
							words.push_back(tmp);
						//TODO: в любом случае будет=2, неверно
						if (words.size()==2)
						{
							CData newData(words.at(0), words.at(1));
							auto separator = newData.GetSeparator();

							std::string longString(lastnameOfSec);
							longString +=separator;
							longString += words.at(0).c_str();
							result.Write(longString, newData);
						}
					
					}


				}

				file.close();
				return result;
			}
			catch (...) {}
		}
	};
	



	return CConf::Conf();
}

bool CConf::CSource::implem::TXT::AddSection(const CString NameOfSection, int CountOfParam)
{
	return false;
}

bool CConf::CSource::implem::TXT::DelSection(const CString NameOfSection)
{
	return false;
}

bool CConf::CSource::implem::TXT::AddParam(const CString NameOfSection, CString NameOfParam)
{
	return false;
}

bool CConf::CSource::implem::TXT::DelParam(const CString NameOfSection, CString NameOfParam)
{
	return false;
}

CConf::CSource::implem::JSON::JSON(CString path, CString name)
{
}



bool CConf::CSource::implem::JSON::SaveConf(const CConf::CSection& SourceInfo)
{
	return false;
}

CConf::Conf CConf::CSource::implem::JSON::LoadConf(const CConf::CSection& SourceInfo)
{
	return CConf::Conf();
}

bool CConf::CSource::implem::JSON::AddSection(const CString NameOfSection, int CountOfParam)
{
	return false;
}

bool CConf::CSource::implem::JSON::DelSection(const CString NameOfSection)
{
	return false;
}

bool CConf::CSource::implem::JSON::AddParam(const CString NameOfSection, CString NameOfParam)
{
	return false;
}

bool CConf::CSource::implem::JSON::DelParam(const CString NameOfSection, CString NameOfParam)
{
	return false;
}

CConf::CSource::implem::XML::XML(CString path, CString name)
{
}



bool CConf::CSource::implem::XML::SaveConf(const CConf::CSection& SourceInfo)
{
	return false;
}

CConf::Conf CConf::CSource::implem::XML::LoadConf(const CConf::CSection& SourceInfo)
{
	return CConf::Conf();
}

bool CConf::CSource::implem::XML::AddSection(const CString NameOfSection, int CountOfParam)
{
	return false;
}

bool CConf::CSource::implem::XML::DelSection(const CString NameOfSection)
{
	return false;
}

bool CConf::CSource::implem::XML::AddParam(const CString NameOfSection, CString NameOfParam)
{
	return false;
}

bool CConf::CSource::implem::XML::DelParam(const CString NameOfSection, CString NameOfParam)
{
	return false;
}

bool CConf::CSource::implem::DB::DelParam(const CString NameOfSection, CString NameOfParam)
{
	return false;
}

CConf::CSource::implem::WINREG::WINREG(CString path, CString name)
{
}


bool CConf::CSource::implem::WINREG::SaveConf(const CConf::CSection& SourceInfo)
{
	return false;
}

CConf::Conf CConf::CSource::implem::WINREG::LoadConf(const CConf::CSection& SourceInfo)
{
	return CConf::Conf();
}

bool CConf::CSource::implem::WINREG::AddSection(const CString NameOfSection, int CountOfParam)
{
	return false;
}

bool CConf::CSource::implem::WINREG::DelSection(const CString NameOfSection)
{
	return false;
}

bool CConf::CSource::implem::WINREG::AddParam(const CString NameOfSection, CString NameOfParam)
{
	return false;
}

bool CConf::CSource::implem::WINREG::DelParam(const CString NameOfSection, CString NameOfParam)
{
	return false;
}

CConf::CSource::implem::INI::INI(CString path, CString name)
{
}



bool CConf::CSource::implem::INI::SaveConf(const CConf::CSection& SourceInfo)
{
	return false;
}

CConf::Conf CConf::CSource::implem::INI::LoadConf(const CConf::CSection& SourceInfo)
{
	return CConf::Conf();
}

bool CConf::CSource::implem::INI::AddSection(const CString NameOfSection, int CountOfParam)
{
	return false;
}

bool CConf::CSource::implem::INI::DelSection(const CString NameOfSection)
{
	return false;
}

bool CConf::CSource::implem::INI::AddParam(const CString NameOfSection, CString NameOfParam)
{
	return false;
}

bool CConf::CSource::implem::INI::DelParam(const CString NameOfSection, CString NameOfParam)
{
	return false;
}

CConf::CSource::implem::DB::DB(CString server, CString db)
{
}



bool CConf::CSource::implem::DB::SaveConf(const CConf::CSection& SourceInfo)
{
	return false;
}

CConf::Conf CConf::CSource::implem::DB::LoadConf(const CConf::CSection& SourceInfo)
{
	return CConf::Conf();
}

bool CConf::CSource::implem::DB::AddSection(const CString NameOfSection, int CountOfParam)
{
	return false;
}

bool CConf::CSource::implem::DB::DelSection(const CString NameOfSection)
{
	return false;
}

bool CConf::CSource::implem::DB::AddParam(const CString NameOfSection, CString NameOfParam)
{
	return false;
}
//static 
bool CConf::CSource::TXTisSectionName(std::string name)
{
	unsigned char separator1 = '[';
	unsigned char separator2 = ']';
	CString temp = name.c_str();
	if (temp.Find(separator1,0)==0&& temp.Find(separator2, 0)== (temp.GetLength()-1))
	{
		return true;
	}

	return false;
}

bool CConf::CSource::TXTisOption(std::string name)
{
	unsigned char separator1 = '=';
	if (CConf::Conf::isHadSeparator(name, separator1))
	{
		return true;
	}

	return false;
}
