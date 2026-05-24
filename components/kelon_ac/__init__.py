import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate

kelon_ac_ns = cg.esphome_ns.namespace("kelon_ac")
KelonAC = kelon_ac_ns.class_("KelonAC", climate.Climate, cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(KelonAC),
})

async def to_code(config):
    var = cg.new_Pvariable(config[KelonAC])
    await climate.register_climate(var, config)
    await cg.register_component(var, config)
