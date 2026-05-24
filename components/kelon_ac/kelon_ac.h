#pragma once
#include "esphome/components/climate/climate.h"
#include "esphome/core/component.h"

namespace esphome {
namespace kelon_ac {

class KelonAC : public climate::Climate, public Component {
 public:
  void setup() override {}
  void loop() override {}

  climate::ClimateTraits traits() override {
    auto traits = climate::ClimateTraits();
    traits.set_supports_current_temperature(true);
    traits.set_supported_modes({
      climate::CLIMATE_MODE_OFF,
      climate::CLIMATE_MODE_COOL,
      climate::CLIMATE_MODE_HEAT,
      climate::CLIMATE_MODE_AUTO,
    });
    traits.set_supported_fan_modes({
      climate::CLIMATE_FAN_LOW,
      climate::CLIMATE_FAN_MEDIUM,
      climate::CLIMATE_FAN_HIGH,
      climate::CLIMATE_FAN_AUTO,
    });
    return traits;
  }

 protected:
  void control(const climate::ClimateCall &call) override {
    if (call.get_mode().has_value())
      this->mode = *call.get_mode();

    if (call.get_target_temperature().has_value())
      this->target_temperature = *call.get_target_temperature();

    if (call.get_fan_mode().has_value())
      this->fan_mode = *call.get_fan_mode();

    // TODO: 在这里发送 IR 指令
    // send_kelon_ir(this->mode, this->target_temperature, this->fan_mode);

    this->publish_state();
  }
};

}  // namespace kelon_ac
}  // namespace esphome
