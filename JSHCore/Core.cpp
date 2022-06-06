#include "Core.h"
void Core::FrameCamera()
{
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.x == g_Input.m_pDragDown.x)
    {
        g_Input.m_pDrag.x = 0;
    }
    if (g_Input.m_bDrag && g_Input.m_ptBeforePos.y == g_Input.m_pDragDown.y)
    {
        g_Input.m_pDrag.y = 0;
    }
    float fYaw = g_fSecPerFrame * g_Input.m_pDrag.x * 5.0f;
    float fPitch = g_fSecPerFrame * g_Input.m_pDrag.y * 5.0f;
    m_Camera.DebugUpdate(TVector4(fPitch, fYaw, 0.0f, 0.0f));
    m_Camera.Frame();
}
LRESULT Core::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return g_Input.MsgProc(hWnd, message, wParam, lParam);
}
bool Core::GameRun()
{
    if (!GameFrame()) return false;
    if (!GameRender()) return false;
    return true;
}
bool Core::GameInit()
{
    Device::SetDevice();
    DxState::Init();
   
    m_Timer.Init();
    g_Input.Init();
    m_Write.Init();

    m_Camera.Init();
    m_Camera.CreateViewMatrix(TVector3(0, 0, -5), TVector3(0, 0, 100000));
    m_Camera.CreateProjMatrix(1.0f, 3000.0f, TBASIS_PI * 0.5f, (float)g_rtClient.right / (float)g_rtClient.bottom);

    IDXGISurface1* m_pBackBuffer;
    m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),(void**)&m_pBackBuffer);
    m_Write.CreateDeviceResources(m_pBackBuffer);
    if (m_pBackBuffer)m_pBackBuffer->Release();

    Init();
    return true;
}
bool Core::GameFrame() 
{
    m_Timer.Frame();
    g_Input.Frame();
    m_Write.Frame();
    FrameCamera();
    
    
    
    if (g_Input.GetKey('1') == KEY_PUSH)
    {
        m_bDebugText = !m_bDebugText;
    }

    /*if (g_Input.GetKey('3') == KEY_PUSH)
    {
        m_bWireFrame = !m_bWireFrame;
    }*/


    if (g_Input.GetKey('3') == KEY_PUSH)
    {
        ApplyRS(m_pImmediateContext, DxState::g_pRSWireFrame);
    }
    if(g_Input.GetKey('4') == KEY_PUSH)
    {
        ApplyRS(m_pImmediateContext, DxState::g_pRSSolid);
    }

    Frame();
    g_Input.m_ptBeforePos = g_Input.m_ptPos;
    return true;
}
bool Core::GameRender() 
{
    PreRender();   
        m_Timer.Render();
        g_Input.Render();
        m_Write.Render();
        
        Render();
        
        if (m_bDebugText)
        {
            RECT  rt = { 0, 0, 800, 600 };
            m_Write.DrawText(rt, m_Timer.m_szTimerString, D2D1::ColorF(1, 1, 1, 1));
        }

        
    PostRender(); 
    
    return true;
}
bool Core::GameRelease() 
{
    Release();
    DxState::Release();
    m_Timer.Release();
    g_Input.Release();
    m_Write.Release();
    m_Camera.Release();
    CleanupDevice();
    return true;
}

bool Core::Init()
{
    return true;
}
bool Core::Frame() 
{
    return true;
}
bool Core::PreRender()
{
    
    float ClearColor[4] = { 0.0f, 0.2f, 0.5f, 1.0f };
    m_pImmediateContext->ClearRenderTargetView(m_DefaultRT.m_pRenderTargetView, ClearColor);
    m_pImmediateContext->ClearDepthStencilView(m_DefaultDS.m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_pImmediateContext->OMSetRenderTargets(1, &m_DefaultRT.m_pRenderTargetView, m_DefaultDS.m_pDepthStencilView);
    //m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    m_pImmediateContext->PSSetSamplers(0, 1, &DxState::g_pWrapSS);
    //ApplySS(m_pImmediateContext, DxState::g_pWrapSS, 0);
    m_pImmediateContext->OMSetDepthStencilState(DxState::g_pLessEqualDSS, 0x01);
    //ApplyDSS(m_pImmediateContext, DxState::g_pLessEqualDSS);
    
    
    
    return true;
}
bool Core::Render() 
{
    return true;
}
bool Core::PostRender() 
{
    assert(m_pSwapChain);
    m_pSwapChain->Present(0, 0);
    return true;
}
bool Core::Release() 
{
    return true;
}