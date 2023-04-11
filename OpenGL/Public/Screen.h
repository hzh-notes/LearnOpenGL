#pragma once

#ifndef __SCREEN_H
#define __SCREEN_H

class Screen
{
public:

	Screen();
	~Screen();

	void Render(unsigned int ColorBuffer);

private:

	int ShaderId = -1;
};

#endif


