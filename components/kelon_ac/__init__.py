import esphome.codegen as cg
import esphome.config_validation as cv

kelon_ac_ns = cg.esphome_ns.namespace("kelon_ac")
KelonAC = kelon_ac_ns.class_("KelonAC", cg.Component)

CONFIG_SCHEMA = cv.Schema({})

async def to_code(config):
    cg.new_Pvariable(config[KelonAC])

