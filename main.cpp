#include <Windows.h>    // Со скидками есть пару багов, но честно, я устал от этого проекта, поэтому я не стал их фиксить
#include <iostream>
#include <String>                  
#include <vector>
#include <fstream>
#include <cmath>

std::ifstream file; // Объявление объекта для файлов
std::vector<std::string> allFile{}; // Вектор который хранит все файлы из файла "AllFile.txt"
std::vector<std::string> category{}; // Вектор который хранит категории 
std::vector<std::string> purchases{}; // Вектор который хранит покупки
std::vector<std::string> chequeStr{}; //Вектор для чека String
std::vector<double> priceDouble{}; // Вектор который хранит цены на покупки в double
std::vector<double> chequeDouble{}; //Вектор для чека Double
long double totalAmount{}; // вектор который считает сумму покупки в момент её выбора
long double totalAmountDiscounts{}; // вектор который считает сумму покупки в момент её выбора с учетом скидки


void CheckingAllFile(); // Проверка файла "AllFile.txt" - где храняться все остальные файлы 
bool OpeningFile(std::string fileName); // Проверяет доп файлы на открытие
void Exit(); // Приветсвие магазина 
void MchoosingCategory(); // выбор категории 
void PrintCategory(std::vector<std::string>& vec); // вывод вектора ( категории )
bool CheckingExistenceCategory(std::string& inputCategory, std::vector<std::string>& vec); // Просматривает, если ли категория которую выбрал пользователь
void MchoosingPurchases(std::string& inputCategory); // выбор покупки в категории 
void СompletionPurchases(std::string& inputCategory); //  Заполняет вектор продуктами и ценами из нужной категории 
void PrintPurchases(std::string& inputCategory); // Выводит продукты и цены
bool CheckingExistencePurchases(std::string& inputPurchases); // Просматривает, если ли покупка которую выбрал пользователь
double ProductQuantity(); // спрашивает какое кол-во тавара надо пользователю
void CalculatingPrice(std::string& inputPurchases, double& productQuantity); //подсчитывает цену товара
void CompilationCheque(std::string& inputPurchases, double& productQuantity, double& priceThisProduct, double& amountPurchase); // Составляет чек
void PrintCheque(); // Функция вывода чека
void ChangeCheque(); // Функция изменения чека
void CalculatingDiscounts(); // Функция для подсчета скидок - сделать

void PrintDiscounts(); // Выводит скидки магазина

inline void PauseCls(); // system("pause"); system("cls");

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	CheckingAllFile();

	return 0;
}

void CheckingAllFile()
{
	bool* test = new bool{ false };
	std::string* nameFiles = new std::string{};

	file.open("AllFile.txt");
	if (!file.is_open())
	{
		std::cout << "Простите, произошли технические неполадки";
	}
	else
	{
		while (!file.eof())
		{
			std::getline(file, *nameFiles);
			allFile.push_back(*nameFiles);
		}
		file.close();
		file.open("Categories.txt");
		if (!file.is_open())
		{
			std::cout << "Простите, произошли технические неполадки";
		}
		else
		{
			while (!file.eof())
			{
				std::getline(file, *nameFiles);
				category.push_back(*nameFiles);
			}
			file.close();
			file.open("AllFile.txt");
			for (int i = 0; i < allFile.size(); i++)
			{
				if (OpeningFile(allFile[i]) != false)
				{
					*test = true;
					break;
				}
			}
			if (*test == false)
			{
				delete nameFiles;
				delete test;
				Exit();
			}
			else
			{
				std::cout << "Простите, произошли технические неполадки";
			}
		}
	}
}

bool OpeningFile(std::string fileName)
{
	file.open(fileName);
	if (!file.is_open())
	{
		return true;
	}
	else
	{
		file.close();
		return false;
	}
}

void Exit()
{
	std::cout << "Добро пожаловать в магазин \"Соки Тимофея\"" << std::endl <<
		"Тут вы найдете огромный выбор товаров лучшего и самого Тимофейнего качечества!" << std::endl;
	MchoosingCategory();
}

