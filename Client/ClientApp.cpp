#include "ClientApp.h"

wxIMPLEMENT_APP(ClientApp);

ClientApp::ClientApp()
{
}

ClientApp::~ClientApp()
{
}

bool ClientApp::OnInit()
{
    gui = new ClientGUI();
    gui->Show();
    return true;
}
