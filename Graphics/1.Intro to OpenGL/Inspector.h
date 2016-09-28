#ifndef _INSPECTOR_H_
#define _INSPECTOR_H_
#pragma once

#include "GameObject.h"

namespace Editor
{
	class Inspector
	{
	public:

		static int init();

		static void drawGui();

		static Object * selected();

		static int quit();

	private:

		static Object *s_selected;

	};
}

#endif // _INSPECTOR_H_
