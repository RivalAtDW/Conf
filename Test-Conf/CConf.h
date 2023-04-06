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

//специфичные контейнеры данных
namespace CDataStorage
{
	enum class TOtypeFile : int8_t { unknown = 0, text, photo, archive, sql_dump, another };
	//обернуть std::string и использовать его как другой тип

	//Based on std::string
	class binary
	{
	public:
		binary(){ };
		explicit binary(std::string CData) {};
	public:
		//!!!std::vector::data()
		std::string GetData() const;
		//проверить тип TOtype и выбрать расширение для файла (mask.jpg mask.txt) для выгрузки в папку
		//1 success 0 except
		bool WriteData(std::string array);
		size_t size();
	private:
		std::string m_data;
		//надо как-то перегружать оператор вывода
		// 
		//4 user - категория бинарных данных - чтоб не анализировать их (заполнять, если известно)
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
		/// создает объект конфига с предустановленными заранее секциями, опциями. 
		/// Этот метод нужен для демонстрации или для удобства (как шаблон)
		/// </summary>
		/// <param name="type"> - тип конфига, который нужен</param>
		/// <returns>конфиг, содержимое которого определяется указанным типом</returns>
		static CConf::Conf CreateConf(typeOfContain type);
		size_t GetSectionsCount(const std::string name);
		/// <summary>
		/// проверить, является ли введенная строка именем для существующей в m_settings секции
		/// </summary>
		/// <param name="name"></param>
		/// <returns>true, если есть секция с таким именем в m_settings</returns>
		bool IsSection(const std::string name);
		/// <summary>
		/// проверить, является ли введенная строка именем для опции
		/// </summary>
		/// <param name="name"></param>
		/// <returns>true, если такая опция есть и если (! IsSection())</returns>
		bool IsOption(const std::string name);
		//добавить уже существующую секцию
		bool AddSection(const CSection newSection);


		bool AddSection(const std::string name);

		//создать подсекцию, например (main.form.tree, test.big) создаст секции main.form.tree.test main.form.tree.test.big, если их нет
		void AddSubSection(const CSection& masterSection, CSection slaveSection);
		//проверить в конфиге наличие всех зависимых секций, если их нет - создать
		void AddSubSection();
		/// <summary>
		/// Записывает данные опции в поля секции, 
		/// притом проверяя есть ли зависимая секция в конфиге и 
		/// если ее нет, то создает ее с конструктором по умолчанию.
		/// Например, для опции main.size.x зависимой секцией является main.size
		/// </summary>
		/// <param name="NameOfData"></param>
		/// <param name="value"></param>
		/// <returns>записана ли информация</returns>
		bool Write(std::string dstSectionSepOption, std::variant<bool, int, double, std::string> srcValue);
		bool Write(std::string dstSectionSepOption, std::string srcValue);
		bool Write(std::string dstSectionSepOption, CData src);
		bool Write(std::string dstSectionSepOption, bool srcValue);
		bool Write(std::string dstSectionSepOption, int srcValue);
		bool Write(std::string dstSectionSepOption, double srcValue);


		//проверка на наличие секции для зависимой от нее опции с указанным именем
		bool CheckExistSectionForThisOption(std::string NameOfDataWithSep) const;
		/// <summary>
		/// удалить секцию
		/// </summary>
		/// <param name="section"></param>
		void Rem(const CConf::CSection& section);
		void Rem(const std::string nameOfSection);
		/// <summary>
		/// просто проверяет наличие сущности в конфиге
		/// </summary>
		/// <param name="name"></param>
		/// <returns>есть ли в конфиге эта запись</returns>
		bool HasEntry(const std::string name) const override;
		void SetOption(const std::string name, std::variant<bool, int, double, std::string> value);

		/// <returns>количество секций</returns>
		size_t Size() const;
		/// <param name="name">имя секции</param>
		/// <returns>количество опций в указанной секции</returns>
		size_t GetOptionsCount(const std::string name);
		void AddDependentSec(std::string name);
		private:
		void AddOption(std::string section, std::string option, std::variant<bool, int, double, std::string> value);

		/// <summary>
		/// Создает в конфиге зависимые секции. Например, AddDependentSec("test/test/test") должен добавить три секции
		/// </summary>
		/// <param name="name">имя с раздделителями</param>

		std::vector<std::string> GetDependedArray(std::vector<std::string> base) const;
		std::vector<std::string> GetDependedArray(std::string name) const;
		std::string GetOptionNameFrom(std::string name) const;
		std::string GetSectionNameFrom(std::string name) const;

		std::map<std::string, CSection> m_settings;
		CConf::CSection m_error;
		
	};
	static std::unique_ptr<CConf::Conf>  CreateConf();
	//выяснить какой символ используется для разделителя
   //поиск опции в любом производном типе ресурса 
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

