#include "CS2/Button.h"

Button::Button(const std::string& button_name, int button)
{
	m_button_name = button_name;
	m_button = button;
}

void Button::update()
{
	int key_state = GetAsyncKeyState(m_button);

	if (is_key_down(key_state)) 
	{
		if (!m_was_previous_update_down) 
		{
			m_new_click = true;
			m_toggled = !m_toggled;
		}
		else 
		{
			m_new_click = false;
		}

		m_was_previous_update_down = true;
	}
	else 
	{
		m_was_previous_update_down = false;
		m_new_click = false;
	}
}

bool Button::is_key_down(unsigned short key_state) const
{
	static constexpr unsigned short most_significant_bit = 0x8000;

	return key_state & most_significant_bit;
}

void Button::set_toggle_button(int button)
{
	m_button = button;
	update();
}

bool Button::was_clicked() const
{
	return m_new_click;
}

bool Button::is_toggled() const
{
	return m_toggled;
}
