// LinkedList.h : subor na vytvaranie spajanych zoznamov
#pragma once

namespace Controller
{
	template <class T> public class LinkedList
	{
	public:
		LinkedList(T* value);
		~LinkedList();

		void InsertToEnd(LinkedList<T>* list);
		unsigned int GetSize();

		T* data;
		LinkedList<T>* next;
	};

	template <class T> LinkedList<T>::LinkedList(T* value)
	{
		data = value;
		next = NULL;
	}

	template <class T> LinkedList<T>::~LinkedList()
	{
		this->next = NULL;
	}

	template <class T> void LinkedList<T>::InsertToEnd(LinkedList<T>* list)
	{
		if(list == NULL)
			return;

		list->next = this->next;
		this->next = list;
		/*LinkedList<T>* tmp = this;
		while(tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = list;*/
	}
	template <class T> unsigned int LinkedList<T>::GetSize()
	{
		unsigned int result = 0;

		LinkedList<T>* tmp = this;
		while(tmp != NULL)
		{
			result++;
			tmp = tmp->next;
		}
		return result;
	}

	template <class T> void DeleteList(LinkedList<T>* list)
	{
		if (list == NULL)
			return;

		LinkedList<T>* tmp1 = list;
		LinkedList<T>* tmp2 = list;
		while(tmp1 != NULL)
		{
			tmp2 = tmp2->next;
			delete tmp1;
			tmp1 = tmp2;
		}
	}
}