#include "Frustum.h"

bool Frustum::Init()
{
   
    m_vFrustum[0] = TVector3(-1.0f, -1.0f, 0.0f);
    m_vFrustum[1] = TVector3(-1.0f, 1.0f, 0.0f);
    m_vFrustum[2] = TVector3(1.0f, 1.0f, 0.0f);
    m_vFrustum[3] = TVector3(1.0f, -1.0f, 0.0f);

    m_vFrustum[4] = TVector3(-1.0f, -1.0f, 1.0f);
    m_vFrustum[5] = TVector3(-1.0f, 1.0f, 1.0f);
    m_vFrustum[6] = TVector3(1.0f, 1.0f, 1.0f);
    m_vFrustum[7] = TVector3(1.0f, -1.0f, 1.0f);

    return true;
}

bool Frustum::PostRender(ID3D11DeviceContext* pContext)
{
    if (m_indexList.size() > 0)
    {
        pContext->DrawIndexed(m_indexList.size(), 0, 0);
    }
    else
    {
        pContext->Draw(m_vertexList.size(), 0);
    }
    return true;
}

bool Frustum::CreateFrustum(TMatrix& matView, TMatrix& matProj)
{
    m_vFrustum[0] = TVector3(-1.0f, -1.0f, 0.0f);
    m_vFrustum[1] = TVector3(-1.0f, 1.0f, 0.0f);
    m_vFrustum[2] = TVector3(1.0f, 1.0f, 0.0f);
    m_vFrustum[3] = TVector3(1.0f, -1.0f, 0.0f);

    m_vFrustum[4] = TVector3(-1.0f, -1.0f, 1.0f);
    m_vFrustum[5] = TVector3(-1.0f, 1.0f, 1.0f);
    m_vFrustum[6] = TVector3(1.0f, 1.0f, 1.0f);
    m_vFrustum[7] = TVector3(1.0f, -1.0f, 1.0f);

    TMatrix matInverse;
    D3DXMatrixMultiply(&matInverse, &matView, &matProj);
    D3DXMatrixInverse(&matInverse, NULL, &matInverse);
    for (int i = 0; i < 8; i++)
    {
        //����(1x3) ���Ϳ� w=1.0f �� �߰��ؼ� ���(4x4) �������� �Լ�
        D3DXVec3TransformCoord(&m_vFrustum[i], &m_vFrustum[i], &matInverse);
    }

    if (m_vertexList.size() > 0)
    {
        // z+
        m_vertexList[0].pos = m_vFrustum[6];
        m_vertexList[1].pos = m_vFrustum[5];
        m_vertexList[2].pos = m_vFrustum[7];
        m_vertexList[3].pos = m_vFrustum[4];

        // z-
        m_vertexList[4].pos = m_vFrustum[1];
        m_vertexList[5].pos = m_vFrustum[2];
        m_vertexList[6].pos = m_vFrustum[0];
        m_vertexList[7].pos = m_vFrustum[3];

        //x+
        m_vertexList[8].pos = m_vFrustum[2];
        m_vertexList[9].pos = m_vFrustum[6];
        m_vertexList[10].pos = m_vFrustum[3];
        m_vertexList[11].pos = m_vFrustum[7];

        //x-
        m_vertexList[12].pos = m_vFrustum[5];
        m_vertexList[13].pos = m_vFrustum[1];
        m_vertexList[14].pos = m_vFrustum[4];
        m_vertexList[15].pos = m_vFrustum[0];
        
        //y+
        m_vertexList[16].pos = m_vFrustum[0];
        m_vertexList[17].pos = m_vFrustum[3];
        m_vertexList[18].pos = m_vFrustum[4];
        m_vertexList[19].pos = m_vFrustum[7];

        //y-
        m_vertexList[20].pos = m_vFrustum[5];
        m_vertexList[21].pos = m_vFrustum[6];
        m_vertexList[22].pos = m_vFrustum[1];
        m_vertexList[23].pos = m_vFrustum[2];

        m_pContext->UpdateSubresource(m_VertexBuffer, 0, NULL, &m_vertexList.at(0), 0, 0);
    }

 

   // 4   5
   //
   // 6   7

     // 0   1
     //
     // 2   3
    
    // z-
    m_Plane[0] = TPlane(m_vFrustum[0], m_vFrustum[1], m_vFrustum[2]);

    //z+
    m_Plane[1] = TPlane(m_vFrustum[6], m_vFrustum[5], m_vFrustum[4]);

    //x-
    m_Plane[2] = TPlane(m_vFrustum[5], m_vFrustum[1], m_vFrustum[0]);

    //x+
    m_Plane[3] = TPlane(m_vFrustum[2], m_vFrustum[6], m_vFrustum[7]);

    //y-
    m_Plane[4] = TPlane(m_vFrustum[7], m_vFrustum[4], m_vFrustum[0]);

    //y+
    m_Plane[5] = TPlane(m_vFrustum[5], m_vFrustum[6], m_vFrustum[1]);

    return true;
}

