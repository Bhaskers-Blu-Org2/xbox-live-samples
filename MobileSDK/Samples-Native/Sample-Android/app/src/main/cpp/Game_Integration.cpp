// Copyright (c) Microsoft Corporation
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <pch.h>
#include <Game_Integration_JNI.h>
#include "Game_Integration.h"
#include <XSAPI_Integration.h>
#include <Adv_Identity_Integration.h>

static Game_Integration* g_gInt = nullptr;

Game_Integration* Game_Integration::getInstance()
{
    if(!g_gInt)
    {
        g_gInt = new (std::nothrow) Game_Integration();
    }

    return g_gInt;
}

void Game_Integration::Init()
{
    // Set Debugger Trace Level for XSAPI
    HCSettingsSetTraceLevel(HCTraceLevel::Verbose);
    HCTraceSetTraceToDebugger(true);

    HRESULT hr = XalInit();
    ASSERT_MESSAGE(SUCCEEDED(hr), "Failed to Init XAL!");
    SampleLog(LL_DEBUG, "XAL Init successful!");

    hr = XblInit();
    ASSERT_MESSAGE(SUCCEEDED(hr), "Failed to Init XboxLive!");
    SampleLog(LL_DEBUG, "Xbl Init successful!");
}

void Game_Integration::Cleanup()
{
    auto asyncBlock = std::make_unique<XAsyncBlock>();
    asyncBlock->queue = nullptr;

    HRESULT  hr = XblCleanupAsync(asyncBlock.get());
    if(SUCCEEDED(hr))
    {
        // Synchronously wait for cleanup to complete
        hr = XAsyncGetStatus(asyncBlock.get(), true);
    }
}

void Game_Integration::setXalUser(XalUserHandle xalUser)
{
    std::lock_guard<std::mutex> lock(m_xblContextMutex);
    if (m_xalUser)
    {
        XalUserCloseHandle(m_xalUser);
    }
    m_xalUser = xalUser;

}
void Game_Integration::setXblContext(XblContextHandle xblContext)
{
    std::lock_guard<std::mutex> lock(m_xblContextMutex);
    m_xblContext = xblContext;
}

XblContextHandle Game_Integration::getXblContext()
{
    std::lock_guard<std::mutex> lock(m_xblContextMutex);
    return m_xblContext;
}

bool Game_Integration::hasXblContext()
{
    return m_xblContext != nullptr;
}

XalUserHandle Game_Integration::getCurrentUser()
{
    return m_xalUser;
}

HRESULT Game_Integration::XblInit()
{
    XblInitArgs args = { };
    args.scid               = "ad560100-4bff-49c2-a4d6-44c431374b88";
    args.javaVM             = Game_Integration_GetJavaVM();
    args.applicationContext = Game_Integration_GetAppActivityIntance();

    return XblInitialize(&args);
}

HRESULT Game_Integration::XalInit()
{
    std::string clientId = "000000004824156c";
    std::string redirUri = "ms-xal-" + clientId + "://auth";

    XalInitArgs xalInitArgs = {};
    xalInitArgs.clientId     = clientId.c_str();
    xalInitArgs.titleId      = 825707400;
    xalInitArgs.sandbox      = "XDKS.1";
    xalInitArgs.redirectUri = redirUri.c_str();
    xalInitArgs.javaVM      = Game_Integration_GetJavaVM();
    xalInitArgs.appContext  = Game_Integration_GetAppActivityIntance();

    Adv_Identity_Init(nullptr, nullptr, Game_Integration_GetPath().c_str());

    return XalInitialize(&xalInitArgs, nullptr);
}