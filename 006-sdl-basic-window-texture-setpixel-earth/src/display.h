#pragma once

/**
 * Display is called display but it is just the wrap to hidden SDL implementation and to access SDL with small calls.
 */

class Display {
  public:
	Display(int x) : value(x) {}

	int get() const
	{
		return value;
	}

  private:
	int value;
};
