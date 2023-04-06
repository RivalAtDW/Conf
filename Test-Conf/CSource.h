#pragma once
//#    define _AFXDLL
#include "CConf.h"
//#include <afxcoll.h>
#include <variant>
#include <string>
#include <fstream>
#    define OUT

namespace CConf
{
	//����������� ������� ����� ��� ����������� ��������� �����������
	class TemplateSource abstract
	{
	public:
		TemplateSource() = default;
		virtual ~TemplateSource();
	public:
		
		//����������� ����� ������ �������������� ��� �������
		//������ ������� ��������� ������ � ����������� �� �������
		//virtual std::vector<std::vector<std::string>> ConfLoad();// const = 0;
		//��������� ������ � ������ � ����������� �� ���� ctype
		//���� ������� �����, ����� ���������, ���� �� � ���������� ��������� ��� ����������� \ ���� �������
		//����� - �������� ������������ � ����� � ����������� �� ��������� 
		//��� � ����������� �������� ������
		//��� ��������� ���� ������
		virtual bool SaveConf(const CSection& SourceInfo);// = 0;
		//load from source
		virtual CConf::Conf LoadConf(const CSection& SourceInfo);// = 0;
		virtual bool AddSection(const std::string NameOfSection, int CountOfParam) = 0;
		virtual bool DelSection(const std::string NameOfSection) = 0;
		virtual bool AddParam(const std::string NameOfSection, std::string  NameOfParam) = 0;
		virtual bool DelParam(const std::string NameOfSection, std::string  NameOfParam) = 0;

	private:
		//������ ��������� ������� - [������ ������ �����-��, �� ������� ������ �����]
		std::vector<std::string> m_state;
	};
	
	class CSource
	{
	public:
		enum class type : int8_t
		{//: ini, json, xml, ������ windows, ���� ��� ����� � "����" �������, ����(�) ������,
			TXT = 1, JSON, XML, WINREG, INI, DB
		};
		class implem
		{
		public:
			
			class TXT : public TemplateSource
			{
			public:

				TXT() :TemplateSource() { };
				TXT(const std::string path, const std::string name);
				explicit TXT(const CConf::CSection& SettingsOfConnectToSource);

				bool SaveConf(const CConf::CSection& SourceInfo) override;
				CConf::Conf LoadConf(const CConf::CSection& SourceInfo) override; 
				bool AddSection(const std::string NameOfSection, int CountOfParam) override;
				bool DelSection(const std::string NameOfSection) override;
				bool AddParam(const std::string NameOfSection, std::string NameOfParam) override;
				bool DelParam(const std::string NameOfSection, std::string NameOfParam) override;

			private:
				unsigned char m_optionSep = '=';
				unsigned char m_secBegin = '[';
				unsigned char m_secEnd= ']';
			};
			class JSON : public TemplateSource
			{
			public:
				JSON() = default;
				JSON(std::string path, std::string name);

				bool SaveConf(const CConf::CSection& SourceInfo) override;
				CConf::Conf LoadConf(const CConf::CSection& SourceInfo) override;
				bool AddSection(const std::string NameOfSection, int CountOfParam) override;
				bool DelSection(const std::string NameOfSection) override;
				bool AddParam(const std::string NameOfSection, std::string NameOfParam) override;
				bool DelParam(const std::string NameOfSection, std::string NameOfParam) override;
			};
			class XML : public TemplateSource
			{
			public:
				XML() = default;
				XML(std::string path, std::string name);

				bool SaveConf(const CConf::CSection& SourceInfo) override;
				CConf::Conf LoadConf(const CConf::CSection& SourceInfo) override;
				bool AddSection(const std::string NameOfSection, int CountOfParam) override;
				bool DelSection(const std::string NameOfSection) override;
				bool AddParam(const std::string NameOfSection, std::string NameOfParam) override;
				bool DelParam(const std::string NameOfSection, std::string NameOfParam) override;
			};
			class WINREG : public TemplateSource
			{
			public:
				WINREG() = default;
				WINREG(std::string path, std::string name);

