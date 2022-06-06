#include "Sample.h"
Sample s;

void Sample::GredeManagement()
{
    int iselect = 0;
    m_bRun = true;

    m_File.m_List.Init();

    while (m_bRun)
    {
        string name = ""; int kor = 0; int mat = 0; int eng = 0;
        cout << "1.Add 2.Deletepop 3.Show 4.Save 5.Load 6.DeleteAll&Quit :";
        cin >> iselect;
        system("cls");
        if (iselect == 6)
        {
            break;
        }
        switch (iselect)
        {
        case 1:
        {
            cout << "이름:" << " ";
            cin >> name;
            cout << "국어점수:" << " ";
            cin >> kor;
            cout << "수학점수:" << " ";
            cin >> mat;
            cout << "영어점수" << " ";
            cin >> eng;
            Student* s = new Student(name, kor, mat, eng);

            m_File.m_List.Add(s);
            m_File.m_List.Show();
        }break;
        case 2:
        {
            m_File.m_List.Delpop();
            m_File.m_List.Show();
        }break;
        case 3:
        {
            m_File.m_List.Show();
        }break;
        case 4:
        {
            m_File.Save();
            cout << "저장완료!" << endl;
        }break;
        case 5:
        {
            m_File.Load();
           
        }break;

        }
    }
    m_File.m_List.DelAll();
    m_File.m_List.~Linkedlist();
}

int main()
{
    s.GredeManagement();
}