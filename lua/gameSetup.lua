player = newEntity(0)
    :setControlled()
    :spawn()
    :setPos(700, 500)
    :setHealth(90)

enemies = {}
for i = 1, 100 do
    enemies[i] = newEntity(1)
        :grantAI(1) --AI_WANDER
        :setPos(300, 500)
        :spawn()
end
