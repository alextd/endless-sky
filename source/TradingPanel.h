/* TradingPanel.h
Copyright (c) 2014 by Michael Zahniser

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "Panel.h"

class PlayerInfo;
class System;



// Overlay on the PlanetPanel showing commodity prices and inventory, and allowing
// buying and selling. This also lets you sell any plundered outfits you are
// carrying, although if you want to sell only certain ones and not others you
// will need to select them individually in the outfitter panel.
class TradingPanel : public Panel {
public:
	explicit TradingPanel(PlayerInfo &player);

	virtual void Step() override;
	virtual void Draw() override;


protected:
	// Only override the ones you need; the default action is to return false.
	virtual bool KeyDown(SDL_Keycode key, Uint16 mod, const Command &command, bool isNewPress) override;
	virtual bool Click(int x, int y, int clicks) override;


private:
	void Buy(int64_t amount);


private:
	PlayerInfo &player;
	const System &system;
	const int COMMODITY_COUNT;

	// Remember whether the "sell all" button will sell all outfits, or sell
	// everything except outfits.
	bool sellOutfits = false;
};
