#include "FileIO.h"

bool FileIO::Save()
{
	FILE* fpWrite = fopen("Text.txt", "wb");
	// ������(���) ����� �Լ�
	int iCouner = m_List.check();
	fwrite(&iCouner, sizeof(int), 1, fpWrite);
	for (LNode<Student>* pNode = m_List.m_Head->m_Next; pNode != m_List.m_Tail; pNode = pNode->m_Next)
	{
		fwrite(pNode->m_iData, sizeof(Student), 1, fpWrite);
	}
	fclose(fpWrite);
	return true;
}

bool FileIO::Load()
{
	FILE* fpRead = fopen("Text.txt", "rb");
	int Count = 0;


	if (fpRead == NULL)
	{
		cout << "�ε� ������ ����!!!" << endl;
	}
	else
	{
		fread(&Count, sizeof(int), 1, fpRead);

		for (int iAdd = 0; iAdd < Count; iAdd++)
		{
			Student* pStudent = new Student();
			memset(pStudent, 0, sizeof(LNode<Student>));
			if (!(fread(pStudent, sizeof(Student), 1, fpRead)))
			{
				break;
			}
			m_List.Add(pStudent);
		}

		fclose(fpRead);
		fpRead = NULL;
	}

	return true;
}