#pragma once
#include "esphome.h"

class KelonAC : public esphome::climate::Climate, public esphome::Component {
 public:

  void setup() override {
    this->target_temperature = 26;
    this->mode = climate::CLIMATE_MODE_OFF;
    this->fan_mode = climate::CLIMATE_FAN_AUTO;
    this->swing_mode = climate::CLIMATE_SWING_OFF;
  }

  void set_current_temperature_sensor(esphome::sensor::Sensor *sensor) {
    this->current_temperature_sensor_ = sensor;
  }

  void loop() override {
    if (this->current_temperature_sensor_) {
      this->current_temperature = this->current_temperature_sensor_->state;
    }
  }

  void control(const climate::ClimateCall &call) override {

    if (call.get_mode().has_value())
      this->mode = *call.get_mode();

    if (call.get_target_temperature().has_value())
      this->target_temperature = *call.get_target_temperature();

    if (call.get_fan_mode().has_value())
      this->fan_mode = *call.get_fan_mode();

    if (call.get_swing_mode().has_value())
      this->swing_mode = *call.get_swing_mode();

    send_state_packet();
    this->publish_state();
  }

  climate::ClimateTraits traits() override {
    auto traits = climate::ClimateTraits();
    traits.set_supported_modes({
      climate::CLIMATE_MODE_OFF,
      climate::CLIMATE_MODE_COOL,
      climate::CLIMATE_MODE_HEAT
    });
    traits.set_supported_fan_modes({
      climate::CLIMATE_FAN_AUTO,
      climate::CLIMATE_FAN_LOW,
      climate::CLIMATE_FAN_HIGH
    });
    traits.set_supported_swing_modes({
      climate::CLIMATE_SWING_OFF,
      climate::CLIMATE_SWING_VERTICAL
    });
    traits.set_supports_current_temperature(true);
    traits.set_visual_min_temperature(16);
    traits.set_visual_max_temperature(30);
    traits.set_visual_temperature_step(1.0f);
    return traits;
  }

  void send_state_packet() {

    uint8_t temp_byte = 0x18 + (int(this->target_temperature) - 16);

    uint8_t mode_byte = 0x00;

    if (this->mode == climate::CLIMATE_MODE_COOL) mode_byte = 0x08;
    if (this->mode == climate::CLIMATE_MODE_HEAT) mode_byte = 0x0B;

    if (this->fan_mode == climate::CLIMATE_FAN_AUTO) mode_byte |= 0x80;
    if (this->fan_mode == climate::CLIMATE_FAN_LOW)  mode_byte |= 0xB0;
    if (this->fan_mode == climate::CLIMATE_FAN_HIGH) mode_byte |= 0x90;

    if (this->swing_mode == climate::CLIMATE_SWING_VERTICAL)
      mode_byte = 0x11;

    uint64_t code =
      ((uint64_t)0xC0 << 40) |
      ((uint64_t)0xE0 << 32) |
      ((uint64_t)0x30 << 24) |
      ((uint64_t)0xB1 << 16) |
      ((uint64_t)temp_byte << 8) |
      (uint64_t)mode_byte;

    auto *tx = esphome::remote_base::RemoteTransmitAction::get_transmitter();
    tx->transmit_nec(code >> 32, code & 0xFFFFFFFF);
  }

 protected:
  esphome::sensor::Sensor *current_temperature_sensor_{nullptr};
};
