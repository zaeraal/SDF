// HashTable.h : subor na vytvaranie hasovacej tabulky
#pragma once

namespace GenericStructures
{
	template <class T> public class HashTable
	{
	public:
		HashTable(unsigned int velkost);
		~HashTable();

		void Insert(T* value);
		bool Contains(T* value);
		bool Contains(unsigned int poradie, T* value);
		void Delete(T* value);
		void Clear();									// only clears actual stuff
		void CompleteDelete();							// clears and deletes everything
		unsigned int Evaluate(unsigned int num);
		unsigned int GetSize();

		T** start;
		unsigned int prealocated;

	private:
		unsigned int size;
	};

	template <class T> HashTable<T>::HashTable(unsigned int velkost)
	{
		prealocated = velkost;
		start = new T* [velkost];
		for(unsigned int i = 0; i < prealocated; i++)
			start[i] = NULL;
		size = 0;
	}


	template <class T> HashTable<T>::~HashTable()
	{
		Clear();
		delete [] start;
	}

	template <class T> void HashTable<T>::Insert(T* value)
	{
		if(size >= prealocated)
			return;

		unsigned int poradie = Evaluate((unsigned int)value);
		for(unsigned int i = 0; i < prealocated; i++)
		{
			if(start[poradie] == NULL)
			{
				start[poradie] = value;
				break;
			}
			if(Contains(poradie, value) == true)
			{
				// uz mame hodnotu
				return;
			}
			else
			{
				// nieco ine tam uz je
				poradie++;
				poradie = poradie % prealocated;
			}
		}
		size++;
	}
	
	template <class T> bool HashTable<T>::Contains(T* value)
	{
		if(size == 0)
			return false;

		for(uinsigned int i = 0; i < prealocated; i++)
		{
			if(start[i] == value)
				return true;
		}
		return false;
	}

	template <class T> bool HashTable<T>::Contains(unsigned int poradie, T* value)
	{
		if(start[poradie] == value)
			return true;
		return false;
	}

	template <class T> void HashTable<T>::Delete(T* value)
	{
		if(size == 0)
			return;

		unsigned int poradie = Evaluate((unsigned int)value);
		for(unsigned int i = 0; i < prealocated; i++)
		{
			if(start[poradie] == NULL)
			{
				// uz nemame este v tabulke
				return;
			}
			if(Contains(poradie, value) == true)
			{
				start[poradie] = NULL;
				break;
			}
			else
			{
				// nieco ine tam uz je
				poradie++;
				poradie = poradie % prealocated;
			}
		}
		size--;
	}

	// vymaze len bunky
	template <class T> void HashTable<T>::Clear()
	{
		if(size == 0)
			return;

		for(unsigned int i = 0; i < prealocated; i++)
			start[i] = NULL;
		size = 0;
	}

	// vymaze aj data
	template <class T> void HashTable<T>::CompleteDelete()
	{
		if(size == 0)
			return;

		for(uinsigned int i = 0; i < prealocated; i++)
		{
			if(start[i] != NULL)
				delete start[i];
			start[i] = NULL;
		}
		size = 0;
	}

	// jednoducha hashovacia funkcia
	template <class T> unsigned int HashTable<T>::Evaluate(unsigned int num)
	{
		return num % prealocated;
	}

	template <class T> unsigned int HashTable<T>::GetSize()
	{
		return size;
	}
}