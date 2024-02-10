#pragma once
#include "ME_EditorWindow.h"

namespace mintye
{
	class AssetsWindow
		: public EditorWindow
	{
	private:

	public:
		AssetsWindow(Application& application);

		virtual void draw() override;

		virtual void reset() override;
	};
}