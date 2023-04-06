#pragma once

#include "CSection.h"
#include "CSource.h"
#include <variant>
#include <string>
#    define OUT
//#    define _AFXDLL
#include <fstream>
//#include <afxcoll.h>
#include <map>
#include <atlbase.h>

//����������� ���������� ������
namespace CDataStorage
{
	enum class TOtypeFile : int8_t { unknown = 0, text, photo, archive, sql_dump, another };
	//�������� std::string � ������������ ��� ��� ������ ���

	//Based on std::string
	class binary
	{
	public:
		binary(){ };
		explicit binary(std::string CData) {};
	public:
		//!!!std::vector::data()
		std::string GetData() const;
		//��������� ��� TOtype � ������� ���������� ��� ����� (mask.jpg mask.txt) ��� �������� � �����
		//1 success 0 except
		bool WriteData(std::string array);
		size_t size();
	private:
		std::string m_data;
		//���� ���-�� ����������� �������� ������
		// 
		//4 user - ��������� �������� ������ - ���� �� ������������� �� (���������, ���� ��������)
		TOtypeFile typeOfBinary=TOtypeFile::unknown;
	};
};

namespace CConf
{
	class Conf : public base
	{
	public:	
		friend class CSource;
		//constructors
		Conf() : base("DefaultCFGName") {};
		explicit Conf(std::string name) : base(name)  {};
		explicit Conf(CConf::CSection SourceInfo) : base() { AddSection(SourceInfo); };
		template <class CSection, class... Args>
		Conf(const std::initializer_list<CSection > init) : base("DefaultCFGName")	{
			for (const CSection* it = begin(init); it != end(init); ++it)
			{
				m_settings.insert(std::pair<std::string, CSection>(it->GetName(), *it));
			}
		}
		template <class CSection, class... Args>
		Conf(std::string name, const std::initializer_list<CSection> init) : base(name)// : private Foo<Args...>
		{
			for (const CSection* it = begin(init); it != end(init); ++it)
				m_settings.emplace(*it);
		}; 
		//end constructors
		virtual ~Conf() = default;
		
		CConf::CSection* GetSection(std::string name);
		bool Contain(std::string nameOfSection) const;

		enum class typeOfContain : int8_t
		{
			client, win, win_client, user
		};	
		/// <summary>
		/// ������� ������ ������� � ������������������ ������� ��������, �������. 
		/// ���� ����� ����� ��� ������������ ��� ��� �������� (��� ������)
		/// </summary>
		/// <param name="type"> - ��� �������, ������� �����</param>
		/// <returns>������, ���������� �������� ������������ ��������� �����</returns>
		static CConf::Conf CreateConf(typeOfContain type);
		size_t GetSectionsCount(const std::string name);
		/// <summary>
		/// ���������, �������� �� ��������� ������ ������ ��� ������������ � m_settings ������
		/// </summary>
		/// <param name="name"></param>
		/// <returns>true, ���� ���� ������ � ����� ������ � m_settings</returns>
		bool IsSection(const std::string name);
		/// <summary>
		/// ���������, �������� �� ��������� ������ ������ ��� �����
		/// </summary>
		/// <param name="name"></param>
		/// <returns>true, ���� ����� ����� ���� � ���� (! IsSection())</returns>
		bool IsOption(const std::string name);
		//�������� ��� ������������ ������
		bool AddSection(const CSection newSection);


		bool AddSection(const std::string name);

		//������� ���������, �������� (main.form.tree, test.big) ������� ������ main.form.tree.test main.form.tree.test.big, ���� �� ���
		void AddSubSection(const CSection& masterSection, CSection slaveSection);
		//��������� � ������� ������� ���� ��������� ������, ���� �� ��� - �������
		void AddSubSection();
		/// <summary>
		/// ���������� ������ ����� � ���� ������, 
		/// ������ �������� ���� �� ��������� ������ � ������� � 
		/// ���� �� ���, �� ������� �� � ������������� �� ���������.
		/// ��������, ��� ����� main.size.x ��������� ������� �������� main.size
		/// </summary>
		/// <param name="NameOfData"></param>
		/// <param name="value"></param>
		/// <returns>�������� �� ����������</returns>
		bool Write(std::string dstSectionSepOption, std::variant<bool, int, double, std::string> srcValue);
		bool Write(std::string dstSectionSepOption, std::string srcValue);
		bool Write(std::string dstSectionSepOption, CData src);
		bool Write(std::string dstSectionSepOption, bool srcValue);
		bool Write(std::string dstSectionSepOption, int srcValue);
		bool Write(std::string dstSectionSepOption, double srcValue);


		//�������� �� ������� ������ ��� ��������� �� ��� ����� � ��������� ������
		bool CheckExistSectionForThisOption(std::string NameOfDataWithSep) const;
		/// <summary>
		/// ������� ������
		/// </summary>
		/// <param name="section"></param>
		void Rem(const CConf::CSection& section);
		void Rem(const std::string nameOfSection);
		/// <summary>
		/// ������ ��������� ������� �������� � �������
		/// </summary>
		/// <param name="name"></param>
		/// <returns>���� �� � ������� ��� ������</returns>
		bool HasEntry(const std::string name) const override;
		void SetOption(const std::string name, std::variant<bool, int, double, std::string> value);

		/// <returns>���������� ������</returns>
		size_t Size() const;
		/// <param name="name">��� ������</param>
		/// <returns>���������� ����� � ��������� ������</returns>
		size_t GetOptionsCount(const std::string name);
		void AddDependentSec(std::string name);
		private:
		void AddOption(std::string section, std::string option, std::variant<bool, int, double, std::string> value);

		/// <summary>
		/// ������� � ������� ��������� ������. ��������, AddDependentSec("test/test/test") ������ �������� ��� ������
		/// </summary>
		/// <param name="name">��� � ��������������</param>

		std::vector<std::string> GetDependedArray(std::vector<std::string> base) const;
		std::vector<std::string> GetDependedArray(std::string name) const;
		std::string GetOptionNameFrom(std::string name) const;
		std::string GetSectionNameFrom(std::string name) const;

		std::map<std::string, CSection> m_settings;
		CConf::CSection m_error;
		
	};
	static std::unique_ptr<CConf::Conf>  CreateConf();
	//�������� ����� ������ ������������ ��� �����������
   //����� ����� � ����� ����������� ���� ������� 
  // static double Load([[maybe_unused]] TemplateSource* src, [[maybe_unused]] std::string name) {
	//   return 2;
  // }
}

static bool isDirectoryExists(std::string path)
{
	LPCWSTR lp = (LPCWSTR)path.c_str();
	DWORD dwFileAttributes = GetFileAttributes(lp);
	if (dwFileAttributes == 0xFFFFFFFF)
		return false;
	return dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

static bool isFileExists(std::string pathAndFile)
{
	std::ifstream file(pathAndFile);
	try
	{
		if (file)
		{
			file.close();
			return true;
		}
	}
	catch (...)//(const std::exception& a)
	{
	//send err message somewhere
	}
	return false;
}

