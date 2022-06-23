#include <iostream>
#include <sstream>
#include <functional>

class HashTable 
{
public:
	static const int Size = 26;
	struct Entry
	{
		std::string Data;
		std::string Status = "never used";
	};

public:
	HashTable() = delete;
	HashTable(std::function<int(std::string)> indexing_function)
	{
		func = indexing_function;
	}

	void Add(const std::string& entryString)
	{
		int insertIndex = GetInsertIndex(entryString);
		if (insertIndex == -1)
			return;
		m_Entries[insertIndex].Data = entryString;
		m_Entries[insertIndex].Status = "occupied";
	}

	void Delete(const std::string& entryString)
	{
		int deleteIndex = GetDeleteIndex(entryString);
		if (deleteIndex == -1)
			return;
		m_Entries[deleteIndex].Status = "tombstone";
	}

	void Print() 
	{
		for (int i = 0; i < Size; ++i)
		{
			//Print in table view
			//std::cout << m_Entries[i].Data << " (" << m_Entries[i].Status << ")" << " " << std::endl;
			if(m_Entries[i].Status == "occupied")
				std::cout << m_Entries[i].Data << " ";
		}
		std::cout << std::endl;
	}
private:
	int GetIndex(const std::string& entryString)
	{
		return func(entryString) % Size;
	}

	int GetDeleteIndex(const std::string& entryString)
	{
		int index = GetIndex(entryString);
		int fixedIndex = index;
		do
		{
			if (m_Entries[index].Data == entryString)
				return index;

			index = (index + 1) % Size;
		} while (fixedIndex != index);
		return -1;
	}

	int GetInsertIndex(const std::string& entryString)
	{
		int index = GetIndex(entryString);
		int fixedIndex = index;
		do
		{
			if (m_Entries[index].Status == "never used" || m_Entries[index].Status == "tombstone")
				return index;
			
			index = (index + 1) % Size;
		} while (fixedIndex != index);
		return -1;
	}
private:
	Entry m_Entries[Size];
	std::function<int(std::string)> func;
};


int main()
{
	//Example
	//Hash the word from last letter 
    //Input: Acar Abar Dcar Ashop Apaper
	//Output: shop paper bar

	std::string input;
	getline(std::cin, input);

	//This function must return value 0-25
	std::function<int(std::string)> func = [](const std::string& entryString) {return entryString.back() - 'a'; };
	HashTable hashTable(func);

	std::stringstream ss(input);
	while (ss.good())
	{
		std::string token;

		ss >> token;

		std::string entryName = token.substr(1);
		if (token[0] == 'A')
			hashTable.Add(entryName);
		if (token[0] == 'D')
			hashTable.Delete(entryName);
	}

	hashTable.Print();
}
