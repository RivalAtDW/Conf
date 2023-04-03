#pragma once
#    define _AFXDLL
#include "CConf.h"
#include <afxcoll.h>
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
		//virtual std::vector<std::vector<CString>> ConfLoad();// const = 0;
		//��������� ������ � ������ � ����������� �� ���� ctype
		//���� ������� �����, ����� ���������, ���� �� � ���������� ��������� ��� ����������� \ ���� �������
		//����� - �������� ������������ � ����� � ����������� �� ��������� 
		//��� � ����������� �������� ������
		//��� ��������� ���� ������
		virtual bool SaveConf(const CSection& SourceInfo);// = 0;
		//load from source
		virtual CConf::Conf LoadConf(const CSection& SourceInfo);// = 0;
		virtual bool AddSection(const CString NameOfSection, int CountOfParam) = 0;
		virtual bool DelSection(const CString NameOfSection) = 0;
		virtual bool AddParam(const CString NameOfSection, CString  NameOfParam) = 0;
		virtual bool DelParam(const CString NameOfSection, CString  NameOfParam) = 0;

	private:
		//������ ��������� ������� - [������ ������ �����-��, �� ������� ������ �����]
		std::vector<CString> m_state;
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
				TXT(const CString path, const CString name);
				explicit TXT(const CConf::CSection& SettingsOfConnectToSource);

				bool SaveConf(const CConf::CSection& SourceInfo) override;
				CConf::Conf LoadConf(const CConf::CSection& SourceInfo) override; 
				bool AddSection(const CString NameOfSection, int CountOfParam) override;
				bool DelSection(const CString NameOfSection) override;
				bool AddParam(const CString NameOfSection, CString NameOfParam) override;
				bool DelParam(const CString NameOfSection, CString NameOfParam) override;

			private:
				unsigned char m_optionSep = '=';
				unsigned char m_secBegin = '[';
				unsigned char m_secEnd= ']';
			};
			class JSON : public TemplateSource
			{
			public:
				JSON() = default;
				JSON(CString path, CString name);

				bool SaveConf(const CConf::CSection& SourceInfo) override;
				CConf::Conf LoadConf(const CConf::CSection& SourceInfo) override;
				bool AddSection(const CString NameOfSection, int CountOfParam) override;
				bool DelSection(const CString NameOfSection) override;
				bool AddParam(const CString NameOfSection, CString NameOfParam) override;
				bool DelParam(const CString NameOfSection, CString NameOfParam) override;
			};
			class XML : public TemplateSource
			{
			public:
				XML() = default;
				XML(CString path, CString name);

				bool SaveConf(const CConf::CSection& SourceInfo) override;
				CConf::Conf LoadConf(const CConf::CSection& SourceInfo) override;
				bool AddSection(const CString NameOfSection, int CountOfParam) override;
				bool DelSection(const CString NameOfSection) override;
				bool AddParam(const CString NameOfSection, CString NameOfParam) override;
				bool DelParam(const CString NameOfSection, CString NameOfParam) override;
			};
			class WINREG : public TemplateSource
			{
			public:
				WINREG() = default;
				WINREG(CString path, CString name);

				bool SaveConf(const CConf::CSection& SourceInfo) override;
				CConf::Conf LoadConf(const CConf::CSection& SourceInfo) override;
				bool AddSection(const CString NameOfSection, int CountOfParam) override;
				bool DelSection(const CString NameOfSection) override;
				bool AddParam(const CString NameOfSection, CString NameOfParam) override;
				bool DelParam(const CString NameOfSection, CString NameOfParam) override;
			};
			class INI : public TemplateSource
			{
			public:
				INI() = default;
				INI(CString path, CString name);

				bool SaveConf(const CConf::CSection& SourceInfo) override;
				CConf::Conf LoadConf(const CConf::CSection& SourceInfo) override;
				bool AddSection(const CString NameOfSection, int CountOfParam) override;
				bool DelSection(const CString NameOfSection) override;
				bool AddParam(const CString NameOfSection, CString NameOfParam) override;
				bool DelParam(const CString NameOfSection, CString NameOfParam) override;
			};
			class DB : public TemplateSource
			{
			public:
				DB() = default;
				DB(CString server, CString db);
				DB(CString server, CString db, CString login, CString pass);

				bool SaveConf(const CConf::CSection& SourceInfo) override;
				CConf::Conf LoadConf(const CConf::CSection& SourceInfo) override;
				bool AddSection(const CString NameOfSection, int CountOfParam) override;
				bool DelSection(const CString NameOfSection) override;
				bool AddParam(const CString NameOfSection, CString NameOfParam) override;
				bool DelParam(const CString NameOfSection, CString NameOfParam) override;
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
