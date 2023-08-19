#pragma once

#include "../native/types.h"
#include "../RDREnums.h"


enum PromptMode
{
	Standard,
	Hold,
	SemiHold
};

class UIPrompt
{
	public:
		int handle;

	private: 
		bool isEnabled;
		bool isVisible;
		const char* text;
		hud::EInputType control;
		Entity targetEntity;
		PromptMode mode;
		bool semiHoldShouldReturn;

	public: 
		UIPrompt(const char* text, hud::EInputType control);
		UIPrompt(const char* text, hud::EInputType control, PromptMode mode);

		Entity getTargetEntity();
		bool getIsEnabled();
		bool getIsVisible();

		void setText(const char* text);
		void setControl(hud::EInputType control);
		void setTargetEntity(Entity entity);
		void setPriority(int priority);
		void setMode(PromptMode mode);

		bool isActivatedByPlayer();
		void remove();
		void show();
		void hide();
		void disable();
		void enable();

	private:
		void update();
};