#pragma once
#include "Client.h"
#include "wx/wx.h"

class ClientGUI : public wxFrame
{
public:
	ClientGUI();
	~ClientGUI();

private:
	Client* client = nullptr;

private:
	wxTextCtrl* ipBox = nullptr;
	wxButton* connectButton = nullptr;
	wxStaticText* connectedText = nullptr;
	wxStaticText* instructions = nullptr;
	void OnButtonClicked(wxCommandEvent &event);
	
	wxDECLARE_EVENT_TABLE();
};