void MchoosingCategory()
{
	std::string* inputCategory = new std::string{};

	PauseCls();

	while (true)
	{
		std::cout << "Выберите категорию покупки";
		PrintCategory(category);
		std::cout << "Раздел скидок магазина - \"скидки\", показать чек - \"закончить покупки\": ";
		std::getline(std::cin, *inputCategory);
		if (CheckingExistenceCategory(*inputCategory, category))
		{
			MchoosingPurchases(*inputCategory);
			break;
		}
		else if (*inputCategory == "скидки")
		{
			PrintDiscounts();
		}
		else if (*inputCategory == "закончить покупки")
		{
			delete inputCategory;
			PrintCheque();
			break;
		}
		else
		{
			std::cout << "Простите, ваш ввод неверный, введите еще раз" << std::endl;
			delete(inputCategory);
			MchoosingCategory();
			break;
		}
	}
}

void PrintCategory(std::vector<std::string>& vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (i % 2 != 0)
		{
			std::cout << vec[i] << "\t\t";
		}
		else
		{
			std::cout << std::endl << vec[i] << "\t\t";
		}
	}
	std::cout << std::endl;
}

bool CheckingExistenceCategory(std::string& inputCategory, std::vector<std::string>& vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == inputCategory)
		{
			return true;
		}
	}
	return false;
}

void MchoosingPurchases(std::string& inputCategory)
{
	double* productQuantity = new double{};
	std::string* inputPurchases = new std::string{};
	PauseCls();
	СompletionPurchases(inputCategory);

	while (true)
	{
		std::cout << "Выберите покупку: ";
		PrintPurchases(inputCategory);
		std::cout << "Вернуться в выбор категорий - \"выход\", показать чек - \"закончить покупки\": ";
		std::getline(std::cin, *inputPurchases);
		if (CheckingExistencePurchases(*inputPurchases))
		{
			*productQuantity = ProductQuantity();
			CalculatingPrice(*inputPurchases, *productQuantity);
			continue;
		}
		else if (*inputPurchases == "выход")
		{
			purchases.clear();
			priceDouble.clear();
			delete productQuantity;
			delete inputPurchases;
			delete& inputCategory;
			MchoosingCategory();
			break;
		}
		else if (*inputPurchases == "закончить покупки")
		{
			delete productQuantity;
			delete inputPurchases;
			delete& inputCategory;
			PrintCheque();
			break;
		}
		else
		{
			std::cout << "Простите, ваш ввод неверный, введите еще раз" << std::endl; // баг строки
			PauseCls();
		}
	}
}

void СompletionPurchases(std::string& inputCategory) {
	std::string* fileStringCategory = new std::string{};
	std::string* pricesFromFile = new std::string{};
	std::string* firstLinePrices = new std::string{ inputCategory + " prices" };
	double* doublePrices = new double{};
	priceDouble.clear(); //Добавил чистку
	purchases.clear(); //Добавил чистку
	for (int i = 0; i < category.size(); i++)
	{
		file.open(allFile[i]);
		std::getline(file, *fileStringCategory);
		if (*fileStringCategory == inputCategory)
		{
			while (!file.eof())
			{
				std::getline(file, *fileStringCategory);
				purchases.push_back(*fileStringCategory);
			}
			file.close();
			break;
		}
		else
		{
			file.close();
		}
	}
	for (int i = 0; i < allFile.size(); i++)
	{
		file.open(allFile[i]);
		std::getline(file, *pricesFromFile);
		if (*pricesFromFile == *firstLinePrices)
		{
			while (!file.eof())
			{
				std::getline(file, *pricesFromFile);
				*doublePrices = std::stod(*pricesFromFile);
				priceDouble.push_back(*doublePrices);
			}
			file.close();
			break;
		}
		else
		{
			file.close();
		}
	}
	delete fileStringCategory;
	delete pricesFromFile;
	delete firstLinePrices;
	delete doublePrices;
}
void PrintPurchases(std::string& inputCategory)
{
	for (int i = 0; i < purchases.size(); i++)
	{
		if (i % 2 != 0)
		{
			std::cout << purchases[i] << " - " << priceDouble[i] << "р" << "\t\t";
		}
		else
		{
			std::cout << std::endl << purchases[i] << " - " << priceDouble[i] << "р" << "\t\t";
		}
	}
	std::cout << std::endl;
}

bool CheckingExistencePurchases(std::string& inputPurchases)
{
	for (int i = 0; i < purchases.size(); i++)
	{
		if (purchases[i] == inputPurchases)
		{
			return true;
		}
	}
	return false;
}

