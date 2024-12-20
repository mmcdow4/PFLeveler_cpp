#pragma once

#ifndef HARDBREAKWRAPTEXT_H
#define HARDBREAKWRAPTEXT_H

#include <wx/window.h>
#include <wx/textwrapper.h>
#include <wx/stattext.h>


class HardBreakWrapper : public wxTextWrapper
{
public:
  HardBreakWrapper(wxWindow* win, const wxString& text, int widthMax) : m_win(win), m_widthMax(widthMax)
  {
    Wrap(m_win, text, m_widthMax);
  }

  wxString const& GetWrapped() const { return m_wrapped; }

  wxString UpdateText(const wxString& text)
  {
    m_wrapped.clear();
    m_text.clear();
    m_text = text;
    Wrap(m_win, text, m_widthMax);
    return GetWrapped();
  }

  wxString UpdateWidth(int widthMax)
  {
    m_widthMax = widthMax;
    wxString textCopy = m_text;
    return(UpdateText(textCopy));
  }

protected:
  virtual void OnOutputLine(const wxString& line)
  {
    m_wrapped += line;
  }
  virtual void OnNewLine()
  {
    m_wrapped += '\n';
  }
private:
  wxWindow* m_win;
  wxString m_text;
  wxString m_wrapped;
  int m_widthMax;
};

//wxString WrapText(wxWindow* win, const wxString& text, int widthMax)
//{
//  HardBreakWrapper wrapper(win, text, widthMax);
//  return wrapper.GetWrapped();
//}

#endif