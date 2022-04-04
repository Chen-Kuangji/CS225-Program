#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "listkmp.h"
using std::cout;
using std::cin;


template<class M> M Listext<M>::operator[](int index)
{
    return reprarray[index];
}

template<class M> int Listext<M>::getlength(void)
{
    return numitems;
}

template<class M> void Listext<M>::setitem(int index, T *person_info)
{
    if ((index >= 0) && (index < numitems))
    {
        reprarray[index] = *person_info;
        return;
    }
    else
        cout << "Index error: index out or range\n";
}

template<class M> M Listext<M>::getitem(int index)
{
    if ((index >= 0) && (index < numitems))
    {
        return reprarray[index];
    }
    else
    {
        cout << "Index error: index out or range\n";
        exit(EXIT_FAILURE);
    }
}

template<class M> void Listext<M>::append(T *person_info)
{
    if (numitems == maxsize)
        allocate();
    reprarray[numitems-1] = *person_info;
    ++numitems;
    return;
}

template<class M> void Listext<M>::insert(int index, T *person_info)
{
    if (numitems == maxsize)
        allocate();
    if (index < numitems)
    {
        for (int j = numitems - 1; j > index; --j)
        {
            reprarray[j] = reprarray[j-1];
        }
        reprarray[index] = *person_info;
        ++numitems;
        return;
    }
    else
    {
        append(*person_info);
        return;
    }
}

template<class M> void Listext<M>::allocate(void)
{
    int newsize = 2 * maxsize;
    T *newarray = new T[newsize];
    for (int i = 0; i < numitems; ++i)
    {
        newarray[i] = reprarray[i];
    }
    delete[] reprarray;
    reprarray = newarray;
    maxsize = newsize;
    return;
}

template<class M> void Listext<M>::remove(int index)
{
    if ((numitems == maxsize / 4) && (maxsize > minsize))
        deallocate();
    if (index < numitems)
    {
        for (int j = index; j < numitems; ++j)
        {
            reprarray[j-1] = reprarray[j];
        }
        --numitems;
        return;
    }
    else
        cout << "Index error: list index out of range\n";
}

template<class M> void Listext<M>::deallocate(void)
{
    int newsize = maxsize / 2;
    T *newarray = new T[newsize];
    for (int i = 0; i < numitems; ++i)
    {
        newarray[i] = reprarray[i];
    }
    delete[] reprarray;
    reprarray = newarray;
    maxsize = newsize;
    return;
}

template<class M> void Listext<M>::concat(Listext<M> &list)
{
    int length = list.getlength();
    for (int i = 0; i < length; ++i)
    {
        append(list[i]);
    }
    return;
}

template<class M> bool Listext<M>::member(T *person_info)
{
    bool result = false;
    for (int i = 0; i < numitems; ++i)
    {
        if (reprarray[i] == value)
        {
            result = true;
            break;
        }
    }
    return result;
}

template<class M> bool Listext<M>::equal(Listext<M> &list)
{
    bool result = true;
    if (numitems == list.getlength())
    {
        for (int i = 0; i < numitems; ++i)
        {
            if (reprarray[i] == list[i + 1])
                continue;
            else
            {
                result = false;
                break;
            }
        }
    }
    else
        result = false;
    return result;
}

template<class M> bool Listext<M>::sublist(Listext<M> &list)
{
    int length = list.getlength();
    bool result = true;
    for (int i = 0, j = 0; (i < length) && (j < numitems); ++i, ++j)
    {
        if (list[i] == reprarray[j])
        {
            if ((j == numitems - 1) && (i < length))
            {
                result = false;
                break;
            }
        }
        else
            if (j == numitems - 1)
            {
                result = false;
                break;
            }
            else
                --i;
    }
    return result;
}

template<class M> void Listext<M>::prettyprint(void)
{
    for (int i = 0; i < numitems; i++)
    {
        cout << i << ":  " << reprarray[i];
        if (i != numitems - 1)
            cout << "; ";
        cout << "\n";
    }
}
