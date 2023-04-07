#include "pch.h"
#include "CConf.h"
#include "CData.h"
//#define equal(x,y){} 

TEST(TestMethod, TestConstructor) {
	//constructor CData
		CData CData1;
		auto CData2 = CData::CData("CData2", 12);
		auto CData3 = CData::CData("CData3", true);
		auto CData4 = CData::CData("CData4", 1);
		auto typeOfCData_CData1 = CData1.ViewTypeOfValue();
		auto typeOfCData_CData2 = CData2.ViewTypeOfValue();
		auto typeOfCData_CData3 = CData3.ViewTypeOfValue();
		auto typeOfCData_CData4 = CData4.ViewTypeOfValue();
		CData::Copy(CData2, CData1);
		CData::Swap(CData3, CData4);
		EXPECT_TRUE(true);
}

	TEST(TestMethod, Swap)
	{
		const auto CData2Source = CData("CData2", 12);
		const auto CData3Source = CData("CData3", true);
		CData CData2 = CData2Source;
		CData CData3 = CData3Source;
		CData::Swap(CData2, CData3);
		bool result = CData2 == CData3Source;
		EXPECT_EQ(result, true);
	}
	
	TEST(TestMethod, sectionCreate)
	{
		setlocale(LC_ALL, "rus");
		std::string secName = "[connection-to-source]";
		CConf::Conf conf("ConfigTestCreation");
		CConf::CSection SettingsOfConnectToSource(secName);//add type - connectinfo
		auto res1 = CData(std::string("PathTXTFile"), std::string("C:\\Новая папка\\"));
		auto res2 = CData(CData(std::string("NameTXTFile"), std::string("b.txt")));
		auto res3 = CData(CData(std::string("Type"), std::string("TXT")));
		SettingsOfConnectToSource.AddData(res1);
		SettingsOfConnectToSource.AddData(res2);
		SettingsOfConnectToSource.AddData(res3);
		conf.AddSection(SettingsOfConnectToSource);
		conf.AddSection(SettingsOfConnectToSource);
		conf.AddSection(CConf::CSection());
		conf.AddSection(SettingsOfConnectToSource);
		conf.AddSection(SettingsOfConnectToSource);
		conf.AddSection(CConf::CSection());
		bool realContain=conf.Contain(secName);
		std::cout << "\n ";		
		conf.Contain("Main");
		//std::unique_ptr<CConf::Conf> backup1{ CConf::CreateConf() };
		auto backup2 = std::make_unique<CConf::Conf>();
		//auto what1 = backup1.get();
		auto what2 = backup2.get();
		auto rich = "App.Globals.DB.HostName";
		std::string aa= std::string("C:\\Новая папка\\a.ini");
		bool existAa= isFileExists(aa);
	}
	
	TEST(FindSeparator, FindSeparatorENG)
	{
		std::string testCString("..main.separator.finder@mail.com");
		std::string value(testCString.c_str());
		CData data = CData(testCString, value);
		unsigned char result;
		CData::FindSeparator(testCString, result);
		EXPECT_TRUE(result =='.');
	}

	TEST(FindSeparator, FindSeparatorRU)
	{
		std::string testCString("..Главный.разделитель.Поисковик@Мыло.ру");
		std::string value= testCString;
		CData data = CData(value, testCString);
		unsigned char result;
		using namespace CConf;
		CData::FindSeparator(testCString, result);
		EXPECT_TRUE(result =='.');
	}

	TEST(AddSubSection, test1)
	{
		std::string testCString2("window.Main.MasterFrame");
		std::string testCString1("window.Main.MasterFrame.ScreenPosition");
		CData data1 = CData(testCString1, testCString1);
		CData data2 = CData(testCString2, testCString2);
		CConf::CSection sec1 = CConf::CSection(testCString1);
		CConf::CSection sec2 = CConf::CSection(testCString2);
		sec1.AddData(data1);
		sec1.AddData(data2);
		sec2.AddData(data1);
		sec2.AddData(data2);
		auto cfg = CConf::Conf( { sec1, sec2 } );


		auto cfg2 = cfg;
		cfg.AddSubSection(sec2, sec1);
		auto* pSect = cfg.GetSection(std::string(sec2.GetName()));
		auto* pSect2 = cfg.GetSection(std::string(sec1.GetName()));
		EXPECT_TRUE(pSect->isHadSubSect());
		EXPECT_FALSE(pSect2->isHadSubSect());
	}
	TEST(AddSubSection, test2)
	{
		std::string testCString2("window.Main.MasterFrame.test1.test1.test1.test1.test1.test1.test1.test1");
		std::string testCString1("window.Main.MasterFrame.ScreenPosition");
		CData data1 = CData(testCString1, testCString1);
		CData data2 = CData(testCString2, testCString2);
		CConf::CSection sec1 = CConf::CSection(testCString1);
		CConf::CSection sec2 = CConf::CSection(testCString2);
		auto cfg = CConf::Conf({ sec2,sec1 });
		sec1.AddSection(sec2);
		cfg.AddDependentSec(sec2.GetName());
		//_crtBreakAlloc = 1587;
		try
		{
			EXPECT_TRUE(cfg.Size() == 12);
		}
		catch (const std::exception&)
		{

		}

		//_CrtDumpMemoryLeaks();
	}
	

	TEST(AddSubSection, test3)
	{
		std::string testCString2("MasterFrame.NewYork.Center");
		std::string testCString1("window.Main.MasterFrame.ScreenPosition");
		CData data1 = CData(testCString1, testCString1);
		CData data2 = CData(testCString2, testCString2);
		CConf::CSection sec1 = CConf::CSection(testCString1);
		CConf::CSection sec2 = CConf::CSection(testCString2);
		auto cfg = CConf::Conf({ sec1, sec2 });
		cfg.AddDependentSec(sec2.GetName());
		cfg.AddDependentSec(sec1.GetName());
		EXPECT_TRUE(cfg.Size() == 7);
	}
	
	TEST(Constructor, SectionAddSection)
	{
	
		auto data1 = CData("Main.MasterFrame.ScreenPosition.x", 600);
		auto sec1 = CConf::CSection("Main.MasterFrame.TESTPosition", { data1 });
		auto sec2 = CConf::CSection("Main.MasterFrame.TEST", { data1 });
		auto sec3 = CConf::CSection("Main.MasterFrame.ScreenPosition", { data1});
		auto sec4 = CConf::CSection("Main.MasterFrame.testing", { data1 });

		sec2.AddSection(sec2);
		sec2.AddSection(sec2);
		sec2.AddSection(sec2);
		sec2.AddSection(sec2);


		sec2.AddSection(sec1);
		sec2.AddSection(sec3);
		sec2.AddSection(sec4);

		EXPECT_TRUE(sec2.GetOptionsCount()==1&& sec2.GetSectCount()==4);

		auto subSect=sec2.GetSubSection("Main.MasterFrame.TESTPosition");
		CConf::CSection sec5 = sec2;
		EXPECT_TRUE(subSect == sec1);
		EXPECT_TRUE(sec5 == sec2);

	}

	TEST(Constructor, SectionGet)
	{

		auto data1 = CData("Main.MasterFrame.ScreenPosition.y", 600);
		auto sec1 = CConf::CSection("Main.MasterFrame.TESTPosition", { data1, data1, data1, data1, data1, data1, data1, data1});

		auto data2 = CData("Main.MasterFrame.ScreenPosition.y2", 600);
		auto data3 = CData("Main.MasterFrame.ScreenPosition.y3", 600);
		auto data4 = CData("Main.MasterFrame.ScreenPosition.y4", 600);
		auto data5 = CData("Main.MasterFrame.ScreenPosition.y5", 600);
		auto data6 = CData("Main.MasterFrame.ScreenPosition.y6", 600);
		auto data7 = CData("Main.MasterFrame.ScreenPosition.y7", 600);
		auto data8 = CData("Main.MasterFrame.ScreenPosition.y8", 600);
		auto sec2 = CConf::CSection("Main.MasterFrame.ScreenPosition2", { data1, data2, data3, data4, data5, data6, data7, data8 });
		auto sec3 = CConf::CSection("Main.MasterFrame.ScreenPosition3", { data1 });
		auto sec4 = CConf::CSection("Main.MasterFrame.testing", { data1 });

		auto sec5 = sec2;
		sec2.AddSection(sec2);
		sec2.AddSection(sec2);
		sec2.AddSection(sec2);

		sec2.AddSection(sec5);

		sec2.AddSection(sec1);
		sec2.AddSection(sec3);
		sec2.AddSection(sec4);

		auto temp1 = sec2.GetSubSection("Main.MasterFrame.ScreenPosition2");

		
		// в этой секции 8 опций и 4 секции
		EXPECT_TRUE(sec2.GetOptionsCount()== 8);
		EXPECT_TRUE(sec2.GetSectCount() ==4);
		
		CConf::Conf cfg;
		
		//cfg.AddSectFromSection(sec2);

	}

	TEST(TestMethod, HasEntry)
	{
		auto data1 = CData("Main.MasterFrame.Size.Height", 200);
		auto data2 = CData("Main.MasterFrame.Size.Width", 600);
		auto sec1 = CConf::CSection("Main.MasterFrame.Size", { data1, data2 });
		data1 = CData("Main.MasterFrame.ScreenPosition.x", 600);
		data2 = CData("Main.MasterFrame.ScreenPosition.y", 200);
		auto sec2 = CConf::CSection("Main.MasterFrame.ScreenPosition", { data1, data2});
		auto data5 = CData("Main.MasterFrame.BkgndColorIdx",1);
		auto sec3 = CConf::CSection("Main.MasterFrame", { data5 });
		auto cfg = CConf::Conf({ sec1, sec2, sec3 });
		//EXPECT_TRUE(cfg.Size()==3&& cfg.at(0).GetOptionsCount()==2&& cfg.at(1).GetOptionsCount() == 2 && cfg.at(2).GetOptionsCount() == 1);
		EXPECT_TRUE(cfg.HasEntry("Main.MasterFrame.BkgndColorIdx"));
		EXPECT_TRUE(cfg.HasEntry("Main.MasterFrame.ScreenPosition"));
		EXPECT_TRUE(cfg.HasEntry("Main.MasterFrame.ScreenPosition.x"));
		EXPECT_TRUE(cfg.HasEntry("Main.MasterFrame.ScreenPosition.y"));
		EXPECT_FALSE(cfg.HasEntry("Main.MasterFrame.Size.FullScreen"));

		if (!cfg.HasEntry("Main.MasterFrame.Size.FullScreen")) {
			EXPECT_TRUE(cfg.HasEntry("Main.MasterFrame.Size.Width"));
			EXPECT_TRUE(cfg.HasEntry("Main.MasterFrame.Size.Height"));
		}
	}

	TEST(TestMethod, IsSection)
	{
		std::string testCString1("Main.MasterFrame");
		std::string testCString2("Main.MasterFrame.BkgndColorIdx");

		CConf::CSection sec1 = CConf::CSection(testCString1,CData("Main.MasterFrame.BkgndColorIdx",1));
		auto cfg = CConf::Conf({ sec1 });
		cfg.AddSubSection();

		EXPECT_TRUE(cfg.IsSection("Main.MasterFrame"));
		EXPECT_FALSE(cfg.IsSection("Main.MasterFrame.BkgndColorIdx"));
	}

	TEST(TestMethod, IsOption)
	{
		std::string nameOfSection("Main.MasterFrame");
		std::string testCString2(nameOfSection +".BkgndColorIdx");
		auto data = CData(testCString2, "Blue");
		auto sec1 = CConf::CSection(nameOfSection, data);
		auto cfg = CConf::Conf({ sec1 });
		auto result1 = cfg.IsOption("Main.MasterFrame");
		EXPECT_FALSE(result1== true);
		auto result2 = cfg.IsOption("Main.MasterFrame.BkgndColorIdx");
		EXPECT_TRUE(result2==true);
	}
	
	
