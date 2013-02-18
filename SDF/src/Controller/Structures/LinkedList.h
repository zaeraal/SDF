// LinkedList.h : subor na vytvaranie spajanych zoznamov
#pragma once

namespace Structures
{
	template <class T> public class LinkedList
	{
	public:
		LinkedList();
		LinkedList(T* value);
		~LinkedList();

		void InsertToEnd(T* value);
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