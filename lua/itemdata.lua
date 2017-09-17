
return {
    {
        texture = loadTexture("brick.png"),
        cooldown = 1.0,
        baseDamage = 5,
        abiCost = 10,
        onUse = function()
        end,
    }, {
        texture = loadTexture("water.png"),
        cooldown = .3,
        baseDamage = 5,
        abiCost = 0,
        onUse = function()
        end,
    }
}
