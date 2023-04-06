#pragma once
#include "CData.h"
//#    define _AFXDLL
//#include <afxcoll.h>
#include <variant>
#include <string>
#include <vector>
#define UNVALID_SECTION CConf::CSection("____unvalid_section_except")
#define DEFAULT_DATA CData(std::string("____default_section"))
namespace CConf
{
	struct SPosition
	{
		size_t x = 0;
		size_t y = 0;
	};
	class CSection : public base
	{
	public:
		friend class Conf;
		CSection() = default;
		explicit CSection(const std::string NameOfSection) : base(NameOfSection) {};
		CSection(std::string NameOfSection, CData value): base(NameOfSection) {
			auto name = value.GetName();
			m_Data.insert(std::pair<std::string, CData>(name, value));
		};
		CSection(std::string NameOfSection, size_t size) : base(NameOfSection) {
			for (size_t i = 0; i < size; i++)
			{
				std::string uniqueName(NameOfSection);
				uniqueName += std::to_string(i);
				m_Data.insert(std::pair<std::string, CData>(uniqueName, DEFAULT_DATA));
			}

		};
		template <class CData, class... Args>
		explicit CSection(std::string NameOfSection, const std::initializer_list<CData>& init) : base(NameOfSection)
		{
			for (const CData* it = begin(init); it != end(init); ++it)
			{
				m_Data.insert(std::pair<std::string, CData>(it->GetName(), *it));
			}
		};

		explicit CSection(std::string NameOfSection, const std::map<std::string, CData> map) : base(NameOfSection), m_Data(map)
		{

		};
		CSection GetSubSection(std::string name);
		//вернуть первую попавшуюся CData с указанным именем
		CData GetData(std::string name) const;
		void AddData(CData& value);
		void AddSection(CSection& section);
		void AddSection(std::string name);
		//id,name,value
		void Write(std::string NameOfSection, std::variant<bool, int, double, std::string> value);
		//id,value
		void Write(CData value);
		//содержит ли основная карта примитив CData с указанным именем
		bool Contain(const std::string nameOfCData) const;
		//содержит ли дополнительная карта с секциями какой-либо объект с таким именем
		bool ContainExt(const std::string nameOfCData) const;
		static bool Contain(CSection& src, const std::string nameOfCData);

		size_t GetOptionsCount() const;
		void Empty();
		bool HasEntry(const std::string name) const override;
		bool Read(std::string from, OUT bool& where);
		bool Read(std::string from, OUT int& where);
		bool Read(std::string from, OUT double& where);
		bool Read(std::string from, OUT std::string& where);
		size_t GetSectCount() const;
		size_t size() const;
		
	private:
		void SetTypeOfValue(std::string name, CData::TOtype type);
		std::map<std::string, CData>  m_Data;
		std::map<std::string, std::map<std::string, CData>> m_sub;
		CData m_error;
	};
}