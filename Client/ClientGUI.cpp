#include "ClientGUI.h"

wxBEGIN_EVENT_TABLE(ClientGUI, wxFrame)
	EVT_BUTTON(16, OnButtonClicked)
wxEND_EVENT_TABLE()

ClientGUI::ClientGUI() : wxFrame(nullptr, wxID_ANY, "Valheim Server Remote Controller - Sam Collier",wxDefaultPosition,wxSize(450,250),wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
	instructions = new wxStaticText(this, wxID_ANY, "Enter Server IP", wxPoint(10, 10), wxSize(90, 20));
	ipBox = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 30), wxSize(300, 20));
	connectButton = new wxButton(this, 16, "Connect", wxPoint(10, 70), wxSize(150, 50));
	connectedText = new wxStaticText(this, wxID_ANY, "Not Connected", wxPoint(10, 140), wxSize(87, 20));
	samCollier = new wxStaticText(this, wxID_ANY, "Sam Collier", wxPoint(215, 180), wxSize(87, 20));
	wxFont author(24,wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	samCollier->SetFont(author);
}

ClientGUI::~ClientGUI()
{
	delete client;
}

void ClientGUI::OnButtonClicked(wxCommandEvent &event)
{
	if(client==nullptr)
	{
		client = new Client(ipBox->GetValue());
		if(client->IsConnected())
		{
			connectButton->SetLabel("Disconnect");
			connectedText->SetLabel("Connected");
		}
	}
	else
	{
		connectButton->SetLabel("Connect");
		connectedText->SetLabel("Not Connected");
		delete client;
		client = nullptr;
	}
	event.Skip();
}