double ProductQuantity()
{
	double* productQuantity = new double;

	PauseCls();

	while (true)
	{
		std::cout << "Введите кол-во выбранного товара: ";
		std::cin >> *productQuantity;
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
			std::cout << std::endl << "Простите, ваш ввод неверный, введите еще раз" << std::endl;
			PauseCls();
			continue;
		}
		else if (*productQuantity < 1)
		{
			std::cout << "Простите, ваш ввод неверный - нельзя кол-во меньше 1, введите еще раз" << std::endl;
			PauseCls();
			continue;
		}
		else
		{

			return *productQuantity;
			delete productQuantity;
			break;
		}
	}
}

void CalculatingPrice(std::string& inputPurchases, double& productQuantity)
{
	double* priceThisProduct = new double{};
	double* amountPurchase = new double{};
	auto iterator = find(purchases.begin(), purchases.end(), inputPurchases);

	*priceThisProduct = priceDouble[iterator - purchases.begin()];
	*amountPurchase = (priceDouble[iterator - purchases.begin()]) * productQuantity; //cout << it - v.begin();
	totalAmount += *amountPurchase;
	CompilationCheque(inputPurchases, productQuantity, *priceThisProduct, *amountPurchase);

	PauseCls();
}
void CompilationCheque(std::string& inputPurchases, double& productQuantity, double& priceThisProduct, double& amountPurchase)
{

	if (find(chequeStr.begin(), chequeStr.end(), inputPurchases) != chequeStr.end()) // Если объект уже есть в векторе
	{
		auto iterator = find(chequeStr.begin(), chequeStr.end(), inputPurchases);
		int index = std::distance(chequeStr.begin(), iterator);
		chequeDouble[index * 3 + 1] += productQuantity;
		chequeDouble[index * 3 + 2] += amountPurchase;
	}
	else
	{
		chequeStr.push_back(inputPurchases);
		chequeDouble.push_back(priceThisProduct);
		chequeDouble.push_back(productQuantity);
		chequeDouble.push_back(amountPurchase);
		//продукт | цена | кол-во | цена * кол-во
	}
	delete& priceThisProduct;
	delete& amountPurchase;
}
void PrintCheque()
{
	system("cls");
	CalculatingDiscounts(); //функция скидок
	std::cout << "ООО \"Соки Тимофея\"" << std::endl << std::endl;
	for (int i = 0; i < chequeStr.size(); i++)
	{
		std::cout << chequeStr[i] << "\t" << chequeDouble[i * 3] << "\t" << chequeDouble[i * 3 + 1] << " Шт \t" << std::floor(chequeDouble[i * 3 + 2]) << std::endl;
		//продукт | цена | кол-во | цена * кол-во
	}

	std::cout << std::endl << "Сумма покупки - " << std::floor(totalAmountDiscounts) << "р" << std::endl << std::endl;
	ChangeCheque();
}

void ChangeCheque()
{
	int* usersInput = new int{};
	system("pause");
	if (chequeStr.size() == 1)
	{
		std::cout << "Удаление какой то покупки из чека доступна когда в чеке более 1 покупки" << std::endl;
	}
	else
	{
		std::cout << "Если хотите удалить какую то покупку из чека введите соответствующую строку ( от 1 до " << chequeStr.size() << " )" << std::endl;
	}
	std::cout << "Если хотите еще что-то купить \"0\"" << std::endl <<
		"Если хотите оплатить то любой ввод кроме чисел ( Покажутся скидки и нельзя будет менять чек): " << std::endl;
	std::cin >> *usersInput;
	if (std::cin.fail() || *usersInput < 0)
	{
		std::cin.clear();
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

		std::cout << std::endl << "СПАСИБО ЗА ПОКУПКУ!" << std::endl;

		purchases.clear();
		priceDouble.clear();
		chequeStr.clear();
		chequeDouble.clear();
		totalAmount = 0.0;
		delete usersInput;
		Sleep(10000);
		system("cls");
		Exit();
	}
	else if (*usersInput == 0)
	{
		purchases.clear();
		priceDouble.clear();
		delete usersInput;
		MchoosingCategory();
	}
	else if (*usersInput > chequeStr.size())
	{
		std::cout << "Если вы хотели удалить что то из чека, то ввели неверное значение, введите ваш выбор еще раз" << std::endl;
		delete usersInput;
		PauseCls();
		PrintCheque();
	}
	else if (chequeStr.size() == 1 && *usersInput == 1)
	{
		std::cout << "Простите, вы не можете удалить весь чек, введите ваш выбор еще раз" << std::endl;
		delete usersInput;
		PauseCls();
		PrintCheque();
	}
	else
	{
		if (*usersInput == chequeStr.size())
		{
			chequeStr.pop_back();
			totalAmount -= chequeDouble[(*usersInput - 1) * 3 + 2];
			chequeDouble.pop_back();
			chequeDouble.pop_back();
			chequeDouble.pop_back();
		}
		else
		{
			*usersInput -= 1;
			totalAmount -= chequeDouble[*usersInput * 3 + 2];
			chequeStr.erase(chequeStr.begin() + *usersInput);
			chequeDouble.erase(chequeDouble.begin() + *usersInput * 3);
			chequeDouble.erase(chequeDouble.begin() + *usersInput * 3);
			chequeDouble.erase(chequeDouble.begin() + *usersInput * 3);

		}
		delete usersInput;
		PauseCls();
		PrintCheque();
	}

}


