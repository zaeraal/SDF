// LinkedList.h : subor na vytvaranie spajanych zoznamov
#pragma once

namespace GenericStructures
{
	template <class T> public class LinkedList
	{
	public:
		LinkedList();
		LinkedList(T* value);
		~LinkedList();

		void InsertToEnd(T* value);
		void InsertToStart(T* value);
		bool Contains(T* value);
		void Clear();
		void CompleteDelete();
		unsigned int GetSize();

		template <class T> class Cell
		{
			public:
			T* data;
			Cell<T>* next;
		};

		Cell<T>* start;
		Cell<T>* end;
	private:
		unsigned int size;
	};

	template <class T> LinkedList<T>::LinkedList()
	{
		start = NULL;
		end = NULL;
		size = 0;
	}

	template <class T> LinkedList<T>::LinkedList(T* value)
	{
		Cell<T>* tmp = new Cell<T>();
		tmp->data = value;
		tmp->next = NULL;
		start = tmp;
		end = tmp;
		size = 1;
	}

	template <class T> LinkedList<T>::~LinkedList()
	{
		Clear();
	}

	template <class T> void LinkedList<T>::InsertToEnd(T* value)
	{
		Cell<T>* tmp = new Cell<T>();
		tmp->data = value;
		tmp->next = NULL;
		if(size == 0)
		{
			start = tmp;
			end = tmp;
		}
		else
		{
			end->next = tmp;
			end = tmp;
		}
		size++;
	}
	
	template <class T> bool LinkedList<T>::Contains(T* value)
	{
		Cell<T>* tmp = start;
		while(tmp != NULL)
		{
			if(tmp->data == value)
				return true;
			tmp = tmp->next;
		}
		return false;
	}

	template <class T> void LinkedList<T>::InsertToStart(T* value)
	{
		Cell<T>* tmp = new Cell<T>();
		tmp->data = value;
		tmp->next = NULL;
		if(size == 0)
		{
			start = tmp;
			end = tmp;
		}
		else
		{
			tmp->next = start;
			start = tmp;
		}
		size++;
	}

	// vymaze len bunky
	template <class T> void LinkedList<T>::Clear()
	{
		if(size == 0)
			return;

		Cell<T>* tmp1 = start;
		Cell<T>* tmp2 = start;
		for(unsigned int i = 0; i < size; i++)
		{
			tmp2 = tmp2->next;
			delete tmp1;
			tmp1 = tmp2;;
		}
		size = 0;
		start = NULL;
		end = NULL;
	}

	// vymaze aj data
	template <class T> void LinkedList<T>::CompleteDelete()
	{
		if(size == 0)
			return;

		Cell<T>* tmp1 = start;
		Cell<T>* tmp2 = start;
		for(unsigned int i = 0; i < size; i++)
		{
			tmp2 = tmp2->next;
			delete tmp1->data;
			delete tmp1;
			tmp1 = tmp2;;
		}
		size = 0;
		start = NULL;
		end = NULL;
	}

	template <class T> unsigned int LinkedList<T>::GetSize()
	{
		return size;
	}
}