bool Frustum::CreateVertexData()
{

    m_vertexList.resize(24);
    //�޸�(n z+) 456 657
    m_vertexList[0] = PNCT_VERTEX(TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[1] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[2] = PNCT_VERTEX(TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));
    m_vertexList[3] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, 0.0f, 1.0f), TVector4(0.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));


    //����(n z-) 012 213
    m_vertexList[4] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[5] = PNCT_VERTEX(TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[6] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));
    m_vertexList[7] = PNCT_VERTEX(TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, 0.0f, -1.0f), TVector4(1.0f, 0.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));


    //������(n x+) 8910 10911
    m_vertexList[8] = PNCT_VERTEX(TVector3(1.0f, 1.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[9] = PNCT_VERTEX(TVector3(1.0f, 1.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[10] = PNCT_VERTEX(TVector3(1.0f, -1.0f, -1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));
    m_vertexList[11] = PNCT_VERTEX(TVector3(1.0f, -1.0f, 1.0f), TVector3(1.0f, 0.0f, 0.0f), TVector4(0.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));

    //����(n x-) 12 13 14 14 13 15
    m_vertexList[12] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[13] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, -1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[14] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, 1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));
    m_vertexList[15] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, -1.0f), TVector3(-1.0f, 0.0f, 0.0f), TVector4(1.0f, 0.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));

    //����(n y+) 16 17 18 18 17 20
    m_vertexList[16] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[17] = PNCT_VERTEX(TVector3(1.0f, 1.0f, 1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[18] = PNCT_VERTEX(TVector3(-1.0f, 1.0f, -1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(0.0f, 1.0f));
    m_vertexList[19] = PNCT_VERTEX(TVector3(1.0f, 1.0f, -1.0f), TVector3(0.0f, 1.0f, 0.0f), TVector4(1.0f, 1.0f, 0.0f, 1.0f), TVector2(1.0f, 1.0f));

    //�Ʒ���(n y-)
    m_vertexList[20] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, -1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 0.0f));
    m_vertexList[21] = PNCT_VERTEX(TVector3(1.0f, -1.0f, -1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 0.0f));
    m_vertexList[22] = PNCT_VERTEX(TVector3(-1.0f, -1.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(0.0f, 1.0f));
    m_vertexList[23] = PNCT_VERTEX(TVector3(1.0f, -1.0f, 1.0f), TVector3(0.0f, -1.0f, 0.0f), TVector4(1.0f, 1.0f, 1.0f, 1.0f), TVector2(1.0f, 1.0f));

    return true;
}

bool Frustum::CreateIndexData()
{
    m_indexList.push_back(0);  m_indexList.push_back(1);  m_indexList.push_back(2);
    m_indexList.push_back(2);  m_indexList.push_back(1);  m_indexList.push_back(3);

    m_indexList.push_back(4);  m_indexList.push_back(5);  m_indexList.push_back(6);
    m_indexList.push_back(6);  m_indexList.push_back(5);  m_indexList.push_back(7);

    m_indexList.push_back(8);  m_indexList.push_back(9);  m_indexList.push_back(10);
    m_indexList.push_back(10);  m_indexList.push_back(9);  m_indexList.push_back(11);

    m_indexList.push_back(12);  m_indexList.push_back(13);  m_indexList.push_back(14);
    m_indexList.push_back(14);  m_indexList.push_back(13);  m_indexList.push_back(15);

    m_indexList.push_back(16);  m_indexList.push_back(17);  m_indexList.push_back(18);
    m_indexList.push_back(18);  m_indexList.push_back(17);  m_indexList.push_back(19);

    m_indexList.push_back(20);  m_indexList.push_back(21);  m_indexList.push_back(22);
    m_indexList.push_back(22);  m_indexList.push_back(21);  m_indexList.push_back(23);

    return true;
}
//6�� ��鿡 ��� �������� �ȿ� �����ϸ� Ʈ��
//�Ѱ��� ������ false
bool Frustum::FrustuminPoint(TVector3* point)
{
    for (int iPlane = 0; iPlane < 6; iPlane++)
    {
        float planeTocircle = m_Plane[iPlane].x * point->x + m_Plane[iPlane].y * point->y + m_Plane[iPlane].z * point->z + m_Plane[iPlane].w;
        if (planeTocircle > 0.0f)
        {
            return false;
        }

    }
    return true;
}
//����������
//bool Frustum::FrustuminSphere(Sphere* sphere)
//{
//    for (int iPlane = 0; iPlane < 6; iPlane++)
//    {
//        float planeTocircle = m_Plane[iPlane].x * sphere->vCenter + m_Plane[iPlane].y * sphere->vCenter + m_Plane[iPlane].z * sphere->vCenter + m_Plane[iPlane].w;
//        if (planeTocircle >= sphere->fRadius)
//        {
//            return false;
//        }
//
//    }
//    return true;
//}

//�࿡ ���ĵǾ��ִ� �ٿ���ڽ�
//min , max �� 2����������ȴ�.
//x = 1,0,0
//y = 0,1,0
//z = 0,0,1
bool Frustum::FrustuminAABB(Box* aabb)
{
    return true;
}

//�ȿ� �ִ���, �����ִ���, �ۿ��ִ���

//�࿡ ���� ������ �ڽ� OBB
//x,y,z ����(��)
//���� (����)
//x,y,z�����κ��� �������� �Ÿ�
//������ �κа� �������� �Ÿ��� �������Ѵ�.
//n,x ����
//n,y ����
//n,z ���� ����ġ�� �Ÿ��� ���´�
bool Frustum::FrustuminOBB(Box* obb)
{
    TVector3 vDir = obb->vAxis[0] * obb->vSize.x;
    return true;
}




