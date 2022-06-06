#pragma once
#include"Object3D.h"


struct Box
{
    TVector3 vList[8];
    TVector3 vMin;
    TVector3 vMax;
    TVector3 vCenter;
    TVector3 vAxis[3];
    TVector3 vSize;

    bool operator == (const Box& box)
    {
        if (fabs((vMin - box.vMin).Length()) < 0.0001f)
        {
            if (fabs((vMax - box.vMax).Length()) < 0.0001f)
            {
                return true;
            }
        }
        return false;
    }
    Box() {};
    Box(TVector3 vMin, TVector3 vMax)
    {
        this->vMin = vMin;
        this->vMax = vMax;
        vCenter = (vMax + vMin);
        vCenter /= 2.0f;
        vSize.x = vMax.x - vMin.x;
        vSize.y = vMax.y - vMin.y;
    }
    Box(TVector3 v, float w, float h, float q)
    {
        this->vMin = v;
        this->vMax = vMin + TVector3(w, h, q);
        vCenter = (vMax + vMin);
        vCenter /= 2.0f;
        this->vSize.x = w;
        this->vSize.y = h;
    }
};
struct Sphere
{
    TVector3 vCenter;
    float    fRadius;
    Sphere()
    {
        fRadius = 3.0f;
    }
};

class LineShape : public Object3D
{
public:
    TVector3 m_vStart;
    TVector3 m_vEnd;
    TVector4 m_vColor;
    UINT m_iVertexSize; 
    vector<PS_VECTOR> m_LineVertexList;
    D3D_PRIMITIVE_TOPOLOGY m_Primitive;
public:
    bool CreateVertexBuffer() override;
    bool CreateVertexData() override;
    bool CreateIndexBuffer() override;
    bool CreateIndexData() override;
    bool CreateVertexLayout() override;
    bool PreRender(ID3D11DeviceContext* pContext) override;
    bool Render(ID3D11DeviceContext* pContext) override;
    bool PostRender(ID3D11DeviceContext* pContext) override;
public:
    bool DrawLine(ID3D11DeviceContext* pContext, TVector3 vStart, TVector3 vEnd, TVector4 vColor);
    
public:
    LineShape();
    virtual ~LineShape();
};

class BoxShape : public Object3D
{
public:
    D3D_PRIMITIVE_TOPOLOGY m_BoxPrimitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
public:
    bool CreateVertexData() override;
    bool CreateIndexData() override;
    bool Render(ID3D11DeviceContext* pContext) override;
    bool PostRender(ID3D11DeviceContext* pContext) override;
};
class PlaneShape : public Object3D
{
public:
    D3D_PRIMITIVE_TOPOLOGY m_PlanePrimitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
public:
    bool CreateVertexData() override;
    bool CreateIndexData() override;
    bool Render(ID3D11DeviceContext* pContext) override;
};


