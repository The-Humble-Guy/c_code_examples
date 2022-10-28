#include <iostream>
#include <string.h>
#include <windows.h>
#include <math.h>
using namespace std;

struct element
{
    char *word;
    element *next;
    element *prev;
};

void print(element *first)
{
    for (element* tmp = first; tmp != nullptr; tmp = (tmp->next))
        cout << tmp->word << endl;
}

element* StrToList(char* str)
{
    element *temp = nullptr;
    static element *previous = nullptr;
    if (strlen(str)>0)
    {
        temp = new element;
        char* tmpStr = new char[255];
        int i;
        for (i=0; ((str[i] != ' ') && (str[i] != '\0')); i++)
            tmpStr[i] = str[i];
        tmpStr[i] = '\0';
        (temp->word) = tmpStr;
        (temp->prev) = previous;
        previous = temp;
        if (str[i] != '\0')
            (temp->next) = StrToList(&str[i+1]);
        else (temp->next)=nullptr;
    };
    return temp;
}

void get_pos(char symbol, element* first, element* &result, int &pos)
{
    pos = -1;
    element* tmp=first;
    char* property = new char[255];
    bool flag = true;
    int i=0;
    while (flag && (tmp!=nullptr))
    {
        property = tmp->word;
        if (property[0] == symbol)
        {
            pos = i;
            flag = false;
        }
        else
        {
            tmp = tmp->next;
            i++;
        }
    };
    result = tmp;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char *str = new char[255];
    cout << "Введите строку длины <255" << endl;
    cin.getline(str, 255);
    element *temp = StrToList(str);
    cout << "Сформированный список:" << endl;
    print(temp);
    element* p=nullptr;
    element* q=nullptr;
    int k,t,n;
    k=t=-1;
    get_pos('к', temp, p, k);
    get_pos('т', temp, q, t);
    n = abs(k-t);

    element* pp=nullptr;
    element* pn=nullptr;
    element* qp=nullptr;
    element* qn=nullptr;

    if ((k != -1) && (t != -1))
    {
        pp=p->prev;
        pn=p->next;
        qp=q->prev;
        qn=q->next;

    if (n==1)
    {
        if ((qp == p) && (pn == q))
        {
            if (pp != nullptr)
                (pp->next) = q;
            (q->next) = p;
            (p->next) = qn;
            (q->prev) = pp;
            (qn->prev) = p;
            (p->prev) = q;
            if (pp == nullptr)
                temp=q;
        }
        else
        {
            if (qp != nullptr)
                (qp->next) = p;
            (p->next) = q;
            (q->next) = pn;
            (p->prev) =qp;
            (q->prev) = p;
            (pn->prev) = q;
            if (qp == nullptr)
                temp = p;
        };
    };

    if ((n == 2) && ( (k+t) == n))
    {
        if (k<t)
        {
            if (pp != nullptr)
                (pp->next) = q;
            (q->next) = pn;
            (pn->next) = p;
            (p->next) = qn;
            (q->prev) = pp;
            (pn->prev) = q;
            (p->prev) = pn;
            if (qn != nullptr)
                (qn->prev) = p;
            if (pp == nullptr)
                temp =q;
        }
        else
        {
            if (qp != nullptr)
                (qp->next) = p;
            (p->next) = qn;
            (qn->next) = q;
            (q->next) = pn;
            (p->prev) = qp;
            (qn->prev) = p;
            (q->prev) = qn;
            if (pn != nullptr)
                (pn->prev) = q;
            if (qp == nullptr)
                temp = p;
        }

    };

    if (n>2)
    {
        if (pp != nullptr)
            (pp->next) = q;
        (q->next) = pn;
        if (qp != nullptr)
            (qp->next) = p;
        (p->next)=qn;
        (q->prev)=pp;
        if (pn != nullptr)
            (pn->prev)=q;
        (p->prev)=qp;
        if (qn != nullptr)
            (qn->prev)=p;
        if (pp == nullptr)
            temp = q;
        if (qp == nullptr)
            temp = p;
    }
    }

    if ((k == -1) || (t == -1))
        cout << "Нужных слов не найдено" << endl;
    else
        {
            cout << endl << endl << endl << "Изменённый список:"<< endl;
            print(temp);
        }

    while (temp != nullptr)
    {
        element* temp2 = temp;
        temp = temp->next;
        delete temp2;
    }

}
