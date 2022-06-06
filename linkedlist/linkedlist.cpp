#include"LLinkedlisk.h"

int main()
{
    LLinkedlisk L;
    for (int i = 0; i < 5; i++)
    {
        LNode* j = new LNode;
        j->m_iValue = i;
        L.Addl(j);    
    }
    L.Show();
    for (int i = 0; i < 5; i++)
    {
        cout << L.Del() << endl;
    }
    
}
