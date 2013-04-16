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

		void Preallocate(unsigned int velkost);
		void InsertToEnd(T* value);
		void InsertToStart(T* value);
		bool Contains(T* value);
		void Clear();									// only clears actual stuff
		void CompleteClear();							// clears preallocation too
		void CompleteDelete();							// clears and deletes everything
		unsigned int GetSize();

		template <class T> class Cell
		{
			public:
				T* data;
				Cell<T>* next;							// next for WHILE loops
				Cell<T>* rnext;							// real next, in case of prealocated array
		};

		Cell<T>* start;									// actual start for prealocated arrays
		Cell<T>* end;									// actual end for prealocated arrays

	private:
		Cell<T>* r_start;								// real start
		Cell<T>* r_end;									// real end
		bool prealocated;
		unsigned int size;
	};

	template <class T> LinkedList<T>::LinkedList()
	{
		start = NULL;
		r_start = NULL;
		end = NULL;
		r_end = NULL;
		prealocated = false;
		size = 0;
	}

	template <class T> LinkedList<T>::LinkedList(T* value)
	{
		Cell<T>* tmp = new Cell<T>();
		tmp->data = value;
		tmp->next = NULL;
		tmp->rnext = NULL;
		start = tmp;
		r_start = tmp;
		end = tmp;
		r_end = tmp;
		prealocated = false;
		size = 1;
	}

	template <class T> LinkedList<T>::~LinkedList()
	{
		CompleteClear();
	}

	template <class T> void LinkedList<T>::Preallocate(unsigned int velkost)
	{
		CompleteClear();
		prealocated = true;
		Cell<T>* prev = NULL;
		for(unsigned int i = 0; i < velkost; i++)
		{
			Cell<T>* tmp = new Cell<T>();
			tmp->data = NULL;
			tmp->next = NULL;
			tmp->rnext = NULL;
			if(prev != NULL)
				prev->rnext = tmp;
			if(i==0)
				r_start = tmp;
			prev = tmp;
		}
		size = 0;						// preallocation does not takes size
		start = NULL;
		end = NULL;
		r_end = prev;
	}
	template <class T> void LinkedList<T>::InsertToEnd(T* value)
	{
		if(prealocated == true)
		{
			if(size == 0)
			{
				Cell<T>* tmp = r_start;
				tmp->data = value;
				tmp->next = NULL;
				start = r_start;
				end = r_start;
			}
			else
			{
				Cell<T>* tmp = end->rnext;
				if(tmp == NULL)
					return;
				end->next = tmp;
				tmp->data = value;
				tmp->next = NULL;
				end = tmp;
			}
		}
		else
		{
			Cell<T>* tmp = new Cell<T>();
			tmp->data = value;
			tmp->next = NULL;
			tmp->rnext = NULL;
			if(size == 0)
			{
				start = tmp;
				r_start = tmp;
				end = tmp;
				r_end = tmp;
			}
			else
			{
				end->next = tmp;
				end->rnext = tmp;
				end = tmp;
				r_end = tmp;
			}
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
		if(prealocated == true)
		{
			if(size == 0)
			{
				Cell<T>* tmp = r_start;
				tmp->data = value;
				tmp->next = NULL;
				start = r_start;
				end = r_start;
			}
			else
			{
				Cell<T>* tmp = end->rnext;
				if(tmp == NULL)
					return;
				//end->next = NULL; malo by ostat NULL
				end->rnext = tmp->rnext;
				tmp->data = value;
				tmp->next = start;
				tmp->rnext = start;
				start = tmp;
				r_start = tmp;
			}
		}
		else
		{
			Cell<T>* tmp = new Cell<T>();
			tmp->data = value;
			tmp->next = NULL;
			tmp->rnext = NULL;
			if(size == 0)
			{
				start = tmp;
				r_start = tmp;
				end = tmp;
				r_end = tmp;
			}
			else
			{
				tmp->next = start;
				tmp->rnext = start;
				start = tmp;
				r_start = tmp;
			}
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
			if(prealocated == true)
			{
				tmp1->data = NULL;
				tmp1->next = NULL;
			}
			else
				delete tmp1;
			tmp1 = tmp2;
		}
		size = 0;
		start = NULL;
		end = NULL;
	}

	// vymaze len bunky a zmaze preallokaciu
	template <class T> void LinkedList<T>::CompleteClear()
	{
		if(size == 0)
			return;

		Cell<T>* tmp1 = r_start;
		Cell<T>* tmp2 = r_start;
		while(tmp1 != NULL)
		{
			tmp2 = tmp2->rnext;
			delete tmp1;
			tmp1 = tmp2;
		}
		size = 0;
		start = NULL;
		end = NULL;
		r_start = NULL;
		r_end = NULL;
		prealocated = false;
	}

	// vymaze aj data
	template <class T> void LinkedList<T>::CompleteDelete()
	{
		if(size == 0)
			return;

		Cell<T>* tmp1 = r_start;
		Cell<T>* tmp2 = r_start;
		while(tmp1 != NULL)
		{
			tmp2 = tmp2->rnext;
			if(tmp1->data != NULL)
				delete tmp1->data;
			delete tmp1;
			tmp1 = tmp2;
		}
		size = 0;
		start = NULL;
		end = NULL;
		r_start = NULL;
		r_end = NULL;
		prealocated = false;
	}

	template <class T> unsigned int LinkedList<T>::GetSize()
	{
		return size;
	}
}