void PrintDiscounts()
{
	std::string* printDiscounts = new std::string{};
	file.open("PrintDiscounts.txt");
	if (!file.is_open())
	{
		system("cls");
		std::cout << "Простите, в данный момент невозможно посмотреть скидки магазина" << std::endl;
		PauseCls();
	}
	else
	{
		system("cls");
		std::cout << "Скидки нашего магазина: " << std::endl;
		system("chcp 65001 > NUL");
		while (!file.eof())
		{
			std::getline(file, *printDiscounts);
			std::cout << *printDiscounts << std::endl;
		}
		std::cout << std::endl;
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		file.close();
		delete printDiscounts;
		PauseCls();
	}
}
void CalculatingDiscounts()
{
	std::string* informationFileStr = new std::string{};
	double* informationFileDouble = new double{};
	file.open("Discounts.txt"); // Проверка его открытия еще с самого начала
	while (!file.eof())
	{
		std::getline(file, *informationFileStr);
		if ((std::find(chequeStr.begin(), chequeStr.end(), *informationFileStr)) != chequeStr.end())
		{
			auto iterator = find(chequeStr.begin(), chequeStr.end(), *informationFileStr);
			int index = std::distance(chequeStr.begin(), iterator);
			std::getline(file, *informationFileStr);
			if (*informationFileStr == ">%")
			{
				std::getline(file, *informationFileStr);
				*informationFileDouble = std::stod(*informationFileStr);
				if (chequeDouble[index + 1] >= *informationFileDouble)
				{
					std::getline(file, *informationFileStr);
					*informationFileDouble = std::stod(*informationFileStr);
					totalAmount -= chequeDouble[index + 2];
					chequeDouble[index + 2] *= (((100 - *informationFileDouble) / 100));
					totalAmount += chequeDouble[index + 2];
					continue;
				}
			}
			else if (*informationFileStr == "=f") // Если покупатель не берет 4 товар когда он бесплатный - значит ему не надо 4 штуки, хоть и 4 бесплатная
			{
				std::getline(file, *informationFileStr);
				*informationFileDouble = std::stod(*informationFileStr);
				for (int i = chequeDouble[index + 1]; i >= *informationFileDouble; i -= 4)
				{
					chequeDouble[index + 2] -= chequeDouble[index];
					totalAmount -= chequeDouble[index];
					//продукт | цена | кол-во | цена * кол-во
				}
				continue;

			}
		}
	}
	file.close();
	file.open("Discounts.txt");
	while (!file.eof())
	{
		std::getline(file, *informationFileStr);
		if (*informationFileStr == "receipt amount")
		{
			std::getline(file, *informationFileStr);
			*informationFileDouble = std::stod(*informationFileStr);
			if (totalAmount >= *informationFileDouble)
			{
				std::getline(file, *informationFileStr);
				*informationFileDouble = std::stod(*informationFileStr);
				totalAmountDiscounts = (totalAmount * ((100 - *informationFileDouble) / 100));
			}
			else
			{
				std::getline(file, *informationFileStr);
				totalAmountDiscounts = totalAmount;
			}
		}
	}
	file.close();
}

inline void PauseCls()
{
	system("pause");
	system("cls");
}