//--------------------------------------------------------------------------------

	struct SEmployee
	{
		std::string  m_strFirstName;
		std::string  m_strMiddleName;
		std::string  m_strLastName;

		int      m_nAge = 0;
		double   m_nSalary = 0.0;
		bool     m_bHasCar = false;
	};

//--------------------------------------------------------------------------------
	
	TEST(TestMethod, Using)
	{
		SEmployee emp_1, emp_2;

		emp_1.m_strFirstName = std::string("Иван");
		emp_1.m_strMiddleName = std::string("Сергеевич");
		emp_1.m_strLastName = std::string("Петров");
		emp_1.m_nAge = 32;
		emp_1.m_nSalary = 1200.52;
		emp_1.m_bHasCar = true;

		CConf::Conf cfg;
		bool bOK = cfg.AddSection(std::string("Company.Stuff.Employees.Emp_147526"));
		EXPECT_TRUE(bOK);
		if (bOK) {
			EXPECT_TRUE(cfg.Write(std::string("Company.Stuff.Employees.Emp_147526.FirstName"), emp_1.m_strFirstName));
			EXPECT_TRUE(cfg.Write(std::string("Company.Stuff.Employees.Emp_147526.MiddleName"), emp_1.m_strMiddleName));
			EXPECT_TRUE(cfg.Write(std::string("Company.Stuff.Employees.Emp_147526.LastName"), emp_1.m_strLastName));

			EXPECT_TRUE(cfg.Write(std::string("Company.Stuff.Employees.Emp_147526.Age"), emp_1.m_nAge));
			EXPECT_TRUE(cfg.Write(std::string("Company.Stuff.Employees.Emp_147526.Salary"), emp_1.m_nSalary));
			EXPECT_TRUE(cfg.Write(std::string("Company.Stuff.Employees.Emp_147526.HasCar"), emp_1.m_bHasCar));
		}
	
		auto * pSect = cfg.GetSection(std::string("Company.Stuff.Employees.Emp_147526"));

		EXPECT_TRUE(pSect != nullptr);

		EXPECT_TRUE(cfg.CheckExistSectionForThisOption(std::string("Company.Stuff.Employees.Emp_147526.Salary")));
		EXPECT_FALSE(cfg.CheckExistSectionForThisOption(std::string("Company.Stuff.Employees.Emp_500.Salary")));


		if (pSect != nullptr) {
			EXPECT_TRUE(pSect->Read(std::string("FirstName"), emp_2.m_strFirstName));
			EXPECT_TRUE(pSect->Read(std::string("MiddleName"), emp_2.m_strMiddleName));
			EXPECT_TRUE(pSect->Read(std::string("LastName"), emp_2.m_strLastName));
			EXPECT_TRUE(pSect->Read(std::string("Age"), emp_2.m_nAge));
			EXPECT_TRUE(pSect->Read(std::string("Salary"), emp_2.m_nSalary));
			EXPECT_TRUE(pSect->Read(std::string("HasCar"), emp_2.m_bHasCar));
		}

		EXPECT_EQ(emp_1.m_strFirstName, emp_2.m_strFirstName);
		EXPECT_EQ(emp_1.m_strMiddleName, emp_2.m_strMiddleName);
		EXPECT_EQ(emp_1.m_strLastName, emp_2.m_strLastName);
		EXPECT_EQ(emp_1.m_nAge, emp_2.m_nAge);
		EXPECT_EQ(emp_1.m_nSalary, emp_2.m_nSalary);
		EXPECT_EQ(emp_1.m_bHasCar, emp_2.m_bHasCar);
		EXPECT_TRUE(bOK); 
		
	}
	
	TEST(TestMethod, Names)
	{
		auto sec1 = CConf::CSection(std::string("Main"), 10);
		auto sec2 = CConf::CSection(std::string("Default"),10);
		auto cfg = CConf::Conf({ sec1, sec2 });

		int n;
		int n2;
		int n3;
		int n4;

		sec1.Read("Default", n); // <- какое из 10 будет прочитано?.. - теперь имена опций в секциях уникальны (благодаря конструктору CSection), поэтому такого имени опции нет
		sec2.Read("Default", n); // <- какое из 10 будет прочитано?.. - теперь имена опций в секциях уникальны (благодаря конструктору CSection), поэтому такого имени опции нет
		sec2.Read("Default0", n);
		sec2.Write("Default0", 100500);
		sec2.Read("Default0", n2);

		EXPECT_EQ(n2, 100500);
		sec2.Write("Default9", 666);
		sec2.Read("Default9", n3);
	
		//следующей секции не существует Default19
		sec2.Read("Default19", n3); 
		EXPECT_EQ(n3, 666);
		sec2.Write("Default19", 1234512345);
		sec2.Read("Default19", n3);
		EXPECT_EQ(n3, 1234512345);
		//метод Read() работает только с существующими уже опциями
		//поскольку он должен считывать значение и записывать в переменную
		sec2.Read("Default9", n4);
		// Read(std::string("Default2229"), n4); вернет false;
		sec2.Read("Default2229", n4);

		EXPECT_EQ(n4, 666);


		cfg.Write("Default", 100500);//false
		cfg.Write("Default0", 100500);//false, потому что непонятно к чему обращена запись, возможно стоит подкрутить поиск
		cfg.Write("Main.Default0", 100500);//TODO: вычленить название опции, т.к. сейчас вставляет рядом с Default0 Main.Default0


		//ложь, нет символа-разделителя, write принимает только строки с разделителями
		//потому, что опции не хранятся в нем вне секций
		//вводимая строка имеет формат "имяСекции.Опция"

		cfg.IsOption("Default");   //  \ - это не опция
									   //   } где что? 
		cfg.IsSection("Default");  //  /  - это секция
		//только нужно добавить проверку на уникальное имя секции в addSection в конфиге и-или- секции
	}

	TEST(TestMethod, Counts)
	{
		auto sec1 = CConf::CSection("Main");
		sec1.AddSection("SubSect_1");
		sec1.AddSection("SubSect_2");
		sec1.AddSection("SubSect_3");
		//false
		sec1.AddSection("SubSect_3");
		sec1.AddSection("SubSect_3");
		sec1.AddSection("SubSect_3");
		sec1.AddSection("SubSect_3");
		//false
		auto cfg = CConf::Conf({ sec1 });

		EXPECT_EQ(sec1.GetSectCount(),3);
		EXPECT_EQ(cfg.Size(), 1);

	}

	TEST(TestMethod, removeSect)
	{
		auto data1 = CData("Main.MasterFrame.Size.Height", 200);
		auto data2 = CData("Main.MasterFrame.Size.Width", 600);
		auto sec1 = CConf::CSection("Main.MasterFrame.Size", { data1, data2 });
		data1 = CData("Main.MasterFrame.ScreenPosition.x", 600);
		data2 = CData("Main.MasterFrame.ScreenPosition.y", 200);
		auto sec2 = CConf::CSection("Main.MasterFrame.ScreenPosition", { data1, data2 });
		auto data5 = CData("Main.MasterFrame.BkgndColorIdx", 1);
		auto sec3 = CConf::CSection("Main.MasterFrame", { data5 });
		auto cfg = CConf::Conf({ sec1, sec2, sec3 });
		cfg.Rem(sec2.GetName());
		EXPECT_EQ(cfg.Size(), 2);
	}

	TEST(TestUsing, Bridge1)
	{
		std::string secName = std::string("[connection-to-source]");
		CConf::CSection SettingsOfConnectToSource(secName);
		auto res1 = CData(std::string("PathTXTFile"), std::string("Новая папка\\"));
		auto res2 = CData(CData(std::string("NameTXTFile"), std::string("b.txt")));
		auto res3 = CData(CData(std::string("Type"), std::string("TXT")));
		SettingsOfConnectToSource.AddData(res1);
		SettingsOfConnectToSource.AddData(res2);
		SettingsOfConnectToSource.AddData(res3);
		auto data1 = CData("Main.MasterFrame.Size.Height", 200);
		auto data2 = CData("Main.MasterFrame.Size.Width", 600);
		auto sec1 = CConf::CSection("Main.MasterFrame.Size", { data1, data2 });
		data1 = CData("Main.MasterFrame.ScreenPosition.x", 600);
		data2 = CData("Main.MasterFrame.ScreenPosition.y", 200);
		auto sec2 = CConf::CSection("Main.MasterFrame.ScreenPosition", { data1, data2 });
		auto data5 = CData("Main.MasterFrame.BkgndColorIdx", 1);
		auto sec3 = CConf::CSection("Main.MasterFrame", { data5 });
		auto cfg = CConf::Conf({ sec1, sec2, sec3, SettingsOfConnectToSource });





		CConf::CSource Source1;
		auto * ref= cfg.GetSection(secName);
 		auto result=Source1.Load(ref);




		//при загрузке тип значения опции некорректен, нужно придумать как его ввести
		// 
		// 
		// 
		auto Res = result.Size();
		auto booleanResult = Res == 4;
		EXPECT_EQ(booleanResult, true);
	}
	

	TEST(TestMethod, ConfAddSection)
	{
		auto data1 = CData("Main.MasterFrame.Size.Height", 200);
		auto data2 = CData("Main.MasterFrame.Size.Width", 600);
		auto sec1 = CConf::CSection("Main.MasterFrame.Size", { data1, data2 });
		data1 = CData("Main.MasterFrame.ScreenPosition.x", 600);
		data2 = CData("Main.MasterFrame.ScreenPosition.y", 200);
		auto sec2 = CConf::CSection("Main.MasterFrame.ScreenPosition", { data1, data2 });
		auto data5 = CData("Main.MasterFrame.BkgndColorIdx", 1);
		auto sec3 = CConf::CSection("Main.MasterFrame", { data5 });
		auto cfg = CConf::Conf({ sec1, sec2, sec3, sec3, sec2, sec3, sec3, sec2, sec3, sec3, sec2, sec3, sec3, sec2, sec3, sec3, sec2, sec3, sec3, sec2, sec3, sec3, sec2, sec3, sec3, sec2, sec3, sec3 });
		cfg.Rem(sec2.GetName());
		EXPECT_EQ(cfg.Size(), 2);
	}	
	