				bool SaveConf(const CConf::CSection& SourceInfo) override;
				CConf::Conf LoadConf(const CConf::CSection& SourceInfo) override;
				bool AddSection(const std::string NameOfSection, int CountOfParam) override;
				bool DelSection(const std::string NameOfSection) override;
				bool AddParam(const std::string NameOfSection, std::string NameOfParam) override;
				bool DelParam(const std::string NameOfSection, std::string NameOfParam) override;
			};
			class INI : public TemplateSource
			{
			public:
				INI() = default;
				INI(std::string path, std::string name);

				bool SaveConf(const CConf::CSection& SourceInfo) override;
				CConf::Conf LoadConf(const CConf::CSection& SourceInfo) override;
				bool AddSection(const std::string NameOfSection, int CountOfParam) override;
				bool DelSection(const std::string NameOfSection) override;
				bool AddParam(const std::string NameOfSection, std::string NameOfParam) override;
				bool DelParam(const std::string NameOfSection, std::string NameOfParam) override;
			};
			class DB : public TemplateSource
			{
			public:
				DB() = default;
				DB(std::string server, std::string db);
				DB(std::string server, std::string db, std::string login, std::string pass);

				bool SaveConf(const CConf::CSection& SourceInfo) override;
				CConf::Conf LoadConf(const CConf::CSection& SourceInfo) override;
				bool AddSection(const std::string NameOfSection, int CountOfParam) override;
				bool DelSection(const std::string NameOfSection) override;
				bool AddParam(const std::string NameOfSection, std::string NameOfParam) override;
				bool DelParam(const std::string NameOfSection, std::string NameOfParam) override;
			};

		};
		//����� � ����� � �������� �����-���� ��������
		CSource();
		CSource(type ttype) : m_type{ ttype }{};
		//friend class Conf;
		//���������� ������-������ � ������
		//��������� ������ �� �������������/����������� //���� ���-�� ���������� ��������� �����������
		bool Init();
		//bool Init(Section& src);
		//��������� �� ���������
		bool Check();
		/// <summary>
		/// �������� ������� �� ����������� ConnectInfo
		/// </summary>
		/// <param name="ConnectInfo"></param>
		/// <returns>������</returns>
		CConf::Conf Load(CConf::CSection* ConnectInfo);
		//bool FindConnInfo(CConf::Conf where, CConf::CSection* ConnectInfo);
		bool Save(CConf::Conf what, CConf::CSection* ConnectInfo);

		//���������������� ������ � conf ��������
		//��������� ������� ������ ������������ � ����� � ��������, 
		//���� ���� ���� ���� - ����� ��������� ���������� ���� ������� ���������� �������� {��� ������� �������}
		//1 - � ����� � exe ������ ���� ���� �� ������ | 0 - ���\�� ���
		bool FindOutTheType();
		bool FindOutTheType(type desitionType);
		//������� ��� �������
		void SetType(type type);
		//bool LoadTo(CConf::Conf& cfg);
		type  GetType();
	private:
		type m_type= type::TXT;
		implem::DB m_funcDB;
		implem::INI m_funcINI;
		implem::JSON m_funcJSON;
		implem::TXT m_funcTXT;
		implem::WINREG m_funcWINREG;
		implem::XML m_funcXML;
		/// <summary>
		/// ���� ������ ��������� ����������� ��������, �� ��� ��� ������
		/// </summary>
		/// <param name="name"></param>
		/// <returns>��������� �� ������</returns>
		static bool TXTisSectionName(std::string name);
		//���� � ������ ���� ���� ���������, �� ��� �����
		static bool TXTisOption(std::string name);
		//CConf::CSection& m_sett4conn2source;
		//��� catch'� ����� ����������� ���������� ����
		CConf::CSection m_error;
	};
}
