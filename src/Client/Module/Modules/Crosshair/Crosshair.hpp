#pragma once

#include "CrosshairListener.hpp"
#include "../Module.hpp"
#include "../../../Events/EventHandler.hpp"

class Crosshair : public Module {
public:
    CrosshairListener* listener = nullptr;

    Crosshair() : Module("Crosshair", "Allows you to change crosshair behavior.", IDR_ITEM_PHYSICS_PNG, "") {
        Module::setup();
    }

    void onEnable() override {
        listener = new CrosshairListener("Crosshair", this);

        EventHandler::registerListener(listener);

        listener->onEnable();

        Module::onEnable();
    }

    void onDisable() override {
        if (listener != nullptr)
            listener->onDisable();

        EventHandler::unregisterListener("Crosshair");

        delete listener;

        Module::onDisable();
    }

    void defaultConfig() override {
        if (settings.getSettingByName<bool>("thirdpersoncrosshair") == nullptr)
            settings.addSetting("thirdpersoncrosshair", true);
    }

    void settingsRender() override {

        float x = Constraints::PercentageConstraint(0.019, "left");
        float y = Constraints::PercentageConstraint(0.10, "top");

        const float scrollviewWidth = Constraints::RelativeConstraint(0.12, "height", true);


        FlarialGUI::ScrollBar(x, y, 140, Constraints::SpacingConstraint(5.5, scrollviewWidth), 2);
        FlarialGUI::SetScrollView(x, Constraints::PercentageConstraint(0.00, "top"),
                                  Constraints::RelativeConstraint(1.0, "width"),
                                  Constraints::RelativeConstraint(0.88f, "height"));

        this->addHeader("Misc");
        this->addToggle("Third Person Crosshair", "",  this->settings.getSettingByName<bool>("thirdpersoncrosshair")->value);

        FlarialGUI::UnsetScrollView();

        this->resetPadding();
    }
};