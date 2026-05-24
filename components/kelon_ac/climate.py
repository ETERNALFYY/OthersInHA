import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.const import CONF_ID, CONF_CURRENT_TEMPERATURE_SENSOR

kelon_ns = cg.esphome_ns.namespace("kelon_ac")
KelonAC = kelon_ns.class_("KelonAC", climate.Climate, cg.Component)

CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(KelonAC),
    cv.Required(CONF_CURRENT_TEMPERATURE_SENSOR): cv.use_id(cg.Sensor),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await climate.register_climate(var, config)
    await cg.register_component(var, config)

    sensor = await cg.get_variable(config[CONF_CURRENT_TEMPERATURE_SENSOR])
    cg.add(var.set_current_temperature_sensor(sensor))
