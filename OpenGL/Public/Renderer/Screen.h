#pragma once

#ifndef __SCREEN_H
#define __SCREEN_H

class Screen
{
public:

	Screen();
	~Screen();

	void Render();

private:
	int ShaderId = -1;
};

#endif


