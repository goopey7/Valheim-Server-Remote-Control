#pragma once
#include "wx/wx.h"
#include "ClientGUI.h"
class ClientApp : public wxApp
{
public:
	ClientApp();
	~ClientApp();
	virtual bool OnInit();
private:
	ClientGUI* gui = nullptr;
};

