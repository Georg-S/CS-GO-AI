#include "Button.h"

Button::Button(const std::string& button_name, int button)
{
	this->button_name = button_name;
	this->button = button;
}

void Button::update()
{
	int key_state = GetAsyncKeyState(button);

	if (is_key_down(key_state)) 
	{
		if (!was_previous_update_down) 
		{
			new_click = true;
			toggled = !toggled;
		}
		else 
		{
			new_click = false;
		}

		was_previous_update_down = true;
	}
	else 
	{
		was_previous_update_down = false;
		new_click = false;
	}
}

bool Button::is_key_down(unsigned short key_state) const
{
	static constexpr unsigned short most_significant_bit = 0x8000;

	return key_state & most_significant_bit;
}

void Button::set_toggle_button(int button)
{
	this->button = button;
	update();
}

bool Button::was_clicked() const
{
	return new_click;
}

bool Button::is_toggled() const
{
	return toggled;
}
