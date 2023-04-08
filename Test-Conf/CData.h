#pragma once
#    define OUT
//#include <afxcoll.h>
#include <variant>
#include <string>
#include <vector>
#include <map>
#include <variant>


#ifdef _EXPORTING
#define BASE    __declspec(dllexport)
#else
#define BASE    __declspec(dllimport)
#endif



class base
{
public:
	BASE base() = default;

	BASE explicit base(std::string name) :m_name{ name.c_str() } {};
	//explicit base(const wchar_t name) :m_name{ name } {};
	BASE virtual ~base() = default;
	BASE virtual std::string GetName() const;
	BASE virtual void SetName(std::string name);
	/// <summary>
	///Проводит поиск в m_name на предмет наличия separator.
	/// true, если есть хотя бы один разделитель, не находящийся на нулевом индексе
	/// </summary>
	///
	///<returns>разделитель(если FindSeparator()) и вектор позиций этого символа в строке</returns> 
	BASE bool FindSeparator(OUT unsigned char& separator) const;
	/// <summary>
	/// поиск separator в любой строке, указанной пользователем
	/// </summary>
	/// <param name="name">имя</param>
	/// <param name="separator">разграничитель</param>
	/// <param name="result">вектор позиций разграничителей</param>
	/// <returns>вектор позиций этого символа в строке</returns>
	std::vector<std::string> GetArrayFrom(std::string base) const;
	static bool isHadSeparator(std::string name, unsigned char separator);
	static bool FindSeparator(std::string name, unsigned char& separator);
	virtual bool HasEntry(const std::string name) const = 0;
	//static std::string ToSTL(CString value);
	//static CString  ToMFC(std::string value);
	static std::string GetOptionNameFrom(std::string name, unsigned char separator);
protected:
	std::string  m_name="Default";
	unsigned char m_separator = '.';
	//метод проводит поиск уникальных символов в переданном массиве, затем ищет и выводит самый часто используемый 
	//подразумевается использование в FindSeparator
	static wchar_t FindFamousChar(const std::vector<wchar_t>& resource);
	static bool ContainChar(wchar_t char4find, std::vector<wchar_t> arrayChar);

};

//Custom Conf Data
//типичный контейнер данных
class CData : public base
{
public:
	BASE CData(void) : base(), m_value{ false }{};
	BASE CData(std::string Name) : base(Name), m_value{ false }{};
	BASE CData(std::string Name, const std::variant<bool, int, double, std::string>& Value) : base(Name), m_value{ Value }{ };

public:
	enum class TOtype : int8_t
	{
		BOOL = 1, INT, DOUBLE, STRING, BYTE, BYTE8, CORRUPT
	};
	BASE std::string GetName() const;
	BASE std::variant<bool, int, double,std::string> GetValue() const;
	BASE CData GetData() const;
	BASE unsigned char GetSeparator();
	BASE static void Copy(const CData& src, CData& dst);
	BASE static void Swap(CData& src, CData& dst);
	//установить в существующую где-то переменную данные по ссылке
	//установить в хранилище данные по ссылке
	BASE void SetData(CData& src);
	//установить в хранилище данные по ссылке
	BASE void SetData(std::string name, std::variant<bool, int, double,std::string>& value);
	BASE void SetValue(CData::TOtype type);
	BASE void SetData(std::variant<bool, int, double, std::string>& value);
	BASE void SetSeparator(unsigned char value);

	BASE bool HasEntry(const std::string name) const override { return false; };
	BASE TOtype ViewTypeOfValue() const;
	BASE friend std::ostream& operator<< (std::ostream& out, const CData& source);
	struct make_string_functor
	{
		std::string operator()(int x) const { return std::to_string(x); }
		std::string operator()(bool x) const {
			std::string res;

			if (x) //std::boolalpha
			{
				res = "true";
			}
			else
			{
				res = "false";
			}

			return res;
		}
		std::string operator()(double x) const { return std::to_string(x); }
		std::string operator()(std::string x) const
		{
			return  x;
		};
	};
	struct make_type_functor
	{
		int8_t operator()(const std::string x) const {
			enum  typechar : char
			{
				TXT = 't', JSON = 'j', XML = 'x', WINREG = 'w', INI = 'i', DB = 'd'
			};

			auto swither = tolower(x[0]);
			switch (static_cast<typechar>(swither))
			{
			case TXT:
			{
				return static_cast<int8_t>(1);
			}

			break;
			case JSON:
			{
				return static_cast<int8_t>(2);
			}
			break;
			case XML:
			{
				return static_cast<int8_t>(3);
			}
			break;
			case WINREG:
			{
				return static_cast<int8_t>(4);
			}
			break;
			case INI:
			{
				return static_cast<int8_t>(5);
			}
			break;
			case DB:
			{
				return static_cast<int8_t>(6);
			}
			break;
			default:
			{
				return static_cast<int8_t>(0);
			}
			break;
			}
		}
		int8_t operator()(int x) const { return static_cast<int8_t>(x); }
		int8_t operator()(bool x) const {

			return static_cast<int8_t>(x);
		}
		int8_t operator()(double x) const { return static_cast<int8_t>(x); }

	};
	friend class Sections;
	friend bool operator == (const CData& lhs, const CData& rhs);
private:
	std::variant<bool, int, double, std::string> m_value;
	TOtype m_whatTheValue = CData::TOtype::STRING;
};

