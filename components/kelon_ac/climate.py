import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID

kelon_ns = cg.esphome_ns.namespace("kelon_ac")
KelonAC = kelon_ns.class_("KelonAC", climate.Climate, cg.Component)

# ESPhome 2026: CLIMATE_SCHEMA 已被移除，必须使用 CLIMATE_SCHEMA_BASE
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(KelonAC),
    cv.Optional("current_temperature_sensor"): cv.use_id(cg.Sensor),
}).extend(climate.CLIMATE_SCHEMA_BASE)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await climate.register_climate(var, config)
    await cg.register_component(var, config)

    if "current_temperature_sensor" in config:
        sensor = await cg.get_variable(config["current_temperature_sensor"])
        cg.add(var.set_current_temperature_sensor(sensor))
