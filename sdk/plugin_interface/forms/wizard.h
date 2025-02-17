///////////////////////////////////////////////////////////////////////////////
//
// wxFormBuilder - A Visual Dialog Editor for wxWidgets.
// Copyright (C) 2005 José Antonio Hurtado
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// Written by
//   Andrea Zanellato - zanellato.andrea@gmail.com
//   based on original wizard.h include in wxWidgets source code
//
///////////////////////////////////////////////////////////////////////////////
#ifndef __WXFB_WIZARD_H__
#define __WXFB_WIZARD_H__

#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/statline.h>
#include <wx/wizard.h>

class Wizard;
class WizardPageSimple;
class WizardEvent;

WX_DEFINE_ARRAY_PTR( WizardPageSimple*, WizardPages );

class WizardPageSimple : public wxPanel
{
public:
    WizardPageSimple( Wizard *parent );
    ~WizardPageSimple();
};

class Wizard : public wxPanel
{
public:
    Wizard( wxWindow *parent, wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL );
    ~Wizard();

    wxBoxSizer* GetPageSizer()  { return m_sizerPage; }
//  WizardPageSimple *GetCurrentPage() { return m_page; }

    // set/get bitmap
//  const wxBitmap& GetBitmap() const { return m_bitmap; }
    void SetBitmap( const wxBitmap& bitmap );

    void AddPage( WizardPageSimple* page );
    WizardPageSimple *GetPage( size_t index ) { return m_pages.Item( index ); }
    size_t GetPageCount() { return m_pages.GetCount(); }
    size_t GetPageIndex( WizardPageSimple *wizpage ) { return m_pages.Index( wizpage ); }
    void SetSelection( size_t pageIndex );
    void ShowHelpButton( bool showhelp ) { m_btnHelp->Show( showhelp ); }

private:
    void OnBackOrNext( wxCommandEvent& event );
    void OnHelp( wxCommandEvent& event );
    void OnCancel( wxCommandEvent& event );
    void OnWizEvent( WizardEvent& event );

    wxBoxSizer       *m_sizerBmpAndPage; // Page area sizer will be inserted here with padding
    wxBoxSizer       *m_sizerPage;       // Actual position and size of pages
    wxStaticBitmap   *m_statbmp;         // the control for the bitmap
    wxButton         *m_btnHelp;
    wxButton         *m_btnPrev;         // the "<Back" button
    wxButton         *m_btnNext;         // the "Next>" or "Finish" button
    wxButton         *m_btnCancel;
    WizardPageSimple *m_page;
    wxBitmap          m_bitmap;          // the default bitmap to show
    WizardPages       m_pages;

//  DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// WizardEvent class represents an event generated by the wizard: this event
// is first sent to the page itself and, if not processed there, goes up the
// window hierarchy as usual
// ----------------------------------------------------------------------------

class WizardEvent : public wxNotifyEvent
{
public:
	explicit WizardEvent(wxEventType type = wxEVT_NULL, int id = wxID_ANY, bool direction = true,
	                     WizardPageSimple* page = nullptr);

    // for EVT_WXFB_WIZARD_PAGE_CHANGING, return true if we're going forward or
    // false otherwise and for EVT_WXFB_WIZARD_PAGE_CHANGED return true if we came
    // from the previous page and false if we returned from the next one
    // (this function doesn't make sense for CANCEL events)
    bool GetDirection() const { return m_direction; }

    WizardPageSimple *GetPage() const { return m_page; }

	wxEvent* Clone() const override {
		return new WizardEvent(*this);
	}

private:
    bool              m_direction;
    WizardPageSimple *m_page;
};

// ----------------------------------------------------------------------------
// macros for handling WizardEvents
// ----------------------------------------------------------------------------
#define wxFBDLLIMPEXP

wxDECLARE_EXPORTED_EVENT(wxFBDLLIMPEXP, wxFB_EVT_WIZARD_PAGE_CHANGED, WizardEvent);
wxDECLARE_EXPORTED_EVENT(wxFBDLLIMPEXP, wxFB_EVT_WIZARD_PAGE_CHANGING, WizardEvent);
wxDECLARE_EXPORTED_EVENT(wxFBDLLIMPEXP, wxFB_EVT_WIZARD_CANCEL, WizardEvent);
wxDECLARE_EXPORTED_EVENT(wxFBDLLIMPEXP, wxFB_EVT_WIZARD_HELP, WizardEvent);
wxDECLARE_EXPORTED_EVENT(wxFBDLLIMPEXP, wxFB_EVT_WIZARD_FINISHED, WizardEvent);
#if wxABI_VERSION >= 20811
wxDECLARE_EXPORTED_EVENT(wxFBDLLIMPEXP, wxFB_EVT_WIZARD_PAGE_SHOWN, WizardEvent);
#endif

using WizardEventFunction = void (wxEvtHandler::*)(WizardEvent&);

#define WizardEventHandler(func) wxEVENT_HANDLER_CAST(WizardEventFunction, func)

#define wxFB__DECLARE_WIZARDEVT( evt, id, fn ) \
    wx__DECLARE_EVT1( wxFB_EVT_WIZARD_ ## evt, id, WizardEventHandler( fn ) )

// notifies that the page has just been changed (can't be vetoed)
#define EVT_WXFB_WIZARD_PAGE_CHANGED( id, fn ) wxFB__DECLARE_WIZARDEVT( PAGE_CHANGED, id, fn )

// the user pressed "<Back" or "Next>" button and the page is going to be
// changed - unless the event handler vetoes the event
#define EVT_WXFB_WIZARD_PAGE_CHANGING( id, fn ) wxFB__DECLARE_WIZARDEVT( PAGE_CHANGING, id, fn )

// the user pressed "Cancel" button and the wizard is going to be dismissed -
// unless the event handler vetoes the event
#define EVT_WXFB_WIZARD_CANCEL(id, fn) wxFB__DECLARE_WIZARDEVT(CANCEL, id, fn)

// the user pressed "Finish" button and the wizard is going to be dismissed -
#define EVT_WXFB_WIZARD_FINISHED( id, fn ) wxFB__DECLARE_WIZARDEVT( FINISHED, id, fn )

// the user pressed "Help" button
#define EVT_WXFB_WIZARD_HELP(id, fn) wxFB__DECLARE_WIZARDEVT( HELP, id, fn )

// the page was just shown and laid out
#define EVT_WXFB_WIZARD_PAGE_SHOWN( id, fn ) wxFB__DECLARE_WIZARDEVT( PAGE_SHOWN, id, fn )

#endif //__WXFB_WIZARD_H__
